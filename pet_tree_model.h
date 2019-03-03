#if !defined(PET_TREE_MODEL_H)
#define PET_TREE_MODEL_H

#include <QAbstractItemModel>
#include "pet_tree_item.h"

class PetTreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit PetTreeModel(QObject *parent = nullptr) : QAbstractItemModel(parent), m_rootItem(nullptr), m_cutItem(nullptr) {}
    ~PetTreeModel() { delete m_rootItem; delete m_cutItem; }

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;

    QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    bool setHeaderData(int, Qt::Orientation, const QVariant&, int=Qt::EditRole) { return false; }
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);

    bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());

    Qt::DropActions supportedDragActions() const { return Qt::MoveAction; }
    Qt::DropActions supportedDropActions() const { return Qt::MoveAction; }
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *mimeData, Qt::DropAction action, int row, int column, const QModelIndex &parent);

    QModelIndex moveUp(const QModelIndex &index);
    QModelIndex moveDown(const QModelIndex &index);
    QModelIndex cut(const QModelIndex &index);
    QModelIndex paste(const QModelIndex &index);
    QModelIndex promote(const QModelIndex &index);
    QModelIndex demote(const QModelIndex &index);

    void clear();

    QString filename() const { return m_fileName; }
    void setFilename(const QString &fileName) { m_fileName = fileName; }
    bool load(const QString &fileName=QString());
    bool save(const QString &fileName=QString());

    QStringList pathForIndex(const QModelIndex &index) const;
    QModelIndex indexForPath(const QStringList &path) const;

private:
    PetTreeItem *itemForIndex(const QModelIndex &index) const;
    bool readPets();
    bool writePets() const;
    void announceItemChanged(PetTreeItem *item);
    QModelIndex indexForPath(const QModelIndex &parent, const QStringList &path) const;
    QModelIndex moveItem(PetTreeItem *parent, int oldRow, int newRow);

    PetTreeItem *m_rootItem;
    PetTreeItem *m_cutItem;
    QString m_fileName;
};


#endif
