#include "logindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Khởi tạo và hiển thị màn hình Đăng nhập độc lập
    LoginDialog login;
    login.show();

    // Duy trì luồng chạy xuyên suốt cho toàn bộ ứng dụng
    return a.exec();
}