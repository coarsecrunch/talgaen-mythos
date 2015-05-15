#include <QFileDialog>
#include <QFile>
#include <QGraphicsPixmapItem>
#include <QListWidgetItem>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  "assetlist.h"
namespace talga
{
  namespace editor
  {

    MainWindow::MainWindow(QWidget *parent) :
      QMainWindow(parent),
      ui(new Ui::MainWindow),
      pmImageViewScene(new QGraphicsScene),
      mTextures(),
      mTileWidth(32),
      mTileHeight(32)
    {
      ui->setupUi(this);
    }

    MainWindow::~MainWindow()
    {
      pmImageViewScene->clear();
      delete pmImageViewScene;

      qDeleteAll(mTextures.values());
      mTextures.clear();

      delete ui;
    }

  }
}

void talga::editor::MainWindow::on_actionLoad_Assets_triggered()
{
  ui->assetList->sl_chooseAssets();
}
