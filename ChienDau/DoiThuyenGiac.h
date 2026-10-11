#pragma once

#include <algorithm>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../BanDo/Thuyen.h"
#include "../BanDo/Coc.h"

// Những việc vừa xảy ra trong một lần cập nhật, để màn chơi phát hiệu ứng và âm thanh tương ứng
struct KetQuaDoiThuyen {
    int soMuiTenTrung = 0;                   // Số mũi tên trúng thuyền người chơi
    bool giacBanTen = false;                 // Có mũi tên mới vừa được bắn
    bool thuyenBiDam = false;                // Thuyền giặc đâm trúng thuyền người chơi
    std::vector<sf::Vector2f> viTriMacCoc;   // Vị trí các thuyền vừa đâm phải cọc
};

// Hạm đội Nam Hán: truy đuổi thuyền nhẹ Đại Việt, bắn tên, hoảng loạn tháo chạy ra biển.
// Dùng chung cho chương 4 (nhử địch) và chương 5 (triều rút).
class DoiThuyenGiac {
public:
    enum class CheDo {
        TIEN_VAO,     // Từ biển tiến vào cửa sông rồi dừng lại dò xét
        DUOI,         // Truy đuổi thuyền người chơi
        HOANG_LOAN    // Bỏ chạy ra biển
    };

private:
    struct MuiTen {
        sf::Vector2f viTri;
        sf::Vector2f van;
        float tuoi;
    };

    std::vector<std::unique_ptr<ThuyenKeDich>> cacThuyen;
    std::vector<float> tocDoDuoi;
    std::vector<float> phaLac;
    std::vector<MuiTen> cacMuiTen;

    CheDo cheDo;
    float doHang;             // 0 đến 100: giặc còn hăng đuổi theo không
    float hoiKhieuKhich;
    float thoiGianTangToc;
    float hoiBan;
    float hoiVaCham;
    float xDungDoXet;
    float heSoTocCoBan;       // Nhân với tốc độ đuổi của mọi thuyền (chương 5 cho giặc đuổi chậm lại)
    bool daToiDiemDung;
    float heSoTocChung;       // Nhân với tốc độ đuổi của mọi thuyền (chương 5 cho giặc chậm lại)
    float nhipBan;            // Số giây giữa hai lần bắn tên

    int ChiSoThuyenGanNhat(sf::Vector2f viTri) const;
    void TachCacThuyenChongNhau();

public:
    DoiThuyenGiac();

    void ThemThuyen(sf::Vector2f viTri, bool soChiHuy);
    void DatCheDo(CheDo c) { cheDo = c; }
    CheDo GetCheDo() const { return cheDo; }
    void DatDiemDungDoXet(float x) { xDungDoXet = x; }
    void DatHeSoTocChung(float k) { heSoTocChung = k; }
    void DatNhipBan(float giay) { nhipBan = giay; }
    void DatHeSoTocCoBan(float k) { heSoTocCoBan = k; }
    bool DaToiDiemDung() const { return daToiDiemDung; }

    // Hô khiêu chiến: giặc hăng hơn tức thì nhưng chạy nhanh hơn một lúc. Trả về false nếu đang hồi.
    bool KhieuKhich();
    float GetHoiKhieuKhich() const { return hoiKhieuKhich; }

    KetQuaDoiThuyen CapNhat(float deltaTime, ThuyenNguoiChoi& nguoi, std::vector<Coc>& coc, float tiLeTrieu);
    void VeMuiTen(sf::RenderWindow& window) const;

    const std::vector<std::unique_ptr<ThuyenKeDich>>& GetThuyen() const { return cacThuyen; }
    float GetDoHang() const { return doHang; }
    void DatDoHang(float giaTri) { doHang = std::max(0.f, std::min(100.f, giaTri)); }
    bool MatHung() const { return doHang <= 0.f; }
    float KhoangCachGanNhat(sf::Vector2f viTri) const;
    int SoThuyenMacCoc() const;
    int SoThuyenDaThoat(float xThoat) const;   // Số thuyền đã chạy ra khỏi khu vực (x lớn hơn xThoat)
};
