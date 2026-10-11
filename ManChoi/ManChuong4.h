#pragma once

#include <vector>
#include "ManChoiSong.h"
#include "../ChienDau/DoiThuyenGiac.h"

// Chương 4: Nhử địch vào sâu
// Bản đồ: cửa sông Bạch Đằng lúc triều cao, bãi cọc đã dựng xong và nằm chìm dưới nước.
// Cách chơi: lái thuyền nhẹ ra khiêu chiến rồi giả thua. Giữ thuyền giặc ở khoảng cách vừa đủ
// để chúng đuổi theo, dẫn cả hạm đội ngược sông qua khỏi bãi cọc, tránh tên và né thuyền chiến.
class ManChuong4 : public ManChoiSong {
private:
    enum MaNhiemVu {
        NV_KHIEU_CHIEN = 1,
        NV_NHU_GIAC
    };

    static constexpr int SO_THUYEN_GIAC = 4;

    DoiThuyenGiac doi;
    std::vector<bool> daQuaBaiCoc;
    int soThuyenDaQua;
    bool daThoaiMoDau;
    bool daBaoKetThuc;

    void DatNhiemVu();
    void XuLyKetQuaDoi(const KetQuaDoiThuyen& ketQua);
    void KiemTraThuyenQuaBaiCoc();
    void KhieuKhich();
    void VeBangThongTin() const;
    int MaNhiemVuHienTai() const { return quanLyNhiemVu.GetMaNhiemVuHienTai(); }

protected:
    void CapNhatRieng(float deltaTime) override;
    void ThemMucVeRieng(std::vector<std::pair<float, std::function<void()>>>& danhSach) override;
    void VeThongTinPhu() override;

public:
    explicit ManChuong4(NguCanhGame& ngu);

    void XuLySuKien(const sf::Event& suKien) override;

    // Dùng khi kiểm thử
    DoiThuyenGiac& GetDoi() { return doi; }
    int GetSoThuyenDaQua() const { return soThuyenDaQua; }
};
