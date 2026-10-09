#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <memory>
#include <string>

class Thuyen
{
private:
    // 8 hướng: lên, chéo lên phải, phải, chéo lên trái,
    // xuống, chéo xuống trái, trái, chéo xuống phải.
    std::array<std::unique_ptr<sf::Texture>, 8> textures;
    std::array<std::unique_ptr<sf::Sprite>, 8> sprites;

    int huongHienTai = 0;

    int sucKhoe;
    int sucKhoeToiDa;
    float tocDo;
    bool daBiPhaHuy = false;

    sf::RectangleShape thanhMauNen;
    sf::RectangleShape thanhMauHienTai;
    sf::Color mauThanhMau;

    void CapNhatThanhMau(sf::Color mauThanhMau);

public:
    Thuyen(
        sf::Vector2f viTriBanDau,
        float tocDoBanDau = 180.f,
        int mauToiDa = 200,
        sf::Color mauThanhMau = sf::Color::Green
    );

    bool HopLe() const;

    void DiChuyen(
        const sf::Vector2f& huong,
        float deltaTime
    );

    void SetViTri(sf::Vector2f viTri);
    sf::Vector2f GetViTri() const;
    sf::FloatRect GetHitBox() const;

    void NhanSatThuong(int satThuong);
    bool DaBiPhaHuy() const;

    int GetMau() const;
    int GetMauToiDa() const;

    void CapNhat(float deltaTime);
    void Ve(sf::RenderWindow& window);
};