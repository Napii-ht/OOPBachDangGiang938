#pragma once

#include <vector>
#include "ManChoiSong.h"
#include "../ChienDau/DoiThuyenGiac.h"

// Chương 5: Triều rút
// Bản đồ: cửa sông Bạch Đằng, nước đang rút dần. Hạm đội Nam Hán đã ở phía thượng nguồn bãi cọc.
// Cách chơi: giữ chân giặc cho tới khi nước rút xuống vùng "cọc đâm thủng được thuyền" (thanh đo chuyển xanh),
// rồi nhấn Space phát hiệu lệnh. Quá sớm thì giặc chạy thoát qua cọc còn chìm, quá muộn thì giặc thấy cọc mà tránh.
class ManChuong5 : public ManChoiSong {
private:
    enum MaNhiemVu {
        NV_GIU_CHAN = 1,
        NV_PHAT_HIEU_LENH
    };

    enum class GiaiDoan {
        GIU_CHAN,       // Nước đang rút, giữ giặc ở phía tây bãi cọc
        HOANG_LOAN,     // Đã phát hiệu lệnh: giặc bỏ chạy ra biển, đâm vào cọc
        KET_THUC
    };

    // Mũi tên từ quân mai phục hai bờ bắn vào thuyền giặc mắc cọc
    struct TenMaiPhuc {
        sf::Vector2f dau;
        sf::Vector2f cuoi;
        float tuoi;
        float toiDa;
    };

    static constexpr float TIEN_DO_RUT = 1.f / 45.f;     // Tốc độ rút của nước mỗi giây (hết trong 45 giây)
    static constexpr float MUC_NUOC_THAP_NHAT_DUNG = 0.60f;   // Từ mức này trở xuống cọc đâm thủng được thuyền
    static constexpr float MUC_NUOC_QUA_MUON = 0.20f;         // Thấp hơn mức này giặc nhìn thấy cọc mà tránh

    static constexpr int SO_THUYEN_GIAC = 4;
    static constexpr int SO_THUYEN_MAC_COC_DAT = 3;

    DoiThuyenGiac doi;
    GiaiDoan giaiDoan;
    std::vector<TenMaiPhuc> cacTenMaiPhuc;
    float demTenMaiPhuc;

    bool daThoaiMoDau;
    bool hieuLenhDungLuc;
    bool hieuLenhSom;
    bool daBaoCuaSoMo;
    float thoiGianHoangLoan;
    float thoiGianChamLai;     // Làm chậm thời gian vài giây khi thuyền đầu tiên đâm cọc
    int soMacCocDaBao;

    void DatNhiemVu();
    void PhatHieuLenh();
    void CapNhatGiuChan(float deltaTime);
    void CapNhatHoangLoan(float deltaTime);
    void XuLyKetQuaDoi(const KetQuaDoiThuyen& ketQua);
    void CapNhatTenMaiPhuc(float deltaTime);
    void KetThucThanhCong();
    int MaNhiemVuHienTai() const { return quanLyNhiemVu.GetMaNhiemVuHienTai(); }
    bool TrongCuaSo() const { return tiLeTrieu <= MUC_NUOC_THAP_NHAT_DUNG && tiLeTrieu >= MUC_NUOC_QUA_MUON; }

    void VeBangThongTin() const;
    void VeThanhNuoc() const;

protected:
    void CapNhatRieng(float deltaTime) override;
    void ThemMucVeRieng(std::vector<std::pair<float, std::function<void()>>>& danhSach) override;
    void VeThongTinPhu() override;
    void VeTrongTheGioi() override;
    sf::Vector2f ViTriMayAnhTheoDoi() const override;

public:
    explicit ManChuong5(NguCanhGame& ngu);

    void XuLySuKien(const sf::Event& suKien) override;

    // Dùng khi kiểm thử
    DoiThuyenGiac& GetDoi() { return doi; }
    float GetTiLeTrieu() const { return tiLeTrieu; }
};
