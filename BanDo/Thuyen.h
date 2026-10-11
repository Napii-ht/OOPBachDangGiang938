
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Coc.h"

class Thuyen {
protected:
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
    float thoiDiemBiTrung = -100.f;  // Mốc dohoa::dongHo() lần trúng đòn gần nhất (hiệu ứng nháy trắng)

public:
    Thuyen(sf::Vector2f viTriBanDau, float dai, float rong, float spd, int mau);
    virtual ~Thuyen() = default;

    virtual void CapNhat(float deltaTime) = 0;
    virtual void Ve(sf::RenderWindow& window) = 0;

    virtual void NhanSatThuong(int damage);
    bool KiemTraVaChamCoc(Coc& coc, float tiLeThuyTrieu);

    sf::Vector2f GetViTri() const { return viTri; }
    void SetViTri(sf::Vector2f pos) { viTri = pos; }
    sf::FloatRect GetHitBox() const;

    bool IsDaMacCoc() const { return daMacCoc; }
    void SetMacCoc(bool mac) { daMacCoc = mac; if (mac) tocDo = 0.f; }
    bool IsDaBiPhaHuy() const { return daBiPhaHuy; }

    int GetSucKhoe() const { return sucKhoe; }
    int GetSucKhoeToiDa() const { return sucKhoeToiDa; }
    // Đổi tốc độ tạm thời (đội thuyền giặc chạy nhanh, chậm theo tình huống)
    void DatTocDo(float giaTri) { tocDo = giaTri; }
    float GetTocDo() const { return tocDo; }
};

// Thuyền nhẹ cảm tử của nghĩa quân Đại Việt (linh hoạt, nhanh nhẹn)
class ThuyenNguoiChoi : public Thuyen {
private:
    float gocLach;
    float huongMat = 1.f;   // +1: mũi thuyền hướng sang phải, -1: sang trái
    float sxLat = 1.f;      // Hệ số lật ngang mượt (chuyển dần từ -1 đến +1 khi quay đầu)

    // Chế độ chèo mượt dùng cho chương 3: có quán tính, đứng yên khi thả phím, chịu dòng chảy
    bool cheDoMuot = false;
    sf::Vector2f vanToc{0.f, 0.f};
    sf::Vector2f huongMuon{0.f, 0.f};
    sf::Vector2f dongChay{0.f, 0.f};
    float heSoToc = 1.f;
    sf::FloatRect gioiHan{{60.f, 180.f}, {1080.f, 340.f}};

public:
    ThuyenNguoiChoi(sf::Vector2f viTriBanDau);

    void DatCheDoMuot(bool bat) { cheDoMuot = bat; }
    void DatGioiHan(const sf::FloatRect& vung) { gioiHan = vung; }
    void DatDongChay(sf::Vector2f v) { dongChay = v; }
    void DatHeSoToc(float k) { heSoToc = k; }
    void DungLai() { huongMuon = {0.f, 0.f}; }   // Thả phím (khi đang thoại, đang chiếu phim)
    sf::Vector2f GetVanToc() const { return vanToc; }
    virtual ~ThuyenNguoiChoi() = default;

    void XuLyPhim();
    void CapNhat(float deltaTime) override;
    void Ve(sf::RenderWindow& window) override;
};

// Chiến thuyền đồ sộ của quân tướng Nam Hán (Lưu Hoằng Thao)
class ThuyenKeDich : public Thuyen {
private:
    std::string tenThuyen;
    float thoiGianKhoiBoc;
    bool thuyenSoChiHuy;
    float huongMat = -1.f;  // Mũi chiến thuyền mặc định hướng sang trái (như bản cũ)
    float sxLat = -1.f;

public:
    ThuyenKeDich(sf::Vector2f viTriBanDau, bool soChiHuy = false);
    virtual ~ThuyenKeDich() = default;

    void DuoiTheo(sf::Vector2f viTriThuyenPlayer, float deltaTime);
    void CapNhat(float deltaTime) override;
    void Ve(sf::RenderWindow& window) override;

    bool IsThuyenChiHuy() const { return thuyenSoChiHuy; }
    void SetDangChay(bool chay) { dangChay = chay; }
    bool IsDangChay() const { return dangChay; }
private:
    bool dangChay = false;
};


