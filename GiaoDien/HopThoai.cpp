
#include "HopThoai.h"
#include "../CotLoi/TienIch.h"
#include <cmath>

HopThoai::HopThoai()
    : dangHienThi(false),
      tenNguoiNoi(""),
      noiDungToanBo(""),
      noiDungHienTai(""),
      thoiGianGoChu(0.f),
      soKyTuDaGo(0),
      daNgatDong(false),
      loaiNguoiNoi(LoaiNPC::NGO_QUYEN),
      daTaiAvatarNgoQuyen(false) {
    if (textureAvatarNgoQuyen.loadFromFile("TaiNguyen/HinhAnh/NgoQuyen_Avatar.png") ||
        textureAvatarNgoQuyen.loadFromFile("TaiNguyen/HinhAnh/NgoQuyen.png") ||
        textureAvatarNgoQuyen.loadFromFile("TaiNguyen/HinhAnh/NgoQuyen_Avatar.jpg")) {
        daTaiAvatarNgoQuyen = true;
    }
}

void HopThoai::MoHopThoai(const std::string& nguoiNoi, const std::string& noiDung, LoaiNPC loai) {
    dangHienThi = true;
    tenNguoiNoi = nguoiNoi;
    noiDungToanBo = noiDung;
    noiDungHienTai = "";
    thoiGianGoChu = 0.f;
    soKyTuDaGo = 0;
    daNgatDong = false;
    loaiNguoiNoi = loai;
}

void HopThoai::DongHopThoai() {
    dangHienThi = false;
    noiDungHienTai = "";
}

void HopThoai::BoQuaHieuUngGo() {
    noiDungHienTai = noiDungToanBo;
    soKyTuDaGo = noiDungToanBo.length();
}

void HopThoai::CapNhat(float deltaTime) {
    if (!dangHienThi) return;

    if (soKyTuDaGo < noiDungToanBo.length()) {
        thoiGianGoChu += deltaTime;
        if (thoiGianGoChu >= 0.025f) { // Tốc độ gõ chữ mượt mà
            thoiGianGoChu = 0.f;
            soKyTuDaGo++;
            // Chữ có dấu chiếm nhiều byte, phải gõ trọn cả ký tự rồi mới hiện
            while (soKyTuDaGo < noiDungToanBo.length() &&
                   (static_cast<unsigned char>(noiDungToanBo[soKyTuDaGo]) & 0xC0) == 0x80) {
                soKyTuDaGo++;
            }
            noiDungHienTai = noiDungToanBo.substr(0, soKyTuDaGo);
        }
    }
}

void HopThoai::Ve(sf::RenderWindow& window, const sf::Font& font) {
    if (!dangHienThi) return;

    // Tự xuống dòng một lần cho cả câu thoại (làm ở đây vì cần font để đo chữ)
    if (!daNgatDong) {
        noiDungToanBo = NgatDong(font, noiDungToanBo, 13, 480.f);
        daNgatDong = true;
    }

    const float x = 80.f;
    const float y = 430.f;
    const float w = 640.f;
    const float h = 140.f;

    // Hộp thoại phong cách Terraria (Gỗ sẫm viền vàng kim quý tộc)
    sf::RectangleShape khung({w, h});
    khung.setPosition({x, y});
    khung.setFillColor(sf::Color(22, 26, 36, 235));
    khung.setOutlineThickness(3.f);
    khung.setOutlineColor(sf::Color(218, 165, 32));
    window.draw(khung);

    // Khung tranh đại diện người nói (Avatar Box)
    sf::RectangleShape khungAvatar({75.f, 85.f});
    khungAvatar.setPosition({x + 18.f, y + 25.f});
    khungAvatar.setFillColor(sf::Color(15, 18, 26));
    khungAvatar.setOutlineThickness(2.f);
    khungAvatar.setOutlineColor(sf::Color(180, 140, 60));
    window.draw(khungAvatar);

    // Vẽ hình đại diện tùy theo nhân vật
    sf::Vector2f avatarCenter(x + 18.f + 37.5f, y + 25.f + 42.5f);
    if (loaiNguoiNoi == LoaiNPC::NGO_QUYEN) {
        if (daTaiAvatarNgoQuyen) {
            sf::Sprite sp(textureAvatarNgoQuyen);
            sf::Vector2u sz = textureAvatarNgoQuyen.getSize();
            if (sz.x > 0 && sz.y > 0) {
                sp.setScale({71.f / static_cast<float>(sz.x), 81.f / static_cast<float>(sz.y)});
                sp.setPosition({x + 20.f, y + 27.f});
                window.draw(sp);
            }
        } else {
            sf::CircleShape dau(20.f);
            dau.setOrigin({20.f, 20.f});
            dau.setPosition(avatarCenter);
            dau.setFillColor(sf::Color(255, 220, 180));
            dau.setOutlineThickness(2.f);
            dau.setOutlineColor(sf::Color(218, 165, 32));
            window.draw(dau);

            sf::RectangleShape muQuan({24.f, 10.f});
            muQuan.setOrigin({12.f, 5.f});
            muQuan.setPosition({avatarCenter.x, avatarCenter.y - 18.f});
            muQuan.setFillColor(sf::Color(218, 165, 32));
            window.draw(muQuan);
        }
    } else {
        sf::CircleShape dau(18.f);
        dau.setOrigin({18.f, 18.f});
        dau.setPosition(avatarCenter);
        dau.setFillColor(sf::Color(245, 210, 175));
        dau.setOutlineThickness(1.5f);
        dau.setOutlineColor(sf::Color(100, 60, 30));
        window.draw(dau);
    }

    // Tên người nói nổi bật viền vàng
    sf::Text textTen(font, VanBan(tenNguoiNoi), 15);
    textTen.setFillColor(sf::Color(255, 215, 0));
    textTen.setPosition({x + 110.f, y + 16.f});
    window.draw(textTen);

    sf::RectangleShape duongNgan({w - 130.f, 1.5f});
    duongNgan.setPosition({x + 110.f, y + 38.f});
    duongNgan.setFillColor(sf::Color(120, 100, 60));
    window.draw(duongNgan);

    // Nội dung đối thoại (có thể ngắt dòng tự động cơ bản)
    sf::Text textNoiDung(font, VanBan(noiDungHienTai), 13);
    textNoiDung.setFillColor(sf::Color::White);
    textNoiDung.setPosition({x + 110.f, y + 46.f});
    window.draw(textNoiDung);

    // Dấu nhắc nút bấm tiếp tục
    sf::Text nutNhan(font, VanBan("[Space / J: Tiếp tục]"), 11);
    nutNhan.setFillColor(sf::Color(180, 200, 220, 180));
    nutNhan.setPosition({x + w - 145.f, y + h - 22.f});
    window.draw(nutNhan);
}


