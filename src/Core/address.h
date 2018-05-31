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
#ifndef ADDRESS_H
#define ADDRESS_H
#include <opaque/numeric_typedef.hpp>
#include <stdint.h>

/**
 * Taken from https://sourceforge.net/p/opaque-typedef/wiki/Home/
 * This exactly what we want
 */

template <typename T>
struct offset : opaque::numeric_typedef<T, offset<T>> {
  using base  = opaque::numeric_typedef<T, offset<T>>;
  using base::base;
};

template <typename T>
struct address : opaque::numeric_typedef_base<T, address<T>>
  , opaque::binop::addable     <address<T>, true , address<T>,  offset<T>>
  , opaque::binop::addable     <address<T>, true ,  offset<T>, address<T>>
  , opaque::binop::subtractable<address<T>, false, address<T>,  offset<T>>
  , opaque::binop::subtractable<offset<T> , false, address<T>, address<T>, T, T>
{
  using base   = opaque::numeric_typedef_base<T, address<T>>;
  using base::base;
  address& operator*=(const address&) = delete;
  address& operator+=(const address&) = delete;
  address& operator-=(const address&) = delete;
  address& operator+=(const offset<T>& o) {
    this->value += o.value;
    return *this;
  }
  address& operator-=(const offset<T>& o) {
    this->value -= o.value;
    return *this;
  }
};

using offset64_t = offset<uint64_t>;
using address64_t = address<uint64_t>;

#endif //ADDRESS_H
