#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <vector>
#include <limits>
#include <windows.h>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

//================== XOA MAN HINH ==================
void xoaManHinh()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

//================== DUNG ==================
void dungManHinh()
{
    cout << "\nNhan Enter de tiep tuc...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

//================== DANG NHAP ADMIN ==================
void dangnhap()
{
    vector<string> DS_ADMIN =
    {
        "Akira",
        "Khoa",
        "Hieu",
        "khanh"
    };

    vector<string> DS_PASS =
    {
        "1412",
        "1234",
        "1234",
        "1234"
    };

    vector<string> DS_ROLE =
    {
        "ADMIN",
        "MANAGER",
        "DEVELOPER",
        "TESTER"
    };

    string tk, mk;

    while (true)
    {
        xoaManHinh();

        cout << "=========================================\n";
        cout << "        HE THONG DANG NHAP ADMIN\n";
        cout << "=========================================\n";

        cout << "Tai khoan : ";
        getline(cin, tk);

        cout << "Mat khau  : ";
        getline(cin, mk);

        bool ok = false;
        string role;

        for (size_t i = 0; i < DS_ADMIN.size(); i++)
        {
            if (tk == DS_ADMIN[i] && mk == DS_PASS[i])
            {
                ok = true;
                role = DS_ROLE[i];
                break;
            }
        }

        if (ok)
        {
            cout << "\nDang nhap thanh cong!\n";
            cout << "Xin chao " << role << endl;
            Sleep(1500);
            return;
        }

        cout << "\nSai tai khoan hoac mat khau!\n";
        dungManHinh();
    }
}

//================== TAI JSON TU SERVER ==================
void taiDuLieu()
{
    system("curl.exe -s http://127.0.0.1:3000/api/registrations -o registrations.json");
}

//================== HIEN THI ==================
void hienThiDangKy()
{
    taiDuLieu();

    ifstream file("registrations.json");

    if (!file)
    {
        cout << "Khong doc duoc registrations.json\n";
        return;
    }

    json data;

    try
    {
        file >> data;
    }
    catch (...)
    {
        cout << "Du lieu JSON khong hop le!\n";
        return;
    }

    file.close();

    if (data.empty())
    {
        cout << "Chua co nguoi dung nao dang ky.\n";
        return;
    }

    cout << "==============================================================\n";
    cout << "             DANH SACH NGUOI DUNG DA DANG KY\n";
    cout << "==============================================================\n\n";

    for (auto &user : data)
    {
        cout << "Nguoi dung : "
             << user["username"] << endl;

        cout << "--------------------------------------------------------------\n";

        auto regs = user["registrations"];

        for (size_t i = 0; i < regs.size(); i++)
        {
            cout << "Dich vu " << i + 1 << endl;

            cout << left
                 << setw(18)
                 << "Ten dich vu:"
                 << regs[i]["service_name"]
                 << endl;

            cout << left
                 << setw(18)
                 << "Tong tien:"
                 << regs[i]["total_price"]
                 << endl;

            cout << left
                 << setw(18)
                 << "Het han:"
                 << regs[i]["expiry_date"]
                 << endl;

            cout << endl;
        }

        cout << "==============================================================\n\n";
    }
}

//================== MAIN ==================
int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    dangnhap();

    while (true)
    {
        xoaManHinh();

        cout << "=================================================\n";
        cout << "          HE THONG QUAN LY DANG KY\n";
        cout << "=================================================\n";
        cout << "1. Xem danh sach dang ky\n";
        cout << "0. Thoat\n";
        cout << "=================================================\n";
        cout << "Nhap lua chon: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            xoaManHinh();
            hienThiDangKy();
            dungManHinh();
            break;

        case 0:
            return 0;

        default:
            cout << "Lua chon khong hop le!\n";
            dungManHinh();
        }
    }

    return 0;
}