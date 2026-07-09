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

    // Ép liên kết nút bấm đề phòng trường hợp không khớp Auto-Connect của Qt
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
    if (!ui->Txttaikhoan || !ui->Txtpassword) {
        QMessageBox::critical(this, "Lỗi hệ thống", "Không tìm thấy ô nhập liệu Txttaikhoan hoặc Txtpassword!");
        return;
    }

    QString username = ui->Txttaikhoan->text().trimmed();
    QString password = ui->Txtpassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng nhập đầy đủ tài khoản và mật khẩu!");
        return;
    }

    QUrl url("http://127.0.0.1:3000/api/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject data;
    data["username"] = username;
    data["password"] = password;
    QJsonDocument doc(data);

    QNetworkReply *reply = networkManager->post(request, doc.toJson());

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument resDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObj = resDoc.object();

            if (jsonObj["success"].toBool()) {
                QMessageBox::information(this, "Thành công", "Đăng nhập thành công!");
                this->hide();

                MainWindow *mainWin = new MainWindow();
                mainWin->setLoggedInUser(username);
                mainWin->showMaximized();
            } else {
                QMessageBox::warning(this, "Thất bại", "Tài khoản hoặc mật khẩu không đúng!");
            }
        } else {
            QMessageBox::critical(this, "Lỗi kết nối",
                                  "Không thể kết nối đến máy chủ Python API!\n"
                                  "Hãy chắc chắn rằng bạn đã chạy lệnh 'python server.py' bên VS Code.");
        }
        reply->deleteLater();
    });
}

// Cách 1: Chuyển sang Đăng ký bằng nút bấm thông thường
void LoginDialog::on_BtnGoToRegister_clicked()
{
    this->hide(); // Ẩn tạm thời form đăng nhập
    RegisterDialog regDlg(this);
    regDlg.exec(); // Mở form đăng ký dạng cửa sổ tương tác (Modal)
    this->show(); // Khi đóng form đăng ký, hiện lại form đăng nhập
}

// Cách 2: Chuyển sang Đăng ký bằng cách click vào dòng chữ link của label_2
void LoginDialog::on_label_2_linkActivated(const QString &link)
{
    Q_UNUSED(link);
    this->hide(); // Ẩn tạm thời form đăng nhập
    RegisterDialog regDlg(this);
    regDlg.exec(); // Mở màn hình đăng ký lên
    this->show(); // Hiện lại form đăng nhập sau khi tắt form đăng ký
}