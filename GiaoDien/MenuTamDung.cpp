#include "MenuTamDung.h"
#include "../CotLoi/TienIch.h"

MenuTamDung::MenuTamDung() {
    danhSachNut.DatDanhSach({"Tiếp tục", "Cài đặt", "Về menu chính"});
    danhSachNut.DatBoCuc({RONG_MAN_HINH * 0.5f, 270.f}, 300.f, 40.f, 52.f, 15);
}

LuaChonTamDung MenuTamDung::XuLySuKien(const sf::Event& suKien, const sf::RenderWindow& window) {
    // Esc lần nữa để tiếp tục chơi
    if (const auto* phim = suKien.getIf<sf::Event::KeyPressed>()) {
        if (phim->code == sf::Keyboard::Key::Escape) return LuaChonTamDung::TIEP_TUC;
    }

    switch (danhSachNut.XuLySuKien(suKien, window)) {
        case 0: return LuaChonTamDung::TIEP_TUC;
        case 1: return LuaChonTamDung::CAI_DAT;
        case 2: return LuaChonTamDung::VE_MENU;
        default: return LuaChonTamDung::KHONG_CO;
    }
}

void MenuTamDung::Ve(sf::RenderWindow& window, const sf::Font& font) const {
    sf::RectangleShape lamToi({RONG_MAN_HINH, CAO_MAN_HINH});
    lamToi.setFillColor(sf::Color(0, 0, 0, 165));
    window.draw(lamToi);

    sf::RectangleShape khung({380.f, 300.f});
    khung.setOrigin({190.f, 150.f});
    khung.setPosition({RONG_MAN_HINH * 0.5f, 295.f});
    khung.setFillColor(sf::Color(20, 25, 35, 245));
    khung.setOutlineThickness(3.f);
    khung.setOutlineColor(sf::Color(255, 215, 0));
    window.draw(khung);

    VeChuGiua(window, font, "TẠM DỪNG", 24, {RONG_MAN_HINH * 0.5f, 185.f}, sf::Color(255, 215, 0), 1.5f, sf::Color(120, 40, 20));
    danhSachNut.Ve(window, font);
    VeChuGiua(window, font, "Esc để tiếp tục", 11, {RONG_MAN_HINH * 0.5f, 425.f}, sf::Color(170, 185, 205));
}
