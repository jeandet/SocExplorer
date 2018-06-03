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
#ifndef SOCREGISTER_H
#define SOCREGISTER_H

#include <address.h>
#include <QString>

struct SOCRegister
{
    SOCRegister(const QString& name, address64_t address)
        :_name(name),_address(address)
    {}

    QString name()const {return _name;}
    address64_t address()const {return _address;}
private:
    QString _name;
    address64_t _address;
};

#endif //SOCREGISTER_H
