#include "screenlist.h"

#include <QCursor>
#include <QPainter>
#include <QRect>
#include <QApplication>

ScreenList::ScreenList(QList<ScreenInfo> list)
{
    this->m_List = list;
    this->initParams();
}

QRect ScreenList::allBoundary(bool isGlobal) const
{
    if(isGlobal == true)
        return m_allBoundary;
    else
        return QRect(0,0,m_allBoundary.width(),m_allBoundary.height());
}

QPixmap ScreenList::allPixMap() const
{
    return m_allPixMap;
}

QRect ScreenList::boundaryAt(int screenIndex,bool isGlobal)
{
    if(isGlobal == true)
        return m_List[screenIndex].boundary;
    else
        return toLocal(m_List[screenIndex].boundary);
}

QScreen *ScreenList::screenAt(int screenIndex)
{
    return m_List[screenIndex].object;
}

QPixmap ScreenList::pixmapAt(int screenIndex)
{
    return m_List[screenIndex].pixmap;
}

int ScreenList::indexAtMouse()
{
    QPoint mousePos = QCursor::pos();
    int mCount = m_List.count();

    for(int i=0;i<mCount;i++)
    {
        if(m_List[i].boundary.contains(mousePos))
            return i;
    }

    return 0;
}

void ScreenList::initParams()
{
    int minX = 0;
    int minY = 0;
    int maxX = 0;
    int maxY = 0;

    if(m_List.size() >0)
    {
        minX = m_List[0].boundary.x();
        minY = m_List[0].boundary.y();

        maxX = minX + m_List[0].boundary.width();
        maxY = minY + m_List[0].boundary.height();
    }

    for(int i=1;i<m_List.size();i++)
    {
        ScreenInfo info = m_List[i];

        minX = qMin(minX,info.boundary.x());
        maxX = qMax(maxX,info.boundary.x()+info.boundary.width());

        minY = qMin(minY,info.boundary.y());
        maxY = qMax(maxY,info.boundary.y()+info.boundary.height());
    }

    m_allBoundary.setRect(minX,minY, maxX-minX,maxY-minY);

    float pixelRetio = 1;
    if(m_List.count() == 1)
    {
        pixelRetio = m_List.at(0).object->devicePixelRatio();
    }

    m_allPixMap = *new QPixmap(m_allBoundary.width()*pixelRetio,m_allBoundary.height()*pixelRetio);
    QRect i_allBoundary(m_allBoundary.x() * pixelRetio,
                        m_allBoundary.y() * pixelRetio,
                        m_allBoundary.width() * pixelRetio,
                        m_allBoundary.height() * pixelRetio);

    QPainter painter(&m_allPixMap);

    for(ScreenInfo info:m_List)
    {
        QRect mRect(info.boundary.x() * pixelRetio,
                       info.boundary.y() * pixelRetio,
                       info.boundary.width() * pixelRetio,
                       info.boundary.height() * pixelRetio);
        QPixmap mPixmap = info.pixmap;

        QRect rRect(mRect.x()-i_allBoundary.x(),mRect.y()-i_allBoundary.y(),mRect.width(),mRect.height());

        painter.drawPixmap(rRect,mPixmap);
    }
}

QRect ScreenList::toLocal(QRect rect)
{
    return QRect(rect.x()-m_allBoundary.x(),rect.y()-m_allBoundary.y(),rect.width(),rect.height());
}

//float ScreenList::scaleAt(int screenIndex)
//{
//    QScreen *screen = screenAt(screenIndex);

//    float refDpi = 120.;

//    float dpi = screen->logicalDotsPerInch();

//    float v = dpi/refDpi;

//    return v;
//}

float ScreenList::scale()
{
    QScreen *screen = m_List.size() == 1 ? m_List[0].object : QApplication::primaryScreen();
    float dpi = screen->logicalDotsPerInch();

    float pixel120ToMM = 1.0/96.0 * 2.54 * 10.0;
    float MMToDpiPixel = 1.0/2.54*dpi/10.0;

    return pixel120ToMM * MMToDpiPixel * screen->devicePixelRatio();

    //return 1.0;
}

void ScreenList::draw(QPainter &painter)
{
    painter.drawPixmap(allBoundary(),m_allPixMap);
}