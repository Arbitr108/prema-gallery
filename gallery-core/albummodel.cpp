#include "albummodel.h"

AlbumModel::AlbumModel(QObject* parent) :
    QAbstractListModel(parent),
    mDb(DatabaseManager::instance()),
    mAlbums(mDb.mAlbumDao.albums())
{

}

QModelIndex AlbumModel::addAlbum(const Album &album)
{
    int rowIndex = this->rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    std::unique_ptr<Album> insertedAlbum(new Album(album));
    mDb.mAlbumDao.addAlbum(*insertedAlbum);
    mAlbums->push_back(std::move(insertedAlbum));
    endInsertRows();
    return index(rowIndex);
}

void AlbumModel::addAlbumFromName(const QString &name)
{
    this->addAlbum(Album(name));
}

int AlbumModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mAlbums->size();
}

QVariant AlbumModel::data(const QModelIndex &index, int role) const
{
    if(!isIndexValid(index)){
        return QVariant();
    }
    //    Album& album = *mAlbums->at(index.row());
    const Album& album = *mAlbums->at(index.row());
    switch (role) {
    case Roles::IdRole:
        return album.id();
    case Roles::NameRole:
    case Qt::DisplayRole:
         return album.name();
    default:
        return QVariant();
    }
}

bool AlbumModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!isIndexValid(index) || role != Roles::NameRole){
        return false;
    }
    Album& album = *mAlbums->at(index.row());
    album.setName(value.toString());
    mDb.mAlbumDao.updateAlbum(album);
    emit dataChanged(index, index);
    return true;
}

bool AlbumModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)
    if( row < 0
            || count < 0
            || (row+count) > rowCount()
            || row > rowCount()){
        return false;
    }
    beginRemoveRows(QModelIndex(), row, count -1);
    int leftToRemove = count;
    while(leftToRemove--){
        const Album& album = *mAlbums->at(row + leftToRemove);
        mDb.mAlbumDao.removeAlbum(album.id());
    }
    mAlbums->erase(mAlbums->begin() + row, mAlbums->begin() + count);
    endRemoveRows();
    return true;
}

QHash<int, QByteArray> AlbumModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::IdRole] = "id";
    roles[Roles::NameRole] = "name";
    return roles;
}

bool AlbumModel::isIndexValid(const QModelIndex &index) const
{
    return index.row() > 0 && index.row() < rowCount() && index.isValid();
}
