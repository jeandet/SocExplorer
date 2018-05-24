/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2014, Plasma Physics Laboratory - CNRS
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
--                     Mail : alexis.jeandet@member.fsf.org
----------------------------------------------------------------------------*/

#ifndef SOCCLK_H
#define SOCCLK_H
#include <QString>
#include <QList>

class SOCclk
{
public:
    SOCclk(const QString& name, qint64 value, qint64 divisor, qint64 multiplier);
    void appendChild(SOCclk* child);
    void setParentClk(SOCclk* parent);
    SOCclk* getClk(const QString& name);

    qint64 Value();
    void setValue(qint64 value);
    qint64 Divisor();
    void setDivisor(qint64 divisor);
    qint64 Multiplier();
    void setMultiplier(qint64 multiplier);
    QString Name();
    void setName(const QString& name);
private:
    qint64 p_Value;
    QString p_Name;
    qint64 p_Divisor;
    qint64 p_Multiplier;
    QList<SOCclk*> p_childrenClk;
    SOCclk* p_parentClk;
};


#endif // SOCCLK_H
