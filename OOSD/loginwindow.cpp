#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "signupwindow.h"
#include "MainWindow.h"
#include "globals.h"
#include "enterbankpin.h"
#include "adminmainwindow.h"
#include "forgotpassword.h"
#include "bankermainwindow.h"
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

    QSqlQuery query2;
    query2.prepare("SELECT username FROM users WHERE userType = 'Banker' AND username = :username AND password = :password");
    query2.bindValue(":username", username);
    query2.bindValue(":password", password);
    query2.exec();

    if (query.exec()) {
        if ((query.size() > 0) && (username != "admin") && (query2.size() == 0)) {

            globalTargetForenames = {};
            globalTargetSurnames = {};

            globalTransactionAmounts = {};
            globalTransactionActions = {};
            globalTransactionBalance = {};

            globalUsername = username;

            QTextStream(stdout) << "\n" << username << " is logged in";
            QMessageBox::StandardButton alert;
            alert = QMessageBox::information(this, "Log In", "Logged in successfully",
                                        QMessageBox::Ok);
            if (alert == QMessageBox::Ok) {
                qDebug() << "\nOk was clicked";
                this->hide();
                EnterBankPin *ebp= new EnterBankPin();
                ebp->show();
            } else {
                qDebug() << "\nOk was *not* clicked";
            }
        } else if ((query.size() > 0) && (username == "admin")){
            globalUsername = username;

            QTextStream(stdout) << "\n" << username << " is logged in";
            QMessageBox::StandardButton alert;
            alert = QMessageBox::information(this, "Log In", "Logged in successfully",
                                        QMessageBox::Ok);
            if (alert == QMessageBox::Ok) {
                qDebug() << "\nOk was clicked";
                this->hide();
                AdminMainWindow *amw= new AdminMainWindow();
                amw->show();
            } else {
                qDebug() << "\nOk was *not* clicked";
            }
        } else if ((query.size() > 0) && (query2.size() > 0)) {
            globalUsername = username;

            // Grabbing the forename of the logged in user
            QSqlQuery query2;
            query2.prepare("SELECT forename FROM users WHERE userType = 'Banker' AND username = :username");
            query2.bindValue(":username", username);
            if(query2.exec()) {
                if (query2.next()) {
                    globalForename = query2.value(0).toString();
                }
            }

            QTextStream(stdout) << "\n" << username << " is logged in";
            QMessageBox::StandardButton alert;
            alert = QMessageBox::information(this, "Log In", "Logged in successfully",
                                        QMessageBox::Ok);
            if (alert == QMessageBox::Ok) {
                qDebug() << "\nOk was clicked";
                this->hide();
                BankerMainWindow *bmw= new BankerMainWindow();
                bmw->show();
            } else {
                qDebug() << "\nOk was *not* clicked";
            }
        } else {
            QTextStream(stdout) << "Username or password is incorrect";
            QMessageBox::StandardButton alert;
            alert = QMessageBox::warning(this, "Log In", "Username or password is incorrect",
                                        QMessageBox::Ok);
            if (alert == QMessageBox::Ok) {
                qDebug() << "\nOk was clicked";
            } else {
                qDebug() << "\nOk was *not* clicked";
            }
        }
    }
}

void LoginWindow::on_ForgotPasswordButton_clicked()
{
    this->hide();
    ForgotPassword *fpw = new ForgotPassword;
    fpw->show();
}

