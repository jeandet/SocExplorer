/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
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
#include "pluginlist.h"
//#include "pluginloader.h"
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QString>
#include <QStringList>
#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <qsvgicon.h>
#include <QWidget>
#include <QDrag>
#include <SocExplorerCore.h>
#include <SOC/SOC.h>

PluginList::PluginList(QWidget *parent):QListWidget(parent)
{
    this->setAcceptDrops(true);
    this->setDragEnabled(true);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(this,&PluginList::itemSelectionChanged, this, &PluginList::updateSelection);
    this->refreshPluginList();
}


void PluginList::refreshPluginList()
{
    this->clear();
    auto pluginList = SocExplorerCore::pluginManager().getPluginList();
    for(const auto& plugin:pluginList)
    {
        auto item = new PluginListItem(plugin,"");
        item->setFlags(item->flags() &~ Qt::ItemIsEditable);
        this->addItem(item);
    }
}

void PluginList::dragEnterEvent(QDragEnterEvent *event)
{
    this->clearSelection();
    this->itemAt(dragStartPosition)->setSelected(true);
    event->acceptProposedAction();
}

void PluginList::dragMoveEvent(QDragMoveEvent *e)
{
    e->accept();
}

void PluginList::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragStartPosition = event->pos();
    }
    QListWidget::mousePressEvent(event);
}

void PluginList::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength()< QApplication::startDragDistance())
        return;
    if(this->selectedItems().count()==0)
        return;
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    drag->setHotSpot(event->pos() - rect().topLeft());
    mimeData->setData("socexplorer/pluginName",QByteArray(((PluginListItem*)this->selectedItems().first())->text().toLatin1()));
    mimeData->setText(((PluginListItem*)this->selectedItems().first())->text().toLatin1());
    drag->setMimeData(mimeData);
    drag->exec(Qt::MoveAction);
}



void PluginList::dropEvent(QDropEvent *event)
{
    Q_UNUSED(event)
}



void PluginList::updateSelection()
{
    QStringList _items;
    for(int i=0;i<this->selectedItems().count();i++)
    {
        _items << ((PluginListItem*)(this->selectedItems().at(i)))->text();
    }
    emit this->itemSelectionChanged(_items);
}
