#pragma once

#include <SFML/Graphics.hpp>

enum class LoaiVat {
    CAY,          // Cây thường, chắn đường
    CAY_LIM,      // Cây lim, có thể đốn lấy gỗ (chương 2)
    BUI,          // Bụi cây thấp, đi xuyên qua được
    DA,           // Tảng đá
    LEU,          // Lều quân (bienThe 0 vải, 1 xanh rêu, 2 lều chỉ huy)
    CHOI_CANH,    // Chòi canh
    DONG_LUA,     // Đống lửa trại, chậu lửa
    GIA_VU_KHI,   // Giá binh khí
    BU_NHIN,      // Bù nhìn tập kiếm
    HANG_RAO,     // Hàng rào tre
    THUNG,        // Thùng gỗ
    LO_REN,       // Lò rèn có đe
    DONG_SAT,     // Đống thanh sắt
    XE_BO,        // Xe bò chở cọc (bienThe 1 là có cọc trên xe)
    COT_CO,       // Cột cờ
    THUYEN_NEO   // Thuyền Đại Việt neo bờ sông
};

// Một vật đặt trên bản đồ (cây, lều, lò rèn...). Vẽ hoàn toàn bằng code.
// viTri là điểm chạm đất ở giữa chân vật, dùng để sắp xếp trước sau theo trục Y.
class VatTrangTri {
private:
    LoaiVat loai;
    sf::Vector2f viTri;
    float tiLe;
    int bienThe;
    bool daDung;            // Cây đã đốn, đống sắt đã lấy hết, ...
    float thoiGianRung;     // Còn rung bao lâu (khi bị chém hoặc bị đốn)
    float thoiGianDaDung;

public:
    VatTrangTri(LoaiVat loaiVat, sf::Vector2f viTriChan, float tiLeVe = 1.f, int bienTheVe = 0);

    void CapNhat(float deltaTime);
    void Ve(sf::RenderWindow& window, float thoiGian) const;

    // Khung chắn đường (kích thước 0 nếu đi xuyên qua được)
    sf::FloatRect GetKhungVaCham() const;
    // Vùng vật chiếm trên màn hình, dùng để bỏ qua vật nằm ngoài camera
    sf::FloatRect GetKhungHienThi() const;

    void Rung(float thoiGian) { thoiGianRung = thoiGian; }
    void DatDaDung(bool giaTri) { daDung = giaTri; thoiGianDaDung = 0.f; }
    bool IsDaDung() const { return daDung; }
    void SetBienThe(int giaTri) { bienThe = giaTri; }

    LoaiVat GetLoai() const { return loai; }
    sf::Vector2f GetViTri() const { return viTri; }
    float GetYSapXep() const { return viTri.y; }
};
