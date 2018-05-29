/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2013, Plasma Physics Laboratory - CNRS
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
#ifndef SOCEXPLORER_H
#define SOCEXPLORER_H

#ifdef WIN32
#ifdef SOCEXPLORER_EXPORTS
#define SOCEXPLORER_EXPORT __declspec(dllexport)
#else
#define SOCEXPLORER_EXPORT __declspec(dllimport)
#endif
#else
#define SOCEXPLORER_EXPORT
#endif

#ifdef WIN32
#include <stdlib.h>
#define socexplorerBswap32(X) _byteswap_ulong(X)
#else
#ifdef UNIX
#include <byteswap.h>
#define socexplorerBswap32(X) bswap_32(X)
#endif
#endif


#ifdef WIN32
    #define SOCEXPLORER_PLUGINS_INSTALL_PATH qApp->applicationDirPath()+"/plugins"
    #define SOCEXPLORER_CONFIG_PATH qApp->applicationDirPath()+"/config"
    #define PYMODULES qApp->applicationDirPath()+"/python"
#else
    #ifdef UNIX
        #define SOCEXPLORER_PLUGINS_INSTALL_PATH QDir::homePath()+"/.SocExplorer/plugins"
        //#define SOCEXPLORER_CONFIG_PATH QDir::homePath()+"/.SocExplorer/config"
        #define PYMODULES "/etc/SocExplorer/python"
    #endif
#endif
#endif // SOCEXPLORER_H
