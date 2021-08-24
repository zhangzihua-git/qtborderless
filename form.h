#ifndef FORM_H
#define FORM_H

#include <QFrame>

namespace Ui {
class Form;
}

class Form : public QFrame
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

private:
    Ui::Form *ui;
};

#endif // FORM_H
