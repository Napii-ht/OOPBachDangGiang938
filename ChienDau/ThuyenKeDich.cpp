#include "ThuyenKeDich.h"
#include "../BanDo/DoHoa.h"
#include <cmath>
#include <cstdint>
#include <algorithm>

using namespace dohoa;

namespace {

// =============================================================================
//  CÔNG CỤ ĐỒ HỌA PROCEDURAL CHO CHIẾN THUYỀN GIẶC
// =============================================================================
struct Bo {
    VA& va;
    Vector2f o;
    float sx, co, si;
    Bo(VA& v, Vector2f origin, float scaleX, float rad)
        : va(v), o(origin), sx(scaleX), co(std::cos(rad)), si(std::sin(rad)) {}

    Vector2f P(float x, float y) const {
        float X = x * sx;
        return {o.x + X * co - y * si, o.y + X * si + y * co};
    }
    void tri(float ax, float ay, float bx, float by, float cx, float cy, Color c) {
        tam(va, P(ax, ay), P(bx, by), P(cx, cy), c);
    }
    void tri3(float ax, float ay, float bx, float by, float cx, float cy, Color ca, Color cb, Color cc) {
        tam3(va, P(ax, ay), P(bx, by), P(cx, cy), ca, cb, cc);
    }
    void rect(float x0, float y0, float x1, float y1, Color c) {
        tu(va, P(x0, y0), P(x1, y0), P(x1, y1), P(x0, y1), c, c, c, c);
    }
    void grad(float x0, float y0, float x1, float y1, Color c00, Color c10, Color c11, Color c01) {
        tu(va, P(x0, y0), P(x1, y0), P(x1, y1), P(x0, y1), c00, c10, c11, c01);
    }
    void quad(Vector2f a, Vector2f b, Vector2f c, Vector2f d, Color ca, Color cb, Color cc, Color cd) {
        tu(va, P(a.x, a.y), P(b.x, b.y), P(c.x, c.y), P(d.x, d.y), ca, cb, cc, cd);
    }
    void ell(float cx, float cy, float rx, float ry, Color c, int seg = 12) {
        for (int i = 0; i < seg; ++i) {
            float a0 = 2.f * PI * static_cast<float>(i) / static_cast<float>(seg);
            float a1 = 2.f * PI * static_cast<float>(i + 1) / static_cast<float>(seg);
            tam(va, P(cx, cy), P(cx + std::cos(a0) * rx, cy + std::sin(a0) * ry),
                P(cx + std::cos(a1) * rx, cy + std::sin(a1) * ry), c);
        }
    }
    void ellT(float cx, float cy, float rx, float ry, Color ct, Color ce, int seg = 14) {
        for (int i = 0; i < seg; ++i) {
            float a0 = 2.f * PI * static_cast<float>(i) / static_cast<float>(seg);
            float a1 = 2.f * PI * static_cast<float>(i + 1) / static_cast<float>(seg);
            tam3(va, P(cx, cy), P(cx + std::cos(a0) * rx, cy + std::sin(a0) * ry),
                 P(cx + std::cos(a1) * rx, cy + std::sin(a1) * ry), ct, ce, ce);
        }
    }
    void line(float ax, float ay, float bx, float by, float th, Color c) {
        duong(va, P(ax, ay), P(bx, by), th, c);
    }
};

struct Vo {
    float xs, xb, W, bowP, sternP;
};

float hwAt(const Vo& v, float x) {
    float u = 2.f * (x - v.xs) / (v.xb - v.xs) - 1.f;
    float a = std::abs(u);
    float p = u >= 0.f ? v.bowP : v.sternP;
    float r = std::pow(std::max(0.f, 1.f - std::pow(a, p)), 0.62f);
    return v.W * 0.5f * r;
}

void dai(Bo& b, const Vo& v, int n, float kOut, float kIn, Color cOut, Color cIn) {
    for (int i = 0; i < n; ++i) {
        float x0 = v.xs + (v.xb - v.xs) * static_cast<float>(i) / static_cast<float>(n);
        float x1 = v.xs + (v.xb - v.xs) * static_cast<float>(i + 1) / static_cast<float>(n);
        float h0 = hwAt(v, x0), h1 = hwAt(v, x1);
        for (int s = -1; s <= 1; s += 2) {
            float sg = static_cast<float>(s);
            b.quad({x0, sg * h0 * kOut}, {x1, sg * h1 * kOut}, {x1, sg * h1 * kIn}, {x0, sg * h0 * kIn}, cOut, cOut, cIn, cIn);
        }
    }
}

void veVetNuoc(Bo& b, const Vo& v, float t, float cuong, float dl) {
    for (int s = -1; s <= 1; s += 2) {
        float sg = static_cast<float>(s);
        Vector2f a = b.P(v.xs + 4.f, sg * v.W * 0.3f);
        Vector2f c = b.P(v.xs - dl, sg * (v.W * 0.3f + dl * 0.3f));
        duongTan(b.va, a, c, 2.8f, 6.f, mau(232, 247, 255, 130.f * cuong), mau(232, 247, 255, 0));
    }
    for (int k = 0; k < 8; ++k) {
        float p = std::fmod(t * 1.3f + static_cast<float>(k) / 8.f, 1.f);
        float x = v.xs - 2.f - p * dl * 0.9f;
        float y = std::sin(p * 10.f + static_cast<float>(k) * 1.7f) * v.W * 0.2f * (0.3f + p);
        float r = 3.4f * (1.f - 0.45f * p);
        b.ell(x, y, r * 1.4f, r * 0.8f, mau(238, 249, 255, (1.f - p) * 175.f * cuong), 7);
    }
}

void veMai(Bo& b, const Vo& v, float x, int side, float ph, float len, float swing, Color shaft, Color blade, bool vungNuoc) {
    float sg = static_cast<float>(side);
    float hx = hwAt(v, x) * 0.92f;
    float s = std::sin(ph);
    float tx = x + s * swing - 2.f, ty = sg * (hx + len);
    b.line(x, sg * hx * 0.5f, tx, ty, 1.6f, shaft);
    b.ell(tx, ty + sg * 1.5f, 2.0f, 3.9f, blade, 8);
    if (vungNuoc && s > 0.55f) {
        float q = (s - 0.55f) / 0.45f;
        b.ell(tx, ty + sg * 2.f, 4.f + 2.f * q, 1.8f + 0.8f * q, mau(236, 250, 255, 90.f * (1.f - 0.3f * q)), 8);
    }
}

void veBuom(Bo& b, float mx, float halfLen, float rong, float t, float ph, Color giua, Color bien, Color vien) {
    const int N = 9;
    const float bu = 3.4f + std::sin(t * 1.6f + ph) * 1.0f;
    float px = mx, py = -halfLen;
    for (int k = 0; k <= N; ++k) {
        float f = static_cast<float>(k) / N * 2.f - 1.f;
        float y = f * halfLen;
        float x = mx + bu * (1.f - f * f);
        if (k > 0) {
            float fm = f - 1.f / N;
            Color c = pha(bien, giua, 1.f - fm * fm);
            b.grad(px - rong * 0.5f, py, x + rong * 0.5f, y, toi(c, 0.76f), c, c, toi(c, 0.76f));
            b.line(px - rong * 0.5f, py, x - rong * 0.5f, y, 1.2f, vien);
            b.line(px + rong * 0.5f, py, x + rong * 0.5f, y, 1.2f, vien);
        }
        px = x; py = y;
    }
    b.line(mx - rong * 0.5f, -halfLen, mx + rong * 0.5f, -halfLen, 1.4f, vien);
    b.line(mx - rong * 0.5f, halfLen, mx + rong * 0.5f, halfLen, 1.4f, vien);
    b.line(mx + bu * 0.1f, -halfLen - 2.f, mx + bu * 0.1f, halfLen + 2.f, 1.1f, mau(84, 56, 32));
}

void veRong(Bo& b, float cx, float cy, float s, Color c) {
    float px = cx, py = cy - 1.6f * s;
    for (int i = 1; i <= 8; ++i) {
        float f = static_cast<float>(i) / 8.f;
        float x = cx + std::sin(f * 6.2f) * 1.1f * s;
        float y = cy - 1.6f * s + f * 3.2f * s;
        b.line(px, py, x, y, 1.5f * s * (1.1f - 0.6f * f), c);
        px = x; py = y;
    }
    b.ell(cx, cy - 1.8f * s, 0.9f * s, 0.8f * s, c, 8);
    b.tri(cx - 0.5f * s, cy - 2.4f * s, cx - 1.4f * s, cy - 3.2f * s, cx - 0.1f * s, cy - 2.0f * s, c);
    b.tri(cx + 0.5f * s, cy - 2.4f * s, cx + 1.4f * s, cy - 3.2f * s, cx + 0.1f * s, cy - 2.0f * s, c);
}

void veCoBo(Bo& b, float x, float y, float dl, float cao, float t, float ph, Color c, Color vien) {
    b.ell(x, y, 1.7f, 1.7f, mau(70, 46, 26), 8);
    const int N = 7;
    float px = x, py = y;
    for (int k = 1; k <= N; ++k) {
        float f = static_cast<float>(k) / N;
        float f0 = static_cast<float>(k - 1) / N;
        float nx = x - dl * f;
        float ny = y + std::sin(t * 6.f + ph + f * 6.f) * 2.4f * f;
        float sh0 = 0.5f + 0.5f * std::sin(t * 6.f + ph + f0 * 6.f + 1.2f);
        float sh1 = 0.5f + 0.5f * std::sin(t * 6.f + ph + f * 6.f + 1.2f);
        Color c0 = toi(c, 0.8f + 0.25f * sh0), c1 = toi(c, 0.8f + 0.25f * sh1);
        float h0 = cao * 0.5f * (1.f - 0.15f * f0), h1 = cao * 0.5f * (1.f - 0.15f * f);
        b.quad({px, py - h0}, {nx, ny - h1}, {nx, ny + h1}, {px, py + h0}, c0, c1, c1, c0);
        b.line(px, py - h0, nx, ny - h1, 1.1f, vien);
        b.line(px, py + h0, nx, ny + h1, 1.1f, vien);
        px = nx; py = ny;
    }
}

void veXacThuyen(VA& va, Vector2f c, float t, std::uint32_t sd, float L) {
    elip(va, c.x, c.y + 2.f, L * 0.55f, L * 0.2f, mau(10, 14, 20, 70), 16);
    elip(va, c.x + 4.f, c.y + 3.f, L * 0.38f, L * 0.12f, mau(60, 44, 70, 40), 14);
    for (int i = 0; i < 7; ++i) {
        float ox = (bam(i, 1, sd) - 0.5f) * L * 0.95f;
        float oy = (bam(i, 2, sd) - 0.5f) * L * 0.4f + std::sin(t * 1.1f + static_cast<float>(i)) * 0.9f;
        float rot = (bam(i, 3, sd) - 0.5f) * 3.f;
        float len = L * (0.1f + 0.1f * bam(i, 4, sd));
        Bo pb(va, {c.x + ox, c.y + oy}, 1.f, rot);
        pb.rect(-len, -1.8f, len, 1.8f, mau(44, 26, 14, 220));
    }
}

} // namespace

// =============================================================================
//  THỰC THI CLASS ThuyenKeDich
// =============================================================================

ThuyenKeDich::ThuyenKeDich(sf::Vector2f viTriBanDau, bool soChiHuy)
    : viTri(viTriBanDau),
      huongDiChuyen(1.f, 0.f),
      tocDo(soChiHuy ? 70.f : 95.f),
      sucKhoe(soChiHuy ? 350 : 180),
      sucKhoeToiDa(soChiHuy ? 350 : 180),
      daMacCoc(false),
      daBiPhaHuy(false),
      chieuDai(soChiHuy ? 115.f : 85.f),
      chieuRong(soChiHuy ? 46.f : 36.f),
      gocXoay(0.f),
      thoiDiemBiTrung(-100.f),
      tenThuyen(soChiHuy ? "Soái hạm Lưu Hoằng Tháo" : "Chiến thuyền Nam Hán"),
      thoiGianKhoiBoc(0.f),
      thuyenSoChiHuy(soChiHuy),
      huongMat(-1.f),
      sxLat(-1.f) {}

sf::FloatRect ThuyenKeDich::GetHitBox() const {
    return sf::FloatRect(
        {viTri.x - chieuDai * 0.5f, viTri.y - chieuRong * 0.5f},
        {chieuDai, chieuRong}
    );
}

void ThuyenKeDich::NhanSatThuong(int damage) {
    thoiDiemBiTrung = dongHo();
    sucKhoe -= damage;
    if (sucKhoe <= 0) {
        sucKhoe = 0;
        daBiPhaHuy = true;
    }
}

void ThuyenKeDich::DuoiTheo(sf::Vector2f viTriThuyenPlayer, float deltaTime) {
    if (daBiPhaHuy || daMacCoc) return;

    float dx = viTriThuyenPlayer.x - viTri.x;
    float dy = viTriThuyenPlayer.y - viTri.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (std::abs(dx) > 6.f) huongMat = dx > 0.f ? 1.f : -1.f;
    sxLat += (huongMat - sxLat) * std::min(1.f, deltaTime * 10.f);

    if (dist > 5.f) {
        huongDiChuyen = sf::Vector2f(dx / dist, dy / dist);
        viTri += huongDiChuyen * tocDo * deltaTime;
    }
}

void ThuyenKeDich::CapNhat(float deltaTime) {
    if (daMacCoc) {
        thoiGianKhoiBoc += deltaTime;
    }
}

void ThuyenKeDich::Ve(sf::RenderWindow& window) {
    static sf::VertexArray va(sf::PrimitiveType::Triangles);
    va.clear();

    const float t = dongHo();
    const std::uint32_t sd = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(this) >> 4) & 0xFFFFu;
    const float ph0 = static_cast<float>(sd & 255u) / 40.f;
    const bool cm = thuyenSoChiHuy;

    if (daBiPhaHuy && sucKhoe <= 0) {
        veXacThuyen(va, viTri, t, sd, chieuDai);
        window.draw(va);
        return;
    }

    const float L = chieuDai, B = chieuRong;
    const float sg = huongMat >= 0.f ? 1.f : -1.f;
    float ang = 0.f;
    if (daMacCoc) ang = 0.09f + std::sin(thoiGianKhoiBoc * 2.f) * 0.015f;
    else          ang = huongDiChuyen.y * 0.2f * sg;
    ang += std::sin(t * 1.1f + ph0) * 0.02f;

    Bo b(va, {viTri.x, viTri.y + (daMacCoc ? 0.4f : std::sin(t * 1.5f + ph0) * 0.8f)}, sxLat, ang);
    const Vo v{-L * 0.5f, L * 0.5f + 18.f, B, 2.0f, 3.6f};

    const Color goCanh  = cm ? mau(56, 18, 18) : mau(52, 32, 22);
    const Color goGiua  = cm ? mau(128, 44, 34) : mau(108, 66, 38);
    const Color manTrai = mau(34, 30, 34);
    const Color vang    = mau(232, 178, 56);
    const Color dongSang = mau(238, 196, 96), dongToi = mau(138, 92, 36);

    b.ell(3.f, 5.f, L * 0.62f, B * 0.6f, mau(0, 0, 0, 80), 20);
    if (!daMacCoc) {
        veVetNuoc(b, v, t, cm ? 0.9f : 1.f, cm ? 78.f : 64.f);
    }

    const int soChoi = std::max(4, static_cast<int>(L / 14.f));
    for (int i = 0; i < soChoi; ++i) {
        float x = v.xs + 14.f + (v.xb - v.xs - 46.f) * static_cast<float>(i) / static_cast<float>(soChoi - 1);
        for (int s = -1; s <= 1; s += 2) {
            float ph = daMacCoc ? (0.6f + static_cast<float>(i) * 0.55f + (s > 0 ? 1.1f : 0.f))
                                : t * 3.4f + static_cast<float>(i) * 0.5f + (s > 0 ? 0.5f : 0.f);
            veMai(b, v, x, s, ph, cm ? 15.f : 13.f, 5.f, mau(60, 40, 26), mau(94, 64, 40), !daMacCoc);
        }
    }

    dai(b, v, 20, 1.13f, 0.f, mau(22, 12, 8), mau(22, 12, 8));
    dai(b, v, 20, 1.0f, 0.f, goCanh, goGiua);
    dai(b, v, 20, 1.0f, 0.89f, manTrai, toi(manTrai, 1.3f));
    dai(b, v, 20, 0.89f, 0.85f, cm ? vang : mau(176, 38, 32), cm ? vang : mau(176, 38, 32));
    dai(b, v, 20, 0.85f, 0.f, cm ? mau(92, 52, 36) : mau(104, 76, 46), cm ? mau(138, 86, 56) : mau(150, 108, 66));

    b.tri3(v.xb + 1.f, 0.f, v.xb - 16.f, -hwAt(v, v.xb - 16.f) * 0.95f, v.xb - 16.f, hwAt(v, v.xb - 16.f) * 0.95f, dongSang, dongToi, dongToi);

    if (!cm) {
        float cx = -L * 0.06f, hl = L * 0.13f, hw2 = B * 0.27f;
        b.rect(cx - hl - 1.f, -hw2 - 1.f, cx + hl + 1.f, hw2 + 1.f, mau(30, 20, 12));
        b.grad(cx - hl, -hw2, cx + hl, hw2, mau(120, 98, 66), mau(136, 112, 76), mau(104, 84, 56), mau(94, 76, 50));
    } else {
        float cx = -L * 0.05f, hl = L * 0.19f, hw2 = B * 0.34f;
        b.rect(cx - hl - 1.2f, -hw2 - 1.2f, cx + hl + 1.2f, hw2 + 1.2f, mau(26, 12, 10));
        b.grad(cx - hl, -hw2, cx + hl, hw2, mau(170, 48, 38), mau(190, 60, 44), mau(130, 34, 28), mau(120, 30, 26));
    }

    const float mx = L * 0.2f;
    b.ell(mx, 0.f, 2.9f, 2.9f, mau(40, 26, 16), 10);
    veBuom(b, mx + 1.f, B * 0.62f, 13.f, t, ph0, mau(58, 58, 68), mau(26, 26, 32), mau(212, 44, 36));
    if (cm) {
        veRong(b, mx + 1.f + 3.6f, 0.f, 3.4f, vang);
    }
    veCoBo(b, v.xs + 5.f, 0.f, cm ? 28.f : 18.f, cm ? 15.f : 10.f, t, ph0, mau(36, 34, 42), mau(212, 44, 36));

    // Hiệu ứng nháy trắng khi bị trúng tên/chém
    float flash = std::max(0.f, 1.f - (t - thoiDiemBiTrung) / 0.25f);
    if (flash > 0.f) dai(b, v, 18, 1.1f, 0.f, mau(255, 255, 255, 170.f * flash), mau(255, 255, 255, 170.f * flash));

    window.draw(va);
}
