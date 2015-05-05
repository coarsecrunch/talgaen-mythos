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
namespace editor
{

class AssetList : public QTreeWidget
{
    Q_OBJECT

public:
    explicit AssetList( QWidget* parent = nullptr);
    ~AssetList();
    void sl_chooseAssets();
signals:
    void sig_textureSelected(QPixmap*);
    void sig_mapSelcted();
    void sig_scriptSelected();
public slots:
    void sl_assetSelected(QTreeWidgetItem*, int);

protected:
    QMap<QString, QPixmap*> mAssets;
    QTreeWidgetItem* mTexturesFolder;
    QTreeWidgetItem* mMapsFolder;
    QTreeWidgetItem* mScriptsFolder;
};

}
}
#endif // ASSETLIST_H
