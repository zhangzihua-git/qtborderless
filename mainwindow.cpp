#include <QGridLayout>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define YYS sideSize()
#define HIDE_WIN_WIDTH (YYS+3)


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
    ui->w_l ->setSideChangeable(sizeChangeable());
    ui->w_t ->setSideChangeable(sizeChangeable());
    ui->w_r ->setSideChangeable(sizeChangeable());
    ui->w_b ->setSideChangeable(sizeChangeable());
    ui->w_lt->setSideChangeable(sizeChangeable());
    ui->w_lb->setSideChangeable(sizeChangeable());
    ui->w_rt->setSideChangeable(sizeChangeable());
    ui->w_rb->setSideChangeable(sizeChangeable());
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
    , isHided(false)
    , ui(new Ui::MainWindow)
{
    b_move = false;

    //配置属性
    set_sideSize(10);
    set_sideColor(QColor(0, 0, 0, 20));
    set_autoHide(true);
    set_sizeChangeable(true);
    set_sideRadius(10);




    ui->setupUi(this);


    //qss
    QString qss = R"(
#title {
border-image: url(:/frame/title.png);
border-top-left-radius: %1px;
border-top-right-radius: %1px;
}
#form {
background-color: rgb(255, 255, 255);
border-bottom-left-radius: %1px;
border-bottom-right-radius: %1px;
}
)";
    setStyleSheet(qss.arg(sideRadius()));



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
    hide_anim->setDuration(150);


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
        QPainter painter(this);

        QColor color = sideColor();
        int max_alpha = color.alpha();
        int yyw =YYS +sideRadius();
        for (int i = 0; i < yyw; i++)
        {
            path.setFillRule(Qt::WindingFill);
            path.addRect(yyw - i, yyw - i, this->width() - (yyw - i) * 2, this->height() - (yyw - i) * 2);
            color.setAlpha(max_alpha-max_alpha*i/yyw);
            painter.setPen(color);
            painter.drawPath(path);
        }

    }
    else
    {
    }
    return QMainWindow::paintEvent(event);
}

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
void MainWindow::enterEvent(QEvent * /*event*/)
#else
void MainWindow::enterEvent(QEnterEvent * /*event*/)
#endif
{
    if(need_show())
    {
        start_show();
    }
}
void MainWindow::leaveEvent(QEvent * /*event*/)
{
    if(need_hide())
    {
        start_hide();
    }
}

static bool check_hide(int hideSize, const QVector<QRect>& screenRects, const QRect& winRect, QPoint& showPoint, QPoint& hidePoint)
{
    for(int i=0; i<screenRects.size(); i++)
    {
        bool bfound = true;
        const QRect& screenRect(screenRects[i]);
        if(screenRect.intersected(winRect).isEmpty())
        {
            bfound = false;
        }
        else if(winRect.top() <= screenRect.top())
        {
            //top
            showPoint.setX(winRect.left());
            showPoint.setY(screenRect.top() - hideSize);
            hidePoint.setX(winRect.left());
            hidePoint.setY(screenRect.top() - winRect.height() + hideSize);
        }
        else if(winRect.left() <= screenRect.left())
        {
            // left
            showPoint.setX(screenRect.left() - hideSize);
            showPoint.setY(winRect.top());
            hidePoint.setX(screenRect.left() - winRect.width() + hideSize);
            hidePoint.setY(winRect.top());
        }
        else if(winRect.right() > screenRect.right())
        {
            // right
            showPoint.setX(screenRect.right() -winRect.width() + hideSize);
            showPoint.setY(winRect.top());
            hidePoint.setX(screenRect.right() - hideSize);
            hidePoint.setY(winRect.top());
        }
        else
        {
            //no
            bfound = false;
        }

        if(bfound)
        {
            for(int j=0; j<screenRects.size(); j++)
            {
                if(i == j)
                {
                    continue;
                }
                const QRect& otherScreenRect(screenRects[j]);
                QRect hideRect(winRect);
                hideRect.moveTo(hidePoint);
                QRect showRect(winRect);
                showRect.moveTo(showPoint);
                if(!otherScreenRect.intersected(hideRect).isEmpty()
                        || !otherScreenRect.intersected(showRect).isEmpty())
                {
                    bfound = false;
                    break;
                }
            }
        }
        if(bfound)
        {
            return true;
        }
    }
    return false;
}
bool MainWindow::need_hide()
{
    if(!autoHide())
    {
        return false;
    }
    if(isMaximized())
    {
        return false;
    }

    else if(hide_anim->state() != QAbstractAnimation::Stopped)
    {
        return false;
    }
    else
    {
        QVector<QRect> screenRects;
        for(QScreen* s : QGuiApplication::screens())
        {
            screenRects.append(s->geometry());
        }
        return check_hide(HIDE_WIN_WIDTH, screenRects, geometry(), hide_show_point, hide_hide_point);
    }

}
bool MainWindow::need_show()
{
    if(!isHided)
    {
        return false;
    }
    if(hide_anim->state() != QAbstractAnimation::Stopped)
    {
        return false;
    }
    return true;
}

void MainWindow::start_hide()
{
    qDebug() << "Dialog::start_hide";

    hide_anim->setStartValue(pos());
    hide_anim->setEndValue(hide_hide_point);
    qDebug() << "hide_anim" << hide_anim->startValue() << hide_anim->endValue();
    isHided = true;
    hide_anim->start();
}
void MainWindow::start_show()
{
    qDebug() << "Dialog::start_show";

    hide_anim->setStartValue(pos());
    hide_anim->setEndValue(hide_show_point);
    qDebug() << "hide_anim" << hide_anim->startValue() << hide_anim->endValue();
    isHided = false;
    hide_anim->start();


}

void MainWindow::mouseReleaseEvent(QMouseEvent * /*event*/)
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
    if(!sizeChangeable())
    {
        ui->max->setVisible(false);
        ui->max->setEnabled(false);
        ui->rest->setVisible(false);
        ui->rest->setEnabled(false);

    }
    else if(isMaximized())
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
    if(!sizeChangeable())
    {
        return ;
    }
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
