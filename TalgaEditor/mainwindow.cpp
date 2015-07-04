#include <QFileDialog>
#include <QFile>
#include <QGraphicsPixmapItem>
#include <QListWidgetItem>
#include <QUndoStack>

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

      //for updating the display every time you undo or redo
      connect(ui->historyView->stack(), SIGNAL(indexChanged(int)), ui->openGLWidget, SLOT(sl_updateGL()));
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
