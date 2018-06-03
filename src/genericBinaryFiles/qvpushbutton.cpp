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
#include "qvpushbutton.h"
#include <QStylePainter>
#include <QMenu>

QVPushButton::QVPushButton(QWidget *parent) :
    QPushButton(parent)
{
    init();
}

QVPushButton::QVPushButton(const QString &text, QWidget *parent)
{
    init();
}

QVPushButton::QVPushButton(const QIcon &icon, const QString &text, QWidget *parent):
    QPushButton(parent)
{
    init();
}

QVPushButton::~QVPushButton()
{

}

Qt::Orientation QVPushButton::orientation() const
{
    return orientation_;
}

void QVPushButton::setOrientation(Qt::Orientation orientation)
{
    orientation_ = orientation;
    switch (orientation)
    {
    case Qt::Horizontal:
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        break;

    case Qt::Vertical:
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        break;
    }
}

bool QVPushButton::mirrored() const
{
    return mirrored_;
}

void QVPushButton::setMirrored(bool mirrored)
{
    mirrored_ = mirrored;
}

QSize QVPushButton::sizeHint() const
{
    QSize size = QPushButton::sizeHint();
    if (orientation_ == Qt::Vertical)
        size.transpose();
    return size;
}

void QVPushButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStylePainter p(this);

    switch (orientation_)
    {
    case Qt::Horizontal:
        if (mirrored_)
        {
            p.rotate(180);
            p.translate(-width(), -height());
        }
        break;

    case Qt::Vertical:
        if (mirrored_)
        {
            p.rotate(-90);
            p.translate(-height(), 0);
        }
        else
        {
            p.rotate(90);
            p.translate(0, -width());
        }
        break;
    }

    p.drawControl(QStyle::CE_PushButton, getStyleOption());
}

QStyleOptionButton QVPushButton::getStyleOption() const
{
    QStyleOptionButton opt;
    opt.initFrom(this);
    if (orientation_ == Qt::Vertical)
    {
        QSize size = opt.rect.size();
        size.transpose();
        opt.rect.setSize(size);
    }
    opt.features = QStyleOptionButton::None;
    if (isFlat())
        opt.features |= QStyleOptionButton::Flat;
    if (menu())
        opt.features |= QStyleOptionButton::HasMenu;
    if (autoDefault() || isDefault())
        opt.features |= QStyleOptionButton::AutoDefaultButton;
    if (isDefault())
        opt.features |= QStyleOptionButton::DefaultButton;
    if (isDown() || (menu() && menu()->isVisible()))
        opt.state |= QStyle::State_Sunken;
    if (isChecked())
        opt.state |= QStyle::State_On;
    if (!isFlat() && !isDown())
        opt.state |= QStyle::State_Raised;
    opt.text = text();
    opt.icon = icon();
    opt.iconSize = iconSize();
    return opt;
}

void QVPushButton::init()
{
    orientation_ = Qt::Horizontal;
    mirrored_ = false;
}
