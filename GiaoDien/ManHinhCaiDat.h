#pragma once

#include <SFML/Graphics.hpp>
#include "DanhSachNut.h"
#include "../CotLoi/LuuGame.h"

enum class HanhDongCaiDat {
    KHONG_CO,
    DOI_AM_LUONG,
    DOI_TOAN_MAN_HINH,
    QUAY_LAI
};

// Màn hình cài đặt: âm lượng và toàn màn hình
class ManHinhCaiDat {
private:
    DanhSachNut danhSachNut;

    void CapNhatNhan(const DuLieuLuu& luu);

public:
    ManHinhCaiDat();

    void Mo(const DuLieuLuu& luu);

    // Chỉnh trực tiếp vào duLieu. QuanLyGame đọc hành động trả về để áp dụng ra cửa sổ và âm thanh
    HanhDongCaiDat XuLySuKien(const sf::Event& suKien, const sf::RenderWindow& window, DuLieuLuu& duLieu);
    void CapNhat(float deltaTime) { danhSachNut.CapNhat(deltaTime); }
    void Ve(sf::RenderWindow& window, const sf::Font& font, const DuLieuLuu& duLieu) const;
};
