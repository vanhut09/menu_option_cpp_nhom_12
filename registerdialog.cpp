#include "registerdialog.h"
#include "ui_registerdialog.h"
#include <QMessageBox>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    networkManager = new QNetworkAccessManager(this);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

// Xử lý sự kiện khi người dùng nhấn nút Xác nhận Đăng ký tài khoản mới
void RegisterDialog::on_BtnRegisterConfirm_clicked()
{
    QString username = ui->TxtRegisterUser->text();
    QString password = ui->TxtRegisterPassword->text();

    // Kiểm tra tính hợp lệ dữ liệu đầu vào
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng nhập đầy đủ thông tin tài khoản và mật khẩu mới!");
        return;
    }

    // Cấu hình Request gửi tới API endpoint Register của Python FastAPI
    QUrl url("http://127.0.0.1:3000/api/register");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Đóng gói dữ liệu JSON
    QJsonObject data;
    data["username"] = username;
    data["password"] = password;
    QJsonDocument doc(data);

    // Tiến hành POST dữ liệu đăng ký lên Server
    QNetworkReply *reply = networkManager->post(request, doc.toJson());
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument resDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = resDoc.object();

            // Nếu Server xử lý thành công (Tài khoản chưa tồn tại và đã chèn vào MongoDB)
            if (jsonObj["success"].toBool()) {
                QMessageBox::information(this, "Thành công", "Đăng ký tài khoản thành công!");
                this->accept(); // Đóng màn hình đăng ký và quay lại màn hình đăng nhập
            } else {
                // Hiển thị thông báo lỗi cụ thể từ Server (Ví dụ: "Tài khoản này đã tồn tại!")
                QMessageBox::warning(this, "Thất bại", jsonObj["message"].toString());
            }
        } else {
            QMessageBox::critical(this, "Lỗi", "Không thể kết nối tới Server Python!");
        }
        reply->deleteLater(); // Giải phóng vùng nhớ của reply
    });
}