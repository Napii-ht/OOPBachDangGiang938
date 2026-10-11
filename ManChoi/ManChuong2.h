#pragma once

#include <vector>
#include "ManChoiDiBo.h"

// Chương 2: Chuẩn bị trận địa
// Bản đồ: rừng lim phía tây, kho sắt phía nam, lò rèn phía bắc, xe bò ngoài bờ sông.
// Cách chơi: đốn gỗ lim, lấy sắt, rèn cọc bịt sắt, chất lên xe, đánh lui thám tử Nam Hán.
class ManChuong2 : public ManChoiDiBo {
private:
    enum MaNhiemVu {
        NV_DON_GO = 1,
        NV_LAY_SAT,
        NV_REN_COC,
        NV_CHAT_COC,
        NV_DIET_THAM_TU,
        NV_BAO_TIN
    };

    enum MaVatPham {
        ID_COC = 1,          // Trùng mã với cọc gỗ của màn thử nghiệm
        ID_GO_LIM = 10,
        ID_THANH_SAT = 11
    };

    static constexpr int SO_COC_CAN = 6;
    static constexpr int SO_THAM_TU = 3;
    static constexpr int SO_NHAT_CHAT = 3;   // Số nhát chặt để một cây lim ngã

    std::vector<int> chiSoCayLim;
    std::vector<int> soNhatDaChat;       // Song song với chiSoCayLim
    std::vector<int> chiSoDongSat;
    int chiSoXeBo;
    int chiSoLoRen;

    // Rèn cọc: mỗi cọc mất một khoảng thời gian, người chơi đứng yên
    bool dangRen;
    float tienDoRen;
    float demTiaLuaRen;

    bool daThoaiMoDau;
    bool daSinhThamTu;
    int soThamTuBanDau;

    void DungBanDo();
    void DatNhiemVu();
    void DatTuongTac();
    void BatDauRen();
    void CapNhatRen(float deltaTime);
    void SinhThamTu();
    void CapNhatChienDau(float deltaTime);
    int MaNhiemVuHienTai() const { return quanLyNhiemVu.GetMaNhiemVuHienTai(); }

protected:
    void CapNhatRieng(float deltaTime) override;
    void VeNenDong() override;
    void ThemMucVeRieng(std::vector<std::pair<float, std::function<void()>>>& danhSach) override;

public:
    explicit ManChuong2(NguCanhGame& ngu);
};
