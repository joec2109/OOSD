#include "enterbankpin.h"
#include "ui_enterbankpin.h"
#include "loginwindow.h"
#include "globals.h"
#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "forgotbankpin.h"
#include <QTextStream>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>

EnterBankPin::EnterBankPin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EnterBankPin)
{
    ui->setupUi(this);
}

EnterBankPin::~EnterBankPin()
{
    delete ui;
}

void EnterBankPin::on_EnterButton_clicked()
{
    QString bankPin = ui->BankPinBox->text();
    QString username = globalUsername;

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE pin = :bankPin AND username = :username");
    query.bindValue(":bankPin", bankPin);
    query.bindValue(":username", username);

    if (query.exec()) {
        if (query.size() > 0) {

            globalBankPin = bankPin;

            // Grabbing the forename of the logged in user
            QSqlQuery query2;
            query2.prepare("SELECT forename FROM users WHERE pin = :bankPin AND username = :username");
            query2.bindValue(":bankPin", bankPin);
            query2.bindValue(":username", username);
            if(query2.exec()) {
                if (query2.next()) {
                    globalForename = query2.value(0).toString();
                }
            }

            // Grabbing the account balance of the logged in user
            QSqlQuery query3;
            query3.prepare("SELECT balance FROM users WHERE pin = :globalBankPin AND username = :globalUsername");
            query3.bindValue(":globalBankPin", globalBankPin);
            query3.bindValue(":globalUsername", globalUsername);
            query3.exec();

            if (query3.next()) {
                globalBalance = query3.value(0).toString();
            }

            QTextStream(stdout) << "\nCorrect bank PIN";
            QMessageBox::StandardButton alert;
            alert = QMessageBox::information(this, "Bank PIN", "Bank PIN correct",
                                        QMessageBox::Ok);
            if (alert == QMessageBox::Ok) {
                qDebug() << "\nOk was clicked";
                this->hide();
                MainWindow *mw= new MainWindow();
                mw->show();
            } else {
                qDebug() << "\nOk was *not* clicked";
            }
        } else {
            QTextStream(stdout) << "Bank PIN is incorrect";
            QMessageBox::StandardButton alert;
            alert = QMessageBox::warning(this, "Bank PIN", "Bank PIN is incorrect",
                                        QMessageBox::Ok);
            if (alert == QMessageBox::Ok) {
                qDebug() << "\nOk was clicked";
            } else {
                qDebug() << "\nOk was *not* clicked";
            }
        }
    }
}


void EnterBankPin::on_backButton_clicked()
{
    this->hide();
    LoginWindow *lw = new LoginWindow;
    lw->show();
}


void EnterBankPin::on_ForgotBankPinButton_clicked()
{
    this->hide();
    ForgotBankPin *fbp = new ForgotBankPin;
    fbp->show();
}

