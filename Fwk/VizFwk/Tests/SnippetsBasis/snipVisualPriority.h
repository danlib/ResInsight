//##################################################################################################
//
//   Custom Visualization Core library
//   Copyright (C) 2011-2013 Ceetron AS
//
//   This library may be used under the terms of either the GNU General Public License or
//   the GNU Lesser General Public License as follows:
//
//   GNU General Public License Usage
//   This library is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This library is distributed in the hope that it will be useful, but WITHOUT ANY
//   WARRANTY; without even the implied warranty of MERCHANTABILITY or
//   FITNESS FOR A PARTICULAR PURPOSE.
//
//   See the GNU General Public License at <<http://www.gnu.org/licenses/gpl.html>>
//   for more details.
//
//   GNU Lesser General Public License Usage
//   This library is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Lesser General Public License as published by
//   the Free Software Foundation; either version 2.1 of the License, or
//   (at your option) any later version.
//
//   This library is distributed in the hope that it will be useful, but WITHOUT ANY
//   WARRANTY; without even the implied warranty of MERCHANTABILITY or
//   FITNESS FOR A PARTICULAR PURPOSE.
//
//   See the GNU Lesser General Public License at <<http://www.gnu.org/licenses/lgpl-2.1.html>>
//   for more details.
//
//##################################################################################################


#pragma once

#include "cvfuTestSnippet.h"
#include "cvfuProperty.h"

namespace snip {

using namespace cvf;
using namespace cvfu;


//==================================================================================================
//
// 
//
//==================================================================================================
class VisualPriority : public TestSnippet
{
    CVFU_DECLARE_SNIPPET("Visual Priority (eyelift, polyoffset)");

public:
    virtual bool    onInitialize();
    virtual void    onPaintEvent(PostEventAction* postEventAction);
    virtual void    onPropertyChanged(Property* property, PostEventAction* postEventAction);

private:
    void            buildModel();
    Part*           addAsSurfacePart(DrawableGeo* geo, const Color3f& color, double liftFactor);
    Part*           addAsLinesPart(DrawableGeo* geo, const Color3f& color, double liftFactor);
    static void     setTransformationMatrixOnPart(Part* part, const Mat4d& mat);

private:
    ref<ModelBasicList> m_theModel;
    ref<PropertyBool>   m_propUseShaders;
    ref<PropertyBool>   m_propAutomaticClipping;
    ref<PropertyBool>   m_propUsePolygonOffset;
    ref<PropertyBool>   m_propUseEyeLift;
};

}
