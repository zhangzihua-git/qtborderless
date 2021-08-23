#include <QMouseEvent>

#include "csidebox.h"

CSideBox::CSideBox(QWidget *parent) : QWidget(parent)
{
    win = nullptr;
}

void CSideBox::setSideTpe(SideType st)
{
    sidetype = st;
    switch (st) {
    case T_LEFT:
    case T_RIGHT:
        setCursor(Qt::SizeHorCursor);
        break;
    case T_TOP:
    case T_BOTTOM:
        setCursor(Qt::SizeVerCursor);
        break;
    case T_LEFT_TOP:
    case T_RIGHT_BOTTOM:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case T_LEFT_BOTTOM:
    case T_RIGHT_TOP:
        setCursor(Qt::SizeBDiagCursor);
        break;
    }


}

void CSideBox::setSideSize(int size)
{
    switch (sidetype) {
    case T_LEFT:
    case T_RIGHT:
        setMinimumWidth(size);
        setMaximumWidth(size);
        break;
    case T_TOP:
    case T_BOTTOM:
        setMinimumHeight(size);
        setMaximumHeight(size);
        break;
    case T_LEFT_TOP:
    case T_RIGHT_BOTTOM:
    case T_LEFT_BOTTOM:
    case T_RIGHT_TOP:
        setMinimumHeight(size);
        setMaximumHeight(size);
        setMinimumWidth(size);
        setMaximumWidth(size);
        break;
    }
}


void CSideBox::mouseMoveEvent(QMouseEvent *event)
{
    if(b_move)
    {
        // 需要移动

        int x = event->pos().x() - x_last;
        int y = event->pos().y() - y_last;
        do_move(x, y);
    }
}
void CSideBox::mousePressEvent(QMouseEvent *event)
{
    x_last = event->pos().x();
    y_last = event->pos().y();
    b_move = true;
}
void CSideBox::mouseReleaseEvent(QMouseEvent */*event*/)
{
    b_move = false;
}


void CSideBox::do_move(int x, int y)
{
    if(!win)
        return ;

    QRect rect = win->geometry();

    switch (sidetype) {
    case T_LEFT:
        rect.setLeft(rect.left()+x);
        break;
    case T_RIGHT:
        rect.setRight(rect.right()+x);
        break;
    case T_TOP:
        rect.setTop(rect.top()+y);
        break;
    case T_BOTTOM:
        rect.setBottom(rect.bottom()+y);
        break;
    case T_LEFT_TOP:
        rect.setLeft(rect.left()+x);
        rect.setTop(rect.top()+y);
        break;
    case T_RIGHT_BOTTOM:
        rect.setRight(rect.right()+x);
        rect.setBottom(rect.bottom()+y);
        break;
    case T_LEFT_BOTTOM:
        rect.setLeft(rect.left()+x);
        rect.setBottom(rect.bottom()+y);
        break;
    case T_RIGHT_TOP:
        rect.setRight(rect.right()+x);
        rect.setTop(rect.top()+y);
        break;
    }

    emit move_rect(rect);

}
