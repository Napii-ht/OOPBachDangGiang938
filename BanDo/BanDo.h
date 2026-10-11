
#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Coc.h"
#include "Song.h"
#include "Thuyen.h"

// Cọc gỗ rơi rải rác trên bờ để người chơi nhặt
struct CocThuThap {
    sf::Vector2f viTri;
    bool daNhat;
};

class BanDo {
private:
    float chieuRong;
    float chieuCao;
    Song song;
    std::vector<Coc> danhSachCoc;
    std::vector<CocThuThap> danhSachCocRoi;

    // ======================= ĐỒ HỌA BẢN ĐỒ =======================
    // Các lớp tĩnh được dựng 1 lần (VertexArray) -> vẽ chỉ vài draw call mỗi khung hình.
    float thoiGian = 0.f;
    sf::VertexArray lopTroi{sf::PrimitiveType::Triangles};  // Bầu trời hoàng hôn, mặt trời, sao
    sf::VertexArray lopBac{sf::PrimitiveType::Triangles};   // Núi đá vôi, đồi rừng, bờ Bắc, doanh trại
    sf::VertexArray lopNam{sf::PrimitiveType::Triangles};   // Bờ Nam: bến thuyền, kho cọc, đường mòn...
    sf::VertexArray lopDong{sf::PrimitiveType::Triangles};  // Bộ đệm cho các vật chuyển động mỗi khung hình

    // Vật thể chuyển động theo gió (cây, tre, lau sậy, cờ, lửa...)
    enum class LoaiVat { CAY_LA, CAY_DA, TRE, CAU, DUOC, BUI, LAU, CO, LUA };
    struct VatDong {
        LoaiVat loai;
        float x;
        float y;          // Điểm chạm đất (đáy vật thể)
        float kichThuoc;  // Hệ số tỉ lệ
        float pha;
        int bienThe;
    };
    std::vector<VatDong> danhSachVat;

    struct May    { float x, y, rong, cao, tocDo, doMo; };
    struct Chim   { float x, y, tocDo, pha, kichThuoc; bool trang; };
    struct DomDom { sf::Vector2f goc; float pha, tocDo, bienDo; };
    struct SuongMu{ float x, y, rong, cao, tocDo, doMo; };
    struct HatSang {
        sf::Vector2f viTri;
        sf::Vector2f van;
        float tuoi, tuoiMax, kichThuoc;
        sf::Color mau;
        int kieu;  // 0 = hạt rơi, 1 = vòng sóng lan, 2 = tia lấp lánh
    };
    std::vector<May> danhSachMay;
    std::vector<Chim> danhSachChim;
    std::vector<DomDom> danhSachDomDom;
    std::vector<SuongMu> danhSachSuong;
    std::vector<HatSang> danhSachHat;

    void DungBauTroi();
    void DungNuiVaBoBac();
    void DungBoNam();
    void KhoiTaoThucVat();
    void KhoiTaoHieuUng();

    void PhatTiaNuoc(sf::Vector2f pos);
    void PhatManhGo(sf::Vector2f pos);
    void PhatLongLanhNhat(sf::Vector2f pos);

    void VeVatDong(sf::RenderWindow& window);
    void VeCocThuThap(sf::RenderWindow& window);
    void VeBongCocDuoiNuoc(sf::RenderWindow& window);
    void VeSuongVaHat(sf::RenderWindow& window);
    void VeTroiDong(sf::RenderWindow& window);

public:
    BanDo(float rong = 1200.f, float cao = 700.f);
    ~BanDo() = default;

    void KhoiTaoBaiCocChuan();
    void KhoiTaoBaiCocRong(float xMin, float xMax, float yMin, float yMax);
    void KhoiTaoCocThuThap(int soLuong = 3);

    void CapNhat(float deltaTime);
    void Ve(sf::RenderWindow& window);

    // Tùy chọn: gọi SAU khi đã vẽ thuyền để có hiệu ứng tiền cảnh
    // (viền tối vignette, tia nắng, cỏ lau tiền cảnh đung đưa)
    void VeTienCanh(sf::RenderWindow& window);

    // Xử lý nhặt cọc gỗ trên bờ
    int KiemTraNhatCoc(sf::Vector2f viTriPlayer, float banKinh = 55.f);
    int GetSoCocChuaNhat() const;

    // Cắm cọc xuống dòng sông
    void ThemCocMoi(sf::Vector2f pos);

    // Quản lý va chạm và tương tác sông nước
    bool XuLyVaChamThuyenVoiBaiCoc(ThuyenKeDich& thuyen);

    Song& GetSong() { return song; }
    const Song& GetSong() const { return song; }
    const std::vector<Coc>& GetDanhSachCoc() const { return danhSachCoc; }
    float GetChieuRong() const { return chieuRong; }
    float GetChieuCao() const { return chieuCao; }
};


