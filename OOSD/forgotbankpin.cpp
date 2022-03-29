#include "forgotbankpin.h"
#include "ui_forgotbankpin.h"
#include "loginwindow.h"
#include <QSql>
#include <QSqlQuery>
#include <QMessageBox>

ForgotBankPin::ForgotBankPin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ForgotBankPin)
{
    ui->setupUi(this);
}

ForgotBankPin::~ForgotBankPin()
{
    delete ui;
}

void ForgotBankPin::on_backButton_clicked()
{
    this->hide();
    LoginWindow *lw = new LoginWindow;
    lw->show();
}


void ForgotBankPin::on_submitButton_clicked()
{
    QString username = ui->usernameBox->text();
    QString secQuestion = ui->SecurityQuestionBox->currentText();
    QString secAnswer = ui->securityAnswerBox->text();
    QString newBankPin = ui->newBankPinBox->text();
    QString confirmNewBankPin = ui->confirmNewBPinBox->text();

    if (!username.isEmpty()) {
        QSqlQuery q1;
        q1.prepare("SELECT username FROM users WHERE username = :username");
        q1.bindValue(":username", username);
        if (q1.exec()) {
            if (q1.size() > 0) {
                QTextStream(stdout) << "\nUsername found";
                QSqlQuery q2;
                q2.prepare("SELECT securityQuestion, securityAnswer FROM users WHERE username = :username AND securityQuestion = :secQuestion AND securityAnswer = :secAnswer");
                q2.bindValue(":username", username);
                q2.bindValue(":secQuestion", secQuestion);
                q2.bindValue(":secAnswer", secAnswer);
                if (q2.exec()) {
                    if ((q2.size() > 0) && (newBankPin == confirmNewBankPin)) {


                        // Insert bank pin validation here

                        bool validPinLength = false;
                        bool validPinChars = false;
                        bool newPinEqualsConfirmPin = false;

                        if (newBankPin.size() == 4) {
                            validPinLength = true;
                        } else {
                            QMessageBox::StandardButton alert;
                            alert = QMessageBox::information(this, "Forgot Bank PIN", "Bank PIN needs to be 4 numbers in length",
                                                        QMessageBox::Ok);
                            if (alert == QMessageBox::Ok) {
                                qDebug() << "\nOk was clicked";
                            } else {
                                qDebug() << "\nOk was *not* clicked";
                            }
                        }
                        if ((!ui->newBankPinBox->text().isEmpty())&&(!ui->confirmNewBPinBox->text().isEmpty())) {
                            int letters = 0;

                            for (int i = 0; i < 4; i++) {
                                if (newBankPin[i].isLetter()){
                                    letters++;
                                }
                            }

                            if (letters > 0) {
                                QMessageBox::StandardButton alert;
                                alert = QMessageBox::information(this, "Forgot Bank PIN", "Bank PIN must only contain numbers",
                                                            QMessageBox::Ok);
                                if (alert == QMessageBox::Ok) {
                                    qDebug() << "\nOk was clicked";
                                } else {
                                    qDebug() << "\nOk was *not* clicked";
                                }
                            } else {
                                validPinChars = true;
                            }
                        } else {
                            QMessageBox::StandardButton alert;
                            alert = QMessageBox::information(this, "Forgot Bank PIN", "New bank PIN cannot be empty",
                                                        QMessageBox::Ok);
                            if (alert == QMessageBox::Ok) {
                                qDebug() << "\nOk was clicked";
                            } else {
                                qDebug() << "\nOk was *not* clicked";
                            }
                        }

                        if ((ui->newBankPinBox->text() == ui->confirmNewBPinBox->text()) && (!ui->newBankPinBox->text().isEmpty()) && (!ui->confirmNewBPinBox->text().isEmpty())){
                            newPinEqualsConfirmPin = true;
                        } else {
                            QMessageBox::StandardButton alert;
                            alert = QMessageBox::information(this, "Forgot Bank PIN", "Confirm Bank PIN must be the same as the new PIN entered",
                                                        QMessageBox::Ok);
                            if (alert == QMessageBox::Ok) {
                                qDebug() << "\nOk was clicked";
                            } else {
                                qDebug() << "\nOk was *not* clicked";
                            }
                        }

                        if ((validPinLength == true) && (validPinChars == true) && (newPinEqualsConfirmPin == true)) {
                            QSqlQuery q1;
                            q1.prepare("UPDATE users SET pin = :newBankPin WHERE username = :username AND securityQuestion = :secQuestion AND securityAnswer = :secAnswer");
                            q1.bindValue(":newBankPin", newBankPin);
                            q1.bindValue(":username", username);
                            q1.bindValue(":secQuestion", secQuestion);
                            q1.bindValue(":secAnswer", secAnswer);
                            if (q1.exec()) {
                                QMessageBox::StandardButton alert;
                                alert = QMessageBox::information(this, "Forgot Bank PIN", "Bank PIN updated",
                                                            QMessageBox::Ok);
                                if (alert == QMessageBox::Ok) {
                                    qDebug() << "\nOk was clicked";
                                    this->hide();
                                    LoginWindow *lw= new LoginWindow();
                                    lw->show();
                                } else {
                                    qDebug() << "\nOk was *not* clicked";
                                }
                            }
                        }

                    } else {
                        QMessageBox::StandardButton alert;
                        alert = QMessageBox::information(this, "Forgot Bank PIN", "Security question or answer is invalid.\nAlso ensure your confirm bank PIN equals your new bank PIN",
                                                    QMessageBox::Ok);
                        if (alert == QMessageBox::Ok) {
                            qDebug() << "\nOk was clicked";
                        } else {
                            qDebug() << "\nOk was *not* clicked";
                        }
                    }
                }
            } else {
                QMessageBox::StandardButton alert;
                alert = QMessageBox::information(this, "Forgot Bank PIN", "Username does not exist",
                                            QMessageBox::Ok);
                if (alert == QMessageBox::Ok) {
                    qDebug() << "\nOk was clicked";
                } else {
                    qDebug() << "\nOk was *not* clicked";
                }
            }
        }
    } else {
        QMessageBox::StandardButton alert;
        alert = QMessageBox::information(this, "Forgot Bank PIN", "A username must be entered",
                                    QMessageBox::Ok);
        if (alert == QMessageBox::Ok) {
            qDebug() << "\nOk was clicked";
        } else {
            qDebug() << "\nOk was *not* clicked";
        }
    }
}

