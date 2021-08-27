#include "cside.h"
#include "mainwindow.h"
#include "csidebox.h"


//CSideBox *w_lt;
//CSideBox *w_t ;
//CSideBox *w_rt;
//CSideBox *w_l ;
//CSideBox *w_r ;
//CSideBox *w_lb;
//CSideBox *w_b ;
//CSideBox *w_rb;



CSide::CSide(MainWindow *parent) : QObject(parent)
{
//    connect(this, &CSide::moved, parent, &MainWindow::move_rect);

    w_lt = new CSideBox(parent);
    w_t  = new CSideBox(parent);
    w_rt = new CSideBox(parent);
    w_l  = new CSideBox(parent);
    w_r  = new CSideBox(parent);
    w_lb = new CSideBox(parent);
    w_b  = new CSideBox(parent);
    w_rb = new CSideBox(parent);
    w_l ->setSideTpe(CSideBox::T_LEFT);
    w_t ->setSideTpe(CSideBox::T_TOP);
    w_r ->setSideTpe(CSideBox::T_RIGHT);
    w_b ->setSideTpe(CSideBox::T_BOTTOM);
    w_lt->setSideTpe(CSideBox::T_LEFT_TOP);
    w_lb->setSideTpe(CSideBox::T_LEFT_BOTTOM);
    w_rt->setSideTpe(CSideBox::T_RIGHT_TOP);
    w_rb->setSideTpe(CSideBox::T_RIGHT_BOTTOM);
    w_l ->setSideChangeable(true);
    w_t ->setSideChangeable(true);
    w_r ->setSideChangeable(true);
    w_b ->setSideChangeable(true);
    w_lt->setSideChangeable(true);
    w_lb->setSideChangeable(true);
    w_rt->setSideChangeable(true);
    w_rb->setSideChangeable(true);

    set_sideSize(10);

    do_resize(parent->rect().size());

    connect(parent, &MainWindow::resized, this, &CSide::do_resize);
    connect(w_lt, &CSideBox::moved, parent, &MainWindow::move_rect);
    connect(w_t , &CSideBox::moved, parent, &MainWindow::move_rect);
    connect(w_rt, &CSideBox::moved, parent, &MainWindow::move_rect);
    connect(w_l , &CSideBox::moved, parent, &MainWindow::move_rect);
    connect(w_r , &CSideBox::moved, parent, &MainWindow::move_rect);
    connect(w_lb, &CSideBox::moved, parent, &MainWindow::move_rect);
    connect(w_b , &CSideBox::moved, parent, &MainWindow::move_rect);
    connect(w_rb, &CSideBox::moved, parent, &MainWindow::move_rect);

}


void CSide::do_resize(QSize size)
{
    MainWindow* win = (MainWindow*)parent();

    int shadowSize = win->shadowSize();
    QRect rectWin(shadowSize, shadowSize,size.width()-2*shadowSize, size.height()-2*shadowSize);
    QRect rectside;

    rectside.setLeft(rectWin.left()-sideSize());
    rectside.setRight(rectWin.left()+sideSize());
    rectside.setTop(rectWin.top()+sideSize());
    rectside.setBottom(rectWin.bottom()-sideSize());
    w_l ->setGeometry(rectside);

    rectside.setLeft(rectWin.right()-sideSize());
    rectside.setRight(rectWin.right()+sideSize());
    rectside.setTop(rectWin.top()+sideSize());
    rectside.setBottom(rectWin.bottom()-sideSize());
    w_r ->setGeometry(rectside);

    rectside.setLeft(rectWin.left()+sideSize());
    rectside.setRight(rectWin.right()-sideSize());
    rectside.setTop(rectWin.top()-sideSize());
    rectside.setBottom(rectWin.top()+sideSize());
    w_t ->setGeometry(rectside);

    rectside.setLeft(rectWin.left()+sideSize());
    rectside.setRight(rectWin.right()-sideSize());
    rectside.setTop(rectWin.bottom()-sideSize());
    rectside.setBottom(rectWin.bottom()+sideSize());
    w_b ->setGeometry(rectside);

    QRect rectSide2(0, 0, sideSize()*2, sideSize()*2);
    rectSide2.moveCenter(rectWin.topLeft());
    w_lt->setGeometry(rectSide2);

    rectSide2.moveCenter(rectWin.topRight());
    w_rt->setGeometry(rectSide2);

    rectSide2.moveCenter(rectWin.bottomLeft());
    w_lb->setGeometry(rectSide2);

    rectSide2.moveCenter(rectWin.bottomRight());
    w_rb->setGeometry(rectSide2);

    qDebug() << "do_resize" << size << w_t->geometry();
}

void CSide::show()
{
    w_lt->setVisible(true);
    w_t ->setVisible(true);
    w_rt->setVisible(true);
    w_l ->setVisible(true);
    w_r ->setVisible(true);
    w_lb->setVisible(true);
    w_b ->setVisible(true);
    w_rb->setVisible(true);
}
void CSide::hide()
{
    w_lt->setVisible(false);
    w_t ->setVisible(false);
    w_rt->setVisible(false);
    w_l ->setVisible(false);
    w_r ->setVisible(false);
    w_lb->setVisible(false);
    w_b ->setVisible(false);
    w_rb->setVisible(false);

}
