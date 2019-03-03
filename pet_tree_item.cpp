#include <QJsonValue>
#include <QJsonObject>
#include <QVariant>
#include "pet_tree_item.h"

PetTreeItem::PetTreeItem(PetTreeItem *parent, const QString &name, const QString &breed)
    : m_parent(parent), m_name(name), m_breed(breed)
{
    if(m_parent) {
        m_parent->addChild(this);
    }
}

PetTreeItem* PetTreeItem::takeChild(int row) {
    PetTreeItem *item = m_children.takeAt(row);
    Q_ASSERT(item);
    item->m_parent = nullptr;
    return item;
}

PetTreeItem* PetTreeItem::load(const QJsonValue &value, PetTreeItem *parent) {
    auto *newItem = new PetTreeItem(parent);
    newItem->setName("hidden root");

    if ( value.isObject())
    {
        auto jsonObject = value.toObject();
        for (const auto &key : jsonObject.keys()){
            QJsonValue subValue = value.toObject().value(key);
            auto *child = load(subValue,newItem);
            child->setName(key);
        }
    } else {
        newItem->setBreed(value.toVariant().toString());
    }

    return newItem;
}

std::ostream& operator<< (std::ostream &out, const PetTreeItem &item) {
    out << "Name: " << item.m_name.toStdString() << ", Breed: " << item.m_breed.toStdString() << ", ";
    out << "Parent: " << item.m_parent << ", Num children: " << item.m_children.count() << '\n';

    for (auto child : item.m_children) {
        out << "    Child: " << child->m_name.toStdString() << ", " << child->m_breed.toStdString() << '\n';
    }

    return out;
}
