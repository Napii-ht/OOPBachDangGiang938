#pragma once

#include <memory>
#include <vector>
#include "ManChoiDiBo.h"
#include "../BanDo/BanDo.h"
#include "../BanDo/Thuyen.h"
#include "../NhanVat/NhanVatPhu.h"
#include "../CanhPhim/TrinhChieuVideo.h"

// Chương 6: Tổng phản công trên sông Bạch Đằng
// Bản đồ: Hoàng hôn sông Bạch Đằng (BanDo) rộng 2000x1000 với núi đá vôi, vách đất, bãi cọc nhọn.
// Camera cuộn mượt mà theo nhân vật (MayAnh).
// Lối chơi gồm 2 Hồi chiến dịch lịch sử:
// - HỒI 1 (Thủy chiến): Điều khiển thuyền nhẹ Đại Việt bao vây hạm đội Nam Hán mắc cạn giữa bãi cọc.
//   Ném bình dầu hỏa phóng hỏa thiêu rụi chiến hạm địch, ngắm bắn cung yểm trợ đồng đội áp sát.
// - HỒI 2 (Đổ bộ cận chiến): Đổ bộ lên bãi cọc và boong tàu, cận chiến giáp lá cà cùng quân Đại Việt.
//   Chủ soái Ngô Quyền xuất trận sát cánh cùng người chơi quyết chiến hạ gục Chủ tướng Lưu Hoằng Tháo.
class ManChuong6 : public ManChoiDiBo {
public:
    enum class GiaiDoan {
        HOI_1_PHONG_HOA,     // Ném bình dầu cháy vào thuyền giặc
        HOI_1_XA_TIEN,        // Bắn cung ngắm bắn tiêu diệt lính trên thuyền giặc
        CHUYEN_HOI,          // Trống trận triều rút dồn dập, toàn quân đổ bộ
        HOI_2_DOT_1,         // Đợt 1 bộ binh tinh nhuệ
        HOI_2_NGHI_1,        // Nghỉ giữa đợt, hồi máu
        HOI_2_DOT_2,         // Đợt 2 hộ vệ đao khiên
        HOI_2_NGO_QUYEN,     // Ngô Quyền xuất hiện, sát cánh quyết chiến Lưu Hoằng Tháo
        KET_THUC
    };

private:
    enum MaNhiemVu {
        NV_PHONG_HOA = 1,
        NV_XA_TIEN,
        NV_DO_BO_CAN_CHIEN,
        NV_QUYET_CHIEN_HOANG_THAO
    };

    // Bình hỏa công bay parabol
    struct BinhHoaCong {
        sf::Vector2f viTri;
        sf::Vector2f vanToc;
        float doCao;         // Độ cao ảo (Z)
        float vanTocZ;
        float trongLuc;
        sf::Vector2f diemDich; // Tọa độ đích nhắm chính xác
        bool daNo;
        float thoiGianNo;
    };

    // Mũi tên người chơi bắn ra khi ngắm
    struct MuiTenNguoiChoi {
        sf::Vector2f viTri;
        sf::Vector2f vanToc;
        float tuoi;
        bool dangChay;
    };

    // Lính địch đứng trên mạn chiến thuyền giặc trong Hồi 1
    struct LinhTrenThuyen {
        sf::Vector2f viTri;
        int mau;
        int mauToiDa;
        float hoiBan;
        float thoiGianBiThuong; // Nháy trắng khi trúng đòn
        float thoiGianChet;     // Hoạt ảnh ngã gục
        bool daHySinh;
        size_t chiSoThuyen;
    };

    // Mũi tên địch bắn trả thuyền ta
    struct MuiTenDich {
        sf::Vector2f viTri;
        sf::Vector2f vanToc;
        float tuoi;
    };

    // Chiến binh Đại Việt đồng minh
    struct LinhDaiViet {
        std::unique_ptr<NhanVatPhu> hinh;
        float hoiDanh;
        bool laNgoQuyen;
    };

    BanDo banDo;                                               // Bản đồ hoàng hôn sông Bạch Đằng
    std::vector<std::unique_ptr<ThuyenKeDich>> thuyenMacCan;   // Thuyền giặc nằm kẹt giữa bãi cọc
    std::unique_ptr<ThuyenNguoiChoi> thuyenTa;                 // Thuyền nhẹ Đại Việt người chơi lái Hồi 1
    std::vector<BinhHoaCong> cacBinhDau;
    std::vector<MuiTenNguoiChoi> cacMuiTen;
    std::vector<LinhTrenThuyen> cacLinhThuyen;
    std::vector<MuiTenDich> cacTenDich;
    std::vector<LinhDaiViet> quanTa;
    TrinhChieuVideo trinhChieuVideo;

    GiaiDoan giaiDoan;
    float demGiay;
    int soThuyenDaDot;
    int soLinhThuyenDaHa;
    int soDichDaSinh;
    int soDichDaTinhTienDo;
    bool daThoaiMoDau;
    bool chuTuongDaChet;
    float thoiGianChamLai;
    bool dangNgamBan;
    float lucKeoCung;
    float thoiGianHoiNem;

    static constexpr int SO_DICH_DOT_1 = 6;
    static constexpr int SO_DICH_DOT_2 = 8;
    static constexpr int SO_VE_BINH = 3;
    static constexpr float THOI_GIAN_NGHI = 3.f;
    static constexpr float TAM_NEM_TOI_DA = 580.f;

    void KhoiTaoHoi1();
    void KhoiTaoHoi2();
    void DatNhiemVu();
    void DatVatCanMoi();
    bool CoVaCham(sf::Vector2f viTriChan) const;
    void GiuNgoaiVatCan(sf::Vector2f viTriCu, NhanVat& nhanVat) const;
    sf::Vector2f ChonViTriTrong(sf::Vector2f tam, float banKinh, unsigned int& hat) const;

    void NemBinhDau(sf::Vector2f mucTieu);
    void BanMuiTen(sf::Vector2f huong, float luc);
    void CapNhatHoi1(float deltaTime);
    void CapNhatHoi2(float deltaTime);
    void SinhDichDot(int soLuong, int mau, int dame, float tocDo);
    void SinhChuTuongVaNgoQuyen();
    void SinhQuanTa();
    void CapNhatQuanTa(float deltaTime);
    void DongBoTienDo();
    void KetThucDot();
    void BatDauDot(GiaiDoan dot);
    void ChuTuongTuTran();
    void KetThucChuong();
    void VeCungThuBoongTau(sf::RenderWindow& window, const LinhTrenThuyen& l);

protected:
    void CapNhatRieng(float deltaTime) override;
    void VeNenDong() override;
    void ThemMucVeRieng(std::vector<std::pair<float, std::function<void()>>>& danhSach) override;
    void VeTrongTheGioi() override;
    void VeThongTinPhu() override;
    sf::Vector2f ViTriMayAnhTheoDoi() const override;

public:
    explicit ManChuong6(NguCanhGame& ngu);
    ~ManChuong6() override = default;

    void XuLySuKien(const sf::Event& suKien) override;
    void CapNhat(float deltaTime) override;
    void Ve() override;

    GiaiDoan GetGiaiDoan() const { return giaiDoan; }
};

