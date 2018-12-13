/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016-     Statoil ASA
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

#include "RicNewValveTemplateFeature.h"

#include "RiaApplication.h"

#include "RimEclipseView.h"
#include "RimValveTemplate.h"
#include "RimValveTemplateCollection.h"
#include "RimOilField.h"
#include "RimProject.h"

#include "Riu3DMainWindowTools.h"

#include "cafSelectionManager.h"

#include "cvfAssert.h"

#include <QAction>

CAF_CMD_SOURCE_INIT(RicNewValveTemplateFeature, "RicNewValveTemplateFeature");

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicNewValveTemplateFeature::selectValveTemplateAndUpdate(RimValveTemplateCollection* templateCollection,
                                                              RimValveTemplate*           valveTemplate)
{
    valveTemplate->loadDataAndUpdate();

    templateCollection->updateConnectedEditors();

    RimProject* project = RiaApplication::instance()->project();

    std::vector<Rim3dView*> views;
    project->allVisibleViews(views);

    for (Rim3dView* view : views)
    {
        if (dynamic_cast<RimEclipseView*>(view))
        {
            view->updateConnectedEditors();
        }
    }

    Riu3DMainWindowTools::selectAsCurrentItem(valveTemplate);
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicNewValveTemplateFeature::onActionTriggered(bool isChecked)
{
    RimProject* project = RiaApplication::instance()->project();
    CVF_ASSERT(project);

    RimOilField* oilfield = project->activeOilField();
    if (oilfield == nullptr) return;

    RimValveTemplateCollection* valveTemplateColl = oilfield->valveTemplateCollection();

    if (valveTemplateColl)
    {
        RimValveTemplate* valveTemplate = new RimValveTemplate();

        valveTemplateColl->addValveTemplate(valveTemplate);
        valveTemplate->setName("ICD Template");
        valveTemplate->setUnitSystem(valveTemplateColl->defaultUnitSystemType());
        valveTemplate->setDefaultValuesFromUnits();

        selectValveTemplateAndUpdate(valveTemplateColl, valveTemplate);
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicNewValveTemplateFeature::setupActionLook(QAction* actionToSetup)
{
    actionToSetup->setIcon(QIcon(":/ICDValve16x16.png"));
    actionToSetup->setText("New Valve Template");
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RicNewValveTemplateFeature::isCommandEnabled()
{
    return true;
}
