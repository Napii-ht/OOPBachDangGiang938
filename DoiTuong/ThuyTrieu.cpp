
#include "ThuyTrieu.h"
#include <algorithm>

ThuyTrieu::ThuyTrieu()
    : trangThai(TrangThaiThuyTrieu::CAO)
{
}

TrangThaiThuyTrieu ThuyTrieu::GetTrangThai() const
{
    return trangThai;
}

void ThuyTrieu::DatTrangThai(
    TrangThaiThuyTrieu trangThaiMoi)
{
    trangThai = trangThaiMoi;
}

void ThuyTrieu::BatDauRutNuoc()
{
    tiLeRutNuoc = 0.f;
    trangThai = TrangThaiThuyTrieu::DANG_XUONG;
}

void ThuyTrieu::CapNhat(float deltaTime)
{
    if (trangThai != TrangThaiThuyTrieu::DANG_XUONG)
        return;

    tiLeRutNuoc += deltaTime / thoiGianRutNuoc;
    tiLeRutNuoc = std::clamp(tiLeRutNuoc, 0.f, 1.f);

    if (tiLeRutNuoc >= 1.f)
        trangThai = TrangThaiThuyTrieu::THAP;
}

float ThuyTrieu::GetTiLeRutNuoc() const
{
    return tiLeRutNuoc;
}