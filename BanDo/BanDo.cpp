
#include "BanDo.h"
#include "DoHoa.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>

using namespace dohoa;

// =============================================================================
//  PHẦN 1 - CÔNG CỤ DỰNG VẬT THỂ TĨNH (vẽ 1 lần vào VertexArray)
// =============================================================================
namespace {

// Gió: giá trị trong [-1, 1], thay đổi theo vị trí và thời gian
float gio(float x, float t) {
    return std::sin(t * 1.15f + x * 0.007f) * 0.65f + std::sin(t * 0.43f + x * 0.019f + 1.7f) * 0.35f;
}

const Color kGo      = mau(122, 80, 46);
const Color kGoToi   = mau(74, 46, 28);
const Color kGoSang  = mau(172, 122, 72);
const Color kTranh   = mau(206, 168, 90);
const Color kTranhT  = mau(150, 112, 56);
const Color kSat     = mau(196, 206, 216);
const Color kDo      = mau(200, 34, 34);
const Color kVang    = mau(240, 192, 66);

void bongDo(VA& va, float x, float y, float rx, float ry, float a = 70.f) {
    elip(va, x, y, rx, ry, mau(0, 0, 0, a), 14);
}

// ---- Tháp canh gỗ (đài quan sát) -------------------------------------------
void thapCanh(VA& va, float x, float y, float s) {
    const float sanY = y - 52.f * s;
    bongDo(va, x - 8.f * s, y + 2.f * s, 30.f * s, 5.f * s);
    // chân sau (tối)
    duong(va, {x - 8.f * s, y}, {x - 7.f * s, sanY}, 3.2f * s, kGoToi);
    duong(va, {x + 8.f * s, y}, {x + 7.f * s, sanY}, 3.2f * s, kGoToi);
    // giằng chữ X
    duong(va, {x - 17.f * s, y - 3.f * s}, {x + 15.f * s, sanY + 6.f * s}, 1.8f * s, kGoToi);
    duong(va, {x + 17.f * s, y - 3.f * s}, {x - 15.f * s, sanY + 6.f * s}, 1.8f * s, kGoToi);
    // chân trước
    duong(va, {x - 19.f * s, y}, {x - 14.f * s, sanY}, 4.f * s, kGo);
    duong(va, {x + 19.f * s, y}, {x + 14.f * s, sanY}, 4.f * s, kGo);
    duong(va, {x - 17.f * s, y - 20.f * s}, {x + 17.f * s, y - 20.f * s}, 2.4f * s, kGo);
    // sàn
    chuNhat(va, x - 21.f * s, sanY, 42.f * s, 5.f * s, kGo);
    chuNhat(va, x - 21.f * s, sanY, 42.f * s, 1.5f * s, kGoSang);
    // lan can
    chuNhat(va, x - 20.f * s, sanY - 13.f * s, 2.4f * s, 13.f * s, kGoToi);
    chuNhat(va, x + 17.6f * s, sanY - 13.f * s, 2.4f * s, 13.f * s, kGoToi);
    chuNhat(va, x - 20.f * s, sanY - 13.f * s, 40.f * s, 2.f * s, kGo);
    for (int i = 1; i < 5; ++i)
        chuNhat(va, x - 20.f * s + static_cast<float>(i) * 8.f * s, sanY - 11.f * s, 1.3f * s, 11.f * s, kGoToi);
    // cột mái
    chuNhat(va, x - 17.f * s, sanY - 32.f * s, 2.6f * s, 32.f * s, kGoToi);
    chuNhat(va, x + 14.4f * s, sanY - 32.f * s, 2.6f * s, 32.f * s, kGoToi);
    // mái tranh
    const float mY = sanY - 31.f * s;
    tam(va, {x - 27.f * s, mY + 2.f * s}, {x, mY + 2.f * s}, {x, mY - 26.f * s}, kTranh);
    tam(va, {x, mY + 2.f * s}, {x + 27.f * s, mY + 2.f * s}, {x, mY - 26.f * s}, kTranhT);
    for (int k = 1; k < 5; ++k) {
        float yk = mY + 2.f * s - static_cast<float>(k) * 5.2f * s;
        float wk = 27.f * s * (1.f - static_cast<float>(k) * 5.2f / 28.f);
        duong(va, {x - wk, yk}, {x + wk, yk}, 0.9f * s, mau(110, 78, 36, 150));
    }
    for (float fx = x - 27.f * s; fx < x + 26.f * s; fx += 4.6f * s)
        tam(va, {fx, mY + 2.f * s}, {fx + 4.4f * s, mY + 2.f * s}, {fx + 2.2f * s, mY + 6.5f * s}, kTranh);
    // thang
    duong(va, {x + 24.f * s, y}, {x + 18.f * s, sanY + 4.f * s}, 1.6f * s, kGoToi);
    duong(va, {x + 29.f * s, y}, {x + 21.f * s, sanY + 4.f * s}, 1.6f * s, kGoToi);
    for (int i = 1; i < 7; ++i) {
        float f = static_cast<float>(i) / 7.f;
        duong(va, {x + 24.f * s + 5.f * s * (1.f - f) + 0.f, y - (y - sanY - 4.f * s) * f},
              {x + 29.f * s - 8.f * s * f, y - (y - sanY - 4.f * s) * f}, 1.1f * s, kGo);
    }
}

// ---- Lều vải của nghĩa quân --------------------------------------------------
void leu(VA& va, float x, float y, float w, float h, Color vai) {
    bongDo(va, x + w * 0.1f, y + 2.f, w * 0.62f, 4.5f);
    tam(va, {x - w * 0.5f, y}, {x, y}, {x, y - h}, vai);
    tam(va, {x, y}, {x + w * 0.5f, y}, {x, y - h}, toi(vai, 0.72f));
    // viền gấu đỏ
    float bw = w * (1.f - 5.f / h) * 0.5f;
    tu(va, {x - bw, y - 5.f}, {x + bw, y - 5.f}, {x + w * 0.5f, y}, {x - w * 0.5f, y},
       mau(170, 36, 30), mau(150, 30, 26), mau(150, 30, 26), mau(170, 36, 30));
    // cửa lều
    tam(va, {x - w * 0.14f, y}, {x + w * 0.14f, y}, {x, y - h * 0.58f}, mau(32, 22, 18));
    tam(va, {x - w * 0.14f, y}, {x - w * 0.02f, y}, {x, y - h * 0.58f}, mau(70, 50, 38));
    // nếp vải
    duong(va, {x, y - h}, {x - w * 0.3f, y - 5.f}, 0.8f, mau(0, 0, 0, 40));
    duong(va, {x, y - h}, {x + w * 0.3f, y - 5.f}, 0.8f, mau(0, 0, 0, 50));
    // dây chằng và cọc neo
    for (int sg = -1; sg <= 1; sg += 2) {
        float sgf = static_cast<float>(sg);
        duong(va, {x, y - h * 0.9f}, {x + sgf * w * 0.8f, y + 1.f}, 0.9f, mau(214, 196, 160, 180));
        tam(va, {x + sgf * w * 0.8f - 1.5f, y + 4.f}, {x + sgf * w * 0.8f + 1.5f, y + 4.f}, {x + sgf * w * 0.8f, y - 2.f}, kGoToi);
    }
}

// ---- Nhà sàn mái tranh -----------------------------------------------------
void nhaTranh(VA& va, float x, float y, float s) {
    const float cao = 12.f * s;
    const float sanY = y - cao - 3.f * s;
    bongDo(va, x - 4.f * s, y + 2.f * s, 40.f * s, 5.f * s);
    for (int i = 0; i < 4; ++i)
        chuNhat(va, x - 28.f * s + static_cast<float>(i) * 18.f * s, y - cao, 3.2f * s, cao, kGoToi);
    chuNhat(va, x - 32.f * s, sanY, 64.f * s, 4.f * s, kGo);
    chuNhat(va, x - 32.f * s, sanY, 64.f * s, 1.2f * s, kGoSang);
    // vách
    chuNhatDoc(va, x - 26.f * s, sanY - 24.f * s, 52.f * s, 24.f * s, mau(156, 108, 62), mau(120, 80, 46));
    for (int i = 1; i < 8; ++i)
        chuNhat(va, x - 26.f * s + static_cast<float>(i) * 6.5f * s, sanY - 24.f * s, 0.8f * s, 24.f * s, mau(70, 44, 26, 130));
    chuNhat(va, x - 6.f * s, sanY - 20.f * s, 12.f * s, 20.f * s, mau(34, 22, 16));
    chuNhat(va, x + 12.f * s, sanY - 18.f * s, 9.f * s, 8.f * s, mau(34, 22, 16));
    chuNhat(va, x + 11.f * s, sanY - 19.f * s, 11.f * s, 1.2f * s, kGoToi);
    // mái tranh dày
    const float mY = sanY - 21.f * s;
    tam(va, {x - 40.f * s, mY + 3.f * s}, {x, mY + 3.f * s}, {x, mY - 32.f * s}, kTranh);
    tam(va, {x, mY + 3.f * s}, {x + 40.f * s, mY + 3.f * s}, {x, mY - 32.f * s}, kTranhT);
    for (int k = 1; k < 7; ++k) {
        float yk = mY + 3.f * s - static_cast<float>(k) * 5.f * s;
        float wk = 40.f * s * (1.f - static_cast<float>(k) * 5.f / 35.f);
        if (wk <= 0.f) break;
        duong(va, {x - wk, yk}, {x + wk, yk}, 1.f * s, mau(104, 74, 34, 150));
    }
    for (float fx = x - 40.f * s; fx < x + 39.f * s; fx += 4.8f * s)
        tam(va, {fx, mY + 3.f * s}, {fx + 4.6f * s, mY + 3.f * s}, {fx + 2.3f * s, mY + 8.f * s}, bam(static_cast<int>(fx), 3) > 0.5f ? kTranh : kTranhT);
    duong(va, {x - 3.f * s, mY - 32.f * s}, {x + 4.f * s, mY - 40.f * s}, 1.6f * s, kGoToi);
    duong(va, {x + 3.f * s, mY - 32.f * s}, {x - 4.f * s, mY - 40.f * s}, 1.6f * s, kGoToi);
    // thang gỗ
    duong(va, {x - 12.f * s, y}, {x - 8.f * s, sanY + 2.f * s}, 1.5f * s, kGoToi);
    duong(va, {x - 5.f * s, y}, {x - 4.f * s, sanY + 2.f * s}, 1.5f * s, kGoToi);
    for (int i = 1; i < 4; ++i)
        duong(va, {x - 11.f * s + static_cast<float>(i) * 1.6f * s, y - static_cast<float>(i) * 3.5f * s},
              {x - 5.f * s + static_cast<float>(i) * 0.4f * s, y - static_cast<float>(i) * 3.5f * s}, 1.f * s, kGo);
    // bầu rượu treo
    elip(va, x + 28.f * s, sanY - 4.f * s, 2.8f * s, 3.4f * s, mau(170, 130, 60), 8);
}

// ---- Trống đồng Đông Sơn trên giá gỗ ----------------------------------------
void trongDong(VA& va, float x, float y, float s) {
    bongDo(va, x, y + 1.f * s, 17.f * s, 3.5f * s);
    chuNhat(va, x - 13.f * s, y - 4.f * s, 4.f * s, 4.f * s, kGoToi);
    chuNhat(va, x + 9.f * s, y - 4.f * s, 4.f * s, 4.f * s, kGoToi);
    // thân trống
    Color dongSang = mau(176, 128, 52), dongToi = mau(104, 72, 30);
    tu(va, {x - 14.f * s, y - 24.f * s}, {x + 14.f * s, y - 24.f * s}, {x + 16.f * s, y - 4.f * s}, {x - 16.f * s, y - 4.f * s},
       dongSang, dongSang, dongToi, dongToi);
    tu(va, {x - 14.f * s, y - 16.f * s}, {x + 14.f * s, y - 16.f * s}, {x + 12.f * s, y - 12.f * s}, {x - 12.f * s, y - 12.f * s},
       mau(120, 82, 34), mau(120, 82, 34), mau(120, 82, 34), mau(120, 82, 34));
    duong(va, {x - 15.f * s, y - 9.f * s}, {x + 15.f * s, y - 9.f * s}, 1.f * s, mau(236, 190, 90, 150));
    // quai trống
    chuNhat(va, x - 17.f * s, y - 22.f * s, 3.f * s, 7.f * s, dongToi);
    chuNhat(va, x + 14.f * s, y - 22.f * s, 3.f * s, 7.f * s, dongToi);
    // mặt trống: elip + hoa văn ngôi sao
    elip(va, x, y - 24.f * s, 15.f * s, 5.2f * s, mau(120, 82, 34), 18);
    elip(va, x, y - 24.5f * s, 13.5f * s, 4.4f * s, dongSang, 18);
    for (int i = 0; i < 8; ++i) {
        float a = PI * 2.f * static_cast<float>(i) / 8.f;
        tam(va, {x, y - 24.5f * s},
            {x + std::cos(a - 0.2f) * 9.f * s, y - 24.5f * s + std::sin(a - 0.2f) * 3.1f * s},
            {x + std::cos(a + 0.2f) * 9.f * s, y - 24.5f * s + std::sin(a + 0.2f) * 3.1f * s}, mau(236, 196, 100));
    }
    vong(va, x, y - 24.5f * s, 11.f * s, 3.7f * s, 0.9f * s, mau(104, 72, 30), 20);
    elip(va, x, y - 24.5f * s, 3.f * s, 1.1f * s, mau(255, 226, 140), 10);
}

// ---- Giá giáo --------------------------------------------------------------
void giaGiao(VA& va, float x, float y, float s) {
    bongDo(va, x, y + 1.f * s, 26.f * s, 3.5f * s);
    for (int i = 0; i < 6; ++i) {
        float bx = x - 20.f * s + static_cast<float>(i) * 8.f * s;
        float lean = (static_cast<float>(i) - 2.5f) * 1.2f * s;
        duong(va, {bx, y - 3.f * s}, {bx + lean, y - 52.f * s}, 1.7f * s, kGo);
        tam(va, {bx + lean - 2.f * s, y - 51.f * s}, {bx + lean + 2.f * s, y - 51.f * s}, {bx + lean, y - 60.f * s}, kSat);
        duong(va, {bx + lean, y - 51.f * s}, {bx + lean, y - 49.f * s}, 2.6f * s, mau(170, 36, 30));
    }
    chuNhat(va, x - 24.f * s, y - 12.f * s, 48.f * s, 2.5f * s, kGoToi);
    chuNhat(va, x - 24.f * s, y - 3.f * s, 3.f * s, 3.f * s, kGoToi);
    chuNhat(va, x + 21.f * s, y - 3.f * s, 3.f * s, 3.f * s, kGoToi);
    // khiên tròn
    elip(va, x + 30.f * s, y - 10.f * s, 8.f * s, 8.f * s, mau(112, 72, 40), 14);
    elip(va, x + 30.f * s, y - 10.f * s, 5.5f * s, 5.5f * s, mau(176, 36, 30), 14);
    elip(va, x + 30.f * s, y - 10.f * s, 1.8f * s, 1.8f * s, kVang, 8);
}

// ---- Kho cọc nhọn (chồng cọc gỗ bịt sắt) -----------------------------------
void khoCoc(VA& va, float x, float y, float s) {
    bongDo(va, x, y + 1.f * s, 32.f * s, 4.f * s);
    const int soCoc[3] = {4, 3, 2};
    for (int tang = 0; tang < 3; ++tang) {
        for (int i = 0; i < soCoc[tang]; ++i) {
            float cx = x - (static_cast<float>(soCoc[tang]) - 1.f) * 0.5f * 7.f * s + static_cast<float>(i) * 7.f * s;
            float cy = y - 4.f * s - static_cast<float>(tang) * 7.f * s;
            float cc = bam(i, tang, 5);
            Color go = pha(mau(120, 78, 42), mau(150, 100, 56), cc);
            chuNhat(va, cx - 14.f * s, cy - 3.6f * s, 28.f * s, 7.2f * s, toi(go, 0.55f));
            chuNhatDoc(va, cx - 13.f * s, cy - 2.8f * s, 26.f * s, 5.6f * s, sang(go, 0.15f), toi(go, 0.8f));
            tam(va, {cx + 13.f * s, cy - 3.4f * s}, {cx + 13.f * s, cy + 3.4f * s}, {cx + 20.f * s, cy}, kSat);
            tam(va, {cx + 13.f * s, cy - 3.4f * s}, {cx + 20.f * s, cy}, {cx + 14.f * s, cy - 1.f * s}, mau(255, 255, 255, 140));
            chuNhat(va, cx + 10.f * s, cy - 3.6f * s, 2.2f * s, 7.2f * s, mau(60, 66, 74));
        }
    }
    // dây buộc
    duong(va, {x - 11.f * s, y - 28.f * s}, {x - 11.f * s, y - 2.f * s}, 1.f * s, mau(200, 180, 130, 200));
    duong(va, {x + 3.f * s, y - 28.f * s}, {x + 3.f * s, y - 2.f * s}, 1.f * s, mau(200, 180, 130, 200));
}

// ---- Bến thuyền gỗ nhô ra sông -------------------------------------------------
void benThuyen(VA& va, float x, float yBo, float dai, float rong) {
    float yDau = yBo - dai;
    // bóng trên mặt nước
    chuNhat(va, x - rong * 0.5f + 5.f, yDau + 6.f, rong, dai + 6.f, mau(0, 10, 20, 70));
    // cọc chống phía dưới
    for (int i = 0; i < 3; ++i) {
        float py = yDau + 2.f + static_cast<float>(i) * (dai - 4.f) / 2.f;
        chuNhat(va, x - rong * 0.5f - 3.f, py, 5.f, 8.f, kGoToi);
        chuNhat(va, x + rong * 0.5f - 2.f, py, 5.f, 8.f, kGoToi);
    }
    // ván sàn
    int n = static_cast<int>(dai / 5.f);
    for (int i = 0; i < n; ++i) {
        float py = yDau + static_cast<float>(i) * 5.f;
        float c = bam(i, 1, 31);
        chuNhat(va, x - rong * 0.5f, py, rong, 4.4f, pha(mau(142, 98, 56), mau(170, 120, 70), c));
        chuNhat(va, x - rong * 0.5f, py + 4.4f, rong, 0.6f, mau(50, 30, 16));
    }
    chuNhat(va, x - rong * 0.5f, yDau, 2.5f, dai, kGoToi);
    chuNhat(va, x + rong * 0.5f - 2.5f, yDau, 2.5f, dai, kGoToi);
    // cọc buộc thuyền ở mút bến
    for (int sg = -1; sg <= 1; sg += 2) {
        float px = x + static_cast<float>(sg) * (rong * 0.5f - 1.f);
        chuNhat(va, px - 2.5f, yDau - 7.f, 5.f, 11.f, kGo);
        elip(va, px, yDau - 7.f, 3.f, 1.6f, kGoSang, 8);
        // dây thừng
        vong(va, px, yDau - 3.f, 4.f, 2.f, 1.2f, mau(214, 190, 140), 10);
    }
}

// ---- Thuyền thúng ------------------------------------------------------------
void thuyenThung(VA& va, float x, float y, float r) {
    bongDo(va, x + 2.f, y + r * 0.5f, r * 1.05f, r * 0.34f, 90);
    elip(va, x, y, r, r * 0.55f, mau(70, 46, 24), 20);
    elip(va, x, y - 0.8f, r * 0.92f, r * 0.48f, mau(176, 130, 72), 20);
    for (int i = 1; i < 4; ++i) {
        float f = 1.f - static_cast<float>(i) * 0.22f;
        vong(va, x, y - 0.8f, r * 0.92f * f, r * 0.48f * f, 0.7f, mau(120, 82, 40, 150), 18);
    }
    elip(va, x, y - 1.f, r * 0.32f, r * 0.18f, mau(100, 66, 32), 10);
    // mái chèo
    duong(va, {x - r * 0.4f, y - 1.f}, {x + r * 1.15f, y + r * 0.5f}, 1.6f, kGoToi);
    elip(va, x + r * 1.18f, y + r * 0.52f, 3.6f, 1.8f, kGo, 8);
}

// ---- Tảng đá ---------------------------------------------------------------
void daTang(VA& va, float x, float y, float rx, float ry) {
    bongDo(va, x + 2.f, y + ry * 0.7f, rx * 1.05f, ry * 0.45f, 70);
    elip(va, x, y, rx, ry, mau(92, 96, 102), 12);
    elip(va, x - rx * 0.12f, y - ry * 0.18f, rx * 0.85f, ry * 0.78f, mau(132, 136, 140), 12);
    elip(va, x - rx * 0.25f, y - ry * 0.4f, rx * 0.5f, ry * 0.36f, mau(176, 176, 172), 10);
    elip(va, x + rx * 0.35f, y + ry * 0.2f, rx * 0.3f, ry * 0.28f, mau(70, 112, 60, 190), 8);  // rêu
}

// ---- Giàn phơi lưới ------------------------------------------------------------
void gianLuoi(VA& va, float x, float y, float w, float h) {
    bongDo(va, x, y + 1.f, w * 0.55f, 3.5f, 55);
    chuNhat(va, x - w * 0.5f - 1.5f, y - h, 3.f, h, kGo);
    chuNhat(va, x + w * 0.5f - 1.5f, y - h, 3.f, h, kGo);
    chuNhat(va, x - w * 0.5f - 3.f, y - h, w + 6.f, 2.5f, kGoToi);
    for (int i = 0; i <= static_cast<int>(w / 5.f); ++i)
        duong(va, {x - w * 0.5f + static_cast<float>(i) * 5.f, y - h + 2.f},
              {x - w * 0.5f + static_cast<float>(i) * 5.f + 1.f, y - h * 0.15f}, 0.7f, mau(214, 204, 168, 150));
    for (int j = 1; j < 6; ++j)
        duong(va, {x - w * 0.5f, y - h + static_cast<float>(j) * h * 0.14f + 2.f},
              {x + w * 0.5f, y - h + static_cast<float>(j) * h * 0.14f + 4.f}, 0.7f, mau(214, 204, 168, 130));
    for (int i = 0; i < 5; ++i)
        elip(va, x - w * 0.4f + static_cast<float>(i) * w * 0.2f, y - h * 0.12f, 1.8f, 2.4f, mau(190, 190, 186), 6);
}

// ---- Hàng rào tre ----------------------------------------------------------
void hangRao(VA& va, float x0, float x1, float y) {
    chuNhat(va, x0, y - 12.f, x1 - x0, 1.8f, kGoToi);
    chuNhat(va, x0, y - 6.f, x1 - x0, 1.8f, kGoToi);
    for (float x = x0; x <= x1; x += 9.f) {
        float h = 18.f + bam(static_cast<int>(x), 7) * 4.f;
        chuNhatDoc(va, x - 1.8f, y - h, 3.6f, h, mau(150, 156, 70), mau(104, 116, 50));
        tam(va, {x - 1.8f, y - h}, {x + 1.8f, y - h}, {x, y - h - 4.f}, mau(170, 176, 86));
    }
}

}  // namespace

// =============================================================================
//  PHẦN 2 - KHỞI TẠO BẢN ĐỒ & LOGIC GAME (giữ nguyên hành vi cũ)
// =============================================================================

BanDo::BanDo(float rong, float cao)
    : chieuRong(rong),
      chieuCao(cao),
      song(cao > 750.f ? sf::FloatRect({0.f, cao * 0.22f}, {rong, cao * 0.50f})
                       : sf::FloatRect({0.f, 150.f}, {rong, 380.f})) {

    if (rong > 1400.f) {
        KhoiTaoBaiCocRong(rong * 0.42f, rong * 0.78f, cao * 0.28f, cao * 0.66f);
    } else {
        KhoiTaoBaiCocChuan();
    }
    KhoiTaoCocThuThap(3);

    // Mặt trời hoàng hôn ở 72% bề ngang -> vệt nắng trên sông khớp với bầu trời
    song.SetToaDoAnhNang(chieuRong * 0.72f);

    DungBauTroi();
    DungNuiVaBoBac();
    DungBoNam();
    KhoiTaoThucVat();
    KhoiTaoHieuUng();
}

void BanDo::KhoiTaoBaiCocChuan() {
    danhSachCoc.clear();

    // Dàn hàng cọc ngầm chữ chi hiểm hóc chặn ngang khúc sông Bạch Đằng
    // Tọa độ X từ 350 đến 750, Y từ 200 đến 480
    for (int col = 0; col < 6; ++col) {
        float x = 380.f + static_cast<float>(col * 75);
        for (int row = 0; row < 5; ++row) {
            float yOffset = (col % 2 == 0) ? 0.f : 30.f;
            float y = 200.f + static_cast<float>(row * 60) + yOffset;
            danhSachCoc.emplace_back(sf::Vector2f(x, y), 16.f, 52.f, true);
        }
    }
}

void BanDo::KhoiTaoBaiCocRong(float xMin, float xMax, float yMin, float yMax) {
    danhSachCoc.clear();
    int soCot = std::max(2, static_cast<int>((xMax - xMin) / 75.f));
    int soHang = std::max(2, static_cast<int>((yMax - yMin) / 55.f));
    for (int col = 0; col < soCot; ++col) {
        float x = xMin + static_cast<float>(col * 75);
        for (int row = 0; row < soHang; ++row) {
            float yOffset = (col % 2 == 0) ? 0.f : 25.f;
            float y = yMin + static_cast<float>(row * 55) + yOffset;
            danhSachCoc.emplace_back(sf::Vector2f(x, y), 16.f, 52.f, true);
        }
    }
}

void BanDo::KhoiTaoCocThuThap(int soLuong) {
    danhSachCocRoi.clear();
    // Đặt cọc gỗ trên bờ để chiến binh nhặt. Với soLuong = 3 (mặc định) vị trí giống hệt bản cũ:
    // (180,100) (450,90) (720,110)
    if (soLuong <= 0) return;
    const float yMau[3] = {100.f, 90.f, 110.f};
    for (int i = 0; i < soLuong; ++i) {
        float x = (soLuong == 1) ? 450.f : 180.f + 540.f * static_cast<float>(i) / static_cast<float>(soLuong - 1);
        danhSachCocRoi.push_back({sf::Vector2f(x, yMau[i % 3]), false});
    }
}

void BanDo::ThemCocMoi(sf::Vector2f pos) {
    danhSachCoc.emplace_back(pos, 16.f, 52.f, true);
    PhatTiaNuoc(pos + sf::Vector2f(0.f, 20.f));
}

int BanDo::KiemTraNhatCoc(sf::Vector2f viTriPlayer, float banKinh) {
    int demNhat = 0;
    for (auto& coc : danhSachCocRoi) {
        if (!coc.daNhat) {
            float dx = viTriPlayer.x - coc.viTri.x;
            float dy = viTriPlayer.y - coc.viTri.y;
            if (std::sqrt(dx * dx + dy * dy) <= banKinh) {
                coc.daNhat = true;
                demNhat++;
                PhatLongLanhNhat(coc.viTri);
            }
        }
    }
    return demNhat;
}

int BanDo::GetSoCocChuaNhat() const {
    int count = 0;
    for (const auto& coc : danhSachCocRoi) {
        if (!coc.daNhat) count++;
    }
    return count;
}

bool BanDo::XuLyVaChamThuyenVoiBaiCoc(ThuyenKeDich& thuyen) {
    bool coVaCham = false;
    float tiLeThuyTrieu = song.GetTiLeThuyTrieu();

    for (auto& coc : danhSachCoc) {
        if (thuyen.KiemTraVaChamCoc(coc, tiLeThuyTrieu)) {
            coVaCham = true;
            PhatManhGo(thuyen.GetViTri());
            PhatTiaNuoc(thuyen.GetViTri() + sf::Vector2f(0.f, 14.f));
        }
    }
    return coVaCham;
}

// =============================================================================
//  PHẦN 3 - DỰNG CÁC LỚP TĨNH
// =============================================================================

void BanDo::DungBauTroi() {
    lopTroi.clear();
    const float W = chieuRong;
    const float yChanTroi = 112.f;

    struct Dung { float v; Color c; };
    const Dung dung[5] = {
        {0.00f, mau(18, 28, 74)},   {0.30f, mau(58, 60, 120)}, {0.55f, mau(142, 92, 140)},
        {0.78f, mau(238, 136, 112)}, {1.00f, mau(255, 198, 130)}};
    for (int i = 0; i < 4; ++i) {
        chuNhatDoc(lopTroi, 0.f, yChanTroi * dung[i].v, W, yChanTroi * (dung[i + 1].v - dung[i].v), dung[i].c, dung[i + 1].c);
    }

    // Những vì sao đầu tiên của buổi chiều tà
    Rng rng(4242);
    for (int i = 0; i < 46; ++i) {
        float x = rng.f() * W, y = rng.range(2.f, 40.f);
        float a = rng.range(70.f, 200.f) * (1.f - y / 46.f);
        chuNhat(lopTroi, x, y, 1.6f, 1.6f, mau(255, 250, 230, a));
    }

    // Mặt trời lặn và quầng sáng
    const float sx = W * 0.72f, sy = 78.f;
    elipTan(lopTroi, sx, sy, 340.f, 120.f, mau(255, 190, 120, 125), mau(255, 150, 100, 0), 30);
    elipTan(lopTroi, sx, sy, 120.f, 72.f, mau(255, 222, 150, 175), mau(255, 190, 120, 0), 24);
    elipTan(lopTroi, sx, sy, 34.f, 34.f, mau(255, 252, 222), mau(255, 228, 150), 26);
}

void BanDo::DungNuiVaBoBac() {
    lopBac.clear();
    const float W = chieuRong;
    const float k = chieuRong / 1200.f;
    const auto vung = song.GetVungSong();
    const float yN = vung.position.y;       // mép nước phía Bắc
    const float sunX = W * 0.72f;
    Rng rng(777);

    // ---------- Núi đá vôi (karst) và đồi rừng ----------
    struct Dinh { float cx, w, h; };
    auto taoDinh = [&](int n, float hMin, float hMax, float wMin, float wMax, float lech) {
        std::vector<Dinh> v;
        for (int i = 0; i < n; ++i) {
            Dinh d;
            d.cx = (static_cast<float>(i) + 0.5f) * W / static_cast<float>(n) + rng.range(-lech, lech);
            d.w = rng.range(wMin, wMax);
            d.h = rng.range(hMin, hMax);
            if (std::abs(d.cx - sunX) < 95.f) d.h = rng.range(10.f, 18.f);   // chừa chỗ cho mặt trời
            v.push_back(d);
        }
        return v;
    };
    auto doCao = [](const std::vector<Dinh>& ds, float x, float ruong) {
        float h = 6.f;
        for (const auto& d : ds) {
            float u = std::abs(x - d.cx) / d.w;
            if (u < 1.f) h = std::max(h, d.h * std::pow(1.f - u * u, 0.38f));
        }
        return h + ruong;
    };
    auto daiNui = [&](float yDay, const std::vector<Dinh>& ds, float nhieuBien, Color tren, Color duoi, Color vien) {
        for (float x = 0.f; x < W; x += 3.f) {
            float h = doCao(ds, x + 1.5f, nhieuBien * nhieu(x * 0.04f, yDay, 3));
            float hT = doCao(ds, x + 4.5f, nhieuBien * nhieu((x + 3.f) * 0.04f, yDay, 3));
            float hP = doCao(ds, x - 1.5f, nhieuBien * nhieu((x - 3.f) * 0.04f, yDay, 3));
            float doDoc = std::abs(hT - hP) * 0.5f;
            chuNhatDoc(lopBac, x, yDay - h, 3.f, h, tren, duoi);
            // viền sáng phía mặt trời: dày theo độ dốc để sườn dựng đứng không bị đứt nét
            chuNhat(lopBac, x, yDay - h - 0.5f, 3.f, std::clamp(doDoc + 1.6f, 1.6f, 16.f), vien);
        }
    };

    // Dãy xa
    auto xa = taoDinh(15, 46.f, 86.f, 22.f, 46.f, 34.f);
    daiNui(100.f, xa, 4.f, mau(104, 90, 142), mau(224, 150, 150), mau(255, 196, 140, 170));
    chuNhatDoc(lopBac, 0.f, 46.f, W, 56.f, mau(255, 175, 140, 0), mau(255, 175, 140, 110));
    // Dãy gần
    auto gan = taoDinh(20, 26.f, 52.f, 18.f, 36.f, 26.f);
    daiNui(106.f, gan, 3.f, mau(62, 66, 108), mau(150, 108, 126), mau(255, 184, 134, 150));
    // Mảng cây trên vách đá
    for (float x = 4.f; x < W; x += 7.f) {
        float h = doCao(gan, x, 4.f * nhieu(x * 0.04f, 106.f, 3));
        if (h > 12.f && bam(static_cast<int>(x), 2, 12) > 0.35f) {
            float r = 3.f + 3.f * bam(static_cast<int>(x), 3, 12);
            elip(lopBac, x, 106.f - h + 3.f + 5.f * bam(static_cast<int>(x), 4, 12), r * 1.3f, r, mau(52, 84, 74, 210), 8);
        }
    }
    chuNhatDoc(lopBac, 0.f, 62.f, W, 50.f, mau(240, 160, 130, 0), mau(240, 160, 130, 90));

    // Đồi rừng gần
    {
        for (float x = 0.f; x < W; x += 3.f) {
            float h = 20.f + 9.f * std::sin(x * 0.011f + 1.f * k) + 6.f * std::sin(x * 0.027f + 2.f) +
                      2.6f * std::abs(std::sin(x * 0.42f)) + 4.f * nhieu(x * 0.05f, 9.f, 5);
            chuNhatDoc(lopBac, x, 118.f - h, 3.f, h, mau(46, 76, 68), mau(60, 96, 70));
            chuNhat(lopBac, x, 118.f - h, 3.f, 1.4f, mau(255, 190, 120, 110));
        }
        for (float x = 3.f; x < W; x += 9.f) {
            float h = 20.f + 9.f * std::sin(x * 0.011f + 1.f * k) + 6.f * std::sin(x * 0.027f + 2.f) +
                      4.f * nhieu(x * 0.05f, 9.f, 5);
            elip(lopBac, x, 118.f - h + 2.f, 6.5f, 5.f, mau(38, 68, 58), 8);
        }
    }

    // ---------- Bờ Bắc: thảm cỏ gạch 8px kiểu Terraria ----------
    const float yDat = 100.f;
    const float yMat = yN - 25.f;   // mép trên của vách đất
    for (float y = yDat; y < yMat; y += 8.f) {
        float hh = std::min(8.f, yMat - y);
        float v = (y - yDat) / (yMat - yDat);
        for (float x = 0.f; x < W; x += 8.f) {
            float n = nhieu(x * 0.03f, y * 0.05f, 3);
            float j = bam(static_cast<int>(x / 8.f), static_cast<int>(y / 8.f), 9);
            Color c = pha(mau(62, 102, 60), mau(100, 152, 64), v);
            c = mau(c.r + (n - 0.5f) * 28.f + (j - 0.5f) * 10.f, c.g + (n - 0.5f) * 36.f + (j - 0.5f) * 12.f, c.b + (n - 0.5f) * 14.f);
            chuNhat(lopBac, x, y, 8.f, hh, c);
        }
    }
    // Chuyển tiếp rừng -> cỏ (hàng cây ven)
    chuNhatDoc(lopBac, 0.f, yDat - 2.f, W, 18.f, mau(30, 58, 44, 170), mau(30, 58, 44, 0));
    for (float x = 0.f; x < W; x += 11.f)
        elip(lopBac, x + bam(static_cast<int>(x), 1, 41) * 6.f, yDat + 1.f, 8.f, 5.5f, mau(40, 70, 52, 230), 8);
    // Đường mòn trên bờ Bắc
    for (float x = 0.f; x < W; x += 6.f) {
        float py = 113.f + 4.f * std::sin(x * 0.012f + 0.8f);
        float n = bam(static_cast<int>(x / 6.f), 5, 6);
        chuNhat(lopBac, x, py - 3.5f, 6.f, 7.f, pha(mau(148, 112, 72), mau(168, 128, 82), n));
        chuNhat(lopBac, x, py - 4.5f, 6.f, 1.4f, mau(120, 130, 60, 120));
    }

    // ---------- Vách đất bờ Bắc (mặt cắt đất, đá, rễ) ----------
    for (float x = 0.f; x < W; x += 8.f) {
        for (int r = 0; r < 4; ++r) {
            float y = yMat + static_cast<float>(r) * 8.f;
            float hh = std::min(8.f, yN - y);
            if (hh <= 0.f) break;
            float n = bam(static_cast<int>(x / 8.f), r, 21);
            Color c = (r == 0) ? pha(mau(80, 120, 54), mau(66, 104, 46), n)
                               : pha(mau(112, 76, 48), mau(90, 60, 38), n);
            c = toi(c, r >= 2 ? 0.78f : 0.9f);   // vách quay lưng nắng nên tối hơn
            chuNhat(lopBac, x, y, 8.f, hh, c);
            if (r >= 1 && n > 0.86f) elip(lopBac, x + 4.f, y + 4.f, 3.6f, 2.8f, mau(118, 120, 126), 7);
            if (r >= 1 && n < 0.12f) chuNhat(lopBac, x + 2.f, y + 1.f, 1.6f, 6.5f, mau(64, 40, 24));
        }
    }
    for (float x = 0.f; x < W; x += 4.f)
        tam(lopBac, {x, yMat + 2.f}, {x + 3.f, yMat + 2.f}, {x + 1.5f, yMat + 4.f + 5.f * bam(static_cast<int>(x), 8, 2)}, mau(68, 112, 46));
    for (float x = 0.f; x < W; x += 4.f)
        tam(lopBac, {x, yMat + 1.f}, {x + 3.f, yMat + 1.f}, {x + 1.5f, yMat - 2.f - 3.f * bam(static_cast<int>(x), 6, 2)}, mau(112, 168, 70));
    chuNhatDoc(lopBac, 0.f, yN - 12.f, W, 12.f, mau(14, 10, 8, 0), mau(14, 10, 8, 120));
    chuNhatDoc(lopBac, 0.f, yMat + 1.f, W, 5.f, mau(0, 0, 0, 0), mau(0, 0, 0, 0));

    // ---------- Cỏ dại, hoa, đá nhỏ rải trên bờ ----------
    auto gan_coc = [&](float x, float y) {
        for (const auto& c : danhSachCocRoi)
            if (std::abs(c.viTri.x - x) < 36.f && std::abs(c.viTri.y - y) < 30.f) return true;
        return false;
    };
    for (int i = 0; i < 170; ++i) {
        float x = rng.range(4.f, W - 4.f), y = rng.range(104.f, yMat - 3.f);
        if (gan_coc(x, y)) continue;
        Color c = pha(mau(84, 132, 56), mau(132, 184, 76), rng.f());
        for (int b = -1; b <= 1; ++b)
            tam(lopBac, {x + static_cast<float>(b) * 2.f - 1.2f, y}, {x + static_cast<float>(b) * 2.f + 1.2f, y},
                {x + static_cast<float>(b) * 3.f, y - 5.f - 3.f * rng.f()}, c);
    }
    for (int i = 0; i < 46; ++i) {
        float x = rng.range(6.f, W - 6.f), y = rng.range(106.f, yMat - 4.f);
        if (gan_coc(x, y)) continue;
        Color c = (i % 3 == 0) ? mau(246, 244, 232) : (i % 3 == 1) ? mau(244, 214, 90) : mau(236, 150, 180);
        chuNhat(lopBac, x, y - 3.f, 1.6f, 3.f, mau(70, 120, 50));
        elip(lopBac, x + 0.8f, y - 3.8f, 2.2f, 2.f, c, 6);
    }
    for (int i = 0; i < 9; ++i) {
        float x = rng.range(20.f, W - 20.f), y = rng.range(110.f, yMat - 2.f);
        if (gan_coc(x, y)) continue;
        daTang(lopBac, x, y, rng.range(5.f, 9.f), rng.range(3.f, 5.f));
    }

    // ---------- Doanh trại nghĩa quân Đại Việt ở bờ Bắc ----------
    const float yb = 122.f;
    thapCanh(lopBac, 96.f * k, yb, 0.8f);
    leu(lopBac, 268.f * k, yb, 62.f, 44.f, mau(196, 170, 128));
    leu(lopBac, 338.f * k, yb, 54.f, 38.f, mau(176, 150, 112));
    trongDong(lopBac, 404.f * k, yb, 0.95f);
    giaGiao(lopBac, 520.f * k, yb, 0.8f);
    nhaTranh(lopBac, 618.f * k, yb, 0.8f);
    leu(lopBac, 936.f * k, yb, 64.f, 46.f, mau(200, 174, 132));
    leu(lopBac, 1004.f * k, yb, 56.f, 40.f, mau(180, 154, 116));
    nhaTranh(lopBac, 1120.f * k, yb, 0.75f);
    hangRao(lopBac, 760.f * k, 820.f * k, 120.f);
}

void BanDo::DungBoNam() {
    lopNam.clear();
    const float W = chieuRong;
    const float H = chieuCao;
    const float k = chieuRong / 1200.f;
    const auto vung = song.GetVungSong();
    const float yS = vung.position.y + vung.size.y;   // mép nước phía Nam
    const float yMat = yS + 25.f;                      // mép dưới vách đất
    Rng rng(991);

    // ---------- Vách đất bờ Nam (quay mặt về phía mặt trời nên ấm màu hơn) ----------
    for (float x = 0.f; x < W; x += 8.f) {
        for (int r = 0; r < 4; ++r) {
            float y = yS + static_cast<float>(r) * 8.f;
            float hh = std::min(8.f, yMat - y);
            if (hh <= 0.f) break;
            float n = bam(static_cast<int>(x / 8.f), r, 23);
            Color c = (r == 0) ? pha(mau(86, 62, 44), mau(70, 50, 36), n)
                               : pha(mau(152, 104, 62), mau(128, 86, 52), n);
            if (r == 3) c = pha(c, mau(86, 132, 56), 0.5f);
            chuNhat(lopNam, x, y, 8.f, hh, c);
            if (r >= 1 && n > 0.84f) elip(lopNam, x + 4.f, y + 4.f, 3.8f, 3.f, mau(150, 150, 152), 7);
            if (r >= 1 && n < 0.1f) chuNhat(lopNam, x + 2.f, y + 1.f, 1.6f, 6.f, mau(74, 46, 28));
        }
    }
    chuNhatDoc(lopNam, 0.f, yS, W, 10.f, mau(10, 8, 6, 110), mau(10, 8, 6, 0));
    chuNhatDoc(lopNam, 0.f, yS + 10.f, W, 15.f, mau(255, 170, 90, 0), mau(255, 170, 90, 36));

    // ---------- Thảm cỏ gạch 8px, đường mòn đất ----------
    auto duongY = [](float x) { return 646.f + 18.f * std::sin(x * 0.0068f + 0.4f) + 6.f * std::sin(x * 0.021f); };
    for (float y = yMat; y < H; y += 8.f) {
        float hh = std::min(8.f, H - y);
        float v = (y - yMat) / (H - yMat);
        for (float x = 0.f; x < W; x += 8.f) {
            float n = nhieu(x * 0.03f, y * 0.05f, 4);
            float j = bam(static_cast<int>(x / 8.f), static_cast<int>(y / 8.f), 19);
            Color c = pha(mau(98, 148, 62), mau(50, 94, 46), v);
            c = mau(c.r + (n - 0.5f) * 30.f + (j - 0.5f) * 12.f, c.g + (n - 0.5f) * 40.f + (j - 0.5f) * 14.f, c.b + (n - 0.5f) * 16.f);
            float d = std::abs(y + 4.f - duongY(x + 4.f));
            if (d < 9.f) c = pha(mau(150, 112, 70), mau(120, 88, 56), j + v * 0.4f);
            else if (d < 13.f) c = pha(c, mau(120, 100, 58), 0.45f);
            chuNhat(lopNam, x, y, 8.f, hh, c);
        }
    }
    // gờ cỏ lòa xòa trên mép vách
    for (float x = 0.f; x < W; x += 4.f)
        tam(lopNam, {x, yMat + 1.f}, {x + 3.f, yMat + 1.f}, {x + 1.5f, yMat - 3.f - 5.f * bam(static_cast<int>(x), 9, 2)}, pha(mau(96, 150, 58), mau(132, 188, 72), bam(static_cast<int>(x), 2, 3)));
    // tối dần về mép dưới
    chuNhatDoc(lopNam, 0.f, H - 90.f, W, 90.f, mau(6, 16, 12, 0), mau(6, 16, 12, 110));

    // ---------- Cỏ, hoa, đá ----------
    for (int i = 0; i < 360; ++i) {
        float x = rng.range(4.f, W - 4.f), y = rng.range(yMat + 6.f, H - 3.f);
        if (std::abs(y - duongY(x)) < 10.f) continue;
        float v = (y - yMat) / (H - yMat);
        Color c = pha(pha(mau(92, 146, 58), mau(140, 192, 76), rng.f()), mau(36, 80, 40), v * 0.6f);
        float sc = 1.f + v * 0.9f;
        for (int b = -1; b <= 1; ++b)
            tam(lopNam, {x + static_cast<float>(b) * 2.4f * sc - 1.4f * sc, y}, {x + static_cast<float>(b) * 2.4f * sc + 1.4f * sc, y},
                {x + static_cast<float>(b) * 3.4f * sc, y - (5.f + 4.f * rng.f()) * sc}, c);
    }
    for (int i = 0; i < 70; ++i) {
        float x = rng.range(6.f, W - 6.f), y = rng.range(yMat + 10.f, H - 6.f);
        if (std::abs(y - duongY(x)) < 10.f) continue;
        Color c = (i % 3 == 0) ? mau(246, 244, 232) : (i % 3 == 1) ? mau(244, 214, 90) : mau(236, 150, 180);
        float sc = 1.f + (y - yMat) / (H - yMat) * 0.8f;
        chuNhat(lopNam, x, y - 4.f * sc, 1.8f, 4.f * sc, mau(66, 114, 48));
        elip(lopNam, x + 0.9f, y - 4.8f * sc, 2.6f * sc, 2.4f * sc, c, 6);
    }
    for (int i = 0; i < 14; ++i) {
        float x = rng.range(20.f, W - 20.f), y = rng.range(yMat + 20.f, H - 12.f);
        if (std::abs(y - duongY(x)) < 14.f) continue;
        float sc = 1.f + (y - yMat) / (H - yMat) * 0.7f;
        daTang(lopNam, x, y, rng.range(6.f, 11.f) * sc, rng.range(3.5f, 6.f) * sc);
    }

    // ---------- Công trình & vật dụng bờ Nam ----------
    benThuyen(lopNam, 300.f * k, yS + 6.f, 40.f, 34.f);
    thuyenThung(lopNam, 362.f * k, yS + 18.f, 13.f);
    thuyenThung(lopNam, 392.f * k, yS + 20.f, 11.f);
    gianLuoi(lopNam, 205.f * k, 600.f, 70.f, 40.f);
    trongDong(lopNam, 262.f * k, 632.f, 1.2f);
    khoCoc(lopNam, 540.f * k, 612.f, 1.2f);
    giaGiao(lopNam, 606.f * k, 596.f, 1.0f);
    leu(lopNam, 666.f * k, 626.f, 84.f, 58.f, mau(198, 172, 130));
    leu(lopNam, 752.f * k, 636.f, 72.f, 50.f, mau(180, 154, 114));
    nhaTranh(lopNam, 872.f * k, 616.f, 1.1f);
    hangRao(lopNam, 950.f * k, 1004.f * k, 600.f);
    thapCanh(lopNam, 1040.f * k, 656.f, 1.0f);
}

// =============================================================================
//  PHẦN 4 - THỰC VẬT & HIỆU ỨNG (đối tượng chuyển động)
// =============================================================================

void BanDo::KhoiTaoThucVat() {
    danhSachVat.clear();
    const float W = chieuRong;
    const float k = chieuRong / 1200.f;
    const auto vung = song.GetVungSong();
    const float yN = vung.position.y;
    const float yS = vung.position.y + vung.size.y;
    Rng rng(2026);

    // Vùng cấm trồng cây: công trình, cọc nhặt
    struct Chan { float x0, x1, y0, y1; };
    std::vector<Chan> chan;
    auto chanCT = [&](float cx, float rong, float yDay, float cao) {
        chan.push_back({cx - rong * 0.5f - 8.f, cx + rong * 0.5f + 8.f, yDay - cao, yDay + 34.f});
    };
    chanCT(96.f * k, 60.f, 122.f, 60.f);   chanCT(268.f * k, 66.f, 122.f, 50.f);  chanCT(338.f * k, 58.f, 122.f, 44.f);
    chanCT(404.f * k, 40.f, 122.f, 30.f);  chanCT(520.f * k, 70.f, 122.f, 56.f);  chanCT(618.f * k, 76.f, 122.f, 70.f);
    chanCT(860.f * k, 50.f, 122.f, 40.f);  chanCT(936.f * k, 68.f, 122.f, 50.f);  chanCT(1004.f * k, 60.f, 122.f, 46.f);
    chanCT(1120.f * k, 70.f, 122.f, 64.f); chanCT(790.f * k, 70.f, 122.f, 24.f);
    chanCT(205.f * k, 80.f, 600.f, 50.f);  chanCT(262.f * k, 40.f, 632.f, 30.f);  chanCT(300.f * k, 50.f, 560.f, 60.f);
    chanCT(380.f * k, 60.f, 552.f, 20.f);  chanCT(430.f * k, 60.f, 624.f, 50.f);  chanCT(540.f * k, 60.f, 612.f, 40.f);
    chanCT(606.f * k, 60.f, 596.f, 60.f);  chanCT(666.f * k, 94.f, 626.f, 64.f);  chanCT(752.f * k, 82.f, 636.f, 56.f);
    chanCT(872.f * k, 84.f, 616.f, 80.f);  chanCT(977.f * k, 70.f, 600.f, 30.f);  chanCT(1040.f * k, 60.f, 656.f, 100.f);
    for (const auto& c : danhSachCocRoi) chan.push_back({c.viTri.x - 40.f, c.viTri.x + 40.f, c.viTri.y - 44.f, c.viTri.y + 30.f});

    std::vector<std::array<float, 3>> daDat;
    auto bi = [&](float x, float y, float r) {
        for (const auto& c : chan)
            if (x + r > c.x0 && x - r < c.x1 && y > c.y0 && y < c.y1) return true;
        for (const auto& d : daDat)
            if (std::abs(d[0] - x) < r + d[2] && std::abs(d[1] - y) < 12.f) return true;
        return false;
    };
    auto datCay = [&](LoaiVat loai, float x, float y, float s, float r) {
        VatDong v;
        v.loai = loai; v.x = x; v.y = y; v.kichThuoc = s;
        v.pha = rng.range(0.f, 6.28f); v.bienThe = rng.irange(0, 5);
        danhSachVat.push_back(v);
        daDat.push_back({x, y, r});
    };
    auto them = [&](LoaiVat loai, float x, float y, float s, int bt = 0) {
        VatDong v;
        v.loai = loai; v.x = x; v.y = y; v.kichThuoc = s;
        v.pha = rng.range(0.f, 6.28f); v.bienThe = bt;
        danhSachVat.push_back(v);
    };

    // ----- Bờ Bắc: rừng ven sông (cây nhỏ hơn vì ở xa) -----
    for (int dem = 0, thuLai = 0; dem < 42 && thuLai < 600; ++thuLai) {
        float x = rng.range(10.f, W - 10.f), y = rng.range(105.f, 121.f);
        float roll = rng.f();
        LoaiVat loai; float r; float s = rng.range(0.62f, 0.9f);
        if (roll < 0.34f)      { loai = LoaiVat::CAY_LA; r = 18.f * s; }
        else if (roll < 0.52f) { loai = LoaiVat::TRE;    r = 14.f * s; }
        else if (roll < 0.62f) { loai = LoaiVat::CAU;    r = 9.f * s; }
        else                   { loai = LoaiVat::BUI;    r = 9.f * s; s *= 0.9f; }
        if (bi(x, y, r)) continue;
        datCay(loai, x, y, s, r);
        ++dem;
    }
    // ----- Bờ Nam: cây to hơn (tiền cảnh) -----
    them(LoaiVat::CAY_DA, 70.f * k, 688.f, 1.45f, 1);
    daDat.push_back({70.f * k, 688.f, 60.f});
    them(LoaiVat::CAY_DA, 1170.f * k, 690.f, 1.1f, 2);
    daDat.push_back({1170.f * k, 690.f, 50.f});
    for (int dem = 0, thuLai = 0; dem < 34 && thuLai < 900; ++thuLai) {
        float x = rng.range(10.f, W - 10.f), y = rng.range(592.f, 692.f);
        float roll = rng.f();
        float s = 0.95f + (y - 592.f) / 100.f * 0.5f;
        LoaiVat loai; float r;
        if (roll < 0.32f)      { loai = LoaiVat::CAY_LA; r = 26.f * s; }
        else if (roll < 0.52f) { loai = LoaiVat::TRE;    r = 18.f * s; }
        else if (roll < 0.64f) { loai = LoaiVat::CAU;    r = 12.f * s; }
        else                   { loai = LoaiVat::BUI;    r = 12.f * s; s *= 0.9f; }
        if (bi(x, y, r)) continue;
        datCay(loai, x, y, s, r);
        ++dem;
    }
    // ----- Rừng đước (sú vẹt) mọc sát mép nước hai bờ -----
    for (float x = 20.f; x < W - 20.f; x += rng.range(70.f, 130.f))
        them(LoaiVat::DUOC, x, yN - 1.f, rng.range(0.85f, 1.15f), rng.irange(0, 3));
    for (float x = 40.f; x < W - 20.f; x += rng.range(90.f, 150.f)) {
        if (x > 262.f * k && x < 420.f * k) continue;   // chừa bến thuyền và thuyền thúng
        them(LoaiVat::DUOC, x, yS + 23.f, rng.range(0.8f, 1.05f), rng.irange(0, 3));
    }
    // ----- Lau sậy ven mép nước -----
    for (float x = 6.f; x < W; x += rng.range(12.f, 24.f)) {
        if (rng.f() < 0.5f) continue;
        them(LoaiVat::LAU, x, yN - 0.5f, rng.range(0.45f, 0.72f), rng.irange(0, 5));
    }
    for (float x = 6.f; x < W; x += rng.range(12.f, 24.f)) {
        if (rng.f() < 0.45f) continue;
        if (x > 276.f * k && x < 330.f * k) continue;   // tránh bến thuyền
        them(LoaiVat::LAU, x, yS + 2.f, rng.range(0.5f, 0.8f), rng.irange(0, 5));
    }
    // ----- Cờ lệnh Đại Việt -----
    them(LoaiVat::CO, 96.f * k, 122.f - 52.f * 0.8f - 31.f * 0.8f - 28.f * 0.8f + 4.f, 0.62f);   // đỉnh tháp canh Bắc
    them(LoaiVat::CO, 268.f * k, 122.f - 44.f - 4.f, 0.62f);
    them(LoaiVat::CO, 936.f * k, 122.f - 46.f - 4.f, 0.62f);
    them(LoaiVat::CO, 560.f * k, 123.f, 0.78f);
    them(LoaiVat::CO, 800.f * k, 123.f, 0.78f);
    them(LoaiVat::CO, 1060.f * k, 123.f, 0.78f);
    them(LoaiVat::CO, 666.f * k, 626.f - 58.f - 4.f, 0.9f);
    them(LoaiVat::CO, 752.f * k, 636.f - 50.f - 4.f, 0.9f);
    them(LoaiVat::CO, 1040.f * k, 656.f - 52.f - 31.f - 26.f + 4.f, 0.8f);   // đỉnh tháp canh Nam
    them(LoaiVat::CO, 470.f * k, 582.f, 1.0f);
    them(LoaiVat::CO, 920.f * k, 660.f, 1.0f);
    them(LoaiVat::CO, 330.f * k, 590.f, 1.0f);
    // ----- Lửa trại & đuốc -----
    them(LoaiVat::LUA, 860.f * k, 121.f, 0.8f, 0);
    them(LoaiVat::LUA, 430.f * k, 624.f, 1.25f, 0);
    them(LoaiVat::LUA, 96.f * k - 17.f * 0.8f, 122.f - 52.f * 0.8f - 2.f, 0.6f, 1);   // đuốc trên tháp
    them(LoaiVat::LUA, 96.f * k + 17.f * 0.8f, 122.f - 52.f * 0.8f - 2.f, 0.6f, 1);
    them(LoaiVat::LUA, 1040.f * k - 17.f, 656.f - 52.f - 2.f, 0.8f, 1);
    them(LoaiVat::LUA, 1040.f * k + 17.f, 656.f - 52.f - 2.f, 0.8f, 1);
    them(LoaiVat::LUA, 300.f * k - 17.f, yS + 6.f - 40.f - 2.f, 0.7f, 1);   // đuốc ở đầu bến thuyền
    them(LoaiVat::LUA, 300.f * k + 17.f, yS + 6.f - 40.f - 2.f, 0.7f, 1);

    std::sort(danhSachVat.begin(), danhSachVat.end(), [](const VatDong& a, const VatDong& b) { return a.y < b.y; });
}

void BanDo::KhoiTaoHieuUng() {
    const float W = chieuRong;
    Rng rng(31337);

    danhSachMay.clear();
    for (int i = 0; i < 9; ++i) {
        May m;
        m.rong = rng.range(110.f, 250.f);
        m.cao = rng.range(12.f, 24.f);
        m.x = rng.range(-m.rong, W);
        m.y = rng.range(10.f, 56.f);
        m.tocDo = 2.f + (m.rong / 250.f) * 7.f;
        m.doMo = rng.range(120.f, 200.f);
        danhSachMay.push_back(m);
    }
    danhSachChim.clear();
    for (int i = 0; i < 7; ++i) {
        Chim c;
        c.trang = (i >= 4);
        c.x = rng.range(0.f, W);
        c.y = c.trang ? rng.range(62.f, 96.f) : rng.range(18.f, 56.f);
        c.tocDo = c.trang ? rng.range(14.f, 22.f) : rng.range(26.f, 38.f);
        c.pha = rng.range(0.f, 6.28f);
        c.kichThuoc = c.trang ? rng.range(0.9f, 1.15f) : rng.range(0.7f, 1.f);
        danhSachChim.push_back(c);
    }
    danhSachDomDom.clear();
    for (int i = 0; i < 30; ++i) {
        DomDom d;
        d.goc = {rng.range(10.f, W - 10.f), rng.range(540.f, chieuCao - 10.f)};
        d.pha = rng.range(0.f, 6.28f);
        d.tocDo = rng.range(0.4f, 1.1f);
        d.bienDo = rng.range(10.f, 34.f);
        danhSachDomDom.push_back(d);
    }
    danhSachSuong.clear();
    for (int i = 0; i < 8; ++i) {
        SuongMu s;
        s.rong = rng.range(190.f, 340.f);
        s.cao = rng.range(14.f, 28.f);
        s.x = rng.range(-s.rong, W);
        s.y = rng.range(170.f, 505.f);
        s.tocDo = rng.range(5.f, 13.f);
        s.doMo = rng.range(16.f, 30.f);
        danhSachSuong.push_back(s);
    }
    danhSachHat.clear();
}

// =============================================================================
//  PHẦN 5 - HIỆU ỨNG HẠT
// =============================================================================

void BanDo::PhatTiaNuoc(sf::Vector2f pos) {
    if (danhSachHat.size() > 400) return;
    for (int i = 0; i < 16; ++i) {
        float a = -PI * (0.15f + 0.7f * static_cast<float>(rand() % 100) / 100.f);
        float sp = 70.f + static_cast<float>(rand() % 110);
        HatSang h;
        h.viTri = pos;
        h.van = {std::cos(a) * sp, std::sin(a) * sp};
        h.tuoi = 0.f;
        h.tuoiMax = 0.55f + static_cast<float>(rand() % 50) / 100.f;
        h.kichThuoc = 2.f + static_cast<float>(rand() % 20) / 10.f;
        h.mau = mau(214, 240, 252, 230);
        h.kieu = 0;
        danhSachHat.push_back(h);
    }
    HatSang r;
    r.viTri = pos; r.van = {0.f, 0.f}; r.tuoi = 0.f; r.tuoiMax = 1.2f; r.kichThuoc = 36.f;
    r.mau = mau(220, 240, 250, 180); r.kieu = 1;
    danhSachHat.push_back(r);
    r.tuoiMax = 1.6f; r.kichThuoc = 54.f; r.mau = mau(220, 240, 250, 110);
    danhSachHat.push_back(r);
}

void BanDo::PhatManhGo(sf::Vector2f pos) {
    if (danhSachHat.size() > 400) return;
    for (int i = 0; i < 18; ++i) {
        float a = -PI * (0.05f + 0.9f * static_cast<float>(rand() % 100) / 100.f);
        float sp = 60.f + static_cast<float>(rand() % 150);
        HatSang h;
        h.viTri = pos + sf::Vector2f(static_cast<float>(rand() % 30) - 15.f, static_cast<float>(rand() % 10) - 5.f);
        h.van = {std::cos(a) * sp, std::sin(a) * sp};
        h.tuoi = 0.f;
        h.tuoiMax = 0.7f + static_cast<float>(rand() % 70) / 100.f;
        h.kichThuoc = 2.f + static_cast<float>(rand() % 30) / 10.f;
        h.mau = (i % 2 == 0) ? mau(156, 104, 58, 255) : mau(92, 60, 34, 255);
        h.kieu = 0;
        danhSachHat.push_back(h);
    }
    HatSang r;
    r.viTri = pos; r.van = {0.f, 0.f}; r.tuoi = 0.f; r.tuoiMax = 0.7f; r.kichThuoc = 44.f;
    r.mau = mau(255, 180, 90, 200); r.kieu = 1;
    danhSachHat.push_back(r);
}

void BanDo::PhatLongLanhNhat(sf::Vector2f pos) {
    if (danhSachHat.size() > 400) return;
    for (int i = 0; i < 20; ++i) {
        float a = 2.f * PI * static_cast<float>(i) / 20.f + static_cast<float>(rand() % 100) / 100.f;
        float sp = 40.f + static_cast<float>(rand() % 70);
        HatSang h;
        h.viTri = pos;
        h.van = {std::cos(a) * sp, std::sin(a) * sp};
        h.tuoi = 0.f;
        h.tuoiMax = 0.6f + static_cast<float>(rand() % 50) / 100.f;
        h.kichThuoc = 1.8f + static_cast<float>(rand() % 20) / 10.f;
        h.mau = (i % 2 == 0) ? mau(255, 232, 130, 255) : mau(255, 255, 220, 255);
        h.kieu = 2;
        danhSachHat.push_back(h);
    }
    HatSang r;
    r.viTri = pos; r.van = {0.f, 0.f}; r.tuoi = 0.f; r.tuoiMax = 0.8f; r.kichThuoc = 40.f;
    r.mau = mau(255, 226, 120, 220); r.kieu = 1;
    danhSachHat.push_back(r);
}

// =============================================================================
//  PHẦN 6 - CẬP NHẬT
// =============================================================================

void BanDo::CapNhat(float deltaTime) {
    song.CapNhat(deltaTime);
    thoiGian += deltaTime;

    const float W = chieuRong;
    for (auto& m : danhSachMay) {
        m.x += m.tocDo * deltaTime;
        if (m.x > W + 20.f) m.x = -m.rong - 20.f;
    }
    for (auto& c : danhSachChim) {
        c.x += c.tocDo * deltaTime;
        if (c.x > W + 40.f) { c.x = -40.f; }
    }
    for (auto& s : danhSachSuong) {
        s.x += s.tocDo * deltaTime;
        if (s.x > W + 20.f) s.x = -s.rong - 20.f;
    }
    for (auto& h : danhSachHat) {
        h.tuoi += deltaTime;
        if (h.kieu == 0) h.van.y += 340.f * deltaTime;
        else if (h.kieu == 2) { h.van.y += 30.f * deltaTime; h.van *= (1.f - 1.8f * deltaTime); }
        h.viTri += h.van * deltaTime;
    }
    danhSachHat.erase(std::remove_if(danhSachHat.begin(), danhSachHat.end(),
                                     [](const HatSang& h) { return h.tuoi >= h.tuoiMax; }),
                      danhSachHat.end());
}

// =============================================================================
//  PHẦN 7 - VẼ ĐỐI TƯỢNG CHUYỂN ĐỘNG
// =============================================================================
namespace {

void veCayLa(VA& va, float x, float y, float s, float w, float ph, int bt) {
    const float H = 58.f * s;
    bongDo(va, x - 5.f * s, y + 2.f * s, 26.f * s, 5.f * s, 60);
    float lech = w * 2.2f * s;
    Color go = mau(92, 60, 36), goSang = mau(132, 90, 52);
    duongTan(va, {x, y}, {x + lech * 0.3f, y - H * 0.55f}, 7.f * s, 4.4f * s, go, go);
    duongTan(va, {x + lech * 0.3f, y - H * 0.45f}, {x - 10.f * s + lech * 0.7f, y - H * 0.72f}, 3.2f * s, 2.f * s, go, go);
    duongTan(va, {x + lech * 0.3f, y - H * 0.45f}, {x + 11.f * s + lech * 0.7f, y - H * 0.74f}, 3.2f * s, 2.f * s, go, go);
    duong(va, {x - 1.8f * s, y - 1.f}, {x + lech * 0.3f - 1.8f * s, y - H * 0.5f}, 1.4f * s, goSang);

    Color dark, mid, lite;
    switch (bt % 3) {
        case 0:  dark = mau(30, 78, 46);  mid = mau(50, 120, 58);  lite = mau(110, 172, 74);  break;
        case 1:  dark = mau(40, 84, 40);  mid = mau(84, 134, 52);  lite = mau(160, 184, 72);  break;
        default: dark = mau(24, 66, 50);  mid = mau(40, 104, 66);  lite = mau(96, 158, 88);   break;
    }
    struct B { float dx, dy, r; };
    static const B blobs[6] = {{-17.f, -4.f, 15.f}, {17.f, -6.f, 16.f}, {0.f, -14.f, 19.f},
                               {-8.f, 2.f, 14.f},  {10.f, 3.f, 13.f},  {-2.f, -26.f, 13.f}};
    float cx = x + lech * 0.7f, cy = y - H * 0.74f;
    for (int i = 0; i < 6; ++i) {
        float fl = std::sin(ph * 3.f + static_cast<float>(i) * 1.7f) * 0.8f * s + w * 1.6f * s;
        elip(va, cx + (blobs[i].dx) * s + fl, cy + blobs[i].dy * s, blobs[i].r * s, blobs[i].r * s * 0.92f, dark, 12);
    }
    for (int i = 0; i < 6; ++i) {
        float fl = std::sin(ph * 3.f + static_cast<float>(i) * 1.7f) * 0.8f * s + w * 1.6f * s;
        float r = blobs[i].r * s;
        elip(va, cx + blobs[i].dx * s + fl - r * 0.08f, cy + blobs[i].dy * s - r * 0.16f, r * 0.82f, r * 0.74f, mid, 12);
    }
    for (int i = 0; i < 6; ++i) {
        if (blobs[i].dy > 0.f) continue;
        float fl = std::sin(ph * 3.f + static_cast<float>(i) * 1.7f) * 0.8f * s + w * 1.6f * s;
        float r = blobs[i].r * s;
        elip(va, cx + blobs[i].dx * s + fl - r * 0.14f, cy + blobs[i].dy * s - r * 0.38f, r * 0.5f, r * 0.34f, lite, 10);
        // viền sáng ngược nắng hoàng hôn
        elip(va, cx + blobs[i].dx * s + fl, cy + blobs[i].dy * s - r * 0.78f, r * 0.34f, r * 0.12f, mau(255, 214, 120, 150), 8);
    }
}

void veCayDa(VA& va, float x, float y, float s, float w, float ph, int bt) {
    const float H = 74.f * s;
    bongDo(va, x - 8.f * s, y + 3.f * s, 54.f * s, 7.f * s, 70);
    float lech = w * 2.f * s;
    Color go = mau(86, 58, 38), goSang = mau(130, 92, 58);
    // thân phình chân + rễ bạnh
    tam(va, {x - 20.f * s, y}, {x - 4.f * s, y - H * 0.5f}, {x - 12.f * s, y - H * 0.5f}, go);
    tam(va, {x + 22.f * s, y}, {x + 4.f * s, y - H * 0.5f}, {x + 12.f * s, y - H * 0.5f}, go);
    duongTan(va, {x, y}, {x + lech * 0.3f, y - H * 0.66f}, 18.f * s, 10.f * s, go, go);
    duong(va, {x - 4.f * s, y - 2.f}, {x + lech * 0.3f - 3.f * s, y - H * 0.6f}, 2.4f * s, goSang);
    duongTan(va, {x + lech * 0.3f, y - H * 0.55f}, {x - 30.f * s + lech, y - H * 0.8f}, 5.f * s, 2.8f * s, go, go);
    duongTan(va, {x + lech * 0.3f, y - H * 0.55f}, {x + 32.f * s + lech, y - H * 0.8f}, 5.f * s, 2.8f * s, go, go);

    Color dark = (bt % 2) ? mau(26, 70, 44) : mau(32, 80, 42);
    Color mid  = (bt % 2) ? mau(44, 108, 62) : mau(56, 120, 54);
    Color lite = (bt % 2) ? mau(92, 156, 84) : mau(116, 166, 70);
    struct B { float dx, dy, r; };
    static const B blobs[9] = {{-44.f, 2.f, 22.f}, {-24.f, -8.f, 26.f}, {0.f, -16.f, 30.f}, {26.f, -8.f, 26.f}, {46.f, 4.f, 21.f},
                               {-30.f, 14.f, 18.f}, {4.f, 8.f, 24.f},  {34.f, 14.f, 17.f}, {-10.f, -34.f, 18.f}};
    float cx = x + lech, cy = y - H * 0.84f;
    for (int i = 0; i < 9; ++i) {
        float fl = std::sin(ph * 2.5f + static_cast<float>(i) * 1.3f) * 0.7f * s + w * 1.2f * s;
        elip(va, cx + blobs[i].dx * s + fl, cy + blobs[i].dy * s, blobs[i].r * s, blobs[i].r * s * 0.9f, dark, 14);
    }
    for (int i = 0; i < 9; ++i) {
        float fl = std::sin(ph * 2.5f + static_cast<float>(i) * 1.3f) * 0.7f * s + w * 1.2f * s;
        float r = blobs[i].r * s;
        elip(va, cx + blobs[i].dx * s + fl - r * 0.08f, cy + blobs[i].dy * s - r * 0.16f, r * 0.8f, r * 0.72f, mid, 14);
    }
    for (int i = 0; i < 9; ++i) {
        if (blobs[i].dy > 4.f) continue;
        float fl = std::sin(ph * 2.5f + static_cast<float>(i) * 1.3f) * 0.7f * s + w * 1.2f * s;
        float r = blobs[i].r * s;
        elip(va, cx + blobs[i].dx * s + fl - r * 0.12f, cy + blobs[i].dy * s - r * 0.38f, r * 0.5f, r * 0.32f, lite, 10);
        elip(va, cx + blobs[i].dx * s + fl, cy + blobs[i].dy * s - r * 0.78f, r * 0.36f, r * 0.12f, mau(255, 214, 120, 140), 8);
    }
    // rễ phụ rủ xuống từ tán cây đa
    for (int i = 0; i < 9; ++i) {
        float rx = cx + (-42.f + static_cast<float>(i) * 10.5f) * s;
        float ry = cy + 14.f * s;
        float len = (H * 0.5f + 20.f * bam(i, bt, 8)) * s * 0.7f;
        float sw = std::sin(ph + static_cast<float>(i) * 0.9f + w) * 2.f * s + w * 2.f * s;
        duongTan(va, {rx, ry}, {rx + sw, std::min(ry + len, y - 2.f)}, 1.8f * s, 0.8f * s, mau(96, 64, 40), mau(96, 64, 40, 160));
    }
}

void veTre(VA& va, float x, float y, float s, float w, float ph, int bt) {
    bongDo(va, x - 3.f * s, y + 2.f * s, 20.f * s, 4.f * s, 55);
    const int n = 4 + bt % 3;
    for (int i = 0; i < n; ++i) {
        float fi = static_cast<float>(i) - static_cast<float>(n - 1) * 0.5f;
        float h = (44.f + 16.f * bam(i, bt, 4)) * s;
        float lean = fi * 2.4f * s;
        Color th = pha(mau(86, 138, 56), mau(116, 168, 70), bam(i, bt, 9));
        float prevX = x + fi * 5.f * s, prevY = y;
        const int seg = 6;
        for (int sidx = 1; sidx <= seg; ++sidx) {
            float f = static_cast<float>(sidx) / seg;
            float px = x + fi * 5.f * s + lean * f + w * f * f * 5.f * s;
            float py = y - h * f;
            float d0 = (3.2f - 1.4f * (f - 1.f / seg)) * s, d1 = (3.2f - 1.4f * f) * s;
            duongTan(va, {prevX, prevY}, {px, py}, d0, d1, th, th);
            // mắt tre
            chuNhat(va, px - d1 * 0.5f - 0.6f, py - 0.4f * s, d1 + 1.2f, 1.4f * s, mau(52, 96, 40));
            // sáng một bên thân
            duong(va, {prevX - d0 * 0.25f, prevY}, {px - d1 * 0.25f, py}, 0.8f * s, mau(180, 214, 110, 120));
            prevX = px; prevY = py;
        }
        // lá tre ở ngọn
        float tx = prevX, ty = prevY;
        for (int l = 0; l < 6; ++l) {
            float a = -PI * (0.12f + 0.76f * static_cast<float>(l) / 5.f);
            float len = (13.f + 5.f * bam(l, i + bt, 3)) * s;
            float sw = std::sin(ph * 2.f + static_cast<float>(l + i)) * 1.3f * s + w * 3.f * s;
            Vector2f tip{tx + std::cos(a) * len + sw, ty + std::sin(a) * len * 0.55f + len * 0.35f};
            Color lc = (l % 2) ? mau(104, 164, 62) : mau(74, 128, 48);
            tam(va, {tx - 1.1f * s, ty}, {tx + 1.1f * s, ty}, tip, lc);
        }
        // chùm lá bên hông
        float f2 = 0.72f;
        float px2 = x + fi * 5.f * s + lean * f2 + w * f2 * f2 * 5.f * s, py2 = y - h * f2;
        for (int l = 0; l < 2; ++l) {
            float sg = l ? 1.f : -1.f;
            Vector2f tip{px2 + sg * 11.f * s + w * 2.f * s, py2 + 2.f * s};
            tam(va, {px2, py2 - 1.f * s}, {px2, py2 + 1.f * s}, tip, mau(90, 148, 56));
        }
    }
}

void veCau(VA& va, float x, float y, float s, float w, float ph, int bt) {
    bongDo(va, x - 2.f * s, y + 2.f * s, 12.f * s, 3.f * s, 55);
    const float h = (54.f + 8.f * static_cast<float>(bt % 3)) * s;
    float prevX = x, prevY = y;
    for (int i = 1; i <= 6; ++i) {
        float f = static_cast<float>(i) / 6.f;
        float px = x + 3.f * s * std::sin(f * 2.f) + w * f * f * 4.f * s;
        float py = y - h * f;
        Color c = pha(mau(144, 124, 88), mau(172, 150, 104), bam(i, bt, 2));
        duongTan(va, {prevX, prevY}, {px, py}, (3.2f - 1.1f * (f - 1.f / 6.f)) * s, (3.2f - 1.1f * f) * s, c, c);
        chuNhat(va, px - 1.8f * s, py, 3.6f * s, 0.9f * s, mau(100, 84, 56, 190));
        prevX = px; prevY = py;
    }
    // quả cau
    elip(va, prevX - 2.f * s, prevY + 3.f * s, 2.f * s, 2.6f * s, mau(200, 140, 40), 6);
    elip(va, prevX + 2.f * s, prevY + 4.f * s, 2.f * s, 2.6f * s, mau(184, 120, 36), 6);
    // tàu lá
    for (int i = 0; i < 7; ++i) {
        float a = -PI * (0.04f + 0.92f * static_cast<float>(i) / 6.f);
        float len = (22.f + 4.f * bam(i, bt, 11)) * s;
        float sw = std::sin(ph * 2.2f + static_cast<float>(i)) * 1.4f * s + w * 3.f * s;
        Vector2f mid{prevX + std::cos(a) * len * 0.55f, prevY + std::sin(a) * len * 0.55f - 2.f * s};
        Vector2f tip{prevX + std::cos(a) * len + sw, prevY + std::sin(a) * len * 0.7f + len * 0.42f};
        Color c = (i % 2) ? mau(70, 130, 62) : mau(98, 156, 70);
        tam(va, {prevX, prevY}, mid + Vector2f(0.f, 2.f * s), tip, toi(c, 0.85f));
        tam(va, {prevX, prevY}, mid - Vector2f(0.f, 1.f * s), tip, c);
    }
}

void veDuoc(VA& va, float x, float y, float s, float w, float ph, int bt) {
    // rễ chống (stilt roots) cong như vòm
    const Color re = mau(86, 56, 34);
    for (int i = 0; i < 6; ++i) {
        float fi = static_cast<float>(i) - 2.5f;
        Vector2f a{x + fi * 1.8f * s, y - 17.f * s};
        Vector2f m{x + fi * 6.f * s, y - 9.f * s};
        Vector2f b{x + fi * 9.f * s, y};
        duongTan(va, a, m, 2.f * s, 1.5f * s, re, re);
        duongTan(va, m, b, 1.5f * s, 1.f * s, re, toi(re, 0.7f));
    }
    elip(va, x, y + 0.5f, 18.f * s, 2.4f * s, mau(30, 22, 18, 150), 10);   // bùn ướt quanh gốc
    chuNhat(va, x - 2.2f * s, y - 28.f * s, 4.4f * s, 14.f * s, mau(76, 50, 32));
    float cx = x + w * 1.4f * s, cy = y - 30.f * s;
    Color dark = mau(28, 80, 50), mid = mau(52, 118, 64), lite = mau(104, 170, 86);
    struct B { float dx, dy, r; };
    static const B blobs[4] = {{-9.f, 2.f, 10.f}, {9.f, 3.f, 10.f}, {0.f, -6.f, 12.f}, {0.f, 5.f, 9.f}};
    for (int i = 0; i < 4; ++i) {
        float fl = std::sin(ph * 3.f + static_cast<float>(i)) * 0.7f * s;
        elip(va, cx + blobs[i].dx * s + fl, cy + blobs[i].dy * s, blobs[i].r * s, blobs[i].r * s * 0.85f, dark, 10);
    }
    for (int i = 0; i < 4; ++i) {
        float fl = std::sin(ph * 3.f + static_cast<float>(i)) * 0.7f * s;
        elip(va, cx + blobs[i].dx * s + fl - 0.8f * s, cy + blobs[i].dy * s - 1.8f * s, blobs[i].r * s * 0.75f, blobs[i].r * s * 0.6f, mid, 10);
    }
    elip(va, cx - 2.f * s, cy - 11.f * s, 6.f * s, 3.f * s, lite, 8);
    if (bt % 2) {   // quả đước (trụ mầm) rủ xuống
        for (int i = 0; i < 3; ++i)
            duong(va, {cx - 8.f * s + static_cast<float>(i) * 8.f * s, cy + 8.f * s},
                  {cx - 8.f * s + static_cast<float>(i) * 8.f * s + w, cy + 14.f * s}, 1.2f * s, mau(120, 150, 60));
    }
}

void veBui(VA& va, float x, float y, float s, float w, float ph, int bt) {
    bongDo(va, x - 2.f * s, y + 1.5f * s, 14.f * s, 3.f * s, 55);
    struct B { float dx, dy, r; };
    static const B blobs[3] = {{-6.f, -4.f, 7.f}, {6.f, -4.f, 8.f}, {0.f, -9.f, 8.f}};
    Color dark = (bt % 2) ? mau(40, 92, 48) : mau(34, 80, 50);
    Color mid  = (bt % 2) ? mau(78, 140, 56) : mau(60, 124, 62);
    for (int i = 0; i < 3; ++i) {
        float fl = w * 1.f * s * (i + 1) * 0.5f;
        elip(va, x + blobs[i].dx * s + fl, y + blobs[i].dy * s, blobs[i].r * s, blobs[i].r * s * 0.9f, dark, 10);
    }
    for (int i = 0; i < 3; ++i) {
        float fl = w * 1.f * s * (i + 1) * 0.5f;
        elip(va, x + blobs[i].dx * s + fl - 0.6f * s, y + blobs[i].dy * s - 1.5f * s, blobs[i].r * s * 0.72f, blobs[i].r * s * 0.6f, mid, 10);
    }
    if (bt % 3 == 0) {
        const Color hoa[3] = {mau(250, 236, 120), mau(244, 140, 170), mau(255, 255, 255)};
        for (int i = 0; i < 4; ++i)
            elip(va, x + (-7.f + static_cast<float>(i) * 4.5f) * s + w * s, y + (-10.f + 3.f * bam(i, bt, 6)) * s, 1.5f * s, 1.5f * s, hoa[bt % 3], 5);
    }
    (void)ph;
}

void veLau(VA& va, float x, float y, float s, float w, float ph, int bt) {
    const int n = 5 + bt % 4;
    for (int i = 0; i < n; ++i) {
        float fi = static_cast<float>(i) - static_cast<float>(n - 1) * 0.5f;
        float bx = x + fi * 2.6f * s;
        float h = (24.f + 16.f * bam(i, bt, 13)) * s;
        float lean = fi * 1.5f * s;
        float sw = (w * 3.2f + std::sin(ph * 1.7f + static_cast<float>(i) * 0.8f) * 1.1f) * s;
        Vector2f mid{bx + lean * 0.4f + sw * 0.25f, y - h * 0.55f};
        Vector2f tip{bx + lean + sw, y - h};
        Color goc = mau(98, 120, 56);
        Color ngon = mau(206, 188, 96);
        tu(va, {bx - 1.3f * s, y}, {bx + 1.3f * s, y}, mid + Vector2f(0.9f * s, 0.f), mid - Vector2f(0.9f * s, 0.f),
           goc, goc, pha(goc, ngon, 0.5f), pha(goc, ngon, 0.5f));
        tam3(va, mid - Vector2f(0.9f * s, 0.f), mid + Vector2f(0.9f * s, 0.f), tip, pha(goc, ngon, 0.5f), pha(goc, ngon, 0.5f), ngon);
        if (i % 3 == 0) {   // bông lau trắng bạc ánh vàng
            tam(va, tip + Vector2f(0.f, -1.f * s), tip + Vector2f(-2.4f * s, 8.f * s), tip + Vector2f(2.4f * s, 8.f * s), mau(244, 224, 200, 230));
            tam(va, tip + Vector2f(0.f, -1.f * s), tip + Vector2f(0.f, 8.f * s), tip + Vector2f(2.4f * s, 8.f * s), mau(255, 244, 220, 230));
        }
    }
}

void veCo(VA& va, float x, float y, float s, float w, float t, float ph) {
    const float cao = 40.f * s;
    // cột cờ
    chuNhat(va, x - 1.3f * s, y - cao, 2.6f * s, cao + 2.f, kGo);
    chuNhat(va, x - 1.3f * s, y - cao, 1.1f * s, cao, kGoSang);
    elip(va, x, y - cao - 1.f * s, 2.4f * s, 2.4f * s, kVang, 8);
    // lá cờ lượn sóng
    const int N = 9;
    const float dai = 30.f * s * (0.86f + 0.14f * w);
    const float cc = 18.f * s;
    float topY = y - cao + 2.f * s;
    float prevX = x + 1.3f * s;
    float prevOff = 0.f;
    for (int i = 1; i <= N; ++i) {
        float f = static_cast<float>(i) / N;
        float px = x + 1.3f * s + dai * f;
        float off = std::sin(t * 5.5f + ph + f * 6.f) * 2.4f * s * f + w * 1.6f * s * f;
        float sh = 0.5f + 0.5f * std::sin(t * 5.5f + ph + f * 6.f + 1.2f);
        float f0 = static_cast<float>(i - 1) / N;
        float sh0 = 0.5f + 0.5f * std::sin(t * 5.5f + ph + f0 * 6.f + 1.2f);
        Color c0 = mau(184 + 44.f * sh0, 24 + 14.f * sh0, 24 + 14.f * sh0);
        Color c1 = mau(184 + 44.f * sh, 24 + 14.f * sh, 24 + 14.f * sh);
        float h1 = cc * (1.f - 0.18f * f);
        float h0 = cc * (1.f - 0.18f * f0);
        tu(va, {prevX, topY + prevOff}, {px, topY + off}, {px, topY + off + h1}, {prevX, topY + prevOff + h0}, c0, c1, c1, c0);
        // viền vàng trên & dưới
        chuNhat(va, prevX, topY + prevOff, px - prevX + 0.3f, 1.5f * s, kVang);
        tu(va, {prevX, topY + prevOff + h0 - 1.5f * s}, {px, topY + off + h1 - 1.5f * s}, {px, topY + off + h1}, {prevX, topY + prevOff + h0},
           kVang, kVang, kVang, kVang);
        prevX = px; prevOff = off;
    }
    // đuôi nheo hình đuôi én
    float fN = 1.f;
    float offN = std::sin(t * 5.5f + ph + fN * 6.f) * 2.4f * s + w * 1.6f * s;
    float hN = cc * 0.82f;
    tam(va, {prevX, topY + offN}, {prevX, topY + offN + hN}, {prevX - 6.f * s, topY + offN + hN * 0.5f}, mau(170, 160, 150, 0));
    // đĩa mặt trời vàng giữa cờ
    float fm = 0.45f;
    float offM = std::sin(t * 5.5f + ph + fm * 6.f) * 2.4f * s * fm + w * 1.6f * s * fm;
    elip(va, x + 1.3f * s + dai * fm, topY + offM + cc * 0.5f, 3.6f * s, 3.6f * s, kVang, 10);
    elip(va, x + 1.3f * s + dai * fm, topY + offM + cc * 0.5f, 2.2f * s, 2.2f * s, mau(255, 226, 120), 8);
}

void veLua(VA& va, float x, float y, float s, float t, float ph, int bt) {
    float fl = 0.92f + 0.08f * std::sin(t * 17.f + ph) * std::sin(t * 7.3f + ph * 2.f);
    if (bt == 0) {
        // vòng đá
        for (int i = 0; i < 8; ++i) {
            float a = 2.f * PI * static_cast<float>(i) / 8.f;
            elip(va, x + std::cos(a) * 13.f * s, y + 3.f * s + std::sin(a) * 4.5f * s, 3.4f * s, 2.6f * s,
                 (i % 2) ? mau(112, 112, 116) : mau(86, 88, 94), 7);
        }
        // ánh lửa hắt lên mặt đất
        elipTan(va, x, y + 2.f * s, 64.f * s * fl, 24.f * s * fl, mau(255, 150, 60, 80), mau(255, 120, 40, 0), 18);
        // củi
        duong(va, {x - 10.f * s, y + 3.f * s}, {x + 9.f * s, y - 2.f * s}, 4.f * s, mau(82, 52, 30));
        duong(va, {x + 10.f * s, y + 3.f * s}, {x - 9.f * s, y - 2.f * s}, 4.f * s, mau(66, 42, 24));
    } else {
        duong(va, {x, y}, {x, y + 10.f * s}, 2.6f * s, kGoToi);
        elipTan(va, x, y - 6.f * s, 30.f * s * fl, 24.f * s * fl, mau(255, 160, 70, 70), mau(255, 120, 40, 0), 14);
    }
    // ngọn lửa 3 lớp
    const float scl[3] = {1.f, 0.7f, 0.4f};
    const Color mc[3] = {mau(255, 84, 18, 235), mau(255, 168, 40, 240), mau(255, 244, 160, 245)};
    for (int l = 0; l < 3; ++l) {
        for (int tg = -1; tg <= 1; ++tg) {
            float off = static_cast<float>(tg) * 4.6f * s * scl[l];
            float fh = (20.f + 9.f * std::sin(t * 11.f + static_cast<float>(tg) * 2.1f + ph)) * scl[l] * s * (tg == 0 ? 1.25f : 0.9f);
            float tipx = x + off * 0.6f + std::sin(t * 9.f + ph + static_cast<float>(tg)) * 2.4f * s;
            tam(va, {x + off - 5.5f * s * scl[l], y - 1.f * s}, {x + off + 5.5f * s * scl[l], y - 1.f * s}, {tipx, y - fh}, mc[l]);
        }
        elip(va, x, y - 4.f * s * scl[l], 6.8f * s * scl[l], 5.4f * s * scl[l], mc[l], 10);
    }
    // tàn lửa
    for (int i = 0; i < 6; ++i) {
        float p = std::fmod(t * 0.7f + static_cast<float>(i) * 0.19f + ph, 1.f);
        float sx = x + std::sin(p * 6.f + static_cast<float>(i) * 2.3f) * 9.f * s;
        float sy = y - 12.f * s - p * 50.f * s;
        float a = (1.f - p) * 230.f;
        float sz = 1.7f * s * (1.f - p * 0.5f);
        chuNhat(va, sx, sy, sz, sz, mau(255, 200, 90, a));
    }
    // khói
    for (int i = 0; i < 3; ++i) {
        float p = std::fmod(t * 0.25f + static_cast<float>(i) * 0.34f + ph, 1.f);
        float sy = y - 24.f * s - p * 62.f * s;
        float sx = x + (7.f * p * static_cast<float>(i + 1)) * s + std::sin(p * 5.f + ph) * 3.f * s;
        float r = (4.f + 11.f * p) * s;
        elip(va, sx, sy, r, r * 0.8f, mau(120, 108, 112, (1.f - p) * 55.f * (bt == 0 ? 1.f : 0.6f)), 9);
    }
}

}  // namespace

void BanDo::VeVatDong(sf::RenderWindow& window) {
    lopDong.clear();
    const float t = thoiGian;
    for (const auto& v : danhSachVat) {
        float w = gio(v.x, t + v.pha * 0.1f);
        switch (v.loai) {
            case LoaiVat::CAY_LA: veCayLa(lopDong, v.x, v.y, v.kichThuoc, w, v.pha + t, v.bienThe); break;
            case LoaiVat::CAY_DA: veCayDa(lopDong, v.x, v.y, v.kichThuoc, w, v.pha + t, v.bienThe); break;
            case LoaiVat::TRE:    veTre(lopDong, v.x, v.y, v.kichThuoc, w, v.pha + t, v.bienThe); break;
            case LoaiVat::CAU:    veCau(lopDong, v.x, v.y, v.kichThuoc, w, v.pha + t, v.bienThe); break;
            case LoaiVat::DUOC:   veDuoc(lopDong, v.x, v.y, v.kichThuoc, w, v.pha + t, v.bienThe); break;
            case LoaiVat::BUI:    veBui(lopDong, v.x, v.y, v.kichThuoc, w, v.pha + t, v.bienThe); break;
            case LoaiVat::LAU:    veLau(lopDong, v.x, v.y, v.kichThuoc, w, v.pha + t, v.bienThe); break;
            case LoaiVat::CO:     veCo(lopDong, v.x, v.y, v.kichThuoc, w, t, v.pha); break;
            case LoaiVat::LUA:    veLua(lopDong, v.x, v.y, v.kichThuoc, t, v.pha, v.bienThe); break;
        }
    }
    window.draw(lopDong);
}

void BanDo::VeCocThuThap(sf::RenderWindow& window) {
    lopDong.clear();
    const float t = thoiGian;
    for (const auto& c : danhSachCocRoi) {
        if (c.daNhat) continue;
        const sf::Vector2f p = c.viTri;
        const float bob = std::sin(t * 3.f + p.x * 0.01f) * 2.2f;
        const float py = p.y + bob;
        const float nhip = 0.5f + 0.5f * std::sin(t * 4.f + p.x);

        // bóng + hào quang nhấp nháy
        bongDo(lopDong, p.x, p.y + 11.f, 15.f - bob * 0.3f, 3.2f, 70);
        elipTan(lopDong, p.x, py, 26.f + 4.f * nhip, 22.f + 3.f * nhip, mau(255, 240, 130, 90), mau(255, 220, 80, 0), 20);
        vong(lopDong, p.x, py, 20.f + 2.f * nhip, 20.f + 2.f * nhip, 1.2f, mau(255, 230, 90, 150), 24);

        // cọc gỗ nằm ngang bịt sắt (vị trí giữ nguyên như bản cũ)
        chuNhat(lopDong, p.x - 15.5f, py - 5.f, 31.f, 10.f, mau(50, 26, 12));
        chuNhatDoc(lopDong, p.x - 14.f, py - 4.5f, 28.f, 9.f, mau(168, 112, 60), mau(112, 70, 36));
        for (int i = 0; i < 4; ++i)
            duong(lopDong, {p.x - 10.f + static_cast<float>(i) * 6.f, py - 3.f}, {p.x - 9.f + static_cast<float>(i) * 6.f, py + 3.f}, 0.8f, mau(70, 40, 20, 150));
        chuNhat(lopDong, p.x + 6.f, py - 5.f, 2.4f, 10.f, mau(70, 76, 84));
        tam(lopDong, {p.x + 14.f, py - 4.6f}, {p.x + 14.f, py + 4.6f}, {p.x + 22.f, py}, kSat);
        tam(lopDong, {p.x + 14.f, py - 4.6f}, {p.x + 22.f, py}, {p.x + 15.f, py - 1.f}, mau(255, 255, 255, 170));

        // 4 tia lấp lánh xoay quanh
        for (int i = 0; i < 4; ++i) {
            float a = t * 1.6f + static_cast<float>(i) * PI * 0.5f;
            float sx = p.x + std::cos(a) * 19.f, sy = py + std::sin(a) * 11.f;
            float q = 0.5f + 0.5f * std::sin(t * 6.f + static_cast<float>(i) * 1.7f);
            float r = 1.4f + 1.8f * q;
            tam(lopDong, {sx, sy - r * 1.8f}, {sx + r * 0.5f, sy}, {sx - r * 0.5f, sy}, mau(255, 246, 190, 210));
            tam(lopDong, {sx, sy + r * 1.8f}, {sx + r * 0.5f, sy}, {sx - r * 0.5f, sy}, mau(255, 246, 190, 210));
        }
        // mũi tên chỉ xuống
        float ay = py - 28.f + std::sin(t * 5.f + p.x) * 3.f;
        tam(lopDong, {p.x - 5.f, ay - 6.f}, {p.x + 5.f, ay - 6.f}, {p.x, ay + 1.f}, mau(255, 226, 80, 230));
        tam(lopDong, {p.x - 5.f, ay - 6.f}, {p.x, ay - 6.f}, {p.x, ay + 1.f}, mau(255, 255, 200, 200));
    }
    window.draw(lopDong);
}

void BanDo::VeBongCocDuoiNuoc(sf::RenderWindow& window) {
    lopDong.clear();
    const float tl = song.GetTiLeThuyTrieu();
    for (const auto& coc : danhSachCoc) {
        if (!coc.IsDaCam()) continue;
        sf::FloatRect hb = coc.GetHitBox();
        float cx = hb.position.x + hb.size.x * 0.5f;
        float cy = hb.position.y + hb.size.y * 0.5f;
        float w = hb.size.x, h = hb.size.y;
        // bóng cọc ngầm dưới làn nước
        elip(lopDong, cx + 3.f, cy + h * 0.42f, w * 1.1f, w * 0.34f, mau(6, 24, 40, 70.f * (0.4f + 0.6f * tl)), 10);
        // dòng nước rẽ quanh cọc khi triều đã rút
        if (tl < 0.75f) {
            float a = (1.f - tl / 0.75f) * 120.f;
            float yy = cy + h * 0.38f;
            for (int sg = -1; sg <= 1; sg += 2) {
                float sgf = static_cast<float>(sg);
                duongTan(lopDong, {cx + w * 0.55f, yy + sgf * 1.f}, {cx + w * 0.55f + 34.f, yy + sgf * 9.f}, 1.8f, 0.5f,
                         mau(226, 244, 252, a), mau(226, 244, 252, 0));
            }
        }
    }
    window.draw(lopDong);
}

void BanDo::VeSuongVaHat(sf::RenderWindow& window) {
    lopDong.clear();
    const float t = thoiGian;
    // sương mù trôi trên mặt sông
    for (const auto& s : danhSachSuong) {
        float yy = s.y + std::sin(t * 0.2f + s.x * 0.01f) * 6.f;
        elipTan(lopDong, s.x + s.rong * 0.5f, yy, s.rong * 0.5f, s.cao, mau(240, 208, 200, s.doMo), mau(240, 208, 200, 0), 18);
    }
    // đom đóm ven bờ Nam
    for (const auto& d : danhSachDomDom) {
        float px = d.goc.x + std::sin(t * d.tocDo + d.pha) * d.bienDo;
        float py = d.goc.y + std::sin(t * d.tocDo * 1.3f + d.pha * 2.f) * d.bienDo * 0.5f;
        float b = 0.5f + 0.5f * std::sin(t * 2.f + d.pha * 3.f);
        b = b * b;
        elipTan(lopDong, px, py, 7.f, 7.f, mau(230, 255, 120, 90.f * b), mau(230, 255, 120, 0), 8);
        chuNhat(lopDong, px - 0.9f, py - 0.9f, 1.8f, 1.8f, mau(255, 255, 190, 60.f + 195.f * b));
    }
    // hạt hiệu ứng (nước bắn, mảnh gỗ, lấp lánh khi nhặt cọc)
    for (const auto& h : danhSachHat) {
        float p = h.tuoi / h.tuoiMax;
        if (h.kieu == 1) {
            float r = h.kichThuoc * p;
            vong(lopDong, h.viTri.x, h.viTri.y, r, r * 0.4f, 1.5f, vaA(h.mau, h.mau.a * (1.f - p)), 26);
        } else {
            float a = h.mau.a * (1.f - p * p);
            float sz = h.kichThuoc * (1.f - p * 0.4f);
            chuNhat(lopDong, h.viTri.x - sz * 0.5f, h.viTri.y - sz * 0.5f, sz, sz, vaA(h.mau, a));
        }
    }
    window.draw(lopDong);
}

void BanDo::VeTroiDong(sf::RenderWindow& window) {
    lopDong.clear();
    const float t = thoiGian;
    // mây hoàng hôn: đáy tím, đỉnh ánh cam
    for (const auto& m : danhSachMay) {
        const int n = 6;
        for (int pass = 0; pass < 3; ++pass) {
            for (int i = 0; i < n; ++i) {
                float f = (static_cast<float>(i) + 0.5f) / n;
                float cx = m.x + m.rong * f;
                float r = m.cao * (0.8f + 0.9f * bam(i, static_cast<int>(m.rong), 5)) * (1.f - 0.5f * std::abs(f - 0.5f));
                float cy = m.y - r * 0.2f;
                if (pass == 0)      elip(lopDong, cx, cy + m.cao * 0.3f, r * 1.6f, r * 0.7f, mau(120, 80, 130, m.doMo * 0.8f), 12);
                else if (pass == 1) elip(lopDong, cx, cy, r * 1.5f, r * 0.75f, mau(246, 170, 150, m.doMo), 12);
                else                elip(lopDong, cx + r * 0.1f, cy - r * 0.32f, r * 1.0f, r * 0.34f, mau(255, 224, 180, m.doMo * 0.8f), 10);
            }
        }
    }
    // chim én (đen) và cò trắng bay
    for (const auto& c : danhSachChim) {
        float flap = std::sin(t * (c.trang ? 5.f : 9.f) + c.pha);
        float s = c.kichThuoc;
        float by = c.y + std::sin(t * 0.6f + c.pha) * 3.f;
        if (c.trang) {
            Color th = mau(246, 240, 232);
            elip(lopDong, c.x, by, 5.5f * s, 2.2f * s, th, 8);
            elip(lopDong, c.x + 6.f * s, by - 1.5f * s, 1.8f * s, 1.5f * s, th, 6);
            tam(lopDong, {c.x + 7.5f * s, by - 1.6f * s}, {c.x + 11.f * s, by - 1.2f * s}, {c.x + 7.5f * s, by - 0.8f * s}, mau(240, 180, 70));
            duong(lopDong, {c.x - 5.f * s, by + 0.5f * s}, {c.x - 12.f * s, by + 2.5f * s}, 1.2f * s, mau(120, 100, 90));
            tam(lopDong, {c.x - 2.f * s, by - 1.f * s}, {c.x + 3.f * s, by - 1.f * s}, {c.x - 1.f * s, by - (9.f * s) * flap - 2.f * s}, mau(255, 255, 255));
            tam(lopDong, {c.x - 2.f * s, by - 1.f * s}, {c.x + 3.f * s, by - 1.f * s}, {c.x + 8.f * s, by - (7.f * s) * flap - 1.f * s}, mau(232, 228, 224));
        } else {
            Color th = mau(26, 24, 40, 220);
            float wy = (6.f * s) * flap;
            duong(lopDong, {c.x, by}, {c.x - 7.f * s, by - wy}, 1.6f * s, th);
            duong(lopDong, {c.x, by}, {c.x + 7.f * s, by - wy}, 1.6f * s, th);
            elip(lopDong, c.x, by, 2.2f * s, 1.2f * s, th, 6);
        }
    }
    window.draw(lopDong);
}

// =============================================================================
//  PHẦN 8 - VẼ TOÀN BẢN ĐỒ
// =============================================================================

void BanDo::Ve(sf::RenderWindow& window) {
    // Thứ tự vẽ: trời -> mây/chim -> núi & bờ Bắc -> sông -> bờ Nam -> cây cối/doanh trại động ->
    //            cọc nhặt -> cọc ngầm -> sương/đom đóm/hạt
    window.draw(lopTroi);
    VeTroiDong(window);
    window.draw(lopBac);

    song.Ve(window);

    window.draw(lopNam);
    VeVatDong(window);
    VeCocThuThap(window);

    VeBongCocDuoiNuoc(window);
    const float tiLe = song.GetTiLeThuyTrieu();
    for (auto& coc : danhSachCoc) {
        coc.Ve(window, tiLe);
    }

    VeSuongVaHat(window);
}

void BanDo::VeTienCanh(sf::RenderWindow& window) {
    lopDong.clear();
    const float W = chieuRong, H = chieuCao;
    const float t = thoiGian;

    // tia nắng xuyên qua màn sương
    const float sx = W * 0.72f, sy = 80.f;
    for (int i = 0; i < 7; ++i) {
        float a = -0.55f + 0.18f * static_cast<float>(i);
        float k = 0.5f + 0.5f * std::sin(t * 0.35f + static_cast<float>(i) * 1.9f);
        float al = 9.f + 14.f * k;
        float bx = sx + std::tan(a) * (H - sy);
        float wb = 40.f + 24.f * static_cast<float>(i % 3);
        tu(lopDong, {sx - 4.f, sy}, {sx + 4.f, sy}, {bx + wb, H}, {bx - wb, H},
           mau(255, 214, 150, al), mau(255, 214, 150, al), mau(255, 214, 150, 0), mau(255, 214, 150, 0));
    }

    // vignette: tối dần ở các mép màn hình
    chuNhatNgang(lopDong, 0.f, 0.f, 170.f, H, mau(8, 10, 24, 150), mau(8, 10, 24, 0));
    chuNhatNgang(lopDong, W - 170.f, 0.f, 170.f, H, mau(8, 10, 24, 0), mau(8, 10, 24, 150));
    chuNhatDoc(lopDong, 0.f, 0.f, W, 60.f, mau(8, 10, 30, 110), mau(8, 10, 30, 0));
    chuNhatDoc(lopDong, 0.f, H - 110.f, W, 110.f, mau(6, 10, 12, 0), mau(6, 10, 12, 150));

    // cỏ tiền cảnh mờ đung đưa sát mép dưới
    for (float x = -4.f; x < W; x += 9.f) {
        float n = bam(static_cast<int>(x), 4, 8);
        float h = 14.f + 24.f * n * n;
        float w = gio(x, t);
        Color c = (static_cast<int>(x / 9.f) % 3 == 0) ? mau(12, 30, 18) : mau(20, 48, 26);
        tam(lopDong, {x, H + 2.f}, {x + 5.f, H + 2.f}, {x + 2.5f + w * 5.f * (h / 38.f), H - h}, c);
    }
    // vài cọng lau lớn ở hai góc
    for (int sd = 0; sd < 2; ++sd) {
        for (int i = 0; i < 4; ++i) {
            float bx = sd == 0 ? 8.f + static_cast<float>(i) * 11.f : W - 8.f - static_cast<float>(i) * 11.f;
            float h = 58.f + 22.f * bam(i, sd, 3);
            float sw = gio(bx, t) * 8.f;
            Color c = mau(10, 22, 16, 235);
            tam(lopDong, {bx - 2.f, H + 2.f}, {bx + 2.f, H + 2.f}, {bx + sw + (sd == 0 ? 6.f : -6.f), H - h}, c);
            elip(lopDong, bx + sw + (sd == 0 ? 6.f : -6.f), H - h - 4.f, 2.4f, 8.f, mau(24, 30, 26, 235), 8);
        }
    }
    window.draw(lopDong);
}


