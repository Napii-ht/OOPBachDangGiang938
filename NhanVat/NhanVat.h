
#pragma once

#include <SFML/Graphics.hpp>

// Class cha chứa các thuộc tính chung cho mọi nhân vật (Player, KeDich, NPC)
class NhanVat {
protected:
    sf::Vector2f viTri;
    float tocDo;
    int sucKhoe;
    int sucKhoeToiDa;
    int satThuong;
    bool daHySinh;

public:
    NhanVat(sf::Vector2f viTriBanDau, float tocDoDiChuyen, int mauToiDa, int dame);
    virtual ~NhanVat() = default;

    // Các hàm bắt buộc class con (Player, KeDich) phải tự viết ruột hàm
    virtual void CapNhat(float deltaTime) = 0;
    virtual void Ve(sf::RenderWindow& window) = 0;

    // Các hành động cơ bản
    virtual void DiChuyen(sf::Vector2f huongDiChuyen, float deltaTime);
    virtual void NhanSatThuong(int damage);
    virtual void HySinh();

    // Lấy và sửa thông tin
    sf::Vector2f GetViTri() const { return viTri; }
    void SetViTri(sf::Vector2f pos) { viTri = pos; }

    int GetSucKhoe() const { return sucKhoe; }
    int GetSucKhoeToiDa() const { return sucKhoeToiDa; }
    int GetSatThuong() const { return satThuong; }
    bool IsDaHySinh() const { return daHySinh; }
};


