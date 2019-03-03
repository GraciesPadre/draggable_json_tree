#if !defined(PET_TREE_ITEM_H)
#define PET_TREE_ITEM_H

#include <QList>
#include <QString>

class PetTreeItem {
public:
    explicit PetTreeItem(PetTreeItem *parent = nullptr, const QString &name = QString(), const QString &breed = QString());
    ~PetTreeItem() { qDeleteAll(m_children); }

    QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }

    QString breed() const { return m_breed; }
    void setBreed(const QString &breed) { m_breed = breed; }

    PetTreeItem* parent() const { return m_parent; }
    PetTreeItem* childAt(int row) const { return m_children.value(row); }
    int rowOfChild(PetTreeItem *child) const { return m_children.indexOf(child); }
    bool hasChildren() const { return ! m_children.isEmpty(); }
    int childCount() const { return m_children.count(); }
    QList<PetTreeItem*> children() const { return m_children; }

    void insertChild(int row, PetTreeItem *item) { item->m_parent = this; m_children.insert(row, item); }
    void addChild(PetTreeItem *item) { item->m_parent = this; m_children << item; }
    void swapChildren(int oldRow, int newRow) { m_children.swap(oldRow, newRow); }
    PetTreeItem* takeChild(int row);

    static PetTreeItem* load(const QJsonValue& value);

    friend std::ostream& operator<< (std::ostream &out, const PetTreeItem &item);

private:
    static PetTreeItem* load(const QJsonValue& value, PetTreeItem *parent);

    QString m_name;
    QString m_breed;
    PetTreeItem *m_parent;
    QList<PetTreeItem*> m_children;
    QString m_filename;
};

#endif
