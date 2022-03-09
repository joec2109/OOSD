#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "enterbankpin.h"
#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "globals.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlRecord>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <Qt>

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
        double d_globalBalance = globalBalance.toDouble();
        double value = round( d_globalBalance * 100.0 ) / 100.0;
        QString balanceValue = QString::number(value);
        ui->accountBalanceText->setText("Account balance: £" + balanceValue);
    }

    QSqlQuery query2;

    query2.prepare("SELECT forename, surname FROM users WHERE forename != :globalForename AND username != :globalUsername AND userType = 'Customer'");
    query2.bindValue(":globalForename", globalForename);
    query2.bindValue(":globalUsername", globalUsername);
    query2.exec();
    while (query2.next()) {
        int forenameID = query2.record().indexOf("forename");
        int surnameID = query2.record().indexOf("surname");
        globalTargetForenames.append(query2.value(forenameID).toString());
        globalTargetSurnames.append(query2.value(surnameID).toString());
        ui->transferCombo->addItem((query2.value(forenameID).toString()) + " " + query2.value(surnameID).toString());
    }

    ui->stackedWidget->setCurrentIndex(0);

    // Collecting account info of logged in user

    QSqlQuery query3;

    query3.prepare("SELECT username, surname, password, securityQuestion, securityAnswer, userType FROM users WHERE forename = :globalForename AND pin = :globalBankPin");
    query3.bindValue(":globalForename", globalForename);
    query3.bindValue(":globalBankPin", globalBankPin);
    query3.exec();

    while (query3.next()) {
        int usernameID = query3.record().indexOf("username");
        int surnameID = query3.record().indexOf("surname");
        int passwordID = query3.record().indexOf("password");
        int securityQuestionID = query3.record().indexOf("securityQuestion");
        int securityAnswerID = query3.record().indexOf("securityAnswer");
        int userTypeID = query3.record().indexOf("userType");

        globalUsername = query3.value(usernameID).toString();
        globalSurname = query3.value(surnameID).toString();
        globalPassword = query3.value(passwordID).toString();
        globalSecurityQ = query3.value(securityQuestionID).toString();
        globalSecurityA = query3.value(securityAnswerID).toString();
        globalUserType = query3.value(userTypeID).toString();
    }


}

MainWindow::~MainWindow()
{
    delete ui;
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
        double d_globalBalance = globalBalance.toDouble();
        double value = round( d_globalBalance * 100.0 ) / 100.0;
        QString balanceValue = QString::number(value);
        ui->accountBalanceText->setText("Account balance: £" + balanceValue);
    }

    // Showing the relevant form elements
    ui->stackedWidget->setCurrentIndex(0);

}

// When the user clicks on the deposit subpage button, show them the correct elements for the page.
void MainWindow::on_depositButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// Deposit the amount inputted by the user in to their account balance.
void MainWindow::on_depositButton_2_clicked()
{
    double amountToDeposit = ui->depositBox->text().toDouble();
    int numOfNumbers = 0;
    int numOfDecimalPoints = 0;
    int numOfSpaces = 0;
    QString depositAmountInput = ui->depositBox->text();

    // Loop through each character inputted by the user to check if they are either a number or a decimal point or a space.
    for (int i = 0; i < depositAmountInput.length(); i++) {
        if (depositAmountInput[i].isNumber()) {
            numOfNumbers++;
        }
        if (depositAmountInput[i] == '.') {
            numOfDecimalPoints++;
        }
        if (depositAmountInput[i] == ' ') {
            numOfSpaces++;
        }
    }

    if (numOfSpaces == 0) {
        // If the deposit amount inputted by the user includes just numbers and maximum 1 bullet point then complete the rest of the code.
        if ((numOfNumbers == depositAmountInput.length() && numOfDecimalPoints ==  0) || (numOfNumbers == depositAmountInput.length()-1 && numOfDecimalPoints == 1)){
            QSqlQuery query;
            query.prepare("UPDATE users SET balance = balance + :amountToDeposit WHERE username = :globalUsername AND pin = :globalBankPin");
            query.bindValue(":amountToDeposit", amountToDeposit);
            query.bindValue(":globalUsername", globalUsername);
            query.bindValue(":globalBankPin", globalBankPin);

            globalTransactionAmounts.append(depositAmountInput);
            globalTransactionActions.append("Deposit");
            globalTransactionBalance.append(QString::number(globalBalance.toDouble() + amountToDeposit));

            globalBalance = QString::number(globalBalance.toDouble() + amountToDeposit);

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
        } else {
            QMessageBox::StandardButton alert;
            alert = QMessageBox::information(this, "Deposit", "Deposit unsuccessful. You can only deposit number values.",
                                        QMessageBox::Ok);
            if (alert == QMessageBox::Ok) {
                qDebug() << "\nOk was clicked";
            } else {
                qDebug() << "\nOk was *not* clicked";
            }
        }
    } else {
        QMessageBox::StandardButton alert;
        alert = QMessageBox::information(this, "Deposit", "Deposit unsuccessful. You can only deposit number values.",
                                    QMessageBox::Ok);
        if (alert == QMessageBox::Ok) {
            qDebug() << "\nOk was clicked";
        } else {
            qDebug() << "\nOk was *not* clicked";
        }
    }
}

// If withdraw subpage button clicked, show the withdraw subpage.
void MainWindow::on_withdrawButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

// If value inputted by user is valid, allow them to withdraw that amount.
void MainWindow::on_withdrawButton2_clicked()
{
    double amountToWithdraw = ui->withdrawBox->text().toDouble();
    QString withdrawAmountInput = ui->withdrawBox->text();
    int numOfNumbers = 0;
    int numOfDecimalPoints = 0;
    int numOfSpaces = 0;

    // Loop through each character inputted by the user to check if they are either a number or a decimal point or a space.
    for (int i = 0; i < withdrawAmountInput.length(); i++) {
        if (withdrawAmountInput[i].isNumber()) {
            numOfNumbers++;
        }
        if (withdrawAmountInput[i] == '.') {
            numOfDecimalPoints++;
        }
        if (withdrawAmountInput[i] == ' ') {
            numOfSpaces++;
        }
    }

    // Grabbing the account balance of the logged in user
    QSqlQuery query;
    query.prepare("SELECT balance FROM users WHERE pin = :globalBankPin AND username = :globalUsername");
    query.bindValue(":globalBankPin", globalBankPin);
    query.bindValue(":globalUsername", globalUsername);
    query.exec();

    if (query.next()) {
        globalBalance = query.value(0).toString();
    }

    // If the user has enough funds in their balance to withdraw the amount they input, complete it.
    if ((globalBalance.toDouble() - amountToWithdraw >= 0) && (withdrawAmountInput != "") && (numOfSpaces == 0)) {
        // If the withdraw amount inputted by the user includes just numbers and maximum 1 bullet point then complete the rest of the code.
        if ((numOfNumbers == withdrawAmountInput.length() && numOfDecimalPoints ==  0) || (numOfNumbers == withdrawAmountInput.length()-1 && numOfDecimalPoints == 1)){
            QSqlQuery query;
            query.prepare("UPDATE users SET balance = balance - :amountToWithdraw WHERE username = :globalUsername AND pin = :globalBankPin");
            query.bindValue(":amountToWithdraw", amountToWithdraw);
            query.bindValue(":globalUsername", globalUsername);
            query.bindValue(":globalBankPin", globalBankPin);

            globalTransactionAmounts.append(withdrawAmountInput);
            globalTransactionActions.append("Withdraw");
            globalTransactionBalance.append(QString::number(globalBalance.toDouble() - amountToWithdraw));

            globalBalance = QString::number(globalBalance.toDouble() - amountToWithdraw);

            if (query.exec()) {
                QTextStream(stdout) << "\nWithdraw succesful";
                QMessageBox::StandardButton alert;
                alert = QMessageBox::information(this, "Withdraw", "Withdraw successful",
                                            QMessageBox::Ok);
                if (alert == QMessageBox::Ok) {
                    qDebug() << "\nOk was clicked";
                } else {
                    qDebug() << "\nOk was *not* clicked";
                }
            }
        } else {
            QMessageBox::StandardButton alert;
            alert = QMessageBox::information(this, "Withdraw", "Withdraw unsuccessful. You can only withdraw number values.",
                                        QMessageBox::Ok);
            if (alert == QMessageBox::Ok) {
                qDebug() << "\nOk was clicked";
            } else {
                qDebug() << "\nOk was *not* clicked";
            }
        }
    } else {
        QMessageBox::StandardButton alert;
        alert = QMessageBox::information(this, "Withdraw", "Withdraw unsuccessful. Insufficient funds.",
                                    QMessageBox::Ok);
        if (alert == QMessageBox::Ok) {
            qDebug() << "\nOk was clicked";
        } else {
            qDebug() << "\nOk was *not* clicked";
        }
    }

}

// Go to transfer subpage if the transfer button is clicked.
void MainWindow::on_transferButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

// Once the transfer button is clicked, check to make sure an amount has been entered.
void MainWindow::on_transferButton2_clicked()
{
    double amountToTransfer = ui->transferBox->text().toDouble();
    QString transferAmountInput = ui->transferBox->text();
    int numOfNumbers = 0;
    int numOfDecimalPoints = 0;
    int numOfSpaces = 0;
    QString targetForename = globalTargetForenames[ui->transferCombo->currentIndex()];
    QString targetSurname = globalTargetSurnames[ui->transferCombo->currentIndex()];

    QTextStream (stdout) << globalUsername +" is attempting to transfer £" + transferAmountInput + " to " + targetForename + " " + targetSurname;

    // Loop through each character inputted by the user to check if they are either a number or a decimal point or a space.
    for (int i = 0; i < transferAmountInput.length(); i++) {
        if (transferAmountInput[i].isNumber()) {
            numOfNumbers++;
        }
        if (transferAmountInput[i] == '.') {
            numOfDecimalPoints++;
        }
        if (transferAmountInput[i] == ' ') {
            numOfSpaces++;
        }
    }

    // Ensure the transfer amount is not empty and doesn't include any spaces and the user actually has enough money to complete it.
    if ((numOfSpaces == 0) && (transferAmountInput != "") && (globalBalance.toDouble() - amountToTransfer >= 0)) {
        // Ensure the transfer amount only includes numbers and a possible decimal point.
        if (((numOfNumbers == transferAmountInput.length()) && (numOfDecimalPoints == 0)) || ((numOfNumbers == transferAmountInput.length() - 1) && (numOfDecimalPoints == 1))){
            // Create query to remove the amount being transferred from the sending users account balance
            QSqlQuery query;
            query.prepare("UPDATE users SET balance = balance - :amountToTransfer WHERE username = :globalUsername AND pin = :globalBankPin");
            query.bindValue(":amountToTransfer", amountToTransfer);
            query.bindValue(":globalUsername", globalUsername);
            query.bindValue(":globalBankPin", globalBankPin);

            // Create query to add the amount being transferred to the receiving users account balance.
            QSqlQuery query2;
            query2.prepare("UPDATE users SET balance = balance + :amountToTransfer WHERE forename = :targetForename AND surname = :targetSurname");
            query2.bindValue(":amountToTransfer", amountToTransfer);
            query2.bindValue(":targetForename", targetForename);
            query2.bindValue(":targetSurname", targetSurname);

            globalTransactionAmounts.append(transferAmountInput);
            globalTransactionActions.append("Transfer");
            globalTransactionBalance.append(QString::number(globalBalance.toDouble() - amountToTransfer));

            globalBalance = QString::number(globalBalance.toDouble() - amountToTransfer);

            if (query.exec() && query2.exec()) {
                QTextStream(stdout) << "\nTransfer successful";
                QMessageBox::StandardButton alert;
                alert = QMessageBox::information(this, "Transfer", "Transfer successful",
                                            QMessageBox::Ok);
                if (alert == QMessageBox::Ok) {
                    qDebug() << "\nOk was clicked";
                } else {
                    qDebug() << "\nOk was *not* clicked";
                }
            } else {
                QTextStream(stdout) << "\nTransfer unsuccesful";
                QMessageBox::StandardButton alert;
                alert = QMessageBox::information(this, "Transfer", "Transfer unsuccesful",
                                            QMessageBox::Ok);
                if (alert == QMessageBox::Ok) {
                    qDebug() << "\nOk was clicked";
                } else {
                    qDebug() << "\nOk was *not* clicked";
                }
            }


        } else {
            QTextStream(stdout) << "\nTransfer unsuccesful";
            QMessageBox::StandardButton alert;
            alert = QMessageBox::information(this, "Transfer", "Transfer unsuccesful",
                                        QMessageBox::Ok);
            if (alert == QMessageBox::Ok) {
                qDebug() << "\nOk was clicked";
            } else {
                qDebug() << "\nOk was *not* clicked";
            }
        }
    } else {
        QTextStream(stdout) << "\nTransfer unsuccesful";
        QMessageBox::StandardButton alert;
        alert = QMessageBox::information(this, "Transfer", "Transfer unsuccesful",
                                    QMessageBox::Ok);
        if (alert == QMessageBox::Ok) {
            qDebug() << "\nOk was clicked";
        } else {
            qDebug() << "\nOk was *not* clicked";
        }
    }
}

void MainWindow::on_transactionsButton_clicked()
{
    ui->transactionsTable->setRowCount(globalTransactionActions.length());
    ui->transactionsTable->setColumnCount(3);

    for (int i = 0; i < ui->transactionsTable->rowCount(); i++) {
        QTableWidgetItem *item1 = new QTableWidgetItem();
        item1->setText("£" + globalTransactionAmounts[i]);
        ui->transactionsTable->setItem(i, 1, item1);
        item1->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *item2 = new QTableWidgetItem();
        item2->setText(globalTransactionActions[i]);
        ui->transactionsTable->setItem(i, 0, item2);
        item2->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *item3 = new QTableWidgetItem();
        item3->setText("£" + globalTransactionBalance[i]);
        ui->transactionsTable->setItem(i, 2, item3);
        item3->setTextAlignment(Qt::AlignCenter);
    }

    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_accountButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);

    ui->forenameBox->setText(globalForename);
    ui->surnameBox->setText(globalSurname);
    ui->usernameBox->setText(globalUsername);
    ui->userTypeBox->setText(globalUserType);
    ui->sqBox->setText(globalSecurityQ);
    ui->sqaBox->setText(globalSecurityA);
    ui->pinBox->setText(globalBankPin);
    ui->pwordBox->setText(globalPassword);

}

