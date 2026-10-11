#pragma once

#include <SFML/Graphics.hpp>
#include "DanhSachNut.h"

enum class LuaChonTamDung {
    TIEP_TUC,
    CAI_DAT,
    VE_MENU,
    KHONG_CO
};

// Menu hiện ra khi bấm Esc trong lúc chơi
class MenuTamDung {
private:
    DanhSachNut danhSachNut;

public:
    MenuTamDung();

    void MoMenu() { danhSachNut.DatChon(0); }
    LuaChonTamDung XuLySuKien(const sf::Event& suKien, const sf::RenderWindow& window);
    void CapNhat(float deltaTime) { danhSachNut.CapNhat(deltaTime); }
    void Ve(sf::RenderWindow& window, const sf::Font& font) const;
};
