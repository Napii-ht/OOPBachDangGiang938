
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "../NhanVat/NhanVatPhu.h"

class HopThoai {
private:
    bool dangHienThi;
    std::string tenNguoiNoi;
    std::string noiDungToanBo;
    std::string noiDungHienTai;
    float thoiGianGoChu;
    size_t soKyTuDaGo;
    bool daNgatDong;
    LoaiNPC loaiNguoiNoi;
    sf::Texture textureAvatarNgoQuyen;
    bool daTaiAvatarNgoQuyen;

public:
    HopThoai();
    ~HopThoai() = default;

    void MoHopThoai(const std::string& nguoiNoi, const std::string& noiDung, LoaiNPC loai = LoaiNPC::NGO_QUYEN);
    void DongHopThoai();
    void BoQuaHieuUngGo();

    void CapNhat(float deltaTime);
    void Ve(sf::RenderWindow& window, const sf::Font& font);

    bool IsDangHienThi() const { return dangHienThi; }
    bool DaGoXong() const { return soKyTuDaGo >= noiDungToanBo.length(); }
};


