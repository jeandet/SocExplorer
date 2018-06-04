#include "qsvgicon.h"

QSvgIcon::QSvgIcon()
    :QIcon()
{

}


QSvgIcon::QSvgIcon(const QString &fileName)
    :QIcon()
{
    this->addFile(fileName);
}


void QSvgIcon::addFile(const QString &fileName, const QSize &size, Mode mode, State state)
{
    Q_UNUSED(size)
    Q_UNUSED(mode)
    Q_UNUSED(state)
    QSvgRenderer* render= new QSvgRenderer(fileName);
    if(render->isValid())
    {
        QPixmap* img= new QPixmap(render->defaultSize());
        img->fill(Qt::transparent);
        QPainter* pntr= new QPainter(img);
        render->render(pntr);
        pntr->end();
        this->addPixmap(*img);
        delete render;
        delete img;
        delete pntr;
    }
}






