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

#include "cafPdmObject.h"

#include "cafPdmChildField.h"
#include "cafPdmField.h"

class RimGridView;
class RimAnnotationLineAppearance;

//==================================================================================================
///
///
//==================================================================================================
class RimLineBasedAnnotation : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    RimLineBasedAnnotation();
    RimAnnotationLineAppearance* appearance() const;

    bool isActive();

protected:
    virtual caf::PdmFieldHandle* objectToggleField() override;

private:
    caf::PdmField<bool> m_isActive;

private:
    caf::PdmChildField<RimAnnotationLineAppearance*> m_appearance;
};