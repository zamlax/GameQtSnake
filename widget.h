#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QBitmap>
#include <QKeyEvent>

#include <QVector>


namespace Ui {
class Widget;
}

class Sprite;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    void GameInit();
    void DrawToTurn(QPainter* painter, int i, int a_end, int a_angle);
    void DrawToDir(QPainter* painter, int i, int a_end, int a_mid);
    void DrawTextPause(QPainter &p);
    int Random(int low, int high);
    void DrawScore(QPainter& p);
    void DrawOst(QPainter& p);
    void DrawTime(QPainter& p);
private:
    Ui::Widget *ui;
    //Переменные для значений размера окна
    int m_width_window;
    int m_height_window;
    //Переменные для создания сетки(поля) игры
    int m_col;
    int m_row;
    int m_size_cell;

    //Переменная для картинки набора тайлов.
    QPixmap m_pixmap_tileset;
    QPixmap m_pixmap_background;

    QPixmap m_pixmap_head;
    QPixmap m_pixmap_mid_tail;
    QPixmap m_pixmap_angle_tail;
    QPixmap m_pixmap_end_tail;
    QPixmap m_pixmap_blood;
    QPixmap m_pixmap_blood2;
    QPixmap m_pixmap_wall;

    //Змея состоит из элементов:
    //Голова. 16х16 px
    Sprite* m_pSprite_head;
    //Хвост. 16х16 px
    //Хвости будет расти на один элемент,
    //по мере поедания еды, головой змеи.
    Sprite* m_pSprite_tail;
    //Еда змеи. Яблоко.
    Sprite* m_pSprite_apple;
    Sprite* m_pSprite_blood;

    Sprite* m_pSprite_apple2;
    Sprite* m_pSprite_blood2;
    //Созданные элементы змеи добавляются в контейнер
    //который моделирует змею
    QVector<Sprite*> snake;

    int timer_id;
    //Переменная для проверки быстрого нажатия клавишь
    //которое приводит к неправильному движению змеи
    bool m_test_key;

    bool m_tl;
    bool m_tr;
    bool m_bl;
    bool m_br;

    bool m_newItem;
    bool m_pause;
    unsigned int score_;
    unsigned int ost;
};

#endif // WIDGET_H
