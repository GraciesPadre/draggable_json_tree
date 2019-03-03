#include <QMimeData>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QtCore/QDataStream>
#include <QtCore/QStringListModel>
#include <iostream>
#include "pet_tree_model.h"

namespace {
    const int ColumnCount = 2;
    enum Column { Name, Breed };
    const QString MimeType = "text/json_tree";
}

Qt::ItemFlags PetTreeModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);

    if (index.isValid()) {
        theFlags |= Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    } else {
        theFlags |= Qt::ItemIsDropEnabled;
    }

    return theFlags;
}

QVariant PetTreeModel::data(const QModelIndex &index, int role) const {
    if (!m_rootItem || !index.isValid() || index.column() < 0 || index.column() >= ColumnCount) {
        return QVariant();
    }

    if (PetTreeItem *item = itemForIndex(index)) {
        std::cout << "PetTreeModel::data" << *item << '\n';
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            switch (index.column()) {
                case Name: return item->name();
                case Breed: return item->breed();
                default: Q_ASSERT(false);
            }
        }
    }
    return QVariant();
}

PetTreeItem* PetTreeModel::itemForIndex(const QModelIndex &index) const {
    if (index.isValid()) {
        // This cast is here in the event the model has items of differing types
        if (auto *item = static_cast<PetTreeItem*>(index.internalPointer())) {
            return item;
        }
    }

    return m_rootItem;
}

QVariant PetTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case Name: return tr("Name");
            case Breed: return tr("Breed");
            default: Q_ASSERT(false);
        }
    }

    return QVariant();
}

int PetTreeModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid() && parent.column() != 0) {
        return 0;
    }

    PetTreeItem *parentItem = itemForIndex(parent);

    return parentItem ? parentItem->childCount() : 0;
}

int PetTreeModel::columnCount(const QModelIndex &parent) const {
    return parent.isValid() && parent.column() != 0 ? 0 : ColumnCount;
}

QModelIndex PetTreeModel::index(int row, int column, const QModelIndex &parent) const {
    if ( ! m_rootItem || row < 0 || column < 0 || column >= ColumnCount || (parent.isValid() && parent.column() != 0)) {
        return QModelIndex();
    }

    PetTreeItem *parentItem = itemForIndex(parent);
    Q_ASSERT(parentItem);
    if (PetTreeItem *item = parentItem->childAt(row)) {
        return createIndex(row, column, item);
    }

    return QModelIndex();
}

QModelIndex PetTreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) {
        return QModelIndex();
    }

    if (PetTreeItem *childItem = itemForIndex(index)) {
        if (PetTreeItem *parentItem = childItem->parent()) {
            if (parentItem == m_rootItem) {
                return QModelIndex();
            }

            if (PetTreeItem *grandParentItem = parentItem->parent()) {
                int row = grandParentItem->rowOfChild(parentItem);
                return createIndex(row, 0, parentItem);
            }
        }
    }

    return QModelIndex();
}

bool PetTreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) {
        return false;
    }

    if (PetTreeItem *item = itemForIndex(index)) {
        if (index.column() == Name) {
            item->setName(value.toString());
        } else {
            item->setBreed(value.toString());
        }

        emit dataChanged(index, index);

        return true;
    }

    return false;
}

bool PetTreeModel::insertRows(int row, int count, const QModelIndex &parent) {
    /*if ( ! m_rootItem) {
        m_rootItem = new PetTreeItem;
    }

    PetTreeItem *parentItem = parent.isValid() ? itemForIndex(parent) : m_rootItem;

    std::cout << parentItem << '\n';

    beginInsertRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        PetTreeItem *item = new PetTreeItem(nullptr, tr("New Pet"), "Gracie");
        parentItem->insertChild(row, item);
    }
    endInsertRows();

    return true;
     */
    /*
    PetTreeItem *parentItem = m_rootItem;

    if (parent.isValid()) {
        PetTreeItem *item = static_cast<PetTreeItem*>(parent.internalPointer());
        if (item)
            parentItem = item;
    }

    bool success;

    beginInsertRows(parent, row, row + count - 1);
    parentItem->
    success = parentItem->insertChildren(row, count, m_rootItem->columnCount());
    endInsertRows();
     */

    /*
     * bool TreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        TreeItem *item = new TreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}


    return success;

    if ( ! m_rootItem) {
        m_rootItem = new PetTreeItem;
    }

    PetTreeItem *parentItem = parent.isValid() ? itemForIndex(parent) : m_rootItem;

    std::cout << parentItem << '\n';

    beginInsertRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        PetTreeItem *item = new PetTreeItem(nullptr, tr("New Pet"), "Gracie");
        parentItem->insertChild(row, item);
    }
    endInsertRows();
*/
    return true;

}

bool PetTreeModel::removeRows(int row, int count, const QModelIndex &parent) {
    if ( ! m_rootItem) {
        return false;
    }

    PetTreeItem *item = parent.isValid() ? itemForIndex(parent) : m_rootItem;

    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        delete item->takeChild(row);
    }
    endRemoveRows();

    return true;
}

QStringList PetTreeModel::mimeTypes() const {
    return QStringList() << MimeType;
}

QMimeData* PetTreeModel::mimeData(const QModelIndexList &indexes) const {
    Q_ASSERT(indexes.count());

    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    for (QModelIndex index : indexes) {
        if (index.isValid()) {
            QString text = data(index, Qt::DisplayRole).toString();
            std::cout << "mimeData text" << text.toStdString() << '\n';
            stream << text;
        }
    }

    mimeData->setData(MimeType, encodedData);
    return mimeData;
}

bool PetTreeModel::dropMimeData(const QMimeData *mimeData, Qt::DropAction action, int row, int column, const QModelIndex &parent) {
    if (action == Qt::IgnoreAction) {
        return true;
    }

    if ( ! mimeData->hasFormat(MimeType)) {
        return false;
    }

    if (column > 0) {
        return false;
    }

    int position;

    if (row != -1) {
        position = row;
    } else if (parent.isValid()) {
        position = parent.row();
    } else {
        position = rowCount(QModelIndex());
    }

    QByteArray encodedData = mimeData->data(MimeType);
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    /* Retrieve row id */
    QList<int> rowIdList;
    while (!stream.atEnd()) {
        QString text;
        stream >> text;
        rowIdList << text.toInt();
    }

    /* Insert rows (one by one) */
            foreach(int rowId, rowIdList) {
            insertRow(position, parent);
        }

    return true;
}

QModelIndex PetTreeModel::moveUp(const QModelIndex &index) {
    if (!index.isValid() || index.row() <= 0) {
        return index;
    }

    PetTreeItem *item = itemForIndex(index);
    Q_ASSERT(item);
    PetTreeItem *parent = item->parent();
    Q_ASSERT(parent);

    return moveItem(parent, index.row(), index.row() - 1);
}

QModelIndex PetTreeModel::moveItem(PetTreeItem *parent, int oldRow,int newRow) {
    Q_ASSERT(0 <= oldRow && oldRow < parent->childCount() && 0 <= newRow && newRow < parent->childCount());

    parent->swapChildren(oldRow, newRow);
    QModelIndex oldIndex = createIndex(oldRow, 0, parent->childAt(oldRow));
    QModelIndex newIndex = createIndex(newRow, 0, parent->childAt(newRow));
    emit dataChanged(oldIndex, newIndex);

    return newIndex;
}

QModelIndex PetTreeModel::moveDown(const QModelIndex &index) {
    if (!index.isValid()) {
        return index;
    }

    PetTreeItem *item = itemForIndex(index);
    Q_ASSERT(item);

    PetTreeItem *parent = item->parent();
    int newRow = index.row() + 1;

    if (!parent || parent->childCount() <= newRow) {
        return index;
    }

    return moveItem(parent, index.row(), newRow);
}

QModelIndex PetTreeModel::cut(const QModelIndex &index) {
    if (!index.isValid()) {
        return index;
    }

    delete m_cutItem;
    m_cutItem = itemForIndex(index);
    Q_ASSERT(m_cutItem);


    PetTreeItem *parent = m_cutItem->parent();
    Q_ASSERT(parent);
    int row = parent->rowOfChild(m_cutItem);
    Q_ASSERT(row == index.row());

    beginRemoveRows(index.parent(), row, row);
    PetTreeItem *child = parent->takeChild(row);
    endRemoveRows();

    Q_ASSERT(child == m_cutItem);

    if (row > 0) {
        --row;
        return createIndex(row, 0, parent->childAt(row));
    }

    if (parent != m_rootItem) {
        PetTreeItem *grandParent = parent->parent();
        Q_ASSERT(grandParent);
        return createIndex(grandParent->rowOfChild(parent), 0, parent);
    }

    return QModelIndex();
}

QModelIndex PetTreeModel::paste(const QModelIndex &index) {
    if (!index.isValid() || ! m_cutItem) {
        return index;
    }

    PetTreeItem *sibling = itemForIndex(index);
    Q_ASSERT(sibling);

    PetTreeItem *parent = sibling->parent();
    Q_ASSERT(parent);

    int row = parent->rowOfChild(sibling) + 1;

    beginInsertRows(index.parent(), row, row);
    parent->insertChild(row, m_cutItem);
    PetTreeItem *child = m_cutItem;
    m_cutItem = nullptr;
    endInsertRows();

    return createIndex(row, 0, child);
}

QModelIndex PetTreeModel::promote(const QModelIndex &index) {
    if (!index.isValid()) {
        return index;
    }

    PetTreeItem *item = itemForIndex(index);
    Q_ASSERT(item);
    PetTreeItem *parent = item->parent();
    Q_ASSERT(parent);

    if (parent == m_rootItem) {
        return index; // Already a top-level item
    }

    int row = parent->rowOfChild(item);
    PetTreeItem *child = parent->takeChild(row);
    Q_ASSERT(child == item);
    PetTreeItem *grandParent = parent->parent();
    Q_ASSERT(grandParent);

    row = grandParent->rowOfChild(parent) + 1;
    grandParent->insertChild(row, child);
    QModelIndex newIndex = createIndex(row, 0, child);

    emit dataChanged(newIndex, newIndex);

    return newIndex;
}

QModelIndex PetTreeModel::demote(const QModelIndex &index) {
    if (!index.isValid()) {
        return index;
    }

    PetTreeItem *item = itemForIndex(index);
    Q_ASSERT(item);
    PetTreeItem *parent = item->parent();
    Q_ASSERT(parent);

    int row = parent->rowOfChild(item);
    if (row == 0) {
        return index; // No preceding sibling to move this under
    }

    PetTreeItem *child = parent->takeChild(row);
    Q_ASSERT(child == item);
    PetTreeItem *sibling = parent->childAt(row - 1);
    Q_ASSERT(sibling);

    sibling->addChild(child);
    QModelIndex newIndex = createIndex(sibling->childCount() - 1, 0, child);

    emit dataChanged(newIndex, newIndex);

    return newIndex;
}

void PetTreeModel::clear() {
    delete m_rootItem;
    m_rootItem = nullptr;
    delete m_cutItem;
    m_cutItem = nullptr;

    beginResetModel();
    endResetModel();
}

bool PetTreeModel::load(const QString &fileName) {
    if ( ! fileName.isEmpty()) {
        m_fileName = fileName;
    }

    if (m_fileName.isEmpty()) {
        return false;
    }

    QFile file(m_fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    clear();

    m_rootItem = new PetTreeItem;

    auto result = readPets();

    beginResetModel();
    endResetModel();

    return result;
}

bool PetTreeModel::readPets() {
    QFile file(m_fileName);

    if ( ! file.open(QIODevice::ReadOnly)) {
        return false;
    }

    struct FileFinalizer {
        QFile &m_fileToClose;

        explicit FileFinalizer(QFile &fileToClose) : m_fileToClose(fileToClose) {}
        ~FileFinalizer() { m_fileToClose.close(); }
    } fileFinalizer(file);

    auto const& jsonDocument = QJsonDocument::fromJson(file.readAll());

    if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
        return false;
    }

    beginResetModel();
    delete m_rootItem;

    m_rootItem = PetTreeItem::load(QJsonValue(jsonDocument.object()));

    endResetModel();

    return true;
}

bool PetTreeModel::save(const QString &fileName) {
    return true;
}

bool PetTreeModel::writePets() const {
    return true;
}

QStringList PetTreeModel::pathForIndex(const QModelIndex &index) const {
    QStringList path;
    QModelIndex thisIndex = index;

    while (thisIndex.isValid()) {
        path.prepend(data(thisIndex).toString());
        thisIndex = thisIndex.parent();
    }

    return path;
}

QModelIndex PetTreeModel::indexForPath(const QStringList &path) const {
    return indexForPath(QModelIndex(), path);
}

QModelIndex PetTreeModel::indexForPath(const QModelIndex &parent, const QStringList &path) const {
    if (path.isEmpty()) {
        return QModelIndex();
    }

    for (int row = 0; row < rowCount(parent); ++row) {
        QModelIndex thisIndex = index(row, 0, parent);
        if (data(thisIndex).toString() == path.at(0)) {
            if (path.count() == 1) {
                return thisIndex;
            }

            thisIndex = indexForPath(thisIndex, path.mid(1));

            if (thisIndex.isValid()) {
                return thisIndex;
            }
        }
    }

    return QModelIndex();
}

void PetTreeModel::announceItemChanged(PetTreeItem *item) {
    if (item == m_rootItem) {
        return;
    }

    PetTreeItem *parent = item->parent();
    Q_ASSERT(parent);

    int row = parent->rowOfChild(item);
    QModelIndex startIndex = createIndex(row, static_cast<int>(Name),item);
    QModelIndex endIndex = createIndex(row, static_cast<int>(Breed),item);
    emit dataChanged(startIndex, endIndex);

    // Update the parent & parent's parent etc too
    announceItemChanged(parent);
}
