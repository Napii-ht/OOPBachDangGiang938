#pragma once

#include "../NhanVat/KeDich.h"
#include "HeThongAI.h"
#include <vector>
#include <unordered_set>
#include <memory>
#include <SFML/Graphics.hpp>

// Quản lý chiến trường và trận đánh cuối trên sông Bạch Đằng
class HeThongChienDau {
private:
    std::vector<std::unique_ptr<KeDich>> danhSachKeDich; // Danh sách toàn bộ quân giặc
    HeThongAI heThongAI;                                 // Bộ não AI để điều khiển lính
    int soKeDichConLai;                                  // Số giặc còn sống
    bool daKhoiTaoTranChien;

    // Danh sách lưu các con lính đã bị chém trúng trong nhát kiếm hiện tại
    // Tránh bug kinh điển: 1 nhát chém tồn tại 0.25s (15 frame) làm giặc bị trừ máu 15 lần
    std::unordered_set<const KeDich*> danhSachDaBiChemTrongNhatNay;

public:
    HeThongChienDau();
    ~HeThongChienDau() = default;

    // Thả 1 con lính ra tọa độ chỉ định
    void ThemKeDich(sf::Vector2f viTri, float tocDo = 100.f, int mau = 80, int dame = 10);

    // Kích hoạt trận đánh cuối: sinh ra 1 bầy lính Nam Hán dàn trận
    void TaoTranChienCuoi(int soLuongDich = 5);

    // Vòng lặp game gọi hàm này mỗi frame để cập nhật toàn bộ quân địch
    void CapNhat(float deltaTime, NhanVat& player);
    void Ve(sf::RenderWindow& window);

    // Xử lý khi player vung kiếm chém
    // Chỉ trừ đúng 1 lần dame cho mỗi con lính trong 1 nhát chém
    int XuLyPlayerTanCong(const sf::FloatRect& vungTanCong, int satThuong, bool playerDangChem);

    // Thắng khi hết sạch giặc (soKeDichConLai == 0)
    int GetSoKeDichConLai() const { return soKeDichConLai; }
    bool KiemTraChienThang() const;

    // Reset làm trận mới
    void XoaToanBo();
    const std::vector<std::unique_ptr<KeDich>>& GetDanhSachKeDich() const { return danhSachKeDich; }
};
