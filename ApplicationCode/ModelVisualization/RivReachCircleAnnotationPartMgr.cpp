/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2011-     Statoil ASA
//  Copyright (C) 2013-     Ceetron Solutions AS
//  Copyright (C) 2011-2012 Ceetron AS
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



#include "RivReachCircleAnnotationPartMgr.h"

#include "RimReachCircleAnnotation.h"

#include "RivPolylineGenerator.h"
#include "RivPartPriority.h"
#include "RivReachCircleAnnotationSourceInfo.h"

#include "cafEffectGenerator.h"

#include "cvfDrawableGeo.h"
#include "cvfModelBasicList.h"
#include "cvfPart.h"
#include "cafDisplayCoordTransform.h"

#include <cmath>

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RivReachCircleAnnotationPartMgr::RivReachCircleAnnotationPartMgr(RimReachCircleAnnotation* annotation)
: m_rimAnnotation(annotation)
{
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RivReachCircleAnnotationPartMgr::~RivReachCircleAnnotationPartMgr()
{

}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RivReachCircleAnnotationPartMgr::buildParts(const caf::DisplayCoordTransform* displayXf, bool doFlatten, double xOffset)
{
    clearAllGeometry();

    cvf::ref<RivReachCircleAnnotationSourceInfo> sourceInfo = new RivReachCircleAnnotationSourceInfo(m_rimAnnotation);

    Vec3d   centerPosition = displayXf->transformToDisplayCoord(m_rimAnnotation->centerPoint());
    double  radius         = m_rimAnnotation->radius();

    // Circle part
    {
        int numPoints = 36;
        std::vector<Vec3d> points;
        for (int i = 0; i < numPoints; i++)
        {
            double rad = 2 * cvf::PI_D * (double)i / (double)numPoints;
            Vec3d pt(centerPosition.x() + cos(rad) * radius, centerPosition.y() + sin(rad) * radius , centerPosition.z());
            points.push_back(pt);
        }
        points.push_back(points.front());

        cvf::ref<cvf::DrawableGeo> drawableGeo = RivPolylineGenerator::createLineAlongPolylineDrawable(points);

        cvf::ref<cvf::Part> part = new cvf::Part;
        part->setDrawable(drawableGeo.p());

        caf::MeshEffectGenerator colorEffgen(cvf::Color3f::RED);
        cvf::ref<cvf::Effect>    eff = colorEffgen.generateUnCachedEffect();

        part->setEffect(eff.p());
        part->setPriority(RivPartPriority::PartType::MeshLines);
        part->setSourceInfo(sourceInfo.p());

        m_circlePart = part;
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RivReachCircleAnnotationPartMgr::clearAllGeometry()
{
    m_circlePart  = nullptr;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RivReachCircleAnnotationPartMgr::appendDynamicGeometryPartsToModel(cvf::ModelBasicList*              model,
                                                                 const caf::DisplayCoordTransform* displayXf)
{
    if (m_rimAnnotation.isNull()) return;
    if (!validateAnnotation(m_rimAnnotation)) return;

    buildParts(displayXf, false, 0.0);
    model->addPart(m_circlePart.p());
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RivReachCircleAnnotationPartMgr::validateAnnotation(const RimReachCircleAnnotation* annotation) const
{
    return m_rimAnnotation->centerPoint() != cvf::Vec3d::ZERO && m_rimAnnotation->radius() > 0.0;
}