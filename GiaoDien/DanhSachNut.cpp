#include "DanhSachNut.h"
#include "../CotLoi/TienIch.h"
#include <cmath>

DanhSachNut::DanhSachNut()
    : chon(0),
      tamTren({RONG_MAN_HINH * 0.5f, 240.f}),
      rongNut(340.f),
      caoNut(42.f),
      khoangCach(52.f),
      coChu(14),
      thoiGian(0.f) {}

void DanhSachNut::DatDanhSach(const std::vector<std::string>& nhan) {
    danhSachNhan = nhan;
    danhSachKhoa.assign(nhan.size(), false);
    if (chon >= static_cast<int>(nhan.size())) chon = 0;
}

void DanhSachNut::DatNhan(int chiSo, const std::string& nhan) {
    if (chiSo >= 0 && chiSo < GetSoNut()) danhSachNhan[chiSo] = nhan;
}

void DanhSachNut::DatKhoa(int chiSo, bool khoa) {
    if (chiSo >= 0 && chiSo < GetSoNut()) danhSachKhoa[chiSo] = khoa;
}

void DanhSachNut::DatBoCuc(sf::Vector2f tamNutDau, float rong, float cao, float khoang, unsigned int co) {
    tamTren = tamNutDau;
    rongNut = rong;
    caoNut = cao;
    khoangCach = khoang;
    coChu = co;
}

void DanhSachNut::DatChon(int chiSo) {
    if (chiSo >= 0 && chiSo < GetSoNut()) chon = chiSo;
}

bool DanhSachNut::IsKhoa(int chiSo) const {
    if (chiSo < 0 || chiSo >= GetSoNut()) return true;
    return danhSachKhoa[chiSo];
}

int DanhSachNut::TimNutDuoiChuot(sf::Vector2f toaDo) const {
    for (int i = 0; i < GetSoNut(); ++i) {
        float tamY = tamTren.y + static_cast<float>(i) * khoangCach;
        if (std::abs(toaDo.x - tamTren.x) <= rongNut * 0.5f && std::abs(toaDo.y - tamY) <= caoNut * 0.5f) {
            return i;
        }
    }
    return -1;
}

int DanhSachNut::XuLySuKien(const sf::Event& suKien, const sf::RenderWindow& window) {
    int soNut = GetSoNut();
    if (soNut == 0) return -1;

    if (const auto* phim = suKien.getIf<sf::Event::KeyPressed>()) {
        if (phim->code == sf::Keyboard::Key::W || phim->code == sf::Keyboard::Key::Up) {
            chon = (chon - 1 + soNut) % soNut;
        } else if (phim->code == sf::Keyboard::Key::S || phim->code == sf::Keyboard::Key::Down) {
            chon = (chon + 1) % soNut;
        } else if (phim->code == sf::Keyboard::Key::Enter || phim->code == sf::Keyboard::Key::Space) {
            if (!IsKhoa(chon)) return chon;
        }
    } else if (const auto* chuot = suKien.getIf<sf::Event::MouseMoved>()) {
        int nut = TimNutDuoiChuot(ToaDoChuot(window, chuot->position));
        if (nut >= 0) chon = nut;
    } else if (const auto* bam = suKien.getIf<sf::Event::MouseButtonPressed>()) {
        if (bam->button == sf::Mouse::Button::Left) {
            int nut = TimNutDuoiChuot(ToaDoChuot(window, bam->position));
            if (nut >= 0) {
                chon = nut;
                if (!IsKhoa(nut)) return nut;
            }
        }
    }
    return -1;
}

void DanhSachNut::Ve(sf::RenderWindow& window, const sf::Font& font) const {
    for (int i = 0; i < GetSoNut(); ++i) {
        float y = tamTren.y + static_cast<float>(i) * khoangCach;
        bool dangChon = (i == chon);
        bool biKhoa = danhSachKhoa[i];

        sf::RectangleShape nut({rongNut, caoNut});
        nut.setOrigin({rongNut * 0.5f, caoNut * 0.5f});
        nut.setPosition({tamTren.x, y});

        if (dangChon) {
            float nhapNhay = (std::sin(thoiGian * 6.f) + 1.f) * 0.5f;
            nut.setFillColor(sf::Color(static_cast<std::uint8_t>(60 + 20 * nhapNhay),
                                       static_cast<std::uint8_t>(75 + 30 * nhapNhay), 110, 240));
            nut.setOutlineThickness(2.5f);
            nut.setOutlineColor(biKhoa ? sf::Color(150, 130, 70) : sf::Color(255, 215, 0));
        } else {
            nut.setFillColor(sf::Color(35, 42, 58, 200));
            nut.setOutlineThickness(1.5f);
            nut.setOutlineColor(sf::Color(100, 115, 140));
        }
        window.draw(nut);

        sf::Color mauChu = sf::Color::White;
        if (biKhoa) mauChu = sf::Color(130, 138, 150);
        else if (dangChon) mauChu = sf::Color(255, 230, 100);

        VeChuGiua(window, font, danhSachNhan[i], coChu, {tamTren.x, y - 1.f}, mauChu);
    }
}
