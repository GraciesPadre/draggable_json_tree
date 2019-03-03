#include "gtest/gtest.h"
#include "pet_tree_item.h"
#include <QString>

TEST(PetTreeItemTests, Add1Child) {
    PetTreeItem hiddenRootItem;

    const char * const name = "Gracie";
    const char * const breed = "Eskimo";

    auto *firstChild = new PetTreeItem;
    firstChild->setName(name);
    firstChild->setBreed(breed);

    hiddenRootItem.addChild(firstChild);

    ASSERT_TRUE(hiddenRootItem.childAt(0)->name() == name);
    ASSERT_TRUE(hiddenRootItem.childAt(0)->breed() == breed);
}

TEST(PetTreeItemTests, RowFor1Child) {
    PetTreeItem hiddenRootItem;

    auto *firstChild = new PetTreeItem(&hiddenRootItem);

    ASSERT_TRUE(hiddenRootItem.rowOfChild(firstChild) == 0);
}

/*
 * PetTreeItem* parent() const { return m_parent; }
    PetTreeItem* childAt(int row) const { return m_children.value(row); }
    int rowOfChild(PetTreeItem *child) const { return m_children.indexOf(child); }
    bool hasChildren() const { return ! m_children.isEmpty(); }
    int childCount() const { return m_children.count(); }
    QList<PetTreeItem*> children() const { return m_children; }

    void insertChild(int row, PetTreeItem *item) { item->m_parent = this; m_children.insert(row, item); }
    void addChild(PetTreeItem *item) { item->m_parent = this; m_children << item; }
    void swapChildren(int oldRow, int newRow) { m_children.swap(oldRow, newRow); }
    PetTreeItem* takeChild(int row);

    static PetTreeItem* load(const QJsonValue& value, PetTreeItem *parent = nullptr);
 */
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
