/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2015, Plasma Physics Laboratory - CNRS
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
#include "socexplorersettings.h"

SocExplorerSettings* SocExplorerSettings::_self=NULL;
QSettings* SocExplorerSettings::m_settings=NULL;
QSettings* SocExplorerSettings::m_sessionSettings=NULL;
SocExplorerSettingsDialog* SocExplorerSettings::m_configDialog = NULL;
#include <socexplorergui.h>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#define INIT() \
    if(Q_UNLIKELY(_self==NULL))\
{\
    init();\
    }


SocExplorerSettings::SocExplorerSettings(QObject *parent) : QObject(parent)
{
    m_settings = new QSettings();
    m_configDialog = new SocExplorerSettingsDialog();
    QAction* trigerGUI = new QAction(tr("Settings"),this);
    connect(trigerGUI,SIGNAL(triggered()),this,SLOT(popConfigDialog()));
    SocExplorerGUI::addSettingsAction(trigerGUI);
}

SocExplorerSettings::~SocExplorerSettings()
{
    if(m_settings)
    {
        m_settings->sync();
        delete m_settings;
    }
    if(m_sessionSettings)
    {
        m_sessionSettings->sync();
        delete m_sessionSettings;
    }
}

void SocExplorerSettings::init()
{
    if(!_self)
    {
        _self= new SocExplorerSettings();
    }
}

void SocExplorerSettings::setValue(QObject *object, const QString &key, const QVariant &value, SettingScope Sscope)
{
    INIT();
    setValue(object->metaObject()->className(),key,value,Sscope);
}

QVariant SocExplorerSettings::value(QObject *object, const QString &key, const QVariant &defaultValue, SettingScope Sscope)
{
    INIT();
    return value(object->metaObject()->className(),key,defaultValue,Sscope);
}

void SocExplorerSettings::setValue(const QString scope, const QString &key, const QVariant &value, SettingScope Sscope)
{
    INIT();
    switch (Sscope)
    {
    case SystemWide:
        if(m_settings)
            m_settings->setValue(scope+"/"+key,value);
        break;
    case Session:
        if(m_sessionSettings)
            m_sessionSettings->setValue(scope+"/"+key,value);
        break;
    default:
        break;
    }
}

QVariant SocExplorerSettings::value(const QString scope, const QString &key, const QVariant &defaultValue, SettingScope Sscope)
{
    INIT();
    switch (Sscope)
    {
    case SystemWide:
        if(m_settings)
            return m_settings->value(scope+"/"+key,defaultValue);
        break;
    case Session:
        if(m_sessionSettings)
            return m_sessionSettings->value(scope+"/"+key,defaultValue);
        break;
    default:
        break;
    }
    return defaultValue;
}

QList<QList<QVariant> > SocExplorerSettings::arrays(const QString &prefix, QStringList keys, SocExplorerSettings::SettingScope Sscope)
{
    INIT();
    QList<QList<QVariant> > defaultValue;
    switch (Sscope)
    {
    case SystemWide:
        if(m_settings)
            return arrays(prefix,keys,m_settings);
        break;
    case Session:
        if(m_sessionSettings)
            return arrays(prefix,keys,m_sessionSettings);
        break;
    default:
        break;
    }
    return defaultValue;
}

void SocExplorerSettings::setArrays(const QString &prefix, QStringList keys, QList<QList<QVariant> > values, SocExplorerSettings::SettingScope Sscope)
{
    INIT();
    switch (Sscope)
    {
    case SystemWide:
        if(m_settings)
            return setArrays(prefix,keys,values,m_settings);
        break;
    case Session:
        if(m_sessionSettings)
            return setArrays(prefix,keys,values,m_sessionSettings);
        break;
    default:
        break;
    }

}

void SocExplorerSettings::sync()
{
    INIT();
    if(m_settings)
    {
        m_settings->sync();
    }
    if(m_sessionSettings)
    {
        m_sessionSettings->sync();
    }
}

bool SocExplorerSettings::registerConfigEntry(SocExplorerSettingsItem *configEntry, QIcon icon, QString text)
{
    INIT();
    return m_configDialog->registerConfigEntry(configEntry,icon,text);
}

bool SocExplorerSettings::loadSession(const QString &session)
{
    INIT();
    QFileInfo sessionInfo(m_settings->fileName());
    QString fullpath=sessionInfo.absoluteDir().absolutePath() +"/"+session+".conf";
    if(m_sessionSettings)
    {
        delete m_sessionSettings;
        m_sessionSettings = NULL;
    }
    m_sessionSettings = new QSettings(fullpath,QSettings::NativeFormat,self());
    qDebug()<< m_sessionSettings->fileName();
    if(m_sessionSettings->status()==QSettings::NoError)
    {
        return true;
    }
    delete m_sessionSettings;
    m_sessionSettings = NULL;
    return false;
}

bool SocExplorerSettings::renameSession(const QString &session, const QString &newName)
{
    INIT();
    sync();
    QFileInfo sessionInfo(m_settings->fileName());
    QString fullpath=sessionInfo.absoluteDir().absolutePath() +"/"+session+".conf";
    QString newFullpath=sessionInfo.absoluteDir().absolutePath() +"/"+newName+".conf";
    if(m_sessionSettings && m_sessionSettings->fileName()==fullpath)
    {
        delete m_sessionSettings;
        QFile::rename(fullpath,newFullpath);
        m_sessionSettings = new QSettings(newFullpath,QSettings::NativeFormat,self());
    }
    else
    {
        QFile::rename(fullpath,newFullpath);
    }
    return true;
}

bool SocExplorerSettings::deleteSession()
{
    INIT();
    if(m_sessionSettings)
    {
        m_sessionSettings->clear();
        QString filename= m_sessionSettings->fileName();
        if(QFile::exists(filename))
        {
            delete m_sessionSettings;
            QFile::remove(filename);
        }
        else
            delete m_sessionSettings;
        m_sessionSettings = NULL;
        return true;
    }
    return false;
}

bool SocExplorerSettings::deleteSession(const QString &session)
{
    QFileInfo sessionInfo(m_settings->fileName());
    QString fullpath=sessionInfo.absoluteDir().absolutePath() +"/"+session+".conf";
    if(m_sessionSettings)
    {
        if(m_sessionSettings->fileName()==fullpath)
        {
            deleteSession();
            return true;
        }
    }
    QSettings* sessionSettings = new QSettings(fullpath,QSettings::NativeFormat,self());
    if(sessionSettings)
    {
        if(sessionSettings->status()==QSettings::NoError)
        {
                sessionSettings->clear();
                QString filename= m_sessionSettings->fileName();
                if(QFile::exists(filename))
                {
                    delete sessionSettings;
                    QFile::remove(filename);
                }
                else
                    delete sessionSettings;
                return true;
        }
        delete sessionSettings;
    }
    return false;
}

void SocExplorerSettings::popConfigDialog()
{
    m_configDialog->popConfigDialog(NULL);
}

QList<QList<QVariant> > SocExplorerSettings::arrays(const QString &prefix, QStringList keys, QSettings *settings)
{
    QList<QList<QVariant> > result;
    if(settings)
    {
        int size = settings->beginReadArray(prefix);
        for (int i = 0; i < size; ++i)
        {
            result.append(QList<QVariant>());
            settings->setArrayIndex(i);
            for(int l=0;l<keys.count();l++)
            {
                result[i].append(settings->value(keys.at(l)));
            }
        }
        settings->endArray();
    }
    return result;
}

void SocExplorerSettings::setArrays(const QString &prefix, QStringList keys, QList<QList<QVariant> > values, QSettings *settings)
{
    if(settings)
    {
        QString key;
        foreach (key, keys)
        {

            settings->remove(prefix+"/"+key);
        }
        settings->beginWriteArray(prefix);
        for (int i = 0; i < values.size(); ++i)
        {
            settings->setArrayIndex(i);
            for(int l=0;l<keys.count();l++)
            {
                settings->setValue(keys[l], values.at(i).at(l));
            }
        }
        settings->endArray();

    }
}

