#ifndef ASSETLIST_H
#define ASSETLIST_H

#include <QTreeWidget>
#include <QString>
#include <QMap>

class QMouseEvent;
class QPixmap;
class QTreeWidgetItem;

namespace talga
{
  class AAsset;

  namespace editor
  {
    typedef QPair<QString, QImage*> TextureAsset;
    class EditorMap;

    class AssetList : public QTreeWidget
    {
      Q_OBJECT

    public:
      explicit AssetList( QWidget* parent = nullptr);
      ~AssetList();

      //for dragging and dropping items into the GLContext
      void mousePressEvent(QMouseEvent *e);
      void mouseMoveEvent(QMouseEvent *e);
    signals:
      // to signal to GLContext to add an asset to the manager
      void sig_assetChosen(QString path);

      void sig_textureSelected(TextureAsset);
      void sig_mapSelcted();
      void sig_scriptSelected();
      void sig_assetSelected(AAsset*);

    public slots:
      void sl_assetSelected(QTreeWidgetItem*, int);
      void sl_chooseAssets();
      void sl_loadAsset(QString path);
      void sl_updateChangedMap(EditorMap*);
      void sl_updateAssetsNames();
    protected:
      std::vector<const AAsset*> mAssets;
      QTreeWidgetItem* mTexturesFolder;
      QTreeWidgetItem* mMapsFolder;
      QTreeWidgetItem* mScriptsFolder;
      QTreeWidgetItem* mLevelFolder;

      QPoint dragStartPosition;
      EditorMap* mMap;
    };

  }
}
#endif // ASSETLIST_H
