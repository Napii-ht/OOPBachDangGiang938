
#include "Coc.h"
#include "DoHoa.h"
#include <algorithm>
#include <cmath>

using namespace dohoa;

Coc::Coc(sf::Vector2f pos, float rong, float cao, bool cam)
    : viTri(pos),
      chieuRong(rong),
      chieuCao(cao),
      daCam(cam),
      satThuongVaCham(60),
      daPhatNoi(false) {}

sf::FloatRect Coc::GetHitBox() const {
    return sf::FloatRect(
        {viTri.x - chieuRong * 0.5f, viTri.y - chieuCao * 0.5f},
        {chieuRong, chieuCao}
    );
}

void Coc::Ve(sf::RenderWindow& window, float tiLeThuyTrieu) {
    if (!daCam) return;

    // Quy ước độ ngập: tiLeThuyTrieu = 1.0 -> cọc chìm hẳn dưới nước đục, 0.0 -> cọc nhô lên trơ trọi.
    // (Cọc chỉ đâm thủng thuyền khi tiLeThuyTrieu <= 0.65 - lúc đó chỉ còn mũi sắt nhô khỏi mặt nước.)
    const float tl = std::clamp(tiLeThuyTrieu, 0.0f, 1.0f);
    const float t = dongHo();

    static sf::VertexArray va(sf::PrimitiveType::Triangles);
    va.clear();

    const float w = chieuRong, h = chieuCao;
    const float yDinh = -0.5f * h, yKhop = -0.2f * h, yDay = 0.5f * h;
    const float nuaThan = w * 0.47f;          // nửa bề ngang thân
    const float nuaNon = w * 0.6f;            // nửa bề ngang chân mũi nón
    const std::uint32_t hat = static_cast<std::uint32_t>(static_cast<int>(viTri.x * 3.f + viTri.y * 7.f)) * 2654435761u;
    const float pha0 = static_cast<float>(hat & 255u) / 255.f;

    // Cọc đã đâm trúng thuyền thì hơi nghiêng đi
    const float goc = daPhatNoi ? 0.09f : 0.f;
    const float co = std::cos(goc), si = std::sin(goc);
    auto P = [&](float x, float y) -> Vector2f {
        float dx = x, dy = y - yDay;
        return {viTri.x + dx * co - dy * si, viTri.y + yDay + dx * si + dy * co};
    };
    auto quad = [&](float x0, float y0, float x1, float y1, Color c00, Color c10, Color c11, Color c01) {
        tu(va, P(x0, y0), P(x1, y0), P(x1, y1), P(x0, y1), c00, c10, c11, c01);
    };
    auto tri = [&](float ax, float ay, float bx, float by, float cx, float cy, Color ca, Color cb, Color cc) {
        tam3(va, P(ax, ay), P(bx, by), P(cx, cy), ca, cb, cc);
    };
    auto dot = [&](float cx, float cy, float rx, float ry, Color c) {
        Vector2f ct = P(cx, cy);
        elip(va, ct.x, ct.y, rx, ry, c, 8);
    };
    auto nhoMat = [&](float y) {  // nửa bề ngang mũi nón tại độ cao y (yDinh -> 0, yKhop -> nuaNon)
        return nuaNon * std::clamp((y - yDinh) / (yKhop - yDinh), 0.f, 1.f);
    };

    // ------------------------------------------------------------------ viền tối + thân gỗ
    quad(-nuaThan - 1.3f, yKhop - 1.f, nuaThan + 1.3f, yDay + 1.f, mau(36, 20, 10), mau(36, 20, 10), mau(30, 16, 8), mau(30, 16, 8));
    const int hang = 9;
    for (int r = 0; r < hang; ++r) {
        float ya = yKhop + (yDay - yKhop) * static_cast<float>(r) / hang;
        float yb = yKhop + (yDay - yKhop) * static_cast<float>(r + 1) / hang;
        float jit = 0.86f + 0.24f * bam(r, static_cast<int>(hat & 1023u), 5);        // lão hóa không đều
        float wet = std::clamp((static_cast<float>(r) - hang * 0.45f) / (hang * 0.55f), 0.f, 1.f);   // phần gần gốc ẩm hơn
        Color cTrai = toi(pha(mau(86, 52, 30), mau(58, 40, 28), wet), jit);
        Color cGiua = toi(pha(mau(138, 90, 50), mau(88, 66, 44), wet), jit);
        Color cSang = toi(pha(mau(186, 128, 72), mau(112, 88, 58), wet), jit);
        Color cPhai = toi(pha(mau(96, 60, 34), mau(60, 44, 30), wet), jit);
        float x0 = -nuaThan, x1 = -nuaThan * 0.35f, x2 = nuaThan * 0.4f, x3 = nuaThan;
        quad(x0, ya, x1, yb, cTrai, cGiua, cGiua, cTrai);
        quad(x1, ya, x2, yb, cGiua, cSang, cSang, cGiua);
        quad(x2, ya, x3, yb, cSang, cPhai, cPhai, cSang);
    }
    // ánh sáng hoàng hôn rọi từ bên phải
    quad(nuaThan * 0.55f, yKhop, nuaThan * 0.9f, yKhop + (yDay - yKhop) * 0.55f,
         mau(255, 214, 140, 120), mau(255, 214, 140, 0), mau(255, 214, 140, 0), mau(255, 214, 140, 80));

    // thớ gỗ, vết nứt dọc, mắt gỗ
    for (int i = 0; i < 4; ++i) {
        float gx = -nuaThan * 0.8f + nuaThan * 1.6f * bam(i, 1, hat);
        float gy0 = yKhop + (yDay - yKhop) * 0.8f * bam(i, 2, hat);
        float gl = (yDay - yKhop) * (0.18f + 0.3f * bam(i, 3, hat));
        quad(gx - 0.5f, gy0, gx + 0.5f, std::min(gy0 + gl, yDay), mau(40, 24, 14, 150), mau(40, 24, 14, 150), mau(40, 24, 14, 60), mau(40, 24, 14, 60));
    }
    for (int i = 0; i < 3; ++i) {
        float ry = yKhop + (yDay - yKhop) * (0.2f + 0.22f * static_cast<float>(i) + 0.08f * bam(i, 9, hat));
        quad(-nuaThan, ry, nuaThan * 0.3f + nuaThan * 0.5f * bam(i, 8, hat), ry + 0.9f, mau(34, 20, 12, 150), mau(34, 20, 12, 90), mau(34, 20, 12, 90), mau(34, 20, 12, 150));
    }
    dot(nuaThan * 0.15f, yKhop + (yDay - yKhop) * 0.38f, 1.9f, 2.6f, mau(60, 36, 20, 220));
    dot(nuaThan * 0.15f - 0.5f, yKhop + (yDay - yKhop) * 0.38f - 0.5f, 1.0f, 1.4f, mau(100, 64, 36, 200));

    // rêu xanh, tảo bám gốc
    quad(-nuaThan, yDay - h * 0.26f, nuaThan, yDay, mau(60, 112, 60, 0), mau(60, 112, 60, 0), mau(52, 100, 56, 170), mau(52, 100, 56, 170));
    for (int i = 0; i < 3; ++i) {
        float bx = -nuaThan * 0.7f + nuaThan * 1.4f * bam(i, 21, hat);
        dot(bx, yDay - h * (0.05f + 0.1f * bam(i, 22, hat)), 2.4f, 1.8f, mau(86, 140, 70, 150));
    }

    // ------------------------------------------------------------------ đai sắt + mũi nón bọc sắt
    // gỉ sét chảy dọc thân
    for (int i = 0; i < 3; ++i) {
        float rx = -nuaThan * 0.6f + nuaThan * 1.2f * static_cast<float>(i) / 2.f;
        float rl = 5.f + 9.f * bam(i, 31, hat);
        quad(rx - 0.6f, yKhop + 2.f, rx + 0.6f, yKhop + 2.f + rl, mau(150, 74, 36, 170), mau(150, 74, 36, 170), mau(150, 74, 36, 0), mau(150, 74, 36, 0));
    }
    quad(-nuaThan - 1.6f, yKhop - 1.5f, nuaThan + 1.6f, yKhop + 3.8f, mau(104, 112, 122), mau(104, 112, 122), mau(52, 58, 68), mau(52, 58, 68));
    quad(-nuaThan - 1.6f, yKhop - 1.5f, nuaThan + 1.6f, yKhop - 0.4f, mau(214, 224, 234), mau(214, 224, 234), mau(214, 224, 234, 0), mau(214, 224, 234, 0));
    for (int i = 0; i < 3; ++i) dot(-nuaThan * 0.6f + nuaThan * 0.6f * static_cast<float>(i), yKhop + 1.2f, 0.9f, 0.9f, mau(30, 36, 44));

    // mũi nón: viền tối, nửa trái sáng, nửa phải tối
    tri(0.f, yDinh - 1.4f, -nuaNon - 1.4f, yKhop - 1.f, nuaNon + 1.4f, yKhop - 1.f, mau(30, 38, 48), mau(30, 38, 48), mau(30, 38, 48));
    tri(0.f, yDinh, -nuaNon, yKhop - 1.5f, 0.f, yKhop - 1.5f, mau(236, 244, 252), mau(150, 164, 180), mau(176, 190, 204));
    tri(0.f, yDinh, 0.f, yKhop - 1.5f, nuaNon, yKhop - 1.5f, mau(160, 174, 190), mau(104, 116, 132), mau(70, 80, 94));
    // vệt phản chiếu thép
    quad(-nuaNon * 0.45f - 0.5f, yDinh + (yKhop - yDinh) * 0.35f, -nuaNon * 0.45f + 0.7f, yKhop - 3.f,
         mau(255, 255, 255, 220), mau(255, 255, 255, 220), mau(255, 255, 255, 40), mau(255, 255, 255, 40));
    // máu khô nếu cọc đã đâm trúng thuyền địch
    if (daPhatNoi) {
        tri(0.f, yDinh + (yKhop - yDinh) * 0.2f, -nuaNon * 0.55f, yKhop - 1.5f, nuaNon * 0.5f, yKhop - 1.5f,
            mau(130, 18, 18, 190), mau(100, 12, 12, 220), mau(100, 12, 12, 220));
        quad(-1.f, yKhop, 0.4f, yKhop + 7.f, mau(120, 16, 16, 200), mau(120, 16, 16, 200), mau(120, 16, 16, 0), mau(120, 16, 16, 0));
        // mảnh gỗ vụn quanh chân
        for (int i = 0; i < 4; ++i) {
            float sx = (i % 2 ? 1.f : -1.f) * (nuaThan + 2.f + 3.f * bam(i, 41, hat));
            quad(sx - 1.2f, yDay - 10.f - 6.f * static_cast<float>(i), sx + 1.2f, yDay - 5.f - 6.f * static_cast<float>(i),
                 mau(150, 100, 56), mau(150, 100, 56), mau(100, 64, 36), mau(100, 64, 36));
        }
    }

    // ------------------------------------------------------------------ mặt nước phủ lên cọc theo thủy triều
    // cover: phần cọc đang nằm dưới nước. tl = 0: chỉ ngập chân; tl = 0.65: chỉ còn mũi sắt ló ra; tl = 1: chìm hẳn.
    const float cover = h * (0.12f + 0.93f * tl);
    float yNuoc = yDay - cover;
    const bool nhoLen = (yNuoc > yDinh - 1.f);
    yNuoc = std::max(yNuoc, yDinh - 10.f);
    {
        const float m = 1.f - tl;
        Color nuoc = pha(mau(22, 68, 102), mau(86, 108, 90), m);
        float aTren = 70.f + 150.f * tl * tl + 20.f * tl;   // triều cạn: nước trong, thấy rõ gỗ; triều cao: đục dần
        float aDuoi = 110.f + 120.f * tl;
        auto mauNuoc = [&](float y) {
            float f = std::clamp((y - yNuoc) / std::max(1.f, yDay - yNuoc), 0.f, 1.f);
            return vaA(nuoc, aTren + (aDuoi - aTren) * f);
        };
        // phần mũi nón nằm dưới nước (hình thang)
        if (yNuoc < yKhop) {
            float yTop = std::max(yNuoc, yDinh);
            float hw0 = nhoMat(yTop) + 0.8f, hw1 = nuaNon + 0.8f;
            tu(va, P(-hw0, yTop), P(hw0, yTop), P(hw1, yKhop + 3.8f), P(-hw1, yKhop + 3.8f),
               mauNuoc(yTop), mauNuoc(yTop), mauNuoc(yKhop), mauNuoc(yKhop));
        }
        // phần thân nằm dưới nước
        float yb0 = std::max(yNuoc, yKhop + 3.8f);
        if (yb0 < yDay) {
            tu(va, P(-nuaThan - 1.3f, yb0), P(nuaThan + 1.3f, yb0), P(nuaThan + 1.3f, yDay + 1.f), P(-nuaThan - 1.3f, yDay + 1.f),
               mauNuoc(yb0), mauNuoc(yb0), mauNuoc(yDay), mauNuoc(yDay));
            // vệt sáng lăn tăn trên phần ngập (hiệu ứng khúc xạ)
            for (int i = 0; i < 3; ++i) {
                float yy = yb0 + (yDay - yb0) * (0.15f + 0.3f * static_cast<float>(i));
                float off = std::sin(t * 2.6f + pha0 * 6.f + static_cast<float>(i) * 2.1f) * 2.f;
                quad(-nuaThan + off, yy, nuaThan * 0.5f + off, yy + 0.9f, mau(180, 224, 244, 0), mau(180, 224, 244, 80), mau(180, 224, 244, 80), mau(180, 224, 244, 0));
            }
        }
    }

    // ------------------------------------------------------------------ đường nước, bọt, sóng gợn quanh chân cọc
    if (nhoLen) {
        float yw = yNuoc;
        float hwNuoc = (yw < yKhop) ? nhoMat(std::max(yw, yDinh)) + 1.f : nuaThan + 1.4f;
        Vector2f c = P(0.f, yw);
        // gờ sóng đánh vào cọc
        float sang = 0.5f + 0.5f * std::sin(t * 3.f + pha0 * 6.f);
        quad(-hwNuoc - 1.5f, yw - 1.f, hwNuoc + 1.5f, yw + 0.8f, mau(238, 250, 255, 40), mau(238, 250, 255, 40), mau(238, 250, 255, 40), mau(238, 250, 255, 40));
        for (int i = -2; i <= 2; ++i) {
            Vector2f q = P(static_cast<float>(i) * (hwNuoc * 0.5f), yw + 0.6f + std::sin(t * 4.f + static_cast<float>(i)) * 0.5f);
            elip(va, q.x, q.y, 2.6f + sang, 1.4f, mau(240, 252, 255, 190), 6);
        }
        // hai vòng sóng lan tỏa
        float fade = std::clamp(1.f - tl * 0.5f, 0.2f, 1.f);
        for (int k = 0; k < 2; ++k) {
            float p = std::fmod(t * 0.55f + pha0 + static_cast<float>(k) * 0.5f, 1.f);
            float rx = w * (0.8f + 1.9f * p);
            vong(va, c.x, c.y + 1.f, rx, rx * 0.3f, 1.3f, mau(214, 240, 252, (1.f - p) * 150.f * fade), 22);
        }
        // hạt bọt vỡ nhỏ
        for (int i = 0; i < 3; ++i) {
            float p = std::fmod(t * 0.9f + pha0 * 3.f + static_cast<float>(i) * 0.33f, 1.f);
            elip(va, c.x + (static_cast<float>(i) - 1.f) * w * 0.7f * (0.4f + p), c.y - p * 4.f, 0.9f, 0.9f, mau(255, 255, 255, (1.f - p) * 200.f), 5);
        }
        // hà bám sát mực nước (chỉ thấy khi cọc đã nhô lên)
        if (tl < 0.45f) {
            for (int i = 0; i < 4; ++i) {
                float bx = -nuaThan * 0.8f + nuaThan * 1.6f * bam(i, 51, hat);
                Vector2f q = P(bx, std::min(yw - 1.5f - 3.f * bam(i, 52, hat), yDay - 2.f));
                elip(va, q.x, q.y, 1.2f, 1.0f, mau(214, 210, 196, 210), 5);
            }
        }
    }

    // ------------------------------------------------------------------ mũi cọc ló khỏi mặt nước: lấp lánh báo nguy hiểm
    if (tl <= 0.7f && yNuoc > yDinh) {
        float k = 0.5f + 0.5f * std::sin(t * 4.f + pha0 * 6.28f);
        k = k * k * k;
        if (k > 0.05f) {
            Vector2f tip = P(0.f, yDinh + 1.f);
            float r = 1.6f + 2.6f * k;
            Color c = mau(255, 255, 255, 230.f * k);
            tam(va, {tip.x, tip.y - r * 1.8f}, {tip.x + 0.9f, tip.y}, {tip.x - 0.9f, tip.y}, c);
            tam(va, {tip.x, tip.y + r * 1.8f}, {tip.x + 0.9f, tip.y}, {tip.x - 0.9f, tip.y}, c);
            tam(va, {tip.x - r * 1.8f, tip.y}, {tip.x, tip.y + 0.9f}, {tip.x, tip.y - 0.9f}, c);
            tam(va, {tip.x + r * 1.8f, tip.y}, {tip.x, tip.y + 0.9f}, {tip.x, tip.y - 0.9f}, c);
        }
    }

    window.draw(va);
}


