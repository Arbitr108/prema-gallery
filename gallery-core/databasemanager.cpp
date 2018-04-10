#include "databasemanager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QStandardPaths>

DatabaseManager &DatabaseManager::instance()
{
    #if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    QFile assertDbFile(":/database/res/db/"  + DATABASE_FILENAME);
    QString destinationDbFile = QStandardPaths::writableLocation(
                QStandardPaths::AppLocalDataLocation)
            .append("/" + DATABASE_FILENAME);
    if(!QFile::exists(destinationDbFile)){
        assertDbFile.copy(destinationDbFile);
        QFile::setPermissions(destinationDbFile,
                              QFile::WriteOwner | QFile::ReadOwner);
    }
    static DatabaseManager singleton(destinationDbFile);
    #else
    static DatabaseManager singleton;
    #endif
    return singleton;
}

DatabaseManager::~DatabaseManager()
{
    mDatabase->close();
    delete mDatabase;
}

DatabaseManager::DatabaseManager(const QString &path):
    mDatabase(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"))),
    mAlbumDao(*mDatabase),
    mPictureDao(*mDatabase)
{
    mDatabase->setDatabaseName(path);
    bool openStatus = mDatabase->open();
    qDebug() << "Database connection: " << (openStatus ? "OK" : "Error") << path;
  //  QHash<QString, Dao>::const_iterator registerIterator = mDaos.constBegin();
    //while (registerIterator != mDaos.constEnd()) {
//        registerIterator.value().init();
//        ++registerIterator;
//    }
    mAlbumDao.init();
    mPictureDao.init();
}

DatabaseManager &DatabaseManager::operator =(const DatabaseManager *rhs)
{
    Q_UNUSED(rhs)
}

void DatabaseManager::debugQuery(const QSqlQuery& query)
{
    if (query.lastError().type() == QSqlError::ErrorType::NoError) {
        qDebug() << "Query OK:"  << query.lastQuery();
    } else {
       qWarning() << "Query KO:" << query.lastError().text();
       qWarning() << "Query text:" << query.lastQuery();
    }
}

//void DatabaseManager::registerDao(const QString key, Dao& dao)
//{
//    mDaos.insert(key, dao);
//}
