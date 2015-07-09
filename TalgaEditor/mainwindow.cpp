#include <QFileDialog>
#include <QFile>
#include <QGraphicsPixmapItem>
#include <QListWidgetItem>
#include <QUndoStack>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  "assetlist.h"
#include "gdata.h"
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

      //for when maps are loaded
      connect(GData::getInstance(), SIGNAL(sig_mapChanged(EditorMap*)), ui->openGLWidget, SLOT(sl_updateChangedMap(EditorMap*)));
      connect(GData::getInstance(), SIGNAL(sig_mapChanged(EditorMap*)), ui->historyView, SLOT(sl_updateChangedMap(EditorMap*)));
      connect(GData::getInstance(), SIGNAL(sig_mapChanged(EditorMap*)), ui->layerStack, SLOT(sl_updateChangedMap(EditorMap*)));
      connect(GData::getInstance(), SIGNAL(sig_mapChanged(EditorMap*)), ui->assetList, SLOT(sl_updateChangedMap(EditorMap*)));
      connect(GData::getInstance(), SIGNAL(sig_mapChanged(EditorMap*)), ui->imageView, SLOT(sl_updateChangedMap(EditorMap*)));
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

void talga::editor::MainWindow::on_actionSave_triggered()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                             "untitled.tmap",
                             tr("Talga map (*.tmap)"));
    GData::getInstance()->sl_saveMap(fileName.toStdString());
}

void talga::editor::MainWindow::on_actionOpen_triggered()
{
  QString files = QFileDialog::getOpenFileName(
                          this,
                          "Select one or more files to open",
                          "/home",
                          "Talga map (*.tmap)");
    GData::getInstance()->sl_loadMap(files.toStdString());
}
