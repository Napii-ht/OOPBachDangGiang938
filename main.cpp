#include <SFML/Graphics.hpp>
#include <iostream>
#include "CotLoi/KiemTraMaHoa.h"
#include "CotLoi/TienIch.h"
#include "CotLoi/QuanLyGame.h"

int main() {
    // Cửa sổ 800x600, có thể phóng to thu nhỏ, hình luôn giữ tỉ lệ 4:3
    sf::RenderWindow window(sf::VideoMode({800, 600}), VanBan("Bạch Đằng 938"));
    window.setFramerateLimit(60);
    window.setMinimumSize(sf::Vector2u(400, 300));

    QuanLyGame game(window);
    if (!game.SanSang()) {
        std::cerr << "Game khong the khoi dong vi thieu tai nguyen (xem thong bao o tren).\n";
        return 1;
    }

    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        if (deltaTime > 0.05f) deltaTime = 0.05f; // Tránh bước thời gian quá lớn khi bị giật lag

        while (const auto suKien = window.pollEvent()) {
            if (suKien->is<sf::Event::Closed>()) {
                window.close();
            } else {
                game.XuLySuKien(*suKien);
            }
        }

        game.CapNhat(deltaTime);
        game.Ve();
    }

    return 0;
}
