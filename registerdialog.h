#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_BtnRegisterConfirm_clicked();

private:
    Ui::RegisterDialog *ui;
    QNetworkAccessManager *networkManager;
};

#endif // REGISTERDIALOG_H