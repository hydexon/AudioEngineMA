#include "QSoundGroupItemModel.h"

#include "Clients/MASoundGrpManager.h"
#include <AzCore/std/smart_ptr/unique_ptr.h>

namespace AudioEngineMA
{



QSoundGroupItemModel::QSoundGroupItemModel(SoundGroupManager *soundManager)
    : m_soundGroupManager(soundManager)
{

}

QModelIndex QSoundGroupItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    SoundGroup* parentGrp = parent.isValid()
            ? static_cast<SoundGroup*>(parent.internalPointer())
            : nullptr;

    SoundGroup* childGrp = parentGrp
            ? (row >= 0 && row < parentGrp->GetChildren().size() ? parentGrp->GetChildren().at(row).get() : nullptr)
            : m_soundGroupManager->GetRootSoundGroupAtIndex(row);

    return createIndex(row, column, childGrp);
}

QModelIndex QSoundGroupItemModel::parent(const QModelIndex &child) const
{
    if(!child.isValid())
        return QModelIndex();

    SoundGroup* childGrp = static_cast<SoundGroup*>(child.internalPointer());
    if(childGrp->GetParent() == nullptr)
        return QModelIndex();

    return QModelIndex();
}

int QSoundGroupItemModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return m_soundGroupManager->GetRootNodeCount();

    SoundGroup* parentGrp = static_cast<SoundGroup*>(parent.internalPointer());
    return parentGrp->GetChildren().size();
}

int QSoundGroupItemModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant QSoundGroupItemModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    SoundGroup* group = static_cast<SoundGroup*>(index.internalPointer());
    if(role == Qt::DisplayRole || role == Qt::EditRole) {
        return QString::fromStdString(group->GetName().GetCStr());
    }

    return QVariant();
}

QMimeData *QSoundGroupItemModel::mimeData(const QModelIndexList &indexes) const
{
    return nullptr;
}

bool QSoundGroupItemModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    return false;
}

Qt::DropActions QSoundGroupItemModel::supportedDropActions() const
{
    return Qt::DropAction::IgnoreAction;
}

Qt::ItemFlags QSoundGroupItemModel::flags(const QModelIndex &index) const
{
    return Qt::ItemFlag::NoItemFlags;
}

int QSoundGroupItemModel::FindRowOfSoundGroup(SoundGroup *group)
{
    if(!group)
        return -1;

    SoundGroup* parent = group->GetParent();
    if(parent == nullptr)
    {
    }
    return -1; //not found.
}

}