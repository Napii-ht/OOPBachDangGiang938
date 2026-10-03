#pragma once
#include "NhanVat.h"

// NguoiChoi ke thua NhanVat: co them the luc, chay, tan cong, tuong tac.
class NguoiChoi : public NhanVat {
private:
    // ----- Thuoc tinh rieng cua nguoi choi -----
    float theLuc;          // the luc hien tai
    float theLucToiDa;
    float tocDoChay;       // toc do khi giu Shift
    float phamViTanCong;   // ban kinh danh (pixel)

    // ----- Trang thai dieu khien (cap nhat moi khung hinh) -----
    sf::Vector2f huongDi;      // huong bam phim hien tai, vd (1,0) = sang phai
    sf::Vector2f huongNhin;    // huong nhin cuoi cung
    float thoiGianHoiChieu;    // cooldown danh (giay con lai)
    float thoiGianTanCong;     // thoi gian "dang danh" con lai

    bool dangChay;
    bool muonChay;             // dang giu Shift?
    bool kietSuc;              // het the luc -> phai hoi lai moi chay duoc
    bool dangTanCong;          // true trong 0.2 giay cua don danh
    bool vuaTanCong;           // true DUNG 1 khung hinh luc vua bam danh
    bool daTuongTac;           // true DUNG 1 khung hinh khi bam E

    bool phimETruoc;           // de phat hien "vua bam" (chong giu phim)
    bool phimCachTruoc;

public:
    NguoiChoi(float x, float y);

    void XuLyPhim();                                              // doc ban phim
    void DiChuyen(const sf::Vector2f& huong, float dt) override;  // ghi de: co chay
    void Chay(float dt);                                          // tinh the luc / che do chay
    void TanCong();
    void TuongTac();
    void CapNhat(float dt) override;
    void Ve(sf::RenderWindow& cuaSo) override;

    // ----- Getter (cac lop khac doc trang thai nguoi choi) -----
    float        GetTheLuc() const        { return theLuc; }
    float        GetTheLucToiDa() const   { return theLucToiDa; }
    float        GetPhamViTanCong() const { return phamViTanCong; }
    sf::Vector2f GetHuongNhin() const     { return huongNhin; }
    bool IsDangChay() const      { return dangChay; }
    bool IsDangTanCong() const   { return dangTanCong; }
    bool IsVuaTanCong() const    { return vuaTanCong; }    // HeThongChienDau dung de tru mau dung 1 lan / don danh
    bool IsDaBamTuongTac() const { return daTuongTac; }    // NPC / coc / thuyen se hoi cai nay
};
