#include <SFML/Graphics.hpp>
#include "GiaoDien/MenuChinh.h"
#include "GiaoDien/ThanhMau.h"

enum class TrangThaiGame {
    MENU,
    DANG_CHOI
};

int main() {
    const float SCREEN_W = 800.f;
    const float SCREEN_H = 600.f;

    sf::RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H), "Bach Dang 938 - Test Menu & HUD");
    window.setFramerateLimit(60);

    // Đường dẫn font tiếng Việt bạn đã chuẩn bị ở Bước 1
    std::string fontPath = "TaiNguyen/PhongChu/font_game.ttf";

    // Khởi tạo Menu và HUD
    MenuChinh menu(SCREEN_W, SCREEN_H, fontPath);

    // Thanh máu đỏ ở góc trên bên trái: x = 20, y = 20, rộng = 200, cao = 25
    ThanhMau thanhMau(20.f, 20.f, 200.f, 25.f, sf::Color::Red);
    // Thanh thể lực xanh lá bên dưới thanh máu: x = 20, y = 55, rộng = 160, cao = 15
    ThanhMau thanhTheLuc(20.f, 55.f, 160.f, 15.f, sf::Color(46, 204, 113));

    // Biến mô phỏng máu để test
    float mauHienTai = 100.f;
    float mauToiDa = 100.f;

    TrangThaiGame trangThai = TrangThaiGame::MENU;

    while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (trangThai == TrangThaiGame::MENU) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    LuaChonMenu chon = menu.XuLyClick(mousePos);
                    if (chon == LuaChonMenu::BAT_DAU) {
                        trangThai = TrangThaiGame::DANG_CHOI;
                    }
                    else if (chon == LuaChonMenu::THOAT) {
                        window.close();
                    }
                }
            }
            else if (trangThai == TrangThaiGame::DANG_CHOI) {
                // Nhấn phím SPACE để test giảm máu, R để hồi đầy máu
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space) {
                        mauHienTai -= 15.f;
                        if (mauHienTai < 0.f) mauHienTai = 0.f;
                    }
                    if (event.key.code == sf::Keyboard::R) {
                        mauHienTai = mauToiDa;
                    }
                    if (event.key.code == sf::Keyboard::Escape) {
                        trangThai = TrangThaiGame::MENU; // Quay lại menu
                    }
                }
            }
        }

        // Cập nhật
        if (trangThai == TrangThaiGame::MENU) {
            menu.CapNhat(mousePos);
        }
        else if (trangThai == TrangThaiGame::DANG_CHOI) {
            thanhMau.CapNhat(mauHienTai, mauToiDa);
            thanhTheLuc.CapNhat(80.f, 100.f); // Giả lập thể lực đang ở 80%
        }

        // Vẽ ra màn hình
        window.clear(sf::Color(25, 25, 35));

        if (trangThai == TrangThaiGame::MENU) {
            menu.Ve(window);
        }
        else if (trangThai == TrangThaiGame::DANG_CHOI) {
            thanhMau.Ve(window);
            thanhTheLuc.Ve(window);
        }

        window.display();
    }

    return 0;
}