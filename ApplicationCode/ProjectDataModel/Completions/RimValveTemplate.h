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

#include "RiaDefines.h"
#include "RiaEclipseUnitTools.h"

#include "RimCheckableNamedObject.h"

#include "cafPdmChildField.h"

class RimWellPathAicdParameters;

class RimValveTemplate : public RimNamedObject
{
    CAF_PDM_HEADER_INIT;
public:

    RimValveTemplate();
    ~RimValveTemplate() override;

    void loadDataAndUpdate();
    void setUnitSystem(RiaEclipseUnitTools::UnitSystemType unitSystem);
    void setDefaultValuesFromUnits();


    virtual QList<caf::PdmOptionItemInfo> calculateValueOptions(const caf::PdmFieldHandle* fieldNeedingOptions, bool* useOptionsOnly) override;

protected:
    virtual void defineUiOrdering(QString uiConfigName, caf::PdmUiOrdering& uiOrdering) override;

private:
    typedef caf::AppEnum<RiaDefines::WellPathComponentType> CompletionTypeEnum;

    caf::PdmField<RiaEclipseUnitTools::UnitSystemType> m_valveTemplateUnit;

    caf::PdmField<CompletionTypeEnum>              m_type;

    // ICD and ICVs only
    caf::PdmField<double> m_orificeDiameter;
    caf::PdmField<double> m_flowCoefficient;

    caf::PdmChildField<RimWellPathAicdParameters*> m_aicdParameters;

};

