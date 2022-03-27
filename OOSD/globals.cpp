#include "globals.h"
#include <QString>
#include <QStringList>

QString globalUsername = "";
QString globalBankPin = "";
QString globalForename = "";
QString globalSurname = "";
QString globalBalance = "";
QString globalSecurityQ = "";
QString globalSecurityA = "";
QString globalPassword = "";
QString globalUserType = "";

// For transfers
QStringList globalTargetForenames = {};
QStringList globalTargetSurnames = {};

QStringList globalTransactionAmounts = {};
QStringList globalTransactionActions = {};
QStringList globalTransactionBalance = {};

// For accounts
QStringList accountNames = {};
QStringList accountPasswords = {};
QStringList accountPins = {};
QStringList accountBalances = {};
QStringList accountUserTypes = {};
