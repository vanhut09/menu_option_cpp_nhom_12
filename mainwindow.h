#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setLoggedInUser(const QString &username);
    void fetchServicesFromMongo();
    void updateCalculations();

private slots:
    void on_BtnAddMore_clicked();
    void on_BtnConfirm_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;
    QJsonArray serviceArray;
    QString currentUsername;
};
#endif // MAINWINDOW_H