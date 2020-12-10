#include "AuthenticationManager.h"
#include "../Generators/NDB_KHiddenGenerator.h"
#include <filesystem>
#include <QPasswordDigestor>
#include <QCryptographicHash>
#include <Streams/NDBFileStream.h>
#include <Utils/RandomValuesGenerator.h>
#include "Validator/NDBValidator.h"


AuthenticationManager::AuthenticationManager(std::string dbPath)
    : _database(dbPath)
{
    if (!std::filesystem::exists(DATA_DIRECTORY))
        std::filesystem::create_directory(DATA_DIRECTORY);
}

bool AuthenticationManager::CheckLogin(QString username, QString password)
{
    auto userData = _database.GetUserData(username);
    if (!userData.has_value())
        return false;
    auto record = GeneratePasswordRecordWithSalt(password.toUtf8(), userData.value().salt);
    return CheckAgainstNDB(record, userData->passwordPath.toStdString());
}

bool AuthenticationManager::Register(QString username, QString password)
{
    if (_database.GetUserData(username).has_value())
        return false;
    std::string ndbPath = DATA_DIRECTORY + "/" + username.toStdString();
    auto salt = GenerateSalt();
    auto record = GeneratePasswordRecordWithSalt(password, ToByteArray(salt));
    NDB_KHiddenGenerator generator(record, record.Size(), {0.7, 0.0, 0.3}, 4.5, 3);

    NDBFileStream stream(ndbPath);
    generator.Generate(stream);
    DatabaseAccess::UserRecord userRecord;
    userRecord.username = username;
    userRecord.passwordPath = QString::fromStdString(ndbPath);
    userRecord.salt = ToByteArray(salt).mid(0, 64);
    _database.CreateUser(userRecord);
    return true;
}

void AuthenticationManager::ChangePassword(QString username, QString password)
{
    auto userData = _database.GetUserData(username);
    if (!userData.has_value())
        return;
    auto record = GeneratePasswordRecordWithSalt(password, userData->salt);
    NDB_KHiddenGenerator generator(record, record.Size(), {0.7, 0.0, 0.3}, 4.5, 3);

    NDBFileStream stream(userData->passwordPath.toStdString());
    generator.Generate(stream);
}

void AuthenticationManager::DeleteAccount(QString username)
{
    auto userData = _database.GetUserData(username);
    if (!userData.has_value())
        return;
    _database.DeleteUserData(username);
    std::filesystem::remove(userData->passwordPath.toStdString());
}

std::vector<char> AuthenticationManager::GenerateSalt()
{
    return RandomValuesGenerator::GetRandomBytes(64);
}

DBRecord AuthenticationManager::GeneratePasswordRecordWithSalt(const QString& password, const QByteArray& salt)
{
    DBRecord record;
    auto digest = QPasswordDigestor::deriveKeyPbkdf2(QCryptographicHash::Sha512, password.toUtf8(), salt, ITERATIONS, 64);
    QCryptographicHash hash(QCryptographicHash::Sha512);
    hash.addData(digest);
    digest += hash.result();
    std::vector<unsigned char> positiveRecord(digest.begin(), digest.end());
    record.AppendBytes(positiveRecord);
    return record;
}

bool AuthenticationManager::CheckAgainstNDB(const DBRecord& record, const std::string& path)
{
    return NDBValidator::ValidateFromNDBFile(DB {record}, path, false);
}

QByteArray AuthenticationManager::ToByteArray(const std::vector<char> vector)
{
    QByteArray result;
    for (auto ch : vector)
    {
        result.push_back(ch);
    }
    return result.mid(0, 64);
}

