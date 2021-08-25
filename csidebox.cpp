#include <QMouseEvent>

#include "csidebox.h"

CSideBox::CSideBox(QWidget *parent) : QWidget(parent)
{
    b_changeable = true;
//    win = nullptr;
}

void CSideBox::setSideChangeable(bool changeable)
{
    b_changeable = changeable;
    if(!b_changeable)
    {
        setCursor(Qt::ArrowCursor);
    }
}
void CSideBox::setSideTpe(SideType st)
{
    if(!b_changeable)
    {
        setCursor(Qt::ArrowCursor);
    }
    else
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
    if(!b_changeable)
    {
        return ;
    }
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
    if(!b_changeable)
    {
        return ;
    }
    x_last = event->pos().x();
    y_last = event->pos().y();
    b_move = true;
}
void CSideBox::mouseReleaseEvent(QMouseEvent */*event*/)
{
    if(!b_changeable)
    {
        return ;
    }
    b_move = false;
}


void CSideBox::do_move(int x, int y)
{
    if(!b_changeable)
    {
        return ;
    }
    int left   =0;
    int top    =0;
    int right  =0;
    int bottom =0;
    switch (sidetype) {
    case T_LEFT:
        left = x;
        break;
    case T_RIGHT:
        right = x;
        break;
    case T_TOP:
        top = y;
        break;
    case T_BOTTOM:
        bottom = y;
        break;
    case T_LEFT_TOP:
        left = x;
        top = y;
        break;
    case T_RIGHT_BOTTOM:
        right = x;
        bottom = y;
        break;
    case T_LEFT_BOTTOM:
        left = x;
        bottom = y;
        break;
    case T_RIGHT_TOP:
        right = x;
        top = y;
        break;
    }
    emit moved(left, top, right, bottom);

}
