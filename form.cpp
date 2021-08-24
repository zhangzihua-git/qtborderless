#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}
