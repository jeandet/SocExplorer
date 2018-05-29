/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2011, Plasma Physics Laboratory - CNRS
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
--                     Mail : alexis.jeandet@lpp.polytechnique.fr
----------------------------------------------------------------------------*/
#ifndef DOCKABLEPLUGINMANAGER_H
#define DOCKABLEPLUGINMANAGER_H

#include <QDockWidget>
#include "pluginmanagerWDGT.h"
#include <pluginmangerview.h>

class dockablePluginManager : public QDockWidget
{
    Q_OBJECT
public:
    explicit dockablePluginManager(QWidget *parent = 0);

signals:
    void loadSysDrviver(const QString name);
    void loadSysDriverToParent(const QString name, const QString instanceName);
    void geteplugintree(void);
    void treeChanged(const QList<socexplorerplugin*>& drivers);
    void changeSysDriverInstName(const QString newinstanceName,const QString previnstanceName);
    void closeSysDriver(const QString instanceName);
    void pluginselected(const QString& instanceName);
public slots:

private:
//    pluginmanagerWDGT* managerGui;
    PluginMangerView* view;
    
};

#endif // DOCKABLEPLUGINMANAGER_H
