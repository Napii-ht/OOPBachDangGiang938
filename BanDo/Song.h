
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

enum class TrangThaiThuyTrieu {
    CAO,         // Thủy triều dâng cao: nước ngập phủ cọc ngầm
    DANG_XUONG,  // Thủy triều bắt đầu rút mạnh
    THAP         // Thủy triều rút cạn: bãi cọc nhô cao sắc nhọn
};

class Song {
private:
    sf::FloatRect vungSong;           // Phạm vi dòng sông Bạch Đằng
    TrangThaiThuyTrieu trangThai;
    float tiLeThuyTrieu;              // 1.0 = cao nhất, 0.0 = thấp nhất
    float thoiGianRut;                // Thời gian đếm nhịp rút triều
    float dongChayOffset;             // Offset cuộn sóng nước

    // Các hạt bọt nước lăn tăn trên bề mặt
    struct BotNuoc {
        sf::Vector2f viTri;
        float tocDo;
        float banKinh;
        float doMo;
    };
    std::vector<BotNuoc> danhSachBotNuoc;

    // ---------------- Hiệu ứng hình ảnh (mới) ----------------
    float thoiGian = 0.f;             // Đồng hồ nội bộ của sông
    float phaDong = 0.f;              // Pha cuộn của sóng (tăng nhanh hơn khi triều rút)
    float toaDoAnhNang = -1.f;        // X của vệt nắng trên mặt nước (<0: mặc định 72% bề ngang)
    float henGonTron = 1.f;           // Đếm ngược để sinh gợn tròn (cá đớp mồi)

    struct GonTron {                  // Gợn nước tròn lan rộng rồi tan
        sf::Vector2f viTri;
        float tuoi;
        float tuoiMax;
        float banKinh;
    };
    std::vector<GonTron> danhSachGonTron;

    struct LongLanh {                 // Điểm nắng lấp lánh trên mặt nước
        float lech;                   // Độ lệch ngang chuẩn hóa quanh vệt nắng [-1,1]
        float v;                      // Vị trí dọc chuẩn hóa [0,1]
        float pha;
        float tocDo;
        float rong;
    };
    std::vector<LongLanh> danhSachLongLanh;

    struct VungBun {                  // Vũng nước đọng trên bãi bùn lúc triều rút
        float x;
        float v;                      // Vị trí trong dải bùn [0,1]
        float rong;
        bool phiaBac;
    };
    std::vector<VungBun> danhSachVungBun;

    sf::VertexArray mangDinh{sf::PrimitiveType::Triangles};  // Toàn bộ mặt sông gom thành 1 draw call

public:
    Song(sf::FloatRect phamVi = sf::FloatRect({0.f, 150.f}, {1200.f, 400.f}));
    ~Song() = default;

    void CapNhat(float deltaTime);
    void Ve(sf::RenderWindow& window);

    void SetTrangThaiThuyTrieu(TrangThaiThuyTrieu tt);
    TrangThaiThuyTrieu GetTrangThaiThuyTrieu() const { return trangThai; }
    float GetTiLeThuyTrieu() const { return tiLeThuyTrieu; }
    void BatDauTrieuRut(float thoiGian = 8.0f);
    // Đặt mực nước trực tiếp (0 = cạn hẳn, 1 = triều cao nhất), dùng khi nước lên xuống theo kịch bản
    void DatTiLeThuyTrieu(float tiLe);

    sf::FloatRect GetVungSong() const { return vungSong; }
    bool OTrongVungSong(sf::Vector2f pos) const { return vungSong.contains(pos); }

    // Mới: vị trí X của mặt trời để vệt nắng trên sông khớp với bầu trời
    void SetToaDoAnhNang(float x) { toaDoAnhNang = x; }
    float GetToaDoAnhNang() const {
        return toaDoAnhNang < 0.f ? vungSong.position.x + vungSong.size.x * 0.72f : toaDoAnhNang;
    }
};


