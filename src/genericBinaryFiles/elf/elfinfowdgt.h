/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2013, Plasma Physics Laboratory - CNRS
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 3 of the License, or
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
#ifndef ELFINFOWDGT_H
#define ELFINFOWDGT_H

#if QT_VERSION >= 0x050000
    #include <QtWidgets/QWidget>
    #include <QtWidgets/QVBoxLayout>
    #include <QtWidgets/QHBoxLayout>
    #include <QtWidgets/QLabel>
    #include <QtWidgets/QTextEdit>
    #include <QtWidgets/QTableWidget>
    #include <QtWidgets/QSplitter>
#else
    #include <QWidget>
    #include <QVBoxLayout>
    #include <QHBoxLayout>
    #include <QLabel>
    #include <QTextEdit>
    #include <QTableWidget>
    #include <QSplitter>
#endif

#include "elfparser.h"
#include <qhexedit.h>

class elfInfoWdgt : public QWidget
{
    Q_OBJECT
public:
    explicit elfInfoWdgt(QWidget *parent = 0);
    
signals:
    

public slots:
    void updateInfo(elfparser* parser);
    void cellActivated ( int row, int column );

private:
    void updateSectionsTable(elfparser* parser);
    void updateSegmentsTable(elfparser* parser);
    QVBoxLayout* mainLayout;
    QLabel* ElfArchitecture;
    QLabel* ElfType;
    QLabel* ElfVersion;
    QTableWidget* segmentsListTableWdgt,*sectionsListTableWdgt;
    QHexEdit* hexViewer;
    QSplitter* splitter;
    QWidget* elfInfoWdgtLay;
    QVBoxLayout* elfInfoWdgtLayout;
    elfparser* parser;

};

#endif // ELFINFOWDGT_H
