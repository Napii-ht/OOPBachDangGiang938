
#pragma once

#include "VatPham.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class TuDo {
private:
    std::vector<VatPham> danhSachVatPham;
    int sucChua;
    int oDangChon; // Ô hotbar đang kích hoạt (0..4)

public:
    TuDo(int kichThuoc = 6);
    ~TuDo() = default;

    bool ThemVatPham(const VatPham& vp);
    bool XoaVatPham(int maVatPham, int soLuong = 1);
    bool KiemTraVatPham(int maVatPham, int soLuong = 1) const;
    int GetSoLuongVatPham(int maVatPham) const;

    const std::vector<VatPham>& GetDanhSachVatPham() const { return danhSachVatPham; }
    int GetSucChua() const { return sucChua; }

    void SetODangChon(int index);
    int GetODangChon() const { return oDangChon; }
    const VatPham* GetVatPhamDangChon() const;

    // Vẽ thanh Hotbar túi đồ phong cách Terraria ở góc trên
    void VeHotbar(sf::RenderWindow& window, const sf::Font& font, sf::Vector2f viTriGoc = {20.f, 75.f});
};


