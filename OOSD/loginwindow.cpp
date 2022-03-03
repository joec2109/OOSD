#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "signupwindow.h"
#include "mainwindow.h"
#include <QTextStream>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>


LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_SignUpButton_clicked()
{
    this->hide();
    SignUpWindow *suw= new SignUpWindow();
    suw->show();
}

void LoginWindow::on_LoginButton_clicked()
{
    QString username = ui->usernameBox->text();
    QString password = ui->passwordBox->text();

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec()) {
        if (query.size() > 0) {
            QString name = query.value(1).toString();
            QTextStream(stdout) << name << "is logged in";
            QMessageBox::StandardButton alert;
            alert = QMessageBox::information(this, "Log In", "Logged in successfully",
                                        QMessageBox::Ok);
            if (alert == QMessageBox::Ok) {
                qDebug() << "Ok was clicked";
                this->hide();
                MainWindow *mw= new MainWindow();
                mw->show();
            } else {
                qDebug() << "Ok was *not* clicked";
            }
        } else {
            QTextStream(stdout) << "Username or password is incorrect";
            QMessageBox::StandardButton alert;
            alert = QMessageBox::warning(this, "Log In", "Username or password is incorrect",
                                        QMessageBox::Ok);
            if (alert == QMessageBox::Ok) {
                qDebug() << "Ok was clicked";
            } else {
                qDebug() << "Ok was *not* clicked";
            }
        }
    }
}

