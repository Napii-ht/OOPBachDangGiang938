
#include "Thuyen.h"
#include "DoHoa.h"
#include <cmath>
#include <cstdint>
#include <algorithm>

using namespace dohoa;

// =============================================================================
//  CÔNG CỤ VẼ THUYỀN (chỉ dùng nội bộ trong file này)
//  Mọi hình vẽ trong "hệ tọa độ thuyền": gốc tại tâm thuyền, mũi thuyền hướng +X, +Y là mạn dưới.
//  Bo tự lật ngang (sx) và xoay (nghiêng/lắc) khi quy ra tọa độ màn hình.
// =============================================================================
namespace {

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
    void lineT(float ax, float ay, float bx, float by, float tA, float tB, Color ca, Color cb) {
        duongTan(va, P(ax, ay), P(bx, by), tA, tB, ca, cb);
    }
};

// Biên dạng thân thuyền nhìn từ trên: xs = đuôi, xb = mũi, W = bề ngang lớn nhất
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

// Dải thân thuyền giữa hai tỉ lệ bề ngang kOut -> kIn (cả hai mạn)
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

// Vệt nước sau đuôi thuyền (chữ V + bọt xoáy giữa)
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

// Sóng rẽ ở mũi thuyền
void veSongMui(Bo& b, const Vo& v, float t, float cuong) {
    for (int s = -1; s <= 1; s += 2) {
        float sg = static_cast<float>(s);
        Vector2f a = b.P(v.xb + 1.f, 0.f);
        Vector2f c = b.P(v.xb - 16.f, sg * (hwAt(v, v.xb - 16.f) + 3.4f));
        duongTan(b.va, a, c, 3.4f, 1.2f, mau(240, 252, 255, 190.f * cuong), mau(240, 252, 255, 30.f * cuong));
        for (int i = 0; i < 4; ++i) {
            float x = v.xb - 4.f - static_cast<float>(i) * 4.f;
            float y = sg * (hwAt(v, x) + 1.6f + std::sin(t * 6.f + static_cast<float>(i)) * 0.7f);
            b.ell(x, y, 2.2f, 1.2f, mau(244, 252, 255, 170.f * cuong), 6);
        }
    }
}

// Mái chèo: gốc ở mạn thuyền, lưỡi chèo vung theo pha ph
void veMai(Bo& b, const Vo& v, float x, int side, float ph, float len, float swing, Color shaft, Color blade, bool vungNuoc) {
    float sg = static_cast<float>(side);
    float hx = hwAt(v, x) * 0.92f;
    float s = std::sin(ph);
    float tx = x + s * swing - 2.f, ty = sg * (hx + len);
    b.line(x, sg * hx * 0.5f, tx, ty, 1.6f, shaft);
    b.ell(tx, ty + sg * 1.5f, 2.0f, 3.9f, blade, 8);
    b.ell(tx - 0.4f, ty + sg * 0.6f, 0.9f, 1.8f, sang(blade, 0.35f), 6);
    if (vungNuoc && s > 0.55f) {
        float q = (s - 0.55f) / 0.45f;
        b.ell(tx, ty + sg * 2.f, 4.f + 2.f * q, 1.8f + 0.8f * q, mau(236, 250, 255, 90.f * (1.f - 0.3f * q)), 8);
    }
}

// Người lính nhìn từ trên: vai áo + nón/mũ tròn
void veLinhNon(Bo& b, float x, float y, Color ao, Color non, float r) {
    b.ell(x, y, r * 0.95f, r * 1.5f, toi(ao, 0.7f), 8);
    b.ell(x, y, r * 0.85f, r * 1.35f, ao, 8);
    b.ell(x + 0.5f, y + 0.5f, r * 1.04f, r * 1.04f, toi(non, 0.55f), 12);
    b.ell(x, y, r, r, non, 12);
    b.ell(x, y, r * 0.66f, r * 0.66f, sang(non, 0.1f), 10);
    b.ell(x - r * 0.2f, y - r * 0.22f, r * 0.34f, r * 0.34f, sang(non, 0.55f), 8);
}

// Buồm phồng nhìn từ trên: dải cong theo trục Y
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
    // xà ngang & dây buồm
    b.line(mx + bu * 0.1f, -halfLen - 2.f, mx + bu * 0.1f, halfLen + 2.f, 1.1f, mau(84, 56, 32));
}

// Rồng vàng cách điệu trên buồm
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

// Cờ phấp phới ở đuôi thuyền
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

// Ngọn lửa nhỏ + khói (cho chiến thuyền cháy)
void veLuaBo(Bo& b, float x, float y, float s, float t, float ph) {
    float fl = 0.9f + 0.1f * std::sin(t * 17.f + ph) * std::sin(t * 7.3f + ph * 2.f);
    b.ellT(x, y, 20.f * s * fl, 14.f * s * fl, mau(255, 150, 50, 95), mau(255, 120, 40, 0), 14);
    const float scl[3] = {1.f, 0.68f, 0.4f};
    const Color mc[3] = {mau(255, 84, 18, 235), mau(255, 170, 40, 240), mau(255, 244, 160, 245)};
    for (int l = 0; l < 3; ++l) {
        for (int tg = -1; tg <= 1; ++tg) {
            float off = static_cast<float>(tg) * 3.4f * s * scl[l];
            float fh = (13.f + 6.f * std::sin(t * 11.f + static_cast<float>(tg) * 2.1f + ph)) * scl[l] * s * (tg == 0 ? 1.25f : 0.9f);
            float tx = x + off * 0.6f + std::sin(t * 9.f + ph + static_cast<float>(tg)) * 1.8f * s;
            b.tri(x + off - 4.f * s * scl[l], y, x + off + 4.f * s * scl[l], y, tx, y - fh, mc[l]);
        }
        b.ell(x, y - 2.f * s * scl[l], 5.f * s * scl[l], 3.8f * s * scl[l], mc[l], 9);
    }
    for (int i = 0; i < 3; ++i) {
        float p = std::fmod(t * 0.35f + static_cast<float>(i) * 0.34f + ph, 1.f);
        float sy = y - 14.f * s - p * 42.f * s;
        float sx2 = x + 5.f * p * static_cast<float>(i + 1) * s + std::sin(p * 5.f + ph) * 2.f * s;
        float r = (3.f + 8.f * p) * s;
        b.ell(sx2, sy, r, r * 0.8f, mau(104, 94, 98, (1.f - p) * 120.f), 9);
    }
    for (int i = 0; i < 4; ++i) {
        float p = std::fmod(t * 0.8f + static_cast<float>(i) * 0.25f + ph, 1.f);
        float sx2 = x + std::sin(p * 6.f + static_cast<float>(i) * 2.3f) * 6.f * s;
        float sy = y - 8.f * s - p * 30.f * s;
        b.rect(sx2, sy, sx2 + 1.4f, sy + 1.4f, mau(255, 200, 90, (1.f - p) * 230.f));
    }
}

// Xác thuyền chìm: ván gỗ trôi, mảnh vải, vệt dầu, bong bóng nổi lên
void veXacThuyen(VA& va, Vector2f c, float t, std::uint32_t sd, float L, bool daiViet) {
    elip(va, c.x, c.y + 2.f, L * 0.55f, L * 0.2f, mau(10, 14, 20, 70), 16);
    elip(va, c.x + 4.f, c.y + 3.f, L * 0.38f, L * 0.12f, mau(60, 44, 70, 40), 14);
    for (int i = 0; i < 7; ++i) {
        float ox = (bam(i, 1, sd) - 0.5f) * L * 0.95f;
        float oy = (bam(i, 2, sd) - 0.5f) * L * 0.4f + std::sin(t * 1.1f + static_cast<float>(i)) * 0.9f;
        float rot = (bam(i, 3, sd) - 0.5f) * 3.f;
        float len = L * (0.1f + 0.1f * bam(i, 4, sd));
        Bo pb(va, {c.x + ox, c.y + oy}, 1.f, rot);
        pb.rect(-len, -1.8f, len, 1.8f, mau(44, 26, 14, 220));
        pb.grad(-len + 0.6f, -1.2f, len - 0.6f, 1.2f, mau(150, 102, 58, 235), mau(150, 102, 58, 235), mau(98, 64, 36, 235), mau(98, 64, 36, 235));
        if (i % 3 == 0) pb.rect(-len * 0.2f, -2.6f, len * 0.2f, -1.8f, mau(70, 44, 24, 220));
    }
    // mảnh buồm/cờ trôi
    Bo cb(va, {c.x - L * 0.12f, c.y + 3.f}, 1.f, 0.4f + std::sin(t * 0.8f) * 0.1f);
    if (daiViet) cb.rect(-6.f, -3.f, 6.f, 3.f, mau(190, 40, 34, 220));
    else         cb.rect(-7.f, -3.f, 7.f, 3.f, mau(44, 44, 52, 220));
    cb.line(-6.f, -3.f, 6.f, -3.f, 1.f, mau(236, 186, 70, 220));
    // bong bóng nổi
    for (int i = 0; i < 5; ++i) {
        float p = std::fmod(t * 0.5f + static_cast<float>(i) * 0.2f, 1.f);
        float r = 1.5f + 5.f * p;
        vong(va, c.x + (bam(i, 6, sd) - 0.5f) * L * 0.5f, c.y + (bam(i, 7, sd) - 0.5f) * 8.f, r * 1.6f, r * 0.6f, 0.9f,
             mau(222, 242, 252, (1.f - p) * 120.f), 12);
    }
}

}  // namespace

// =============================================================================
//  PHẦN LOGIC GAME - GIỮ NGUYÊN NHƯ BẢN GỐC (chỉ thêm các dòng ghi chú "Mới")
// =============================================================================

Thuyen::Thuyen(sf::Vector2f viTriBanDau, float dai_, float rong, float spd, int mau_)
    : viTri(viTriBanDau),
      huongDiChuyen(1.f, 0.f),
      tocDo(spd),
      sucKhoe(mau_),
      sucKhoeToiDa(mau_),
      daMacCoc(false),
      daBiPhaHuy(false),
      chieuDai(dai_),
      chieuRong(rong),
      gocXoay(0.f) {}

sf::FloatRect Thuyen::GetHitBox() const {
    return sf::FloatRect(
        {viTri.x - chieuDai * 0.5f, viTri.y - chieuRong * 0.5f},
        {chieuDai, chieuRong}
    );
}

void Thuyen::NhanSatThuong(int damage) {
    thoiDiemBiTrung = dongHo();   // Mới: kích hoạt hiệu ứng nháy trắng khi trúng đòn
    sucKhoe -= damage;
    if (sucKhoe <= 0) {
        sucKhoe = 0;
        daBiPhaHuy = true;
    }
}

bool Thuyen::KiemTraVaChamCoc(Coc& coc, float tiLeThuyTrieu) {
    if (daBiPhaHuy || !coc.IsDaCam()) return false;

    // Chỉ khi thủy triều rút xuống dưới 0.65 thì cọc mới đâm thủng được đáy thuyền
    if (tiLeThuyTrieu > 0.65f) return false;

    if (GetHitBox().findIntersection(coc.GetHitBox()).has_value()) {
        if (!coc.IsDaPhatNoi()) {
            NhanSatThuong(coc.GetSatThuong());
            coc.SetDaPhatNoi(true);
            SetMacCoc(true);
            return true;
        }
    }
    return false;
}

// ----------------- THUYỀN CHIẾN ĐẠI VIỆT -----------------

ThuyenNguoiChoi::ThuyenNguoiChoi(sf::Vector2f viTriBanDau)
    : Thuyen(viTriBanDau, 68.f, 28.f, 185.f, 250),
      gocLach(0.f) {}

void ThuyenNguoiChoi::XuLyPhim() {
    if (daBiPhaHuy) return;

    sf::Vector2f huong(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        huong.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        huong.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        huong.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        huong.x += 1.f;

    float len = std::sqrt(huong.x * huong.x + huong.y * huong.y);
    if (len > 0.001f) {
        huongDiChuyen = huong / len;
        // Mới: quay đầu thuyền theo hướng đi ngang gần nhất
        if (huongDiChuyen.x > 0.2f) huongMat = 1.f;
        else if (huongDiChuyen.x < -0.2f) huongMat = -1.f;
        // Góc nghiêng lượn sóng
        gocLach = huongDiChuyen.y * 12.f;
        huongMuon = huongDiChuyen;
    } else {
        gocLach *= 0.9f;
        huongMuon = {0.f, 0.f};
    }
}

void ThuyenNguoiChoi::CapNhat(float deltaTime) {
    if (daBiPhaHuy) return;

    // Mới: lật ngang mượt khi quay đầu
    sxLat += (huongMat - sxLat) * std::min(1.f, deltaTime * 12.f);

    if (cheDoMuot) {
        // Tăng tốc dần về hướng muốn đi, thả phím thì hãm lại; dòng chảy cuốn thuyền xuôi theo nước
        sf::Vector2f mucTieu = huongMuon * tocDo * heSoToc;
        float heSo = (huongMuon.x != 0.f || huongMuon.y != 0.f) ? 4.5f : 3.2f;
        vanToc += (mucTieu - vanToc) * std::min(1.f, heSo * deltaTime);
        viTri += (vanToc + dongChay) * deltaTime;
        viTri.x = std::clamp(viTri.x, gioiHan.position.x, gioiHan.position.x + gioiHan.size.x);
        viTri.y = std::clamp(viTri.y, gioiHan.position.y, gioiHan.position.y + gioiHan.size.y);
        return;
    }

    // Giữ thuyền di chuyển trong sông Bạch Đằng
    viTri += huongDiChuyen * tocDo * deltaTime;
    viTri.x = std::clamp(viTri.x, 60.f, 1140.f);
    viTri.y = std::clamp(viTri.y, 180.f, 520.f);
}

void ThuyenNguoiChoi::Ve(sf::RenderWindow& window) {
    static sf::VertexArray va(sf::PrimitiveType::Triangles);
    va.clear();

    const float t = dongHo();
    const std::uint32_t sd = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(this) >> 4) & 0xFFFFu;
    const float ph0 = static_cast<float>(sd & 255u) / 40.f;

    if (daBiPhaHuy) {   // Mới: xác thuyền thay vì biến mất hẳn
        veXacThuyen(va, viTri, t, sd, chieuDai, true);
        window.draw(va);
        return;
    }

    const float L = chieuDai, B = chieuRong;
    const float sg = huongMat >= 0.f ? 1.f : -1.f;
    const float ang = gocLach * PI / 180.f * sg + std::sin(t * 1.3f + ph0) * 0.03f;
    Bo b(va, {viTri.x, viTri.y + std::sin(t * 1.7f + ph0) * 0.9f}, sxLat, ang);
    const Vo v{-L * 0.5f, L * 0.5f + 14.f, B, 2.3f, 4.2f};

    // bóng dưới nước + vệt nước + sóng mũi
    b.ell(2.f, 4.f, L * 0.62f, B * 0.6f, mau(0, 0, 0, 70), 18);
    veVetNuoc(b, v, t, 1.f, 62.f);

    // mái chèo (vẽ trước thân để gốc chèo nằm dưới mạn thuyền)
    const float xc[4] = {-22.f, -8.f, 6.f, 20.f};
    for (int i = 0; i < 4; ++i)
        for (int s = -1; s <= 1; s += 2)
            veMai(b, v, xc[i], s, t * 5.2f + static_cast<float>(i) * 0.6f + (s > 0 ? 0.5f : 0.f), 15.f, 6.f,
                  mau(138, 92, 50), mau(178, 132, 78), true);

    // thân thuyền: viền, gỗ, mạn sơn đỏ, viền vàng, sàn
    dai(b, v, 18, 1.15f, 0.f, mau(46, 24, 12), mau(46, 24, 12));
    dai(b, v, 18, 1.0f, 0.f, mau(100, 60, 32), mau(180, 122, 68));
    dai(b, v, 18, 1.0f, 0.86f, mau(180, 42, 32), mau(150, 32, 26));
    dai(b, v, 18, 0.86f, 0.81f, mau(240, 188, 72), mau(240, 188, 72));
    dai(b, v, 18, 0.81f, 0.f, mau(146, 100, 58), mau(204, 154, 92));
    for (float x = v.xs + 5.f; x < v.xb - 9.f; x += 4.6f) {
        float h = hwAt(v, x) * 0.8f;
        b.line(x, -h, x, h, 0.7f, mau(72, 46, 26, 110));
    }
    for (int i = 0; i < 4; ++i) {   // ghế chèo
        float h = hwAt(v, xc[i]) * 0.8f;
        b.rect(xc[i] - 1.7f, -h, xc[i] + 1.7f, h, mau(110, 72, 40));
        b.rect(xc[i] - 1.7f, -h, xc[i] - 0.6f, h, mau(160, 112, 66));
    }
    // mũi: mỏ thuyền mạ vàng + đôi mắt thuyền
    b.tri(v.xb - 1.f, -1.8f, v.xb + 5.5f, 0.f, v.xb - 1.f, 1.8f, mau(240, 188, 72));
    b.tri(v.xb - 1.f, -1.8f, v.xb + 5.5f, 0.f, v.xb + 1.f, -0.4f, mau(255, 232, 150));
    {
        float xe = v.xb - 11.f, ye = hwAt(v, xe) * 0.72f;
        for (int s = -1; s <= 1; s += 2) {
            float sg2 = static_cast<float>(s);
            b.ell(xe, sg2 * ye, 2.5f, 2.0f, mau(250, 250, 244), 8);
            b.ell(xe + 0.6f, sg2 * ye, 1.15f, 1.15f, mau(20, 16, 14), 6);
        }
    }
    b.rect(v.xs + 0.4f, -hwAt(v, v.xs + 3.f) * 0.8f, v.xs + 3.2f, hwAt(v, v.xs + 3.f) * 0.8f, mau(86, 54, 30));   // bảng đuôi

    // quân sĩ: 4 người chèo + 1 thủ lĩnh
    for (int i = 0; i < 2; ++i)
        for (int s = -1; s <= 1; s += 2)
            veLinhNon(b, xc[i], static_cast<float>(s) * 5.4f, mau(100, 68, 42), mau(218, 190, 116), 3.9f);
    veLinhNon(b, 21.f, 0.f, mau(156, 38, 30), mau(240, 202, 92), 4.3f);

    // cột buồm + buồm đỏ Đại Việt
    b.line(3.f, 0.f, v.xb - 2.f, 0.f, 0.6f, mau(228, 212, 176, 110));
    b.line(3.f, 0.f, v.xs + 2.f, 0.f, 0.6f, mau(228, 212, 176, 110));
    b.ell(3.f, 0.f, 2.6f, 2.6f, mau(60, 38, 20), 10);
    b.ell(2.6f, -0.4f, 1.5f, 1.5f, mau(150, 104, 60), 8);
    veBuom(b, 4.f, 17.f, 11.f, t, ph0, mau(224, 52, 46), mau(168, 30, 28), mau(244, 196, 80));
    b.ell(4.f + 3.4f, 0.f, 3.4f, 3.4f, mau(244, 196, 80), 10);
    b.ell(4.f + 3.4f, 0.f, 2.1f, 2.1f, mau(255, 230, 130), 8);

    // cờ nheo ở đuôi
    veCoBo(b, v.xs + 4.f, 0.f, 15.f, 8.f, t, ph0, mau(206, 36, 32), mau(244, 196, 80));

    // sóng ở mũi (vẽ trên cùng vì nước bắn lên)
    veSongMui(b, v, t, 1.f);

    // nháy trắng khi trúng đòn
    float flash = std::max(0.f, 1.f - (t - thoiDiemBiTrung) / 0.25f);
    if (flash > 0.f) dai(b, v, 18, 1.1f, 0.f, mau(255, 255, 255, 170.f * flash), mau(255, 255, 255, 170.f * flash));

    window.draw(va);
}

// ----------------- THUYỀN CHIẾN NAM HÁN -----------------

ThuyenKeDich::ThuyenKeDich(sf::Vector2f viTriBanDau, bool soChiHuy)
    : Thuyen(viTriBanDau, soChiHuy ? 115.f : 85.f, soChiHuy ? 46.f : 36.f, soChiHuy ? 70.f : 95.f, soChiHuy ? 350 : 180),
      tenThuyen(soChiHuy ? "Soái hạm Lưu Hoằng Tháo" : "Chiến thuyền Nam Hán"),
      thoiGianKhoiBoc(0.f),
      thuyenSoChiHuy(soChiHuy) {}

void ThuyenKeDich::DuoiTheo(sf::Vector2f viTriThuyenPlayer, float deltaTime) {
    if (daBiPhaHuy || daMacCoc) return;

    // AI truy đuổi tàu nhẹ Đại Việt
    float dx = viTriThuyenPlayer.x - viTri.x;
    float dy = viTriThuyenPlayer.y - viTri.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    // Mới: mũi thuyền quay về phía người chơi, lật mượt
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
        // Tàu đã chìm, chỉ còn ván gỗ, cờ rách và bong bóng trôi dạt
        veXacThuyen(va, viTri, t, sd, chieuDai, false);
        window.draw(va);
        return;
    }

    const float L = chieuDai, B = chieuRong;
    const float sg = huongMat >= 0.f ? 1.f : -1.f;
    const float ratio = std::clamp(static_cast<float>(sucKhoe) / static_cast<float>(sucKhoeToiDa), 0.f, 1.f);
    float ang;
    if (daMacCoc) ang = 0.09f + std::sin(thoiGianKhoiBoc * 2.f) * 0.015f;   // nghiêng sườn vì mắc cọc
    else          ang = huongDiChuyen.y * 0.2f * sg;
    ang += std::sin(t * 1.1f + ph0) * 0.02f;

    Bo b(va, {viTri.x, viTri.y + (daMacCoc ? 0.4f : std::sin(t * 1.5f + ph0) * 0.8f)}, sxLat, ang);
    const Vo v{-L * 0.5f, L * 0.5f + 18.f, B, 2.0f, 3.6f};

    const Color goCanh  = cm ? mau(56, 18, 18) : mau(52, 32, 22);
    const Color goGiua  = cm ? mau(128, 44, 34) : mau(108, 66, 38);
    const Color manTrai = mau(34, 30, 34);
    const Color vang    = mau(232, 178, 56);
    const Color dongSang = mau(238, 196, 96), dongToi = mau(138, 92, 36);

    // bóng + sóng
    b.ell(3.f, 5.f, L * 0.62f, B * 0.6f, mau(0, 0, 0, 80), 20);
    if (!daMacCoc) {
        veVetNuoc(b, v, t, cm ? 0.9f : 1.f, cm ? 78.f : 64.f);
    } else {
        // mắc cọc: nước sủi bọt quanh thân
        for (int i = 0; i < 9; ++i) {
            float p = std::fmod(t * 0.9f + static_cast<float>(i) * 0.11f, 1.f);
            float a = static_cast<float>(i) * 0.7f;
            b.ell(std::cos(a) * L * 0.5f * (1.f + 0.15f * p), std::sin(a) * B * 0.62f * (1.f + 0.25f * p), 3.f + 3.f * p, 1.6f + 1.2f * p,
                  mau(238, 249, 255, (1.f - p) * 150.f), 7);
        }
    }

    // mái chèo
    const int soChoi = std::max(4, static_cast<int>(L / 14.f));
    for (int i = 0; i < soChoi; ++i) {
        float x = v.xs + 14.f + (v.xb - v.xs - 46.f) * static_cast<float>(i) / static_cast<float>(soChoi - 1);
        for (int s = -1; s <= 1; s += 2) {
            float ph = daMacCoc ? (0.6f + static_cast<float>(i) * 0.55f + (s > 0 ? 1.1f : 0.f))   // chèo gãy buông xõa
                                : t * 3.4f + static_cast<float>(i) * 0.5f + (s > 0 ? 0.5f : 0.f);
            veMai(b, v, x, s, ph, cm ? 15.f : 13.f, 5.f, mau(60, 40, 26), mau(94, 64, 40), !daMacCoc);
        }
    }

    // thân tàu: viền, gỗ sẫm, mạn sơn đen, viền, sàn
    dai(b, v, 20, 1.13f, 0.f, mau(22, 12, 8), mau(22, 12, 8));
    dai(b, v, 20, 1.0f, 0.f, goCanh, goGiua);
    dai(b, v, 20, 1.0f, 0.89f, manTrai, toi(manTrai, 1.3f));
    dai(b, v, 20, 0.89f, 0.85f, cm ? vang : mau(176, 38, 32), cm ? vang : mau(176, 38, 32));
    dai(b, v, 20, 0.85f, 0.f, cm ? mau(92, 52, 36) : mau(104, 76, 46), cm ? mau(138, 86, 56) : mau(150, 108, 66));
    for (float x = v.xs + 5.f; x < v.xb - 12.f; x += 5.f) {
        float h = hwAt(v, x) * 0.84f;
        b.line(x, -h, x, h, 0.7f, mau(40, 26, 16, 120));
    }

    // mũi tàu bọc đồng + đôi mắt hung tợn
    b.tri3(v.xb + 1.f, 0.f, v.xb - 16.f, -hwAt(v, v.xb - 16.f) * 0.95f, v.xb - 16.f, hwAt(v, v.xb - 16.f) * 0.95f, dongSang, dongToi, dongToi);
    b.line(v.xb, 0.f, v.xb - 16.f, 0.f, 0.9f, mau(70, 44, 18));
    b.line(v.xb - 8.f, -hwAt(v, v.xb - 8.f) * 0.7f, v.xb - 8.f, hwAt(v, v.xb - 8.f) * 0.7f, 0.8f, mau(70, 44, 18, 160));
    {
        float xe = v.xb - 24.f, ye = hwAt(v, xe) * 0.55f;
        for (int s = -1; s <= 1; s += 2) {
            float sg2 = static_cast<float>(s);
            b.ell(xe, sg2 * ye, 2.7f, 2.2f, mau(210, 30, 24), 8);
            b.ell(xe + 0.5f, sg2 * ye, 1.0f, 1.5f, mau(255, 224, 90), 6);
        }
    }

    // hàng khiên tròn dọc mạn + lính mũ giáp
    for (float x = v.xs + 9.f; x < v.xb - 30.f; x += 9.f) {
        int idx = static_cast<int>((x - v.xs) / 9.f);
        for (int s = -1; s <= 1; s += 2) {
            float sg2 = static_cast<float>(s);
            float y = sg2 * hwAt(v, x) * 0.9f;
            Color c = (idx % 2 == 0) ? mau(186, 40, 32) : mau(40, 38, 46);
            b.ell(x, y, 3.5f, 3.5f, toi(c, 0.5f), 10);
            b.ell(x, y, 3.0f, 3.0f, c, 10);
            b.ell(x, y, 1.1f, 1.1f, vang, 6);
        }
    }
    for (float x = v.xs + 13.f; x < v.xb - 34.f; x += 12.f) {
        for (int s = -1; s <= 1; s += 2) {
            float y = static_cast<float>(s) * hwAt(v, x) * 0.56f;
            b.ell(x, y, 2.4f, 4.0f, mau(52, 40, 40), 8);
            b.ell(x, y, 2.5f, 2.5f, mau(142, 150, 162), 8);
            b.ell(x - 0.4f, y - 0.4f, 1.0f, 1.0f, mau(220, 228, 236), 6);
            b.ell(x + 0.4f, y + 0.9f, 0.8f, 0.8f, mau(200, 40, 34), 5);
        }
    }

    // lầu thuyền
    if (!cm) {
        float cx = -L * 0.06f, hl = L * 0.13f, hw2 = B * 0.27f;
        b.rect(cx - hl - 1.f, -hw2 - 1.f, cx + hl + 1.f, hw2 + 1.f, mau(30, 20, 12));
        b.grad(cx - hl, -hw2, cx + hl, hw2, mau(120, 98, 66), mau(136, 112, 76), mau(104, 84, 56), mau(94, 76, 50));
        for (float x = cx - hl + 3.f; x < cx + hl; x += 4.5f) b.line(x, -hw2, x, hw2, 0.9f, mau(60, 44, 26, 160));
        b.line(cx - hl, 0.f, cx + hl, 0.f, 1.2f, mau(204, 176, 112, 180));
    } else {
        float cx = -L * 0.05f, hl = L * 0.19f, hw2 = B * 0.34f;
        b.rect(cx - hl - 1.2f, -hw2 - 1.2f, cx + hl + 1.2f, hw2 + 1.2f, mau(26, 12, 10));
        b.grad(cx - hl, -hw2, cx + hl, hw2, mau(170, 48, 38), mau(190, 60, 44), mau(130, 34, 28), mau(120, 30, 26));
        for (float x = cx - hl + 2.5f; x < cx + hl; x += 3.6f) b.line(x, -hw2, x, hw2, 0.8f, mau(70, 14, 12, 150));
        b.line(cx - hl, 0.f, cx + hl, 0.f, 2.f, vang);
        b.line(cx - hl, -hw2, cx + hl, -hw2, 1.4f, vang);
        b.line(cx - hl, hw2, cx + hl, hw2, 1.4f, vang);
        float hl2 = hl * 0.55f, hw3 = hw2 * 0.55f;
        b.rect(cx - hl2 - 1.f, -hw3 - 1.f, cx + hl2 + 1.f, hw3 + 1.f, mau(26, 12, 10));
        b.grad(cx - hl2, -hw3, cx + hl2, hw3, mau(120, 30, 26), mau(150, 42, 34), mau(100, 24, 22), mau(90, 22, 20));
        b.line(cx - hl2, 0.f, cx + hl2, 0.f, 1.6f, vang);
        for (int s = -1; s <= 1; s += 2)
            for (int e = -1; e <= 1; e += 2)
                b.ell(cx + static_cast<float>(e) * hl, static_cast<float>(s) * hw2, 1.6f, 1.6f, vang, 6);
        // hai chiếc trống trận ở đuôi
        for (int s = -1; s <= 1; s += 2) {
            float sg2 = static_cast<float>(s);
            b.ell(v.xs + 10.f, sg2 * B * 0.2f, 4.9f, 4.9f, mau(60, 20, 18), 12);
            b.ell(v.xs + 10.f, sg2 * B * 0.2f, 4.2f, 4.2f, mau(188, 52, 40), 12);
            b.ell(v.xs + 10.f, sg2 * B * 0.2f, 2.2f, 2.2f, vang, 10);
            b.ell(v.xs + 9.4f, sg2 * B * 0.2f - 0.6f, 0.9f, 0.9f, mau(255, 226, 150), 6);
        }
    }

    // cột buồm + buồm chiến đen viền đỏ
    const float mx = L * 0.2f;
    b.ell(mx, 0.f, 2.9f, 2.9f, mau(40, 26, 16), 10);
    b.ell(mx - 0.4f, -0.4f, 1.6f, 1.6f, mau(110, 76, 44), 8);
    veBuom(b, mx + 1.f, B * 0.62f, 13.f, t, ph0, mau(58, 58, 68), mau(26, 26, 32), mau(212, 44, 36));
    if (cm) {
        veRong(b, mx + 1.f + 3.6f, 0.f, 3.4f, vang);
    } else {
        b.ell(mx + 1.f + 3.6f, 0.f, 3.8f, 3.8f, mau(200, 36, 30), 10);
        b.ell(mx + 1.f + 3.6f, 0.f, 1.7f, 1.7f, vang, 8);
    }

    // cờ hiệu Nam Hán
    veCoBo(b, v.xs + 5.f, 0.f, cm ? 28.f : 18.f, cm ? 15.f : 10.f, t, ph0, mau(36, 34, 42), mau(212, 44, 36));

    // hư hại: lỗ thủng ván sàn
    auto lo = [&](float x, float y, float r) {
        b.ell(x, y, r * 1.2f, r, mau(14, 8, 6), 8);
        for (int k = 0; k < 5; ++k) {
            float a = 2.f * PI * static_cast<float>(k) / 5.f + x;
            b.tri(x + std::cos(a) * r * 0.9f, y + std::sin(a) * r * 0.8f,
                  x + std::cos(a + 0.3f) * r * 1.7f, y + std::sin(a + 0.3f) * r * 1.5f,
                  x + std::cos(a + 0.6f) * r * 0.9f, y + std::sin(a + 0.6f) * r * 0.8f, mau(168, 122, 70));
        }
    };
    if (ratio < 0.75f) lo(-L * 0.24f, B * 0.12f, 3.2f);
    if (ratio < 0.5f)  lo(L * 0.08f, -B * 0.2f, 3.6f);
    if (ratio < 0.28f) lo(-L * 0.34f, -B * 0.14f, 4.0f);
    if (ratio < 0.28f) b.ell(0.f, 0.f, L * 0.5f, B * 0.45f, mau(18, 56, 70, 60), 16);   // nước tràn vào thân

    // cháy khi mắc cọc hoặc bị trúng hỏa công: vết nứt, vết cháy, lửa và khói
    if (daMacCoc || dangChay) {
        b.ell(-L * 0.05f, B * 0.1f, L * 0.2f, B * 0.2f, mau(10, 8, 8, 120), 12);
        float px = -L * 0.2f, py = -B * 0.38f;
        for (int k = 1; k <= 5; ++k) {
            float nx = px + 5.f + 3.f * bam(k, 1, sd);
            float ny = py + (bam(k, 2, sd) - 0.3f) * 8.f;
            b.line(px, py, nx, ny, 1.1f, mau(18, 10, 6));
            px = nx; py = ny;
        }
        float k = std::min(1.f, 0.3f + thoiGianKhoiBoc * 0.35f);
        veLuaBo(b, -L * 0.22f, B * 0.08f, 1.35f * k + 0.3f, t, ph0);
        veLuaBo(b, L * 0.05f, -B * 0.18f, 1.1f * k + 0.3f, t, ph0 + 2.f);
        if (ratio < 0.5f || dangChay) veLuaBo(b, -L * 0.36f, -B * 0.1f, 1.2f, t, ph0 + 4.f);
        if (dangChay) {
            veLuaBo(b, 0.f, 0.f, 1.6f, t, ph0 + 1.2f);
            veLuaBo(b, L * 0.22f, 0.f, 1.4f, t, ph0 + 3.1f);
        }
        // khói bốc to hơn (giữ nhịp phập phồng như bản cũ)
        float kh = (dangChay ? 14.f : 8.f) + std::sin(thoiGianKhoiBoc * 6.f) * 4.f;
        b.ell(-L * 0.1f, -B * 0.5f - 6.f, kh * 1.3f, kh, mau(110, 108, 112, 140), 12);
    }

    // sóng rẽ mũi
    if (!daMacCoc) veSongMui(b, v, t, 0.9f);

    // nháy trắng khi trúng đòn
    float flash = std::max(0.f, 1.f - (t - thoiDiemBiTrung) / 0.25f);
    if (flash > 0.f) {
        dai(b, v, 20, 1.1f, 0.f, mau(255, 255, 255, 170.f * flash), mau(255, 255, 255, 170.f * flash));
        dai(b, v, 20, 1.1f, 0.f, mau(255, 120, 90, 60.f * flash), mau(255, 120, 90, 60.f * flash));
    }

    // ---------------- Thanh máu trên đầu chiến thuyền ----------------
    {
        const float rong = chieuDai * 0.8f;
        const float cx = viTri.x, cy = viTri.y - chieuRong * 0.7f - 10.f;
        const float x0 = cx - rong * 0.5f, y0 = cy - 3.f;
        Color vien = cm ? mau(218, 165, 32) : mau(18, 16, 18);
        chuNhat(va, x0 - 1.8f, y0 - 1.8f, rong + 3.6f, 9.6f, mau(10, 8, 8, 210));
        chuNhat(va, x0 - 1.4f, y0 - 1.4f, rong + 2.8f, 1.f, vien);
        chuNhat(va, x0 - 1.4f, y0 + 6.4f, rong + 2.8f, 1.f, vien);
        chuNhat(va, x0 - 1.4f, y0 - 1.4f, 1.f, 8.8f, vien);
        chuNhat(va, x0 + rong + 0.4f, y0 - 1.4f, 1.f, 8.8f, vien);
        chuNhat(va, x0, y0, rong, 6.f, mau(34, 26, 26));
        if (ratio > 0.f) {
            float lowPulse = (ratio < 0.25f) ? 0.75f + 0.25f * std::sin(t * 9.f) : 1.f;
            chuNhatDoc(va, x0, y0, rong * ratio, 6.f, mau(250, 96, 70, 255.f * lowPulse), mau(168, 22, 22, 255.f * lowPulse));
            chuNhat(va, x0, y0, rong * ratio, 1.2f, mau(255, 200, 180, 150));
        }
        for (int i = 1; i < 4; ++i) chuNhat(va, x0 + rong * static_cast<float>(i) * 0.25f - 0.4f, y0, 0.8f, 6.f, mau(10, 8, 8, 170));
        if (flash > 0.f) chuNhat(va, x0, y0, rong, 6.f, mau(255, 255, 255, 140.f * flash));
        if (cm) {   // biểu tượng soái hạm: hình thoi vàng
            tam(va, {x0 - 6.f, cy}, {x0 - 2.f, cy - 4.f}, {x0 + 0.f, cy}, vang);
            tam(va, {x0 - 6.f, cy}, {x0 - 2.f, cy + 4.f}, {x0 + 0.f, cy}, toi(vang, 0.75f));
        }
    }

    window.draw(va);
}


