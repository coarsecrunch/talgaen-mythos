#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QString>

namespace Ui {
  class MainWindow;
}

class QListWidgetItem;
class QGraphicsScene;
class QPixmap;
class QGraphicsPixmapItem;
class QActionGroup;

namespace talga
{
  namespace editor
  {

    class MainWindow : public QMainWindow
    {
      Q_OBJECT

    public:
      explicit MainWindow(QWidget *parent = 0);
      ~MainWindow();

      Ui::MainWindow* getUi() {return ui;}
    signals:
      void sig_setModeCollisionEdit();
      void sig_setModeTileEdit();
      void sig_addRect();
      void sig_addTri();
    private slots:

      void on_actionLoad_Assets_triggered();

      void on_actionSave_triggered();

      void on_actionOpen_triggered();

      void on_actionSave_as_triggered();

      void on_actionAssetManager_triggered(bool checked);

      void on_actionAssetViewer_triggered(bool checked);

      void on_actionHistory_triggered(bool checked);

      void on_actionLayers_triggered(bool checked);

      void on_actionNew_triggered();

      void on_actionCollisionEditingMode_triggered(bool checked);

      void on_actionTile_Edit_Mode_triggered();

      void on_actionTile_Edit_Mode_triggered(bool checked);

      void on_actionCreate_New_Rectangle_Geometry_triggered();

      void on_actionCreate_New_Triangle_Geometry_triggered();

    protected:
      Ui::MainWindow *ui;

      QActionGroup* editModes;
      QActionGroup* collisionEditModeActions;
      QGraphicsScene* pmImageViewScene;
      QMap<QString, QPixmap* > mTextures;
    };

  }
}
#endif // MAINWINDOW_H
