#ifndef CSIDEBOX_H
#define CSIDEBOX_H

#include <QWidget>

class CSideBox : public QWidget
{
    Q_OBJECT

    int x_last;
    int y_last;
    bool b_move;
    bool b_changeable;
public:
    explicit CSideBox(QWidget *parent = nullptr);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    enum SideType
    {
        T_LEFT,
        T_TOP,
        T_RIGHT,
        T_BOTTOM,
        T_LEFT_TOP,
        T_RIGHT_TOP,
        T_LEFT_BOTTOM,
        T_RIGHT_BOTTOM,
    } ;
    void setSideTpe(SideType st);
//    void setWin(QWidget*win)
//    {
//        this->win = win;
//    }
    void setSideSize(int size);
    void setSideChangeable(bool changeable);
signals:
    void moved(int left, int top, int right, int bottom);
private:
    SideType sidetype;
//    QWidget* win;

    void do_move(int x, int y);
};

#endif // CSIDEBOX_H
