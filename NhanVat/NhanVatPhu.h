
#pragma once

#include "NhanVat.h"
#include <string>
#include <vector>

enum class LoaiNPC {
    NGO_QUYEN,       // Chủ soái Ngô Quyền (áo bào tím than, gươm báu, phong thái uy nghiêm)
    LINH_DAI_VIET,   // Quân tiên phong Đại Việt
    DAN_LANG         // Dân làng hỗ trợ đóng cọc đẽo gỗ
};

class NhanVatPhu : public NhanVat {
private:
    std::string ten;
    LoaiNPC loai;
    std::vector<std::string> danhSachLoiThoai;
    int chiSoLoiThoaiHienTai;
    float phamViTuongTac;
    bool dangTuongTac;
    float thoiGianHoatAnh;

    sf::VertexArray mangDinh{sf::PrimitiveType::Triangles};

public:
    NhanVatPhu(sf::Vector2f viTriBanDau, LoaiNPC loaiNpc, const std::string& tenNpc,
               const std::vector<std::string>& hoiThoai);
    virtual ~NhanVatPhu() = default;

    void CapNhat(float deltaTime) override;
    void Ve(sf::RenderWindow& window) override;

    bool KiemTraPhamViTuongTac(sf::Vector2f viTriPlayer) const;
    std::string LayLoiThoaiTiepTheo();
    void ResetLoiThoai();

    std::string GetTen() const { return ten; }
    LoaiNPC GetLoai() const { return loai; }
    bool IsDangTuongTac() const { return dangTuongTac; }
    void SetDangTuongTac(bool st) { dangTuongTac = st; }
};


