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

#include "elfinfowdgt.h"

elfInfoWdgt::elfInfoWdgt(QWidget *parent) :
    QWidget(parent)
{
    this->mainLayout = new QVBoxLayout;
    this->ElfArchitecture= new QLabel;
    this->ElfType = new QLabel;
    this->ElfVersion = new QLabel;
    this->splitter = new QSplitter;
    this->hexViewer  = new QHexEdit;
    this->elfInfoWdgtLay = new QWidget;
    this->elfInfoWdgtLayout = new QVBoxLayout;
    this->segmentsListTableWdgt = new QTableWidget;
    this->segmentsListTableWdgt->setColumnCount(7);
    this->segmentsListTableWdgt->setHorizontalHeaderLabels(QStringList()<<"Type"<<"Offset"<<"Vaddr"<<"Paddr"<<"File Size"<<"Mem Size"<<"Flags");
    this->sectionsListTableWdgt = new QTableWidget;
    this->sectionsListTableWdgt->setColumnCount(2);
    this->sectionsListTableWdgt->setHorizontalHeaderLabels(QStringList()<<"Name"<<"Type");
    this->ElfArchitecture->setText(QString("Architecture: "));
    this->ElfType->setText(QString("Type: "));
    this->ElfVersion->setText(QString("Version: "));
    this->elfInfoWdgtLayout->addWidget(this->ElfArchitecture);
    this->elfInfoWdgtLayout->addWidget(this->ElfType);
    this->elfInfoWdgtLayout->addWidget(this->ElfVersion);
    this->elfInfoWdgtLayout->addWidget(this->segmentsListTableWdgt);
    this->elfInfoWdgtLayout->addWidget(this->sectionsListTableWdgt);
    this->splitter->addWidget(this->hexViewer);
    this->splitter->addWidget(this->elfInfoWdgtLay);
    this->elfInfoWdgtLay->setLayout(this->elfInfoWdgtLayout);
    this->mainLayout->addWidget(this->splitter);
    this->setLayout(this->mainLayout);
    connect(this->segmentsListTableWdgt,SIGNAL(cellActivated(int,int)),this,SLOT(cellActivated(int,int)));
    connect(this->sectionsListTableWdgt,SIGNAL(cellActivated(int,int)),this,SLOT(cellActivated(int,int)));
}


void elfInfoWdgt::updateInfo(elfparser *parser)
{
    if(parser!=NULL)
    {
        this->ElfArchitecture->setText(QString("Architecture: ")+parser->getArchitecture());
        this->ElfType->setText(QString("Type: ")+parser->getType());
        this->ElfVersion->setText(QString("Version: 0x")+QString::number(parser->getVersion(),16));
        this->updateSectionsTable(parser);
        this->updateSegmentsTable(parser);
    }

}

void elfInfoWdgt::cellActivated(int row, int column)
{
    char* buff=NULL;
    this->parser->getSectionData(row,&buff);
    this->hexViewer->setData(QByteArray(buff,this->parser->getSectionDatasz(row)));
}


void elfInfoWdgt::updateSectionsTable(elfparser* parser)
{
    if(parser!=NULL)
    {
        this->parser = parser;
        QTableWidgetItem *item;
        for(int i=0 ;i<this->sectionsListTableWdgt->rowCount();i++)
        {
            for(int l=0;l<this->sectionsListTableWdgt->columnCount();l++)
            {
                item = this->sectionsListTableWdgt->item(i,l);
                delete item;
            }
        }
        this->sectionsListTableWdgt->setRowCount(parser->getSectioncount());
        for(int i=0;i<parser->getSectioncount();i++)
        {
            item = new QTableWidgetItem(parser->getSectionName(i));
            item->setFlags(item->flags() &~ Qt::ItemIsEditable);
            this->sectionsListTableWdgt->setItem(i,0,item);
            item = new QTableWidgetItem(parser->getSectionType(i));
            item->setFlags(item->flags() &~ Qt::ItemIsEditable);
            this->sectionsListTableWdgt->setItem(i,1,item);
        }
        this->sectionsListTableWdgt->resizeColumnsToContents();
        this->sectionsListTableWdgt->resizeRowsToContents();
    }
}


void elfInfoWdgt::updateSegmentsTable(elfparser* parser)
{
    if(parser!=NULL)
    {
        this->parser = parser;
        QTableWidgetItem *item;
        for(int i=0 ;i<this->segmentsListTableWdgt->rowCount();i++)
        {
            for(int l=0;l<this->segmentsListTableWdgt->columnCount();l++)
            {
                item = this->segmentsListTableWdgt->item(i,l);
                delete item;
            }
        }
        this->segmentsListTableWdgt->setRowCount(parser->getSegmentcount());
        for(int i=0;i<parser->getSegmentcount();i++)
        {
            item = new QTableWidgetItem(parser->getSegmentType(i));
            item->setFlags(item->flags() &~ Qt::ItemIsEditable);
            this->segmentsListTableWdgt->setItem(i,0,item);
            item = new QTableWidgetItem("0x"+QString::number(parser->getSegmentOffset(i),16));
            item->setFlags(item->flags() &~ Qt::ItemIsEditable);
            this->segmentsListTableWdgt->setItem(i,1,item);
            item = new QTableWidgetItem("0x"+QString::number(parser->getSegmentVaddr(i),16));
            item->setFlags(item->flags() &~ Qt::ItemIsEditable);
            this->segmentsListTableWdgt->setItem(i,2,item);
            item = new QTableWidgetItem("0x"+QString::number(parser->getSegmentPaddr(i),16));
            item->setFlags(item->flags() &~ Qt::ItemIsEditable);
            this->segmentsListTableWdgt->setItem(i,3,item);
            item = new QTableWidgetItem("0x"+QString::number(parser->getSegmentFilesz(i),16));
            item->setFlags(item->flags() &~ Qt::ItemIsEditable);
            this->segmentsListTableWdgt->setItem(i,4,item);
            item = new QTableWidgetItem("0x"+QString::number(parser->getSegmentMemsz(i),16));
            item->setFlags(item->flags() &~ Qt::ItemIsEditable);
            this->segmentsListTableWdgt->setItem(i,5,item);
            item = new QTableWidgetItem(parser->getSegmentFlags(i));
            item->setFlags(item->flags() &~ Qt::ItemIsEditable);
            this->segmentsListTableWdgt->setItem(i,6,item);
        }
        this->segmentsListTableWdgt->resizeColumnsToContents();
        this->segmentsListTableWdgt->resizeRowsToContents();
    }
}











