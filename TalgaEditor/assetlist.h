#ifndef ASSETLIST_H
#define ASSETLIST_H

#include <QListWidget>
#include <QString>
#include <QMap>

class QMouseEvent;
class QPixmap;
class QListWidgetItem;

namespace talga
{
namespace editor
{

class AssetList : public QListWidget
{
    Q_OBJECT

public:
    explicit AssetList( QWidget* parent = nullptr);
    ~AssetList();
    void sl_chooseAssets();
signals:
    void sig_textureSelected(QPixmap*);

public slots:
    void sl_assetSelected(QListWidgetItem*);

protected:
    QMap<QString, QPixmap*> mAssets;
};

}
}
#endif // ASSETLIST_H
