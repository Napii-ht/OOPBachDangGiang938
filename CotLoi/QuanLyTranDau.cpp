#include "QuanLyTranDau.h"

QuanLyTranDau::QuanLyTranDau()
    : trangThai(TrangThaiTranDau::CHUAN_BI),
      thoiGianTrongTrangThai(0.f)
{
}

void QuanLyTranDau::CapNhat(float dt)
{
    thoiGianTrongTrangThai += dt;
}

bool QuanLyTranDau::ChuyenTrangThai(TrangThaiTranDau moi)
{
    // Cac trang thai xep theo thu tu, chi cho di toi (so lon hon), khong cho lui lai
    if (static_cast<int>(moi) <= static_cast<int>(trangThai)) return false;

    TrangThaiTranDau cu = trangThai;
    trangThai = moi;
    thoiGianTrongTrangThai = 0.f;

    for (const HamKhiDoiTrangThai& ham : danhSachLangNghe) ham(cu, moi);   // bao cho moi nguoi dang ky
    return true;
}

bool QuanLyTranDau::TiepTheo()
{
    if (IsKetThuc()) return false;
    return ChuyenTrangThai(static_cast<TrangThaiTranDau>(static_cast<int>(trangThai) + 1));
}

void QuanLyTranDau::DatLai()
{
    TrangThaiTranDau cu = trangThai;
    trangThai = TrangThaiTranDau::CHUAN_BI;
    thoiGianTrongTrangThai = 0.f;
    for (const HamKhiDoiTrangThai& ham : danhSachLangNghe) ham(cu, trangThai);
}

void QuanLyTranDau::DangKyLangNghe(HamKhiDoiTrangThai ham)
{
    danhSachLangNghe.push_back(std::move(ham));
}
