#pragma once

#include "../NhanVat/KeDich.h"
#include "../BanDo/Thuyen.h"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <memory>

class HeThongAI {
public:
    HeThongAI() = default;
    ~HeThongAI() = default;

    // Điều khiển hành vi của lính bộ binh Nam Hán
    void CapNhatAI(KeDich& keDich, sf::Vector2f viTriPlayer, float deltaTime);

    // Tránh việc nhiều lính dẫm đạp dính chặt vào 1 điểm (Flocking / Separation)
    void XuLyGianCachLinh(std::vector<std::unique_ptr<KeDich>>& danhSachDich, float khoangCachToiThieu = 28.f);

    // Điều khiển chiến thuyền giặc đuổi theo tàu nhẹ Đại Việt
    void CapNhatAIThuyen(ThuyenKeDich& thuyen, sf::Vector2f viTriThuyenPlayer, float deltaTime);

    // Hàm toán học vector
    static float TinhKhoangCach(sf::Vector2f p1, sf::Vector2f p2);
    static sf::Vector2f TinhHuong(sf::Vector2f nguon, sf::Vector2f dich);
};
