#include <QApplication>
#include <QDebug>
#include <QtWidgets/QTreeView>
#include <QDropEvent>
#include "pet_tree_model.h"

using namespace std;

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    
    PetTreeModel model;
    model.load("/tmp/simple_tree.json");

    auto treeView = new QTreeView;
    treeView->setModel(&model);
    treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    treeView->setDragEnabled(true);
    treeView->viewport()->setAcceptDrops(true);
    treeView->setDropIndicatorShown(true);
    treeView->setDragDropMode(QAbstractItemView::InternalMove);
    treeView->show();

    return app.exec();
}

