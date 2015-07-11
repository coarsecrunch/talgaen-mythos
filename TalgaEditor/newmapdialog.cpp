#include "newmapdialog.h"
#include "ui_newmapdialog.h"

namespace talga
{
namespace editor
{


NewMapDialog::NewMapDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::NewMapDialog)
{
  ui->setupUi(this);
}

Map NewMapDialog::getData() const
{
  std::string name = ui->mapNameLine->text().toStdString();
  if (name.find(".tmap") == std::string::npos)
      name.append(".tmap");
  return Map::createEmptyMap(ui->tileWidthSpinBox->value(), ui->tileHeightSpinBox->value(), ui->widthSpinBox->value(), ui->heightSpinbox->value(), name);
}


NewMapDialog::~NewMapDialog()
{
  delete ui;
}

}
}
