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
#ifndef PLUGINLIST_H
#define PLUGINLIST_H
#include <QTableWidget>
#include <QListWidget>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragLeaveEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>
#include <QtCore/QMimeData>
#include <QtCore/QUrl>
#include <QtCore/QList>
#include <QPoint>
#include <QStringList>
#include <QMenu>
#include <QAction>
//#include <pluginloader.h>

class PluginListItem: public QListWidgetItem
{
public:
    explicit PluginListItem( const QString & Name,const QString & fullPath, QListWidget * parent = 0, int type = Type )
        :QListWidgetItem(Name,parent,type),fullPath(fullPath)
    {
    }
    QString fullPath;
};


class PluginList: public QListWidget
{
    Q_OBJECT
public:
    explicit PluginList(QWidget * parent = Q_NULLPTR);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *e);

public slots:
    void refreshPluginList();
private slots:
    void updateSelection();
signals:
    void itemSelectionChanged(const QStringList& items);


private:
    QPoint dragStartPosition;
};

#endif // PLUGINLIST_H
