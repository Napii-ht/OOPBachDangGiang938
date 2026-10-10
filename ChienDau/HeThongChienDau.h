#pragma once

#include "../NhanVat/KeDich.h"
#include "../Thuyen/Thuyen.h"
#include "HeThongAI.h"
#include <vector>
#include <unordered_set>
#include <memory>
#include <SFML/Graphics.hpp>

class BanDo; // Forward declaration

class HeThongChienDau {
private:
    std::vector<std::unique_ptr<KeDich>> danhSachKeDich;          // Bộ binh Nam Hán
    std::vector<std::unique_ptr<ThuyenKeDich>> danhSachThuyenDich; // Hạm đội chiến thuyền Nam Hán
    HeThongAI heThongAI;
    int soKeDichConLai;
    bool daKhoiTaoTranChien;

    // Chống bug dính sát thương nhiều lần trong 1 nhát kiếm
    std::unordered_set<const KeDich*> danhSachDaBiChemTrongNhatNay;

public:
    HeThongChienDau();
    ~HeThongChienDau() = default;

    // Sinh bộ binh và Tướng Lưu Hoằng Thao trong trận phản công cuối
    void ThemKeDich(sf::Vector2f viTri, float tocDo = 100.f, int mau = 80, int dame = 10,
                    LoaiKeDich loai = LoaiKeDich::LINH_THUONG);
    void TaoTranChienCuoi(int soLuongDich = 6);

    // Màn hải chiến nhử địch (Chương 4 & 5)
    void TaoDoanThuyenNamHan(int soLuong = 4);
    void CapNhatHaiChien(float deltaTime, sf::Vector2f viTriThuyenPlayer, BanDo& banDo);
    void VeHaiChien(sf::RenderWindow& window);
    int GetSoThuyenConSong() const;
    bool KiemTraTatCaThuyenDaBiPhaHuy() const;

    // Cập nhật và vẽ bộ chiến
    void CapNhat(float deltaTime, NhanVat& player);
    void Ve(sf::RenderWindow& window);

    // Chém kiếm
    int XuLyPlayerTanCong(const sf::FloatRect& vungTanCong, int satThuong, bool playerDangChem);

    // Kiểm tra kết quả
    int GetSoKeDichConLai() const { return soKeDichConLai; }
    bool KiemTraChienThang() const;
    void XoaToanBo();

    const std::vector<std::unique_ptr<KeDich>>& GetDanhSachKeDich() const { return danhSachKeDich; }
    std::vector<std::unique_ptr<ThuyenKeDich>>& GetDanhSachThuyenDich() { return danhSachThuyenDich; }

    // Vẽ thanh máu Boss Lưu Hoằng Thao phong cách Terraria ở giữa đỉnh màn hình
    void VeBossBar(sf::RenderWindow& window, const sf::Font& font);
};
