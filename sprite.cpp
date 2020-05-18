#include "sprite.h"

Sprite::Sprite(QPixmap& pixmap, int num_frame, qreal angle)
{
    // Инициализация переменных
    m_iNumFrames = num_frame;
    m_Pixmap = MyTransform(pixmap, angle);
    m_rcPosition.setRect(0, 0, pixmap.width()/m_iNumFrames, pixmap.height());
    m_ptVelocity.setX(0);
    m_ptVelocity.setY(0);
    m_currFrame = 0.0f;
    m_bOneFrame = false;
}

Sprite::Sprite(const QPixmap& pixmap, int num_frame, qreal angle)
{
    // Инициализация переменных
    m_iNumFrames = num_frame;
    m_Pixmap = MyTransform(pixmap, angle);
    m_rcPosition.setRect(0, 0, pixmap.width()/m_iNumFrames, pixmap.height());
    m_ptVelocity.setX(0);
    m_ptVelocity.setY(0);
    m_currFrame = 0.0f;
    m_bOneFrame = false;
}

void Sprite::SetPosition(QRect& rcPosition)
{
    m_rcPosition.moveTo(rcPosition.left(), rcPosition.top());
}

void Sprite::SetPosition(QPoint ptPosition)
{
    m_rcPosition.moveTo(ptPosition);
}

void Sprite::SetPosition(int x, int y)
{
    m_rcPosition.moveTo(x,y);
}

void Sprite::SetVelocity(int x, int y)
{
    m_ptVelocity.setX(x);
    m_ptVelocity.setY(y);
}

void Sprite::SetVelocity(QPoint ptVelocity)
{
    m_ptVelocity.setX(ptVelocity.x());
    m_ptVelocity.setY(ptVelocity.y());
}

void Sprite::Draw(QPainter* painter)
{
    //painter->drawPixmap(m_rcPosition.left(), m_rcPosition.top(), m_Pixmap);
    painter->drawTiledPixmap(m_rcPosition.left(), m_rcPosition.top(),
                             m_Pixmap.width()/m_iNumFrames,m_Pixmap.height(),
                             m_Pixmap,
                             (m_Pixmap.width()/m_iNumFrames)*int(m_currFrame),0);
}

void Sprite::Draw(QPainter* painter, QPixmap& pixmap, qreal angle)
{
    painter->drawPixmap(m_rcPosition.left(), m_rcPosition.top(), MyTransform(pixmap, angle));
}

QPixmap Sprite::MyTransform(QPixmap& pixmap, qreal angle)
{
    QTransform trnsf;
    return pixmap.transformed(trnsf.rotate(angle));
}

QPixmap Sprite::MyTransform(const QPixmap& pixmap, qreal angle)
{
    QTransform trnsf;
    return pixmap.transformed(trnsf.rotate(angle));
}

void Sprite::SetNumFrames(int iNumFrames)
{
    m_iNumFrames = iNumFrames;


    QRect rect = GetPosition();
    rect.setRight(rect.left() + ((rect.right()) - rect.left()) / iNumFrames);
    SetPosition(rect);
}

void Sprite::UpdateFrame()
{
    m_currFrame+=0.8f;

    if(m_currFrame > m_iNumFrames)
    {
        m_currFrame-=m_iNumFrames;
    }
}
