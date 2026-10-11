#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "../CotLoi/ManChoi.h"
#include "../CotLoi/TienIch.h"
#include "../NhanVat/NguoiChoi.h"
#include "../NhanVat/NhanVatPhu.h"
#include "../NhiemVu/QuanLyNhiemVu.h"
#include "../GiaoDien/QuanLyGiaoDien.h"
#include "../CanhPhim/QuanLyCanhPhim.h"
#include "../ChienDau/HeThongChienDau.h"
#include "../BanDo/MayAnh.h"
#include "../BanDo/VatTrangTri.h"

// Một câu thoại trong đoạn hội thoại có kịch bản
struct CauThoai {
    std::string nguoiNoi;
    std::string noiDung;
    LoaiNPC loai;
};

// Điểm người chơi đứng gần rồi nhấn E để tương tác (nói chuyện, nhặt đồ, rèn cọc...)
struct DiemTuongTac {
    sf::Vector2f viTri;
    float banKinh;
    std::string goiY;                  // Hiện khi dùng được, ví dụ "Trò chuyện"
    std::string goiYKhiKhoa;           // Hiện mờ khi chưa dùng được (để trống thì không hiện gì)
    float docCao;                      // Gợi ý nằm cao hơn điểm bao nhiêu pixel
    std::function<bool()> dangMo;      // Trả về true nếu đang dùng được. Để trống nghĩa là luôn mở.
    std::function<void()> hanhDong;
};

// Chữ nổi lên rồi mờ dần ở vị trí trong thế giới, ví dụ "+1 Gỗ lim"
struct ChuBay {
    sf::Vector2f viTri;
    std::string noiDung;
    sf::Color mau;
    float tuoi;
    float toiDa;
};

// Lớp cơ sở cho các chương đi bộ trên bản đồ lớn (chương 1 và 2):
// camera cuộn, vật cản, NPC, hội thoại có kịch bản, điểm tương tác, nhiệm vụ, màn kết chương.
// Chương cụ thể kế thừa rồi dựng bản đồ, nhân vật, nhiệm vụ và logic riêng.
class ManChoiDiBo : public ManChoi {
protected:
    sf::RenderWindow& window;
    NguoiChoi nguoiChoi;
    MayAnh mayAnh;
    QuanLyNhiemVu quanLyNhiemVu;
    QuanLyGiaoDien quanLyGiaoDien;
    QuanLyCanhPhim quanLyCanhPhim;
    HeThongChienDau heThongChienDau;

    sf::Vector2f kichThuocTheGioi;
    sf::RenderTexture nenBanDo;                          // Nền tĩnh (cỏ, đường, sông) vẽ một lần
    std::vector<VatTrangTri> danhSachVat;
    std::vector<sf::FloatRect> danhSachVatCan;
    std::vector<std::unique_ptr<NhanVatPhu>> danhSachNPC;
    std::vector<DiemTuongTac> danhSachDiemTuongTac;
    std::vector<ChuBay> danhSachChuBay;

    float thoiGian;
    float thoiGianKhoaDieuKhien;                        // Đang bận (rèn cọc, mới đóng thoại): chưa cho đi

    // Hội thoại có kịch bản
    std::vector<CauThoai> hangDoiThoai;
    size_t chiSoCauThoai;
    std::function<void()> khiThoaiXong;

    // Thông báo nhanh ở giữa phía trên màn hình
    std::string thongBao;
    float thoiGianThongBao;
    sf::Vector2f viTriThongBao;                         // Tâm hộp thông báo, lớp con đổi được khi giao diện chật

    // Kết thúc chương
    bool daKetThuc;
    KetQuaManChoi ketQuaDuKien;
    std::string tieuDeKet;
    std::string noiDungKet;

    // Chế độ thuyền (chương 3): nhân vật đi bộ không được vẽ và không nhận phím, HUD mặc định ẩn.
    // Lớp con tự điều khiển thuyền và đặt nguoiChoi cùng vị trí với thuyền để điểm tương tác vẫn dùng được.
    bool cheDoThuyen;
    bool duocDieuKhienKhung;                            // Khung hình này người chơi có được điều khiển không

    std::string dongGoiYDieuKhien;                      // Dòng nhắc phím ở góc dưới màn hình
    int chiSoDiemGan;                                   // Điểm tương tác đang được nhắm tới, -1 nếu không có
    bool coDiemGanKhoa;                                 // Điểm gần nhất đang bị khóa (chỉ hiện gợi ý mờ)
    int chiSoDiemKhoaGan;

    // ----- Dành cho lớp con -----
    void KhoiTaoThongSo(float rongTheGioi, float caoTheGioi, sf::Vector2f viTriBatDau);
    void BatDauVeNen();
    void KetThucVeNen();

    int ThemVat(LoaiVat loai, sf::Vector2f viTri, float tiLe = 1.f, int bienThe = 0);
    void CapNhatVatCan();
    NhanVatPhu* ThemNPC(sf::Vector2f viTri, LoaiNPC loai, const std::string& ten,
                        const std::vector<std::string>& loiThoai, bool tuDongTroChuyen = true);
    void ThemDiemTuongTac(const DiemTuongTac& diem) { danhSachDiemTuongTac.push_back(diem); }

    void BatDauHoiThoai(const std::vector<CauThoai>& cacCau, std::function<void()> khiXong = nullptr);
    void HienThongBao(const std::string& noiDung, float thoiGianHien = 4.f);
    void ThemChuBay(sf::Vector2f viTri, const std::string& noiDung, sf::Color mau);
    void BatDauCanhMoDau(const std::string& tieuDe, const std::string& loiDan, float thoiLuong = 5.f);
    void HoanThanhChuong(const std::string& tieuDe, const std::string& noiDung);
    void BaoThatBai(const std::string& loiNhan = "");

    // Cho nhiệm vụ thu thập: lấy số vật phẩm đang có trong túi làm tiến độ
    void DongBoTienDoVoiTuDo(int maVatPham);
    // Hộp chữ nhật có viền dùng cho gợi ý và thông báo
    void VeHopChu(const std::string& noiDung, unsigned int coChu, sf::Vector2f tam, sf::Color mauChu, sf::Color mauNen) const;

    // Dựng nền: các hàm vẽ lên nenBanDo
    static void SonNenCo(sf::RenderTarget& dich, sf::FloatRect vung, std::uint32_t hat, sf::Color mauToi, sf::Color mauSang);
    static void SonDuongDat(sf::RenderTarget& dich, const std::vector<sf::Vector2f>& diem, float rong);
    // Dòng sông chạy dọc cạnh phải của thế giới: bờ cát, bọt sóng, nước nông sang nước sâu
    static void SonSongBenPhai(sf::RenderTarget& dich, sf::FloatRect vungNuoc, std::uint32_t hat);
    void VeAnhNuoc(sf::FloatRect vungNuoc) const;       // Ánh sáng lấp lánh chuyển động trên mặt nước

    // ----- Móc nối cho lớp con -----
    virtual void CapNhatRieng(float deltaTime) = 0;
    virtual void VeNenDong() {}                         // Chi tiết động vẽ ngay trên nền (nước)
    virtual void ThemMucVeRieng(std::vector<std::pair<float, std::function<void()>>>& danhSach) { (void)danhSach; }
    virtual void VeThongTinPhu() {}
    virtual sf::Vector2f ViTriMayAnhTheoDoi() const { return nguoiChoi.GetViTri(); }
    virtual void VeTrongTheGioi() {}                    // Vẽ thêm trong thế giới, sau mọi vật (nước bắn, vùng đánh dấu...)                     // Vẽ thêm lên giao diện (thanh tiến độ rèn cọc...)

public:
    ManChoiDiBo(NguCanhGame& ngu, float rongTheGioi, float caoTheGioi, sf::Vector2f viTriBatDau);
    ~ManChoiDiBo() override = default;

    void XuLySuKien(const sf::Event& suKien) override;
    void CapNhat(float deltaTime) override;
    void Ve() override;

    NguoiChoi& GetNguoiChoi() { return nguoiChoi; }
    QuanLyNhiemVu& GetQuanLyNhiemVu() { return quanLyNhiemVu; }
    bool DaKetThucChuong() const { return daKetThuc; }
    bool DangHoiThoai() const;
};
