/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018-     Equinor ASA
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

#pragma once

#include "RimPlotCurve.h"
#include "RimWellLogPlot.h"
#include "RimWellPathAttribute.h"

#include "cafPdmBase.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"

#include "cvfColor4.h"

#include <QBrush>
#include <QString>

class RigWellLogCurveData;
class RimFishbonesMultipleSubs;
class RimFracture;
class RimPerforationInterval;
class RimWellPath;
class QwtPlotItem;

//==================================================================================================
///  
///  
//==================================================================================================
class RiuWellPathAttributePlotObject
{
   
public:
    RiuWellPathAttributePlotObject(const RimWellPath*              wellPath);

    RiuWellPathAttributePlotObject(const RimWellPath*              wellPath,
                                   const RimWellPathAttribute*     wellPathAttribute);

    RiuWellPathAttributePlotObject(const RimWellPath*              wellPath,
                                   const RimPerforationInterval*   perforationInterval);

    RiuWellPathAttributePlotObject(const RimWellPath*              wellPath,
                                   const RimFishbonesMultipleSubs* fishbones);

    RiuWellPathAttributePlotObject(const RimWellPath*              wellPath,
                                   const RimFracture*              fracture);

    ~RiuWellPathAttributePlotObject();

    QString label();
    void    loadDataAndUpdate(bool updateParentPlot);

    RimWellPathAttribute::AttributeType attributeType() const;

    bool xValueRange(double* minimumValue, double* maximumValue) const;
    bool yValueRange(double* minimumValue, double* maximumValue) const;

    void setShowLabel(bool showLabel);
    void setDepthType(RimWellLogPlot::DepthTypeEnum depthType);
    void setBaseColor(const cvf::Color3f& baseColor);
    void setBaseColor(const cvf::Color4f& baseColor);

    void setParentQwtPlotAndReplot(QwtPlot* plot);
    void setParentQwtPlotNoReplot(QwtPlot* plot);
    void attachToQwt();
    void detachFromQwt();
    void reattachToQwt();

private:
    void onLoadDataAndUpdate(bool updateParentPlot);

    std::pair<double, double> depthsOfDepthType() const;

    void addMarker(double                        posX,
                   double                        depth,
                   int                           size,
                   RiuQwtSymbol::PointSymbolEnum symbolType,
                   cvf::Color4f                  baseColor,
                   QString                       label             = QString(""),
                   Qt::Alignment                 labelAlignment    = Qt::AlignTop,
                   Qt::Orientation               labelOrientation  = Qt::Vertical,
                   bool                          drawLine          = false,
                   bool                          contrastTextColor = true);
    void addColumnFeature(double startX,
                          double endX,
                          double startDepth,
                          double endDepth,
                          cvf::Color4f baseColor,
                          Qt::BrushStyle brushStyle = Qt::SolidPattern);

    void drawColumnFeature(double         startX,
                           double         endX,
                           double         startDepth,
                           double         endDepth,
                           cvf::Color4f   baseColor,
                           Qt::BrushStyle brushStyle = Qt::SolidPattern);

private:
    const RimWellPath*                      m_wellPath;

    RimWellPathAttribute::AttributeType     m_attributeType;
    double                                  m_startMD;
    double                                  m_endMD;
    QString                                 m_label;

    RimWellLogPlot::DepthTypeEnum           m_depthType;
    QPointer<QwtPlot>                       m_parentQwtPlot;
    std::vector<QwtPlotItem*>               m_plotFeatures;
    cvf::Color4f                            m_baseColor;

    bool                                    m_showLabel;
};