
#pragma once

#include <string>
#include <SFML/Graphics.hpp>

enum class LoaiVatPham {
    COC_GO,         // Cọc gỗ bịt sắt để cắm bãi cọc
    VU_KHI,         // Kiếm rèn, cung tên
    HOI_MAU,        // Bình thuốc hồi phục sinh lực
    THUYEN_ITEM,    // Cờ lệnh / thuyền chiến
    NHIEM_VU        // Vật phẩm cốt truyện
};

class VatPham {
private:
    int maVatPham;
    std::string ten;
    std::string moTa;
    LoaiVatPham loai;
    int soLuong;
    int soLuongToiDa;
    sf::Color mauBieuTuong;

public:
    VatPham(int id = 0, const std::string& tenVatPham = "", const std::string& moTaVatPham = "",
            LoaiVatPham loaiItem = LoaiVatPham::COC_GO, int sl = 1, int maxSl = 99,
            sf::Color mau = sf::Color(139, 69, 19));

    int GetMaVatPham() const { return maVatPham; }
    std::string GetTen() const { return ten; }
    std::string GetMoTa() const { return moTa; }
    LoaiVatPham GetLoai() const { return loai; }
    int GetSoLuong() const { return soLuong; }
    int GetSoLuongToiDa() const { return soLuongToiDa; }
    sf::Color GetMauBieuTuong() const { return mauBieuTuong; }

    void TangSoLuong(int sl) { soLuong += sl; }
    bool GiamSoLuong(int sl);
    void SetSoLuong(int sl) { soLuong = sl; }

    void VeBieuTuong(sf::RenderWindow& window, sf::Vector2f viTri, float kichThuoc = 32.f) const;
};


