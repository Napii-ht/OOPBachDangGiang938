#include "MenuChinh.h"
#include "../CotLoi/TienIch.h"
#include <cmath>

MenuChinh::MenuChinh()
    : dangMoHuongDan(false),
      dangMoGioiThieu(false),
      thoiGian(0.f) {
    danhSachNut.DatDanhSach({"Bắt đầu chơi", "Hướng dẫn", "Cài đặt", "Giới thiệu nhóm", "Thoát game"});
    danhSachNut.DatBoCuc({RONG_MAN_HINH * 0.5f, 250.f}, 320.f, 40.f, 50.f, 15);
}

LuaChonMenu MenuChinh::XuLySuKien(const sf::Event& suKien, const sf::RenderWindow& window) {
    // Đang mở bảng phụ thì chỉ chờ đóng lại
    if (IsDangMoMenuCon()) {
        bool dong = false;
        if (const auto* phim = suKien.getIf<sf::Event::KeyPressed>()) {
            dong = (phim->code == sf::Keyboard::Key::Escape || phim->code == sf::Keyboard::Key::Space ||
                    phim->code == sf::Keyboard::Key::Enter);
        } else if (const auto* bam = suKien.getIf<sf::Event::MouseButtonPressed>()) {
            dong = (bam->button == sf::Mouse::Button::Left);
        }
        if (dong) {
            dangMoHuongDan = false;
            dangMoGioiThieu = false;
        }
        return LuaChonMenu::KHONG_CO;
    }

    int nut = danhSachNut.XuLySuKien(suKien, window);
    switch (nut) {
        case 0: return LuaChonMenu::CHOI;
        case 1: dangMoHuongDan = true; return LuaChonMenu::HUONG_DAN;
        case 2: return LuaChonMenu::CAI_DAT;
        case 3: dangMoGioiThieu = true; return LuaChonMenu::GIOI_THIEU;
        case 4: return LuaChonMenu::THOAT;
        default: return LuaChonMenu::KHONG_CO;
    }
}

void MenuChinh::CapNhat(float deltaTime) {
    thoiGian += deltaTime;
    danhSachNut.CapNhat(deltaTime);
}

void MenuChinh::VeKhungPhu(sf::RenderWindow& window, const sf::Font& font, const std::string& tieuDe, const std::string& noiDung) const {
    sf::RectangleShape lamMo({RONG_MAN_HINH, CAO_MAN_HINH});
    lamMo.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(lamMo);

    sf::RectangleShape khung({600.f, 400.f});
    khung.setOrigin({300.f, 200.f});
    khung.setPosition({RONG_MAN_HINH * 0.5f, CAO_MAN_HINH * 0.5f});
    khung.setFillColor(sf::Color(20, 25, 35, 250));
    khung.setOutlineThickness(3.f);
    khung.setOutlineColor(sf::Color(255, 215, 0));
    window.draw(khung);

    VeChuGiua(window, font, tieuDe, 18, {RONG_MAN_HINH * 0.5f, 118.f}, sf::Color(255, 215, 0));
    VeChuTrai(window, font, noiDung, 13, {RONG_MAN_HINH * 0.5f - 270.f, 150.f}, sf::Color::White);
    VeChuGiua(window, font, "[ Nhấn Esc, Enter hoặc click chuột để đóng ]", 12, {RONG_MAN_HINH * 0.5f, 476.f}, sf::Color(180, 200, 220));
}

void MenuChinh::Ve(sf::RenderWindow& window, const sf::Font& font) const {
    float w = RONG_MAN_HINH;

    // Đang mở bảng phụ thì chỉ vẽ bảng đó cho khỏi rối
    if (!IsDangMoMenuCon()) {
        // Tiêu đề game, hơi nhấp nhô như sóng
        float nhun = std::sin(thoiGian * 1.8f) * 2.f;
        VeChuGiua(window, font, "BẠCH ĐẰNG 938", 48, {w * 0.5f, 105.f + nhun}, sf::Color(255, 215, 0), 3.f, sf::Color(150, 40, 20));
        VeChuGiua(window, font, "Một dòng sông. Một trận chiến. Một bước ngoặt lịch sử.", 14, {w * 0.5f, 160.f}, sf::Color(215, 225, 240), 1.f, sf::Color(0, 0, 0, 160));

        danhSachNut.Ve(window, font);

        VeChuGiua(window, font, "Chọn bằng W/S hoặc mũi tên, Enter để xác nhận, hoặc dùng chuột", 11, {w * 0.5f, 578.f}, sf::Color(170, 185, 205));
    }

    if (dangMoHuongDan) {
        VeKhungPhu(window, font, "HƯỚNG DẪN ĐIỀU KHIỂN",
            "Đi bộ và chèo thuyền:  W A S D hoặc phím mũi tên\n"
            "Chạy nhanh khi đi bộ:  giữ Shift trái (tốn thể lực)\n"
            "Chém bằng kiếm:  Space hoặc J\n"
            "Nói chuyện, nhặt đồ, chặt cây, bốc và cắm cọc:  E\n"
            "Khiêu khích giặc khi nhử địch (chương 4 và 5):  Q\n"
            "Phát hiệu lệnh trống khi nước rút (chương 5):  Space\n"
            "Chọn vật phẩm trên thanh nhanh:  phím số 1 đến 6\n"
            "Tạm dừng:  Esc          Toàn màn hình:  F11\n\n"
            "Mẹo: bãi cọc chỉ đâm thủng được thuyền khi nước triều đã rút thấp.\n"
            "Hãy nhử giặc vào lúc triều cao, rồi mới phát hiệu lệnh.");
    }

    if (dangMoGioiThieu) {
        VeKhungPhu(window, font, "NHÓM THỰC HIỆN",
            "Đồ án môn Lập trình hướng đối tượng: Bạch Đằng 938\n\n"
            "Hoàng Tuấn:  Người chơi, lõi game, lưu và tải game\n"
            "Huyền Trang:  Nhân vật phụ, nhiệm vụ, lời thoại, sự kiện lịch sử\n"
            "Minh Nhân:  Kẻ địch, trí tuệ nhân tạo, hệ thống chiến đấu\n"
            "Hoàng Yến:  Bản đồ, sông, thuyền, bãi cọc, thủy triều, túi đồ\n"
            "Quỳnh Ngân:  Giao diện, âm thanh, cảnh phim\n\n"
            "Công nghệ:  C++17, SFML 3.1.0");
    }
}
