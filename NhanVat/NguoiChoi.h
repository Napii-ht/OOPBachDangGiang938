
#pragma once

#include "NhanVat.h"
#include "../VatPham/TuDo.h"
#include <SFML/Graphics.hpp>
#include <vector>

class NguoiChoi : public NhanVat {
private:
    float theLuc;
    float theLucToiDa;
    float tocDoChay;
    bool dangChayNhanh;

    float phamViTanCong;
    bool dangTanCong;
    float thoiGianTanCong;
    int kieuChem;                  // 0 = chặt từ trên xuống, 1 = hất từ dưới lên (đổi qua lại khi chém liên tiếp)
    float thoiGianSanSang;         // Còn bao lâu kiếm vẫn cầm trên tay sau nhát chém, hết thì tra vào vỏ
    int huongNhin; // 1 = Phải, -1 = Trái

    // Hoạt ảnh chuyển động & nghệ thuật đồ họa
    float thoiGianHoatAnh;
    float nhipBuocChan;
    bool dangDiChuyen;
    float heSoLat; // Lật mượt từ -1 đến 1

    // Giới hạn bản đồ và vật cản (dùng cho các chương đi bộ). Mặc định không giới hạn như bản cũ.
    bool coGioiHan;
    sf::FloatRect gioiHan;
    const std::vector<sf::FloatRect>* danhSachVatCan;
    bool coKiem;   // Chưa nhận kiếm thì chưa chém được và không đeo kiếm

    // Túi đồ cá nhân của chiến binh
    TuDo tuDo;

    // Bộ đệm vẽ VertexArray gom đỉnh
    sf::VertexArray mangDinh{sf::PrimitiveType::Triangles};

public:
    NguoiChoi(sf::Vector2f viTriBanDau);
    virtual ~NguoiChoi() = default;

    // deltaTime dùng để đi đúng tốc độ trên mọi máy. Gọi không tham số thì coi như 1/60 giây (bản cũ).
    void XuLyPhim(float deltaTime = 1.f / 60.f);
    void CapNhat(float deltaTime) override;
    void Ve(sf::RenderWindow& window) override;

    bool TanCong();
    void Chay(bool kichHoat);
    bool NhanVatPham(const VatPham& vp);

    sf::FloatRect GetVungTanCong() const;
    bool IsDangTanCong() const { return dangTanCong; }
    sf::FloatRect GetHitBox() const;

    // Vùng chân dùng để xét va chạm với cây, lều, nước
    sf::FloatRect GetBanChan(sf::Vector2f viTriXet) const;
    bool KiemTraVaCham(sf::Vector2f viTriXet) const;
    void DatGioiHan(const sf::FloatRect& vung) { gioiHan = vung; coGioiHan = true; }
    void DatVatCan(const std::vector<sf::FloatRect>* ds) { danhSachVatCan = ds; }
    void SetCoKiem(bool co) { coKiem = co; }
    bool IsCoKiem() const { return coKiem; }
    void HoiMau(int luong);
    // Gọi khi người chơi bị khóa điều khiển (đang thoại, đang chiếu phim) để dừng hoạt ảnh chạy bộ
    void DungLai() { dangDiChuyen = false; dangChayNhanh = false; }

    float GetTheLuc() const { return theLuc; }
    float GetTheLucToiDa() const { return theLucToiDa; }
    TuDo& GetTuDo() { return tuDo; }
    const TuDo& GetTuDo() const { return tuDo; }

    int GetHuongNhin() const { return huongNhin; }
};


