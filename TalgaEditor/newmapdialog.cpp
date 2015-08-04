#include "newmapdialog.h"
#include "ui_newmapdialog.h"
#include "gdata.h"
#include <QFileDialog>

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
  return Map::createEmptyMap(ui->tileWidthSpinBox->value()
                             , ui->tileHeightSpinBox->value()
                             , ui->widthSpinBox->value()
                             , ui->heightSpinbox->value()
                             , name
                             , ui->luaInitLine->text().toStdString()
                             , *GData::getInstance()->getManager());
}

void NewMapDialog::sl_onFolderDialogClicked()
{
  QString file = QFileDialog::getOpenFileName(
        this,
        "select init lua script",
        "/home",
        "lua script (*.lua)");
  if (file == "") return;
  ui->luaInitLine->setText(file);
}


NewMapDialog::~NewMapDialog()
{
  delete ui;
}

}
}
