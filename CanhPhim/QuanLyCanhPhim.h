
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

enum class LoaiCanhPhim {
    KHONG_CO,
    MO_DAU,              // Giới thiệu lịch sử năm 938
    QUAN_DICH_XUAT_HIEN, // Tàu chiến Hoằng Thao tiến vào cửa sông
    THUY_TRIEU_RUT,      // Triều rút nhanh, bãi cọc nhô lên đâm vỡ mạn thuyền
    TONG_PHAN_CONG,      // Trống trận thúc giục, quân Đại Việt mai phục đổ ra
    CHIEN_THANG_LICH_SU  // Khải hoàn môn, ý nghĩa lịch sử Bạch Đằng 938
};

class QuanLyCanhPhim {
private:
    LoaiCanhPhim canhHienTai;
    float thoiGianCanh;
    float thoiGianToiDa;
    bool dangChieuPhim;

    // Hiệu ứng Rung chấn màn hình (Screen Shake)
    float doRung;
    float thoiGianRung;
    sf::Vector2f doLechCamera;

    // Phụ đề điện ảnh (Cinematic Subtitles)
    std::string loiDan;
    std::string tieuDeCanh;
    float chieuCaoDaiDen; // Letterbox black bars

public:
    QuanLyCanhPhim();
    ~QuanLyCanhPhim() = default;

    void BatDauCanh(LoaiCanhPhim loai, float thoiLuong = 5.0f);
    // Cảnh dải đen điện ảnh với tiêu đề và lời dẫn tự đặt (dùng cho các chương)
    void BatDauCanhTuyChinh(const std::string& tieuDe, const std::string& loiDanMoi, float thoiLuong);
    void KetThucCanh();

    void KichHoatRungManHinh(float cuongDo = 12.f, float thoiGian = 0.5f);
    void CapNhat(float deltaTime);

    void VeHieuUngDienAnh(sf::RenderWindow& window, const sf::Font& font);

    bool IsDangChieuPhim() const { return dangChieuPhim; }
    LoaiCanhPhim GetCanhHienTai() const { return canhHienTai; }
    sf::Vector2f GetDoLechCamera() const { return doLechCamera; }
};


