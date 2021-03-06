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
#ifndef MEMTESTER_H
#define MEMTESTER_H
#include <isocexplorerplugin.h>
#include <stdint.h>
#include <memory>
#include <functional>
#include <address.h>

class MemTester
{
    template<unsigned int bits=32>
    static bool checkSpace(std::function<unsigned int(unsigned int* Value, int count, address64_t address)>Read,
                           address64_t startAddress, address64_t endAddress)
    {
        unsigned int Value;
        offset64_t shift{bits/8};
        auto address=startAddress;
        while(address <= endAddress)
        {
            Value=!address;
            Read(&Value, 1, address);
            if(Value!=address.value)
                return false;
            shift = shift<<1;
            address = startAddress + shift;
        };
        return true;
    }

    MemTester(){}
    ~MemTester(){}

public:

    template<unsigned int bits=32>
    static std::size_t measureMemSize(std::function<unsigned int(unsigned int* Value, int count, address64_t address)>Write,
                                      std::function<unsigned int(unsigned int* Value, int count, address64_t address)>Read,
                                      address64_t address,unsigned int maxSize=0xFFFFFFFF
            )
    {
        unsigned int curVal=address.value;
        address64_t testAddress=address;
        offset64_t shift{bits/8};
        while(testAddress>=address)
        {
            Write(&curVal,1,testAddress);
            if(!checkSpace<bits>(Read,address,testAddress))
                break;
            testAddress = address + shift;
            curVal = testAddress.value;
            shift = shift<<1;
        }
        return (testAddress-address).value;
    }
};

#endif // MEMTESTER_H
