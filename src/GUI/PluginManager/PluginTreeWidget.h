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
#ifndef PLUGINTREE_H
#define PLUGINTREE_H

#include <QWidget>
#include <QTreeWidget>
#include <isocexplorerplugin.h>
#include <QList>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragLeaveEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>
#include <QtCore/QMimeData>
#include <QtCore/QUrl>
#include <QtCore/QList>
#include <QPoint>
#include <QLabel>

class PluginTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit PluginTreeWidget(QWidget *parent = Q_NULLPTR);
    ~PluginTreeWidget();
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
        void geteplugintree(void);
        void pluginselected(const QString& Name);
        void loadSysDriverToParent(const QString name, const QString instanceName);
        void loadSysDriver(const QString name);
        void closeSysDriver(const QString instanceName);
        void changeSysDriverInstName(const QString instanceName);
        void changeSysDriverInstName(const QString newinstanceName,const QString previnstanceName);

public  slots:
        void treeChanged(const QHash<QString,std::shared_ptr<ISocexplorerPlugin>>& tree);
private slots:
        void pluginselectedslt( QTreeWidgetItem * item, int column);
        void itemSelectionChangedslt();

private:
        void addplugin(const std::shared_ptr<ISocexplorerPlugin> &driver, QTreeWidgetItem* item);
        QTreeWidgetItem* editeditem;
        QString* editeditemprev;
        bool editingItem;
        int soc_callbak_id;
};

#endif // PLUGINTREE_H
