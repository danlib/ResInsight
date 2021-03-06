/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017     Statoil ASA
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

#include "qwt_plot.h"

#include "cafPdmPointer.h"

#include <QPointer>
#include <QFrame>

#include "RiuInterfaceToViewWindow.h"

class RimWellRftPlot;
class RiuNightchartsWidget;

class QLabel;

namespace cvf {
    class Color3f;
}

//==================================================================================================
//
//
//
//==================================================================================================
class RiuWellRftPlot : public QFrame, public RiuInterfaceToViewWindow
{
    Q_OBJECT;
public:
    RiuWellRftPlot(RimWellRftPlot* plotDefinition, QWidget* parent = nullptr);
    ~RiuWellRftPlot() override;

    RimWellRftPlot*                 ownerPlotDefinition();
    RimViewWindow*          ownerViewWindow() const override;

    void                            showTitle(const QString& title);
    void                            hideTitle();

protected:
    QSize                   sizeHint() const override;
    QSize                   minimumSizeHint() const override;

    void                    contextMenuEvent(QContextMenuEvent *) override;

private:
    void                            setDefaults();

private:
    caf::PdmPointer<RimWellRftPlot> m_plotDefinition;
    QPointer<QLabel> m_titleLabel;
};
