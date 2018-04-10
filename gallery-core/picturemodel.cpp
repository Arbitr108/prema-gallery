#include "picturemodel.h"
#include "databasemanager.h"
#include "album.h"
#include "albummodel.h"

using namespace std;

PictureModel::PictureModel(const AlbumModel& albumModel, QObject *parent) :
    QAbstractListModel(parent),
    mDb(DatabaseManager::instance()),
    mAlbumId(-1),
    mPictures(new vector<unique_ptr<Picture>>())
{
    connect(&albumModel, &AlbumModel::rowsRemoved, this, &PictureModel::deletePicturesForAlbum);
}

QModelIndex PictureModel::addPicture(const Picture &picture)
{
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    unique_ptr<Picture> newPicture(new Picture(picture));
    mDb.mPictureDao.addPictureInAlbum(mAlbumId, *newPicture);
    mPictures->push_back(move(newPicture));
    endInsertRows();
    return index(rowIndex, 0);
}

int PictureModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mPictures->size();
}

QVariant PictureModel::data(const QModelIndex &index, int role) const
{
    if(!isIndexValid(index)){
        return QVariant();
    }
    const Picture& picture = *mPictures->at(index.row());
    switch (role) {
    case Qt::DisplayRole:
        return picture.fileUrl().fileName();
    case Roles::UrlRole:
        return picture.fileUrl();
    case Roles::FilePathRole:
        return picture.fileUrl().toLocalFile();
    default:
        return QVariant();
    }
}

bool PictureModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(row < 0 || count < 0 || row >= rowCount() || (row + count) > rowCount()){
        return false;
    }
    beginRemoveRows(parent, row, count -1);
    //remove from db
    int leftToRemove = count;
    while(leftToRemove--){
        const Picture& picture = *mPictures->at(row + leftToRemove);
        mDb.mPictureDao.removePicture(picture.id());
    }
    //remove from model
    mPictures->erase(mPictures->begin() + row, mPictures->begin() + count);
    endRemoveRows();
    return true;
}

QHash<int, QByteArray> PictureModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::FilePathRole] = "filepath";
    return roles;
}

void PictureModel::addPictureFromUrl(const QUrl &fileUrl)
{
    addPicture(Picture(fileUrl));
}

void PictureModel::setAlbumId(int albumId)
{
    beginResetModel();
    mAlbumId = albumId;
    loadPictures(albumId);
    endResetModel();
}

/**
  TODO: I think it should be at least protected,
  but never public
 * @brief PictureModel::clearAlbum
 */
void PictureModel::clearAlbum()
{
    setAlbumId(-1);
}

/**
  TODO: Why we do not reset the model here??
 * @brief PictureModel::deletePicturesForAlbum
 */
void PictureModel::deletePicturesForAlbum()
{
//    beginResetModel();
    mDb.mPictureDao.removePicturesForAlbum(mAlbumId);
    clearAlbum();
//    endResetModel();
}

void PictureModel::loadPictures(int albumId)
{
    if(albumId <= 0){
        mPictures.reset(new vector<unique_ptr<Picture>>());
        return;
    }
    mPictures = mDb.mPictureDao.picturesForAlbum(albumId);
}

bool PictureModel::isIndexValid(const QModelIndex &index) const
{
    return index.row() > 0 && index.row() < rowCount() && index.isValid();
}
