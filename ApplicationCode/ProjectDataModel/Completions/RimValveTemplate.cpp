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

#include "RimValveTemplate.h"

#include "RimWellPathAicdParameters.h"

CAF_PDM_SOURCE_INIT(RimValveTemplate, "ValveTemplate");


//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimValveTemplate::RimValveTemplate()
{
    CAF_PDM_InitObject("Valve Template", ":/ICDValve16x16.png", "", "");

    CAF_PDM_InitField(&m_valveTemplateUnit, "UnitSystem", caf::AppEnum<RiaEclipseUnitTools::UnitSystem>(RiaEclipseUnitTools::UNITS_UNKNOWN), "Units System", "", "", "");
    m_valveTemplateUnit.uiCapability()->setUiReadOnly(true);

    CAF_PDM_InitFieldNoDefault(&m_type, "CompletionType", "Type    ", "", "", "");
    m_type = RiaDefines::ICD;

    CAF_PDM_InitField(&m_orificeDiameter, "OrificeDiameter", 8.0, "Orifice Diameter [mm]", "", "", "");
    CAF_PDM_InitField(&m_flowCoefficient, "FlowCoefficient", 0.7, "Flow Coefficient", "", "", "");

    CAF_PDM_InitFieldNoDefault(&m_aicdParameters, "AICDParameters", "AICD Parameters", "", "", "");
    m_aicdParameters = new RimWellPathAicdParameters;
    m_aicdParameters.uiCapability()->setUiTreeHidden(true);
    m_aicdParameters.uiCapability()->setUiTreeChildrenHidden(true);

}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimValveTemplate::~RimValveTemplate()
{
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimValveTemplate::loadDataAndUpdate()
{

}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimValveTemplate::setUnitSystem(RiaEclipseUnitTools::UnitSystemType unitSystem)
{
    m_valveTemplateUnit = unitSystem;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimValveTemplate::setDefaultValuesFromUnits()
{
    if (m_valveTemplateUnit == RiaEclipseUnitTools::UNITS_METRIC)
    {
        m_orificeDiameter = 8;
    }
    else if (m_valveTemplateUnit == RiaEclipseUnitTools::UNITS_FIELD)
    {
        m_orificeDiameter = 0.315;
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QList<caf::PdmOptionItemInfo> RimValveTemplate::calculateValueOptions(const caf::PdmFieldHandle* fieldNeedingOptions,
                                                                      bool*                      useOptionsOnly)
{
    QList<caf::PdmOptionItemInfo> options;

    if (fieldNeedingOptions == &m_type)
    {
        std::set<RiaDefines::WellPathComponentType> supportedTypes = {RiaDefines::ICD, RiaDefines::AICD, RiaDefines::ICV};
        for (RiaDefines::WellPathComponentType type : supportedTypes)
        {
            options.push_back(caf::PdmOptionItemInfo(CompletionTypeEnum::uiText(type), type));
        }
    }
    return options;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimValveTemplate::defineUiOrdering(QString uiConfigName, caf::PdmUiOrdering& uiOrdering)
{
    uiOrdering.add(&m_type);

    if (m_type() == RiaDefines::ICV || m_type() == RiaDefines::ICD)
    {
        if (m_valveTemplateUnit == RiaEclipseUnitTools::UNITS_METRIC)
        {
            m_orificeDiameter.uiCapability()->setUiName("Orifice Diameter [mm]");
        }
        else if (m_valveTemplateUnit == RiaEclipseUnitTools::UNITS_FIELD)
        {
            m_orificeDiameter.uiCapability()->setUiName("Orifice Diameter [in]");
        }
    }

    if (m_type() == RiaDefines::ICV || m_type() == RiaDefines::ICD)
    {
        caf::PdmUiGroup* group = uiOrdering.addNewGroup("MSW Valve Parameters");
        group->add(&m_orificeDiameter);
        group->add(&m_flowCoefficient);
    }
    else
    {
        caf::PdmUiGroup* group = uiOrdering.addNewGroup("MSW AICD Parameters");
        m_aicdParameters->uiOrdering(uiConfigName, *group);
    }
    uiOrdering.skipRemainingFields(true);
}
