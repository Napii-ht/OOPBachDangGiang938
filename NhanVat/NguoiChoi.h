#pragma once
#include <functional>
#include <string>
#include <vector>
#include "NhanVat.h"
#include "../DoiTuong/TuDo.h"
#include "../DoiTuong/VatPham.h"

// NguoiChoi ke thua NhanVat: co them the luc, chay, tan cong, tuong tac, va cham, nhat do.
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

    TuDo tuDo;

    // ----- Giai doan 3: va cham + nhat do -----
    sf::FloatRect gioiHanBanDo;                          // khong cho di ra ngoai khung nay
    const std::vector<sf::FloatRect>* danhSachVatCan;    // cac vat can (tuong, nuoc sau...) do BanDo cua Yen cung cap
    std::function<void(int, const std::string&)> khiNhatVatPham;  // "moc noi" sang TuDo cua Yen

    sf::FloatRect TaoHitBoxTai(const sf::Vector2f& tam) const;   // hop va cham neu dung o vi tri 'tam'
    bool BiChanTai(const sf::Vector2f& tam) const;               // dung o 'tam' thi co bi chan khong?

public:
    NguoiChoi(float x, float y);

    bool NhatCoc(int maVatPham);
    int GetSoLuongCoc() const;
    bool DaThuThapDu3Coc() const;

    void XuLyPhim();                                              // doc ban phim
    void DiChuyen(const sf::Vector2f& huong, float dt) override;  // ghi de: co chay + va cham
    void Chay(float dt);                                          // tinh the luc / che do chay
    void TanCong();
    void TuongTac();
    void Chet();                                                  // = HySinh(), dung ten theo tai lieu nhom
    void CapNhat(float dt) override;
    void Ve(sf::RenderWindow& cuaSo) override;

    // ----- Va cham -----
    bool KiemTraVaCham(const sf::FloatRect& khac) const;          // hop cua nguoi choi co cham 'khac' khong?
    void DatGioiHanBanDo(const sf::FloatRect& gioiHan)            { gioiHanBanDo = gioiHan; }
    void DatDanhSachVatCan(const std::vector<sf::FloatRect>* ds)  { danhSachVatCan = ds; }

    // ----- Nhat do -----
    bool NhatVatPham(int maVatPham, const std::string& ten);      // goi khi nhat duoc do (vd coc go)
    void DatKhiNhatVatPham(std::function<void(int, const std::string&)> f) { khiNhatVatPham = std::move(f); }

    // ----- Getter (cac lop khac doc trang thai nguoi choi) -----
    float        GetTheLuc() const        { return theLuc; }
    float        GetTheLucToiDa() const   { return theLucToiDa; }
    float        GetPhamViTanCong() const { return phamViTanCong; }
    sf::Vector2f GetHuongNhin() const     { return huongNhin; }
    // Vung danh = hinh vuong quanh nguoi choi, canh = 2 * phamViTanCong.
    // Dua thang cho HeThongChienDau::XuLyPlayerTanCong(vungTanCong, satThuong, playerDangChem)
    sf::FloatRect GetVungTanCong() const {
        return sf::FloatRect(viTri - sf::Vector2f(phamViTanCong, phamViTanCong),
                             sf::Vector2f(phamViTanCong * 2.f, phamViTanCong * 2.f));
    }
    bool IsDangChay() const      { return dangChay; }
    bool IsDangTanCong() const   { return dangTanCong; }
    bool IsVuaTanCong() const    { return vuaTanCong; }    // HeThongChienDau dung de tru mau dung 1 lan / don danh
    bool IsDaBamTuongTac() const { return daTuongTac; }    // NPC / coc / thuyen se hoi cai nay
};
