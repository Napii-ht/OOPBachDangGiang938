#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "ManChoi.h"

// Thông tin mô tả một chương, dùng cho màn hình chọn chương
struct ThongTinChuong {
    int ma;               // 1 đến 6 cho các chương chính, 0 cho màn thử nghiệm
    std::string ten;
    std::string bangMap;  // Mô tả ngắn bản đồ của chương
    std::string moTa;     // Cách chơi và nhiệm vụ
    bool daCoSan;         // false nếu chương chưa được làm
    std::function<std::unique_ptr<ManChoi>(NguCanhGame&)> taoManChoi;
};

// Quản lý danh sách chương, tạo màn, chuyển cảnh mờ dần
class QuanLyManChoi {
private:
    std::vector<ThongTinChuong> danhSachChuong;
    std::unique_ptr<ManChoi> manHienTai;
    int chiSoHienTai;

    float doToi;                  // 0 = sáng hẳn, 1 = đen hẳn
    bool dangTatDan;              // Đang mờ dần để thoát màn
    bool daDongXong;              // Đã thoát màn xong, chờ QuanLyGame đọc kết quả
    KetQuaManChoi ketQuaCuoi;
    int maChuongCuoi;

public:
    QuanLyManChoi();

    void DangKyChuong(const ThongTinChuong& chuong);
    const std::vector<ThongTinChuong>& GetDanhSachChuong() const { return danhSachChuong; }

    // Bắt đầu chơi chương ở vị trí chiSo trong danh sách
    bool BatDauChuong(int chiSo, NguCanhGame& nguCanh);

    // Thoát màn đang chơi (mờ dần rồi hủy)
    void YeuCauThoat();

    bool DangChoi() const { return manHienTai != nullptr; }
    bool DangChuyenCanh() const { return dangTatDan; }

    void XuLySuKien(const sf::Event& suKien);
    void CapNhat(float deltaTime);
    void Ve(sf::RenderWindow& window);

    // Trả về true một lần duy nhất sau khi màn vừa kết thúc
    bool LayKetQuaKhiDong(KetQuaManChoi& ketQua, int& maChuong);
};
