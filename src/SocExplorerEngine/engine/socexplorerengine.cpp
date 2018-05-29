/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2012, Plasma Physics Laboratory - CNRS
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
#include "socexplorerengine.h"
#include <proxy/socexplorerproxy.h>
#include <socexplorersettings.h>
#include <socexplorercoresettingsgui.h>
#include <socexplorerconfigkeys.h>

SocExplorerEngine* SocExplorerEngine::_self = NULL;
socExplorerXmlModel* SocExplorerEngine::p_xmlmodel=NULL;
QMainWindow* SocExplorerEngine::mainWindow=NULL;
QList<SOCModel*>* SocExplorerEngine::SOCs=NULL;
QSettings* SocExplorerEngine::m_settings=NULL;
int SocExplorerEngine::loglvl=1;


SocExplorerEngine::SocExplorerEngine(QObject *parent) :
    QObject(parent)
{
    if(SOCs==NULL)
    {
        SOCs = new QList<SOCModel*>;
    }
    m_settings = new QSettings();
    SocExplorerCoreSettingsGUI* cfggui=new SocExplorerCoreSettingsGUI();
    SocExplorerSettings::registerConfigEntry(cfggui,QIcon(":/images/config.svg"),"SocExplorer Core");
}


void SocExplorerEngine::init()
{
    QDir dir;
    if(!_self)
    {
        _self= new SocExplorerEngine;
    }
    if(!dir.exists(configFolder()))
        dir.mkdir(configFolder());
    p_xmlmodel = new socExplorerXmlModel(_self);
    p_xmlmodel->updateSOClist();
}

QString SocExplorerEngine::configFolder()
{
    return QString(SOCEXPLORER_CONFIG_PATH);
}

QStringList SocExplorerEngine::pluginFolders()
{
    if(!_self)
        init();
    QStringList folders;
    QDir pluginFolders(QString(SOCEXPLORER_CONFIG_PATH)+"/plugin.conf.d");
    if(pluginFolders.exists())
    {
        pluginFolders.setFilter(QDir::Files | QDir::NoSymLinks);
        QFileInfoList list = pluginFolders.entryInfoList();
        for (int i = 0; i < list.size(); ++i)
        {
            QFileInfo fileInfo = list.at(i);
            if(fileInfo.suffix()=="conf")
            {
                QFile confFile(fileInfo.absoluteFilePath());
                if(confFile.open(QIODevice::ReadOnly))
                {
                    while (!confFile.atEnd())
                    {
                        QString line = confFile.readLine();
                        QDir plugDir(line.remove("\n"));
                        if(plugDir.exists() && !folders.contains(plugDir.absolutePath()))
                        {
                            folders.append(plugDir.absolutePath());
                        }
                    }
                }
            }
        }
    }
    QStringList localCfg = SocExplorerSettings::value(SOCEXPLORERENGINE_SETTINGS_SCOPE,SOCEXPLORERENGINE_SETTINGS_PLUGINS_LOOKUP_PATH).toString().split(";");
    QString dir;
    foreach (dir, localCfg)
    {
        QDir plugDir(dir);
        if(plugDir.exists())
            folders.append(dir);
    }
    return folders;
}

SOCModel *SocExplorerEngine::plugin2Soc(socexplorerplugin *plugin)
{
    if(!_self)
        init();
    if(plugin)
    {
        while (plugin->parent!=NULL) {
            plugin = plugin->parent;
        }
        for(int i=0;i<SOCs->count();i++)
        {
            if(SOCs->at(i)->isRootDev(plugin))
                return SOCs->at(i);
        }
        //no soc found so create a new one
        SOCModel* soc=new SOCModel(plugin);
        SOCs->append(soc);
        return soc;
    }
    return NULL;
}


int SocExplorerEngine::addEnumDevice(socexplorerplugin* rootPlugin,int VID, int PID, qint32 baseAddress, const QString &name)
{
    if(!_self)
        init();
    SOCModel* soc = plugin2Soc(rootPlugin);
    if(soc && !soc->enumDeviceExists(baseAddress))
    {
        emit _self->enumDeviceAdded(soc->addEnumDevice(VID,PID,baseAddress,name));
        return 1;
    }
    return 0;
}

QList<SOCModel *> *SocExplorerEngine::getSOCs()
{
    if(!_self)
        init();
    return SOCs;
}

qint32 SocExplorerEngine::getEnumDeviceBaseAddress(const QString& rootPlugin,int VID, int PID, int count)
{
    socexplorerplugin* plugin = socexplorerproxy::findPlugin(rootPlugin);
    if(plugin==NULL)return -1;
    SOCModel* soc = plugin2Soc(plugin);
    if(soc==NULL)
        return -1;
    return soc->getEnumDeviceBaseAddress(VID,PID,count);
}

qint32 SocExplorerEngine::getEnumDeviceBaseAddress(socexplorerplugin *plugin, int VID, int PID, int count)
{
    if(plugin==NULL)return -1;
    SOCModel* soc = plugin2Soc(plugin);
    if(soc==NULL)
        return -1;
    return soc->getEnumDeviceBaseAddress(VID,PID,count);
}

qint32 SocExplorerEngine::getEnumDeviceCount(socexplorerplugin *plugin, int VID, int PID)
{
    if(plugin==NULL)return 0;
    SOCModel* soc = plugin2Soc(plugin);
    if(soc==NULL)
        return 0;
    return soc->getEnumDeviceCount(VID,PID);
}

qint32 SocExplorerEngine::getEnumDeviceCount(const QString &rootPlugin, int VID, int PID)
{
    socexplorerplugin* plugin = socexplorerproxy::findPlugin(rootPlugin);
    if(plugin==NULL)return 0;
    SOCModel* soc = plugin2Soc(plugin);
    if(soc==NULL)
        return 0;
    return soc->getEnumDeviceCount(VID,PID);
}

int SocExplorerEngine::addEnumDevice(const QString &rootPlugin, int VID, int PID, qint32 baseAddress, const QString &name)
{
    socexplorerplugin* plugin = socexplorerproxy::findPlugin(rootPlugin);
    if(plugin==NULL)return -1;
    SOCModel* soc = plugin2Soc(plugin);
    if(soc==NULL)
        return -1;
    soc->addEnumDevice(VID,PID,baseAddress,name);
    return  1;
}

unsigned int SocExplorerEngine::memMeasureSize(socexplorerplugin *plugin, unsigned int address, unsigned int maxSize)
{
  //return MemTester::measureMemSize(plugin,address,maxSize);
}

unsigned int SocExplorerEngine::memMeasureSize(const QString &plugin, unsigned int address, unsigned int maxSize)
{
  //return MemTester::measureMemSize(plugin,address,maxSize);
}


QString SocExplorerEngine::getDevName(int VID, int PID)
{
    QList<QDomNodeList> list=p_xmlmodel->getAllNodes("peripheral");
    for(int i=0;i<list.count();i++)
    {
        QDomNodeList nodes=list.at(i);
        for(int l=0;l<nodes.count();l++)
        {
            QDomElement node=nodes.at(l).toElement();
            int nodeVID=node.attribute("vid","0").toInt();
            int nodePID=node.attribute("pid","0").toInt();
            if((nodeVID==VID)&&(nodePID==PID))
            {
                return node.attribute("name","Unknow device");
            }
        }
    }
    return QString("Unknow device");
}

QString SocExplorerEngine::SocExplorerVersion(){return QString(SOCEXPLORER_VERSION);}

QString SocExplorerEngine::SocExplorerChangeset(){return QString(SOCEXPLORER_CHAGESET).split(" ").at(0);}

QString SocExplorerEngine::SocExplorerBranch(){return QString(SOCEXPLORER_BRANCH);}

socExplorerXmlModel *SocExplorerEngine::xmlModel()
{
    if(!_self)
        init();
    return p_xmlmodel;
}

void SocExplorerEngine::setMainWindow(QMainWindow *Mainwindow)
{
    if(!_self)
        init();
    mainWindow=Mainwindow;
}

QProgressBar *SocExplorerEngine::getProgressBar(const QString& format, int max)
{
    if(!_self)
        init();
    QProgressBar* progressBar;
    if(mainWindow!=NULL)
    {
        progressBar = new QProgressBar(mainWindow);
        mainWindow->statusBar()->addWidget(progressBar);
    }
    else
    {
        progressBar = new QProgressBar();
    }
    progressBar->setMaximum(max);
    progressBar->setFormat(format);
    return progressBar;
}

void SocExplorerEngine::deleteProgressBar(QProgressBar *progressBar)
{
    if(mainWindow!=NULL)
    {
        mainWindow->statusBar()->removeWidget(progressBar);
    }
    delete progressBar;
}

void SocExplorerEngine::addSOC(socexplorerplugin *rootPlugin)
{
    plugin2Soc(rootPlugin);
}

void SocExplorerEngine::removeSOC(socexplorerplugin *rootPlugin)
{
    SOCModel* soc=plugin2Soc(rootPlugin);
    SOCs->removeAll(soc);
    delete soc;
}


void SocExplorerEngine::message(socexplorerplugin *sender, const QString &message, int debugLevel)
{
    if(!_self)
        init();
     SocExplorerEngine::message(sender->instanceName(),message,debugLevel);
}

void SocExplorerEngine::message(QObject *sender, const QString &message, int debugLevel)
{
    if(!_self)
        init();
     SocExplorerEngine::message(sender->objectName(),message,debugLevel);
}

void SocExplorerEngine::message(const QString &sender, const QString &message, int debugLevel)
{
    // TODO add multi output message manager IE also log in files
    static QTextStream SocExplorerEngineStdout(stdout);
    if(!_self)
        init();
    if(loglvl>=debugLevel)
        SocExplorerEngineStdout << QTime::currentTime().toString()+" " + sender+":"+message << endl;
}

void SocExplorerEngine::setLogLevel(int level)
{
    if(!_self)
        init();
    printf("Set log level to %d\n",level);
    loglvl = level;
}

bool SocExplorerEngine::isSocLitleEndian(socexplorerplugin *plugin)
{
  return plugin2Soc(plugin)->isLitleEndian();
}



