#include "logindialog.h"
#include "ui_logindialog.h"
#include "mainwindow.h"
#include "registerdialog.h"
#include <QMessageBox>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    networkManager = new QNetworkAccessManager(this);

    // 🚀 GIẢI PHÁP ÉP LIÊN KẾT NÚT BẤM:
    // Nếu tên nút Login trên giao diện UI của bạn đang đặt là "login" (chữ l viết thường)
    // Đoạn code dưới đây sẽ ép hệ thống bắt buộc phải gọi hàm đăng nhập khi click chuột.
    if (ui->login) {
        connect(ui->login, &QPushButton::clicked, this, &LoginDialog::on_BtnLogin_clicked);
    }
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

// Hàm xử lý kiểm tra và xác nhận Đăng nhập
void LoginDialog::on_BtnLogin_clicked()
{
    // Kiểm tra tính an toàn chống crash nếu linh kiện UI chưa load kịp
    if (!ui->Txttaikhoan || !ui->Txtpassword) {
        QMessageBox::critical(this, "Lỗi hệ thống", "Không tìm thấy ô nhập liệu Txttaikhoan hoặc Txtpassword!");
        return;
    }

    // Lấy dữ liệu chuẩn xác từ UI của bạn
    QString username = ui->Txttaikhoan->text();
    QString password = ui->Txtpassword->text();

    // Kiểm tra trống dữ liệu đầu vào
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng nhập đầy đủ tài khoản và mật khẩu!");
        return;
    }

    // Thiết lập kết nối API tới server FastAPI
    QUrl url("http://127.0.0.1:3000/api/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Đóng gói JSON
    QJsonObject data;
    data["username"] = username;
    data["password"] = password;
    QJsonDocument doc(data);

    // Thực hiện gửi gói tin đăng nhập
    QNetworkReply *reply = networkManager->post(request, doc.toJson());

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument resDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = resDoc.object();

            if (jsonObj["success"].toBool()) {
                QMessageBox::information(this, "Thành công", "Đăng nhập thành công!");
                this->hide();

                // Chuyển sang màn hình chính và gán User định danh hóa đơn
                MainWindow *mainWin = new MainWindow();
                mainWin->setLoggedInUser(ui->Txttaikhoan->text());
                mainWin->showMaximized();
            } else {
                QMessageBox::warning(this, "Thất bại", "Tài khoản hoặc mật khẩu không đúng!");
            }
        } else {
            // Trường hợp Server Python chưa bật hoặc lỗi kết nối mạng nội bộ
            QMessageBox::critical(this, "Lỗi kết nối",
                                  "Không thể kết nối đến máy chủ Python API!\n"
                                  "Hãy chắc chắn rằng bạn đã chạy lệnh 'python server.py' bên VS Code.");
        }
        reply->deleteLater();
    });
}

void LoginDialog::on_BtnGoToRegister_clicked()
{
    RegisterDialog regDlg(this);
    regDlg.exec();
}