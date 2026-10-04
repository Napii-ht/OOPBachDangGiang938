#pragma once

#include "../NhanVat/KeDich.h"
#include <SFML/System/Vector2.hpp>

// Bộ não tính toán xem lính nên làm gì (đứng yên, đuổi theo hay chém)
class HeThongAI {
public:
    HeThongAI() = default;
    ~HeThongAI() = default;

    // Tính toán hành động cho từng con lính dựa vào tọa độ của player
    void CapNhatAI(KeDich& keDich, sf::Vector2f viTriPlayer, float deltaTime);

    // Hàm tính khoảng cách Pytago giữa 2 điểm
    static float TinhKhoangCach(sf::Vector2f p1, sf::Vector2f p2);

    // Hàm tính hướng đi từ A đến B (vector đơn vị có độ dài = 1)
    static sf::Vector2f TinhHuong(sf::Vector2f nguon, sf::Vector2f dich);
};
