#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include "sprite.h"
#include <chrono>
#include <thread>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //инициализируем переменные сетки(поля)
    m_col = 40;//количество колонок
    m_row = 30;//колоичество строк
    m_size_cell = 16;//размер ячейки
    m_width_window = m_col * m_size_cell;//640 ширина окна
    m_height_window = m_row * m_size_cell;//480 высота окна
    resize(m_width_window,m_height_window);

    //инициализируем генератор случайных чисел
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    //загружаем файл
    m_pixmap_tileset.load("tileset.png");
    //m_pixmap_tileset.setMask(m_pixmap_tileset.createMaskFromColor(QColor(255,255,255)));
    m_pixmap_tileset.setMask(m_pixmap_tileset.createMaskFromColor(QColor(255,0,255)));
    //делаем фон картинки прозрачным
    m_pixmap_background = m_pixmap_tileset.copy(0,16,640,480);

    m_pixmap_blood.load("blood.png");
    m_pixmap_blood.setMask(m_pixmap_blood.createMaskFromColor(QColor(255,255,255)));

    m_pixmap_blood2.load("blood.png");
    m_pixmap_blood2.setMask(m_pixmap_blood2.createMaskFromColor(QColor(255,255,255)));

    m_pixmap_wall = m_pixmap_tileset.copy(130,0,16,16);

    GameInit();

    //запускаем таймер
    timer_id = startTimer(140);//140
    m_test_key = false;
}

Widget::~Widget()
{
    delete ui;
    killTimer(timer_id);
    for(int i = 0;i<snake.size();i++)
    {
        delete snake[i];
        snake[i] = 0;
    }

    snake.clear();
}


void Widget::GameInit()
{
    m_test_key = false;
    m_tl = false;
    m_tr = false;
    m_bl = false;
    m_br = false;
    m_newItem = false;

    //Элемент голова змеи
    m_pixmap_head = m_pixmap_tileset.copy(0,0,16,16);
    m_pSprite_head = new Sprite(m_pixmap_head, 1, 180);
    m_pSprite_head->SetPosition(m_width_window/2, m_height_window/2);
    m_pSprite_head->SetVelocity(16, 0);
    //Добавляем в контейнер
    snake.push_back(m_pSprite_head);

    //1 элемент тела змеи
    m_pixmap_mid_tail = m_pixmap_tileset.copy(16,0,16,16);
    m_pSprite_tail = new Sprite(m_pixmap_mid_tail, 1);
    m_pSprite_tail->SetPosition(m_width_window/2 - m_pSprite_tail->GetPosition().width(),
                               m_height_window/2);
    m_pSprite_tail->SetVelocity(16, 0);
    //Добавляем в контейнер
    snake.push_back(m_pSprite_tail);

    //2 элемент тела змеи
    m_pSprite_tail = new Sprite(m_pixmap_mid_tail, 1);
    m_pSprite_tail->SetPosition(m_width_window/2 - m_pSprite_tail->GetPosition().width()*2,
                               m_height_window/2);
    m_pSprite_tail->SetVelocity(16, 0);
    //Добавляем в контейнер
    snake.push_back(m_pSprite_tail);

    m_pixmap_angle_tail = m_pixmap_tileset.copy(16*2,0,16,16);
    m_pixmap_end_tail = m_pixmap_tileset.copy(16*3,0,16,16);

    //Создаем еду
    m_pSprite_apple = new Sprite(m_pixmap_tileset.copy(16*5,0,16*3,16), 3);
    //Еда появляется в случайно координате игрового поля.
    m_pSprite_apple->SetPosition(Random(2, m_col-2)*m_size_cell,
                                 Random(2, m_row-2)*m_size_cell);
    m_pSprite_apple->SetVelocity(0, 0);

    m_pSprite_blood = new Sprite(m_pixmap_blood.copy(0,0,40*5,0), 5);
    m_pSprite_blood->SetVelocity(0, 0);

    //Создаем еду
    m_pSprite_apple2 = new Sprite(m_pixmap_tileset.copy(16*5,0,16*3,16), 3);
    //Еда появляется в случайно координате игрового поля.
    m_pSprite_apple2->SetPosition(Random(2, m_col-2)*m_size_cell,
                                 Random(2, m_row-2)*m_size_cell);
    m_pSprite_apple2->SetVelocity(0, 0);

    m_pSprite_blood2 = new Sprite(m_pixmap_blood2.copy(0,0,40*5,0), 5);
    m_pSprite_blood2->SetVelocity(0, 0);

    score_ = 0;
    ost = 20;
}

int Widget::Random(int low, int high)
{
    return low + qrand() % ((high + 1) - low);
}

void Widget::timerEvent(QTimerEvent *e)
{
    if(timer_id == e->timerId())
    {
        if(m_pause == false)
        {
            //перемещение змеи
            for(int i = snake.size()-1;i > 0 ;i--)
            {
                snake[i]->SetPosition(snake[i-1]->GetPosition());
                snake[i]->SetVelocity(snake[i-1]->GetVelocity());
            }

            snake.front()->SetPosition(snake.front()->GetPosition().left() + snake.front()->GetVelocity().x(),
                    snake.front()->GetPosition().top() + snake.front()->GetVelocity().y());
            //делаем истина только после перемещения головы змеи
            m_test_key = true;
        }

        //Проверка условий выхода за границы экрана (окна)
        //если змея вышла за границы
        //то возвращаем её с другой стороны экрана (окна)
        //змея выходит за пределы игрового поля
        if(snake.front()->GetPosition().left() < m_size_cell)
        {
            snake.front()->SetPosition((m_width_window-m_size_cell) - snake.front()->GetPosition().width(),
                                       snake.front()->GetPosition().top());
        }
        else
        if(snake.front()->GetPosition().right() > m_width_window-m_size_cell)
            snake.front()->SetPosition(m_size_cell, snake.front()->GetPosition().top());

        if(snake.front()->GetPosition().bottom() > m_height_window-m_size_cell)
            snake.front()->SetPosition(snake.front()->GetPosition().left(), m_size_cell);
        else
        if(snake.front()->GetPosition().top() < m_size_cell)
        {
            snake.front()->SetPosition(snake.front()->GetPosition().left(),
                                       m_height_window-m_size_cell-snake.front()->GetPosition().height());
        }

        //змея ест яблоко
        for(int i = 0; i < snake.size(); i++)
        {
            //если яблоко попало на змею, считаем его проглоченым
            if(m_pSprite_apple->GetPosition().contains(snake[i]->GetPosition()))
            {
                //и растет
                m_newItem = true;
                m_pSprite_tail = new Sprite(m_pixmap_tileset.copy(16,0,16,16),1);
                m_pSprite_tail->SetPosition(snake[snake.size()-1]->GetPosition());
                m_pSprite_tail->SetVelocity(snake[snake.size()-1]->GetVelocity());
                snake.push_back(m_pSprite_tail);

                m_pSprite_blood->SetPosition(m_pSprite_apple->GetPosition());
                m_pSprite_blood->SetOneFrame(true);



                score_=score_+10;
                ost=ost-1;

                //перемещаем яблоко в случайную координату
                m_pSprite_apple->SetPosition(Random(2, m_col-2)*m_size_cell,
                                             Random(2, m_row-2)*m_size_cell);

                break;
            }
            if(m_pSprite_apple2->GetPosition().contains(snake[i]->GetPosition()))

            {
                //и растет
                m_newItem = true;
                m_pSprite_tail = new Sprite(m_pixmap_tileset.copy(16,0,16,16),1);
                m_pSprite_tail->SetPosition(snake[snake.size()-1]->GetPosition());
                m_pSprite_tail->SetVelocity(snake[snake.size()-1]->GetVelocity());
                snake.push_back(m_pSprite_tail);



                m_pSprite_blood2->SetPosition(m_pSprite_apple2->GetPosition());
                m_pSprite_blood2->SetOneFrame(true);

                score_=score_+10;
                ost=ost-1;

                //перемещаем яблоко в случайную координату
                m_pSprite_apple2->SetPosition(Random(2, m_col-2)*m_size_cell,
                                             Random(2, m_row-2)*m_size_cell);
                break;
            }
        }

        //змея кусает свой хвост, умирает...
        for(int i = 1; i < snake.size(); i++)
        {
            if(snake.front()->GetPosition().contains(snake[i]->GetPosition()))
            {
               QMessageBox::about(0,"Snake","Game Over");

               for(int i = 0;i<snake.size();i++)
               {
                   delete snake[i];
                   snake[i] = 0;
               }

               snake.clear();
               // ...и опять возрождается
               GameInit();
            }
        }
    }
    m_pSprite_apple->UpdateFrame();
    m_pSprite_blood->UpdateFrame();

    m_pSprite_apple2->UpdateFrame();
    m_pSprite_blood2->UpdateFrame();
    //Обновляем окно для вызова события рисовальщика
    update();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    //Нажимается каждый раз когда нажимаем на клавиши
    switch(event->key())
    {
        case Qt::Key_Left:
        {
            if((snake.front()->GetVelocity().y() > 0 || snake.front()->GetVelocity().y() < 0) && m_test_key)
                snake.front()->SetVelocity(-16,0);
        }break;
        case Qt::Key_Right:
        {
            if((snake.front()->GetVelocity().y() > 0 || snake.front()->GetVelocity().y() < 0) && m_test_key)
                snake.front()->SetVelocity(16,0);
        }break;
        case Qt::Key_Up:
        {
            if((snake.front()->GetVelocity().x() > 0 || snake.front()->GetVelocity().x() < 0) && m_test_key)
                snake.front()->SetVelocity(0,-16);
        }break;
        case Qt::Key_Down:
        {
            if((snake.front()->GetVelocity().x() > 0 || snake.front()->GetVelocity().x() < 0) && m_test_key)
                snake.front()->SetVelocity(0,16);
        }break;
        case Qt::Key_Space:
        {
            m_pause = (m_pause) ? false : true;
        }break;
    }
    m_test_key = false;
}

void Widget::DrawToTurn(QPainter* painter, int i, int a_end, int a_angle)
{
    m_tl = false;
    m_tr = false;
    m_bl = false;
    m_br = false;
    if(i == snake.size()-1)
        snake[i]->Draw(painter, m_pixmap_end_tail, a_end);
    else
    if(m_newItem)
    {
        m_newItem = false;
        snake[i-1]->Draw(painter, m_pixmap_angle_tail, a_angle);
    }
    else
        snake[i]->Draw(painter, m_pixmap_angle_tail, a_angle);
}

void Widget::DrawToDir(QPainter* painter, int i, int a_end, int a_mid)
{
    if(i == snake.size()-1)
        snake[i]->Draw(painter, m_pixmap_end_tail, a_end);
    else
    if(m_newItem)
    {
        m_newItem = false;
        if(!m_br || !m_tr)
            snake[i-1]->Draw(painter, m_pixmap_mid_tail, a_mid);
    }
    else
        snake[i]->Draw(painter, m_pixmap_mid_tail, a_mid);
}

void Widget::paintEvent(QPaintEvent*)
{
    //Вызывается каждый раз при обновлении окна, например по таймеру.
    //еще при создании окна, перемещению и изменению размера окна.
    QPainter painter(this);
    //int side = qMin(width(), height());
    //painter.setViewport((width() - side)/2, (height() - side)/2, side, side);
    //Устанавливаем значение логических координат в физические координаты окна
    //При изменении размера окна наша картинка в окне тоже растягивается.
    painter.setWindow(0,0,m_width_window, m_height_window);
    painter.drawPixmap(0,0,m_pixmap_background);

    for(int row=0;row<m_row*m_size_cell;row+=m_size_cell)
        for(int col=0;col<m_col*m_size_cell;col+=m_size_cell)
        {
            if(row == 0 || row == (m_row-1)*m_size_cell ||
                    col == 0 || col == (m_col-1)*m_size_cell)
            {
                if(row==0*m_size_cell && (col>=2*m_size_cell && col<=20*m_size_cell) )
                {
                    continue;
                }
                else
                    painter.drawPixmap(col,row,m_pixmap_wall);
            }
        }

    DrawScore(painter);
    DrawOst(painter);
    DrawTime(painter);
    //Первое яблоко
    if(m_pause == false)
    {
        m_pSprite_apple2->Draw(&painter);

        if(m_pSprite_blood2->GetOneFrame())
        {
            m_pSprite_blood2->Draw(&painter);
            m_pSprite_blood2->SetOneFrame(false);
        }
    }
    else
    {
        DrawTextPause(painter);
    }

    if(m_pause == false)
    {
        //head
        if(snake.front()->GetVelocity() == QPoint(0,-16))
            snake.front()->Draw(&painter, m_pixmap_head, 90);
        else
        if(snake.front()->GetVelocity() == QPoint(0,16))
            snake.front()->Draw(&painter, m_pixmap_head, -90);
        else
        if(snake.front()->GetVelocity() == QPoint(-16,0))
            snake.front()->Draw(&painter, m_pixmap_head, 0);
        else
        if(snake.front()->GetVelocity() == QPoint(16,0))
            snake.front()->Draw(&painter, m_pixmap_head, 180);


        for(int i = snake.size()-1;i>0;--i)
        {
            if(snake[i]->GetVelocity() == QPoint(-16,0))
            {
                if(m_tl) DrawToTurn(&painter, i, 90,-90);
                else
                if(m_bl) DrawToTurn(&painter, i,90,0);
                else DrawToDir(&painter, i,90,0);

                m_br = true;
                m_tr = true;
            }
            else
            if(snake[i]->GetVelocity() == QPoint(16,0))
            {
                if(m_tr) DrawToTurn(&painter, i, -90, 180);
                else
                if(m_br) DrawToTurn(&painter, i, -90, 90);
                else DrawToDir(&painter, i,-90,0);

                m_bl = true;
                m_tl = true;
            }
            else
            if(snake[i]->GetVelocity() == QPoint(0,16))
            {
                if(m_tr) DrawToTurn(&painter, i,0, -180);
                else
                if(m_tl) DrawToTurn(&painter, i, 0,-90);
                else DrawToDir(&painter, i,0,90);

                m_bl = true;
                m_br = true;
            }
            else
            if(snake[i]->GetVelocity() == QPoint(0,-16))
            {
                if(m_br) DrawToTurn(&painter, i,180,90);
                else
                if(m_bl) DrawToTurn(&painter, i,180,0);
                else DrawToDir(&painter, i,180,90);

                m_tl = true;
                m_tr = true;
            }
        }
    }
    //Второе яблоко
    if(m_pause == false)
    {
        m_pSprite_apple->Draw(&painter);

        if(m_pSprite_blood->GetOneFrame())
        {
            m_pSprite_blood->Draw(&painter);
            m_pSprite_blood->SetOneFrame(false);

        }
    }
    else
    {
        DrawTextPause(painter);
    }

    if(m_pause == false)
    {
        //head
        if(snake.front()->GetVelocity() == QPoint(0,-16))
            snake.front()->Draw(&painter, m_pixmap_head, 90);
        else
        if(snake.front()->GetVelocity() == QPoint(0,16))
            snake.front()->Draw(&painter, m_pixmap_head, -90);
        else
        if(snake.front()->GetVelocity() == QPoint(-16,0))
            snake.front()->Draw(&painter, m_pixmap_head, 0);
        else
        if(snake.front()->GetVelocity() == QPoint(16,0))
            snake.front()->Draw(&painter, m_pixmap_head, 180);


        for(int i = snake.size()-1;i>0;--i)
        {
            if(snake[i]->GetVelocity() == QPoint(-16,0))
            {
                if(m_tl) DrawToTurn(&painter, i, 90,-90);
                else
                if(m_bl) DrawToTurn(&painter, i,90,0);
                else DrawToDir(&painter, i,90,0);

                m_br = true;
                m_tr = true;
            }
            else
            if(snake[i]->GetVelocity() == QPoint(16,0))
            {
                if(m_tr) DrawToTurn(&painter, i, -90, 180);
                else
                if(m_br) DrawToTurn(&painter, i, -90, 90);
                else DrawToDir(&painter, i,-90,0);

                m_bl = true;
                m_tl = true;
            }
            else
            if(snake[i]->GetVelocity() == QPoint(0,16))
            {
                if(m_tr) DrawToTurn(&painter, i,0, -180);
                else
                if(m_tl) DrawToTurn(&painter, i, 0,-90);
                else DrawToDir(&painter, i,0,90);

                m_bl = true;
                m_br = true;
            }
            else
            if(snake[i]->GetVelocity() == QPoint(0,-16))
            {
                if(m_br) DrawToTurn(&painter, i,180,90);
                else
                if(m_bl) DrawToTurn(&painter, i,180,0);
                else DrawToDir(&painter, i,180,90);

                m_tl = true;
                m_tr = true;
            }
        }
    }
}

void Widget::DrawTextPause(QPainter &p)
{
    QFont font;
    font.setPixelSize(40);
    p.setFont(font);
    int width_text = 350;
    int height_text = 100;
    QRect rect;
    rect.setX(640/2-width_text/2);
    rect.setY(480/2-height_text/2);
    rect.setWidth(width_text);
    rect.setHeight(height_text);
    p.drawText(rect, Qt::AlignCenter,"P A U S E\npress space to play");
}
void Widget::DrawScore(QPainter& p)
{
    QFont font;
    font.setPixelSize(16);
    font.setBold(true);
    p.setFont(font);
    p.drawText(16*2,0,250,16,Qt::TextSingleLine,"S C O R E: " + QString::number(score_));
}
void Widget::DrawOst(QPainter& p)
{
    QFont font;
    font.setPixelSize(16);
    font.setBold(true);
    p.setFont(font);
    p.drawText(92*2,0,250,16,Qt::TextSingleLine,"E A T: " + QString::number(ost));
}

void Widget::DrawTime(QPainter& p)
{


}

