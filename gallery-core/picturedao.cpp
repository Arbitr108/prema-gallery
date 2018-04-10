#include "picturedao.h"
#include "databasemanager.h"
#include "picture.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariant>
#include <QDebug>

using namespace std;

#define GALLERY_PICTURES_TABLE "pictures"

PictureDao::PictureDao(QSqlDatabase &database):
    mDatabase(database)
{

}

void PictureDao::init() const
{
    if(!mDatabase.tables()
            .contains(GALLERY_PICTURES_TABLE)){
        QSqlQuery query(mDatabase);
        query.exec(QString("CREATE TABLE %1")
                   .arg(GALLERY_PICTURES_TABLE)
                   + " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   + "album_id INTEGER, "
                   + "url TEXT)");
        DatabaseManager::debugQuery(query);
    }    
}

void PictureDao::addPictureInAlbum(int albumId, Picture &picture) const
{
    QSqlQuery query(mDatabase);
    QString queryString = "INSERT INTO %1 (url, album_id) VALUES (:url, :album_id)";
    query.prepare(queryString.arg(GALLERY_PICTURES_TABLE));
    query.bindValue(":url", picture.fileUrl());
    query.bindValue(":album_id", albumId);
    query.exec();
    DatabaseManager::debugQuery(query);
    picture.setId(query.lastInsertId().toInt());
    picture.setAlbumId(albumId);
}

void PictureDao::removePicture(int id) const
{
    QSqlQuery query(mDatabase);
    QString queryString = "DELETE FROM %1 WHERE id = :id";
    query.prepare(queryString.arg(GALLERY_PICTURES_TABLE));
    query.bindValue(":id", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

void PictureDao::removePicturesForAlbum(int albumId) const
{
    QSqlQuery query(mDatabase);
    QString queryString = "DELETE FROM %1 WHERE album_id = :album_id";
    query.prepare(queryString.arg(GALLERY_PICTURES_TABLE));
    query.bindValue(":album_id", albumId);
    query.exec();
    DatabaseManager::debugQuery(query);
}

//QVector<Picture *> PictureDao::picturesForAlbum(int albumId) const
//{

//}

unique_ptr<vector<unique_ptr<Picture> > > PictureDao::picturesForAlbum(int albumId) const
{
    QSqlQuery query(mDatabase);
    QString queryString = "SELECT * FROM %1 WHERE album_id = :album_id";
    query.prepare(queryString.arg(GALLERY_PICTURES_TABLE));
    query.bindValue(":album_id", albumId);
    query.exec();
    DatabaseManager::debugQuery(query);
    unique_ptr<vector<unique_ptr<Picture>>> listUniquePointer(new vector<unique_ptr<Picture>>());
    while(query.next()) {
        unique_ptr<Picture> pictureUniquePointer(new Picture());
        pictureUniquePointer->setAlbumId(query.value("album_id").toInt());
        pictureUniquePointer->setFileUrl(query.value("url").toString());
        pictureUniquePointer->setId(query.value("id").toInt());
        listUniquePointer->push_back(move(pictureUniquePointer));
    }
    return listUniquePointer;
}
