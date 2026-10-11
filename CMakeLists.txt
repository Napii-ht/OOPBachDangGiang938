cmake_minimum_required(VERSION 3.16)

project(OOPBachDangGiang938 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System QUIET)
if(NOT SFML_FOUND)
    include(FetchContent)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON)
    FetchContent_MakeAvailable(SFML)
endif()

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
    NhanVat/NhanVatPhu.cpp
    ChienDau/HeThongAI.cpp
    ChienDau/HeThongChienDau.cpp
    ChienDau/ThuyenKeDich.cpp

    Nhiemvu/Quanlynhiemvu.cpp
    CotLoi/QuanLyTranDau.cpp
    CotLoi/TienIch.cpp
)

target_link_libraries(OOPBachDangGiang938
    PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

if(MSVC)
    target_compile_options(OOPBachDangGiang938 PRIVATE /utf-8)
endif()

set_target_properties(OOPBachDangGiang938 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Tu dong chep thu muc Assets canh file exe de chay duoc tu moi noi
add_custom_command(TARGET OOPBachDangGiang938 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/Assets" "$<TARGET_FILE_DIR:OOPBachDangGiang938>/Assets")