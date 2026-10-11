
#include "QuanLyNhiemVu.h"
#include "../CotLoi/TienIch.h"

QuanLyNhiemVu::QuanLyNhiemVu()
    : chiSoNhiemVuHienTai(0),
      thoiGianThongBao(0.f),
      thongBaoMoi(""),
      tieuDeBang("MỤC TIÊU CHIẾN DỊCH") {
    KhoiTaoChuoiNhiemVu();
}

void QuanLyNhiemVu::XoaToanBo() {
    danhSachNhiemVu.clear();
    chiSoNhiemVuHienTai = 0;
    thoiGianThongBao = 0.f;
    thongBaoMoi.clear();
}

void QuanLyNhiemVu::ThemNhiemVu(std::unique_ptr<NhiemVu> nhiemVu) {
    danhSachNhiemVu.push_back(std::move(nhiemVu));
}

void QuanLyNhiemVu::BatDau() {
    chiSoNhiemVuHienTai = 0;
    if (!danhSachNhiemVu.empty()) danhSachNhiemVu[0]->SetDaKichHoat(true);
}

int QuanLyNhiemVu::GetMaNhiemVuHienTai() const {
    const NhiemVu* nv = GetNhiemVuHienTai();
    return nv ? nv->GetMaNhiemVu() : -1;
}

void QuanLyNhiemVu::KhoiTaoChuoiNhiemVu() {
    danhSachNhiemVu.clear();

    // 5 Nhiệm vụ cốt lõi theo dòng lịch sử Bạch Đằng 938
    danhSachNhiemVu.push_back(std::make_unique<NhiemVuKhamPha>(
        1, "Hội kiến Ngô Quyền", "Đến gặp Ngô Quyền bên bờ sông để nhận lệnh."
    ));

    danhSachNhiemVu.push_back(std::make_unique<NhiemVuThuThap>(
        2, "Thu thập 3 cọc gỗ", "Nhặt 3 cọc gỗ bịt sắt trên bờ (đến gần và nhấn [E]).", 1, 3
    ));

    danhSachNhiemVu.push_back(std::make_unique<NhiemVuThuThap>(
        3, "Hoàn thành trận địa cọc", "Mang cọc gỗ ra mép sông cắm trận địa chặn giặc (nhấn [E]).", 1, 3
    ));

    danhSachNhiemVu.push_back(std::make_unique<NhiemVuKhamPha>(
        4, "Nhử thuyền giặc Nam Hán", "Lên thuyền nhẹ, điều khiển dụ thuyền Lưu Hoằng Tháo vào bãi cọc."
    ));

    danhSachNhiemVu.push_back(std::make_unique<NhiemVuChienDau>(
        5, "Tổng phản công giặc Nam Hán", "Tiêu diệt toàn bộ quân Lưu Hoằng Tháo trên sông và bãi cọc!", 6
    ));

    chiSoNhiemVuHienTai = 0;
    if (!danhSachNhiemVu.empty()) {
        danhSachNhiemVu[0]->SetDaKichHoat(true);
    }
}

void QuanLyNhiemVu::CapNhat(float deltaTime) {
    if (thoiGianThongBao > 0.f) {
        thoiGianThongBao -= deltaTime;
    }
}

NhiemVu* QuanLyNhiemVu::GetNhiemVuHienTai() {
    if (chiSoNhiemVuHienTai >= 0 && chiSoNhiemVuHienTai < static_cast<int>(danhSachNhiemVu.size())) {
        return danhSachNhiemVu[chiSoNhiemVuHienTai].get();
    }
    return nullptr;
}

const NhiemVu* QuanLyNhiemVu::GetNhiemVuHienTai() const {
    if (chiSoNhiemVuHienTai >= 0 && chiSoNhiemVuHienTai < static_cast<int>(danhSachNhiemVu.size())) {
        return danhSachNhiemVu[chiSoNhiemVuHienTai].get();
    }
    return nullptr;
}

void QuanLyNhiemVu::CapNhatTienDoNhiemVu(int luong) {
    auto* nv = GetNhiemVuHienTai();
    if (nv && !nv->IsDaHoanThanh()) {
        nv->CapNhatTienDo(luong);
        if (nv->IsDaHoanThanh()) {
            HoanThanhNhiemVuHienTai();
        }
    }
}

bool QuanLyNhiemVu::HoanThanhNhiemVuHienTai() {
    if (chiSoNhiemVuHienTai < static_cast<int>(danhSachNhiemVu.size())) {
        danhSachNhiemVu[chiSoNhiemVuHienTai]->KiemTraHoanThanh();
        thongBaoMoi = "HOÀN THÀNH: " + danhSachNhiemVu[chiSoNhiemVuHienTai]->GetTenNhiemVu();
        thoiGianThongBao = 4.0f;

        chiSoNhiemVuHienTai++;
        if (chiSoNhiemVuHienTai < static_cast<int>(danhSachNhiemVu.size())) {
            danhSachNhiemVu[chiSoNhiemVuHienTai]->SetDaKichHoat(true);
        }
        return true;
    }
    return false;
}

bool QuanLyNhiemVu::DaHoanThanhToanBo() const {
    return chiSoNhiemVuHienTai >= static_cast<int>(danhSachNhiemVu.size());
}

void QuanLyNhiemVu::Ve(sf::RenderWindow& window, const sf::Font& font, sf::Vector2f viTriGoc) {
    const float chieuRongHop = 225.f;

    // Mô tả của nhiệm vụ đang làm, tự xuống dòng cho vừa khung
    std::string moTaHienTai;
    if (const NhiemVu* nv = GetNhiemVuHienTai()) moTaHienTai = NgatDong(font, nv->GetMoTa(), 10, chieuRongHop - 24.f);
    int soDongMoTa = moTaHienTai.empty() ? 0 : 1;
    for (char c : moTaHienTai) if (c == '\n') soDongMoTa++;

    // Khung cao theo số nhiệm vụ để chương nào cũng vừa
    const float chieuCaoHop = 40.f + 18.f * static_cast<float>(danhSachNhiemVu.size()) +
                              (soDongMoTa > 0 ? 10.f + 13.f * static_cast<float>(soDongMoTa) : 0.f);

    // Bảng nhiệm vụ gỗ/kim loại phong cách Terraria
    sf::RectangleShape nenHop({chieuRongHop, chieuCaoHop});
    nenHop.setPosition(viTriGoc);
    nenHop.setFillColor(sf::Color(25, 30, 42, 210));
    nenHop.setOutlineThickness(2.f);
    nenHop.setOutlineColor(sf::Color(190, 150, 80));
    window.draw(nenHop);

    // Tiêu đề bảng nhiệm vụ
    sf::Text tieuDe(font, VanBan(tieuDeBang), 13);
    tieuDe.setFillColor(sf::Color(255, 215, 0));
    tieuDe.setPosition({viTriGoc.x + 12.f, viTriGoc.y + 8.f});
    window.draw(tieuDe);

    sf::RectangleShape duongKe({chieuRongHop - 24.f, 1.5f});
    duongKe.setPosition({viTriGoc.x + 12.f, viTriGoc.y + 26.f});
    duongKe.setFillColor(sf::Color(120, 100, 60));
    window.draw(duongKe);

    // Hiển thị danh sách các bước nhiệm vụ
    float yOffset = viTriGoc.y + 32.f;
    for (size_t i = 0; i < danhSachNhiemVu.size(); ++i) {
        const auto& nv = danhSachNhiemVu[i];
        std::string dongChu;
        sf::Color mauChu;

        if (nv->IsDaHoanThanh()) {
            dongChu = "[v] " + nv->GetTenNhiemVu();
            mauChu = sf::Color(100, 200, 100, 180);
        } else if (static_cast<int>(i) == chiSoNhiemVuHienTai) {
            dongChu = "[>] " + nv->GetTenNhiemVu() + " (" + nv->GetTienDoChu() + ")";
            mauChu = sf::Color(255, 230, 100);
        } else {
            dongChu = "[ ] " + nv->GetTenNhiemVu();
            mauChu = sf::Color(140, 145, 155, 140);
        }

        sf::Text textDong(font, VanBan(dongChu), 11);
        textDong.setFillColor(mauChu);
        textDong.setPosition({viTriGoc.x + 10.f, yOffset});
        window.draw(textDong);

        yOffset += 18.f;
    }

    // Mô tả nhiệm vụ hiện tại ở cuối bảng
    if (soDongMoTa > 0) {
        sf::RectangleShape duongKe2({chieuRongHop - 24.f, 1.f});
        duongKe2.setPosition({viTriGoc.x + 12.f, yOffset + 3.f});
        duongKe2.setFillColor(sf::Color(120, 100, 60, 160));
        window.draw(duongKe2);

        sf::Text textMoTa(font, VanBan(moTaHienTai), 10);
        textMoTa.setFillColor(sf::Color(200, 210, 225));
        textMoTa.setPosition({viTriGoc.x + 12.f, yOffset + 9.f});
        window.draw(textMoTa);
    }

    // Banner thông báo hoàn thành nổi bật
    if (thoiGianThongBao > 0.f) {
        sf::RectangleShape banner({300.f, 32.f});
        banner.setOrigin({150.f, 16.f});
        banner.setPosition({390.f, 150.f});
        banner.setFillColor(sf::Color(40, 120, 40, 220));
        banner.setOutlineThickness(2.f);
        banner.setOutlineColor(sf::Color::Yellow);
        window.draw(banner);

        sf::Text tbText(font, VanBan(thongBaoMoi), 13);
        tbText.setFillColor(sf::Color::White);
        tbText.setOrigin({tbText.getLocalBounds().size.x * 0.5f, tbText.getLocalBounds().size.y * 0.5f});
        tbText.setPosition({390.f, 147.f});
        window.draw(tbText);
    }
}


