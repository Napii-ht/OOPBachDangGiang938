
#pragma once

enum class TrangThaiThuyTrieu
{
    CAO,
    DANG_XUONG,
    THAP
};

class ThuyTrieu
{
private:
    TrangThaiThuyTrieu trangThai;
    float tiLeRutNuoc = 0.f;
    float thoiGianRutNuoc = 20.f;

public:
    ThuyTrieu();

    TrangThaiThuyTrieu GetTrangThai() const;
    void DatTrangThai(TrangThaiThuyTrieu trangThaiMoi);

    void CapNhat(float deltaTime);
    float GetTiLeRutNuoc() const;
    void BatDauRutNuoc();
};