#include "TuDo.h"

bool TuDo::ThemVatPham(const VatPham& vatPham)
{
    if (SoLuongVatPham() >= sucChuaToiDa)
        return false;

    if (KiemTraVatPham(vatPham.GetMaVatPham()))
        return false;

    danhSachVatPham.push_back(vatPham);
    return true;
}

bool TuDo::XoaVatPham(int maVatPham)
{
    for (auto it = danhSachVatPham.begin();
         it != danhSachVatPham.end(); ++it)
    {
        if (it->GetMaVatPham() == maVatPham)
        {
            danhSachVatPham.erase(it);
            return true;
        }
    }

    return false;
}

bool TuDo::KiemTraVatPham(int maVatPham) const
{
    for (const auto& vatPham : danhSachVatPham)
    {
        if (vatPham.GetMaVatPham() == maVatPham)
            return true;
    }

    return false;
}

int TuDo::SoLuongVatPham() const
{
    return static_cast<int>(danhSachVatPham.size());
}

int TuDo::GetSucChuaToiDa() const
{
    return sucChuaToiDa;
}