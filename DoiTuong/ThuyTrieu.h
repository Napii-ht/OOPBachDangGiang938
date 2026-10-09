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

public:
    ThuyTrieu();

    TrangThaiThuyTrieu GetTrangThai() const;
    void DatTrangThai(TrangThaiThuyTrieu trangThaiMoi);
};