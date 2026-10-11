#include "ManHinhCaiDat.h"
#include "../CotLoi/TienIch.h"
#include <algorithm>

ManHinhCaiDat::ManHinhCaiDat() {
    danhSachNut.DatDanhSach({"", "", "Quay lại"});
    danhSachNut.DatBoCuc({RONG_MAN_HINH * 0.5f, 215.f}, 380.f, 52.f, 66.f, 14);
}

void ManHinhCaiDat::CapNhatNhan(const DuLieuLuu& luu) {
    danhSachNut.DatNhan(0, "Âm lượng: " + std::to_string(luu.amLuong) + "%");
    danhSachNut.DatNhan(1, std::string("Toàn màn hình: ") + (luu.toanManHinh ? "Bật" : "Tắt"));
}

void ManHinhCaiDat::Mo(const DuLieuLuu& luu) {
    danhSachNut.DatChon(0);
    CapNhatNhan(luu);
}

HanhDongCaiDat ManHinhCaiDat::XuLySuKien(const sf::Event& suKien, const sf::RenderWindow& window, DuLieuLuu& duLieu) {
    if (const auto* phim = suKien.getIf<sf::Event::KeyPressed>()) {
        if (phim->code == sf::Keyboard::Key::Escape) return HanhDongCaiDat::QUAY_LAI;

        // Trái phải chỉnh âm lượng khi đang đứng ở dòng âm lượng
        if (danhSachNut.GetChon() == 0) {
            if (phim->code == sf::Keyboard::Key::Left || phim->code == sf::Keyboard::Key::A) {
                duLieu.amLuong = std::max(0, duLieu.amLuong - 10);
                CapNhatNhan(duLieu);
                return HanhDongCaiDat::DOI_AM_LUONG;
            }
            if (phim->code == sf::Keyboard::Key::Right || phim->code == sf::Keyboard::Key::D) {
                duLieu.amLuong = std::min(100, duLieu.amLuong + 10);
                CapNhatNhan(duLieu);
                return HanhDongCaiDat::DOI_AM_LUONG;
            }
        }
    }

    switch (danhSachNut.XuLySuKien(suKien, window)) {
        case 0:
            // Enter hoặc click vào dòng âm lượng thì tăng 10, quá 100 thì quay về 0
            duLieu.amLuong = (duLieu.amLuong >= 100) ? 0 : duLieu.amLuong + 10;
            CapNhatNhan(duLieu);
            return HanhDongCaiDat::DOI_AM_LUONG;
        case 1:
            duLieu.toanManHinh = !duLieu.toanManHinh;
            CapNhatNhan(duLieu);
            return HanhDongCaiDat::DOI_TOAN_MAN_HINH;
        case 2:
            return HanhDongCaiDat::QUAY_LAI;
        default:
            return HanhDongCaiDat::KHONG_CO;
    }
}

void ManHinhCaiDat::Ve(sf::RenderWindow& window, const sf::Font& font, const DuLieuLuu& duLieu) const {
    sf::RectangleShape lamToi({RONG_MAN_HINH, CAO_MAN_HINH});
    lamToi.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(lamToi);

    VeChuGiua(window, font, "CÀI ĐẶT", 28, {RONG_MAN_HINH * 0.5f, 100.f}, sf::Color(255, 215, 0), 2.f, sf::Color(120, 40, 20));
    danhSachNut.Ve(window, font);

    // Thanh âm lượng nằm trong nút đầu tiên
    float x = RONG_MAN_HINH * 0.5f - 140.f;
    float y = 215.f + 15.f;
    sf::RectangleShape nen({280.f, 6.f});
    nen.setPosition({x, y});
    nen.setFillColor(sf::Color(15, 18, 26));
    nen.setOutlineThickness(1.f);
    nen.setOutlineColor(sf::Color(100, 115, 140));
    window.draw(nen);

    sf::RectangleShape day({280.f * static_cast<float>(duLieu.amLuong) / 100.f, 6.f});
    day.setPosition({x, y});
    day.setFillColor(sf::Color(255, 215, 0));
    window.draw(day);

    VeChuGiua(window, font, "Trái/Phải chỉnh âm lượng, Enter để đổi, Esc để quay lại", 11, {RONG_MAN_HINH * 0.5f, 578.f}, sf::Color(170, 185, 205));
}
