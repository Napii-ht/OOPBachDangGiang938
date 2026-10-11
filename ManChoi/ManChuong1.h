#pragma once

#include <set>
#include <vector>
#include "ManChoiDiBo.h"

// Chương 1: Hội kiến Ngô Quyền
// Bản đồ: doanh trại Từ Lương Xâm trên đồng cỏ ven sông (cuộn theo nhân vật).
// Cách chơi: đi bộ, nói chuyện, nhận kiếm, luyện kiếm trên bù nhìn.
class ManChuong1 : public ManChoiDiBo {
private:
    // Mã nhiệm vụ trong chương này, dùng để biết đang ở bước nào (không dùng số cứng)
    enum MaNhiemVu {
        NV_VAO_DOANH_TRAI = 1,
        NV_HOI_KIEN,
        NV_NHAN_KIEM,
        NV_LUYEN_KIEM,
        NV_BAO_CAO
    };

    enum MaVatPham {
        ID_KIEM = 2
    };

    std::vector<int> chiSoBuNhin;       // Vị trí các bù nhìn trong danhSachVat
    std::set<int> daChamTrongNhatNay;   // Mỗi bù nhìn chỉ tính một lần cho mỗi nhát chém
    sf::FloatRect khuDoanhTrai;         // Bước vào vùng này là tính đã vào doanh trại
    bool daHienHuongDan;
    bool daNhacDiLuyen;

    void DungBanDo();
    void DatNhiemVu();
    void DatNhanVatVaTuongTac();
    void XuLyChemBuNhin();
    int MaNhiemVuHienTai() const { return quanLyNhiemVu.GetMaNhiemVuHienTai(); }

protected:
    void CapNhatRieng(float deltaTime) override;
    void VeNenDong() override;

public:
    explicit ManChuong1(NguCanhGame& ngu);
};
