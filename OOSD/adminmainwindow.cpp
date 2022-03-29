#include "adminmainwindow.h"
#include "ui_adminmainwindow.h"
#include "loginwindow.h"
#include <QSql>
#include <QSqlQuery>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <Qt>
#include "globals.h"

AdminMainWindow::AdminMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminMainWindow)
{
    ui->setupUi(this);
    // Insert transactions in to the transactions table
    ui->transactionsTable->setRowCount(globalTransactionActions.length());
    for (int i = 0; i < ui->transactionsTable->rowCount(); i++) {
        QTableWidgetItem *item1 = new QTableWidgetItem();
        item1->setText(globalTransactionActions[i]);
        ui->transactionsTable->setItem(i, 0, item1);
        QTableWidgetItem *item2 = new QTableWidgetItem();
        item2->setText("£ " + globalTransactionAmounts[i]);
        ui->transactionsTable->setItem(i, 1, item2);
        QTableWidgetItem *item3 = new QTableWidgetItem();
        item3->setText("£ " + globalTransactionBalance[i]);
        ui->transactionsTable->setItem(i, 2, item3);
    }

    // Insert accounts in to the account table
    ui->accountsTable->setRowCount(accountNames.length());
    for (int i = 0; i < ui->accountsTable->rowCount(); i++) {
        QTableWidgetItem *item1 = new QTableWidgetItem();
        item1 -> setText(accountNames[i]);
        ui->accountsTable->setItem(i, 0, item1);
        QTableWidgetItem *item2 = new QTableWidgetItem();
        item2 -> setText(accountPasswords[i]);
        ui->accountsTable->setItem(i, 1, item2);
        QTableWidgetItem *item3 = new QTableWidgetItem();
        item3 -> setText(accountPins[i]);
        ui->accountsTable->setItem(i, 2, item3);
        QTableWidgetItem *item4 = new QTableWidgetItem();
        item4 -> setText(accountBalances[i]);
        ui->accountsTable->setItem(i, 3, item4);
        QTableWidgetItem *item5 = new QTableWidgetItem();
        item5 -> setText(accountUserTypes[i]);
        ui->accountsTable->setItem(i, 4, item5);
    }
}

AdminMainWindow::~AdminMainWindow()
{
    delete ui;
}

// Take user to home page
void AdminMainWindow::on_homeButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// Insert the transactions in to the transactions table
void AdminMainWindow::on_viewTransactionsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    for (int i = 0; i < ui->transactionsTable->rowCount(); i++) {
        QTableWidgetItem *item1 = new QTableWidgetItem();
        item1->setText(globalTransactionActions[i]);
        ui->transactionsTable->setItem(i, 0, item1);
        QTableWidgetItem *item2 = new QTableWidgetItem();
        item2->setText("£ " + globalTransactionAmounts[i]);
        ui->transactionsTable->setItem(i, 1, item2);
        QTableWidgetItem *item3 = new QTableWidgetItem();
        item3->setText("£ " + globalTransactionBalance[i]);
        ui->transactionsTable->setItem(i, 2, item3);
    }
}

// Insert the accounts in to the accounts table
void AdminMainWindow::on_viewAccountsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

    for (int i = 0; i < ui->accountsTable->rowCount(); i++) {
        QTableWidgetItem *item1 = new QTableWidgetItem();
        item1 -> setText(accountNames[i]);
        ui->accountsTable->setItem(i, 0, item1);
        QTableWidgetItem *item2 = new QTableWidgetItem();
        item2 -> setText(accountPasswords[i]);
        ui->accountsTable->setItem(i, 1, item2);
        QTableWidgetItem *item3 = new QTableWidgetItem();
        item3 -> setText(accountPins[i]);
        ui->accountsTable->setItem(i, 2, item3);
        QTableWidgetItem *item4 = new QTableWidgetItem();
        item4 -> setText(accountBalances[i]);
        ui->accountsTable->setItem(i, 3, item4);
        QTableWidgetItem *item5 = new QTableWidgetItem();
        item5 -> setText(accountUserTypes[i]);
        ui->accountsTable->setItem(i, 4, item5);
    }
}

// Log the admin out and remove details from the accounts table
void AdminMainWindow::on_pushButton_clicked()
{
    ui->accountsTable->setRowCount(0);
    this->hide();
    LoginWindow *lw = new LoginWindow();
    lw->show();
}


// Delete account button
void AdminMainWindow::on_pushButton_2_clicked()
{
    QModelIndexList accountSelectedIndex = ui->accountsTable->selectionModel()->selectedIndexes();
    QString rowToDelete = QString::number(accountSelectedIndex[0].row());
    QString usernameToDelete = ui->accountsTable->item(accountSelectedIndex[0].row(), 0)->data(Qt::DisplayRole).toString();
    QString passwordToDelete = ui->accountsTable->item(accountSelectedIndex[0].row(), 1)->data(Qt::DisplayRole).toString();
    QStringList names = usernameToDelete.split(" ");
    QString forename = names[0];
    QString surname = names[1];
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE forename = :forename AND surname = :surname AND password = :passwordToDelete");
    query.bindValue(":forename", forename);
    query.bindValue(":surname", surname);
    query.bindValue(":passwordToDelete", passwordToDelete);
    query.exec();
    QTextStream(stdout) << "\n" + usernameToDelete + "'s account deleted.";

    // Remove the account details from the various global variables associated with it
    ui->accountsTable->removeRow(accountSelectedIndex[0].row());
    accountNames.remove(accountSelectedIndex[0].row());
    accountPasswords.remove(accountSelectedIndex[0].row());
    accountPins.remove(accountSelectedIndex[0].row());
    accountBalances.remove(accountSelectedIndex[0].row());
    accountUserTypes.remove(accountSelectedIndex[0].row());
}

// Promote selected user to a banker
void AdminMainWindow::on_promoteButton_clicked()
{
    QModelIndexList accountSelectedIndex = ui->accountsTable->selectionModel()->selectedIndexes();
    QString rowToPromote = QString::number(accountSelectedIndex[0].row());
    QString usernameToPromote = ui->accountsTable->item(accountSelectedIndex[0].row(), 0)->data(Qt::DisplayRole).toString();
    QString passwordToPromote = ui->accountsTable->item(accountSelectedIndex[0].row(), 1)->data(Qt::DisplayRole).toString();
    QStringList names = usernameToPromote.split(" ");
    QString forename = names[0];
    QString surname = names[1];
    QSqlQuery query;
    query.prepare("UPDATE users SET userType = 'Banker' WHERE forename = :forename AND surname = :surname AND password= :passwordToPromote");
    query.bindValue(":forename", forename);
    query.bindValue(":surname", surname);
    query.bindValue(":passwordToPromote", passwordToPromote);
    query.exec();
    QTextStream(stdout) << "\n" + usernameToPromote + " has been promoted.";
    ui->accountsTable->item(accountSelectedIndex[0].row(), 4)->setText("Banker");
    accountUserTypes[accountSelectedIndex[0].row()] = "Banker";
}

// Demote selected user to a regular customer
void AdminMainWindow::on_demoteButton_clicked()
{
    QModelIndexList accountSelectedIndex = ui->accountsTable->selectionModel()->selectedIndexes();
    QString rowToDemote = QString::number(accountSelectedIndex[0].row());
    QString usernameToDemote = ui->accountsTable->item(accountSelectedIndex[0].row(), 0)->data(Qt::DisplayRole).toString();
    QString passwordToDemote = ui->accountsTable->item(accountSelectedIndex[0].row(), 1)->data(Qt::DisplayRole).toString();
    QStringList names = usernameToDemote.split(" ");
    QString forename = names[0];
    QString surname = names[1];
    QSqlQuery query;
    query.prepare("UPDATE users SET userType = 'Customer' WHERE forename = :forename AND surname = :surname AND password= :passwordToDemote");
    query.bindValue(":forename", forename);
    query.bindValue(":surname", surname);
    query.bindValue(":passwordToDemote", passwordToDemote);
    query.exec();
    QTextStream(stdout) << "\n" + usernameToDemote + " has been demoted.";
    ui->accountsTable->item(accountSelectedIndex[0].row(), 4)->setText("Customer");
    accountUserTypes[accountSelectedIndex[0].row()] = "Customer";
}

// Revert the selected transaction
void AdminMainWindow::on_revertTransactionButton_clicked()
{
    QModelIndexList transactionSelectedIndex = ui->transactionsTable->selectionModel()->selectedIndexes();
    QString rowToRevert = QString::number(transactionSelectedIndex[0].row());
    QStringList usernamesToRevert = ui->transactionsTable->item(transactionSelectedIndex[0].row(), 0)->data(Qt::DisplayRole).toString().split(" ");
    QStringList splitAmountColumn = ui->transactionsTable->item(transactionSelectedIndex[0].row(), 1)->data(Qt::DisplayRole).toString().split(" ");
    QString amountToRevert = splitAmountColumn[1];
    QString usernameToRevert = "";
    QString username2ToRevert = "";
    // If transaction is a withdraw or a deposit, then only the username of the person doing the action is needed.
    if ((usernamesToRevert[1] == "deposit") || (usernamesToRevert[1] == "withdraw")) {
        usernameToRevert = usernamesToRevert[0];
    } else {    // If the transaction is a transfer, the name of the person doing the action and the name of the target user is needed.
        usernameToRevert = usernamesToRevert[0];
        username2ToRevert = usernamesToRevert[3] + usernamesToRevert[4];
    }

    // Handle reverting withdrawals
    if (usernamesToRevert[1] == "withdraw") {
        QSqlQuery q1;
        q1.prepare("UPDATE users SET balance = balance + :amountToRevert WHERE username = :usernameToRevert");
        q1.bindValue(":amountToRevert", amountToRevert);
        q1.bindValue(":usernameToRevert", usernameToRevert);
        q1.exec();
        ui->transactionsTable->removeRow(transactionSelectedIndex[0].row());
        globalTransactionActions.remove(transactionSelectedIndex[0].row());
        globalTransactionAmounts.remove(transactionSelectedIndex[0].row());
        globalTransactionBalance.remove(transactionSelectedIndex[0].row());
    }
    // Handle reverting deposits
    else if (usernamesToRevert[1] == "deposit") {
        QSqlQuery q1;
        q1.prepare("UPDATE users SET balance = balance - :amountToRevert WHERE username = :usernameToRevert");
        q1.bindValue(":amountToRevert", amountToRevert);
        q1.bindValue(":usernameToRevert", usernameToRevert);
        q1.exec();
        ui->transactionsTable->removeRow(transactionSelectedIndex[0].row());
        globalTransactionActions.remove(transactionSelectedIndex[0].row());
        globalTransactionAmounts.remove(transactionSelectedIndex[0].row());
        globalTransactionBalance.remove(transactionSelectedIndex[0].row());
    } else {
        // Handle reverting transfers
        QSqlQuery q1;
        q1.prepare("UPDATE users SET balance =  balance + :amountToRevert WHERE username = :usernameToRevert");
        q1.bindValue(":amountToRevert", amountToRevert);
        q1.bindValue(":usernameToRevert", usernameToRevert);
        q1.exec();
        QSqlQuery q2;
        q2.prepare("UPDATE users SET balance = balance - :amountToRevert WHERE forename = :forenameToRevert AND surname = :surnameToRevert");
        QTextStream(stdout) << "\ntaking away from " + username2ToRevert;
        q2.bindValue(":amountToRevert", amountToRevert);
        q2.bindValue(":forenameToRevert", usernamesToRevert[3]);
        q2.bindValue(":surnameToRevert", usernamesToRevert[4]);
        q2.exec();
        ui->transactionsTable->removeRow(transactionSelectedIndex[0].row());
        globalTransactionActions.remove(transactionSelectedIndex[0].row());
        globalTransactionAmounts.remove(transactionSelectedIndex[0].row());
        globalTransactionBalance.remove(transactionSelectedIndex[0].row());
        globalTargetForenames.remove(transactionSelectedIndex[0].row());
        globalTargetSurnames.remove(transactionSelectedIndex[0].row());
    }
}

