
#include "QuanLyCanhPhim.h"
#include "../CotLoi/TienIch.h"
#include <cstdlib>
#include <algorithm>

QuanLyCanhPhim::QuanLyCanhPhim()
    : canhHienTai(LoaiCanhPhim::KHONG_CO),
      thoiGianCanh(0.f),
      thoiGianToiDa(5.f),
      dangChieuPhim(false),
      doRung(0.f),
      thoiGianRung(0.f),
      doLechCamera(0.f, 0.f),
      loiDan(""),
      tieuDeCanh(""),
      chieuCaoDaiDen(0.f) {}

void QuanLyCanhPhim::BatDauCanh(LoaiCanhPhim loai, float thoiLuong) {
    canhHienTai = loai;
    thoiGianCanh = 0.f;
    thoiGianToiDa = thoiLuong;
    dangChieuPhim = true;

    if (loai == LoaiCanhPhim::MO_DAU) {
        tieuDeCanh = "NĂM 938: KHOẢNH KHẮC VÀO LỊCH SỬ";
        loiDan = "Quân Nam Hán tiến vào cửa sông Bạch Đằng. Ngô Quyền gấp rút bố trí trận địa.";
    } else if (loai == LoaiCanhPhim::QUAN_DICH_XUAT_HIEN) {
        tieuDeCanh = "CHIẾN THUYỀN NAM HÁN TIẾN VÀO";
        loiDan = "Soái hạm Lưu Hoằng Tháo xuất hiện! Hãy lái thuyền nhẹ nhử địch vào bãi cọc ngầm!";
    } else if (loai == LoaiCanhPhim::THUY_TRIEU_RUT) {
        tieuDeCanh = "THỦY TRIỀU RÚT NHANH: TÀU GIẶC MẮC CỌC!";
        loiDan = "Nước sông rút xuống! Bãi cọc nhọn đâm thủng thuyền giặc, quân địch hoảng loạn!";
        KichHoatRungManHinh(16.f, 1.8f);
    } else if (loai == LoaiCanhPhim::TONG_PHAN_CONG) {
        tieuDeCanh = "TỔNG PHẢN CÔNG: TRỐNG TRẬN NỔI LÊN!";
        loiDan = "Ngô Quyền ra lệnh tổng tiến công! Quân mai phục Đại Việt đồng loạt xông ra quyết chiến!";
        KichHoatRungManHinh(8.f, 0.8f);
    } else if (loai == LoaiCanhPhim::CHIEN_THANG_LICH_SU) {
        tieuDeCanh = "BẠCH ĐẰNG 938: CHIẾN THẮNG VANG DỘI";
        loiDan = "Một dòng sông. Một trận chiến. Một bước ngoặt lịch sử.\nChấm dứt hơn một nghìn năm Bắc thuộc, mở ra kỷ nguyên độc lập tự chủ lâu dài!";
    }
}

void QuanLyCanhPhim::BatDauCanhTuyChinh(const std::string& tieuDe, const std::string& loiDanMoi, float thoiLuong) {
    canhHienTai = LoaiCanhPhim::MO_DAU;
    thoiGianCanh = 0.f;
    thoiGianToiDa = thoiLuong;
    dangChieuPhim = true;
    tieuDeCanh = tieuDe;
    loiDan = loiDanMoi;
}

void QuanLyCanhPhim::KetThucCanh() {
    dangChieuPhim = false;
    canhHienTai = LoaiCanhPhim::KHONG_CO;
}

void QuanLyCanhPhim::KichHoatRungManHinh(float cuongDo, float thoiGian) {
    doRung = cuongDo;
    thoiGianRung = thoiGian;
}

void QuanLyCanhPhim::CapNhat(float deltaTime) {
    // Xử lý rung chấn camera
    if (thoiGianRung > 0.f) {
        thoiGianRung -= deltaTime;
        float rx = static_cast<float>((rand() % 200) - 100) / 100.f * doRung;
        float ry = static_cast<float>((rand() % 200) - 100) / 100.f * doRung;
        doLechCamera = sf::Vector2f(rx, ry);
        doRung *= 0.92f;
    } else {
        doLechCamera = sf::Vector2f(0.f, 0.f);
        doRung = 0.f;
    }

    // Tiến trình cảnh phim
    if (dangChieuPhim) {
        thoiGianCanh += deltaTime;
        if (chieuCaoDaiDen < 50.f) chieuCaoDaiDen += deltaTime * 120.f;

        if (thoiGianCanh >= thoiGianToiDa) {
            KetThucCanh();
        }
    } else {
        if (chieuCaoDaiDen > 0.f) chieuCaoDaiDen -= deltaTime * 120.f;
        if (chieuCaoDaiDen < 0.f) chieuCaoDaiDen = 0.f;
    }
}

void QuanLyCanhPhim::VeHieuUngDienAnh(sf::RenderWindow& window, const sf::Font& font) {
    if (chieuCaoDaiDen <= 0.1f && !dangChieuPhim) return;

    float w = RONG_MAN_HINH;
    float h = CAO_MAN_HINH;

    // Hai dải đen điện ảnh letterbox
    sf::RectangleShape daiTren({w, chieuCaoDaiDen});
    daiTren.setPosition({0.f, 0.f});
    daiTren.setFillColor(sf::Color::Black);
    window.draw(daiTren);

    sf::RectangleShape daiDuoi({w, chieuCaoDaiDen});
    daiDuoi.setPosition({0.f, h - chieuCaoDaiDen});
    daiDuoi.setFillColor(sf::Color::Black);
    window.draw(daiDuoi);

    if (dangChieuPhim) {
        // Banner tiêu đề sự kiện giữa dải trên
        sf::Text textTieuDe(font, VanBan(tieuDeCanh), 15);
        textTieuDe.setFillColor(sf::Color(255, 215, 0));
        textTieuDe.setOrigin({textTieuDe.getLocalBounds().size.x * 0.5f, textTieuDe.getLocalBounds().size.y * 0.5f});
        textTieuDe.setPosition({w * 0.5f, 25.f});
        window.draw(textTieuDe);

        // Lời dẫn truyện ở dải dưới
        sf::Text textDan(font, VanBan(NgatDong(font, loiDan, 13, 700.f)), 13);
        textDan.setFillColor(sf::Color::White);
        textDan.setOrigin({textDan.getLocalBounds().size.x * 0.5f, textDan.getLocalBounds().size.y * 0.5f});
        textDan.setPosition({w * 0.5f, h - 25.f});
        window.draw(textDan);
    }
}


