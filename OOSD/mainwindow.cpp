#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "enterbankpin.h"
#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "globals.h"
#include <QSqlQuery>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->WelcomeLabel->setText("Welcome, " + globalForename + ".");

    QSqlQuery query;
    query.prepare("SELECT balance FROM users WHERE pin = :globalBankPin AND username = :globalUsername");
    query.bindValue(":globalBankPin", globalBankPin);
    query.bindValue(":globalUsername", globalUsername);
    query.exec();

    if (query.next()) {
        globalBalance = query.value(0).toString();
        ui->accountBalanceText->setText("Account balance: £" + globalBalance);
    }

    ui->DepositText1->hide();
    ui->DepositText2->hide();
    ui->depositBox->hide();
    ui->depositButton_2->hide();

}

MainWindow::~MainWindow()
{
    delete ui;

    ui->WelcomeLabel->setText("Welcome, " + globalForename + ".");
}

void MainWindow::on_homeButton_clicked()
{
    // Grabbing the account balance of the logged in user
    QSqlQuery query;
    query.prepare("SELECT balance FROM users WHERE pin = :globalBankPin AND username = :globalUsername");
    query.bindValue(":globalBankPin", globalBankPin);
    query.bindValue(":globalUsername", globalUsername);
    query.exec();

    if (query.next()) {
        globalBalance = query.value(0).toString();
        ui->accountBalanceText->setText("Account balance: £" + globalBalance);
    }

    // Showing the relevant form elements
    ui->WelcomeLabel->show();
    ui->Text2->show();
    ui->accountBalanceText->show();

    // Hiding other form elements
    ui->DepositText1->hide();
    ui->DepositText2->hide();
    ui->depositBox->hide();
    ui->depositButton_2->hide();

}


void MainWindow::on_depositButton_clicked()
{
    ui->WelcomeLabel->hide();
    ui->Text2->hide();
    ui->accountBalanceText->hide();

    ui->DepositText1->show();
    ui->DepositText2->show();
    ui->depositBox->show();
    ui->depositButton_2->show();
}


void MainWindow::on_depositButton_2_clicked()
{
    double amountToDeposit = ui->depositBox->text().toDouble();

    QSqlQuery query;
    query.prepare("UPDATE users SET balance = balance + :amountToDeposit WHERE username = :globalUsername AND pin = :globalBankPin");
    query.bindValue(":amountToDeposit", amountToDeposit);
    query.bindValue(":globalUsername", globalUsername);
    query.bindValue(":globalBankPin", globalBankPin);
    if (query.exec()) {
        QTextStream(stdout) << "\nDeposit succesful";
        QMessageBox::StandardButton alert;
        alert = QMessageBox::information(this, "Deposit", "Deposit successful",
                                    QMessageBox::Ok);
        if (alert == QMessageBox::Ok) {
            qDebug() << "\nOk was clicked";
        } else {
            qDebug() << "\nOk was *not* clicked";
        }
    }
}

