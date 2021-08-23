#include <QGridLayout>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QScreen>
#include <QMouseEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define YYS 10
#define HIDE_WIN_WIDTH (YYS+3)
#define YYCLR 0, 0, 0


void MainWindow::show_side()
{
    ui->w_l ->setVisible(true);
    ui->w_t ->setVisible(true);
    ui->w_r ->setVisible(true);
    ui->w_b ->setVisible(true);
    ui->w_lt->setVisible(true);
    ui->w_lb->setVisible(true);
    ui->w_rt->setVisible(true);
    ui->w_rb->setVisible(true);
    ui->w_l ->setSideSize(YYS);
    ui->w_t ->setSideSize(YYS);
    ui->w_r ->setSideSize(YYS);
    ui->w_b ->setSideSize(YYS);
    ui->w_lt->setSideSize(YYS);
    ui->w_lb->setSideSize(YYS);
    ui->w_rt->setSideSize(YYS);
    ui->w_rb->setSideSize(YYS);


}
void MainWindow::hide_side()
{
    ui->w_l ->setVisible(false);
    ui->w_t ->setVisible(false);
    ui->w_r ->setVisible(false);
    ui->w_b ->setVisible(false);
    ui->w_lt->setVisible(false);
    ui->w_lb->setVisible(false);
    ui->w_rt->setVisible(false);
    ui->w_rb->setVisible(false);
    ui->w_l ->setSideSize(0);
    ui->w_t ->setSideSize(0);
    ui->w_r ->setSideSize(0);
    ui->w_b ->setSideSize(0);
    ui->w_lt->setSideSize(0);
    ui->w_lb->setSideSize(0);
    ui->w_rt->setSideSize(0);
    ui->w_rb->setSideSize(0);
}
void MainWindow::init_side()
{
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

    show_side();

}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    b_move = false;

    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);    // 去掉边框
    setAttribute(Qt::WA_TranslucentBackground); // 背景透明

    //ctrl
    connect(ui->close, &QToolButton::released, this, &MainWindow::close);
    connect(ui->min, &QToolButton::released, this,&MainWindow::showMinimized);
    connect(ui->max, &QToolButton::released, this, &MainWindow::showMaxOrNormal);
    connect(ui->rest, &QToolButton::released, this, &MainWindow::showMaxOrNormal);

    show_max_or_rest_icon();

    //side
    init_side();

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
    if (!isMaximized())
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
            color.setAlpha(5 - i*1.0/YYS*5);
            painter.setPen(color);
            painter.drawPath(path);
        }

    }
    else
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(0, 0, this->width(), this->height());
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.fillPath(path, QBrush(Qt::white));
    }
    return QMainWindow::paintEvent(event);
}

void MainWindow::enterEvent(QEnterEvent * /*event*/)
{
    if(need_show())
    {
        start_show();
    }
}
void MainWindow::leaveEvent(QEvent * /*event*/)
{
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
        QRect deskrct = QGuiApplication::screens()[0]->geometry();
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

void MainWindow::mouseReleaseEvent(QMouseEvent */*event*/)
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
    showMaxOrNormal();
}

void MainWindow::show_max_or_rest_icon()
{
    if(isMaximized())
    {
        ui->max->setVisible(false);
        ui->max->setEnabled(false);
        ui->rest->setVisible(true);
        ui->rest->setEnabled(true);

    }
    else
    {
        ui->max->setVisible(true);
        ui->max->setEnabled(true);
        ui->rest->setVisible(false);
        ui->rest->setEnabled(false);
    }
}

void MainWindow::move_rect(const QRect& rect)
{
    if(isMaximized())
        return ;
    setGeometry(rect);
}

void MainWindow::showMaxOrNormal()
{
    if(isMaximized())
    {
        showNormal();
        show_side();
    }
    else
    {
        showMaximized();
        hide_side();
    }
    show_max_or_rest_icon();
    update();
}
