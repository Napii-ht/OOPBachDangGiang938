#include "KeDich.h"
#include "../BanDo/DoHoa.h"
#include <cmath>
#include <algorithm>

using namespace dohoa;

KeDich::KeDich(sf::Vector2f viTriBanDau, float tocDo, int mau, int dame,
               float tamDanh, float tamPhatHien, LoaiKeDich loaiDich)
    : NhanVat(viTriBanDau, tocDo, mau, dame),
      loai(loaiDich),
      tamDanh(tamDanh),
      tamPhatHien(tamPhatHien),
      trangThaiHienTai(TrangThaiKeDich::DUNG_YEN),
      thoiGianHoiChieu(loaiDich == LoaiKeDich::TUONG_HOANG_THAO ? 0.75f : 1.1f),
      thoiGianTroiQua(1.0f),
      huongNhin(-1),
      thoiGianHoatAnh(0.f),
      nhipBuocChan(0.f),
      thoiGianBiThuong(0.f) {}

bool KeDich::PhatHienNguoiChoi(sf::Vector2f viTriPlayer) const {
    if (daHySinh) return false;
    float dx = viTriPlayer.x - viTri.x;
    float dy = viTriPlayer.y - viTri.y;
    return (dx * dx + dy * dy) <= (tamPhatHien * tamPhatHien);
}

void KeDich::DuoiTheo(sf::Vector2f viTriPlayer, float deltaTime) {
    if (daHySinh) return;

    float dx = viTriPlayer.x - viTri.x;
    float dy = viTriPlayer.y - viTri.y;
    float khoangCach = std::sqrt(dx * dx + dy * dy);

    huongNhin = (dx >= 0) ? 1 : -1;

    if (khoangCach <= tamDanh) {
        trangThaiHienTai = TrangThaiKeDich::TAN_CONG;
        return;
    }

    if (khoangCach > 0.0001f) {
        sf::Vector2f huong(dx / khoangCach, dy / khoangCach);
        DiChuyen(huong, deltaTime);
        trangThaiHienTai = TrangThaiKeDich::DUOI_THEO;
    }
}

bool KeDich::CoTheTanCong(sf::Vector2f viTriPlayer) const {
    if (daHySinh) return false;
    float dx = viTriPlayer.x - viTri.x;
    float dy = viTriPlayer.y - viTri.y;
    float khoangCach = std::sqrt(dx * dx + dy * dy);
    return (khoangCach <= tamDanh) && (thoiGianTroiQua >= thoiGianHoiChieu);
}

bool KeDich::TanCong(NhanVat& mucTieu) {
    if (daHySinh) return false;

    if (thoiGianTroiQua >= thoiGianHoiChieu) {
        mucTieu.NhanSatThuong(satThuong);
        thoiGianTroiQua = 0.0f;
        trangThaiHienTai = TrangThaiKeDich::TAN_CONG;
        return true;
    }
    return false;
}

void KeDich::NhanSatThuong(int damage) {
    NhanVat::NhanSatThuong(damage);
    thoiGianBiThuong = 0.18f; // Nháy trắng khi trúng đòn
    if (sucKhoe <= 0) {
        Chet();
    } else {
        trangThaiHienTai = TrangThaiKeDich::BI_THUONG;
    }
}

void KeDich::Chet() {
    HySinh();
    trangThaiHienTai = TrangThaiKeDich::DA_CHET;
}

sf::FloatRect KeDich::GetHitBox() const {
    float rong = (loai == LoaiKeDich::TUONG_HOANG_THAO) ? 36.f : 26.f;
    float cao = (loai == LoaiKeDich::TUONG_HOANG_THAO) ? 44.f : 32.f;
    return sf::FloatRect({viTri.x - rong * 0.5f, viTri.y - cao * 0.5f}, {rong, cao});
}

void KeDich::CapNhat(float deltaTime) {
    if (daHySinh) return;

    thoiGianTroiQua += deltaTime;
    thoiGianHoatAnh += deltaTime;

    if (thoiGianBiThuong > 0.f) {
        thoiGianBiThuong -= deltaTime;
    }

    if (trangThaiHienTai == TrangThaiKeDich::DUOI_THEO) {
        nhipBuocChan += deltaTime * 14.f;
    } else {
        nhipBuocChan *= 0.85f;
    }
}

void KeDich::Ve(sf::RenderWindow& window) {
    if (daHySinh) return;

    mangDinh.clear();

    const float x = viTri.x;
    const float y = viTri.y;
    const float s = static_cast<float>(huongNhin);
    const float t = thoiGianHoatAnh;
    const bool isBoss = (loai == LoaiKeDich::TUONG_HOANG_THAO);
    const float kScale = isBoss ? 1.35f : 1.0f;
    const float flash = (thoiGianBiThuong > 0.f) ? 0.7f : 0.f;

    auto C = [&](Color col) -> Color {
        return sang(col, flash);
    };

    // 1. BÓNG ĐỔ DƯỚI CHÂN
    elip(mangDinh, x, y + 16.f * kScale, 15.f * kScale, 5.5f * kScale, mau(0, 0, 0, 80.f), 12);

    float doNhun = (trangThaiHienTai == TrangThaiKeDich::DUOI_THEO) ? std::abs(std::sin(nhipBuocChan)) * 2.5f : 0.f;
    float gocChan = std::sin(nhipBuocChan) * 12.f;

    if (isBoss) {
        // =========================================================================
        //  VẠN VƯƠNG LƯU HOẰNG THAO - CHỦ TƯỚNG NAM HÁN
        // =========================================================================
        Color kAoChoang = C(mau(185, 25, 25));
        Color kAoChoangToi = C(mau(110, 15, 15));
        Color kGiapSat = C(mau(55, 45, 52));
        Color kVangDong = C(mau(218, 165, 32));
        Color kVangSang = C(mau(255, 220, 80));
        Color kDa = C(mau(235, 195, 160));

        // Áo choàng đỏ viền vàng phía sau tung bay dữ dội
        float gio = std::sin(t * 3.f) * 5.f;
        tu(mangDinh,
           {x - 14.f, y - 12.f}, {x + 14.f, y - 12.f},
           {x + 20.f + gio, y + 24.f}, {x - 20.f + gio, y + 24.f},
           kAoChoang, kAoChoang, kAoChoangToi, kAoChoangToi);

        // Đôi chân giáp sắt bọc đồng bước tới
        chuNhatDoc(mangDinh, x - 9.f * s - gocChan * 0.3f, y + 10.f - doNhun, 7.f, 10.f, kGiapSat, mau(30, 25, 28));
        chuNhatDoc(mangDinh, x + 3.f * s + gocChan * 0.3f, y + 10.f - doNhun, 7.f, 10.f, kGiapSat, kVangDong);

        // Thân giáp vảy rồng đen - vàng uy lực
        float yThan = y - 12.f - doNhun;
        tu(mangDinh,
           {x - 13.f * s, yThan}, {x + 13.f * s, yThan},
           {x + 15.f * s, yThan + 22.f}, {x - 15.f * s, yThan + 22.f},
           kGiapSat, kGiapSat, mau(35, 28, 32), mau(35, 28, 32));

        // Tấm gương hộ tâm mặt quỷ nanh sư tử bằng vàng
        elip(mangDinh, x + 1.f * s, yThan + 9.f, 8.f, 8.f, kVangDong, 10);
        elip(mangDinh, x + 2.f * s, yThan + 8.f, 4.f, 4.f, kVangSang, 8);

        // Giáp vai đại bàng / phượng hoàng mạ vàng chìa rộng
        elip(mangDinh, x - 15.f * s, yThan - 2.f, 7.f, 6.f, kVangDong, 8);
        elip(mangDinh, x + 15.f * s, yThan - 2.f, 7.f, 6.f, kVangDong, 8);

        // Đầu & Mũ trụ đại tướng Nam Hán
        float yDau = yThan - 14.f;
        elip(mangDinh, x, yDau, 10.5f, 11.5f, kDa, 14);

        // Mắt trợn trừng hung tợn
        float matX = x + 4.5f * s;
        elip(mangDinh, matX, yDau - 1.f, 2.2f, 2.2f, mau(200, 20, 20), 6); // Tròng đỏ giận dữ
        elip(mangDinh, matX, yDau - 1.f, 1.2f, 1.2f, mau(20, 20, 20), 6);
        // Râu ria quỷ quyệt
        tam(mangDinh, {x - 4.f, yDau + 6.f}, {x + 4.f, yDau + 6.f}, {x, yDau + 16.f}, mau(20, 16, 14));

        // Mũ chiến đại tướng thời Ngũ Đại: chóp nhọn, 2 cánh phượng vút cao
        tu(mangDinh,
           {x - 13.f * s, yDau - 6.f}, {x + 13.f * s, yDau - 6.f},
           {x + 11.f * s, yDau - 14.f}, {x - 11.f * s, yDau - 14.f},
           kVangDong, kVangDong, mau(150, 100, 25), mau(150, 100, 25));
        tam(mangDinh, {x - 6.f, yDau - 14.f}, {x + 6.f, yDau - 14.f}, {x, yDau - 24.f}, kVangSang);
        // Chùm lông đỏ rực đỉnh mũ bay phất phới
        duong(mangDinh, {x, yDau - 23.f}, {x - 8.f * s, yDau - 32.f}, 3.5f, mau(220, 30, 30));

        // ĐẠI ĐAO / QUAN ĐAO TRÊN TAY
        float xTay = x + 14.f * s;
        float yTay = yThan + 5.f;
        Vector2f pChuoi{xTay, yTay};
        Vector2f pMui{xTay + 34.f * s, yTay - 16.f};
        duong(mangDinh, {xTay - 18.f * s, yTay + 20.f}, pMui, 4.2f, mau(120, 80, 45)); // Cán đao
        // Lưỡi đại đao khổng lồ sáng loáng
        tu(mangDinh,
           pMui, {pMui.x - 14.f * s, pMui.y - 12.f},
           {pMui.x + 8.f * s, pMui.y - 14.f}, {pMui.x + 16.f * s, pMui.y - 2.f},
           mau(245, 248, 255), mau(200, 215, 230), mau(160, 175, 195), mau(160, 175, 195));

        // Đao khí màu đỏ thẫm rực lửa khi tấn công
        if (trangThaiHienTai == TrangThaiKeDich::TAN_CONG) {
            float rAura = 56.f;
            for (int i = 0; i < 8; ++i) {
                float a = -1.2f + static_cast<float>(i) * 0.3f;
                Vector2f pWave{x + std::cos(a) * rAura * s, yThan + std::sin(a) * rAura};
                elip(mangDinh, pWave.x, pWave.y, 7.f, 5.f, mau(255, 60, 30, 200), 6);
            }
        }

    } else {
        // =========================================================================
        //  LÍNH BỘ BINH NAM HÁN (NGŨ ĐẠI THẬP QUỐC)
        // =========================================================================
        Color kGiapSat = C(mau(60, 65, 72));
        Color kChiDo = C(mau(180, 35, 35));
        Color kDa = C(mau(235, 195, 160));

        // Hai chân chạy khom
        chuNhatDoc(mangDinh, x - 6.f * s - gocChan * 0.25f, y + 8.f - doNhun, 5.f, 8.f, kGiapSat, mau(35, 38, 44));
        chuNhatDoc(mangDinh, x + 2.f * s + gocChan * 0.25f, y + 8.f - doNhun, 5.f, 8.f, kGiapSat, mau(35, 38, 44));

        // Thân áo giáp vảy cá đen viền đỏ
        float yThan = y - 9.f - doNhun;
        tu(mangDinh,
           {x - 9.f * s, yThan}, {x + 9.f * s, yThan},
           {x + 10.f * s, yThan + 17.f}, {x - 10.f * s, yThan + 17.f},
           kGiapSat, kGiapSat, mau(40, 42, 48), mau(40, 42, 48));
        // Đai lưng đỏ
        chuNhat(mangDinh, x - 9.f * s, yThan + 11.f, 18.f * s, 3.f, kChiDo);

        // Đầu & Nón chiến sắt chóp nhọn có rèm che gáy
        float yDau = yThan - 10.f;
        elip(mangDinh, x, yDau, 8.f, 8.5f, kDa, 12);

        // Mắt hung hãn nhìn mục tiêu
        float matX = x + 3.5f * s;
        elip(mangDinh, matX, yDau - 1.f, 1.8f, 1.8f, mau(20, 20, 20), 6);

        // Mũ sắt chóp nhọn thời Nam Hán
        tam(mangDinh, {x - 9.5f * s, yDau - 4.f}, {x + 9.5f * s, yDau - 4.f}, {x, yDau - 15.f}, kGiapSat);
        // Rèm giáp hộ cảnh che sau gáy
        tu(mangDinh,
           {x - 9.f * s, yDau - 4.f}, {x - 4.f * s, yDau - 4.f},
           {x - 5.f * s, yDau + 4.f}, {x - 11.f * s, yDau + 3.f},
           kChiDo, kChiDo, mau(120, 20, 20), mau(120, 20, 20));

        // Thanh liễu diệp đao cong cầm trên tay
        float xTay = x + 9.f * s;
        float yTay = yThan + 4.f;
        Vector2f pC{xTay, yTay};
        Vector2f pM{xTay + 20.f * s, yTay - 6.f};
        duong(mangDinh, pC, pM, 3.2f, mau(220, 230, 240));
        // Vát cong mũi đao
        tam(mangDinh, pM, {pM.x - 4.f * s, pM.y - 6.f}, {pM.x + 4.f * s, pM.y - 2.f}, mau(240, 245, 255));
    }

    // THANH MÁU MINI TRÊN ĐẦU
    float rongThanh = isBoss ? 52.f : 34.f;
    float caoThanh = isBoss ? 6.f : 4.5f;
    float yThanhMau = y - 32.f * kScale;
    float tiLeMau = std::clamp(static_cast<float>(sucKhoe) / static_cast<float>(sucKhoeToiDa), 0.0f, 1.0f);

    chuNhat(mangDinh, x - rongThanh * 0.5f - 1.5f, yThanhMau - 1.5f, rongThanh + 3.f, caoThanh + 3.f,
            isBoss ? mau(218, 165, 32, 240) : mau(30, 30, 36, 220));
    chuNhat(mangDinh, x - rongThanh * 0.5f, yThanhMau, rongThanh, caoThanh, mau(45, 20, 20, 200));
    if (tiLeMau > 0.01f) {
        chuNhat(mangDinh, x - rongThanh * 0.5f, yThanhMau, rongThanh * tiLeMau, caoThanh,
                isBoss ? mau(255, 120, 20) : mau(220, 25, 35));
        chuNhat(mangDinh, x - rongThanh * 0.5f, yThanhMau, rongThanh * tiLeMau, 1.5f, mau(255, 200, 160));
    }

    window.draw(mangDinh);
}
