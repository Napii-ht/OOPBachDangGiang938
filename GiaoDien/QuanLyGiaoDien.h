
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "HopThoai.h"
#include "../NhanVat/NguoiChoi.h"

struct SoSatThuong {
    sf::Vector2f viTri;
    std::string text;
    sf::Color mau;
    float thoiGianTonTai;
    float thoiGianToiDa;
};

struct HatHieuUng {
    sf::Vector2f viTri;
    sf::Vector2f vanToc;
    sf::Color mau;
    float kichThuoc;
    float thoiGianTonTai;
    float thoiGianToiDa;
};

class QuanLyGiaoDien {
private:
    const sf::Font& phongChu;   // Font dùng chung, do QuanLyGame nạp một lần
    HopThoai hopThoai;

    std::vector<SoSatThuong> danhSachSoSatThuong;
    std::vector<HatHieuUng> danhSachHatHieuUng;

public:
    explicit QuanLyGiaoDien(const sf::Font& font);
    ~QuanLyGiaoDien() = default;

    void CapNhat(float deltaTime);

    // Vẽ thanh máu, thể lực phong cách Terraria ở góc trên bên trái
    void VeHUD(sf::RenderWindow& window, const NguoiChoi& player);

    // Hiệu ứng số sát thương nảy lên (Floating Damage Numbers)
    void ThemSoSatThuong(sf::Vector2f viTri, int satThuong, bool chiMang = false);

    // Hiệu ứng hạt (Particles)
    void ThemHieuUngChem(sf::Vector2f viTri);
    void ThemHieuUngNuocBan(sf::Vector2f viTri);
    void ThemHieuUngVaChamGo(sf::Vector2f viTri);

    void VeHieuUngVaSatThuong(sf::RenderWindow& window);

    // Màn hình Chiến Thắng / Thất Bại
    void VeManHinhChienThang(sf::RenderWindow& window);
    void VeManHinhGameOver(sf::RenderWindow& window);

    const sf::Font& GetPhongChu() const { return phongChu; }
    HopThoai& GetHopThoai() { return hopThoai; }
};


