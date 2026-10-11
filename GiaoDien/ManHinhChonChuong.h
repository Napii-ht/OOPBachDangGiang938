#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "DanhSachNut.h"
#include "../CotLoi/QuanLyManChoi.h"
#include "../CotLoi/LuuGame.h"

// Màn hình chọn chương: danh sách bên trái, mô tả chương đang chọn bên phải
class ManHinhChonChuong {
private:
    DanhSachNut danhSachNut;
    std::vector<ThongTinChuong> chuong;   // Bản sao thông tin để vẽ (không cần hàm tạo màn)
    int chuongDaMo;
    int chuongHoanThanh;

    bool ChuongChoiDuoc(const ThongTinChuong& c) const;

public:
    ManHinhChonChuong();

    // Gọi mỗi lần mở màn hình này để cập nhật trạng thái khóa theo tiến độ đã lưu
    void DatDuLieu(const std::vector<ThongTinChuong>& danhSach, const DuLieuLuu& luu);

    static constexpr int QUAY_LAI = -2;
    static constexpr int CHUA_CHON = -1;

    // Trả về chỉ số chương cần chơi, QUAY_LAI hoặc CHUA_CHON
    int XuLySuKien(const sf::Event& suKien, const sf::RenderWindow& window);
    void CapNhat(float deltaTime) { danhSachNut.CapNhat(deltaTime); }
    void Ve(sf::RenderWindow& window, const sf::Font& font) const;
};
