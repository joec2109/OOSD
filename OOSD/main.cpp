#include "MainWindow.h"
#include "loginwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QTextStream>
#include <QtSql>
#include <QSqlDatabase>
#include <QStringList>
#include <QSql>
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

    QSqlQuery q1;
    q1.prepare("IF NOT EXISTS CREATE DATABASE oosdproject");
    if (q1.exec()) {
        QTextStream(stdout) << "\nDatabase created";
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("oosdproject");

    if (db.open()) {
        QTextStream(stdout) << "\nDatabase is connected succesfully";
    } else {
        QSqlError error = db.lastError();
        QTextStream(stdout) << "\nDatabase is not connected\n"+error.text();
    }

    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS users (id INTEGER AUTO_INCREMENT PRIMARY KEY, username VARCHAR(32), forename VARCHAR(32), surname VARCHAR(32), password VARCHAR(32), securityQuestion VARCHAR(255), securityAnswer VARCHAR(255), pin INTEGER, userType VARCHAR(32), balance DOUBLE NOT NULL)" );

    QSqlQuery query2;
    query2.prepare("SELECT username FROM users WHERE username='admin");
    query2.exec();
    if (query2.next()) {
        QTextStream(stdout) << "\nAdmin user already exists";
    } else {
        QSqlQuery query3;
        query3.prepare("INSERT INTO users (username, password) VALUES (admin, Admin123)");
    }

    if (query.exec()) {
        QTextStream(stdout) << "\nUsers table created successfully";
    } else {
        QTextStream(stdout) << "\nUsers table already exists\n" + query.lastError().text();
    }

    LoginWindow lw;
    lw.show();
    return a.exec();

}
