#ifndef SPRITE_H
#define SPRITE_H

#include <QtCore>
//Подключаем заголовочные файлы
#include <QPixmap>
#include <QPainter>

class Sprite
{
public:
    Sprite(QPixmap& pixmap, int num_frame, qreal angle = 0);
    Sprite(const QPixmap& pixmap, int num_frame, qreal angle = 0);

    QRect& GetPosition(){ return m_rcPosition; }
    void SetPosition(QRect& rcPosition);
    void SetPosition(QPoint ptPosition);
    void SetPosition(int x, int y);

    QPoint GetVelocity() { return m_ptVelocity; }
    void SetVelocity(QPoint ptVelocity);
    void SetVelocity(int x, int y);

    void Draw(QPainter* painter);
    void Draw(QPainter* painter, QPixmap& pixmap, qreal angle = 0);

    QPixmap MyTransform(QPixmap& pixmap, qreal angle);
    QPixmap MyTransform(const QPixmap& pixmap, qreal angle);

    void SetNumFrames(int iNumFrames);
    void UpdateFrame();
    void SetOneFrame(bool one_frame) {m_bOneFrame = one_frame;}
    bool GetOneFrame() {return m_bOneFrame;}

    void SetFrameRate(float rate) {m_fFrameRate = rate;}

protected:
    QRect m_rcPosition;
    QPoint m_ptVelocity;
    QPixmap m_Pixmap;
    float m_currFrame;
    int m_iNumFrames;
    bool m_bOneFrame;
    float m_fFrameRate;
};

#endif // SPRITE_H
