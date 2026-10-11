
#include "KeDich.h"
#include "../BanDo/DoHoa.h"
#include <cmath>
#include <algorithm>
#include <functional>

using namespace dohoa;

namespace {
const float TG_LAY_DA = 0.30f;      // Thời gian giơ đao lấy đà trước khi chém
}

KeDich::KeDich(sf::Vector2f viTriBanDau, float tocDo, int mau, int dame,
               float tamDanh, float tamPhatHien, LoaiKeDich loaiDich)
    : NhanVat(viTriBanDau, tocDo, mau, dame),
      loai(loaiDich),
      tamDanh(tamDanh),
      tamPhatHien(tamPhatHien),
      trangThaiHienTai(TrangThaiKeDich::DUNG_YEN),
      thoiGianHoiChieu(loaiDich == LoaiKeDich::TUONG_HOANG_THAO ? 0.55f : 0.85f),
      thoiGianTroiQua(1.0f),
      huongNhin(-1),
      thoiGianHoatAnh(0.f),
      nhipBuocChan(0.f),
      thoiGianBiThuong(0.f),
      dangLayDa(false),
      thoiGianLayDa(0.f),
      thoiGianSauDon(99.f),
      thoiGianChet(0.f),
      bienThe(static_cast<int>(std::abs(viTriBanDau.x * 7.f + viTriBanDau.y * 13.f)) & 3) {}

bool KeDich::PhatHienNguoiChoi(sf::Vector2f viTriPlayer) const {
    if (daHySinh) return false;
    float dx = viTriPlayer.x - viTri.x;
    float dy = viTriPlayer.y - viTri.y;
    return (dx * dx + dy * dy) <= (tamPhatHien * tamPhatHien);
}

void KeDich::DuoiTheo(sf::Vector2f viTriPlayer, float deltaTime) {
    if (daHySinh) return;

    float dx = viTriPlayer.x - viTri.x;
    float dy = viTriPlayer.y - viTri.y;
    float khoangCach = std::sqrt(dx * dx + dy * dy);

    huongNhin = (dx >= 0) ? 1 : -1;

    if (khoangCach <= tamDanh) {
        trangThaiHienTai = TrangThaiKeDich::TAN_CONG;
        return;
    }

    if (khoangCach > 0.0001f) {
        sf::Vector2f huong(dx / khoangCach, dy / khoangCach);
        DiChuyen(huong, deltaTime);
        trangThaiHienTai = TrangThaiKeDich::DUOI_THEO;
    }
}

bool KeDich::CoTheTanCong(sf::Vector2f viTriPlayer) const {
    if (daHySinh) return false;
    float dx = viTriPlayer.x - viTri.x;
    float dy = viTriPlayer.y - viTri.y;
    float khoangCach = std::sqrt(dx * dx + dy * dy);
    return (khoangCach <= tamDanh) && (thoiGianTroiQua >= thoiGianHoiChieu);
}

bool KeDich::TanCong(NhanVat& mucTieu) {
    if (daHySinh) return false;
    if (thoiGianTroiQua < thoiGianHoiChieu) return false;

    // Gọi lần đầu: bắt đầu giơ đao lấy đà. Chỉ chém trúng sau khi lấy đà xong (người chơi có thể né).
    if (!dangLayDa) {
        dangLayDa = true;
        thoiGianLayDa = 0.f;
        return false;
    }
    if (thoiGianLayDa < TG_LAY_DA) return false;

    mucTieu.NhanSatThuong(satThuong);
    thoiGianTroiQua = 0.0f;
    thoiGianSauDon = 0.f;
    dangLayDa = false;
    trangThaiHienTai = TrangThaiKeDich::TAN_CONG;
    return true;
}

void KeDich::NhanSatThuong(int damage) {
    NhanVat::NhanSatThuong(damage);
    thoiGianBiThuong = 0.18f; // Nháy trắng khi trúng đòn
    if (sucKhoe <= 0) {
        Chet();
    } else {
        trangThaiHienTai = TrangThaiKeDich::BI_THUONG;
    }
}

void KeDich::Chet() {
    HySinh();
    thoiGianChet = 0.f;
    dangLayDa = false;
    trangThaiHienTai = TrangThaiKeDich::DA_CHET;
}

sf::FloatRect KeDich::GetHitBox() const {
    float rong = (loai == LoaiKeDich::TUONG_HOANG_THAO) ? 36.f : 26.f;
    float cao = (loai == LoaiKeDich::TUONG_HOANG_THAO) ? 44.f : 32.f;
    return sf::FloatRect({viTri.x - rong * 0.5f, viTri.y - cao * 0.5f}, {rong, cao});
}

void KeDich::CapNhat(float deltaTime) {
    if (daHySinh) {
        thoiGianChet += deltaTime;   // Chỉ để chạy hoạt ảnh ngã
        return;
    }

    thoiGianTroiQua += deltaTime;
    thoiGianHoatAnh += deltaTime;
    thoiGianSauDon += deltaTime;

    // Người chơi bỏ chạy khỏi tầm đánh giữa lúc lấy đà thì hủy đòn
    if (dangLayDa) {
        thoiGianLayDa += deltaTime;
        if (thoiGianLayDa > TG_LAY_DA + 0.45f) dangLayDa = false;
    }

    if (thoiGianBiThuong > 0.f) {
        thoiGianBiThuong -= deltaTime;
    }

    if (trangThaiHienTai == TrangThaiKeDich::DUOI_THEO) {
        nhipBuocChan += deltaTime * 14.f;
    } else {
        nhipBuocChan *= 0.85f;
    }
}

namespace {
const float TG_VUNG_CHEM = 0.14f;   // Thời gian vung đao xuống
const float TG_THU_DAO = 0.26f;     // Thời gian thu đao về thế thủ
const float TG_BIEN_MAT = 1.0f;     // Thời gian ngã xuống rồi mờ dần

float Kep01(float p) { return std::max(0.f, std::min(1.f, p)); }
float Tron(float a, float b, float t) { return a + (b - a) * t; }
float DauRaMuot(float p) { p = Kep01(p); float q = 1.f - p; return 1.f - q * q * q; }
float DauVaoRaMuot(float p) { p = Kep01(p); return p * p * (3.f - 2.f * p); }

// Liễu diệp đao: lưỡi cong về phía sống, có viền tối, hai nửa sáng tối, đĩa chắn bằng đồng, chuôi quấn dây đỏ
void VeDaoCong(VA& va, Vector2f tay, float goc, float s, float dai, const std::function<Color(Color)>& C) {
    Vector2f d(std::cos(goc) * s, std::sin(goc));
    Vector2f n(-d.y, d.x);

    // Tua đỏ ở đầu chuôi
    Vector2f dauChuoi = tay - d * 3.f;
    duong(va, dauChuoi, dauChuoi - d * 7.f + n * 2.f, 1.4f, C(mau(190, 36, 32)));
    elip(va, (dauChuoi - d * 8.f + n * 2.f).x, (dauChuoi - d * 8.f + n * 2.f).y, 2.f, 2.6f, C(mau(220, 54, 42)), 6);

    // Chuôi gỗ quấn dây
    duong(va, tay - d * 2.5f, tay + d * 5.f, 2.8f, C(mau(84, 56, 36)));
    for (int k = 0; k < 2; ++k) {
        Vector2f p = tay + d * (0.3f + static_cast<float>(k) * 2.4f);
        duong(va, p - n * 1.5f, p + n * 1.5f, 1.1f, C(mau(176, 36, 34)));
    }

    // Đĩa chắn đồng
    Vector2f g = tay + d * 5.4f;
    duong(va, g - n * 3.6f, g + n * 3.6f, 2.4f, C(mau(190, 142, 52)));

    // Lưỡi cong: chia 5 đoạn, bề rộng thon dần về mũi
    const int seg = 5;
    Vector2f g0 = g + d * 1.2f;
    float cong = dai * 0.2f;
    auto duongGiua = [&](float t) { return g0 + d * (dai * t) - n * (cong * t * t); };
    auto rong = [&](float t) { return Tron(2.7f, 0.9f, t); };
    for (int i = 0; i < seg; ++i) {
        float t0 = static_cast<float>(i) / seg;
        float t1 = static_cast<float>(i + 1) / seg;
        Vector2f a0 = duongGiua(t0), a1 = duongGiua(t1);
        float w0 = rong(t0), w1 = rong(t1);
        // Viền tối
        tu(va, a0 + n * (w0 + 0.8f), a1 + n * (w1 + 0.8f), a1 - n * (w1 + 0.8f), a0 - n * (w0 + 0.8f),
           C(mau(40, 46, 60)), C(mau(40, 46, 60)), C(mau(40, 46, 60)), C(mau(40, 46, 60)));
        // Thép: nửa lưng sáng, nửa lưỡi tối
        tu(va, a0 + n * w0, a1 + n * w1, a1, a0, C(mau(214, 224, 238)), C(mau(214, 224, 238)), C(mau(236, 242, 250)), C(mau(236, 242, 250)));
        tu(va, a0, a1, a1 - n * w1, a0 - n * w0, C(mau(170, 184, 206)), C(mau(170, 184, 206)), C(mau(126, 140, 166)), C(mau(126, 140, 166)));
    }
    Vector2f mui = duongGiua(1.f) + d * 3.5f - n * 1.f;
    tam3(va, duongGiua(1.f) + n * 0.9f, duongGiua(1.f) - n * 0.9f, mui, C(mau(214, 224, 238)), C(mau(126, 140, 166)), C(mau(240, 245, 252)));
}

// Quan đao của chủ tướng: cán dài, lưỡi to cong như trăng khuyết, vòng vàng và tua đỏ
void VeQuanDao(VA& va, Vector2f tay, float goc, float s, const std::function<Color(Color)>& C) {
    Vector2f d(std::cos(goc) * s, std::sin(goc));
    Vector2f n(-d.y, d.x);

    // Cán gỗ sẫm có đai vàng
    Vector2f dauCan = tay - d * 16.f;
    Vector2f chanLuoi = tay + d * 30.f;
    duong(va, dauCan, chanLuoi, 3.4f, C(mau(78, 48, 32)));
    for (int k = 0; k < 4; ++k) {
        Vector2f p = dauCan + (chanLuoi - dauCan) * (0.12f + 0.24f * static_cast<float>(k));
        duong(va, p - n * 2.2f, p + n * 2.2f, 1.6f, C(mau(222, 168, 48)));
    }
    elip(va, dauCan.x, dauCan.y, 2.6f, 2.6f, C(mau(222, 168, 48)), 8);

    // Tua đỏ dưới chân lưỡi
    Vector2f tuaGoc = chanLuoi - d * 2.f;
    tam3(va, tuaGoc + n * 2.f, tuaGoc - n * 2.f, tuaGoc - d * 9.f + n * 2.5f, C(mau(210, 36, 32)), C(mau(210, 36, 32)), C(mau(150, 24, 24)));

    // Lưỡi bản rộng: sống thẳng (phía -n), bụng lưỡi phồng về phía +n, mũi hơi vênh
    const int seg = 6;
    const float dai = 30.f;
    auto lungLuoi = [&](float t) { return chanLuoi + d * (dai * t) - n * (1.6f + 6.f * t * t * t); };
    auto luoi = [&](float t) { return chanLuoi + d * (dai * t) + n * (10.f * std::sin(3.14159f * std::pow(t, 0.8f)) * (1.f - 0.15f * t)) - n * (6.f * t * t * t); };
    for (int i = 0; i < seg; ++i) {
        float t0 = static_cast<float>(i) / seg;
        float t1 = static_cast<float>(i + 1) / seg;
        tu(va, lungLuoi(t0) - n * 0.8f, lungLuoi(t1) - n * 0.8f, luoi(t1) + n * 0.9f, luoi(t0) + n * 0.9f,
           C(mau(40, 46, 60)), C(mau(40, 46, 60)), C(mau(40, 46, 60)), C(mau(40, 46, 60)));
        tu(va, lungLuoi(t0), lungLuoi(t1), luoi(t1), luoi(t0),
           C(mau(238, 244, 252)), C(mau(220, 230, 244)), C(mau(150, 164, 190)), C(mau(150, 164, 190)));
    }
    // Vòng vàng ở chân lưỡi
    duong(va, chanLuoi - n * 4.f, chanLuoi + n * 4.f, 2.6f, C(mau(240, 190, 60)));
}

// Vệt chém của địch: lưỡi liềm đỏ, đầu sáng trắng hồng
void VeVetDich(VA& va, Vector2f tam0, float s, float gocDau, float gocCuoi, float banKinh, float doDuc, bool boss) {
    const int seg = 16;
    auto vt = [&](float a, float r) { return Vector2f(tam0.x + std::cos(a) * r * s, tam0.y + std::sin(a) * r * 0.92f); };
    for (int i = 0; i < seg; ++i) {
        float t0 = static_cast<float>(i) / seg;
        float t1 = static_cast<float>(i + 1) / seg;
        float a0 = Tron(gocDau, gocCuoi, t0);
        float a1 = Tron(gocDau, gocCuoi, t1);
        float r0 = Tron(banKinh * 0.97f, banKinh * 0.55f, t0);
        float r1 = Tron(banKinh * 0.97f, banKinh * 0.55f, t1);
        float k0 = std::pow(t0, 1.5f) * doDuc;
        float k1 = std::pow(t1, 1.5f) * doDuc;
        Color ngoai0 = boss ? mau(255, 90, 40, 255.f * k0) : mau(255, 120, 110, 235.f * k0);
        Color ngoai1 = boss ? mau(255, 220, 160, 255.f * k1) : mau(255, 232, 226, 235.f * k1);
        Color trong0 = mau(200, 30, 30, 140.f * k0);
        Color trong1 = boss ? mau(255, 130, 50, 160.f * k1) : mau(240, 80, 70, 140.f * k1);
        tu(va, vt(a0, banKinh), vt(a1, banKinh), vt(a1, r1), vt(a0, r0), ngoai0, ngoai1, trong1, trong0);
    }
}

// Quay và co giãn mọi đỉnh quanh một điểm: dùng cho phóng to chủ tướng, giật người khi trúng đòn, ngã xuống
void BienDoiDinh(sf::VertexArray& va, Vector2f tru, float tyLe, float goc, Vector2f dich, float doDuc) {
    float c = std::cos(goc), sn = std::sin(goc);
    for (size_t i = 0; i < va.getVertexCount(); ++i) {
        sf::Vertex& v = va[i];
        float dx = (v.position.x - tru.x) * tyLe;
        float dy = (v.position.y - tru.y) * tyLe;
        v.position = {tru.x + dx * c - dy * sn + dich.x, tru.y + dx * sn + dy * c + dich.y};
        v.color.a = static_cast<std::uint8_t>(v.color.a * doDuc);
    }
}
}

void KeDich::Ve(sf::RenderWindow& window) {
    if (IsDaBienMat()) return;

    mangDinh.clear();

    const float x = viTri.x;
    const float y = viTri.y;
    const float s = static_cast<float>(huongNhin);
    const float absS = 1.f;
    const float t = thoiGianHoatAnh;
    const bool boss = (loai == LoaiKeDich::TUONG_HOANG_THAO);
    const bool chet = daHySinh;
    const float tienDoChet = chet ? Kep01(thoiGianChet / TG_BIEN_MAT) : 0.f;
    const float flash = (thoiGianBiThuong > 0.f) ? 0.7f : (chet && tienDoChet < 0.12f ? 0.8f : 0.f);
    const bool khien = !boss && (bienThe & 1);
    const bool longVang = !boss && bienThe >= 2;

    std::function<Color(Color)> C = [&](Color col) -> Color { return sang(col, flash); };

    // Bóng đổ vẽ riêng để không bị xoay theo thân người
    {
        VA bong(sf::PrimitiveType::Triangles);
        float k = boss ? 1.35f : 1.f;
        elip(bong, x, y + 16.f * k, 15.f * k * (chet ? 1.f - 0.4f * tienDoChet : 1.f), 5.5f * k, mau(0, 0, 0, 80.f * (chet ? 1.f - tienDoChet : 1.f)), 12);
        window.draw(bong);
    }

    // ---- Bảng màu ----
    Color kSat = C(boss ? mau(48, 40, 48) : mau(72, 78, 92));
    Color kSatToi = C(boss ? mau(28, 22, 30) : mau(40, 44, 56));
    Color kSatSang = C(boss ? mau(110, 96, 110) : mau(132, 142, 160));
    Color kDo = C(mau(178, 34, 32));
    Color kDoToi = C(mau(110, 20, 22));
    Color kVang = C(mau(218, 165, 32));
    Color kVangSang = C(mau(255, 222, 96));
    Color kDa = C(mau(234, 192, 156));
    Color kDaToi = C(mau(200, 158, 124));
    Color kDa2 = C(mau(40, 30, 28));       // Giày, tóc, ria
    Color kQuan = C(boss ? mau(36, 28, 34) : mau(50, 44, 52));
    Color kVienChinh = boss ? kVang : kDo; // Màu viền chính: chủ tướng viền vàng, lính viền đỏ

    // ---- Thế đánh: lấy đà, vung, thu ----
    const bool layDa = dangLayDa && !chet;
    const float wLay = layDa ? Kep01(thoiGianLayDa / TG_LAY_DA) : 0.f;
    const bool vung = !chet && !layDa && thoiGianSauDon < TG_VUNG_CHEM;
    const bool thu = !chet && !layDa && thoiGianSauDon >= TG_VUNG_CHEM && thoiGianSauDon < TG_VUNG_CHEM + TG_THU_DAO;
    const bool dangDi = (trangThaiHienTai == TrangThaiKeDich::DUOI_THEO) && !chet;
    const float gocSan = (boss ? -0.95f : -0.55f) + std::sin(t * 3.f) * 0.04f;
    const float gocLayDa = boss ? -2.1f : -1.95f;
    const float gocChem = boss ? 0.7f : 0.85f;

    float gocKiem = gocSan;
    float chom = 0.f;
    float tienDoVung = 0.f;
    if (layDa) {
        gocKiem = Tron(gocSan, gocLayDa, DauRaMuot(wLay));
        chom = -2.6f * DauRaMuot(wLay);
    } else if (vung) {
        tienDoVung = thoiGianSauDon / TG_VUNG_CHEM;
        gocKiem = Tron(gocLayDa, gocChem, DauRaMuot(tienDoVung));
        chom = Tron(-2.6f, 5.f, DauRaMuot(tienDoVung));
    } else if (thu) {
        float q = (thoiGianSauDon - TG_VUNG_CHEM) / TG_THU_DAO;
        gocKiem = Tron(gocChem, gocSan, DauVaoRaMuot(q));
        chom = Tron(5.f, 0.f, DauVaoRaMuot(q));
    }

    const float xT = x + (chom + (dangDi ? 1.5f : 0.f)) * s;       // Thân trên chồm theo đòn đánh và khi chạy
    const float phaBuoc = std::sin(nhipBuocChan);
    float doNhun = dangDi ? std::abs(phaBuoc) * 2.5f : std::sin(t * 2.5f) * 0.7f;
    float yThan = y - 10.f - doNhun;
    float yDau = yThan - 11.f;

    // Vẽ cánh tay hai đoạn có khuỷu
    auto veTay = [&](Vector2f vai, Vector2f tay, float lech, Color ao) {
        Vector2f huong = tay - vai;
        float dai = std::max(1.f, std::sqrt(huong.x * huong.x + huong.y * huong.y));
        float dau = (s >= 0.f) ? 1.f : -1.f;
        Vector2f khuyu = (vai + tay) * 0.5f + Vector2f(-huong.y, huong.x) / dai * lech * dau;
        duong(mangDinh, vai, khuyu, 4.6f, ao);
        duong(mangDinh, khuyu, tay, 4.f, toi(ao, 0.85f));
        duong(mangDinh, tay - (tay - khuyu) * 0.3f, tay, 3.6f, kSat);       // Giáp bảo vệ cẳng tay
    };

    // 1. ÁO CHOÀNG CHỦ TƯỚNG (phía sau lưng)
    if (boss) {
        float gio = std::sin(t * 3.f + nhipBuocChan * 0.5f) * 4.f;
        float lui = (dangDi ? 9.f : 4.f) + std::max(0.f, -chom) * 1.5f + (vung ? 5.f : 0.f);
        Color kAo = C(mau(190, 26, 28));
        Color kAoToi = C(mau(104, 14, 18));
        tu(mangDinh, {xT - 8.f * s, yThan}, {xT + 3.f * s, yThan},
           {xT + 4.f * s + gio * 0.4f, yThan + 25.f}, {xT - (11.f + lui) * s + gio, yThan + 26.f}, kAoToi, kAo, kAo, kAoToi);
        duong(mangDinh, {xT - (11.f + lui) * s + gio, yThan + 26.f}, {xT + 4.f * s + gio * 0.4f, yThan + 25.f}, 1.8f, kVang);
    }

    // 2. TAY SAU: cầm khiên tròn (lính có khiên) hoặc buông tự nhiên
    {
        Vector2f vaiSau{xT - 6.f * s, yThan + 4.f};
        float lua = dangDi ? -phaBuoc * 4.f : std::sin(t * 2.5f + 1.f) * 0.7f;
        Vector2f tay{xT - (8.f + lua * 0.4f) * s, yThan + 12.f};
        veTay(vaiSau, tay, 1.6f, kDoToi);
        if (khien) {
            // Khiên tròn gỗ bọc da đỏ, viền sắt, núm giữa
            Vector2f tamKhien{tay.x - 2.f * s, tay.y - 1.f};
            elip(mangDinh, tamKhien.x, tamKhien.y, 8.f * 0.75f, 9.5f, kSatToi, 14);
            elip(mangDinh, tamKhien.x, tamKhien.y, 7.f * 0.75f, 8.5f, kDo, 14);
            elip(mangDinh, tamKhien.x, tamKhien.y, 3.2f * 0.75f, 3.6f, kSatSang, 10);
            elip(mangDinh, tamKhien.x, tamKhien.y, 1.4f, 1.6f, kVangSang, 6);
        } else {
            elip(mangDinh, tay.x, tay.y, 2.8f, 2.8f, kDaToi, 8);
        }
    }

    // 3. HAI CHÂN: quần tối, xà cạp đỏ, ủng da
    for (int lop = 0; lop < 2; ++lop) {
        float dau = (lop == 0) ? -1.f : 1.f;
        float buocPha = dangDi ? phaBuoc * dau : 0.f;
        float buocX = buocPha * 5.f + (vung || thu || layDa ? dau * chom * 0.8f : 0.f);
        float nhac = dangDi ? std::max(0.f, -buocPha) * 3.5f : 0.f;
        float legX = x + (dau * 4.5f + buocX) * s;
        float legY = y + 6.f - doNhun * 0.5f - nhac;
        Color giay = (lop == 0) ? kSatToi : kDa2;
        chuNhatDoc(mangDinh, legX - 2.8f, legY, 5.6f, 6.f, kQuan, toi(kQuan, 0.8f));
        chuNhat(mangDinh, legX - 3.f, legY + 4.6f, 6.f, 2.6f, kDo);                     // Xà cạp đỏ
        chuNhatDoc(mangDinh, legX - 2.7f, legY + 7.2f, 5.4f, 3.4f, giay, toi(giay, 0.7f));
        elip(mangDinh, legX + 2.f * s, legY + 10.6f, 5.f, 2.5f, giay, 8);
    }

    // 4. GIÁP TÀ: các dải giáp rũ xuống dưới đai lưng, đung đưa theo bước
    for (int k = -2; k <= 2; ++k) {
        float lac = std::sin(t * 4.f + static_cast<float>(k)) * 0.6f + (dangDi ? phaBuoc * 1.2f : 0.f);
        float bx = x + static_cast<float>(k) * 3.6f * s;
        tu(mangDinh, {bx - 1.8f, yThan + 15.f}, {bx + 1.8f, yThan + 15.f},
           {bx + 1.5f + lac * s, yThan + 22.f}, {bx - 1.5f + lac * s, yThan + 22.f},
           kSatSang, kSatSang, kSat, kSat);
        duong(mangDinh, {bx - 1.5f + lac * s, yThan + 22.f}, {bx + 1.5f + lac * s, yThan + 22.f}, 1.1f, kVienChinh);
    }

    // 5. THÂN: áo giáp vảy, giáp vai, đai lưng đỏ
    tu(mangDinh, {xT - 9.5f * s, yThan}, {xT + 9.5f * s, yThan}, {x + 10.5f * s, yThan + 17.f}, {x - 10.5f * s, yThan + 17.f},
       kSat, kSat, kSatToi, kSatToi);
    // Vảy giáp: ba hàng so le
    for (int hang = 0; hang < 3; ++hang) {
        for (int k = -3; k <= 3; ++k) {
            float off = (hang % 2 == 0) ? 0.f : 1.4f;
            float bx = xT + (static_cast<float>(k) * 2.7f + off) * s;
            float by = yThan + 3.f + static_cast<float>(hang) * 3.8f;
            float lech = (x - xT) * (by - yThan) / 17.f;
            Color mauVay = ((k + hang) % 2 == 0) ? kSatSang : kSat;
            elip(mangDinh, bx + lech, by, 1.7f, 1.7f, mauVay, 6);
        }
    }
    duong(mangDinh, {xT, yThan + 1.f}, {x, yThan + 15.f}, 1.1f, kSatToi);              // Sống áo giáp ở giữa
    // Giáp hộ tâm
    elip(mangDinh, xT + 1.f * s, yThan + 7.5f, boss ? 6.5f : 5.2f, boss ? 6.5f : 5.2f, boss ? kVang : kSatSang, 12);
    elip(mangDinh, xT + 1.5f * s, yThan + 7.f, boss ? 3.2f : 2.4f, boss ? 3.2f : 2.4f, boss ? kVangSang : kSat, 8);
    // Giáp vai
    float rongVai = boss ? 5.6f : 4.4f;
    elip(mangDinh, xT - 9.f * s, yThan + 2.f, rongVai, rongVai * 0.8f, boss ? kVang : kSatSang, 10);
    elip(mangDinh, xT + 9.f * s, yThan + 2.f, rongVai, rongVai * 0.8f, boss ? kVang : kSatSang, 10);
    elip(mangDinh, xT + 9.f * s, yThan + 1.f, rongVai * 0.5f, rongVai * 0.35f, boss ? kVangSang : C(mau(180, 190, 206)), 8);
    if (boss) {
        // Đầu thú nhô ra ở vai chủ tướng
        tam(mangDinh, {xT + 12.f * s, yThan - 1.f}, {xT + 17.f * s, yThan - 5.f}, {xT + 14.f * s, yThan + 3.f}, kVangSang);
        tam(mangDinh, {xT - 12.f * s, yThan - 1.f}, {xT - 17.f * s, yThan - 5.f}, {xT - 14.f * s, yThan + 3.f}, kVang);
    }
    // Đai lưng đỏ và khóa
    chuNhat(mangDinh, x - 10.f * s, yThan + 12.f, 20.f * s, 3.6f, kDo);
    chuNhat(mangDinh, x - 2.4f * s, yThan + 11.6f, 4.8f * s, 4.4f, boss ? kVangSang : kSatSang);

    // 6. ĐẦU: mặt, mày, mắt, ria, mũ sắt chóp nhọn
    chuNhat(mangDinh, xT - 3.f * s, yThan - 3.f, 6.f * s, 4.f, kDaToi);                    // Cổ
    elip(mangDinh, xT - 6.5f * s, yDau + 1.f, 2.2f, 2.8f, kDaToi, 8);                      // Tai
    elip(mangDinh, xT + 1.f * s, yDau, 8.4f, 9.4f, kDa, 14);                               // Mặt

    float matX = xT + 4.5f * s;
    if (boss) {
        elip(mangDinh, matX, yDau - 1.f, 2.2f, 2.3f, C(mau(255, 70, 40)), 6);               // Mắt rực lửa
        elip(mangDinh, matX, yDau - 1.f, 1.1f, 1.2f, mau(30, 8, 8), 6);
    } else {
        elip(mangDinh, matX, yDau - 1.f, 1.7f, 1.9f, mau(22, 20, 20), 6);
        elip(mangDinh, matX + 0.6f * s, yDau - 1.8f, 0.6f, 0.6f, mau(255, 255, 255), 5);
    }
    // Lông mày dựng ngược trông hung dữ (dữ hơn khi lấy đà)
    float domMay = layDa ? 2.4f : 1.2f;
    duong(mangDinh, {matX - 3.f * s, yDau - 3.2f - domMay * 0.3f}, {matX + 3.f * s, yDau - 4.6f - domMay}, 1.8f, kDa2);
    // Ria mép và miệng
    duong(mangDinh, {matX - 1.2f * s, yDau + 3.8f}, {matX + 3.4f * s, yDau + 3.4f}, 1.4f, kDa2);
    if (layDa || vung) elip(mangDinh, matX + 1.2f * s, yDau + 5.8f, 2.2f, 1.6f, mau(120, 30, 30), 6);   // Hét to khi chém
    else duong(mangDinh, {matX - 0.4f * s, yDau + 5.6f}, {matX + 2.4f * s, yDau + 5.6f}, 1.0f, mau(130, 60, 54));
    if (boss) {
        // Râu dài của chủ tướng
        tam(mangDinh, {xT - 3.f * s, yDau + 5.f}, {xT + 5.f * s, yDau + 5.f}, {xT + 1.f * s, yDau + 16.f}, kDa2);
    }

    // Rèm giáp che gáy phía sau mũ
    tu(mangDinh, {xT - 9.f * s, yDau - 4.f}, {xT - 3.f * s, yDau - 4.f},
       {xT - 4.f * s, yDau + 6.f}, {xT - 12.f * s, yDau + 5.f}, kSat, kSat, kSatToi, kSatToi);
    duong(mangDinh, {xT - 12.f * s, yDau + 5.f}, {xT - 4.f * s, yDau + 6.f}, 1.6f, kVienChinh);

    // Mũ sắt: chóp nhọn, vành đỏ, gờ giữa, quai cằm
    tam(mangDinh, {xT - 10.f * s, yDau - 3.5f}, {xT + 10.f * s, yDau - 3.5f}, {xT, yDau - (boss ? 18.f : 15.f)}, kSat);
    tam(mangDinh, {xT + 1.f * s, yDau - 3.5f}, {xT + 10.f * s, yDau - 3.5f}, {xT, yDau - (boss ? 18.f : 15.f)}, kSatToi);
    duong(mangDinh, {xT, yDau - 3.5f}, {xT, yDau - (boss ? 17.f : 14.f)}, 1.1f, kSatSang);
    chuNhat(mangDinh, xT - 10.f * s, yDau - 5.2f, 20.f * s, 2.8f, kVienChinh);
    duong(mangDinh, {xT + 5.f * s, yDau + 3.f}, {xT + 3.f * s, yDau + 8.5f}, 0.9f, kDa2);
    // Lông trên đỉnh mũ phất phơ
    {
        float lac = std::sin(t * 6.f) * 2.f - (dangDi ? 3.f : 0.f) * s;
        Vector2f dinh{xT, yDau - (boss ? 18.f : 15.f)};
        Color mauLong = boss ? C(mau(220, 30, 30)) : (longVang ? kVang : kDo);
        duong(mangDinh, dinh, dinh + Vector2f(-3.f * s + lac, -(boss ? 8.f : 5.f)), 2.4f, mauLong);
        elip(mangDinh, dinh.x - 3.5f * s + lac, dinh.y - (boss ? 9.f : 6.f), 2.4f, 3.f, mauLong, 8);
        if (boss) {
            // Hai cánh phượng hai bên mũ chủ tướng
            tam(mangDinh, {xT - 10.f * s, yDau - 6.f}, {xT - 19.f * s, yDau - 12.f}, {xT - 11.f * s, yDau - 1.f}, kVang);
            tam(mangDinh, {xT + 10.f * s, yDau - 6.f}, {xT + 19.f * s, yDau - 12.f}, {xT + 11.f * s, yDau - 1.f}, kVangSang);
        }
    }

    // 7. CÁNH TAY CẦM VŨ KHÍ + VẾT CHÉM
    Vector2f vai{xT + 6.f * s, yThan + 4.f};
    const float banKinhTay = 9.f;
    auto viTriTay = [&](float goc) { return Vector2f(vai.x + std::cos(goc) * banKinhTay * s, vai.y + std::sin(goc) * banKinhTay); };
    Vector2f tay = viTriTay(gocKiem);

    // Vệt chém đỏ trong lúc vung và thu
    if (vung || (thu && thoiGianSauDon < TG_VUNG_CHEM + 0.12f)) {
        float doDuc = vung ? 1.f : 1.f - (thoiGianSauDon - TG_VUNG_CHEM) / 0.12f;
        float duoi = vung ? gocLayDa : Tron(gocLayDa, gocChem, 0.8f * (1.f - doDuc));
        float dau = vung ? gocKiem : gocChem;
        if (std::abs(dau - duoi) > 0.05f) VeVetDich(mangDinh, vai, s, duoi, dau, boss ? 78.f : 56.f, doDuc, boss);
    }

    veTay(vai, tay, 2.2f, kDo);
    if (boss) VeQuanDao(mangDinh, tay, gocKiem, s, C);
    else VeDaoCong(mangDinh, tay, gocKiem, s, 25.f, C);
    elip(mangDinh, tay.x, tay.y, 3.1f, 3.1f, kDa, 8);

    // Dấu hiệu báo trước: lưỡi đao lóe sáng khi đang lấy đà, chủ tướng còn có quầng đỏ
    if (layDa) {
        float nhay = 0.5f + 0.5f * std::sin(thoiGianLayDa * 40.f);
        Vector2f mui = tay + Vector2f(std::cos(gocKiem) * s, std::sin(gocKiem)) * (boss ? 56.f : 30.f);
        elip(mangDinh, mui.x, mui.y, 4.5f, 4.5f, mau(255, 255, 255, 120.f + 120.f * nhay), 8);
        if (boss) elip(mangDinh, xT, yThan + 8.f, 26.f * wLay, 30.f * wLay, mau(255, 60, 30, 46.f), 18);
    }

    // ---- Biến đổi cả hình: phóng to chủ tướng, giật người khi trúng đòn, ngã xuống khi chết ----
    Vector2f tru{x, y + 16.f};
    float tyLe = boss ? 1.35f : 1.f;
    float gocXoay = 0.f;
    Vector2f dich{0.f, 0.f};
    float doDucNhan = 1.f;
    if (thoiGianBiThuong > 0.f && !chet) {
        float r = thoiGianBiThuong / 0.18f;
        gocXoay = -s * 0.16f * r;           // Giật ngửa ra sau
        dich.x = -s * 2.5f * r;
    }
    if (chet) {
        float e = DauVaoRaMuot(tienDoChet * 1.15f);
        gocXoay = -s * 1.5f * e;            // Ngã ngửa về phía sau
        dich.y = 5.f * e;
        dich.x = -s * 6.f * e;
        doDucNhan = (tienDoChet < 0.55f) ? 1.f : 1.f - (tienDoChet - 0.55f) / 0.45f;
    }
    BienDoiDinh(mangDinh, tru, tyLe, gocXoay, dich, doDucNhan);

    // THANH MÁU MINI TRÊN ĐẦU (không xoay, không vẽ khi đã chết)
    if (!chet) {
        float rongThanh = boss ? 52.f : 34.f;
        float caoThanh = boss ? 6.f : 4.5f;
        float yThanhMau = boss ? y - 68.f : y - 49.f;      // Nằm phía trên chóp mũ và lông mũ
        float tiLeMau = std::clamp(static_cast<float>(sucKhoe) / static_cast<float>(sucKhoeToiDa), 0.0f, 1.0f);

        chuNhat(mangDinh, x - rongThanh * 0.5f - 1.5f, yThanhMau - 1.5f, rongThanh + 3.f, caoThanh + 3.f,
                boss ? mau(218, 165, 32, 240) : mau(30, 30, 36, 220));
        chuNhat(mangDinh, x - rongThanh * 0.5f, yThanhMau, rongThanh, caoThanh, mau(45, 20, 20, 200));
        if (tiLeMau > 0.01f) {
            chuNhat(mangDinh, x - rongThanh * 0.5f, yThanhMau, rongThanh * tiLeMau, caoThanh,
                    boss ? mau(255, 120, 20) : mau(220, 25, 35));
            chuNhat(mangDinh, x - rongThanh * 0.5f, yThanhMau, rongThanh * tiLeMau, 1.5f, mau(255, 200, 160));
        }
    }

    (void)absS;
    window.draw(mangDinh);
}
