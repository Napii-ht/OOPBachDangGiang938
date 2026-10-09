#pragma once
#include <functional>
#include <vector>
#include "TrangThaiTranDau.h"

// Quan ly "tran dau dang o giai doan nao".
// Cac nguoi khac KHONG tu y doi trang thai bang tay lung tung, ma goi ChuyenTrangThai() khi dieu kien cua ho dat.
// Ai can biet khi trang thai doi (Ngan phat canh phim, Yen cho thuy trieu xuong...) thi DangKyLangNghe().
class QuanLyTranDau {
public:
    // Ham se duoc goi moi khi doi trang thai: (trang thai cu, trang thai moi)
    using HamKhiDoiTrangThai = std::function<void(TrangThaiTranDau, TrangThaiTranDau)>;

private:
    TrangThaiTranDau trangThai;
    float thoiGianTrongTrangThai;                 // so giay da o trong trang thai hien tai
    std::vector<HamKhiDoiTrangThai> danhSachLangNghe;

public:
    QuanLyTranDau();

    void CapNhat(float dt);                                 // cong don thoi gian
    bool ChuyenTrangThai(TrangThaiTranDau moi);             // chi cho di TOI, khong cho lui. Tra ve true neu doi duoc
    bool TiepTheo();                                        // sang trang thai ke tiep
    void DatLai();                                          // ve CHUAN_BI (choi lai)
    void DangKyLangNghe(HamKhiDoiTrangThai ham);

    TrangThaiTranDau GetTrangThai() const          { return trangThai; }
    bool Is(TrangThaiTranDau tt) const             { return trangThai == tt; }
    bool IsKetThuc() const                         { return trangThai == TrangThaiTranDau::CHIEN_THANG; }
    float GetThoiGianTrongTrangThai() const        { return thoiGianTrongTrangThai; }
};
