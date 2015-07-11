#pragma once

#include <QDialog>
#include "Map.h"

namespace Ui {
  class NewMapDialog;
}

namespace talga
{
namespace editor
{

class NewMapDialog : public QDialog
{
  Q_OBJECT

public:
  explicit NewMapDialog(QWidget *parent = 0);
  ~NewMapDialog();
  Map getData() const;
private:
  Ui::NewMapDialog *ui;
};

}
}
