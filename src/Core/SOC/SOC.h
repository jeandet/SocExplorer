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
#ifndef SOC_H
#define SOC_H
#include <isocexplorerplugin.h>
#include <memory>
#include <vector>
#include "pluginloaderV2/pluginmanager.h"

class SOC
{
    std::vector<std::shared_ptr<ISocexplorerPlugin>> plugins;
    std::vector<std::shared_ptr<ISocexplorerPlugin>> root_plugins;

    QString name;
    SOC(const QString& name)
        :name(name)
    {

    }
public:
    static SOC& instance()
    {
        static SOC inst("SOC");
        return inst;
    }

    bool loadPlugin(const QString& name)
    {

    }
};
#endif //SOC_H
