
#include "NhanVat.h"
#include <algorithm>

NhanVat::NhanVat(sf::Vector2f viTriBanDau, float tocDoDiChuyen, int mauToiDa, int dame)
    : viTri(viTriBanDau),
      tocDo(tocDoDiChuyen),
      sucKhoe(mauToiDa),
      sucKhoeToiDa(mauToiDa),
      satThuong(dame),
      daHySinh(false) {}

void NhanVat::DiChuyen(sf::Vector2f huongDiChuyen, float deltaTime) {
    viTri += huongDiChuyen * tocDo * deltaTime;
}

void NhanVat::NhanSatThuong(int damage) {
    if (daHySinh) return;

    sucKhoe = std::max(0, sucKhoe - damage);
    if (sucKhoe <= 0) {
        HySinh();
    }
}

void NhanVat::HySinh() {
    daHySinh = true;
    sucKhoe = 0;
}


