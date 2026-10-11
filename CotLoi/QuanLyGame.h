#pragma once

#include <SFML/Graphics.hpp>
#include "QuanLyManChoi.h"
#include "LuuGame.h"
#include "ManChoi.h"
#include "../AmThanh/QuanLyAmThanh.h"
#include "../GiaoDien/CanhNenMenu.h"
#include "../GiaoDien/MenuChinh.h"
#include "../GiaoDien/MenuTamDung.h"
#include "../GiaoDien/ManHinhChonChuong.h"
#include "../GiaoDien/ManHinhCaiDat.h"
#include "../GiaoDien/ManHinhKetThuc.h"

// Các trạng thái cấp cao nhất của game
enum class TrangThaiGame {
    MENU,
    CHON_CHUONG,
    CAI_DAT,
    DANG_CHOI,
    TAM_DUNG,
    KET_THUC      // Lời bạt và credits sau khi thắng chương cuối
};

// Lõi của game: giữ cửa sổ, font, âm thanh, dữ liệu lưu và điều phối giữa menu với các chương.
// Phần chơi cụ thể nằm trong từng lớp ManChoi do QuanLyManChoi quản lý.
class QuanLyGame {
private:
    sf::RenderWindow& window;
    sf::Font phongChu;
    QuanLyAmThanh amThanh;
    DuLieuLuu duLieuLuu;
    NguCanhGame nguCanh;

    QuanLyManChoi quanLyManChoi;
    CanhNenMenu canhNen;
    MenuChinh menuChinh;
    MenuTamDung menuTamDung;
    ManHinhChonChuong manHinhChonChuong;
    ManHinhCaiDat manHinhCaiDat;
    ManHinhKetThuc manHinhKetThuc;

    bool daNapFont;
    TrangThaiGame trangThai;
    TrangThaiGame trangThaiTruocCaiDat;   // Mở cài đặt từ đâu thì đóng lại về đó
    TrangThaiGame trangThaiSauKhiThoatMan;

    void DangKyCacChuong();
    void ChuyenSangMenu();
    void ChuyenSangChonChuong();
    void BatDauChuong(int chiSo);
    void XuLyKhiManKetThuc();
    void ApDungToanManHinh(bool bat);
    void ApDungAmLuong();
    void LuuTienDo();

public:
    explicit QuanLyGame(sf::RenderWindow& win);

    void XuLySuKien(const sf::Event& suKien);
    void CapNhat(float deltaTime);
    void Ve();

    // false nếu thiếu font, khi đó main.cpp báo lỗi và thoát thay vì để game bị crash
    bool SanSang() const { return daNapFont; }
    TrangThaiGame GetTrangThai() const { return trangThai; }
};
