#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "DanhSachNut.h"

enum class LuaChonMenu {
    CHOI,         // Mở màn hình chọn chương
    HUONG_DAN,
    CAI_DAT,
    GIOI_THIEU,
    THOAT,
    KHONG_CO
};

// Menu chính: tiêu đề game và các nút. Nền do CanhNenMenu vẽ.
class MenuChinh {
private:
    DanhSachNut danhSachNut;
    bool dangMoHuongDan;
    bool dangMoGioiThieu;
    float thoiGian;

    void VeKhungPhu(sf::RenderWindow& window, const sf::Font& font, const std::string& tieuDe, const std::string& noiDung) const;

public:
    MenuChinh();

    LuaChonMenu XuLySuKien(const sf::Event& suKien, const sf::RenderWindow& window);
    void CapNhat(float deltaTime);
    void Ve(sf::RenderWindow& window, const sf::Font& font) const;

    bool IsDangMoMenuCon() const { return dangMoHuongDan || dangMoGioiThieu; }
};
