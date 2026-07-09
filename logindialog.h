#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginDialog; }
QT_END_NAMESPACE

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_BtnLogin_clicked();
    void on_BtnGoToRegister_clicked();

private:
    Ui::LoginDialog *ui;
    QNetworkAccessManager *networkManager;
};
#endif // LOGINDIALOG_H