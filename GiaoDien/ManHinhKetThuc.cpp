#include "ManHinhKetThuc.h"
#include "../CotLoi/TienIch.h"
#include <algorithm>
#include <cmath>

namespace {
const float THOI_GIAN_MOI_TRANG = 9.f;
const float TOC_DO_CUON = 34.f;
}

ManHinhKetThuc::ManHinhKetThuc()
    : buoc(Buoc::LOI_BAT),
      thoiGian(0.f),
      viTriCuon(0.f),
      chiSoTrang(0),
      daXong(false) {
    cacTrangLoiBat = {
        "Mùa đông năm 938, thủy quân Nam Hán kéo vào cửa sông Bạch Đằng.\n"
        "Ngô Quyền cho đóng cọc nhọn bịt sắt dưới lòng sông,\n"
        "dùng thuyền nhẹ nhử giặc vào lúc triều lên,\n"
        "rồi tổng phản công khi nước triều rút.",

        "Thuyền giặc mắc cọc, quân Nam Hán đại bại,\n"
        "Lưu Hoằng Tháo bị giết.\n"
        "Chiến thắng ấy chấm dứt hơn một nghìn năm Bắc thuộc,\n"
        "mở ra thời kỳ độc lập, tự chủ lâu dài cho dân tộc.",

        "Bạch Đằng giang, thiên thu chiến tích."
    };

    // Vai trò theo bảng phân công của nhóm
    dongCredits = {
        {"", "BẠCH ĐẰNG 938"},
        {"", "Đồ án môn Lập trình hướng đối tượng"},
        {"", ""},
        {"Người chơi, lõi game, lưu và tải game", "Hoàng Tuấn"},
        {"Nhân vật phụ, nhiệm vụ, lời thoại, sự kiện lịch sử", "Huyền Trang"},
        {"Kẻ địch, trí tuệ nhân tạo, hệ thống chiến đấu", "Minh Nhân"},
        {"Bản đồ, sông, thuyền, bãi cọc, thủy triều, túi đồ", "Hoàng Yến"},
        {"Giao diện, âm thanh, cảnh phim", "Quỳnh Ngân"},
        {"", ""},
        {"Công nghệ", "C++17, SFML 3.1.0"},
        {"Đồ họa", "Vẽ hoàn toàn bằng mã nguồn"},
        {"", ""},
        {"", "Cảm ơn bạn đã chơi."}
    };
}

void ManHinhKetThuc::Mo() {
    buoc = Buoc::LOI_BAT;
    thoiGian = 0.f;
    viTriCuon = 0.f;
    chiSoTrang = 0;
    daXong = false;
}

void ManHinhKetThuc::ChuyenBuoc() {
    if (buoc == Buoc::LOI_BAT) {
        ++chiSoTrang;
        thoiGian = 0.f;
        if (chiSoTrang >= cacTrangLoiBat.size()) {
            buoc = Buoc::CREDITS;
            viTriCuon = 0.f;
        }
    } else if (buoc == Buoc::CREDITS) {
        buoc = Buoc::XONG;
        daXong = true;
    }
}

void ManHinhKetThuc::XuLySuKien(const sf::Event& suKien) {
    if (const auto* phim = suKien.getIf<sf::Event::KeyPressed>()) {
        if (phim->code == sf::Keyboard::Key::Escape) {
            daXong = true;
        } else if (phim->code == sf::Keyboard::Key::Enter || phim->code == sf::Keyboard::Key::Space) {
            ChuyenBuoc();
        }
    } else if (const auto* bam = suKien.getIf<sf::Event::MouseButtonPressed>()) {
        if (bam->button == sf::Mouse::Button::Left) ChuyenBuoc();
    }
}

void ManHinhKetThuc::CapNhat(float deltaTime) {
    canhNen.CapNhat(deltaTime);
    thoiGian += deltaTime;

    if (buoc == Buoc::LOI_BAT) {
        if (thoiGian >= THOI_GIAN_MOI_TRANG) ChuyenBuoc();
    } else if (buoc == Buoc::CREDITS) {
        viTriCuon += TOC_DO_CUON * deltaTime;
        // Cuộn hết thì kết thúc
        float tongCao = static_cast<float>(dongCredits.size()) * 44.f + CAO_MAN_HINH;
        if (viTriCuon > tongCao) ChuyenBuoc();
    }
}

void ManHinhKetThuc::Ve(sf::RenderWindow& window, const sf::Font& font) const {
    canhNen.Ve(window);

    sf::RectangleShape lamToi({RONG_MAN_HINH, CAO_MAN_HINH});
    lamToi.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(lamToi);

    if (buoc == Buoc::LOI_BAT && chiSoTrang < cacTrangLoiBat.size()) {
        // Chữ hiện dần rồi mờ dần ở cuối mỗi trang
        float mo = std::min(1.f, thoiGian / 1.2f);
        float tat = std::min(1.f, (THOI_GIAN_MOI_TRANG - thoiGian) / 1.2f);
        std::uint8_t doDuc = static_cast<std::uint8_t>(255.f * std::max(0.f, std::min(mo, tat)));

        bool trangCuoi = (chiSoTrang + 1 == cacTrangLoiBat.size());
        // Căn giữa từng dòng một cho cân đối
        std::vector<std::string> cacDong;
        std::string dong;
        for (char c : cacTrangLoiBat[chiSoTrang]) {
            if (c == '\n') { cacDong.push_back(dong); dong.clear(); }
            else dong += c;
        }
        cacDong.push_back(dong);
        float khoang = trangCuoi ? 36.f : 30.f;
        float y0 = CAO_MAN_HINH * 0.5f - khoang * static_cast<float>(cacDong.size() - 1) * 0.5f;
        for (size_t i = 0; i < cacDong.size(); ++i) {
            VeChuGiua(window, font, cacDong[i], trangCuoi ? 26u : 17u,
                      {RONG_MAN_HINH * 0.5f, y0 + khoang * static_cast<float>(i)},
                      trangCuoi ? sf::Color(255, 215, 0, doDuc) : sf::Color(235, 240, 250, doDuc),
                      trangCuoi ? 2.f : 1.f, sf::Color(0, 0, 0, static_cast<std::uint8_t>(doDuc * 0.7f)));
        }
        VeChuGiua(window, font, "[ Enter hoặc Space để tiếp tục ]", 11, {RONG_MAN_HINH * 0.5f, 570.f}, sf::Color(170, 185, 205, doDuc));
        return;
    }

    if (buoc == Buoc::CREDITS) {
        float y0 = CAO_MAN_HINH - viTriCuon;
        for (size_t i = 0; i < dongCredits.size(); ++i) {
            float y = y0 + static_cast<float>(i) * 44.f;
            if (y < -40.f || y > CAO_MAN_HINH + 40.f) continue;

            // Mờ dần ở mép trên và dưới màn hình
            float mo = std::min(1.f, std::min(y / 80.f, (CAO_MAN_HINH - y) / 80.f));
            std::uint8_t a = static_cast<std::uint8_t>(255.f * std::max(0.f, mo));

            const auto& dong = dongCredits[i];
            if (dong.first.empty()) {
                bool tieuDe = (i == 0);
                VeChuGiua(window, font, dong.second, tieuDe ? 38u : 16u, {RONG_MAN_HINH * 0.5f, y},
                          tieuDe ? sf::Color(255, 215, 0, a) : sf::Color(235, 240, 250, a), tieuDe ? 3.f : 0.f, sf::Color(150, 40, 20, a));
            } else {
                VeChuGiua(window, font, dong.first, 12, {RONG_MAN_HINH * 0.5f, y - 9.f}, sf::Color(170, 190, 215, a));
                VeChuGiua(window, font, dong.second, 18, {RONG_MAN_HINH * 0.5f, y + 11.f}, sf::Color(255, 240, 200, a));
            }
        }
        sf::RectangleShape dai({RONG_MAN_HINH, 34.f});
        dai.setPosition({0.f, CAO_MAN_HINH - 34.f});
        dai.setFillColor(sf::Color(8, 12, 24, 235));
        window.draw(dai);
        VeChuGiua(window, font, "[ Enter để bỏ qua, Esc để về menu ]", 11, {RONG_MAN_HINH * 0.5f, 582.f}, sf::Color(150, 165, 185, 180));
    }
}
