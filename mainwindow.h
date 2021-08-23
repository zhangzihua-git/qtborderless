#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QPropertyAnimation* hide_anim;
    QPropertyAnimation* show_anim;
    int x_last;
    int y_last;
    bool b_move;
    bool bShowMax;


    int hided_type;
    int need_hide();
    bool need_show();
    void start_hide(int hide_type);
    void start_show();

    void paintEvent(QPaintEvent *event);
    void enterEvent(QEnterEvent * event);
    void leaveEvent(QEvent * event);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    void show_max_or_rest();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void move_rect(const QRect& rect);
};
#endif // MAINWINDOW_H
