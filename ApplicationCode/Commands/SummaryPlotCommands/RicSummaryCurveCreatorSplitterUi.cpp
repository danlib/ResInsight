/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017-     Statoil ASA
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

#include "RicSummaryCurveCreatorSplitterUi.h"

#include "RicSummaryCurveCreator.h"
#include "RicSummaryCurveCreatorUiKeywords.h"

#include "cafPdmUiFieldEditorHandle.h"
#include "cafPdmUiFieldHandle.h"
#include "cafPdmUiGroup.h"
#include "cafPdmUiTreeView.h"

#include "QMinimizePanel.h"

#include <QBoxLayout>
#include <QSplitter>
#include <QFrame>
#include <QTreeView>
#include "RimSummaryPlot.h"
#include "RimSummaryCurveCollection.h"

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RicSummaryCurveCreatorSplitterUi::RicSummaryCurveCreatorSplitterUi(QWidget* parent)
{
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
RicSummaryCurveCreatorSplitterUi::~RicSummaryCurveCreatorSplitterUi()
{

}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RicSummaryCurveCreatorSplitterUi::recursivelyConfigureAndUpdateTopLevelUiItems(const std::vector<caf::PdmUiItem *>& topLevelUiItems, const QString& uiConfigName)
{
    RicSummaryCurveCreator* sumCurveCreator = dynamic_cast<RicSummaryCurveCreator*>(this->pdmItem());
    if (sumCurveCreator)
    {
        if (sumCurveCreator->isCloseButtonPressed())
        {
            sumCurveCreator->clearCloseButton();

            emit signalCloseButtonPressed();
        }
    }

    if (!m_layout) return;

    int splitterPositionIndex = 0;
    for (size_t i = 0; i < topLevelUiItems.size(); ++i)
    {
        if (topLevelUiItems[i]->isUiHidden(uiConfigName)) continue;

        if (topLevelUiItems[i]->isUiGroup())
        {
            caf::PdmUiGroup* group = static_cast<caf::PdmUiGroup*>(topLevelUiItems[i]);
            auto groupBox = createGroupBoxWithContent(group, uiConfigName);

            m_firstRowLayout->addWidget(groupBox);

            // Add group boxes until summaries are detected

            if (group->keyword() == RicSummaryCurveCreatorUiKeywords::summaries())
                break;
        }
    }

    caf::PdmUiGroup* appearanceGroup = findGroupByKeyword(topLevelUiItems, RicSummaryCurveCreatorUiKeywords::appearance(), uiConfigName);
    auto appearanceGroupBox = createGroupBoxWithContent(appearanceGroup, uiConfigName);
    m_lowerLeftLayout->insertWidget(0, appearanceGroupBox);

    caf::PdmUiGroup* nameConfigGroup = findGroupByKeyword(topLevelUiItems, RicSummaryCurveCreatorUiKeywords::nameConfig(), uiConfigName);
    auto nameConfigGroupBox = createGroupBoxWithContent(nameConfigGroup, uiConfigName);
    m_lowerLeftLayout->insertWidget(1, nameConfigGroupBox);

    m_lowerLeftLayout->insertWidget(2, getOrCreateCurveTreeWidget(), 1);

    m_secondRowLayout->insertWidget(1, getOrCreatePlotWidget());

    // Fields at bottom of dialog
    configureAndUpdateFields(1, m_bottomFieldLayout, topLevelUiItems, uiConfigName);
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QWidget* RicSummaryCurveCreatorSplitterUi::createWidget(QWidget* parent)
{
    QWidget* widget = new QWidget(parent);

    m_layout = new QVBoxLayout();
    m_layout->setContentsMargins(0, 0, 0, 0);
    widget->setLayout(m_layout);

    QFrame* firstRowFrame = new QFrame(widget);
    m_firstRowLayout = new QHBoxLayout;
    firstRowFrame->setLayout(m_firstRowLayout);

    QFrame* secondRowFrame = new QFrame(widget);
    m_secondRowLayout = new QHBoxLayout;
    secondRowFrame->setLayout(m_secondRowLayout);

    m_lowerLeftLayout = new QVBoxLayout;
    m_secondRowLayout->addLayout(m_lowerLeftLayout);

    m_firstColumnSplitter = new QSplitter(Qt::Vertical);
    m_firstColumnSplitter->insertWidget(0, firstRowFrame);
    m_firstColumnSplitter->insertWidget(1, secondRowFrame);

    m_layout->addWidget(m_firstColumnSplitter);

    m_bottomFieldLayout = new QHBoxLayout;
    m_layout->addLayout(m_bottomFieldLayout);
    m_bottomFieldLayout->insertStretch(0, 1);

    return widget;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
caf::PdmUiGroup* RicSummaryCurveCreatorSplitterUi::findGroupByKeyword(const std::vector<caf::PdmUiItem *>& topLevelUiItems, const QString& keyword, const QString& uiConfigName)
{
    for (auto uiItem : topLevelUiItems)
    {
        if (uiItem->isUiHidden(uiConfigName)) continue;

        if (uiItem->isUiGroup())
        {
            caf::PdmUiGroup* group = static_cast<caf::PdmUiGroup*>(uiItem);
            if (group->keyword() == keyword)
            {
                return group;
            }
        }
    }

    return nullptr;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QWidget* RicSummaryCurveCreatorSplitterUi::getOrCreateCurveTreeWidget()
{
    if (!m_curvesPanel)
    {
        m_curvesPanel = new QMinimizePanel(this->widget());
        m_curvesPanel->setTitle("Curves");
        QVBoxLayout* curvesLayout = new QVBoxLayout(m_curvesPanel->contentFrame());

        caf::PdmUiTreeView* curveTreeView = new caf::PdmUiTreeView(m_curvesPanel->contentFrame());
        curvesLayout->addWidget(curveTreeView);

        RicSummaryCurveCreator* sumCurveCreator = dynamic_cast<RicSummaryCurveCreator*>(this->pdmItem());
        if (sumCurveCreator)
        {
            RimSummaryCurveCollection* sumColl = sumCurveCreator->previewPlot()->summaryCurveCollection();
            curveTreeView->setPdmItem(sumColl);
        }

        curveTreeView->treeView()->setHeaderHidden(true);
    }

    return m_curvesPanel;

}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QWidget* RicSummaryCurveCreatorSplitterUi::getOrCreatePlotWidget()
{
    RicSummaryCurveCreator* sumCurveCreator = dynamic_cast<RicSummaryCurveCreator*>(this->pdmItem());
    if (sumCurveCreator)
    {
        // TODO: Rename previewPlot()->createViewWidget to getOrCreateViewWidget
        return sumCurveCreator->previewPlot()->createViewWidget(this->widget());
    }

    return nullptr;
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void RicSummaryCurveCreatorSplitterUi::configureAndUpdateFields(int widgetStartIndex, 
                                                                QBoxLayout* layout,
                                                                const std::vector<caf::PdmUiItem *>& uiItems,
                                                                const QString& uiConfigName)
{
    int currentWidgetIndex = widgetStartIndex;

    for (size_t i = 0; i < uiItems.size(); ++i)
    {
        if (uiItems[i]->isUiHidden(uiConfigName)) continue;
        if (uiItems[i]->isUiGroup()) continue;

        {
            caf::PdmUiFieldHandle* field = dynamic_cast<caf::PdmUiFieldHandle*>(uiItems[i]);

            caf::PdmUiFieldEditorHandle* fieldEditor = findOrCreateFieldEditor(this->widget(), field, uiConfigName);

            if (fieldEditor)
            {
                fieldEditor->setField(field);

                // Place the widget(s) into the correct parent and layout
                QWidget* fieldCombinedWidget = fieldEditor->combinedWidget();

                if (fieldCombinedWidget)
                {
                    fieldCombinedWidget->setParent(this->widget());
                    layout->insertWidget(currentWidgetIndex++, fieldCombinedWidget);
                }
                else
                {
                    caf::PdmUiItemInfo::LabelPosType labelPos = field->uiLabelPosition(uiConfigName);

                    QWidget* fieldEditorWidget = fieldEditor->editorWidget();

                    if (labelPos != caf::PdmUiItemInfo::HIDDEN)
                    {
                        QWidget* fieldLabelWidget = fieldEditor->labelWidget();
                        if (fieldLabelWidget)
                        {
                            fieldLabelWidget->setParent(this->widget());

                            layout->insertWidget(currentWidgetIndex++, fieldLabelWidget);

                            fieldLabelWidget->show();
                        }
                    }
                    else
                    {
                        QWidget* fieldLabelWidget = fieldEditor->labelWidget();
                        if (fieldLabelWidget) fieldLabelWidget->hide();
                    }

                    if (fieldEditorWidget)
                    {
                        fieldEditorWidget->setParent(this->widget()); // To make sure this widget has the current group box as parent.

                        layout->insertWidget(currentWidgetIndex++, fieldEditorWidget);
                    }
                }

                fieldEditor->updateUi(uiConfigName);
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
QMinimizePanel* RicSummaryCurveCreatorSplitterUi::createGroupBoxWithContent(caf::PdmUiGroup* group,
                                                                            const QString& uiConfigName)
{
    QMinimizePanel* groupBox = findOrCreateGroupBox(this->widget(), group, uiConfigName);

    const std::vector<caf::PdmUiItem*>& groupChildren = group->uiItems();
    recursivelyConfigureAndUpdateUiItemsInGridLayoutColumn(groupChildren, groupBox->contentFrame(), uiConfigName);
    return groupBox;
}