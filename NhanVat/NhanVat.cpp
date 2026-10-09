#include "NhanVat.h"
#include <cmath>

NhanVat::NhanVat(float x, float y, float tocDoBanDau, int mauToiDa, int satThuongBanDau)
    : viTri(x, y),
      sucKhoe(mauToiDa),
      sucKhoeToiDa(mauToiDa),
      satThuong(satThuongBanDau),
      tocDo(tocDoBanDau),
      daHySinh(false)
{
    hinhDang.setSize(sf::Vector2f(40.f, 40.f));
    hinhDang.setOrigin({20.f, 20.f});          // dat goc o giua de vi tri = tam nhan vat
    hinhDang.setFillColor(sf::Color::Red);
    hinhDang.setPosition(viTri);
}

NhanVat::NhanVat(sf::Vector2f viTriBanDau, float tocDoBanDau, int mauToiDa, int satThuongBanDau)
    : NhanVat(viTriBanDau.x, viTriBanDau.y, tocDoBanDau, mauToiDa, satThuongBanDau)   // goi lai ham khoi tao ben tren
{
}

void NhanVat::DiChuyen(const sf::Vector2f& huong, float dt)
{
    if (daHySinh) return;

    float doDai = std::sqrt(huong.x * huong.x + huong.y * huong.y);
    if (doDai > 0.f) {
        sf::Vector2f h = huong / doDai;        // chuan hoa de di cheo khong bi nhanh hon
        viTri += h * tocDo * dt;
        hinhDang.setPosition(viTri);
    }
}

void NhanVat::Ve(sf::RenderWindow& cuaSo)
{
    cuaSo.draw(hinhDang);
}

void NhanVat::NhanSatThuong(int damage)
{
    if (daHySinh || damage <= 0) return;

    sucKhoe -= damage;
    if (sucKhoe <= 0) {
        sucKhoe = 0;
        HySinh();
    }
}

void NhanVat::HySinh()
{
    daHySinh = true;
    hinhDang.setFillColor(sf::Color(100, 100, 100));   // xam = da chet
}

void NhanVat::SetViTri(float x, float y)
{
    viTri = sf::Vector2f(x, y);
    hinhDang.setPosition(viTri);
}
