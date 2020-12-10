#include "PasswordStrengthValidator.h"

#include <QRegExp>

PasswordStrengthValidator::PasswordStrengthValidator()
{

}

bool PasswordStrengthValidator::Validate(const QString& password, QString username)
{
    auto size = password.size() > 7;
    auto digit = password.contains(QRegExp("[0-9]"));
    auto upper = password.contains(QRegExp("[A-Z]"));
    auto lower = password.contains(QRegExp("[a-z]"));
    auto special = password.contains(QRegExp("[!@#$%&*()_+=]"));
    return size && digit && upper && lower && special && username.toLower() != password.toLower();
}

QString PasswordStrengthValidator::GetRequirements()
{
    auto requirements = QString("\nPassword has to be at least 8 characters long. \nPassword has to contain at least one special character,\none digit, one lowercase and one uppercase character ");
    requirements += "\nPassword can not be identical to username";
    return requirements;
}
