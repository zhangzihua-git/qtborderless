#ifndef CSIDE_H
#define CSIDE_H

#include <QObject>
#include <QSize>

#include "head.h"

class MainWindow;
class CSideBox;

class CSide : public QObject
{
    Q_OBJECT

    ADD_PROPERTY(int, sideSize)

public:
    explicit CSide(MainWindow *parent = nullptr);
    void show();
    void hide();
private slots:
    void do_resize(QSize size);
private:
    CSideBox *w_lt;
    CSideBox *w_t;
    CSideBox *w_rt;
    CSideBox *w_l;
    CSideBox *w_r;
    CSideBox *w_lb;
    CSideBox *w_b;
    CSideBox *w_rb;
};

#endif // CSIDE_H
