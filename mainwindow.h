#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>

#include "head.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class CSide;
class MainWindow : public QMainWindow
{
    Q_OBJECT


    ADD_PROPERTY(int, shadowSize)
    ADD_PROPERTY(QColor, shadowColor)
    ADD_PROPERTY(bool, autoHide)
    ADD_PROPERTY(bool, sizeChangable)
    ADD_PROPERTY(int , sideRadius)
    ADD_PROPERTY(int , titleSize)


    QPropertyAnimation* hide_anim;
    QPoint hide_show_point;
    QPoint hide_hide_point;
    bool isHided;

    int x_last;
    int y_last;
    bool b_move;


//    int hided_type;
    bool need_hide();
    bool need_show();
    void start_hide();
    void start_show();

    void paintEvent(QPaintEvent *event);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    void enterEvent(QEvent * event);
#else
    void enterEvent(QEnterEvent * event);
#endif
    void leaveEvent(QEvent * event);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
     void resizeEvent(QResizeEvent *event);



public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void move_rect(int left, int top, int right, int bottom);
signals:
    void resized(QSize size);
private:
    Ui::MainWindow *ui;


    CSide* side;

private slots:
    void do_shadowSize(int);
    void do_titleSize(int);
    void show_shadow();
    void show_max_or_rest_icon();
    void init_side();
    void show_side();
    void hide_side();
    void hide_shadow();
    void show_sideRadius();
    void hide_sideRadius();
    void showMaxOrNormal();
};
#endif // MAINWINDOW_H
