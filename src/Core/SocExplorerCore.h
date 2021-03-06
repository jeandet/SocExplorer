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
#ifndef SOCEXPLORERCORE_H
#define SOCEXPLORERCORE_H
#include <isocexplorerplugin.h>
#include <memory>
#include <vector>
#include "pluginloaderV2/pluginmanager.h"
#include "Logger/Logger.h"
#include <iostream>
#include <fstream>
#include <pluginloaderV2/pluginmanager.h>

class SocExplorerCore
{
    Logger _logger;
    PluginManager<SocExplorerCore> plugin_loader;

    SocExplorerCore()
        :_logger(&std::cout),plugin_loader({})
    {
    }
    ~SocExplorerCore(){}

    void p_message(const QString& sender, const QString& text, int level)
    {
        _logger.message(sender,text,level);
    }

public:
    static SocExplorerCore& instance()
    {
        static SocExplorerCore inst;
        return inst;
    }
    static void changeLoggerOutput(std::ostream* out)
    {
        SocExplorerCore::instance()._logger.changeOutput(out);
    }
    static void message(const QString& sender, const QString& text, int level)
    {
        SocExplorerCore::instance().p_message(sender,text,level);
    }

    inline static std::shared_ptr<ISocexplorerPlugin> makeInstance(const QString &pluginName)
    {
        return SocExplorerCore::instance().plugin_loader.makeInstance(pluginName);
    }

    inline static void addPluginLookupPath(const QString& path)
    {
        return SocExplorerCore::instance().plugin_loader.addPluginLookupPath(path, true);
    }

    inline static const PluginManager<SocExplorerCore>& pluginManager()
    {
        return SocExplorerCore::instance().plugin_loader;
    }
};

#endif //SOCEXPLORERCORE_H
