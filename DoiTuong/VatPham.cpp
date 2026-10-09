#include "VatPham.h"

VatPham::VatPham(int ma, const std::string& tenVatPham)
    : maVatPham(ma), ten(tenVatPham)
{
}

int VatPham::GetMaVatPham() const
{
    return maVatPham;
}

const std::string& VatPham::GetTen() const
{
    return ten;
}