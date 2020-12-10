#pragma once
#include <string>
#include <QtSql/QSqlDatabase>

class DatabaseAccess
{
public:
    DatabaseAccess(std::string path);
    ~DatabaseAccess();
    struct UserRecord
    {
        QString username;
        QByteArray salt;
        QString passwordPath;
    };
    std::optional<UserRecord> GetUserData(const QString& username);
    void DeleteUserData(const QString& username);
    void CreateUser(const UserRecord& record);
private:
    bool IsDatabaseCreated();
    void CreateDatabase();
    void OpenDatabase();
    std::string _dbPath;
    QSqlDatabase _db;
};

