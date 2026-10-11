#include "CanhNenMenu.h"
#include "../CotLoi/TienIch.h"
#include <cmath>

namespace {
const float DUONG_CHAN_TROI = 400.f;   // Y của mép nước
const float TRANG_X = 705.f;
const float TRANG_Y = 92.f;

void ThemChuNhatDoc(sf::VertexArray& va, float x, float y, float w, float h, sf::Color tren, sf::Color duoi) {
    va.append({{x, y}, tren});
    va.append({{x + w, y}, tren});
    va.append({{x, y + h}, duoi});
    va.append({{x + w, y}, tren});
    va.append({{x + w, y + h}, duoi});
    va.append({{x, y + h}, duoi});
}

void ThemTamGiac(sf::VertexArray& va, sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Color mau) {
    va.append({a, mau});
    va.append({b, mau});
    va.append({c, mau});
}

sf::Color PhaMau(sf::Color a, sf::Color b, float t) {
    return sf::Color(static_cast<std::uint8_t>(a.r + (b.r - a.r) * t),
                     static_cast<std::uint8_t>(a.g + (b.g - a.g) * t),
                     static_cast<std::uint8_t>(a.b + (b.b - a.b) * t));
}
}

CanhNenMenu::CanhNenMenu() : thoiGian(0.f) {
    // Sao rải đều trên bầu trời, mỗi sao có nhịp nhấp nháy riêng
    for (int i = 0; i < 70; ++i) {
        float x = static_cast<float>((i * 137) % 800);
        float y = static_cast<float>((i * 59) % 300);
        danhSachSao.push_back({x, y, static_cast<float>(i) * 0.7f, 0.4f + 0.6f * static_cast<float>((i * 31) % 10) / 10.f});
    }

    // Ba chiến thuyền ở ba khoảng cách khác nhau cho có chiều sâu
    danhSachThuyen.push_back({700.f, 440.f, 0.55f, 9.f});
    danhSachThuyen.push_back({260.f, 480.f, 0.85f, 14.f});
    danhSachThuyen.push_back({520.f, 540.f, 1.25f, 20.f});

    DungLopTinh();
}

void CanhNenMenu::DungLopTinh() {
    lopTinh.clear();
    lopTroi.clear();

    // Bầu trời đêm chuyển dần sang màu hừng ở chân trời
    const int soDai = 8;
    float caoDai = DUONG_CHAN_TROI / static_cast<float>(soDai);
    sf::Color dinhTroi(8, 12, 34);
    sf::Color chanTroi(70, 62, 105);
    for (int i = 0; i < soDai; ++i) {
        float t0 = static_cast<float>(i) / static_cast<float>(soDai);
        float t1 = static_cast<float>(i + 1) / static_cast<float>(soDai);
        ThemChuNhatDoc(lopTroi, 0.f, caoDai * static_cast<float>(i), RONG_MAN_HINH, caoDai,
                       PhaMau(dinhTroi, chanTroi, t0 * t0), PhaMau(dinhTroi, chanTroi, t1 * t1));
    }

    // Dãy đồi xa dạng sóng lượn
    sf::Color mauDoiXa(30, 36, 66);
    for (float x = 0.f; x < RONG_MAN_HINH; x += 8.f) {
        float cao1 = 55.f + 28.f * std::sin(x * 0.012f + 1.f) + 16.f * std::sin(x * 0.031f + 2.f);
        float cao2 = 55.f + 28.f * std::sin((x + 8.f) * 0.012f + 1.f) + 16.f * std::sin((x + 8.f) * 0.031f + 2.f);
        sf::Vector2f a(x, DUONG_CHAN_TROI - cao1), b(x + 8.f, DUONG_CHAN_TROI - cao2);
        sf::Vector2f c(x, DUONG_CHAN_TROI), d(x + 8.f, DUONG_CHAN_TROI);
        ThemTamGiac(lopTinh, a, b, c, mauDoiXa);
        ThemTamGiac(lopTinh, b, d, c, mauDoiXa);
    }

    // Núi đá vôi nhọn như vùng cửa sông Bạch Đằng
    sf::Color mauNui(20, 26, 48);
    const float dinhNui[][3] = {   // x, rộng, cao
        {70.f, 80.f, 120.f}, {160.f, 60.f, 85.f}, {470.f, 70.f, 100.f}, {690.f, 90.f, 135.f}, {765.f, 50.f, 80.f}
    };
    for (const auto& n : dinhNui) {
        float x = n[0], rong = n[1], cao = n[2];
        ThemTamGiac(lopTinh, {x - rong * 0.5f, DUONG_CHAN_TROI}, {x + rong * 0.5f, DUONG_CHAN_TROI},
                    {x - rong * 0.08f, DUONG_CHAN_TROI - cao}, mauNui);
        ThemTamGiac(lopTinh, {x - rong * 0.08f, DUONG_CHAN_TROI - cao}, {x + rong * 0.5f, DUONG_CHAN_TROI},
                    {x + rong * 0.2f, DUONG_CHAN_TROI - cao * 0.62f}, sf::Color(26, 33, 58));
    }

    // Mặt sông
    const int soDaiSong = 5;
    float caoSong = CAO_MAN_HINH - DUONG_CHAN_TROI;
    float caoDaiSong = caoSong / static_cast<float>(soDaiSong);
    sf::Color mauSongTren(38, 52, 96);
    sf::Color mauSongDuoi(8, 14, 34);
    for (int i = 0; i < soDaiSong; ++i) {
        float t0 = static_cast<float>(i) / static_cast<float>(soDaiSong);
        float t1 = static_cast<float>(i + 1) / static_cast<float>(soDaiSong);
        ThemChuNhatDoc(lopTinh, 0.f, DUONG_CHAN_TROI + caoDaiSong * static_cast<float>(i), RONG_MAN_HINH, caoDaiSong,
                       PhaMau(mauSongTren, mauSongDuoi, t0), PhaMau(mauSongTren, mauSongDuoi, t1));
    }
}

void CanhNenMenu::CapNhat(float deltaTime) {
    thoiGian += deltaTime;
    for (auto& t : danhSachThuyen) {
        t.x -= t.tocDo * deltaTime;
        if (t.x < -120.f * t.tiLe) t.x = RONG_MAN_HINH + 120.f * t.tiLe;
    }
}

void CanhNenMenu::VeThuyen(sf::RenderWindow& window, const ThuyenNen& t) const {
    float s = t.tiLe;
    float nhun = std::sin(thoiGian * 1.3f + t.x * 0.01f) * 1.5f * s;
    sf::Vector2f goc(t.x, t.y + nhun);
    sf::Color mauThan(10, 13, 24);

    // Thân thuyền hình thang, mũi thuyền hướng sang trái
    sf::ConvexShape than(4);
    than.setPoint(0, {-52.f * s, -6.f * s});
    than.setPoint(1, {52.f * s, -6.f * s});
    than.setPoint(2, {38.f * s, 10.f * s});
    than.setPoint(3, {-40.f * s, 10.f * s});
    than.setPosition(goc);
    than.setFillColor(mauThan);
    window.draw(than);

    // Cột buồm
    sf::RectangleShape coc({3.f * s, 52.f * s});
    coc.setOrigin({1.5f * s, 52.f * s});
    coc.setPosition({goc.x - 4.f * s, goc.y - 5.f * s});
    coc.setFillColor(mauThan);
    window.draw(coc);

    // Cánh buồm cong nhẹ theo gió
    float gio = std::sin(thoiGian * 0.9f + t.y) * 2.f * s;
    sf::ConvexShape buom(4);
    buom.setPoint(0, {0.f, -50.f * s});
    buom.setPoint(1, {30.f * s + gio, -44.f * s});
    buom.setPoint(2, {28.f * s + gio, -14.f * s});
    buom.setPoint(3, {0.f, -12.f * s});
    buom.setPosition({goc.x - 2.f * s, goc.y});
    buom.setFillColor(sf::Color(32, 24, 34));
    window.draw(buom);

    // Lá cờ đỏ trên đỉnh cột
    sf::ConvexShape co(3);
    co.setPoint(0, {0.f, 0.f});
    co.setPoint(1, {14.f * s + gio * 0.5f, 3.f * s});
    co.setPoint(2, {0.f, 7.f * s});
    co.setPosition({goc.x - 4.f * s, goc.y - 57.f * s});
    co.setFillColor(sf::Color(170, 38, 30));
    window.draw(co);
}

void CanhNenMenu::Ve(sf::RenderWindow& window) const {
    window.draw(lopTroi);

    // Sao
    for (const auto& s : danhSachSao) {
        float nhapNhay = 0.5f + 0.5f * std::sin(thoiGian * 1.7f + s.pha);
        sf::RectangleShape cham({1.8f, 1.8f});
        cham.setPosition({s.x, s.y});
        cham.setFillColor(sf::Color(255, 250, 225, static_cast<std::uint8_t>(60.f + 160.f * s.doSang * nhapNhay)));
        window.draw(cham);
    }

    // Trăng và quầng sáng
    for (int i = 4; i >= 1; --i) {
        float r = 34.f + static_cast<float>(i) * 14.f;
        sf::CircleShape quang(r);
        quang.setOrigin({r, r});
        quang.setPosition({TRANG_X, TRANG_Y});
        quang.setFillColor(sf::Color(255, 240, 200, static_cast<std::uint8_t>(10 + (5 - i) * 4)));
        window.draw(quang);
    }
    sf::CircleShape trang(34.f);
    trang.setOrigin({34.f, 34.f});
    trang.setPosition({TRANG_X, TRANG_Y});
    trang.setFillColor(sf::Color(255, 246, 215));
    window.draw(trang);

    // Núi, đồi, sông
    window.draw(lopTinh);

    // Bóng trăng dưới sông, lay động theo sóng
    for (int i = 0; i < 16; ++i) {
        float y = DUONG_CHAN_TROI + 8.f + static_cast<float>(i) * 11.f;
        float rong = 46.f - static_cast<float>(i) * 1.6f + 6.f * std::sin(thoiGian * 2.f + static_cast<float>(i));
        float lech = 5.f * std::sin(thoiGian * 1.4f + static_cast<float>(i) * 0.8f);
        sf::RectangleShape vet({rong, 2.2f});
        vet.setOrigin({rong * 0.5f, 1.1f});
        vet.setPosition({TRANG_X + lech, y});
        vet.setFillColor(sf::Color(255, 240, 200, static_cast<std::uint8_t>(130 - i * 6)));
        window.draw(vet);
    }

    // Vạch sóng nhẹ trên mặt nước
    for (int i = 0; i < 22; ++i) {
        float y = DUONG_CHAN_TROI + 14.f + static_cast<float>((i * 29) % 180);
        float x = std::fmod(static_cast<float>((i * 83) % 800) + thoiGian * (6.f + static_cast<float>(i % 5) * 2.f), RONG_MAN_HINH + 60.f) - 30.f;
        sf::RectangleShape song({26.f + static_cast<float>(i % 4) * 8.f, 1.3f});
        song.setPosition({x, y});
        song.setFillColor(sf::Color(150, 175, 230, 38));
        window.draw(song);
    }

    // Chiến thuyền: vẽ xa trước, gần sau
    for (const auto& t : danhSachThuyen) VeThuyen(window, t);
}
