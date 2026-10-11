
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../NhanVat/NguoiChoi.h"
#include "../NhanVat/NhanVatPhu.h"
#include "../BanDo/BanDo.h"
#include "../BanDo/Thuyen.h"
#include "../ChienDau/HeThongChienDau.h"
#include "../NhiemVu/QuanLyNhiemVu.h"
#include "../AmThanh/QuanLyAmThanh.h"
#include "../CanhPhim/QuanLyCanhPhim.h"
#include "../GiaoDien/QuanLyGiaoDien.h"
#include "../CotLoi/ManChoi.h"
#include "../CotLoi/TrangThaiTranDau.h"

// Trạng thái bên trong màn thử nghiệm này
enum class TrangThaiMan {
    DANG_CHOI,
    CHIEN_THANG,
    THUA_TRAN
};

// Màn thử nghiệm: toàn bộ trận Bạch Đằng của bản cũ gói vào một ManChoi.
// Giữ lại để nhóm vẫn demo được trong lúc các chương mới (1 đến 6) đang được làm.
class ManBachDangCu : public ManChoi {
private:
    sf::RenderWindow& window;
    TrangThaiMan trangThaiMan;
    TrangThaiTranDau trangThaiTranDau;

    // Các thực thể và hệ thống cốt lõi
    std::unique_ptr<NguoiChoi> nguoiChoi;
    std::unique_ptr<ThuyenNguoiChoi> thuyenNguoiChoi;
    bool dangLaiThuyen;

    std::vector<std::unique_ptr<NhanVatPhu>> danhSachNPC;
    BanDo banDo;
    HeThongChienDau heThongChienDau;
    QuanLyNhiemVu quanLyNhiemVu;
    QuanLyCanhPhim quanLyCanhPhim;
    QuanLyGiaoDien quanLyGiaoDien;   // Hộp thoại, hạt hiệu ứng, số sát thương của màn này

    float thoiGianTrangThai;
    int soCocDaCam;
    bool daThongBaoChienThang;

    void KhoiTaoGameMoi();
    void XuLySuKienDoiThoai();
    void XuLyNhatVaCamCoc();
    void KiemTraChuyenGiaiDoanTranDau(float deltaTime);

public:
    explicit ManBachDangCu(NguCanhGame& ngu);
    ~ManBachDangCu() override = default;

    void XuLySuKien(const sf::Event& event) override;
    void CapNhat(float deltaTime) override;
    void Ve() override;

    TrangThaiMan GetTrangThaiMan() const { return trangThaiMan; }
    TrangThaiTranDau GetTrangThaiTranDau() const { return trangThaiTranDau; }
};


