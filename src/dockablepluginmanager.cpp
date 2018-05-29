/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2014, Plasma Physics Laboratory - CNRS
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this program; if not, write to the Free Software
--  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
-------------------------------------------------------------------------------*/
/*--                  Author : Alexis Jeandet
--                     Mail : alexis.jeandet@member.fsf.org
----------------------------------------------------------------------------*/
#include "dockablepluginmanager.h"

dockablePluginManager::dockablePluginManager(QWidget *parent) :
    QDockWidget(parent)
{
    this->view = new PluginMangerView();
//    this->managerGui = new pluginmanagerWDGT;
//    connect(this->managerGui,SIGNAL(loadSysDrviver(QString)),this,SIGNAL(loadSysDrviver(QString)));
//    connect(this->managerGui,SIGNAL(loadSysDriverToParent(QString,QString)),this,SIGNAL(loadSysDriverToParent(QString,QString)));
//    connect(this->managerGui,SIGNAL(geteplugintree()),this,SIGNAL(geteplugintree()));
//    connect(this,SIGNAL(treeChanged(QList<socexplorerplugin*>)),this->managerGui,SIGNAL(treeChanged(QList<socexplorerplugin*>)));
//    connect(this->managerGui,SIGNAL(changeSysDriverInstName(QString,QString)),this,SIGNAL(changeSysDriverInstName(QString,QString)));
//    connect(this->managerGui,SIGNAL(closeSysDriver(QString)),this,SIGNAL(closeSysDriver(QString)));
//    connect(this->managerGui,SIGNAL(pluginselected(QString)),this,SIGNAL(pluginselected(QString)));
    this->setWidget(this->view);
    this->setWindowTitle(this->view->windowTitle());
}


