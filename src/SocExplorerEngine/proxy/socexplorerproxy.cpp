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
#include "socexplorerproxy.h"
#include <pluginmanagerWDGT.h>
#include <socexplorerengine.h>
#include <pluginloader.h>
#include <socexplorerplugin.h>

socexplorerproxy* socexplorerproxy::_self=NULL;
QMainWindow* socexplorerproxy::mainWindow=NULL;
QVector<socexplorerplugin*>* socexplorerproxy::drivers=NULL;
QVector<socexplorerplugin*>* socexplorerproxy::linearDriverList=NULL;
socexplorerplugin* socexplorerproxy::root=NULL;
socexplorerplugin* socexplorerproxy::parent=NULL;
QStringList socexplorerproxy::linearDriverPathList=QStringList{};

socexplorerproxy::socexplorerproxy(QObject *parent) :
    QObject(parent)
{
    drivers = new QVector<socexplorerplugin*>;
    linearDriverList=new QVector<socexplorerplugin*>;
    root = NULL;
}


socexplorerproxy::socexplorerproxy(QMainWindow *Mainwindow, QObject *parent):
    QObject(parent)
{
    mainWindow = Mainwindow;
    drivers = new QVector<socexplorerplugin*>;
    linearDriverList=new QVector<socexplorerplugin*>;
    root = NULL;
}


void socexplorerproxy::init()
{
    if(!_self)
    {
        _self = new socexplorerproxy();
        mainWindow = NULL;
    }

}

void socexplorerproxy::setMainWindow(QMainWindow *Mainwindow)
{
    if(!_self)init();
    mainWindow=Mainwindow;
}

void socexplorerproxy::loadSysDriver(const QString name)
{
    if(!_self)init();
    if(pluginloader::libcanberoot(name))
    {
        socexplorerplugin* driver = pluginloader::newsocexplorerplugin(name);
        QString driverName = _self->getinstanceName(driver->baseName());
        loadSysDriver(driver,driverName,name);
    }
}

void socexplorerproxy::loadSysDriver(const QString name, const QString instanceName)
{
    if(!_self)init();
    if(pluginloader::libcanberoot(name) && !_self->instanceExists(instanceName))
    {
        socexplorerplugin* driver = pluginloader::newsocexplorerplugin(name);
        loadSysDriver(driver,instanceName,name);
    }
}


void socexplorerproxy::loadSysDriver(socexplorerplugin *driver, const QString instanceName, const QString path)
{
    if(!_self)init();
    driver->setInstanceName(instanceName);
    driver->parent = NULL;
    drivers->append(driver);
    linearDriverList->append(driver);
    if(path.isEmpty())
        linearDriverPathList.append(driver->baseName());
    else
        linearDriverPathList.append(path);
    connectChildToProxy(driver);
    emit _self->addPluginGUI(driver);
    emit _self->clearMenu();
    emit _self->treeChanged(QVector<socexplorerplugin*>(*drivers));
    SocExplorerEngine::addSOC(driver);
    driver->postInstantiationTrigger();
}

void socexplorerproxy::loadChildSysDriver(socexplorerplugin *parent, const QString child)
{
    if(!_self)init();
    if(pluginloader::libcanbechild(child))
    {
        socexplorerplugin* driver = pluginloader::newsocexplorerplugin(child);
        QString driverName = _self->getinstanceName(driver->baseName());
        bool ok=true;
        if(ok)
        {
            if(parent!=NULL)_self->loadSysDriverToParent(driver,parent,driverName,child);
        }
    }
}

void socexplorerproxy::loadSysDriverToParent(const QString name,const QString ParentInst)
{
    if(!_self)init();
    if(pluginloader::libcanbechild(name))
    {
        socexplorerplugin* driver = pluginloader::newsocexplorerplugin(name);
        QString driverName = _self->getinstanceName(driver->baseName());
        bool ok=true;
        if(ok)
        {
            socexplorerplugin* parent=_self->getSysDriver(ParentInst);
            if(parent!=NULL)loadSysDriverToParent(driver,parent,driverName,name);
        }
    }
}


void socexplorerproxy::loadSysDriverToParent(const QString name,const QString instanceName,const QString ParentInst)
{
    if(!_self)init();
    if(pluginloader::libcanbechild(name) && !_self->instanceExists(instanceName))
    {
        socexplorerplugin* driver = pluginloader::newsocexplorerplugin(name);
        bool ok=true;
        if(ok)
        {
            socexplorerplugin* parent=_self->getSysDriver(ParentInst);
            if(parent!=NULL)loadSysDriverToParent(driver,parent,instanceName,name);
        }
    }
}

void socexplorerproxy::loadSysDriverToParent(socexplorerplugin *driver,socexplorerplugin *parent, const QString instanceName,const QString path)
{
    if(!_self)init();
    linearDriverList->append(driver);
    if(path.isEmpty())
        linearDriverPathList.append(driver->baseName());
    else
        linearDriverPathList.append(path);
    driver->parent = parent;
    driver->setInstanceName(instanceName);
    parent->childs.append(driver);
    connectChildToProxy(driver);
    connectChildToParent(parent,driver);
    emit _self->clearMenu();
    emit _self->addPluginGUI(driver);
    emit _self->treeChanged(QVector<socexplorerplugin*>(*drivers));
    driver->postInstantiationTrigger();
}


void socexplorerproxy::changeSysDriverInstName(const QString instanceName)
{
    Q_UNUSED(instanceName)
}

void socexplorerproxy::changeSysDriverInstName(const QString newinstanceName, const QString previnstanceName)
{
    if(!_self)init();
    socexplorerplugin*temp=_self->getSysDriver(previnstanceName);
    if(temp!=NULL)
    {
        if(NULL!=_self->getSysDriver(newinstanceName))
        {
            emit _self->treeChanged(QVector<socexplorerplugin*>(*drivers));
            return;
        }
        temp->setInstanceName(newinstanceName);
    }
    emit _self->treeChanged(QVector<socexplorerplugin*>(*drivers));
}


void socexplorerproxy::connectChildToParent(socexplorerplugin *parent, socexplorerplugin *child)
{
    if(!_self)init();
    connect(parent,SIGNAL(activateSig(bool)),child,SLOT(activate(bool)));
    child->activate(parent->isConnected());
}

void socexplorerproxy::disconnectChildToParent(socexplorerplugin *child)
{
    if(!_self)init();
    disconnect(child->parent,SIGNAL(activateSig(bool)),child,SLOT(activate(bool)));
}


void socexplorerproxy::connectChildToProxy(socexplorerplugin *child)
{
    if(!_self)init();
    connect(child,SIGNAL(registerObject(QObject*,QString)),_self,SIGNAL(registerObject(QObject*,QString)));
    connect(child,SIGNAL(closePlugin(socexplorerplugin*)),_self,SLOT(closeSysDriver(socexplorerplugin*)));
}

void socexplorerproxy::disconnectChildToProxy(socexplorerplugin *child)
{
    if(!_self)init();
    disconnect(child,SIGNAL(registerObject(QObject*,QString)),_self,SIGNAL(registerObject(QObject*,QString)));
    disconnect(child,SIGNAL(closePlugin(socexplorerplugin*)),_self,SLOT(closeSysDriverFromDriver(socexplorerplugin*)));
}

QString socexplorerproxy::getinstanceName(const QString& baseName)
{
    if(!_self)init();
    int i=0;
    QString name;
    bool validName = false;
    while(!validName)
    {
        name.clear();
        name.append(baseName+QString::number(i));
        validName = instanceNameIsValid(name);
        i++;
    }
    return name;
}

void socexplorerproxy::changeSysDriverInstName(socexplorerplugin* driver)
{
    if(!_self)init();
    QString prevName(driver->instanceName());
}

bool socexplorerproxy::instanceNameIsValid(const QString& instanceName)
{
    if(!_self)init();
    for(int k=0;k<linearDriverList->count();k++)
    {
        if(!linearDriverList->at(k)->instanceName().compare(instanceName))
            return false;
    }
    return true;
}

socexplorerplugin *socexplorerproxy::findPlugin(const QString &instanceName)
{
    if(!_self)init();
    for(int k=0;k<linearDriverList->count();k++)
    {
        if(!linearDriverList->at(k)->instanceName().compare(instanceName))
            return linearDriverList->at(k);
    }
    return NULL;
}

QStringList socexplorerproxy::getPluginsList()
{
    if(!_self)init();
    QStringList result;
    socexplorerplugin* plugin;
    for(int i=0; i<linearDriverList->count();i++)
    {
        QString parent="";
        plugin=linearDriverList->at(i);
        if(plugin->parent)parent=plugin->parent->instanceName();
        result.append(linearDriverList->at(i)->instanceName()+":"+parent+":"+linearDriverPathList.at(i));
    }
    return result;
}

bool socexplorerproxy::loadPluginsList(QStringList plugins)
{
    if(!_self)init();
    QString plugin;
    int lastLoadedPlugins=-1;
    QStringList loadedPlugins;
    while(plugins.count() && lastLoadedPlugins!=loadedPlugins.count())
    {
        lastLoadedPlugins=loadedPlugins.count();
        for (int i = 0; i < plugins.count(); i++)
        {
            plugin=plugins[i];
            QStringList args=plugin.split(':',QString::KeepEmptyParts);
            if(args.count()==3)
            {
                if(args[1].isEmpty())
                {
                    _self->loadSysDriver(args[2],args[0]);
                    loadedPlugins.append(args[0]);
                    plugins.removeAt(i);
                }
                else if(loadedPlugins.contains(args[1]))
                {
                    _self->loadSysDriverToParent(args[2],args[0],args[1]);
                    loadedPlugins.append(args[0]);
                    plugins.removeAt(i);
                }
            }
            else
            {
                plugins.removeAt(i);
            }
        }
    }
}

bool socexplorerproxy::instanceExists(const QString &instanceName)
{
    return !socexplorerproxy::instanceNameIsValid(instanceName);
}

void socexplorerproxy::close()
{
    if(!_self)init();
    socexplorerplugin* tmpPtr;
    while(drivers->count()>0)
    {
        tmpPtr = drivers->last();
        drivers->removeLast();
        _self->closeSysDriver(tmpPtr);
    }
    if(root!=NULL)
    {
        _self->closeSysDriver(root);
    }

}

socexplorerplugin* socexplorerproxy::getSysDriver(const QString instanceName)
{
    if(!_self)init();
    for(int i=0;i<linearDriverList->count();i++)
    {
        if(!linearDriverList->at(i)->instanceName().compare(instanceName))
            return linearDriverList->at(i);
    }
    return NULL;
}


void socexplorerproxy::closeSysDriver(const QString instanceName)
{
    if(!_self)init();
    closeSysDriver(getSysDriver(instanceName),false);
}

void socexplorerproxy::closeSysDriver(socexplorerplugin *driver, bool recursive)
{
    if(!_self)init();
    if(driver!=NULL)
    {
        emit _self->removePluginGUI(driver);
        if(driver->parent==NULL)SocExplorerEngine::removeSOC(driver);
        while(driver->childs.count()!=0)closeSysDriver(driver->childs.first());
        linearDriverPathList.removeAt(linearDriverList->indexOf(driver));
        linearDriverList->removeOne(driver);
        if(driver->parent!= NULL)
        {
            driver->parent->childs.removeOne(driver);     //Have parent so it's a child
            disconnectChildToParent(driver);
            disconnectChildToProxy(driver);
            delete driver;
        }
        else
        {
            drivers->removeOne(driver);
            disconnectChildToProxy(driver);
            delete driver;
        }
        if(!recursive)
        {
            emit _self->clearMenu();
            emit _self->registermenu(mainWindow);            
            emit _self->treeChanged(QVector<socexplorerplugin*>(*drivers));
        }
    }

}

void socexplorerproxy::geteplugintree()
{
    if(!_self)init();
    emit _self->treeChanged(QVector<socexplorerplugin*>(*drivers));
}

void socexplorerproxy::closeSysDriverFromDriver(socexplorerplugin *driver)
{
    if(!_self)init();
    emit _self->closeSysDriverSig(driver);
}

void socexplorerproxy::updateText()
{
    if(!_self)init();
    emit _self->clearMenu();
    emit _self->registermenu(mainWindow);
}





void socexplorerproxy::makeMenu(QMenu* menu)
{
    if(!_self)init();
    for(int i=0;i<drivers->count();i++)
    {
        drivers->at(i)->registermenu(menu);
    }
}


