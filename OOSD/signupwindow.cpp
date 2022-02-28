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


    // Checking password length
    if (ui->PasswordBox->text().length() < 8) {
        QTextStream(stdout) << "\nPassword is not long enough";
        ui->PasswordLabel->setStyleSheet("QLabel { color : red; }");
    } else {
        ui->PasswordLabel->setStyleSheet("QLabel { color : black; }");
    }

    QString password = ui->PasswordBox->text();
    int upper = 0, lower = 0, numbers = 0;


    // Checking password characters
    for (int i = 0; i < ui->PasswordBox->text().length(); i++)
    {
        if (password[i].isLetter()){
            if (password[i].isUpper()){
                upper++;
            } else {
                lower++;
            }
        } else if (password[i].isDigit()){
            numbers++;
        }
    }

    if (upper >= 1 && lower >= 1 && numbers >= 1) {
        QTextStream(stdout) << "\nPassword is valid";
        ui->PasswordLabel->setStyleSheet("QLabel { color : black; }");
    }
    if (upper == 0) {
        QTextStream(stdout) << "\nPassword needs at least 1 upper case char";
        ui->PasswordLabel->setStyleSheet("QLabel { color : red; }");
    }
    if (lower == 0) {
        QTextStream(stdout) << "\nPassword needs at least 1 lower case char";
        ui->PasswordLabel->setStyleSheet("QLabel { color : red; }");
    }
    if (numbers == 0) {
        QTextStream(stdout) << "\nPassword needs at least 1 number";
        ui->PasswordLabel->setStyleSheet("QLabel { color : red; }");
    }


    // Confirm password validation
    if (ui->ConfirmPasswordBox->text().isEmpty()) {
        QTextStream(stdout) << "\nConfirm password is empty";
        ui->ConfirmPasswordLabel->setStyleSheet("QLabel { color : red; }");
    } else {
        ui->ConfirmPasswordLabel->setStyleSheet("QLabel { color : black; }");
    }

    if (ui->ConfirmPasswordBox->text() != ui->PasswordBox->text()){
        QTextStream(stdout) << "\nConfirm password must match password";
        ui->ConfirmPasswordLabel->setStyleSheet("QLabel { color : red; }");
    } else {
        ui->ConfirmPasswordLabel->setStyleSheet("QLabel { color : black; }");
    }

    // Bank PIN validation

    QString pin = ui->BankPINBox->text();

    if (ui->BankPINBox->text().length() != 4){
        QTextStream(stdout) << "\nBank PIN must be at least 4 characters";
        ui->BankPINLabel->setStyleSheet("QLabel { color : red; }");
    } else {
        ui->BankPINLabel->setStyleSheet("QLabel { color : black; }");
    }

    int letters = 0;

    // Ensuring bank PIN only includes numbers
    if (ui->BankPINBox->text().length() == 4) {
        for (int i = 0; i < 4; i++) {
            if (pin[i].isLetter()){
                letters++;
            }
        }

        if (letters > 0) {
            QTextStream(stdout) << "\nBank PIN must only contain numbers";
            ui->BankPINLabel->setStyleSheet("QLabel { color : red; }");
        } else {
            ui->BankPINLabel->setStyleSheet("QLabel { color : black; }");
        }
    }

    // Confirm bank PIN validation

    if (ui->ConfirmBankPINBox->text() != ui->BankPINBox->text()) {
        QTextStream(stdout) << "\nConfirm bank PIN must be the same as the bank PIN";
        ui->ConfirmBankPINLabel->setStyleSheet("QLabel { color : red; }");
    } else {
        ui->ConfirmBankPINLabel->setStyleSheet("QLabel { color : black; }");
    }

}

