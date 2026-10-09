#pragma once

#include "NhanVat.h"
#include <SFML/Graphics.hpp>
#include <string>

enum class TrangThaiKeDich {
    DUNG_YEN,    // Chưa thấy ai thì đứng yên
    DUOI_THEO,   // Phát hiện mục tiêu thì áp sát
    TAN_CONG,    // Đủ cự ly thì chém
    BI_THUONG,   // Bị trúng đòn khựng lại
    DA_CHET      // Hết máu ngã xuống
};

enum class LoaiKeDich {
    LINH_THUONG,        // Lính bộ binh Nam Hán
    TUONG_HOANG_THAO    // Tướng giặc Lưu Hoằng Thao (Chỉ huy)
};

class KeDich : public NhanVat {
private:
    LoaiKeDich loai;
    float tamDanh;
    float tamPhatHien;
    TrangThaiKeDich trangThaiHienTai;

    float thoiGianHoiChieu;
    float thoiGianTroiQua;
    int huongNhin;

    // Hoạt ảnh & Nghệ thuật tạo hình
    float thoiGianHoatAnh;
    float nhipBuocChan;
    float thoiGianBiThuong; // Nháy trắng khi trúng đòn

    sf::VertexArray mangDinh{sf::PrimitiveType::Triangles};

public:
    KeDich(sf::Vector2f viTriBanDau, float tocDo = 120.f, int mau = 100, int dame = 15,
           float tamDanh = 42.f, float tamPhatHien = 260.f, LoaiKeDich loaiDich = LoaiKeDich::LINH_THUONG);
    virtual ~KeDich() = default;

    void CapNhat(float deltaTime) override;
    void Ve(sf::RenderWindow& window) override;
    void NhanSatThuong(int damage) override;

    bool PhatHienNguoiChoi(sf::Vector2f viTriPlayer) const;
    void DuoiTheo(sf::Vector2f viTriPlayer, float deltaTime);
    bool CoTheTanCong(sf::Vector2f viTriPlayer) const;
    bool TanCong(NhanVat& mucTieu);
    void Chet();

    float GetTamDanh() const { return tamDanh; }
    float GetTamPhatHien() const { return tamPhatHien; }
    TrangThaiKeDich GetTrangThai() const { return trangThaiHienTai; }
    void SetTrangThai(TrangThaiKeDich st) { trangThaiHienTai = st; }
    LoaiKeDich GetLoai() const { return loai; }
    bool IsBoss() const { return loai == LoaiKeDich::TUONG_HOANG_THAO; }

    sf::FloatRect GetHitBox() const;
};
