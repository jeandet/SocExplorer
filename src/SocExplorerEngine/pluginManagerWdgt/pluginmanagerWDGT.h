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
#ifndef PLUGINMANAGERWDGT_H
#define PLUGINMANAGERWDGT_H

#include <QApplication>
#include <QWidget>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QDir>
#include <QStringList>
#include <QFileInfoList>
#include <QFileInfo>
#include <QTextEdit>
#include <QPushButton>
#include <QSplitter>
#include "plugininfoswdgt.h"
#include "pluginlist.h"
#include <socexplorerplugin.h>
#include <QList>
#include "PluginTreeWidget.h"

#if defined(SOCEXPLORER_SDK_BUILD)
#  define SOCEXPLORER_SDK_EXPORT Q_DECL_EXPORT
#else
#  define SOCEXPLORER_SDK_EXPORT Q_DECL_IMPORT
#endif

class pluginmanagerWDGT : public QSplitter
{
    Q_OBJECT
public:
SOCEXPLORER_SDK_EXPORT        explicit pluginmanagerWDGT(QWidget *parent = 0);
SOCEXPLORER_SDK_EXPORT        void setRootLoadable(bool flag);
SOCEXPLORER_SDK_EXPORT        void setChildLoadable(bool flag);

signals:
    void updatepluginInfo(const QString libname);
    void loadSysDrviver(const QString name);
    void loadSysDriverToParent(const QString name, const QString instanceName);
    void geteplugintree(void);
    void treeChanged(const QList<socexplorerplugin*>& drivers);
    void changeSysDriverInstName(const QString newinstanceName,const QString previnstanceName);
    void closeSysDriver(const QString instanceName);
    void pluginselected(const QString& instanceName);

public slots:


private:

    bool rootLoadable;
    bool childLoadable;
    QWidget* pluginListWidgetContainer;
    QVBoxLayout* pluginListLayout;
    QHBoxLayout* ButtonsLayout;
    QSplitter* mainlayoutSpliter;
    PluginList* pluginTable;
    pluginInfosWdgt* pluginInfos;
    PluginTreeWidget* treeview;
    QPushButton* addPluginBt;
    QPushButton* removePluginBt;
    QPushButton* loadPluginBt;
    QPushButton* refreshPluginListBt;


};

#endif // PLUGINMANAGERWDGT_H
