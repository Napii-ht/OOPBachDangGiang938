#pragma once

#include <vector>
#include "VatPham.h"

class TuDo
{
private:
    std::vector<VatPham> danhSachVatPham;
    int sucChuaToiDa = 3;

public:
    bool ThemVatPham(const VatPham& vatPham);
    bool XoaVatPham(int maVatPham);
    bool KiemTraVatPham(int maVatPham) const;

    int SoLuongVatPham() const;
    int GetSucChuaToiDa() const;
};