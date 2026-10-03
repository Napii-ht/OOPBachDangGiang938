#include "NguoiChoi.h"
#include <cmath>

NguoiChoi::NguoiChoi(float x, float y)
    : NhanVat(x, y, 150.f, 100, 10),      // toc do 150, 100 mau, sat thuong 10
      theLuc(100.f),
      theLucToiDa(100.f),
      tocDoChay(260.f),
      phamViTanCong(70.f),
      huongDi(0.f, 0.f),
      huongNhin(1.f, 0.f),
      thoiGianHoiChieu(0.f),
      thoiGianTanCong(0.f),
      dangChay(false),
      muonChay(false),
      kietSuc(false),
      dangTanCong(false),
      vuaTanCong(false),
      daTuongTac(false),
      phimETruoc(false),
      phimCachTruoc(false)
{
    hinhDang.setFillColor(sf::Color(60, 120, 255));   // xanh duong
}

void NguoiChoi::XuLyPhim()
{
    daTuongTac = false;                    // cac co nay chi dung 1 khung hinh
    vuaTanCong = false;
    huongDi = sf::Vector2f(0.f, 0.f);
    muonChay = false;

    if (IsDaHySinh()) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    huongDi.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))  huongDi.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  huongDi.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) huongDi.x += 1.f;

    muonChay = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift);

    // E = tuong tac (chi tinh luc VUA bam)
    bool eHienTai = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);
    if (eHienTai && !phimETruoc) TuongTac();
    phimETruoc = eHienTai;

    // Space = tan cong (chi tinh luc VUA bam)
    bool cachHienTai = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    if (cachHienTai && !phimCachTruoc) TanCong();
    phimCachTruoc = cachHienTai;
}

void NguoiChoi::Chay(float dt)
{
    bool dangDiChuyen = (huongDi.x != 0.f || huongDi.y != 0.f);

    if (muonChay && dangDiChuyen && !kietSuc && theLuc > 0.f) {
        dangChay = true;
        theLuc -= 25.f * dt;               // chay ton 25 the luc / giay
        if (theLuc <= 0.f) {
            theLuc = 0.f;
            kietSuc = true;                // het hoi -> khoa chay cho den khi hoi >= 20
        }
    } else {
        dangChay = false;
        theLuc += 15.f * dt;               // hoi 15 the luc / giay
        if (theLuc > theLucToiDa) theLuc = theLucToiDa;
        if (kietSuc && theLuc >= 20.f) kietSuc = false;
    }
}

void NguoiChoi::DiChuyen(const sf::Vector2f& huong, float dt)
{
    if (IsDaHySinh()) return;

    float doDai = std::sqrt(huong.x * huong.x + huong.y * huong.y);
    if (doDai > 0.f) {
        sf::Vector2f h = huong / doDai;
        huongNhin = h;                                   // nho huong cuoi de danh
        float v = dangChay ? tocDoChay : tocDo;          // chay thi nhanh hon
        viTri += h * v * dt;
        hinhDang.setPosition(viTri);
    }
}

void NguoiChoi::TanCong()
{
    if (IsDaHySinh() || thoiGianHoiChieu > 0.f) return;

    dangTanCong = true;
    vuaTanCong = true;           // HeThongChienDau thay co nay thi quet ke dich trong GetPhamViTanCong()
    thoiGianTanCong = 0.2f;      // "don danh" keo dai 0.2 giay
    thoiGianHoiChieu = 0.5f;     // 0.5 giay moi danh lai duoc
}

void NguoiChoi::TuongTac()
{
    if (IsDaHySinh()) return;
    daTuongTac = true;           // NPC, coc, thuyen... se kiem tra co bien nay
}

void NguoiChoi::CapNhat(float dt)
{
    if (IsDaHySinh()) return;

    Chay(dt);
    DiChuyen(huongDi, dt);

    if (thoiGianHoiChieu > 0.f) thoiGianHoiChieu -= dt;

    if (dangTanCong) {
        thoiGianTanCong -= dt;
        if (thoiGianTanCong <= 0.f) dangTanCong = false;
    }
}

void NguoiChoi::Ve(sf::RenderWindow& cuaSo)
{
    NhanVat::Ve(cuaSo);          // ve than nhan vat nhu lop cha

    if (dangTanCong) {           // ve vong tron tam thoi de thay pham vi danh
        sf::CircleShape vong(phamViTanCong);
        vong.setOrigin({phamViTanCong, phamViTanCong});
        vong.setPosition(viTri);
        vong.setFillColor(sf::Color(255, 255, 0, 80));
        cuaSo.draw(vong);
    }
}
