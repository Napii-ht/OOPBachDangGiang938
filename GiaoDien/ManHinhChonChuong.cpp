#include "ManHinhChonChuong.h"
#include "../CotLoi/TienIch.h"
#include <algorithm>

ManHinhChonChuong::ManHinhChonChuong() : chuongDaMo(1), chuongHoanThanh(0) {
    danhSachNut.DatBoCuc({215.f, 140.f}, 380.f, 38.f, 46.f, 13);
}

bool ManHinhChonChuong::ChuongChoiDuoc(const ThongTinChuong& c) const {
    if (!c.daCoSan) return false;
    if (c.ma >= 1 && c.ma > chuongDaMo) return false;
    return true;
}

void ManHinhChonChuong::DatDuLieu(const std::vector<ThongTinChuong>& danhSach, const DuLieuLuu& luu) {
    chuong = danhSach;
    chuongDaMo = luu.chuongDaMo;
    chuongHoanThanh = luu.chuongHoanThanh;

    std::vector<std::string> nhan;
    for (const auto& c : chuong) {
        if (c.ma >= 1) {
            std::string dong = "Chương " + std::to_string(c.ma) + ": " + c.ten;
            if (c.ma <= chuongHoanThanh) dong += "  (đã xong)";
            nhan.push_back(dong);
        } else {
            nhan.push_back(c.ten);
        }
    }
    nhan.push_back("Quay lại menu");
    danhSachNut.DatDanhSach(nhan);

    for (size_t i = 0; i < chuong.size(); ++i) {
        danhSachNut.DatKhoa(static_cast<int>(i), !ChuongChoiDuoc(chuong[i]));
    }

    // Mặc định trỏ vào chương đang chơi dở (chương mở khóa cao nhất)
    for (size_t i = 0; i < chuong.size(); ++i) {
        if (chuong[i].ma == std::min(chuongDaMo, 6)) {
            danhSachNut.DatChon(static_cast<int>(i));
            break;
        }
    }
}

int ManHinhChonChuong::XuLySuKien(const sf::Event& suKien, const sf::RenderWindow& window) {
    if (const auto* phim = suKien.getIf<sf::Event::KeyPressed>()) {
        if (phim->code == sf::Keyboard::Key::Escape) return QUAY_LAI;
    }

    int nut = danhSachNut.XuLySuKien(suKien, window);
    if (nut < 0) return CHUA_CHON;
    if (nut == static_cast<int>(chuong.size())) return QUAY_LAI;
    return nut;
}

void ManHinhChonChuong::Ve(sf::RenderWindow& window, const sf::Font& font) const {
    sf::RectangleShape lamToi({RONG_MAN_HINH, CAO_MAN_HINH});
    lamToi.setFillColor(sf::Color(0, 0, 0, 120));
    window.draw(lamToi);

    VeChuGiua(window, font, "CHỌN CHƯƠNG", 28, {RONG_MAN_HINH * 0.5f, 62.f}, sf::Color(255, 215, 0), 2.f, sf::Color(120, 40, 20));
    danhSachNut.Ve(window, font);

    // Bảng mô tả bên phải
    sf::RectangleShape bang({330.f, 380.f});
    bang.setPosition({440.f, 120.f});
    bang.setFillColor(sf::Color(20, 25, 35, 235));
    bang.setOutlineThickness(2.5f);
    bang.setOutlineColor(sf::Color(218, 165, 32));
    window.draw(bang);

    int chon = danhSachNut.GetChon();
    if (chon >= 0 && chon < static_cast<int>(chuong.size())) {
        const ThongTinChuong& c = chuong[chon];
        std::string tieuDe = (c.ma >= 1) ? "Chương " + std::to_string(c.ma) : "Màn thử nghiệm";
        VeChuTrai(window, font, tieuDe, 12, {456.f, 134.f}, sf::Color(180, 200, 225));
        VeChuTrai(window, font, NgatDong(font, c.ten, 17, 296.f), 17, {456.f, 154.f}, sf::Color(255, 215, 0));
        VeChuTrai(window, font, NgatDong(font, "Bản đồ: " + c.bangMap, 12, 296.f), 12, {456.f, 202.f}, sf::Color(150, 205, 160));
        VeChuTrai(window, font, NgatDong(font, c.moTa, 12, 296.f), 12, {456.f, 250.f}, sf::Color::White);

        std::string trangThai;
        sf::Color mau = sf::Color(120, 220, 130);
        if (!c.daCoSan) {
            trangThai = "Sắp ra mắt";
            mau = sf::Color(230, 180, 90);
        } else if (!ChuongChoiDuoc(c)) {
            trangThai = "Chưa mở khóa: hãy hoàn thành chương trước";
            mau = sf::Color(230, 120, 100);
        } else if (c.ma >= 1 && c.ma <= chuongHoanThanh) {
            trangThai = "Đã hoàn thành. Có thể chơi lại";
        } else {
            trangThai = "Có thể chơi";
        }
        VeChuTrai(window, font, NgatDong(font, trangThai, 12, 296.f), 12, {456.f, 462.f}, mau);
    } else {
        VeChuTrai(window, font, "Quay lại menu chính.", 13, {456.f, 140.f}, sf::Color::White);
    }

    VeChuGiua(window, font, "W/S hoặc chuột để chọn, Enter để vào chơi, Esc để quay lại", 11, {RONG_MAN_HINH * 0.5f, 578.f}, sf::Color(170, 185, 205));
}
