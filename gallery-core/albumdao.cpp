//#include "databasemanager.h"
#include "albumdao.h"
#include "album.h"
#include "databasemanager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariant>
#include <QDebug>


using namespace std;

#define GALLERY_ALBUMS_TABLE "albums"

AlbumDao::AlbumDao(QSqlDatabase &database):
    mDatabase(database)
{

}

void AlbumDao::init() const
{
    QSqlQuery query(mDatabase);
    QString queryString = "CREATE TABLE IF NOT EXISTS %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)";
    query.exec(queryString.arg(GALLERY_ALBUMS_TABLE));
    DatabaseManager::debugQuery(query);
}

void AlbumDao::addAlbum(Album &album) const
{
    QSqlQuery query(mDatabase)  ;
    QString queryString = "INSERT INTO %1 (name) VALUES (:name)";
    query.prepare(queryString.arg(GALLERY_ALBUMS_TABLE));
    query.bindValue(":name", album.name());
    query.exec();
    DatabaseManager::debugQuery(query);
    album.setId(query.lastInsertId().toInt());
}

void AlbumDao::updateAlbum(const Album &album) const
{
    QString queryString = "UPDATE %1 SET name = (:name) WHERE id = (:id)";
    QSqlQuery query(mDatabase);
    query.prepare(queryString.arg(GALLERY_ALBUMS_TABLE));
    query.bindValue(":name", album.name());
    query.bindValue(":id", album.id());
    query.exec();
    DatabaseManager::debugQuery(query);
}

void AlbumDao::removeAlbum(int id) const
{
    QString queryString = "DELETE FROM %1 WHERE id = (:id)";
    QSqlQuery query(mDatabase);
    query.prepare(queryString.arg(GALLERY_ALBUMS_TABLE));
    query.bindValue(":id", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

//QVector<Album *> AlbumDao::albums() const
//{
//    QString queryString = "SELECT * FROM %1";
//    QSqlQuery query(queryString.arg(GALLERY_ALBUMS_TABLE), mDatabase);
//    query.exec();
//    QVector<Album*> list;
//    while(query.next()){
//        Album* album = new Album();
//        album->setId(query.value("id").toInt());
//        album->setName(query.value("name").toString());
//        list.append(album);
//    }
//    return list;
//}

unique_ptr<vector<unique_ptr<Album> > > AlbumDao::albums() const
{
    QString queryString = "SELECT * FROM %1";
    QSqlQuery query(queryString.arg(GALLERY_ALBUMS_TABLE), mDatabase);
    query.exec();
    DatabaseManager::debugQuery(query);
    unique_ptr<vector<unique_ptr<Album>>> listUniquePointer(new vector<unique_ptr<Album>>());
    while(query.next()){
        unique_ptr<Album> album(new Album());
        album->setId(query.value("id").toInt());
        album->setName(query.value("name").toString());
        listUniquePointer->push_back(move(album));
    }
    return listUniquePointer;
}
