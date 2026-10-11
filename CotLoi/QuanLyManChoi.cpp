#include "QuanLyManChoi.h"
#include "TienIch.h"
#include <algorithm>

QuanLyManChoi::QuanLyManChoi()
    : chiSoHienTai(-1),
      doToi(0.f),
      dangTatDan(false),
      daDongXong(false),
      ketQuaCuoi(KetQuaManChoi::THOAT),
      maChuongCuoi(0) {}

void QuanLyManChoi::DangKyChuong(const ThongTinChuong& chuong) {
    danhSachChuong.push_back(chuong);
}

bool QuanLyManChoi::BatDauChuong(int chiSo, NguCanhGame& nguCanh) {
    if (chiSo < 0 || chiSo >= static_cast<int>(danhSachChuong.size())) return false;

    const ThongTinChuong& chuong = danhSachChuong[chiSo];
    if (!chuong.daCoSan || !chuong.taoManChoi) return false;

    manHienTai = chuong.taoManChoi(nguCanh);
    chiSoHienTai = chiSo;
    doToi = 1.f; // Bắt đầu từ màn đen rồi sáng dần
    dangTatDan = false;
    daDongXong = false;
    return manHienTai != nullptr;
}

void QuanLyManChoi::YeuCauThoat() {
    if (!manHienTai || dangTatDan) return;
    dangTatDan = true;
    ketQuaCuoi = manHienTai->GetKetQua();
    if (ketQuaCuoi == KetQuaManChoi::DANG_CHOI) ketQuaCuoi = KetQuaManChoi::THOAT;
    maChuongCuoi = danhSachChuong[chiSoHienTai].ma;
}

void QuanLyManChoi::XuLySuKien(const sf::Event& suKien) {
    // Đang mờ dần thì không nhận phím nữa
    if (manHienTai && !dangTatDan) manHienTai->XuLySuKien(suKien);
}

void QuanLyManChoi::CapNhat(float deltaTime) {
    if (!manHienTai) return;

    if (dangTatDan) {
        doToi += deltaTime / 0.5f;
        if (doToi >= 1.f) {
            doToi = 1.f;
            manHienTai.reset();
            chiSoHienTai = -1;
            dangTatDan = false;
            daDongXong = true;
        }
        return;
    }

    // Sáng dần khi vào màn
    if (doToi > 0.f) doToi = std::max(0.f, doToi - deltaTime / 0.6f);

    manHienTai->CapNhat(deltaTime);

    // Màn báo kết thúc thì bắt đầu mờ dần để thoát
    // Chờ một chút cho màn chiến thắng, thua hiện ra bằng cách để màn tự gọi ketQua sau khi người chơi bấm phím
    if (manHienTai->GetKetQua() != KetQuaManChoi::DANG_CHOI) {
        YeuCauThoat();
    }
}

void QuanLyManChoi::Ve(sf::RenderWindow& window) {
    if (manHienTai) manHienTai->Ve();

    if (doToi > 0.f) {
        ApDungViewLogic(window);
        sf::RectangleShape man({RONG_MAN_HINH, CAO_MAN_HINH});
        man.setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(255.f * std::min(1.f, doToi))));
        window.draw(man);
    }
}

bool QuanLyManChoi::LayKetQuaKhiDong(KetQuaManChoi& ketQua, int& maChuong) {
    if (!daDongXong) return false;
    daDongXong = false;
    ketQua = ketQuaCuoi;
    maChuong = maChuongCuoi;
    return true;
}
