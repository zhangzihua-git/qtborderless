#include <QDebug>

#include "form.h"
#include "ui_form.h"
#include "mainwindow.h"
#include "QColorDialog";

extern MainWindow* g_mainwin;

Form::Form(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    ui->le_shadow->setText(QString::number(g_mainwin->shadowSize()));
    QString clrss = QString("QWidget{background-color:%1;}").arg(g_mainwin->shadowColor().name(QColor::HexArgb));
    ui->w_clr->setStyleSheet(clrss);  //设置样式表
    ui->w_clr->setProperty("myclr", g_mainwin->shadowColor());

    ui->cb_hide->setChecked(g_mainwin->autoHide());
    ui->cb_sizeChangable->setChecked(g_mainwin->sizeChangable());

    ui->le_radius->setText(QString::number(g_mainwin->sideRadius()));
    ui->le_titleSize->setText(QString::number(g_mainwin->titleSize()));

    connect(ui->pb_apply, &QPushButton::released, this, &Form::do_apply);
    connect(ui->pb_clr, &QPushButton::released, this, &Form::do_setclr);
}

Form::~Form()
{
    delete ui;
}


void Form::do_apply()
{
    qDebug() << "reset MainWindow property:";
    qDebug() << "shadowSize" << ui->le_shadow->text().toInt();
    g_mainwin->set_shadowSize(ui->le_shadow->text().toInt());
    qDebug() << "shadowColor" << ui->w_clr->property("myclr").value<QColor>();
    g_mainwin->set_shadowColor(ui->w_clr->property("myclr").value<QColor>());
    qDebug() << "autoHide" << ui->cb_hide->isChecked();
    g_mainwin->set_autoHide(ui->cb_hide->isChecked());
    qDebug() << "sizeChangeable" << ui->cb_sizeChangable->isChecked();
    g_mainwin->set_sizeChangable(ui->cb_sizeChangable->isChecked());
    qDebug() << "sideRadius" << ui->le_radius->text().toInt();
    g_mainwin->set_sideRadius(ui->le_radius->text().toInt());
    qDebug() << "titleSize" << ui->le_titleSize->text().toInt();
    g_mainwin->set_titleSize(ui->le_titleSize->text().toInt());

}

void Form::do_setclr()
{
    QColor colortmp =  ui->w_clr->property("myclr").value<QColor>();
    QColor color = QColorDialog::getColor(colortmp,this,
                                          tr("颜色对话框！"),
                                          QColorDialog::ShowAlphaChannel);
    QString clrss = QString("QWidget{background-color:%1;}").arg(color.name(QColor::HexArgb));
    ui->w_clr->setStyleSheet(clrss);  //设置样式表
    ui->w_clr->setProperty("myclr", color);

}
