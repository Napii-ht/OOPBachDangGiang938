
#pragma once
// ============================================================================
//  DoHoa.h - Bộ công cụ vẽ dùng chung cho Song / BanDo (header-only)
//  Mọi hình đều được "gom lô" vào sf::VertexArray (Triangles) => rất ít draw call.
// ============================================================================
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>

namespace dohoa {

using sf::Color;
using sf::Vector2f;
using VA = sf::VertexArray;

constexpr float PI = 3.14159265358979f;

// Đồng hồ toàn cục (giây) cho các hiệu ứng động của thuyền/cọc, không phụ thuộc deltaTime của game
inline float dongHo() {
    static const auto goc = std::chrono::steady_clock::now();
    return std::chrono::duration<float>(std::chrono::steady_clock::now() - goc).count();
}

// ----------------------------- Màu sắc -------------------------------------
inline std::uint8_t u8(float v) { return static_cast<std::uint8_t>(std::clamp(v, 0.f, 255.f)); }

inline Color mau(float r, float g, float b, float a = 255.f) { return Color(u8(r), u8(g), u8(b), u8(a)); }

inline Color pha(const Color& a, const Color& b, float t) {
    t = std::clamp(t, 0.f, 1.f);
    return mau(a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t,
               a.b + (b.b - a.b) * t, a.a + (b.a - a.a) * t);
}
inline Color toi(const Color& c, float k) { return mau(c.r * k, c.g * k, c.b * k, c.a); }   // k < 1: tối đi
inline Color sang(const Color& c, float k) { return pha(c, Color(255, 255, 255, c.a), k); } // k: 0..1 về phía trắng
inline Color vaA(const Color& c, float a) { return mau(c.r, c.g, c.b, a); }                // đổi alpha

// ----------------------------- Ngẫu nhiên / nhiễu ---------------------------
struct Rng {
    std::uint32_t s;
    explicit Rng(std::uint32_t seed = 1u) : s(seed ? seed : 1u) {}
    std::uint32_t next() { s ^= s << 13; s ^= s >> 17; s ^= s << 5; return s; }
    float f() { return static_cast<float>(next() & 0xFFFFFFu) / 16777216.f; }          // [0,1)
    float range(float a, float b) { return a + (b - a) * f(); }
    int irange(int a, int b) { return a + static_cast<int>(next() % static_cast<std::uint32_t>(b - a + 1)); }
};

inline float bam(int x, int y, std::uint32_t seed = 0) {
    std::uint32_t h = static_cast<std::uint32_t>(x) * 374761393u +
                      static_cast<std::uint32_t>(y) * 668265263u + seed * 2246822519u;
    h = (h ^ (h >> 13)) * 1274126177u;
    h ^= (h >> 16);
    return static_cast<float>(h & 0xFFFFu) / 65535.f;
}
inline float muot(float t) { return t * t * (3.f - 2.f * t); }
inline float nhieu(float x, float y, std::uint32_t seed = 0) {  // value-noise mượt [0,1]
    int xi = static_cast<int>(std::floor(x)), yi = static_cast<int>(std::floor(y));
    float fx = muot(x - static_cast<float>(xi)), fy = muot(y - static_cast<float>(yi));
    float a = bam(xi, yi, seed), b = bam(xi + 1, yi, seed);
    float c = bam(xi, yi + 1, seed), d = bam(xi + 1, yi + 1, seed);
    return a + (b - a) * fx + (c - a) * fy + (a - b - c + d) * fx * fy;
}

// ----------------------------- Nguyên liệu hình học -------------------------
inline void dinh(VA& va, Vector2f p, Color c) {
    sf::Vertex v;
    v.position = p;
    v.color = c;
    va.append(v);
}
inline void tam(VA& va, Vector2f a, Vector2f b, Vector2f c, Color col) {
    dinh(va, a, col); dinh(va, b, col); dinh(va, c, col);
}
inline void tam3(VA& va, Vector2f a, Vector2f b, Vector2f c, Color ca, Color cb, Color cc) {
    dinh(va, a, ca); dinh(va, b, cb); dinh(va, c, cc);
}
// Tứ giác tô màu theo đỉnh: p0 TL, p1 TR, p2 BR, p3 BL
inline void tu(VA& va, Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3,
               Color c0, Color c1, Color c2, Color c3) {
    tam3(va, p0, p1, p2, c0, c1, c2);
    tam3(va, p0, p2, p3, c0, c2, c3);
}
inline void chuNhat(VA& va, float x, float y, float w, float h, Color c) {
    tu(va, {x, y}, {x + w, y}, {x + w, y + h}, {x, y + h}, c, c, c, c);
}
inline void chuNhatDoc(VA& va, float x, float y, float w, float h, Color tren, Color duoi) {
    tu(va, {x, y}, {x + w, y}, {x + w, y + h}, {x, y + h}, tren, tren, duoi, duoi);
}
inline void chuNhatNgang(VA& va, float x, float y, float w, float h, Color trai, Color phai) {
    tu(va, {x, y}, {x + w, y}, {x + w, y + h}, {x, y + h}, trai, phai, phai, trai);
}
inline void elip(VA& va, float cx, float cy, float rx, float ry, Color c, int seg = 14) {
    for (int i = 0; i < seg; ++i) {
        float a0 = 2.f * PI * static_cast<float>(i) / static_cast<float>(seg);
        float a1 = 2.f * PI * static_cast<float>(i + 1) / static_cast<float>(seg);
        tam(va, {cx, cy}, {cx + std::cos(a0) * rx, cy + std::sin(a0) * ry},
            {cx + std::cos(a1) * rx, cy + std::sin(a1) * ry}, c);
    }
}
inline void elipTan(VA& va, float cx, float cy, float rx, float ry, Color tam_, Color vien, int seg = 16) {
    for (int i = 0; i < seg; ++i) {
        float a0 = 2.f * PI * static_cast<float>(i) / static_cast<float>(seg);
        float a1 = 2.f * PI * static_cast<float>(i + 1) / static_cast<float>(seg);
        tam3(va, {cx, cy}, {cx + std::cos(a0) * rx, cy + std::sin(a0) * ry},
             {cx + std::cos(a1) * rx, cy + std::sin(a1) * ry}, tam_, vien, vien);
    }
}
inline void vong(VA& va, float cx, float cy, float rx, float ry, float day, Color c, int seg = 24) {
    for (int i = 0; i < seg; ++i) {
        float a0 = 2.f * PI * static_cast<float>(i) / static_cast<float>(seg);
        float a1 = 2.f * PI * static_cast<float>(i + 1) / static_cast<float>(seg);
        Vector2f o0{cx + std::cos(a0) * rx, cy + std::sin(a0) * ry};
        Vector2f o1{cx + std::cos(a1) * rx, cy + std::sin(a1) * ry};
        Vector2f i0{cx + std::cos(a0) * (rx - day), cy + std::sin(a0) * (ry - day * 0.5f)};
        Vector2f i1{cx + std::cos(a1) * (rx - day), cy + std::sin(a1) * (ry - day * 0.5f)};
        tu(va, o0, o1, i1, i0, c, c, c, c);
    }
}
inline void duong(VA& va, Vector2f a, Vector2f b, float day, Color c) {
    Vector2f d = b - a;
    float len = std::sqrt(d.x * d.x + d.y * d.y);
    if (len < 0.0001f) return;
    Vector2f n{-d.y / len * day * 0.5f, d.x / len * day * 0.5f};
    tu(va, a + n, b + n, b - n, a - n, c, c, c, c);
}
inline void duongTan(VA& va, Vector2f a, Vector2f b, float dayA, float dayB, Color ca, Color cb) {
    Vector2f d = b - a;
    float len = std::sqrt(d.x * d.x + d.y * d.y);
    if (len < 0.0001f) return;
    Vector2f n{-d.y / len * 0.5f, d.x / len * 0.5f};
    tu(va, a + n * dayA, b + n * dayB, b - n * dayB, a - n * dayA, ca, cb, cb, ca);
}

}  // namespace dohoa


