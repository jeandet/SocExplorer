/*------------------------------------------------------------------------------
--  This file is a part of the socexplorer Software
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
#ifndef SOCEXPLORERPLUGININTERFACE_H
#define SOCEXPLORERPLUGININTERFACE_H

#include "socexplorerplugininterface_global.h"
#include <QString>
#include <QtGlobal>
#include <QtPlugin>

typedef int pluginT;

typedef void* (*socexplorerpluginCreateObjectT)();
typedef pluginT (*socexplorerpluginTypeT)();
typedef  int (*socexplorerpluginpidT)();
typedef  int (*socexplorerpluginvidT)();
typedef  int (*socexplorerplugincanberootT)();
typedef  int (*socexplorerplugincanbechildT)();
typedef QString (*socexplorerpluginNameT)();
typedef QString (*socexplorerpluginVersionT)();
typedef QString (*socexplorerpluginAuthorT)();
typedef QString (*socexplorerpluginDescriptionT)();

extern "C"
{
#ifndef QT_ENSURE_STACK_ALIGNED_FOR_SSE
    #define QT_ENSURE_STACK_ALIGNED_FOR_SSE
#endif
//! \header This the C interface socexplorer uses to load dynamically any plugin, a C interface is the
//! easyest way to manually resolve functions in a dynamic library.

//! \fn socexplorerpluginCreateObject Plugin constructor.
    LIBSOCEXPLORERPLUGIN_EXPORT  void* socexplorerpluginCreateObject()QT_ENSURE_STACK_ALIGNED_FOR_SSE;
//! \fn socexplorerpluginpid Plugin's PID.
    LIBSOCEXPLORERPLUGIN_EXPORT  int socexplorerpluginpid()QT_ENSURE_STACK_ALIGNED_FOR_SSE;
//! \fn socexplorerpluginvid Plugin's VID.
    LIBSOCEXPLORERPLUGIN_EXPORT  int socexplorerpluginvid()QT_ENSURE_STACK_ALIGNED_FOR_SSE;
//! \fn socexplorerplugincanberoot Tells if the plugin can be a root plugin.
    LIBSOCEXPLORERPLUGIN_EXPORT  int socexplorerplugincanberoot()QT_ENSURE_STACK_ALIGNED_FOR_SSE;
//! \fn socexplorerplugincanberoot Tells if the plugin can be a child plugin.
    LIBSOCEXPLORERPLUGIN_EXPORT  int socexplorerplugincanbechild()QT_ENSURE_STACK_ALIGNED_FOR_SSE;
//! \fn socexplorerpluginType Gives the plugin's type, not used yet.
    LIBSOCEXPLORERPLUGIN_EXPORT  pluginT socexplorerpluginType()QT_ENSURE_STACK_ALIGNED_FOR_SSE;
//! \fn socexplorerpluginVersion Gives the plugin's version.
    LIBSOCEXPLORERPLUGIN_EXPORT  QString socexplorerpluginVersion()QT_ENSURE_STACK_ALIGNED_FOR_SSE;
//! \fn socexplorerpluginName Gives the plugin's name.
    LIBSOCEXPLORERPLUGIN_EXPORT  QString socexplorerpluginName()QT_ENSURE_STACK_ALIGNED_FOR_SSE;
//! \fn socexplorerpluginAuthor Gives the plugin's author.
    LIBSOCEXPLORERPLUGIN_EXPORT  QString socexplorerpluginAuthor()QT_ENSURE_STACK_ALIGNED_FOR_SSE;
//! \fn socexplorerpluginDescription Gives the plugin's description.
    LIBSOCEXPLORERPLUGIN_EXPORT  QString socexplorerpluginDescription()QT_ENSURE_STACK_ALIGNED_FOR_SSE;
}

#endif // SOCEXPLORERPLUGININTERFACE_H
