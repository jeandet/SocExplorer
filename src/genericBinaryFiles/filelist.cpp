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
#include "filelist.h"
#include <QMimeData>
#include <QUrl>

FileList::FileList(QWidget *parent) :
    QTableWidget(parent)
{
    setHorizontalHeaderLabels(QStringList()<<"File"<<"Type");
    this->setAcceptDrops(true);
}

void FileList::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void FileList::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void FileList::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();

        if (mimeData->hasUrls())
        {
            QStringList pathList;
            QList<QUrl> urlList = mimeData->urls();

            for (int i = 0; i < urlList.size() && i < 32; ++i)
            {
                pathList.append(urlList.at(i).toLocalFile());
            }
            emit openFiles(pathList);
            event->acceptProposedAction();
        }
}
