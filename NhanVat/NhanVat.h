#pragma once
#include <SFML/Graphics.hpp>

// Lop co so cho MOI nhan vat trong game (NguoiChoi, NhanVatPhu, KeDich...)
// Day la lop TRUU TUONG (co ham thuan ao CapNhat) -> khong tao truc tiep NhanVat duoc,
// chi tao qua lop con. Lop con BAT BUOC viet ham CapNhat().
class NhanVat {
protected:                       // protected: lop con truy cap duoc
    sf::Vector2f viTri;          // vi tri hien tai (tam nhan vat)
    int   sucKhoe;               // mau hien tai
    int   sucKhoeToiDa;          // mau toi da
    int   satThuong;             // sat thuong gay ra moi don danh
    float tocDo;                 // toc do di chuyen (pixel / giay)
    bool  daHySinh;              // true = da chet / guc nga

    sf::RectangleShape hinhDang; // hinh tam thoi (o vuong) - sau nay thay bang Sprite

public:
    NhanVat(float x, float y, float tocDoBanDau, int mauToiDa, int satThuongBanDau);
    NhanVat(sf::Vector2f viTriBanDau, float tocDoBanDau, int mauToiDa, int satThuongBanDau);  // cach goi khac, KeDich dung
    virtual ~NhanVat() = default;   // BAT BUOC virtual khi co ke thua

    // ----- Cac ham chinh -----
    virtual void DiChuyen(const sf::Vector2f& huong, float dt);
    virtual void CapNhat(float dt) = 0;               // thuan ao: lop con BAT BUOC viet
    virtual void Ve(sf::RenderWindow& cuaSo);         // mac dinh ve o vuong, lop con co the ghi de
    virtual void NhanSatThuong(int damage);           // tru mau; het mau thi HySinh()
    virtual void HySinh();

    // ----- Interface cho cac module khac (AI, Map, UI...) -----
    sf::Vector2f  GetViTri() const   { return viTri; }                      // AI dung de duoi theo
    sf::FloatRect GetHitBox() const  { return hinhDang.getGlobalBounds(); } // dung de tinh va cham
    bool          IsDaHySinh() const { return daHySinh; }                   // AI ngung danh khi true

    // ----- Getter / Setter khac -----
    void  SetViTri(float x, float y);
    float GetTocDo() const        { return tocDo; }
    int   GetSucKhoe() const      { return sucKhoe; }
    int   GetSucKhoeToiDa() const { return sucKhoeToiDa; }
    int   GetSatThuong() const    { return satThuong; }
};
