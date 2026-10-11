#pragma once

#include "ManChoiSong.h"

// Chương 3: Dựng bãi cọc
// Cách chơi: chèo thuyền chở cọc từ bến Từ Lương Xâm ra lòng sông lúc triều thấp,
// cắm vào các ô của bãi cọc trước khi nước dâng. Dòng chảy cuốn thuyền xuôi về phía biển.
class ManChuong3 : public ManChoiSong {
private:
    enum MaNhiemVu {
        NV_NHAN_LENH = 1,
        NV_BOC_COC,
        NV_CAM_COC,
        NV_BAO_CAO
    };

    enum class GiaiDoan {
        CHOI,
        TRIEU_LEN      // Cảnh phim nước dâng che kín bãi cọc
    };

    static constexpr int SO_COC_TOI_THIEU = 24;     // Cắm được từng này là đạt
    static constexpr int SUC_CHUA = 8;              // Mỗi chuyến thuyền chở tối đa
    static constexpr float THOI_GIAN_TOI_DA = 240.f;
    static constexpr float THOI_GIAN_NUOC_DANG = 45.f;   // Mấy giây cuối nước bắt đầu dâng

    std::vector<std::unique_ptr<ThuyenKeDich>> thuyenGiac;   // Hạm đội giặc hiện ở xa cho căng thẳng

    int soCocTrenThuyen;
    int soCocTrongKho;
    int soDaCam;
    float thoiGianConLai;
    bool daThoaiMoDau;
    bool daBaoGiac;
    GiaiDoan giaiDoan;
    float thoiGianCanhTrieu;

    void DatNhiemVu();
    void DatTuongTac();
    void BocCoc();
    void CamCoc(int chiSoO);
    void CapNhatTrieu();
    void BatDauCanhTrieuLen();
    void KetThucChuong();
    int SoSao() const;
    int ChiSoOGanNhat(sf::Vector2f viTri, float banKinhToiDa) const;
    int MaNhiemVuHienTai() const { return quanLyNhiemVu.GetMaNhiemVuHienTai(); }

    void VeBangThongTin() const;

protected:
    void CapNhatRieng(float deltaTime) override;
    void ThemMucVeRieng(std::vector<std::pair<float, std::function<void()>>>& danhSach) override;
    void VeThongTinPhu() override;
    sf::Vector2f ViTriMayAnhTheoDoi() const override;

public:
    explicit ManChuong3(NguCanhGame& ngu);

    // Dùng khi kiểm thử
    int GetSoDaCam() const { return soDaCam; }
};
