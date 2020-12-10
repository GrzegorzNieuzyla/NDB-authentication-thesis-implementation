#include <QString>

#pragma once


class PasswordStrengthValidator
{
public:
    PasswordStrengthValidator();
    bool Validate(const QString& password, QString username);
    QString GetRequirements();
};

