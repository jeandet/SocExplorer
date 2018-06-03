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
#ifndef GENERICBINARYFILEWIDGET_H
#define GENERICBINARYFILEWIDGET_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <abstractbinfile.h>

namespace Ui {
class genericBinaryFileWidget;
}

typedef enum {Type_SREC,Type_Bin,Type_Elf}FileTypeEnum;
class FileListElement
{
public:
    FileListElement() {}
    FileListElement(QString fileName,bool isOpened,FileTypeEnum FileType,abstractBinFileWidget* viewer=0,abstractBinFile* parser=0)
        :fileName(fileName),isOpened(isOpened),FileType(FileType),viewer(viewer),parser(parser){}
    QString type()
    {
        switch (this->FileType) {
        case Type_SREC:
            return "Srec";
            break;
        case Type_Bin:
            return "Binary";
            break;
        case Type_Elf:
            return "Elf";
            break;
        default:
            return "Unknow";
            break;
        }
    }
    QString fileName;
    bool isOpened;
    FileTypeEnum FileType;
    abstractBinFileWidget* viewer;
    abstractBinFile* parser;
};

class genericBinaryFileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit genericBinaryFileWidget(QWidget *parent = 0);
    ~genericBinaryFileWidget();

public slots:
    void openFile();
    void openFile(const QStringList& FileList);
    void updateFileList();
    void removeFiles();
    void fileCellActivated(int row, int column);
    void hideFileList();
    void showFileList();

private:
    Ui::genericBinaryFileWidget *ui;
    QList<FileListElement*> files;
};

#endif // GENERICBINARYFILEWIDGET_H
