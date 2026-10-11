#pragma once

#include <memory>
#include <vector>
#include "ManChoiDiBo.h"
#include "../BanDo/Song.h"
#include "../BanDo/Coc.h"
#include "../BanDo/Thuyen.h"

// Một ô trong bãi cọc: cọc cắm đúng ô này thì mới tính
struct OCoc {
    sf::Vector2f viTri;
    bool daCam;
};

// Lớp cơ sở cho các chương chèo thuyền trên sông Bạch Đằng (chương 3, 4, 5).
// Dựng sẵn: dòng sông cửa Bạch Đằng, hai bờ Vũ Yên và Đình Vũ, bến Từ Lương Xâm, 32 ô của bãi cọc,
// thuyền nhẹ của người chơi, thủy triều và bản đồ nhỏ. Chương cụ thể chỉ thêm luật chơi riêng.
class ManChoiSong : public ManChoiDiBo {
protected:
    static constexpr float RONG_THE_GIOI = 2000.f;
    static constexpr float CAO_THE_GIOI = 1000.f;
    static constexpr int SO_O = 32;
    static inline const sf::FloatRect VUNG_SONG{{0.f, 270.f}, {2000.f, 480.f}};
    static inline const sf::FloatRect KHUNG_BAI_COC{{950.f, 418.f}, {530.f, 222.f}};
    static inline const sf::Vector2f TAM_BAI_COC{1205.f, 530.f};
    static inline const sf::Vector2f DIEM_BOC_COC{380.f, 345.f};
    static constexpr float X_MEP_TAY_BAI_COC = 930.f;    // Thuyền nào có x nhỏ hơn mốc này là đã qua khỏi bãi cọc về phía thượng nguồn

    Song song;
    std::unique_ptr<ThuyenNguoiChoi> thuyen;
    std::vector<OCoc> cacO;
    std::vector<Coc> cocDaCam;
    sf::VertexArray mangBen{sf::PrimitiveType::Triangles};
    float tiLeTrieu;

    ManChoiSong(NguCanhGame& ngu, sf::Vector2f viTriThuyen);

    void DungBoSong();                  // Nền, hai bờ, bến, cây cối, cờ mai phục và 32 ô cọc
    void CamTatCaCoc();                 // Chương 4 và 5: bãi cọc đã dựng xong từ chương 3
    void CapNhatThuyenSong(float deltaTime, bool choPhepLai, sf::Vector2f dongChay, float heSoToc);
    void ThemMucVeSong(std::vector<std::pair<float, std::function<void()>>>& danhSach);
    void VeMinimapSong(const std::vector<sf::Vector2f>& diemDich) const;
    void VeThanh(float x, float y, float rong, float tiLe, sf::Color mauDay, sf::Color mauNen) const;

    void VeNenDong() override { song.Ve(window); }
    sf::Vector2f ViTriMayAnhTheoDoi() const override;

public:
    const Song& GetSong() const { return song; }
    ThuyenNguoiChoi& GetThuyen() { return *thuyen; }
    int GetSoCocDaCam() const { return static_cast<int>(cocDaCam.size()); }
};
