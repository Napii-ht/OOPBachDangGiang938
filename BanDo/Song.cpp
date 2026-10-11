
#include "Song.h"
#include "DoHoa.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>

using namespace dohoa;

Song::Song(sf::FloatRect phamVi)
    : vungSong(phamVi),
      trangThai(TrangThaiThuyTrieu::CAO),
      tiLeThuyTrieu(1.0f),
      thoiGianRut(0.0f),
      dongChayOffset(0.0f) {

    // Sinh các đốm bọt nước trôi dạt tự nhiên
    for (int i = 0; i < 40; ++i) {
        BotNuoc b;
        b.viTri = sf::Vector2f(
            vungSong.position.x + static_cast<float>(rand() % static_cast<int>(vungSong.size.x)),
            vungSong.position.y + static_cast<float>(rand() % static_cast<int>(vungSong.size.y))
        );
        b.tocDo = 40.f + static_cast<float>(rand() % 60);
        b.banKinh = 2.f + static_cast<float>(rand() % 4);
        b.doMo = 120.f + static_cast<float>(rand() % 100);
        danhSachBotNuoc.push_back(b);
    }

    // Điểm nắng lấp lánh (tập trung quanh vệt nắng hoàng hôn)
    Rng rng(0x5EEDu);
    for (int i = 0; i < 90; ++i) {
        LongLanh g;
        float a = rng.f() + rng.f() + rng.f() - 1.5f;   // phân phối gần chuông
        g.lech = std::clamp(a * 0.9f, -1.f, 1.f);
        g.v = rng.range(0.02f, 0.98f);
        g.pha = rng.range(0.f, 6.28f);
        g.tocDo = rng.range(1.2f, 3.4f);
        g.rong = rng.range(5.f, 15.f);
        danhSachLongLanh.push_back(g);
    }

    // Vũng nước trên bãi bùn
    for (int i = 0; i < 26; ++i) {
        VungBun p;
        p.x = vungSong.position.x + rng.f() * vungSong.size.x;
        p.v = rng.range(0.25f, 0.75f);
        p.rong = rng.range(6.f, 16.f);
        p.phiaBac = (i % 2 == 0);
        danhSachVungBun.push_back(p);
    }
}

void Song::SetTrangThaiThuyTrieu(TrangThaiThuyTrieu tt) {
    trangThai = tt;
    if (tt == TrangThaiThuyTrieu::CAO) {
        tiLeThuyTrieu = 1.0f;
    } else if (tt == TrangThaiThuyTrieu::THAP) {
        tiLeThuyTrieu = 0.0f;
    }
}

void Song::DatTiLeThuyTrieu(float tiLe) {
    tiLeThuyTrieu = std::clamp(tiLe, 0.f, 1.f);
    // Chỉ dùng hai trạng thái ổn định, tránh kích hoạt cơ chế tự rút nước của DANG_XUONG
    trangThai = (tiLeThuyTrieu > 0.5f) ? TrangThaiThuyTrieu::CAO : TrangThaiThuyTrieu::THAP;
}

void Song::BatDauTrieuRut(float thoiGian_) {
    trangThai = TrangThaiThuyTrieu::DANG_XUONG;
    thoiGianRut = thoiGian_;
}

void Song::CapNhat(float deltaTime) {
    dongChayOffset += deltaTime * 50.f;
    if (dongChayOffset > 1000.f) dongChayOffset -= 1000.f;

    thoiGian += deltaTime;
    // Triều rút: dòng chảy ra biển mạnh hơn rõ rệt
    float heSoDong = (trangThai == TrangThaiThuyTrieu::DANG_XUONG) ? 2.6f : 1.f;
    phaDong += deltaTime * heSoDong;

    // Tiến trình hạ mực nước khi triều rút
    if (trangThai == TrangThaiThuyTrieu::DANG_XUONG) {
        if (thoiGianRut > 0.001f) {
            tiLeThuyTrieu -= (deltaTime / thoiGianRut);
            if (tiLeThuyTrieu <= 0.0f) {
                tiLeThuyTrieu = 0.0f;
                trangThai = TrangThaiThuyTrieu::THAP;
            }
        } else {
            tiLeThuyTrieu = 0.0f;
            trangThai = TrangThaiThuyTrieu::THAP;
        }
    }

    // Cập nhật vị trí bọt nước xuôi theo dòng chảy sông Bạch Đằng
    for (auto& b : danhSachBotNuoc) {
        b.viTri.x += b.tocDo * deltaTime * heSoDong;
        // Lượn sóng nhẹ theo trục Y
        b.viTri.y += std::sin((b.viTri.x + dongChayOffset) * 0.03f) * 0.4f;

        if (b.viTri.x > vungSong.position.x + vungSong.size.x) {
            b.viTri.x = vungSong.position.x;
            b.viTri.y = vungSong.position.y + static_cast<float>(rand() % static_cast<int>(vungSong.size.y));
        }
    }

    // Gợn tròn ngẫu nhiên (cá quẫy, giọt nước)
    henGonTron -= deltaTime;
    if (henGonTron <= 0.f && danhSachGonTron.size() < 8) {
        GonTron g;
        g.viTri = {vungSong.position.x + 40.f + static_cast<float>(rand() % std::max(1, static_cast<int>(vungSong.size.x) - 80)),
                   vungSong.position.y + 40.f + static_cast<float>(rand() % std::max(1, static_cast<int>(vungSong.size.y) - 80))};
        g.tuoi = 0.f;
        g.tuoiMax = 1.6f + static_cast<float>(rand() % 100) / 100.f;
        g.banKinh = 14.f + static_cast<float>(rand() % 14);
        danhSachGonTron.push_back(g);
        henGonTron = 0.5f + static_cast<float>(rand() % 140) / 100.f;
    }
    for (auto& g : danhSachGonTron) {
        g.tuoi += deltaTime;
        g.viTri.x += 6.f * deltaTime * heSoDong;   // trôi theo dòng
    }
    danhSachGonTron.erase(
        std::remove_if(danhSachGonTron.begin(), danhSachGonTron.end(),
                       [](const GonTron& g) { return g.tuoi >= g.tuoiMax; }),
        danhSachGonTron.end());
}

void Song::Ve(sf::RenderWindow& window) {
    mangDinh.clear();

    const float x0 = vungSong.position.x;
    const float y0 = vungSong.position.y;
    const float W = vungSong.size.x;
    const float H = vungSong.size.y;
    const float m = 1.f - std::clamp(tiLeThuyTrieu, 0.f, 1.f);   // 0 = triều cao, 1 = triều cạn
    const float t = thoiGian;
    const float sunX = GetToaDoAnhNang();

    // Độ lệch cong theo chiều sâu: nhiều hàng sóng nhỏ ở xa (phía bắc), thưa & lớn ở gần
    auto yTheoV = [&](float v) { return y0 + (H) * std::pow(v, 1.15f); };

    // Màu nước theo vị trí: xa phản chiếu bầu trời, giữa sâu thẳm, gần tối rêu
    auto mauNuoc = [&](float x, float v) {
        Color xa  = pha(mau(78, 124, 146), mau(112, 132, 112), m);
        Color giu = pha(mau(24, 68, 102), mau(80, 104, 90), m);
        Color gan = pha(mau(12, 44, 68), mau(58, 80, 66), m);
        Color c = v < 0.5f ? pha(xa, giu, v * 2.f) : pha(giu, gan, (v - 0.5f) * 2.f);
        float dx = (x - sunX) / (150.f + 260.f * v);
        float k = std::exp(-dx * dx) * (1.f - 0.4f * v);
        return pha(c, mau(240, 164, 104), k * 0.6f);   // vệt ánh hoàng hôn
    };

    // ---------------------------------------------------------------- 1. Nền nước (lưới màu)
    {
        const int cot = 40, hang = 14;
        std::vector<Color> luoi(static_cast<size_t>((cot + 1) * (hang + 1)));
        for (int j = 0; j <= hang; ++j) {
            float v = static_cast<float>(j) / hang;
            for (int i = 0; i <= cot; ++i) {
                float x = x0 + W * static_cast<float>(i) / cot;
                // dải sáng tối chậm chạp như độ sâu thay đổi
                float n = nhieu(x * 0.01f + t * 0.05f, v * 3.f + t * 0.03f, 11) - 0.5f;
                Color c = mauNuoc(x, v);
                luoi[static_cast<size_t>(j * (cot + 1) + i)] = mau(c.r + n * 22.f, c.g + n * 22.f, c.b + n * 18.f);
            }
        }
        for (int j = 0; j < hang; ++j) {
            for (int i = 0; i < cot; ++i) {
                float xa = x0 + W * static_cast<float>(i) / cot, xb = x0 + W * static_cast<float>(i + 1) / cot;
                float ya = y0 + H * static_cast<float>(j) / hang, yb = y0 + H * static_cast<float>(j + 1) / hang;
                tu(mangDinh, {xa, ya}, {xb, ya}, {xb, yb}, {xa, yb},
                   luoi[static_cast<size_t>(j * (cot + 1) + i)], luoi[static_cast<size_t>(j * (cot + 1) + i + 1)],
                   luoi[static_cast<size_t>((j + 1) * (cot + 1) + i + 1)], luoi[static_cast<size_t>((j + 1) * (cot + 1) + i)]);
            }
        }
    }

    // ---------------------------------------------------------------- 2. Phản chiếu mờ của bờ (bóng cây xanh sẫm sát mép nước)
    {
        Color bongBac = mau(18, 40, 38, 90 * (1.f - m * 0.7f));
        Color bongNam = mau(8, 24, 28, 120 * (1.f - m * 0.7f));
        chuNhatDoc(mangDinh, x0, y0, W, 26.f, bongBac, vaA(bongBac, 0));
        chuNhatDoc(mangDinh, x0, y0 + H - 34.f, W, 34.f, vaA(bongNam, 0), bongNam);
    }

    // ---------------------------------------------------------------- 3. Sóng gợn (nét đứt pixel-art, trôi theo dòng)
    const int soHang = 15;
    for (int i = 0; i < soHang; ++i) {
        float v = (static_cast<float>(i) + 0.5f) / soHang;
        float yc = yTheoV(v);
        if (yc < y0 + 6.f || yc > y0 + H - 6.f) continue;
        float buoc = 6.f + 6.f * v;
        float day = 1.3f + 2.4f * v;
        float bienDo = 0.8f + 2.8f * v;
        float cuon = phaDong * (9.f + 26.f * v);
        float pi = static_cast<float>(i) * 2.17f;

        auto yTai = [&](float x) {
            float xs = x - cuon;
            return yc + std::sin(xs * 0.045f + t * 1.1f + pi) * bienDo;
        };
        for (float x = x0 - buoc; x < x0 + W; x += buoc) {
            float xs = x - cuon;
            float s = std::sin(xs * 0.019f + pi) * std::sin(xs * 0.0067f + pi * 1.7f + t * 0.15f);
            float a = std::clamp((s - 0.1f) * 4.5f, 0.f, 1.f);
            if (a < 0.03f) continue;

            float dx = (x - sunX) / (160.f + 240.f * v);
            float anh = std::exp(-dx * dx);
            Color sangC = pha(mau(172, 218, 238), mau(255, 206, 150), anh * 0.8f);
            float al = (34.f + 78.f * v) * a * (1.f - 0.45f * m) + anh * 40.f * a;
            float ya = yTai(x), yb = yTai(x + buoc);

            // bóng tối dưới đỉnh sóng -> cảm giác khối
            tu(mangDinh, {x, ya + day}, {x + buoc, yb + day}, {x + buoc, yb + day * 2.2f}, {x, ya + day * 2.2f},
               mau(6, 28, 48, al * 0.55f), mau(6, 28, 48, al * 0.55f), mau(6, 28, 48, 0), mau(6, 28, 48, 0));
            // đỉnh sóng sáng
            tu(mangDinh, {x, ya - day * 0.5f}, {x + buoc, yb - day * 0.5f}, {x + buoc, yb + day * 0.5f}, {x, ya + day * 0.5f},
               vaA(sangC, al), vaA(sangC, al), vaA(sangC, al), vaA(sangC, al));
        }
    }

    // ---------------------------------------------------------------- 4. Nắng lấp lánh
    for (const auto& g : danhSachLongLanh) {
        float v = g.v;
        float y = yTheoV(v);
        float x = sunX + g.lech * (40.f + 200.f * v) + std::sin(t * 0.5f + g.pha) * 8.f;
        float b = std::sin(t * g.tocDo + g.pha);
        if (b <= 0.f) continue;
        b = b * b * b;
        float w = g.rong * (0.6f + 1.1f * v);
        float h = 1.4f + 1.8f * v;
        float al = 235.f * b * (1.f - 0.6f * m);
        chuNhat(mangDinh, x - w * 0.5f, y - h * 0.5f, w, h, mau(255, 224, 160, al));
        chuNhat(mangDinh, x - w * 0.25f, y - h * 0.5f, w * 0.5f, h, mau(255, 252, 235, al));
        // quầng nhỏ
        elip(mangDinh, x, y, w * 0.8f, h * 2.2f, mau(255, 200, 120, al * 0.18f), 8);
    }

    // ---------------------------------------------------------------- 5. Gợn tròn (cá quẫy)
    for (const auto& g : danhSachGonTron) {
        float p = g.tuoi / g.tuoiMax;
        float a = (1.f - p) * 120.f;
        for (int k = 0; k < 3; ++k) {
            float pk = p - 0.12f * static_cast<float>(k);
            if (pk <= 0.f) continue;
            float r = g.banKinh * pk * 1.6f;
            vong(mangDinh, g.viTri.x, g.viTri.y, r, r * 0.38f, 1.4f, mau(210, 236, 250, a * (1.f - 0.25f * static_cast<float>(k))), 22);
        }
    }

    // ---------------------------------------------------------------- 6. Bọt nước trôi
    for (const auto& b : danhSachBotNuoc) {
        float a = b.doMo * 0.75f;
        elip(mangDinh, b.viTri.x, b.viTri.y, b.banKinh * 1.3f, b.banKinh * 0.6f, mau(222, 244, 255, a), 8);
        elip(mangDinh, b.viTri.x - b.banKinh * 0.3f, b.viTri.y - b.banKinh * 0.15f, b.banKinh * 0.45f, b.banKinh * 0.22f,
             mau(255, 255, 255, a), 6);
    }

    // ---------------------------------------------------------------- 7. Bãi bùn lộ ra khi triều rút (m > 0)
    const float lo = m * 16.f;
    auto veBaiBun = [&](bool phiaBac) {
        float yTrenDai = phiaBac ? y0 : y0 + H - lo;
        float yMepNuoc = phiaBac ? y0 + lo : y0 + H - lo;
        int soCot = static_cast<int>(W / 10.f) + 1;
        for (int i = 0; i < soCot; ++i) {
            float x = x0 + static_cast<float>(i) * 10.f;
            float n = bam(i, phiaBac ? 3 : 4, 77);
            Color kho = pha(mau(120, 98, 70), mau(98, 80, 58), n);
            Color uot = pha(mau(66, 52, 42), mau(52, 42, 36), n);
            if (phiaBac) chuNhatDoc(mangDinh, x, yTrenDai, 10.f, lo, kho, uot);
            else         chuNhatDoc(mangDinh, x, yTrenDai, 10.f, lo, uot, kho);
            // lỗ cua, lỗ cá thòi lòi
            if (n > 0.8f && lo > 6.f) {
                float hy = yTrenDai + lo * (0.3f + 0.4f * bam(i, 9, 5));
                elip(mangDinh, x + 4.f, hy, 2.2f, 1.4f, mau(30, 22, 18, 200), 6);
            }
            // vệt cát thô sáng
            if (n < 0.18f) chuNhat(mangDinh, x + 1.f, yTrenDai + lo * 0.35f, 6.f, 1.f, mau(168, 146, 112, 130));
        }
        // lớp bóng loáng ướt sát mép nước
        float k = 5.f;
        if (phiaBac) chuNhatDoc(mangDinh, x0, yMepNuoc - k, W, k, mau(180, 190, 190, 0), mau(190, 205, 210, 70));
        else         chuNhatDoc(mangDinh, x0, yMepNuoc, W, k, mau(190, 205, 210, 70), mau(180, 190, 190, 0));
    };

    if (lo > 0.6f) {
        veBaiBun(true);
        veBaiBun(false);
        for (const auto& p : danhSachVungBun) {
            float yy = p.phiaBac ? y0 + lo * p.v : y0 + H - lo + lo * p.v;
            Color nuoc = mau(86, 124, 134, 190);
            elip(mangDinh, p.x, yy, p.rong, p.rong * 0.22f, nuoc, 10);
            elip(mangDinh, p.x - p.rong * 0.25f, yy - 0.6f, p.rong * 0.5f, p.rong * 0.07f, mau(238, 200, 170, 150), 8);
        }
    }

    // ---------------------------------------------------------------- 8. Bọt sóng vỗ bờ (chạy theo mép nước)
    auto veBotBo = [&](bool phiaBac) {
        float yMep = phiaBac ? y0 + lo : y0 + H - lo;
        float dau = phiaBac ? 1.f : -1.f;
        // dải nước nông sáng màu sát bờ
        chuNhatDoc(mangDinh, x0, phiaBac ? yMep : yMep - 8.f, W, 8.f,
                   phiaBac ? mau(150, 205, 220, 70) : mau(150, 205, 220, 0),
                   phiaBac ? mau(150, 205, 220, 0) : mau(150, 205, 220, 70));
        for (float x = x0; x < x0 + W + 14.f; x += 11.f) {
            float s = std::sin(x * 0.09f + t * 1.9f);
            float r = 3.2f + 2.2f * (0.5f + 0.5f * std::sin(x * 0.31f + 1.3f));
            float y = yMep + dau * (1.5f + s * 1.6f);
            elip(mangDinh, x, y, r * 1.5f, r * 0.7f, mau(232, 246, 252, 120), 8);
        }
        // sóng liếm vào bờ
        float lap = 0.5f + 0.5f * std::sin(t * 0.9f + (phiaBac ? 0.f : 2.f));
        for (float x = x0; x < x0 + W; x += 7.f) {
            float yy = yMep + dau * (3.f + lap * 4.f + std::sin(x * 0.05f + t) * 1.5f);
            chuNhat(mangDinh, x, yy, 7.f, 1.2f, mau(235, 248, 255, 70 * (1.f - lap * 0.5f)));
        }
    };
    veBotBo(true);
    veBotBo(false);

    window.draw(mangDinh);
}


