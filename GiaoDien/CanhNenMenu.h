#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

// Cảnh nền chung cho menu, chọn chương, cài đặt: đêm trăng trên sông Bạch Đằng,
// núi đá vôi phía xa, vài chiến thuyền Đại Việt lững lờ trôi. Toàn bộ vẽ bằng code.
class CanhNenMenu {
private:
    struct Sao {
        float x, y, pha, doSang;
    };
    struct ThuyenNen {
        float x, y, tiLe, tocDo;
    };

    float thoiGian;
    std::vector<Sao> danhSachSao;
    std::vector<ThuyenNen> danhSachThuyen;
    sf::VertexArray lopTroi{sf::PrimitiveType::Triangles};   // Bầu trời (vẽ trước sao và trăng)
    sf::VertexArray lopTinh{sf::PrimitiveType::Triangles};   // Đồi, núi, sông (vẽ sau sao và trăng)

    void DungLopTinh();
    void VeThuyen(sf::RenderWindow& window, const ThuyenNen& t) const;

public:
    CanhNenMenu();

    void CapNhat(float deltaTime);
    void Ve(sf::RenderWindow& window) const;
};
