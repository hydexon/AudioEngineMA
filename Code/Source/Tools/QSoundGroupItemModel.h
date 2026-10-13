#pragma once

#include <QAbstractItemModel>
#include "Clients/MASoundGrpManager.h"

namespace AudioEngineMA
{

class SoundGroupManager;
class SoundGroup;

class QSoundGroupItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    QSoundGroupItemModel(SoundGroupManager* soundManager);
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    Qt::DropActions supportedDropActions() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
private:
    int FindRowOfSoundGroup(SoundGroup* group);
    SoundGroupManager* m_soundGroupManager;
};

}