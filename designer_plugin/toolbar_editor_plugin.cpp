/**

@author Mattia Basaglia

@section License

    Copyright (C) 2012-2020 Mattia Basaglia

    This file is part of Color Widgets.

    Color Widgets is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Color Widgets is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Color Widgets.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "toolbar_editor_plugin.hpp"
#include "toolbar_editor.hpp"
#include <QtPlugin>

Toolbar_Editor_Plugin::Toolbar_Editor_Plugin(QObject *parent)
    : QObject(parent), initialized(false)
{
}


void Toolbar_Editor_Plugin::initialize(QDesignerFormEditorInterface *)
{
    if (initialized)
        return;

    initialized = true;
}

bool Toolbar_Editor_Plugin::isInitialized() const
{
    return initialized;
}

QWidget *Toolbar_Editor_Plugin::createWidget(QWidget *parent)
{
    return new Toolbar_Editor(parent);
}

QString Toolbar_Editor_Plugin::name() const
{
    return "Toolbar_Editor";
}

QString Toolbar_Editor_Plugin::group() const
{
    return "Complex Config Widgets";
}

QIcon Toolbar_Editor_Plugin::icon() const
{
    return QIcon::fromTheme("configure-toolbars");
}

QString Toolbar_Editor_Plugin::toolTip() const
{
    return "Edit QMainWindow toolbars from its menus";
}

QString Toolbar_Editor_Plugin::whatsThis() const
{
    return toolTip();
}

bool Toolbar_Editor_Plugin::isContainer() const
{
    return false;
}

QString Toolbar_Editor_Plugin::domXml() const
{

    return "<ui language=\"c++\">\n"
           " <widget class=\"Toolbar_Editor\" name=\"Toolbar_Editor\">\n"
           " </widget>\n"
            "</ui>\n";
}

QString Toolbar_Editor_Plugin::includeFile() const
{
    return "toolbar_editor.hpp";
}



