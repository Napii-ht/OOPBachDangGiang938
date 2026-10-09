#pragma once

#include <string>

class VatPham
{
private:
    int maVatPham;
    std::string ten;

public:
    VatPham(int ma, const std::string& tenVatPham);

    int GetMaVatPham() const;
    const std::string& GetTen() const;
};