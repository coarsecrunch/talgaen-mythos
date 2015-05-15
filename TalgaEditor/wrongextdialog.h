#ifndef WRONGEXTDIALOG_H
#define WRONGEXTDIALOG_H

#include <QDialog>

namespace Ui {
  class WrongExtDialog;
}

class WrongExtDialog : public QDialog
{
  Q_OBJECT

public:
  explicit WrongExtDialog(QWidget *parent = 0);
  ~WrongExtDialog();

private:
  Ui::WrongExtDialog *ui;
};

#endif // WRONGEXTDIALOG_H
