cmake_minimum_required(VERSION 3.16)

project(Super_System VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Thêm gói Network để Qt có thể gọi sang Server Python của bạn
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

# Quản lý các file code nguồn của bạn (Đã bỏ file anh.qrc bị lỗi đường dẫn)
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    logindialog.cpp
    logindialog.h
    logindialog.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Super_System
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        registerdialog.h registerdialog.cpp registerdialog.ui
    )
else()
    add_executable(Super_System ${PROJECT_SOURCES})
endif()

# Liên kết ứng dụng với thành phần mạng của Qt
target_link_libraries(Super_System PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Super_System)
endif()