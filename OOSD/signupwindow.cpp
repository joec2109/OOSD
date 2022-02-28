#include "signupwindow.h"
#include "ui_signupwindow.h"
#include <QTextStream>

using namespace std;

SignUpWindow::SignUpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SignUpWindow)
{
    ui->setupUi(this);
}

SignUpWindow::~SignUpWindow()
{
    delete ui;
}

void SignUpWindow::on_SignUpButton_clicked()
{
    // Validation

    // Forename validation
    if (ui->ForenameBox->text().isEmpty()) {
        QTextStream(stdout) << "\nForename is empty";
        ui->ForenameLabel->setStyleSheet("QLabel { color : red; }");
    } else {
        ui->ForenameLabel->setStyleSheet("QLabel { color : black; }");
    }

    // Surname validation
    if (ui->SurnameBox->text().isEmpty()) {
        QTextStream(stdout) << "\nSurname is empty";
        ui->SurnameLabel->setStyleSheet("QLabel { color : red; }");
    } else {
        ui->SurnameLabel->setStyleSheet("QLabel { color : black; }");
    }

    // Password validation
    if (ui->PasswordBox->text().isEmpty()) {
        QTextStream(stdout) << "\nPassword is empty";
        ui->PasswordLabel->setStyleSheet("QLabel { color : red; }");
    } else {
        ui->PasswordLabel->setStyleSheet("QLabel { color : black; }");
    }

    // Confirm password validation
    if (ui->ConfirmPasswordBox->text().isEmpty()) {
        QTextStream(stdout) << "\nConfirm password is empty";
        ui->ConfirmPasswordLabel->setStyleSheet("QLabel { color : red; }");
    } else {
        ui->ConfirmPasswordLabel->setStyleSheet("QLabel { color : black; }");
    }
}

