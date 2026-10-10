cmake_minimum_required(VERSION 3.16)

project(OOPBachDangGiang938 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(OOPBachDangGiang938
    main.cpp

    BanDo/BanDo.cpp
    DoiTuong/BaiCoc.cpp
    DoiTuong/ThuyTrieu.cpp
    DoiTuong/VatPham.cpp
    DoiTuong/TuDo.cpp
    Thuyen/Thuyen.cpp
    
    NhanVat/NhanVat.cpp
    NhanVat/NguoiChoi.cpp

    NhanVat/KeDich.cpp
    ChienDau/HeThongAI.cpp
    ChienDau/HeThongChienDau.cpp
    ChienDau/ThuyenKeDich.cpp
)

target_link_libraries(OOPBachDangGiang938
    PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)