#include "BaiCoc.h"

BaiCoc::BaiCoc(int ma, sf::Vector2f viTriBanDau)
    : maVatPham(ma),
      viTri(viTriBanDau),
      dangLo(false),
      daDuocNhat(false)
{
    hinhDang.setSize({12.f, 30.f});
    hinhDang.setOrigin({6.f, 15.f});
    hinhDang.setPosition(viTri);
    hinhDang.setFillColor(sf::Color(105, 67, 38));
}

int BaiCoc::GetMaVatPham() const
{
    return maVatPham;
}

sf::Vector2f BaiCoc::GetViTri() const
{
    return viTri;
}

sf::FloatRect BaiCoc::GetHitBox() const
{
    return hinhDang.getGlobalBounds();
}

bool BaiCoc::DangLo() const
{
    return dangLo;
}

bool BaiCoc::DaDuocNhat() const
{
    return daDuocNhat;
}

void BaiCoc::DatDangLo(bool lo)
{
    dangLo = lo;
}

bool BaiCoc::NhatCoc()
{
    if (!dangLo || daDuocNhat)
        return false;

    daDuocNhat = true;
    return true;
}

void BaiCoc::Ve(sf::RenderWindow& cuaSo) const
{
    if (dangLo && !daDuocNhat)
        cuaSo.draw(hinhDang);
}