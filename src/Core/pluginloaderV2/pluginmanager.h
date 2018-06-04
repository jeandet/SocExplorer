/*------------------------------------------------------------------------------
███████╗ ██████╗  ██████╗    ███████╗██╗  ██╗██████╗ ██╗      ██████╗ ██████╗ ███████╗██████╗
██╔════╝██╔═══██╗██╔════╝    ██╔════╝╚██╗██╔╝██╔══██╗██║     ██╔═══██╗██╔══██╗██╔════╝██╔══██╗
███████╗██║   ██║██║         █████╗   ╚███╔╝ ██████╔╝██║     ██║   ██║██████╔╝█████╗  ██████╔╝
╚════██║██║   ██║██║         ██╔══╝   ██╔██╗ ██╔═══╝ ██║     ██║   ██║██╔══██╗██╔══╝  ██╔══██╗
███████║╚██████╔╝╚██████╗    ███████╗██╔╝ ██╗██║     ███████╗╚██████╔╝██║  ██║███████╗██║  ██║
╚══════╝ ╚═════╝  ╚═════╝    ╚══════╝╚═╝  ╚═╝╚═╝     ╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝

--  This file is a part of the SOC Explorer Software
--  Copyright (C) 2018, Plasma Physics Laboratory - CNRS
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
#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H
#include <isocexplorerplugin.h>
#include <QHash>
#include <QString>
#include <QStringList>
#include <iostream>
#include <QDir>
#include <QLibrary>
#include <QPluginLoader>
#include <memory>

namespace pluginKeys
{
    const auto Name=QStringLiteral("Name");
    const auto Author=QStringLiteral("Author");
    const auto Version=QStringLiteral("Version");
    const auto Description=QStringLiteral("Description");
    const auto CanBeRoot=QStringLiteral("root");
    const auto CanBeChild=QStringLiteral("child");
    const auto VID=QStringLiteral("VID");
    const auto PID=QStringLiteral("PID");

    const auto all={Name, Author, Version, Description, CanBeRoot, CanBeChild, VID, PID};
};

template<typename T>
static auto loadMetadata(const QString& name, const QString &pluginName,const QHash<QString, QHash<QString,QString>>& plugins_metada_cache);

template<>
auto loadMetadata<QString>(const QString& name, const QString &pluginName,const QHash<QString, QHash<QString,QString>>& plugins_metada_cache)
{
    if(plugins_metada_cache.contains(pluginName))
    {
        return plugins_metada_cache[pluginName][name];
    }
    return QString();
}

template<>
auto loadMetadata<int>(const QString& name, const QString &pluginName,const QHash<QString, QHash<QString,QString>>& plugins_metada_cache)
{
    if(plugins_metada_cache.contains(pluginName))
    {
        return plugins_metada_cache[pluginName][name].toInt();
    }
    return -1;
}

template<>
auto loadMetadata<bool>(const QString& name, const QString &pluginName,const QHash<QString, QHash<QString,QString>>& plugins_metada_cache)
{
    if(plugins_metada_cache.contains(pluginName))
    {
        return bool(plugins_metada_cache[pluginName][name].toInt());
    }
    return false;
}

template <class logger>
class PluginManager
{
    void loadPluginMetaData(const QString &pluginPath)
    {
        if(!this->plugins_metada_cache.contains(pluginPath))
        {
            if (QLibrary::isLibrary(pluginPath))
            {
                QPluginLoader pluginLoader{pluginPath};
                if(pluginLoader.isLoaded())
                {
                    auto metadata = pluginLoader.metaData().value("MetaData").toObject();
                    for (auto key:pluginKeys::all)
                    {
                        if(metadata.contains(key))
                            this->plugins_metada_cache[pluginPath][key] = metadata.value(key).toString();
                    }
                }

            }
        }
    }

    bool _isPlugin(const QString &pluginPath)const
    {
        if(!this->plugins_metada_cache.contains(pluginPath))
        {
            if (QLibrary::isLibrary(pluginPath))
            {
                QPluginLoader pluginLoader{pluginPath};
                if(pluginLoader.load())
                {
                    return true;
                }
            }
        }
        return false;
    }

    QString resolvePluginName(const QString &pluginName)const
    {
        if( plugins_metada_cache.contains(pluginName))
            return pluginName;
        else if (pluginTable.contains(pluginName)) {
            return pluginTable[pluginName].last();
        }
        return QString();
    }
public:
    PluginManager(const QStringList& folderList)
        :folderList(folderList)
    {
        scanFolders();
    }

    void addPluginLookupPath(const QString& path, bool update=false)
    {
        this->folderList.append(path);
        if(update)
            this->scanFolders();
    }
    std::shared_ptr<ISocexplorerPlugin> makeInstance(const QString &pluginName)
    {
        auto plugin = resolvePluginName(pluginName);
        if (QLibrary::isLibrary(plugin))
        {
            QPluginLoader pluginLoader{plugin};
            if (auto pluginInstance = qobject_cast<ISocexplorerPlugin *>(pluginLoader.instance())) {
                return std::make_shared<ISocexplorerPlugin>(pluginInstance);
            }
        }
        return Q_NULLPTR;
    }

#define METADATA_GETTER(type, name)\
    type plugin##name(const QString &pluginName)const{return loadMetadata<type>(pluginKeys::name, resolvePluginName(pluginName), plugins_metada_cache);}

    METADATA_GETTER(QString, Name)
    METADATA_GETTER(QString, Author)
    METADATA_GETTER(QString, Description)
    METADATA_GETTER(QString, Version)

    METADATA_GETTER(bool, CanBeRoot)
    METADATA_GETTER(bool, CanBeChild)

    METADATA_GETTER(int, VID)
    METADATA_GETTER(int, PID)

    void scanFolders()
    {
        QDir dir;
        QStringList filters{"*.so", "*.dll"};
        pluginTable.clear();
        for(auto&& folder:folderList)
        {
            dir.setPath(folder);
            dir.setFilter(QDir::Files);
            dir.setNameFilters(filters);
            QFileInfoList list = dir.entryInfoList();
            for (auto&& fileInfo:list)
            {
                auto path = fileInfo.filePath();
                logger::message("pluginloader::scanFolders","Checking "+ path,3);
                if(_isPlugin(path))
                {
                    loadPluginMetaData(path);
                    auto name = plugins_metada_cache[path][pluginKeys::Name];
                    pluginTable[name].append(path);
                }
            }
        }
    }

    bool isPlugin(const QString &pluginName)const
    {
        return this->_isPlugin(resolvePluginName(pluginName));
    }


    QStringList getPluginList() const
    {
        return this->pluginTable.keys();
    }

private:
    QHash<QString, QHash<QString,QString>> plugins_metada_cache;

    /** pluginTable maps plugin names and plugins DLL path
    this helps to get the list of DLLs which provides a given plugin
    idealy we should have only one DLL for each plugin name.
    **/
    QHash<QString, QStringList> pluginTable;

    QStringList folderList;
};

#endif // PLUGINMANAGER_H
