#pragma once
#include "Thuyen.h"

class ThuyenKeDich : public Thuyen {
private:
    bool soaiHam;     // True nếu là Soái hạm của Lưu Hoằng Thao
    bool daMacCoc;    // Trạng thái va phải bãi cọc ngầm Bạch Đằng

public:
    // Constructor nhận vào vị trí ban đầu và cờ xác định có phải soái hạm không
    ThuyenKeDich(sf::Vector2f viTriBanDau, bool laSoaiHam = false)
        : Thuyen(viTriBanDau, laSoaiHam ? 140.f : 170.f, laSoaiHam ? 320 : 160, sf::Color::Red),
        soaiHam(laSoaiHam),
        daMacCoc(false) {
    }

    // Kiểm tra có phải soái hạm không
    bool IsSoaiHam() const { return soaiHam; }

    // Kiểm tra trạng thái mắc cọc ngầm
    bool IsDaMacCoc() const { return daMacCoc; }
    void SetDaMacCoc(bool macCoc) { daMacCoc = macCoc; }

    // Hàm điều khiển AI đuổi theo thuyền người chơi
    void DuoiTheo(sf::Vector2f viTriThuyenPlayer, float deltaTime) {
        if (DaBiPhaHuy() || daMacCoc) return;

        sf::Vector2f viTriHienTai = GetViTri();
        sf::Vector2f huong = viTriThuyenPlayer - viTriHienTai;

        // Gọi hàm DiChuyen sẵn có của lớp Thuyen để di chuyển và đổi hướng sprite 8 hướng
        DiChuyen(huong, deltaTime);
    }
};