#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include "albumdao.h"
#include "picturedao.h"

class QSqlDatabase;
class QSqlQuery;

const QString DATABASE_FILENAME = "gallery.db";

class DatabaseManager
{
public:
    static DatabaseManager& instance();
    static void debugQuery(const QSqlQuery& query);
    ~DatabaseManager();

protected:
    DatabaseManager(const QString& path = DATABASE_FILENAME);
    DatabaseManager& operator =(const DatabaseManager* rhs);

private:
    QSqlDatabase* mDatabase;

public:
    const AlbumDao mAlbumDao;
    const PictureDao mPictureDao;
};

#endif // DATABASEMANAGER_H
