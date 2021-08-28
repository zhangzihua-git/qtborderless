#include <QGridLayout>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cside.h"

#define HIDE_WIN_WIDTH (shadowSize()+3)

MainWindow* g_mainwin= nullptr;

void MainWindow::show_shadow()
{
    ui->win->layout()->setContentsMargins(shadowSize(), shadowSize(), shadowSize(), shadowSize());
}
void MainWindow::hide_shadow()
{
    ui->win->layout()->setContentsMargins(0,0,0,0);
}
void MainWindow::init_side()
{
    side = new CSide(this);
}
void MainWindow::show_side()
{
    if(sizeChangable())
    {
        side->show();
    }
    else
    {
        side->hide();
    }
}
void MainWindow::hide_side()
{
    side->hide();
}

void MainWindow::show_sideRadius()
{
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
}
void MainWindow::hide_sideRadius()
{
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
    setStyleSheet(qss.arg(0));
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , isHided(false)
    , ui(new Ui::MainWindow)
    , side(nullptr)
{
    g_mainwin = this;
    b_move = false;

    //配置属性
    set_shadowSize(10);
    set_shadowColor(QColor(255, 0, 0, 100));
    set_autoHide(true);
    set_sizeChangable(true);
    set_sideRadius(10);
    set_titleSize(50);


    ui->setupUi(this);

    //size
    resize(ui->form->size().width()+2*shadowSize(), ui->form->size().height()+2*shadowSize() + titleSize());

    setWindowFlags(Qt::FramelessWindowHint);    // 去掉边框
    setAttribute(Qt::WA_TranslucentBackground); // 背景透明

    //ctrl
    connect(ui->close, &QToolButton::released, this, &MainWindow::close);
    connect(ui->min, &QToolButton::released, this,&MainWindow::showMinimized);
    connect(ui->max, &QToolButton::released, this, &MainWindow::showMaxOrNormal);
    connect(ui->rest, &QToolButton::released, this, &MainWindow::showMaxOrNormal);

    //property modify
    connect(this, &MainWindow::shadowSize_changed, this, &MainWindow::do_shadowSize);
    connect(this, &MainWindow::shadowColor_changed, this, (void (MainWindow::*)())&MainWindow::update);
    connect(this, &MainWindow::sizeChangable_changed, this, &MainWindow::show_max_or_rest_icon);
    connect(this, &MainWindow::sizeChangable_changed, this, &MainWindow::show_side);
    connect(this, &MainWindow::sideRadius_changed, this, &MainWindow::show_sideRadius);
    connect(this, &MainWindow::sideRadius_changed, this, (void (MainWindow::*)())&MainWindow::update);
    connect(this, &MainWindow::titleSize_changed, this, &MainWindow::do_titleSize);


    show_max_or_rest_icon();

    //shadow
    show_shadow();

    //side
    init_side();
    show_side();

    //round window
    show_sideRadius();

    //title
    ui->title->setMinimumHeight(titleSize());
    ui->title->setMaximumHeight(titleSize());

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
    qDebug() << size() << ui->form->size();
    const int POINT_SHADOW_EX = 5;
    if (!isMaximized())
    {
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform); //抗锯齿和使用平滑转换算法

        QColor color = shadowColor();
        int max_alpha = color.alpha();
        int yyw =shadowSize() +sideRadius();
        for (int i = 0; i < yyw; i++)
        {
            QPainterPath path;
            QPoint prect1(yyw - i, yyw - i);
            QPoint prect2(this->width() - (yyw - i), yyw - i);
            QPoint prect3(this->width() - (yyw - i),this->height() - (yyw - i));
            QPoint prect4(yyw - i, this->height() - (yyw - i));

            int radius = POINT_SHADOW_EX + i;

            QPoint p1 = prect1 + QPoint(radius, 0);
            QPoint p2 = prect2 + QPoint(-radius, 0);
            QPoint p3 = prect2 + QPoint(0, radius);
            QPoint p4 = prect3 + QPoint(0, -radius);
            QPoint p5 = prect3 + QPoint(-radius, 0);
            QPoint p6 = prect4 + QPoint(radius, 0);
            QPoint p7 = prect4 + QPoint(0, -radius);
            QPoint p8 = prect1 + QPoint(0, radius);

            path.moveTo(p1);
            path.lineTo(p2);
            path.quadTo(prect2, p3);
            path.lineTo(p4);
            path.quadTo(prect3, p5);
            path.lineTo(p6);
            path.quadTo(prect4, p7);
            path.lineTo(p8);
            path.quadTo(prect1, p1);
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

    hide_anim->setStartValue(pos());
    hide_anim->setEndValue(hide_hide_point);
    isHided = true;
    hide_anim->start();
}
void MainWindow::start_show()
{
    hide_anim->setStartValue(pos());
    hide_anim->setEndValue(hide_show_point);
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

void MainWindow::resizeEvent(QResizeEvent *event)
{
    emit resized(event->size());
}
void MainWindow::show_max_or_rest_icon()
{
    if(!sizeChangable())
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

void MainWindow::move_rect(int left, int top, int right, int bottom)
{
    if(isMaximized())
        return ;
    QRect nowrect = geometry();
    nowrect.setLeft(nowrect.left()+left);
    nowrect.setTop(nowrect.top()+top);
    nowrect.setRight(nowrect.right()+right);
    nowrect.setBottom(nowrect.bottom()+bottom);
    setGeometry(nowrect);
}

void MainWindow::showMaxOrNormal()
{
    if(!sizeChangable())
    {
        return ;
    }
    if(isMaximized())
    {
        showNormal();
        show_shadow();
        show_sideRadius();
        show_side();
    }
    else
    {
        showMaximized();
        hide_shadow();
        hide_sideRadius();
        hide_side();
    }
    show_max_or_rest_icon();
//    update();
}

void MainWindow::do_shadowSize(int /*s*/)
{
    show_shadow();
    emit resized(size());
    update();
}
void MainWindow::do_titleSize(int /*s*/)
{
    //title
    ui->title->setMinimumHeight(titleSize());
    ui->title->setMaximumHeight(titleSize());
    resize(ui->form->size().width()+2*shadowSize(), ui->form->size().height()+2*shadowSize() + titleSize());

}
