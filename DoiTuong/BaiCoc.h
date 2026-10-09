#pragma once

#include <SFML/Graphics.hpp>

class BaiCoc
{
private:
    int maVatPham;
    sf::Vector2f viTri;
    sf::RectangleShape hinhDang;

    bool dangLo;
    bool daDuocNhat;

public:
    BaiCoc(int ma, sf::Vector2f viTriBanDau);

    int GetMaVatPham() const;
    sf::Vector2f GetViTri() const;
    sf::FloatRect GetHitBox() const;

    bool DangLo() const;
    bool DaDuocNhat() const;

    void DatDangLo(bool lo);
    bool NhatCoc();

    void Ve(sf::RenderWindow& cuaSo) const;
};