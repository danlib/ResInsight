/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018     Statoil ASA
//
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
//
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#include "RiuMohrsCirclePlot.h"

#include "RiaColorTables.h"

#include "RiuSelectionManager.h"
#include "RiuSummaryQwtPlot.h"

#include "RigFemPart.h"
#include "RigFemPartCollection.h"
#include "RigFemPartGrid.h"
#include "RigFemPartResultsCollection.h"
#include "RigFemResultPosEnum.h"
#include "RigGeoMechCaseData.h"

#include "RimGeoMechCase.h"
#include "RimGeoMechCellColors.h"
#include "RimGeoMechResultDefinition.h"
#include "RimGeoMechView.h"

#include "cvfAssert.h"

#include <QPainterPath>
#include <QWidget>

#include "qwt_legend.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_layout.h"
#include "qwt_plot_rescaler.h"
#include "qwt_plot_shapeitem.h"

#include <cmath>

//==================================================================================================
///
/// \class RiuMohrsCirclePlot
///
///
///
//==================================================================================================

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RiuMohrsCirclePlot::RiuMohrsCirclePlot(QWidget* parent)
    : QwtPlot(parent), m_sourceGeoMechViewOfLastPlot(nullptr)
{
    RiuSummaryQwtPlot::setCommonPlotBehaviour(this);

    m_rescaler = new QwtPlotRescaler(this->canvas());
    m_rescaler->setReferenceAxis(QwtPlot::yLeft);
    m_rescaler->setAspectRatio(QwtPlot::xBottom, 1.0);
    m_rescaler->setRescalePolicy(QwtPlotRescaler::Fixed);
    m_rescaler->setEnabled(true);

    enableAxis(QwtPlot::xBottom, true);
    enableAxis(QwtPlot::yLeft, true);
    enableAxis(QwtPlot::xTop, false);
    enableAxis(QwtPlot::yRight, false);

    setAxisTitle(QwtPlot::xBottom, "Effective Normal Stress");
    setAxisTitle(QwtPlot::yLeft, "Shear Stress");

    // The legend will be deleted in the destructor of the plot or when
    // another legend is inserted.
    QwtLegend* legend = new QwtLegend(this);
    this->insertLegend(legend, BottomLegend);

    // this->setTitle(QString("SE"));
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RiuMohrsCirclePlot::~RiuMohrsCirclePlot()
{
    deletePlotItems();

    if (m_rescaler)
    {
        delete m_rescaler;
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RiuMohrsCirclePlot::updateOnSelectionChanged(const RiuSelectionItem* selectionItem)
{
    const RiuGeoMechSelectionItem* geoMechSelectionItem = dynamic_cast<const RiuGeoMechSelectionItem*>(selectionItem);
    
    m_sourceGeoMechViewOfLastPlot = nullptr;
    
    if (!geoMechSelectionItem)
    {
        this->clearPlot();
        return;
    }

    RimGeoMechView* geoMechView = geoMechSelectionItem->m_view;
    CVF_ASSERT(geoMechView);

    if (this->isVisible())
    {
        const size_t       gridIndex = geoMechSelectionItem->m_gridIndex;
        const size_t       cellIndex = geoMechSelectionItem->m_cellIndex;
        const cvf::Color3f color     = geoMechSelectionItem->m_color;

        queryDataAndUpdatePlot(geoMechView, gridIndex, cellIndex, cvf::Color3ub(color));
        
        m_sourceGeoMechViewOfLastPlot = geoMechView;
    }
    else
    {
        this->clearPlot();
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RiuMohrsCirclePlot::clearPlot()
{
    deletePlotItems();

    this->replot();
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RiuMohrsCirclePlot::updateOnTimeStepChanged(Rim3dView* changedView)
{
    if (!this->isVisible())
    {
        return;
    }

    // Don't update the plot if the view that changed time step is different from the view that was the source of the current plot
    RimGeoMechView* geoMechView = dynamic_cast<RimGeoMechView*>(changedView);
    if (!geoMechView || geoMechView != m_sourceGeoMechViewOfLastPlot)
    {
        return;
    }

    // Fetch the current global selection and only continue if the selection's view matches the view with time step change
    const RiuGeoMechSelectionItem* geoMechSelectionItem = dynamic_cast<const RiuGeoMechSelectionItem*>(RiuSelectionManager::instance()->selectedItem());
    if (geoMechSelectionItem && geoMechSelectionItem->m_view == geoMechView)
    {
        const size_t gridIndex = geoMechSelectionItem->m_gridIndex;
        const size_t gridCellIndex = geoMechSelectionItem->m_cellIndex;
        const cvf::Color3f color = geoMechSelectionItem->m_color;

        deletePlotItems();

        queryDataAndUpdatePlot(geoMechView, gridIndex, gridCellIndex, cvf::Color3ub(color));
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QSize RiuMohrsCirclePlot::sizeHint() const
{
    return QSize(100, 100);
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QSize RiuMohrsCirclePlot::minimumSizeHint() const
{
    return QSize(0, 0);
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RiuMohrsCirclePlot::addMohrCircles(const MohrsCirclesInfo& mohrsCirclesInfo)
{
    const cvf::Vec3f& principals = mohrsCirclesInfo.principals;

    std::array<std::pair<double /*radius*/, double /*centerX*/>, 3> mohrsCircles;

    mohrsCircles[0].first  = (principals[0] - principals[2]) / 2.0;
    mohrsCircles[0].second = (principals[0] + principals[2]) / 2.0;

    mohrsCircles[1].first  = (principals[1] - principals[2]) / 2.0;
    mohrsCircles[1].second = (principals[1] + principals[2]) / 2.0;

    mohrsCircles[2].first  = (principals[0] - principals[1]) / 2.0;
    mohrsCircles[2].second = (principals[0] + principals[1]) / 2.0;

    for (size_t i = 0; i < 3; i++)
    {
        QwtPlotShapeItem* plotItem = new QwtPlotShapeItem("Circle");

        QPainterPath* circleDrawing = new QPainterPath();
        QPointF       center(mohrsCircles[i].second, 0);
        circleDrawing->addEllipse(center, mohrsCircles[i].first, mohrsCircles[i].first);

        plotItem->setPen(QColor(mohrsCirclesInfo.color.r(), mohrsCirclesInfo.color.g(), mohrsCirclesInfo.color.b()));
        plotItem->setShape(*circleDrawing);
        plotItem->setRenderHint(QwtPlotItem::RenderAntialiased, true);

        if (i == 0)
        {
            QString textBuilder;
            textBuilder.append(QString("<b>FOS</b>: %1, ").arg(QString::number(mohrsCirclesInfo.factorOfSafety, 'f', 2)));

            textBuilder.append(QString("<b>Element Id</b>: %1, <b>ijk</b>[%2, %3, %4],")
                                   .arg(mohrsCirclesInfo.elmIndex)
                                   .arg(mohrsCirclesInfo.i)
                                   .arg(mohrsCirclesInfo.j)
                                   .arg(mohrsCirclesInfo.k));

            textBuilder.append(QString("<b>&sigma;<sub>1</sub></b>: %1, ").arg(principals[0]));
            textBuilder.append(QString("<b>&sigma;<sub>2</sub></b>: %1, ").arg(principals[1]));
            textBuilder.append(QString("<b>&sigma;<sub>3</sub></b>: %1").arg(principals[2]));

            plotItem->setTitle(textBuilder);
            plotItem->setItemAttribute(QwtPlotItem::Legend);
        }

        plotItem->attach(this);

        m_circlePlotItems.push_back(plotItem);
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RiuMohrsCirclePlot::deleteCircles()
{
    for (size_t i = 0; i < m_circlePlotItems.size(); i++)
    {
        m_circlePlotItems[i]->detach();
        delete m_circlePlotItems[i];
    }

    m_circlePlotItems.clear();

    for (size_t i = 0; i < m_transparentCurves.size(); i++)
    {
        m_transparentCurves[i]->detach();
        delete m_transparentCurves[i];
    }

    m_transparentCurves.clear();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RiuMohrsCirclePlot::addEnvelope(const cvf::Vec3f& principals, RimGeoMechView* view)
{
    if (!view) return;

    double cohesion      = view->geoMechCase()->cohesion();
    double frictionAngle = view->geoMechCase()->frictionAngleDeg();

    if (cohesion == HUGE_VAL || frictionAngle == HUGE_VAL)
    {
        return;
    }

    double xVals[2];
    double yVals[2];

    double tanFrictionAngle = cvf::Math::abs(cvf::Math::tan(cvf::Math::toRadians(frictionAngle)));

    if (tanFrictionAngle == 0 || tanFrictionAngle == HUGE_VAL)
    {
        return;
    }

    yVals[0] = 0;

    double x = cohesion / tanFrictionAngle;
    if (principals[0] < 0)
    {
        xVals[0] = x;
        xVals[1] = principals[2];

        yVals[1] = (cohesion / x) * (x + principals[2]);
    }
    else
    {
        xVals[0] = -x;
        xVals[1] = principals[0];

        yVals[1] = (cohesion / x) * (x + principals[0]);
    }


    // If envelope for the view already exists, check if a "larger" envelope should be created
    if (m_envolopePlotItems.find(view) != m_envolopePlotItems.end())
    {
        if (yVals[1] <= m_envolopePlotItems[view]->maxYValue())
        {
            return;
        }
        else
        {
            m_envolopePlotItems[view]->detach();
            delete m_envolopePlotItems[view];
            m_envolopePlotItems.erase(view);
        }
    }

    QwtPlotCurve* qwtCurve = new QwtPlotCurve();

    qwtCurve->setSamples(xVals, yVals, 2);

    qwtCurve->setStyle(QwtPlotCurve::Lines);
    qwtCurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);

    const QPen curvePen(envelopeColor(view));
    qwtCurve->setPen(curvePen);

    qwtCurve->setTitle(QString("<b>Envelope for %1</b>, (<b>S<sub>0</sub></b>: %2, <b>&Phi;</b>: %3)")
                           .arg(view->geoMechCase()->caseUserDescription)
                           .arg(cohesion)
                           .arg(frictionAngle));

    qwtCurve->attach(this);

    m_envolopePlotItems[view] = qwtCurve;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RiuMohrsCirclePlot::deleteEnvelopes()
{
    for (const std::pair<RimGeoMechView*, QwtPlotCurve*>& envelope : m_envolopePlotItems)
    {
        envelope.second->detach();
        delete envelope.second;
    }

    m_envolopePlotItems.clear();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RiuMohrsCirclePlot::queryDataAndUpdatePlot(RimGeoMechView*      geoMechView,
                                                size_t               gridIndex,
                                                size_t               elmIndex,
                                                const cvf::Color3ub& color)
{
    CVF_ASSERT(geoMechView);
    m_sourceGeoMechViewOfLastPlot = geoMechView;

    RigFemPartResultsCollection* resultCollection = geoMechView->geoMechCase()->geoMechData()->femPartResults();

    int                 frameIdx = geoMechView->currentTimeStep();
    RigFemResultAddress address(RigFemResultPosEnum::RIG_ELEMENT_NODAL, "SE", "");

    // TODO: All tensors are calculated every time this function is called. FIX
    std::vector<caf::Ten3f> vertexTensors = resultCollection->tensors(address, 0, frameIdx);
    if (vertexTensors.empty())
    {
        return;
    }

    RigFemPart* femPart = geoMechView->geoMechCase()->geoMechData()->femParts()->part(gridIndex);

    // Calculate average tensor in element
    caf::Ten3f tensorSumOfElmNodes = vertexTensors[femPart->elementNodeResultIdx((int)elmIndex, 0)];
    for (int i = 1; i < 8; i++)
    {
        tensorSumOfElmNodes = tensorSumOfElmNodes + vertexTensors[femPart->elementNodeResultIdx((int)elmIndex, i)];
    }
    caf::Ten3f elmTensor = tensorSumOfElmNodes * (1.0 / 8.0);

    cvf::Vec3f dirs[3];
    cvf::Vec3f principals = elmTensor.calculatePrincipals(dirs);

    if (!isValidPrincipals(principals))
    {
        return;
    }

    double cohesion      = geoMechView->geoMechCase()->cohesion();
    double frictionAngleDeg = geoMechView->geoMechCase()->frictionAngleDeg();

    size_t i, j, k;
    femPart->structGrid()->ijkFromCellIndex(elmIndex, &i, &j, &k);

    MohrsCirclesInfo mohrsCircle;
    mohrsCircle.color          = color;
    mohrsCircle.elmIndex       = elmIndex;
    mohrsCircle.factorOfSafety = calculateFOS(principals, frictionAngleDeg, cohesion);
    mohrsCircle.principals     = principals;
    mohrsCircle.i              = i;
    mohrsCircle.j              = j;
    mohrsCircle.k              = k;

    addMohrsCirclesInfo(mohrsCircle, geoMechView);
    replotAndScaleAxis();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RiuMohrsCirclePlot::addMohrsCirclesInfo(const MohrsCirclesInfo& mohrsCircleInfo, RimGeoMechView* view)
{
    m_mohrsCiclesInfos.push_back(mohrsCircleInfo);

    addEnvelope(mohrsCircleInfo.principals, view);
    addMohrCircles(mohrsCircleInfo);
}

//--------------------------------------------------------------------------------------------------
/// Add a transparent curve to make tooltip available on principals crossing the x-axis
//--------------------------------------------------------------------------------------------------
void RiuMohrsCirclePlot::updateTransparentCurvesOnPrincipals()
{
    for (size_t i = 0; i < m_transparentCurves.size(); i++)
    {
        m_transparentCurves[i]->detach();
        delete m_transparentCurves[i];
    }

    m_transparentCurves.clear();

    for (const MohrsCirclesInfo& mohrCircleInfo : m_mohrsCiclesInfos)
    {
        QwtPlotCurve* transparentCurve = new QwtPlotCurve();

        QVector<QPointF> qVectorPoints;

        qVectorPoints.push_back(QPointF(mohrCircleInfo.principals[0], 0));
        qVectorPoints.push_back(QPointF(mohrCircleInfo.principals[1], 0));
        qVectorPoints.push_back(QPointF(mohrCircleInfo.principals[2], 0));

        transparentCurve->setSamples(qVectorPoints);
        transparentCurve->setYAxis(QwtPlot::yLeft);
        transparentCurve->setStyle(QwtPlotCurve::NoCurve);
        transparentCurve->setLegendAttribute(QwtPlotCurve::LegendNoAttribute);

        transparentCurve->attach(this);
        m_transparentCurves.push_back(transparentCurve);
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
double RiuMohrsCirclePlot::largestCircleRadiusInPlot() const
{
    double currentLargestDiameter = -HUGE_VAL;

    for (const MohrsCirclesInfo& mohrCircleInfo : m_mohrsCiclesInfos)
    {
        if (mohrCircleInfo.principals[0] > currentLargestDiameter)
        {
            currentLargestDiameter = mohrCircleInfo.principals[0] - mohrCircleInfo.principals[2];
        }
    }

    return currentLargestDiameter / 2;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
double RiuMohrsCirclePlot::smallestPrincipal() const
{
    double currentSmallestPrincipal = HUGE_VAL;

    for (const MohrsCirclesInfo& mohrCircleInfo : m_mohrsCiclesInfos)
    {
        if (mohrCircleInfo.principals[2] < currentSmallestPrincipal)
        {
            currentSmallestPrincipal = mohrCircleInfo.principals[2];
        }
    }

    return currentSmallestPrincipal;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RiuMohrsCirclePlot::replotAndScaleAxis()
{
    double maxYEnvelope = -HUGE_VAL;

    for (const std::pair<RimGeoMechView*, QwtPlotCurve*>& envelope : m_envolopePlotItems)
    {
        double tempMax = envelope.second->maxYValue();
        if (tempMax > maxYEnvelope)
        {
            maxYEnvelope = tempMax;
        }
    }

    double yHeight = std::max(maxYEnvelope, 1.2 * largestCircleRadiusInPlot());

    this->setAxisScale(QwtPlot::yLeft, 0, yHeight);

    double minXEvelope = HUGE_VAL;

    for (const std::pair<RimGeoMechView*, QwtPlotCurve*>& envelope : m_envolopePlotItems)
    {
        double tempMin = envelope.second->minXValue();
        if (tempMin < minXEvelope)
        {
            minXEvelope = tempMin;
        }
    }

    double xMin;
    if (minXEvelope < 0)
    {
        xMin = minXEvelope;
    }
    else
    {
        xMin = 1.1 * smallestPrincipal();
    }

    // When using the rescaler, xMax is ignored
    this->setAxisScale(QwtPlot::xBottom, xMin, 0);

    updateTransparentCurvesOnPrincipals();

    //Replotting must be done before rescaling
    this->replot();
    m_rescaler->rescale();
    this->plotLayout()->setAlignCanvasToScales(true);
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RiuMohrsCirclePlot::isValidPrincipals(const cvf::Vec3f& principals)
{
    float p1 = principals[0];
    float p2 = principals[1];
    float p3 = principals[2];

    // Inf
    if (p1 == HUGE_VAL || p2 == HUGE_VAL || p3 == HUGE_VAL)
    {
        return false;
    }

    // Nan
    if ((p1 != p1) || (p2 != p2) || p3 != p3)
    {
        return false;
    }

    // Principal rules:
    if ((p1 < p2) || (p2 < p3))
    {
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
float RiuMohrsCirclePlot::calculateFOS(const cvf::Vec3f& principals, double frictionAngle, double cohesion)
{
    float se1 = principals[0];
    float se3 = principals[2];

    if (cvf::Math::cos(frictionAngle) == 0)
    {
        return 0;
    }

    float tanFricAng = cvf::Math::tan(cvf::Math::toRadians(frictionAngle));
    float cohPrTanFricAngle = 1.0f * cohesion / tanFricAng;

    float pi_4 = 0.785398163397448309616f;
    float rho  = 2.0f * (cvf::Math::atan(cvf::Math::sqrt((se1 + cohPrTanFricAngle) / (se3 + cohPrTanFricAngle))) - pi_4);

    return tanFricAng / cvf::Math::tan(rho);
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QColor RiuMohrsCirclePlot::envelopeColor(RimGeoMechView* view)
{
    if (m_envolopeColors.find(view) == m_envolopeColors.end())
    {
        cvf::Color3ub cvfColor = RiaColorTables::summaryCurveDefaultPaletteColors().cycledColor3ub(m_envolopePlotItems.size());

        QColor color(cvfColor.r(), cvfColor.g(), cvfColor.b());

        m_envolopeColors[view] = color;
    }

    return m_envolopeColors[view];
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RiuMohrsCirclePlot::deletePlotItems()
{
    m_mohrsCiclesInfos.clear();

    deleteCircles();
    deleteEnvelopes();
}
