#include "DatabaseAccess.h"
#include <filesystem>

#include <QtSql/QSqlQuery>
#include <QVariant>

DatabaseAccess::DatabaseAccess(std::string path)
 : _dbPath(std::move(path))
{
    if (!IsDatabaseCreated())
        CreateDatabase();
    else
        OpenDatabase();
}

DatabaseAccess::~DatabaseAccess()
{
    _db.close();
}

std::optional<DatabaseAccess::UserRecord> DatabaseAccess::GetUserData(const QString& username)
{
    QSqlQuery query(_db);
    query.prepare("SELECT passwordPath, salt from users WHERE username = ?;");
    query.addBindValue(username);
    query.exec();
    if (query.next())
    {
        UserRecord result;
        result.username = username;
        result.passwordPath = query.value(0).toString();
        result.salt = query.value(1).toByteArray().mid(0, 64);
        return std::move(result);
    }
    else
        return {};
}

void DatabaseAccess::DeleteUserData(const QString& username)
{
    QSqlQuery query(_db);
    query.prepare("DELETE FROM users WHERE username=?");
    query.addBindValue(username);
    query.exec();
    _db.commit();
}

void DatabaseAccess::CreateUser(const DatabaseAccess::UserRecord& record)
{
    QSqlQuery query(_db);
    query.prepare("INSERT INTO users (username, passwordPath, salt) VALUES (?,?,?)");
    query.addBindValue(record.username);
    query.addBindValue(record.passwordPath);
    query.addBindValue(record.salt);
    query.exec();
    _db.commit();
}


bool DatabaseAccess::IsDatabaseCreated()
{
    return std::filesystem::exists(_dbPath);
}

void DatabaseAccess::CreateDatabase()
{
    QString createQuery = "CREATE TABLE users (id integer primary key autoincrement, username text, passwordPath text, salt blob);";

    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(QString::fromStdString(_dbPath));
    _db.open();
    _db.exec(createQuery);
    _db.commit();
}

void DatabaseAccess::OpenDatabase()
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(QString::fromStdString(_dbPath));
    _db.open();
}
