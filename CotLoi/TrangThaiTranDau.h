#pragma once

// Cac giai doan cua tran Bach Dang, di theo THU TU tu tren xuong duoi.
enum class TrangThaiTranDau {
    CHUAN_BI,              // 1:15-2:00  thu thap coc, chuan bi tran dia
    QUAN_DICH_XUAT_HIEN,   // 2:00       tau dich xuat hien (canh phim cua Ngan)
    NHU_DICH,              // 2:00-2:45  nguoi choi lai thuyen nhu dich
    THUY_TRIEU_XUONG,      // 2:45-3:30  thuy trieu ha, coc lo ra (Yen)
    TAU_MAC_COC,           //            tau dich dam vao coc
    PHAN_CONG,             // 3:30-4:30  tong phan cong, danh nhau voi quan Nam Han (Nhan)
    CHIEN_THANG            // 4:30-5:00  ket thuc
};

// Doi ten trang thai thanh chu de in ra / hien tieu de cua so
inline const char* TenTrangThai(TrangThaiTranDau tt)
{
    switch (tt) {
        case TrangThaiTranDau::CHUAN_BI:             return "CHUAN BI";
        case TrangThaiTranDau::QUAN_DICH_XUAT_HIEN:  return "QUAN DICH XUAT HIEN";
        case TrangThaiTranDau::NHU_DICH:             return "NHU DICH";
        case TrangThaiTranDau::THUY_TRIEU_XUONG:     return "THUY TRIEU XUONG";
        case TrangThaiTranDau::TAU_MAC_COC:          return "TAU MAC COC";
        case TrangThaiTranDau::PHAN_CONG:            return "PHAN CONG";
        case TrangThaiTranDau::CHIEN_THANG:          return "CHIEN THANG";
    }
    return "?";
}
