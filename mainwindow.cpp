#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    networkManager = new QNetworkAccessManager(this);

    // Thời hạn
    ui->cbDuration->addItem("-- Chọn gói thời hạn --", 0);
    ui->cbDuration->addItem("1 Tháng", 1);
    ui->cbDuration->addItem("3 Tháng", 3);
    ui->cbDuration->addItem("6 Tháng", 6);
    ui->cbDuration->addItem("12 Tháng", 12);

    connect(ui->cbService,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::updateCalculations);

    connect(ui->cbDuration,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::updateCalculations);

    fetchServicesFromMongo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLoggedInUser(const QString &username)
{
    currentUsername = username;
}

void MainWindow::fetchServicesFromMongo()
{
    QUrl url("http://127.0.0.1:3000/api/get-services");

    QNetworkReply *reply =
        networkManager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, [=]() {

        if(reply->error()==QNetworkReply::NoError)
        {
            serviceArray =
                QJsonDocument::fromJson(reply->readAll()).array();

            ui->cbService->clear();
            ui->cbService->addItem("-- Chọn dịch vụ --",0);

            for(const QJsonValue &value : serviceArray)
            {
                QJsonObject obj=value.toObject();

                ui->cbService->addItem(
                    obj["service_name"].toString(),
                    obj["price_per_month"].toInt()
                    );
            }
        }
        else
        {
            QMessageBox::warning(this,
                                 "Lỗi",
                                 "Không tải được danh sách dịch vụ.");
        }

        reply->deleteLater();
    });
}

void MainWindow::updateCalculations()
{
    int pricePerMonth =
        ui->cbService->currentData().toInt();

    int months =
        ui->cbDuration->currentData().toInt();

    if(pricePerMonth>0 && months>0)
    {
        int total=pricePerMonth*months;

        ui->lblPrice->setText(QString::number(total)+" VNĐ");

        ui->lblExpiry->setText(
            QDate::currentDate()
                .addMonths(months)
                .toString("dd/MM/yyyy"));
    }
    else
    {
        ui->lblPrice->setText("0 VNĐ");
        ui->lblExpiry->setText("--/--/----");
    }
}

void MainWindow::on_BtnAddMore_clicked()
{
    ui->cbService->setCurrentIndex(0);
    ui->cbDuration->setCurrentIndex(0);

    ui->lblPrice->setText("0 VNĐ");
    ui->lblExpiry->setText("--/--/----");
}

void MainWindow::on_BtnConfirm_clicked()
{
    if(ui->cbService->currentIndex()==0)
    {
        QMessageBox::warning(
            this,
            "Thông báo",
            "Vui lòng chọn dịch vụ.");

        return;
    }

    if(ui->cbDuration->currentIndex()==0)
    {
        QMessageBox::warning(
            this,
            "Thông báo",
            "Vui lòng chọn thời hạn.");

        return;
    }

    QNetworkRequest request(
        QUrl("http://127.0.0.1:3000/api/save-registration"));

    request.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json");

    QJsonObject registration;

    registration["service_name"]=ui->cbService->currentText();
    registration["total_price"]=ui->lblPrice->text();
    registration["expiry_date"]=ui->lblExpiry->text();

    QJsonArray arr;
    arr.append(registration);

    QJsonObject data;

    if(currentUsername.isEmpty())
        data["username"]="Khách hàng";
    else
        data["username"]=currentUsername;

    data["registrations"]=arr;

    QNetworkReply *reply=
        networkManager->post(
            request,
            QJsonDocument(data).toJson());

    connect(reply,&QNetworkReply::finished,[=](){

        if(reply->error()==QNetworkReply::NoError)
        {
            QMessageBox::information(
                this,
                "Thành công",
                "Đăng ký dịch vụ thành công.");

            ui->cbService->setCurrentIndex(0);
            ui->cbDuration->setCurrentIndex(0);

            ui->lblPrice->setText("0 VNĐ");
            ui->lblExpiry->setText("--/--/----");
        }
        else
        {
            QMessageBox::critical(
                this,
                "Lỗi",
                reply->errorString());
        }

        reply->deleteLater();
    });
}