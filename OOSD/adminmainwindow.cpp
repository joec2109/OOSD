#include "adminmainwindow.h"
#include "ui_adminmainwindow.h"

AdminMainWindow::AdminMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminMainWindow)
{
    ui->setupUi(this);
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
}


void AdminMainWindow::on_viewAccountsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

