#include "signupwindow.h"
#include "ui_signupwindow.h"
#include "mainwindow.h"
#include <QTextStream>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>

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
    bool forenameValid = false, surnameValid = false, passwordValid = false, SQValid = false, SQAnswerValid = false, pinValid = false;

    // Validation

    // Forename validation
    if (ui->ForenameBox->text().isEmpty()) {
        QTextStream(stdout) << "\nForename is empty";
        ui->ForenameLabel->setStyleSheet("QLabel { color : red; }");
        forenameValid = false;
    } else {
        ui->ForenameLabel->setStyleSheet("QLabel { color : black; }");
        forenameValid = true;
    }

    // Surname validation
    if (ui->SurnameBox->text().isEmpty()) {
        QTextStream(stdout) << "\nSurname is empty";
        ui->SurnameLabel->setStyleSheet("QLabel { color : red; }");
        surnameValid = false;
    } else {
        ui->SurnameLabel->setStyleSheet("QLabel { color : black; }");
        surnameValid = true;
    }

    // Password validation
    if (ui->PasswordBox->text().isEmpty()) {
        QTextStream(stdout) << "\nPassword is empty";
        ui->PasswordLabel->setStyleSheet("QLabel { color : red; }");
        passwordValid = false;
    } else {
        ui->PasswordLabel->setStyleSheet("QLabel { color : black; }");
        passwordValid = true;
    }


    // Checking password length
    if (ui->PasswordBox->text().length() < 8) {
        QTextStream(stdout) << "\nPassword is not long enough";
        ui->PasswordLabel->setStyleSheet("QLabel { color : red; }");
        passwordValid = false;
    } else {
        ui->PasswordLabel->setStyleSheet("QLabel { color : black; }");
        passwordValid = true;
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
        passwordValid = true;
    }
    if (upper == 0) {
        QTextStream(stdout) << "\nPassword needs at least 1 upper case char";
        ui->PasswordLabel->setStyleSheet("QLabel { color : red; }");
        passwordValid = false;
    }
    if (lower == 0) {
        QTextStream(stdout) << "\nPassword needs at least 1 lower case char";
        ui->PasswordLabel->setStyleSheet("QLabel { color : red; }");
        passwordValid = false;
    }
    if (numbers == 0) {
        QTextStream(stdout) << "\nPassword needs at least 1 number";
        ui->PasswordLabel->setStyleSheet("QLabel { color : red; }");
        passwordValid = false;
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

    // Security question validation

    if (ui->SecurityQuestionBox->currentText() != "") {
        ui->SecurityQuestionLabel->setStyleSheet("QLabel { color : black; }");
        SQValid = true;
    } else {
        ui->SecurityQuestionLabel->setStyleSheet("QLabel { color : red; }");
        SQValid = false;
    }

    // Security question answer validation

    if (ui->SQAnswerBox->text().isEmpty()) {
        ui->SecurityQuestionAnswerLabel->setStyleSheet("QLabel { color : red; }");
        SQAnswerValid = false;
    } else {
        ui->SecurityQuestionAnswerLabel->setStyleSheet("QLabel { color : black; }");
        SQAnswerValid = true;
    }

    // Bank PIN validation

    QString pin = ui->BankPINBox->text();

    if (ui->BankPINBox->text().length() != 4){
        QTextStream(stdout) << "\nBank PIN must be at least 4 characters";
        ui->BankPINLabel->setStyleSheet("QLabel { color : red; }");
        pinValid = false;
    } else {
        ui->BankPINLabel->setStyleSheet("QLabel { color : black; }");
        pinValid = true;
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
            pinValid = false;
        } else {
            ui->BankPINLabel->setStyleSheet("QLabel { color : black; }");
            pinValid = true;
        }
    }

    // Confirm bank PIN validation

    if (ui->ConfirmBankPINBox->text() != ui->BankPINBox->text()) {
        QTextStream(stdout) << "\nConfirm bank PIN must be the same as the bank PIN";
        ui->ConfirmBankPINLabel->setStyleSheet("QLabel { color : red; }");
    } else {
        ui->ConfirmBankPINLabel->setStyleSheet("QLabel { color : black; }");
    }

    // If data entered is valid, insert in to the database.

    if (forenameValid == true && surnameValid == true && passwordValid == true && SQValid == true && SQAnswerValid == true && pinValid == true) {
        QString forename = ui->ForenameBox->text();
        QString surname = ui->SurnameBox->text();
        QString pword = ui->PasswordBox->text();
        QString secquestion = ui->SecurityQuestionBox->currentText();
        QString sqanswer = ui->SQAnswerBox->text();
        QString str = ui->BankPINBox->text();
        int bankpin =  str.toInt();

        QSqlQuery qry;
        qry.prepare("INSERT INTO users (forename, surname, password, securityQuestion, securityAnswer, pin)"
                    "VALUES (:forename, :surname, :password, :securityQuestion, :securityAnswer, :bankpin)");

        qry.bindValue(":forename", forename);
        qry.bindValue(":surname", surname);
        qry.bindValue(":password", password);
        qry.bindValue(":securityQuestion", secquestion);
        qry.bindValue(":securityAnswer", sqanswer);
        qry.bindValue(":bankpin", bankpin);

        if (qry.exec()) {
            QTextStream(stdout) << "\nUser inserted into DB";

            QMessageBox::StandardButton alert;
            alert = QMessageBox::information(this, "Sign Up", "Signed up successfully",
                                        QMessageBox::Ok);
            if (alert == QMessageBox::Ok) {
                qDebug() << "Ok was clicked";
                this->hide();
                MainWindow *mw= new MainWindow();
                mw->show();
            } else {
                qDebug() << "Ok was *not* clicked";
            }

        } else {
            QTextStream(stdout) << "\nUser failed to create";
        }
    }
}

