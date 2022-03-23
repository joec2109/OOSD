#include "adminmainwindow.h"
#include "ui_adminmainwindow.h"
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
}

AdminMainWindow::~AdminMainWindow()
{
    delete ui;
}

void AdminMainWindow::on_homeButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


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


void AdminMainWindow::on_viewAccountsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

