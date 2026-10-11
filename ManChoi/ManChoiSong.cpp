#include "ManChoiSong.h"
#include "../BanDo/DoHoa.h"
#include <algorithm>
#include <cmath>

using namespace dohoa;

ManChoiSong::ManChoiSong(NguCanhGame& ngu, sf::Vector2f viTriThuyen)
    : ManChoiDiBo(ngu, RONG_THE_GIOI, CAO_THE_GIOI, viTriThuyen),
      song(VUNG_SONG),
      tiLeTrieu(0.f) {
    cheDoThuyen = true;
    song.SetToaDoAnhNang(1500.f);

    thuyen = std::make_unique<ThuyenNguoiChoi>(viTriThuyen);
    thuyen->DatCheDoMuot(true);
    thuyen->DatGioiHan({{40.f, 300.f}, {1920.f, 420.f}});
}

void ManChoiSong::DungBoSong() {
    BatDauVeNen();
    SonNenCo(nenBanDo, {{0.f, 0.f}, {RONG_THE_GIOI, CAO_THE_GIOI}}, 21, sf::Color(52, 100, 50), sf::Color(104, 150, 72));

    // Bờ cát hai bên mép sông, lượn sóng theo nhiễu
    VA va(sf::PrimitiveType::Triangles);
    for (float x = 0.f; x < RONG_THE_GIOI; x += 20.f) {
        float bacA = 252.f + nhieu(x * 0.02f, 1.f, 4) * 12.f;
        float bacB = 252.f + nhieu((x + 20.f) * 0.02f, 1.f, 4) * 12.f;
        tu(va, {x, bacA - 26.f}, {x + 20.f, bacB - 26.f}, {x + 20.f, 272.f}, {x, 272.f},
           mau(206, 188, 140, 0), mau(206, 188, 140, 0), mau(214, 198, 152), mau(214, 198, 152));

        float namA = 770.f - nhieu(x * 0.02f, 7.f, 5) * 12.f;
        float namB = 770.f - nhieu((x + 20.f) * 0.02f, 7.f, 5) * 12.f;
        tu(va, {x, 748.f}, {x + 20.f, 748.f}, {x + 20.f, namB + 26.f}, {x, namA + 26.f},
           mau(214, 198, 152), mau(214, 198, 152), mau(206, 188, 140, 0), mau(206, 188, 140, 0));
    }
    nenBanDo.draw(va);

    // Đất nện quanh bến và doanh trại
    SonDuongDat(nenBanDo, {{140.f, 200.f}, {330.f, 230.f}, {330.f, 262.f}}, 52.f);
    SonDuongDat(nenBanDo, {{140.f, 200.f}, {520.f, 190.f}}, 40.f);
    KetThucVeNen();

    // ----- Cầu bến gỗ nhô ra mặt nước -----
    mangBen.clear();
    const float x0 = 305.f, x1 = 355.f, y0 = 246.f, y1 = 338.f;
    chuNhat(mangBen, x0 + 6.f, y0 + 8.f, x1 - x0, y1 - y0, mau(0, 0, 0, 60));
    for (float y = y0; y < y1; y += 9.f) {
        Color c = (static_cast<int>(y / 9.f) % 2 == 0) ? mau(150, 108, 66) : mau(134, 96, 58);
        chuNhat(mangBen, x0, y, x1 - x0, 8.f, c);
        chuNhat(mangBen, x0, y + 7.f, x1 - x0, 1.2f, mau(84, 58, 36));
    }
    for (float y = y0 + 6.f; y <= y1; y += 30.f) {
        elip(mangBen, x0 - 2.f, y, 4.f, 4.f, mau(104, 72, 44), 8);
        elip(mangBen, x1 + 2.f, y, 4.f, 4.f, mau(104, 72, 44), 8);
    }
    chuNhat(mangBen, x1 - 3.f, y1 - 6.f, 6.f, 12.f, mau(110, 76, 46));

    // ----- Doanh trại Từ Lương Xâm sát bến -----
    ThemVat(LoaiVat::LEU, {170.f, 160.f}, 1.f, 0);
    ThemVat(LoaiVat::LEU, {470.f, 150.f}, 1.f, 1);
    ThemVat(LoaiVat::LEU, {250.f, 90.f}, 1.f, 2);
    ThemVat(LoaiVat::DONG_LUA, {400.f, 200.f});
    ThemVat(LoaiVat::COT_CO, {290.f, 215.f});
    ThemVat(LoaiVat::XE_BO, {215.f, 232.f}, 1.f, 1);
    ThemVat(LoaiVat::THUNG, {455.f, 235.f});
    ThemVat(LoaiVat::THUNG, {480.f, 232.f});

    // ----- Cây cối hai bờ -----
    Rng rng(938u);
    for (int i = 0; i < 70; ++i) {
        float x = rng.range(40.f, 1960.f);
        float y = rng.range(40.f, 215.f);
        if (x > 120.f && x < 540.f && y > 50.f) continue;   // Chừa khu doanh trại
        float k = rng.f();
        if (k < 0.15f) ThemVat(LoaiVat::DA, {x, y + 20.f}, 0.8f + rng.f() * 0.5f);
        else if (k < 0.4f) ThemVat(LoaiVat::BUI, {x, y + 10.f}, 0.9f + rng.f() * 0.4f, rng.irange(0, 5));
        else ThemVat(LoaiVat::CAY, {x, y}, 0.85f + rng.f() * 0.4f, rng.irange(0, 5));
    }
    for (int i = 0; i < 64; ++i) {
        float x = rng.range(40.f, 1960.f);
        float y = rng.range(800.f, 960.f);
        float k = rng.f();
        if (k < 0.15f) ThemVat(LoaiVat::DA, {x, y}, 0.8f + rng.f() * 0.5f);
        else if (k < 0.4f) ThemVat(LoaiVat::BUI, {x, y}, 0.9f + rng.f() * 0.4f, rng.irange(0, 5));
        else ThemVat(LoaiVat::CAY, {x, y}, 0.85f + rng.f() * 0.4f, rng.irange(0, 5));
    }

    // Cờ đỏ ló ra sau bụi cây hai bên bờ, chỗ quân Đại Việt mai phục
    const sf::Vector2f coMaiPhuc[] = {{1020.f, 215.f}, {1180.f, 205.f}, {1340.f, 218.f}, {1060.f, 830.f}, {1250.f, 842.f}, {1400.f, 826.f}};
    for (const auto& p : coMaiPhuc) {
        ThemVat(LoaiVat::BUI, {p.x - 26.f, p.y + 6.f}, 1.3f, 1);
        ThemVat(LoaiVat::BUI, {p.x + 24.f, p.y + 8.f}, 1.2f, 2);
        ThemVat(LoaiVat::COT_CO, p, 0.8f);
    }
    CapNhatVatCan();

    // ----- Các ô cọc: so le như hoa mai, 8 cột x 4 hàng -----
    cacO.clear();
    for (int hang = 0; hang < 4; ++hang) {
        for (int cot = 0; cot < 8; ++cot) {
            float x = 985.f + static_cast<float>(cot) * 58.f + ((hang % 2 == 1) ? 29.f : 0.f);
            float y = 445.f + static_cast<float>(hang) * 52.f;
            cacO.push_back({{x, y}, false});
        }
    }
}

void ManChoiSong::CamTatCaCoc() {
    cocDaCam.clear();
    for (auto& o : cacO) {
        o.daCam = true;
        cocDaCam.emplace_back(sf::Vector2f(o.viTri.x, o.viTri.y - 22.f));
    }
}

void ManChoiSong::CapNhatThuyenSong(float deltaTime, bool choPhepLai, sf::Vector2f dongChay, float heSoToc) {
    if (choPhepLai) thuyen->XuLyPhim();
    else thuyen->DungLai();
    thuyen->DatDongChay(dongChay);
    thuyen->DatHeSoToc(heSoToc);
    thuyen->CapNhat(deltaTime);

    // Đặt nhân vật trùng vị trí thuyền để điểm tương tác dùng được
    nguoiChoi.SetViTri(thuyen->GetViTri());
}

sf::Vector2f ManChoiSong::ViTriMayAnhTheoDoi() const {
    // Nhìn lệch về phía trước thuyền một chút cho dễ thấy chỗ sắp tới
    sf::Vector2f van = thuyen->GetVanToc();
    return thuyen->GetViTri() + van * 0.35f;
}

void ManChoiSong::ThemMucVeSong(std::vector<std::pair<float, std::function<void()>>>& danhSach) {
    danhSach.push_back({0.f, [this]() { window.draw(mangBen); }});

    for (auto& coc : cocDaCam) {
        Coc* c = &coc;
        danhSach.push_back({c->GetViTri().y + 22.f, [this, c]() { c->Ve(window, tiLeTrieu); }});
    }
    danhSach.push_back({thuyen->GetViTri().y, [this]() { thuyen->Ve(window); }});
}

void ManChoiSong::VeThanh(float x, float y, float rong, float tiLe, sf::Color mauDay, sf::Color mauNen) const {
    sf::RectangleShape nen({rong, 10.f});
    nen.setPosition({x, y});
    nen.setFillColor(mauNen);
    nen.setOutlineThickness(1.f);
    nen.setOutlineColor(sf::Color(90, 100, 120));
    window.draw(nen);

    sf::RectangleShape day({rong * std::max(0.f, std::min(1.f, tiLe)), 10.f});
    day.setPosition({x, y});
    day.setFillColor(mauDay);
    window.draw(day);
}

void ManChoiSong::VeMinimapSong(const std::vector<sf::Vector2f>& diemDich) const {
    const float kichRong = 220.f;
    const float kichCao = 110.f;
    const sf::Vector2f goc(12.f, CAO_MAN_HINH - 28.f - kichCao);
    const float k = kichRong / RONG_THE_GIOI;

    sf::RectangleShape nen({kichRong + 6.f, kichCao + 6.f});
    nen.setPosition(goc - sf::Vector2f(3.f, 3.f));
    nen.setFillColor(sf::Color(15, 20, 28, 220));
    nen.setOutlineThickness(1.5f);
    nen.setOutlineColor(sf::Color(218, 165, 32));
    window.draw(nen);

    auto khung = [&](sf::FloatRect r, sf::Color c) {
        sf::RectangleShape s({r.size.x * k, r.size.y * k});
        s.setPosition(goc + sf::Vector2f(r.position.x * k, r.position.y * k));
        s.setFillColor(c);
        window.draw(s);
    };
    khung({{0.f, 0.f}, {RONG_THE_GIOI, CAO_THE_GIOI}}, sf::Color(60, 104, 56));
    khung(VUNG_SONG, sf::Color(42, 100, 150));
    khung(KHUNG_BAI_COC, sf::Color(218, 165, 32, 90));
    khung({{300.f, 246.f}, {56.f, 92.f}}, sf::Color(150, 108, 66));

    for (const auto& c : cocDaCam) {
        sf::RectangleShape cham({2.f, 2.f});
        cham.setPosition(goc + sf::Vector2f(c.GetViTri().x * k, c.GetViTri().y * k));
        cham.setFillColor(sf::Color(255, 215, 0));
        window.draw(cham);
    }

    for (const auto& d : diemDich) {
        sf::RectangleShape cham({4.f, 3.f});
        cham.setOrigin({2.f, 1.5f});
        cham.setPosition(goc + sf::Vector2f(std::min(d.x, RONG_THE_GIOI) * k, d.y * k));
        cham.setFillColor(sf::Color(230, 70, 60));
        window.draw(cham);
    }

    sf::CircleShape thuyenCham(3.f);
    thuyenCham.setOrigin({3.f, 3.f});
    thuyenCham.setPosition(goc + sf::Vector2f(thuyen->GetViTri().x * k, thuyen->GetViTri().y * k));
    thuyenCham.setFillColor(sf::Color(250, 250, 250));
    thuyenCham.setOutlineThickness(1.f);
    thuyenCham.setOutlineColor(sf::Color(40, 120, 220));
    window.draw(thuyenCham);

    sf::FloatRect nhin = mayAnh.GetVungNhin();
    sf::RectangleShape khungNhin({nhin.size.x * k, nhin.size.y * k});
    khungNhin.setPosition(goc + sf::Vector2f(nhin.position.x * k, nhin.position.y * k));
    khungNhin.setFillColor(sf::Color::Transparent);
    khungNhin.setOutlineThickness(1.f);
    khungNhin.setOutlineColor(sf::Color(255, 255, 255, 180));
    window.draw(khungNhin);

    VeChuTrai(window, nguCanh.phongChu, "Bản đồ sông", 10, goc + sf::Vector2f(2.f, -15.f), sf::Color(210, 220, 235));
}
