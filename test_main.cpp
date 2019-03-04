#include "gtest/gtest.h"
#include "pet_tree_item.h"
#include "pet_tree_model.h"
#include <QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

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

TEST(PetTreeItemTests, HasChildren) {
    PetTreeItem hiddenRootItem;

    new PetTreeItem(&hiddenRootItem);

    ASSERT_TRUE(hiddenRootItem.hasChildren() == true);
}

TEST(PetTreeItemTests, ChildCount) {
    PetTreeItem hiddenRootItem;

    new PetTreeItem(&hiddenRootItem);

    ASSERT_TRUE(hiddenRootItem.childCount() == 1);
}

TEST(PetTreeItemTests, Children) {
    PetTreeItem hiddenRootItem;

    auto firstChild = new PetTreeItem(&hiddenRootItem);

    auto children = hiddenRootItem.children();

    ASSERT_TRUE(children.count() == 1);

    for (auto child : children) {
        ASSERT_TRUE(child == firstChild);
    }
}

TEST(PetTreeItemTests, SwapChildren) {
    PetTreeItem hiddenRootItem;

    auto firstChild = new PetTreeItem(&hiddenRootItem, "first");
    auto secondChild = new PetTreeItem(&hiddenRootItem, "second");

    ASSERT_TRUE(hiddenRootItem.childCount() == 2);

    hiddenRootItem.swapChildren(1,0);

    ASSERT_TRUE(hiddenRootItem.childAt(0) == secondChild);
    ASSERT_TRUE(hiddenRootItem.childAt(1) == firstChild);
}

TEST(PetTreeItemTests, TakeChild) {
    PetTreeItem hiddenRootItem;

    auto firstChild = new PetTreeItem(&hiddenRootItem, "first");
    auto secondChild = new PetTreeItem(&hiddenRootItem, "second");

    auto removedChild = hiddenRootItem.takeChild(0);

    ASSERT_TRUE(removedChild == firstChild);
    ASSERT_TRUE(hiddenRootItem.childAt(0) == secondChild);
}

TEST(PetTreeItemTests, Load) {
    QFile file("/tmp/simple_tree.json");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly));

    struct FileFinalizer {
        QFile &m_file;

        explicit FileFinalizer(QFile &file) : m_file(file) {}
        ~FileFinalizer() { m_file.close(); }
    } fileFinalizer(file);

    auto const& jsonDocument = QJsonDocument::fromJson(file.readAll());

    ASSERT_FALSE(jsonDocument.isEmpty());
    ASSERT_FALSE(jsonDocument.isNull());

    auto rootItem = PetTreeItem::load(QJsonValue(jsonDocument.object()));

    ASSERT_TRUE(rootItem->childCount() == 2);
    ASSERT_TRUE(rootItem->childAt(0)->childCount() == 2);
    ASSERT_TRUE(rootItem->childAt(1)->childCount() == 3);
    ASSERT_TRUE(rootItem->childAt(2) == nullptr);
}

TEST(PetTreeModelTests, Load) {
    PetTreeModel petTreeModel;
    ASSERT_TRUE(petTreeModel.load("/tmp/simple_tree.json"));
    ASSERT_TRUE(petTreeModel.hasChildren());
    ASSERT_TRUE(petTreeModel.rowCount() == 2);
}

TEST(PetTreeModelTests, LoadFails) {
    PetTreeModel petTreeModel;
    ASSERT_FALSE(petTreeModel.load("/path/to/non/existent/file"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
