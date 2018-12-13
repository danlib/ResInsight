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

#include "RimValveTemplateCollection.h"

#include "RiaApplication.h"

#include "RimProject.h"
#include "RimValveTemplate.h"

CAF_PDM_SOURCE_INIT(RimValveTemplateCollection, "ValveTemplateCollection");


//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimValveTemplateCollection::RimValveTemplateCollection()
{
    CAF_PDM_InitObject("Valve Templates", ":/ICDValve16x16.png", "", "");
    CAF_PDM_InitFieldNoDefault(&m_valveDefinitions, "ValveDefinitions", "", "", "", "");
    m_valveDefinitions.uiCapability()->setUiHidden(true);
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimValveTemplateCollection::~RimValveTemplateCollection()
{
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<RimValveTemplate*> RimValveTemplateCollection::valveTemplates() const
{
    std::vector<RimValveTemplate*> templates;
    for (auto& templ : m_valveDefinitions)
    {
        templates.push_back(templ);
    }
    return templates;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimValveTemplateCollection::addValveTemplate(RimValveTemplate* valveTemplate)
{
    m_valveDefinitions.push_back(valveTemplate);
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RiaEclipseUnitTools::UnitSystemType RimValveTemplateCollection::defaultUnitSystemType() const
{
    return m_defaultUnitsForFracTemplates;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimValveTemplateCollection::setDefaultUnitSystemBasedOnLoadedCases()
{
    RimProject* proj = RiaApplication::instance()->project();

    auto commonUnitSystem = proj->commonUnitSystemForAllCases();
    if (commonUnitSystem != RiaEclipseUnitTools::UNITS_UNKNOWN)
    {
        m_defaultUnitsForFracTemplates = commonUnitSystem;
    }
}

