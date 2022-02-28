#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "signupwindow.h"

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
