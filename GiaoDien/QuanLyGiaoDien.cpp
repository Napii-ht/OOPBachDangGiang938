
#include "QuanLyGiaoDien.h"
#include "../CotLoi/TienIch.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <iostream>

QuanLyGiaoDien::QuanLyGiaoDien(const sf::Font& font) : phongChu(font) {}

void QuanLyGiaoDien::CapNhat(float deltaTime) {
    hopThoai.CapNhat(deltaTime);

    // Cập nhật số sát thương bay lên và mờ dần
    for (auto it = danhSachSoSatThuong.begin(); it != danhSachSoSatThuong.end();) {
        it->thoiGianTonTai += deltaTime;
        it->viTri.y -= deltaTime * 35.f; // Bay lên trên

        if (it->thoiGianTonTai >= it->thoiGianToiDa) {
            it = danhSachSoSatThuong.erase(it);
        } else {
            ++it;
        }
    }

    // Cập nhật các hạt hiệu ứng
    for (auto it = danhSachHatHieuUng.begin(); it != danhSachHatHieuUng.end();) {
        it->thoiGianTonTai += deltaTime;
        it->viTri += it->vanToc * deltaTime;
        it->vanToc *= 0.94f; // Giảm tốc

        if (it->thoiGianTonTai >= it->thoiGianToiDa) {
            it = danhSachHatHieuUng.erase(it);
        } else {
            ++it;
        }
    }
}

void QuanLyGiaoDien::ThemSoSatThuong(sf::Vector2f viTri, int satThuong, bool chiMang) {
    SoSatThuong st;
    st.viTri = viTri + sf::Vector2f(static_cast<float>((rand() % 20) - 10), -10.f);
    st.text = "-" + std::to_string(satThuong);
    st.mau = chiMang ? sf::Color(255, 60, 40) : sf::Color(255, 230, 70);
    st.thoiGianTonTai = 0.f;
    st.thoiGianToiDa = 0.8f;
    danhSachSoSatThuong.push_back(st);
}

void QuanLyGiaoDien::ThemHieuUngChem(sf::Vector2f viTri) {
    for (int i = 0; i < 10; ++i) {
        HatHieuUng h;
        h.viTri = viTri;
        float goc = static_cast<float>(rand() % 360) * 3.14159f / 180.f;
        float tocDo = 80.f + static_cast<float>(rand() % 120);
        h.vanToc = sf::Vector2f(std::cos(goc) * tocDo, std::sin(goc) * tocDo);
        h.mau = (i % 2 == 0) ? sf::Color(255, 240, 150) : sf::Color(255, 100, 50);
        h.kichThuoc = 2.5f + static_cast<float>(rand() % 3);
        h.thoiGianTonTai = 0.f;
        h.thoiGianToiDa = 0.35f;
        danhSachHatHieuUng.push_back(h);
    }
}

void QuanLyGiaoDien::ThemHieuUngNuocBan(sf::Vector2f viTri) {
    for (int i = 0; i < 16; ++i) {
        HatHieuUng h;
        h.viTri = viTri;
        float goc = static_cast<float>(rand() % 360) * 3.14159f / 180.f;
        float tocDo = 60.f + static_cast<float>(rand() % 140);
        h.vanToc = sf::Vector2f(std::cos(goc) * tocDo, std::sin(goc) * tocDo - 50.f);
        h.mau = sf::Color(170, 230, 255, 220);
        h.kichThuoc = 3.f + static_cast<float>(rand() % 4);
        h.thoiGianTonTai = 0.f;
        h.thoiGianToiDa = 0.5f;
        danhSachHatHieuUng.push_back(h);
    }
}

void QuanLyGiaoDien::ThemHieuUngVaChamGo(sf::Vector2f viTri) {
    for (int i = 0; i < 14; ++i) {
        HatHieuUng h;
        h.viTri = viTri;
        float goc = static_cast<float>(rand() % 360) * 3.14159f / 180.f;
        float tocDo = 70.f + static_cast<float>(rand() % 110);
        h.vanToc = sf::Vector2f(std::cos(goc) * tocDo, std::sin(goc) * tocDo);
        h.mau = sf::Color(139, 85, 45); // Mảnh gỗ văng
        h.kichThuoc = 3.5f + static_cast<float>(rand() % 3);
        h.thoiGianTonTai = 0.f;
        h.thoiGianToiDa = 0.45f;
        danhSachHatHieuUng.push_back(h);
    }
}

void QuanLyGiaoDien::VeHUD(sf::RenderWindow& window, const NguoiChoi& player) {
    // 1. Khung Thanh Máu phong cách Terraria (Góc trên trái)
    const float x = 20.f;
    const float y = 16.f;
    const float chieuDaiMau = 160.f;
    const float chieuCaoMau = 18.f;

    // Khung kim loại viền vàng
    sf::RectangleShape khungMau({chieuDaiMau + 6.f, chieuCaoMau + 6.f});
    khungMau.setPosition({x - 3.f, y - 3.f});
    khungMau.setFillColor(sf::Color(25, 25, 30, 220));
    khungMau.setOutlineThickness(2.f);
    khungMau.setOutlineColor(sf::Color(218, 165, 32));
    window.draw(khungMau);

    // Vạch máu đỏ rực rỡ
    float tiLeMau = static_cast<float>(player.GetSucKhoe()) / static_cast<float>(player.GetSucKhoeToiDa());
    tiLeMau = std::clamp(tiLeMau, 0.0f, 1.0f);

    sf::RectangleShape vachMau({chieuDaiMau * tiLeMau, chieuCaoMau});
    vachMau.setPosition({x, y});
    vachMau.setFillColor(sf::Color(220, 20, 40));
    window.draw(vachMau);

    // Chữ số hiển thị máu
    std::string chuHp = "HP: " + std::to_string(player.GetSucKhoe()) + " / " + std::to_string(player.GetSucKhoeToiDa());
    sf::Text textHp(phongChu, VanBan(chuHp), 11);
    textHp.setFillColor(sf::Color::White);
    textHp.setPosition({x + 6.f, y + 2.f});
    window.draw(textHp);

    // 2. Thanh Thể lực (Stamina bar màu ngọc bích ngay dưới thanh máu)
    const float yTheLuc = y + chieuCaoMau + 6.f;
    const float chieuDaiTL = 130.f;
    const float chieuCaoTL = 10.f;

    sf::RectangleShape khungTL({chieuDaiTL + 4.f, chieuCaoTL + 4.f});
    khungTL.setPosition({x - 2.f, yTheLuc - 2.f});
    khungTL.setFillColor(sf::Color(20, 25, 25, 200));
    khungTL.setOutlineThickness(1.5f);
    khungTL.setOutlineColor(sf::Color(160, 180, 100));
    window.draw(khungTL);

    float tiLeTL = player.GetTheLuc() / player.GetTheLucToiDa();
    tiLeTL = std::clamp(tiLeTL, 0.0f, 1.0f);

    sf::RectangleShape vachTL({chieuDaiTL * tiLeTL, chieuCaoTL});
    vachTL.setPosition({x, yTheLuc});
    vachTL.setFillColor(sf::Color(46, 204, 113));
    window.draw(vachTL);

    // 3. Hotbar túi đồ
    const_cast<TuDo&>(player.GetTuDo()).VeHotbar(window, phongChu, {x, yTheLuc + 18.f});
}

void QuanLyGiaoDien::VeHieuUngVaSatThuong(sf::RenderWindow& window) {
    // Vẽ các hạt hiệu ứng
    for (const auto& h : danhSachHatHieuUng) {
        float doMo = 1.0f - (h.thoiGianTonTai / h.thoiGianToiDa);
        sf::CircleShape hat(h.kichThuoc);
        hat.setOrigin({h.kichThuoc, h.kichThuoc});
        hat.setPosition(h.viTri);
        sf::Color c = h.mau;
        c.a = static_cast<std::uint8_t>(c.a * doMo);
        hat.setFillColor(c);
        window.draw(hat);
    }

    // Vẽ số sát thương nảy lên phong cách Terraria
    for (const auto& st : danhSachSoSatThuong) {
        float doMo = 1.0f - (st.thoiGianTonTai / st.thoiGianToiDa);
        sf::Text textSt(phongChu, VanBan(st.text), 14);
        textSt.setOrigin({textSt.getLocalBounds().size.x * 0.5f, textSt.getLocalBounds().size.y * 0.5f});
        textSt.setPosition(st.viTri);
        sf::Color c = st.mau;
        c.a = static_cast<std::uint8_t>(255.f * doMo);
        textSt.setFillColor(c);
        textSt.setOutlineThickness(1.5f);
        textSt.setOutlineColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(200.f * doMo)));
        window.draw(textSt);
    }
}

void QuanLyGiaoDien::VeManHinhChienThang(sf::RenderWindow& window) {
    float w = RONG_MAN_HINH;
    float h = CAO_MAN_HINH;

    // Màn đen phủ mờ chiến thắng
    sf::RectangleShape manPhu({w, h});
    manPhu.setFillColor(sf::Color(10, 20, 30, 225));
    window.draw(manPhu);

    // Bảng vinh danh vàng kim
    sf::RectangleShape bang({600.f, 380.f});
    bang.setOrigin({300.f, 190.f});
    bang.setPosition({w * 0.5f, h * 0.5f});
    bang.setFillColor(sf::Color(25, 30, 42, 240));
    bang.setOutlineThickness(3.5f);
    bang.setOutlineColor(sf::Color(255, 215, 0));
    window.draw(bang);

    // Tiêu đề chiến thắng
    VeChuGiua(window, phongChu, "ĐẠI THẮNG BẠCH ĐẰNG 938", 28, {w * 0.5f, h * 0.5f - 130.f}, sf::Color(255, 215, 0), 2.f, sf::Color(120, 40, 20));

    // Lời bình lịch sử
    VeChuGiua(window, phongChu,
        "Quân Nam Hán đại bại, Lưu Hoằng Tháo bỏ mạng.\n"
        "Bãi cọc ngầm đóng sẵn dưới lòng sông đã nhấn chìm hạm đội xâm lược.\n\n"
        "Chiến thắng Bạch Đằng năm 938 của Ngô Quyền\n"
        "chấm dứt hơn một nghìn năm Bắc thuộc,\n"
        "mở ra thời kỳ độc lập, tự chủ lâu dài cho dân tộc.\n\n"
        "Bạch Đằng giang, thiên thu chiến tích.\n\n"
        "[ Nhấn Enter hoặc Space để quay về chọn chương ]", 13, {w * 0.5f, h * 0.5f + 25.f}, sf::Color(230, 240, 255));
}

void QuanLyGiaoDien::VeManHinhGameOver(sf::RenderWindow& window) {
    float w = RONG_MAN_HINH;
    float h = CAO_MAN_HINH;

    sf::RectangleShape manPhu({w, h});
    manPhu.setFillColor(sf::Color(40, 10, 10, 230));
    window.draw(manPhu);

    VeChuGiua(window, phongChu, "TRẬN ĐỊA THẤT THỦ", 30, {w * 0.5f, h * 0.5f - 60.f}, sf::Color(220, 40, 40), 2.f, sf::Color(0, 0, 0, 200));
    VeChuGiua(window, phongChu, "Chiến binh đã hy sinh vì tổ quốc.\n\n[ Nhấn Enter hoặc Space để quay về chọn chương ]", 14,
              {w * 0.5f, h * 0.5f + 30.f}, sf::Color::White);
}
