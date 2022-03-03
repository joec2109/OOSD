#include "mainwindow.h"
#include "loginwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QTextStream>
#include <QtSql>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "OOSD_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // Connecting to db

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("oosdproject");

    if (db.open()) {
        QTextStream(stdout) << "\nDatabase is connected succesfully";
    } else {
        QSqlError error = db.lastError();
        QTextStream(stdout) << "\nDatabase is not connected";
    }

    // Loading login window

    LoginWindow lw;
    lw.show();
    return a.exec();
}
