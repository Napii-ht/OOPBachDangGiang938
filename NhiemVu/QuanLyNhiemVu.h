
#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "NhiemVu.h"

class QuanLyNhiemVu {
private:
    std::vector<std::unique_ptr<NhiemVu>> danhSachNhiemVu;
    int chiSoNhiemVuHienTai;
    float thoiGianThongBao;
    std::string thongBaoMoi;
    std::string tieuDeBang;

public:
    QuanLyNhiemVu();
    ~QuanLyNhiemVu() = default;

    void KhoiTaoChuoiNhiemVu();   // Chuỗi 5 nhiệm vụ của màn thử nghiệm (bản cũ)

    // Dùng cho các chương mới: tự dựng chuỗi nhiệm vụ riêng
    void XoaToanBo();
    void ThemNhiemVu(std::unique_ptr<NhiemVu> nhiemVu);
    void BatDau();                                  // Kích hoạt nhiệm vụ đầu tiên
    void DatTieuDeBang(const std::string& tieuDe) { tieuDeBang = tieuDe; }
    int GetMaNhiemVuHienTai() const;                // -1 nếu đã xong hết
    int GetSoNhiemVu() const { return static_cast<int>(danhSachNhiemVu.size()); }
    void CapNhat(float deltaTime);

    NhiemVu* GetNhiemVuHienTai();
    const NhiemVu* GetNhiemVuHienTai() const;
    int GetChiSoNhiemVuHienTai() const { return chiSoNhiemVuHienTai; }

    bool HoanThanhNhiemVuHienTai();
    void CapNhatTienDoNhiemVu(int luong = 1);
    bool DaHoanThanhToanBo() const;

    // Hiển thị khung Quest Tracker phong cách Terraria ở góc trên bên phải
    void Ve(sf::RenderWindow& window, const sf::Font& font, sf::Vector2f viTriGoc = {560.f, 20.f});
};


