#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// =============================================================================
//  ThuyenKeDich.h - Chiến thuyền Nam Hán & Soái hạm Lưu Hoằng Tháo
//  Phân hệ phụ trách: Nguyễn Minh Nhân (Kẻ địch, AI & Hệ thống chiến đấu)
// =============================================================================
class ThuyenKeDich {
private:
    sf::Vector2f viTri;
    sf::Vector2f huongDiChuyen;
    float tocDo;
    int sucKhoe;
    int sucKhoeToiDa;
    bool daMacCoc;
    bool daBiPhaHuy;
    float chieuDai;
    float chieuRong;
    float gocXoay;
    float thoiDiemBiTrung;

    std::string tenThuyen;
    float thoiGianKhoiBoc;
    bool thuyenSoChiHuy;
    float huongMat;
    float sxLat;

public:
    ThuyenKeDich(sf::Vector2f viTriBanDau, bool soChiHuy = false);
    ~ThuyenKeDich() = default;

    void DuoiTheo(sf::Vector2f viTriThuyenPlayer, float deltaTime);
    void CapNhat(float deltaTime);
    void Ve(sf::RenderWindow& window);

    void NhanSatThuong(int damage);
    sf::Vector2f GetViTri() const { return viTri; }
    void SetViTri(sf::Vector2f pos) { viTri = pos; }
    sf::FloatRect GetHitBox() const;

    bool IsDaMacCoc() const { return daMacCoc; }
    void SetMacCoc(bool mac) { daMacCoc = mac; if (mac) tocDo = 0.f; }
    bool IsDaBiPhaHuy() const { return daBiPhaHuy; }

    int GetSucKhoe() const { return sucKhoe; }
    int GetSucKhoeToiDa() const { return sucKhoeToiDa; }
    void DatTocDo(float giaTri) { tocDo = giaTri; }
    float GetTocDo() const { return tocDo; }
    bool IsThuyenChiHuy() const { return thuyenSoChiHuy; }
};
