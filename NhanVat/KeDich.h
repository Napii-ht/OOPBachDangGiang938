#pragma once

#include "NhanVat.h"
#include <SFML/Graphics.hpp>

// Các trạng thái hành động của lính giặc
enum class TrangThaiKeDich {
    DUNG_YEN,    // Chưa thấy ai thì đứng
    DUOI_THEO,   // Thấy player rồi thì lao tới
    TAN_CONG,    // Áp sát được thì đứng lại chém
    BI_THUONG,   // Bị ăn đòn
    DA_CHET      // Hết máu thì nằm xuống
};

// Class quân lính Nam Hán
class KeDich : public NhanVat {
private:
    float tamDanh;                 // Đứng cách player bao xa thì vung kiếm trúng
    float tamPhatHien;             // Cách bao xa thì phát hiện ra player để rượt
    TrangThaiKeDich trangThaiHienTai;

    // Tránh việc lính spam chém liên tục mỗi frame (cần có thời gian hồi chiêu)
    float thoiGianHoiChieu;        // Nghỉ 1 giây mới chém tiếp được
    float thoiGianTroiQua;         // Đếm thời gian để biết khi nào chém được tiếp

    // Hình ảnh tạm thời (vẽ hình tròn màu đỏ để test vì chưa có sprite thật)
    sf::CircleShape hinhDang;
    sf::RectangleShape thanhMauNen;
    sf::RectangleShape thanhMauHienTai;

public:
    KeDich(sf::Vector2f viTriBanDau, float tocDo = 120.f, int mau = 100, int dame = 15, float tamDanh = 40.f, float tamPhatHien = 250.f);
    virtual ~KeDich() = default;

    // Ghi đè lại hàm của class cha NhanVat
    void CapNhat(float deltaTime) override;
    void Ve(sf::RenderWindow& window) override;
    void NhanSatThuong(int damage) override;

    // Các hàm xử lý hành động
    bool PhatHienNguoiChoi(sf::Vector2f viTriPlayer) const;
    void DuoiTheo(sf::Vector2f viTriPlayer, float deltaTime);
    bool CoTheTanCong(sf::Vector2f viTriPlayer) const;
    bool TanCong(NhanVat& mucTieu);
    void Chet();

    // Lấy thông tin
    float GetTamDanh() const { return tamDanh; }
    float GetTamPhatHien() const { return tamPhatHien; }
    TrangThaiKeDich GetTrangThai() const { return trangThaiHienTai; }
    void SetTrangThai(TrangThaiKeDich st) { trangThaiHienTai = st; }

    // Lấy khung va chạm để tính chém trúng
    sf::FloatRect GetHitBox() const {
        return hinhDang.getGlobalBounds();
    }
};
