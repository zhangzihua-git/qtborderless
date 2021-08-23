#include <QGridLayout>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QScreen>
#include <QMouseEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define YYS 5
#define HIDE_WIN_WIDTH (YYS+3)
#define YYCLR 255, 192, 128

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    b_move = false;

    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);    // 去掉边框
    setAttribute(Qt::WA_TranslucentBackground); // 背景透明
//    ui-> win->setContentsMargins(YYS, YYS, YYS, YYS); // 注意和阴影大小的协调

    //ctrl
    connect(ui->close, &QToolButton::released, this, &MainWindow::close);
    connect(ui->min, &QToolButton::released, this,&MainWindow::showMinimized);
    connect(ui->max, &QToolButton::released, [&](){
        if(bShowMax)
        {
            bShowMax = false;
            showMaximized();
            show_max_or_rest();
            qDebug() << "showMaximized";
        }
    });
    connect(ui->rest, &QToolButton::released, [&](){
        if(!bShowMax)
        {
            bShowMax = true;
            showNormal();
            show_max_or_rest();
            qDebug() << "showNormal";
        }
    });

    bShowMax = true;
    show_max_or_rest();

    //size
    ui->w_l ->setSideTpe(CSideBox::T_LEFT);
    ui->w_t ->setSideTpe(CSideBox::T_TOP);
    ui->w_r ->setSideTpe(CSideBox::T_RIGHT);
    ui->w_b ->setSideTpe(CSideBox::T_BOTTOM);
    ui->w_lt->setSideTpe(CSideBox::T_LEFT_TOP);
    ui->w_lb->setSideTpe(CSideBox::T_LEFT_BOTTOM);
    ui->w_rt->setSideTpe(CSideBox::T_RIGHT_TOP);
    ui->w_rb->setSideTpe(CSideBox::T_RIGHT_BOTTOM);
    ui->w_l ->setWin(this);
    ui->w_t ->setWin(this);
    ui->w_r ->setWin(this);
    ui->w_b ->setWin(this);
    ui->w_lt->setWin(this);
    ui->w_lb->setWin(this);
    ui->w_rt->setWin(this);
    ui->w_rb->setWin(this);
    connect(ui->w_l, &CSideBox::move_rect, this,  &MainWindow::move_rect);
    connect(ui->w_t, &CSideBox::move_rect, this,  &MainWindow::move_rect);
    connect(ui->w_r, &CSideBox::move_rect, this,  &MainWindow::move_rect);
    connect(ui->w_b, &CSideBox::move_rect, this,  &MainWindow::move_rect);
    connect(ui->w_lt, &CSideBox::move_rect, this, &MainWindow::move_rect);
    connect(ui->w_lb, &CSideBox::move_rect, this, &MainWindow::move_rect);
    connect(ui->w_rt, &CSideBox::move_rect, this, &MainWindow::move_rect);
    connect(ui->w_rb, &CSideBox::move_rect, this, &MainWindow::move_rect);

//    setMouseTracking(true);
//    ui->win->setMouseTracking(true);
//    setAttribute( Qt::WA_Hover,true);

    //anim
    hide_anim = new QPropertyAnimation(this, "pos", this);
    show_anim = new QPropertyAnimation(this, "pos", this);
    hide_anim->setDuration(150);
    show_anim->setDuration(150);
    hided_type = 0;



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if (true)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(YYS, YYS, this->width() - YYS * 2, this->height() - YYS * 2);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.fillPath(path, QBrush(Qt::white));
        QColor color(0, YYCLR);
        for (int i = 0; i < YYS; i++)
        {
            path.setFillRule(Qt::WindingFill);
            path.addRect(YYS - i, YYS - i, this->width() - (YYS - i) * 2, this->height() - (YYS - i) * 2);
            color.setAlpha(100 - qSqrt(i) * 50);
            painter.setPen(color);
            painter.drawPath(path);
        }

    }
    return QMainWindow::paintEvent(event);
}

void MainWindow::enterEvent(QEnterEvent * /*event*/)
{
//    qDebug() << "MainWindow::enterEvent";
    if(need_show())
    {
        start_show();
    }
}
void MainWindow::leaveEvent(QEvent * /*event*/)
{
//    qDebug() << "MainWindow::leaveEvent";
    int type = need_hide();
    if(type != 0)
    {
        start_hide(type);
    }
}

int MainWindow::need_hide()
{
    if(isMaximized())
    {
        return 0;
    }

    if(hided_type != 0)
    {
        return 0;
    }
    else if(hide_anim->state() != QAbstractAnimation::Stopped)
    {
        return 0;
    }
    else if(show_anim->state() != QAbstractAnimation::Stopped)
    {
        return 0;
    }
    else
    {
        QRect rct = geometry();
        QRect deskrct = QGuiApplication::screens().first()->geometry();
        //if(rct.x() < deskrct.x() || rct.y() < deskrct.y() || rct.x()+rct.width() > rct.width())
        if(rct.y() <= deskrct.y())
        {
            return 1;
        }
        else if(rct.x() <= deskrct.x() )
        {
            return 2;
        }
        else if(rct.x()+rct.width() >= deskrct.width())
        {
            return 3;
        }
        else
        {
            return 0;
        }
    }

}
bool MainWindow::need_show()
{

    if( hided_type == 0)
    {
        return false;
    }
    else if(hide_anim->state() != QAbstractAnimation::Stopped)
    {
        return false;
    }
    else if(show_anim->state() != QAbstractAnimation::Stopped)
    {
        return false;
    }
    return true;
}

void MainWindow::start_hide(int hide_type)
{
    qDebug() << "Dialog::start_hide" << hide_type;
    hided_type = hide_type;

    switch(hide_type)
    {
    case 0:
        break;
    case 1:
        //top
    {
        hide_anim->setStartValue(QPoint(geometry().x(), geometry().y()));
        hide_anim->setEndValue(QPoint(geometry().x(), HIDE_WIN_WIDTH-geometry().height()));
        hide_anim->start();
        break;
    }
    case 2:
        //left
    {
        hide_anim->setStartValue(QPoint(geometry().x(), geometry().y()));
        hide_anim->setEndValue(QPoint(HIDE_WIN_WIDTH-geometry().width(), geometry().y()));
        hide_anim->start();
        break;
    }
    case 3:
        //right
    {
        QRect deskrct = QGuiApplication::screens()[0]->geometry();
        hide_anim->setStartValue(QPoint(geometry().x(), geometry().y()));
        hide_anim->setEndValue(QPoint(deskrct.width()-HIDE_WIN_WIDTH, geometry().y()));
        hide_anim->start();
        break;
    }
    }
}
void MainWindow::start_show()
{
    qDebug() << "Dialog::start_show";
    int tmp = hided_type;
    hided_type = 0;

    switch(tmp)
    {
    case 0:
        break;
    case 1:
        //top
    {
        hide_anim->setStartValue(QPoint(geometry().x(), geometry().y()));
        hide_anim->setEndValue(QPoint(geometry().x(), -HIDE_WIN_WIDTH));
        hide_anim->start();
        break;
    }
    case 2:
        //left
    {
        hide_anim->setStartValue(QPoint(geometry().x(), geometry().y()));
        hide_anim->setEndValue(QPoint(-HIDE_WIN_WIDTH, geometry().y()));
        hide_anim->start();
        break;
    }
    case 3:
        //right
    {
        QRect deskrct = QGuiApplication::screens()[0]->geometry();
        hide_anim->setStartValue(QPoint(geometry().x(), geometry().y()));
        hide_anim->setEndValue(QPoint(deskrct.width()-geometry().width()+HIDE_WIN_WIDTH, geometry().y()));
        hide_anim->start();
        break;
    }
    }


}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    b_move = false;
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(ui->title->geometry().contains(event->pos()))
    {
        x_last = event->pos().x();
        y_last = event->pos().y();
        b_move = true;
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(isMaximized())
    {
        return ;
    }
    if(b_move)
    {
        int x = event->pos().x() - x_last;
        int y = event->pos().y() - y_last;
        move(this->x()+x, this->y()+y);
    }

}
void MainWindow::mouseDoubleClickEvent(QMouseEvent *)
{
}

void MainWindow::show_max_or_rest()
{
    if(bShowMax)
    {
        ui->max->setVisible(true);
        ui->max->setEnabled(true);
        ui->rest->setVisible(false);
        ui->rest->setEnabled(false);
    }
    else
    {
        ui->max->setVisible(false);
        ui->max->setEnabled(false);
        ui->rest->setVisible(true);
        ui->rest->setEnabled(true);

    }
}

void MainWindow::move_rect(const QRect& rect)
{
    if(isMaximized())
        return ;
    setGeometry(rect);
}
//bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
//{
////    qDebug() << "MainWindow::nativeEvent" << eventType;
//    return QMainWindow::nativeEvent(eventType, message, result);
//}


