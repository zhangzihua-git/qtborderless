#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define ADD_PROPERTY(t, p) \
    Q_PROPERTY(t p READ p WRITE set_##p NOTIFY p##_changed)\
    t m_##p;\
public:\
    Q_SIGNAL void p##_changed(t p);\
    t p(){return m_##p;}\
    void set_##p(t p){m_##p = p;}\
private:


class MainWindow : public QMainWindow
{
    Q_OBJECT


    ADD_PROPERTY(int, sideSize)
    //alpha 0最浅 255 最深
    ADD_PROPERTY(QColor, sideColor)
    ADD_PROPERTY(bool, autoHide)
    ADD_PROPERTY(bool, sizeChangeable)
    ADD_PROPERTY(int , sideRadius)

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

    void show_max_or_rest_icon();
    void init_side();

    void show_side();
    void hide_side();
    void showMaxOrNormal();
    void move_rect(const QRect& rect);


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
