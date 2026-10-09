#include "ThuyTrieu.h"

ThuyTrieu::ThuyTrieu()
    : trangThai(TrangThaiThuyTrieu::CAO)
{
}

TrangThaiThuyTrieu ThuyTrieu::GetTrangThai() const
{
    return trangThai;
}

void ThuyTrieu::DatTrangThai(
    TrangThaiThuyTrieu trangThaiMoi
)
{
    trangThai = trangThaiMoi;
}