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
#include "genericbinaryfilewidget.h"
#include "ui_genericbinaryfilewidget.h"
#include <QFileDialog>
#include <QFile>
#include "srec/srecfile.h"
#include "srec/srecfilewidget.h"
#include "BinFile/binaryfile.h"
#include "BinFile/binaryfilewidget.h"
#include "elf/elffile.h"
#include "elf/elffilewidget.h"


genericBinaryFileWidget::genericBinaryFileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::genericBinaryFileWidget)
{
    ui->setupUi(this);
    ui->showFileListQpb->setOrientation(Qt::Vertical);
    ui->showFileListWdgt->setHidden(true);
    connect(this->ui->openFileQpb,SIGNAL(clicked()),this,SLOT(openFile()));
    connect(this->ui->removeFileQpb,SIGNAL(clicked()),this,SLOT(removeFiles()));
    connect(this->ui->fileList,SIGNAL(cellActivated(int,int)),this,SLOT(fileCellActivated(int,int)));
    connect(this->ui->fileList,SIGNAL(openFiles(QStringList)),this,SLOT(openFile(QStringList)));
    connect(this->ui->hideFileListQpb,SIGNAL(clicked()),this,SLOT(hideFileList()));
    connect(this->ui->showFileListQpb,SIGNAL(clicked()),this,SLOT(showFileList()));

}

genericBinaryFileWidget::~genericBinaryFileWidget()
{
    delete ui;
}

void genericBinaryFileWidget::openFile()
{
    QStringList filesNames = QFileDialog::getOpenFileNames(
                this,
                "Select one or more files to open",
                NULL,
                "Binary Files (*.bin);;SREC Files (*.srec);;Elf Files (*)");

    openFile(filesNames);
}

void genericBinaryFileWidget::openFile(const QStringList &FileList)
{
    for(int i=0;i<FileList.count();i++)
    {
        bool fileOpened = false;
        for(int l=0;l<files.count();l++)
        {
            if(FileList.at(i)==files.at(l)->fileName)
            {
                fileOpened = true;
            }
        }
        QFile file(FileList.at(i));
        if(!fileOpened && file.open(QIODevice::ReadOnly))
        {
            char magic[4];
            file.read(magic,4);
            QString line;
            switch((int)magic[0])
            {
            case 0x7F:
                if((magic[1]=='E') && (magic[2]=='L') && (magic[3]=='F'))
                {
                    files.append(new FileListElement(FileList.at(i),false,Type_Elf,NULL,NULL));
                }
                break;
            case 'S':
                file.seek(0);
                line = file.readLine();
                if(srecFile::checkSum(line))
                {
                    files.append(new FileListElement(FileList.at(i),false,Type_SREC,NULL,NULL));
                }
                break;
            default:
                files.append(new FileListElement(FileList.at(i),false,Type_Bin,NULL,NULL));
                break;
            }
        }
    }
    updateFileList();
}

void genericBinaryFileWidget::updateFileList()
{
    this->ui->fileList->clear();
    this->ui->fileList->setRowCount(files.count());
    this->ui->fileList->setHorizontalHeaderLabels(QStringList()<<"File"<<"Type");
    for(int i=0;i<files.count();i++)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(files.at(i)->fileName);
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->fileList->setItem(i, 0, newItem);

        newItem = new QTableWidgetItem(files.at(i)->type());
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        this->ui->fileList->setItem(i, 1, newItem);
    }
    this->ui->fileList->resizeColumnsToContents();
}

void genericBinaryFileWidget::removeFiles()
{
    QList<QTableWidgetItem*> items = this->ui->fileList->selectedItems();
    for(int i=0;i<items.count();i++)
    {
        QString filename = this->ui->fileList->item(items.at(i)->row(),0)->text();
        for(int l=0;l<files.count();l++)
        {
            if(files.at(l)->fileName==filename)
            {
                if(files.at(l)->isOpened)
                {
                    for(int m=0;m<this->ui->fileViewerTab->count();m++)
                    {
                        if(this->ui->fileViewerTab->widget(m)==this->files.at(l)->viewer)
                        {
                            this->ui->fileViewerTab->removeTab(m);
                        }
                    }
                    delete this->files.at(l)->viewer;
                    delete this->files.at(l)->parser;
                }
                delete files.at(l);
                files.removeAt(l);
            }
        }
    }
    updateFileList();
}

void genericBinaryFileWidget::fileCellActivated(int row, int column)
{
    Q_UNUSED(column)
    QString fileName = this->ui->fileList->item(row,0)->text();
    if(fileName!="")
    {
        for(int l=0;l<files.count();l++)
        {
            FileListElement* file = files.at(l);
            if(file->fileName==fileName)
            {
               if(!file->isOpened)
               {
                    if(file->parser==NULL)
                    {
                        switch (file->FileType)
                        {
                        case Type_Bin:
                            file->parser = new binaryFile(file->fileName);
                            if(file->viewer==NULL)
                            {
                                file->viewer = new binaryFileWidget();
                                file->viewer->setFile(file->parser);
                                this->ui->fileViewerTab->addTab(file->viewer,file->fileName);
                            }
                            file->isOpened = true;
                            break;
                        case Type_Elf:
                            file->parser = new ElfFile(file->fileName);
                            if(file->viewer==NULL)
                            {
                                file->viewer = new elfFileWidget();
                                file->viewer->setFile(file->parser);
                                this->ui->fileViewerTab->addTab(file->viewer,file->fileName);
                            }
                            file->isOpened = true;
                            break;
                        case Type_SREC:
                            file->parser = new srecFile(file->fileName);
                            if(file->viewer==NULL)
                            {
                                file->viewer = new srecFileWidget();
                                file->viewer->setFile(file->parser);
                                this->ui->fileViewerTab->addTab(file->viewer,file->fileName);
                            }
                            file->isOpened = true;
                            break;
                        default:
                            break;
                        }
                    }
               }
            }
        }
    }
}

void genericBinaryFileWidget::hideFileList()
{
    this->ui->fileListGBox->setHidden(true);
    this->ui->showFileListWdgt->setVisible(true);
}

void genericBinaryFileWidget::showFileList()
{
    this->ui->fileListGBox->setVisible(true);
    this->ui->showFileListWdgt->setHidden(true);
}




