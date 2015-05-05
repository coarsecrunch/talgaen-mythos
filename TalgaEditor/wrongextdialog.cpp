#include "wrongextdialog.h"
#include "ui_wrongextdialog.h"

WrongExtDialog::WrongExtDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WrongExtDialog)
{
    ui->setupUi(this);
}

WrongExtDialog::~WrongExtDialog()
{
    delete ui;
}
