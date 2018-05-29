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
#ifndef SOCEXPLORERPROXY_H
#define SOCEXPLORERPROXY_H

#include <QObject>
#include <QString>
#include <QList>
#include <QVector>
#include "pluginscache.h"
#include <socexplorerplugin.h>
#include <socexplorerplugininterface.h>
#include <QInputDialog>
#include <QtGlobal>

#if defined(SOCEXPLORER_SDK_BUILD)
#  define SOCEXPLORER_SDK_EXPORT Q_DECL_EXPORT
#else
#  define SOCEXPLORER_SDK_EXPORT Q_DECL_IMPORT
#endif

class SOCEXPLORER_SDK_EXPORT socexplorerproxy : public QObject
{
    Q_OBJECT
private:
    socexplorerproxy(QObject *parent = 0);
    socexplorerproxy(QMainWindow* Mainwindow,QObject *parent = 0);
    static socexplorerproxy* _self;
public:
    static socexplorerproxy* self(){if(!_self)init(); return _self;}
    static void init();
    static void setMainWindow(QMainWindow* Mainwindow);
    static void connectChildToParent(socexplorerplugin* parent, socexplorerplugin* child);
    static void connectChildToProxy(socexplorerplugin* child);
    static void disconnectChildToParent(socexplorerplugin* child);
    static void disconnectChildToProxy(socexplorerplugin* child);
    static bool instanceNameIsValid(const QString& instanceName);
    static socexplorerplugin* findPlugin(const QString &instanceName);
    static QStringList getPluginsList();
    static bool loadPluginsList( QStringList plugins);

    QT_ENSURE_STACK_ALIGNED_FOR_SSE static void loadChildSysDriver(socexplorerplugin* parent,const QString child);

signals:
    void addPluginGUI(QDockWidget* plugin);
    void removePluginGUI(QDockWidget* plugin);
    int registermenu(QMainWindow* menuHolder);
    void clearMenu();
    QStringList findDrivers(int VID,int PID);
    void rootDriverAdded(socexplorerplugin* driver);
    void rootDriverRemoved();
    void closeSysDriverSig(socexplorerplugin* driver);
    void registerObject(QObject* object,const QString& instanceName);
    void treeChanged(const QVector<socexplorerplugin*>& drivers);

public slots:
    QT_ENSURE_STACK_ALIGNED_FOR_SSE void loadSysDriver(const QString name);
    QT_ENSURE_STACK_ALIGNED_FOR_SSE void loadSysDriver(const QString name,const QString instanceName);
    QT_ENSURE_STACK_ALIGNED_FOR_SSE void loadSysDriver(socexplorerplugin* driver,const QString instanceName,const QString path="");
    QT_ENSURE_STACK_ALIGNED_FOR_SSE void loadSysDriverToParent(const QString name,const QString ParentInst);
    QT_ENSURE_STACK_ALIGNED_FOR_SSE void loadSysDriverToParent(const QString name,const QString instanceName,const QString ParentInst);
    QT_ENSURE_STACK_ALIGNED_FOR_SSE void loadSysDriverToParent(socexplorerplugin *driver,socexplorerplugin *parent, const QString instanceName,const QString path="");
    QT_ENSURE_STACK_ALIGNED_FOR_SSE void changeSysDriverInstName(const QString instanceName);
    QT_ENSURE_STACK_ALIGNED_FOR_SSE void changeSysDriverInstName(const QString newinstanceName,const QString previnstanceName);
 //   QT_ENSURE_STACK_ALIGNED_FOR_SSE void loadChild(socexplorerplugin* parent);
    void updateText();
    QT_ENSURE_STACK_ALIGNED_FOR_SSE void closeSysDriverFromDriver(socexplorerplugin* driver);
    QT_ENSURE_STACK_ALIGNED_FOR_SSE void closeSysDriver(socexplorerplugin* driver,bool recursive=false);
    QT_ENSURE_STACK_ALIGNED_FOR_SSE void closeSysDriver(const QString instanceName);
    void makeMenu(QMenu* menu);
    void close();
    QString getinstanceName(const QString& baseName);
    void geteplugintree();
    QT_ENSURE_STACK_ALIGNED_FOR_SSE socexplorerplugin* getSysDriver(const QString instanceName);
    bool instanceExists(const QString& instanceName);
private:

    static void changeSysDriverInstName(socexplorerplugin* driver);
    static QMainWindow* mainWindow;
    static QVector<socexplorerplugin*>* drivers;
    static QVector<socexplorerplugin*>* linearDriverList;
    static QStringList linearDriverPathList;
    static socexplorerplugin* root;
    static socexplorerplugin* parent;
};

#endif // SOCEXPLORERPROXY_H
