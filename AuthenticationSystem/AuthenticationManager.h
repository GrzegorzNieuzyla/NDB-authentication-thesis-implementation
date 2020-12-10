#pragma once
#include "DatabaseAccess.h"

#include <QString>
#include <string>
#include "DB.h"
#include <memory>

class AuthenticationManager
{
public:
    AuthenticationManager(std::string dbPath);
    bool CheckLogin(QString username, QString password);
    bool Register(QString username, QString password);
    void ChangePassword(QString username, QString password);
    void DeleteAccount(QString username);
private:
    std::vector<char> GenerateSalt();
    DBRecord GeneratePasswordRecordWithSalt(const QString& password, const QByteArray& salt);
    bool CheckAgainstNDB(const DBRecord& record, const std::string& path);
    QByteArray ToByteArray(const std::vector<char> vector);
    DatabaseAccess _database;
    const std::string DATA_DIRECTORY = "./auth-data";
    const int ITERATIONS = 10000;
};

