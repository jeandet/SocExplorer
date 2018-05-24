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

#include "pluginmanager.h"
#include <QDir>
#include <QLibrary>
#include <QPluginLoader>
#include <socexplorerengine.h>

void PluginManager::loadPluginMetaData(const QString &pluginPath)
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

bool PluginManager::isPlugin(const QString &pluginPath)const
{
    if(!this->plugins_metada_cache.contains(pluginPath))
    {
        if (QLibrary::isLibrary(pluginPath))
        {
            QPluginLoader pluginLoader{pluginPath};
            if(pluginLoader.isLoaded())
            {
                return true;
            }
        }
    }
    return false;
}


PluginManager::PluginManager(const QStringList &folderList)
    :folderList(folderList)
{
    scanFolders();
}

ISocexplorerPlugin *PluginManager::makeInstance(const QString& pluginName)
{
    auto plugin = resolvePluginName(pluginName);
    if (QLibrary::isLibrary(plugin))
    {
        QPluginLoader pluginLoader{plugin};
        if (auto pluginInstance = qobject_cast<ISocexplorerPlugin *>(pluginLoader.instance())) {
            return pluginInstance;
        }
    }
    return Q_NULLPTR;
}


void PluginManager::scanFolders()
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
            SocExplorerEngine::message("pluginloader::scanFolders","Checking "+ path,3);
            if(isPlugin(path))
            {
                loadPluginMetaData(path);
                auto name = plugins_metada_cache[path][pluginKeys::Name];
                pluginTable[name].append(path);
            }
        }
    }
}
