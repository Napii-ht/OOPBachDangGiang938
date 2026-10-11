
#include "NguoiChoi.h"
#include "../BanDo/DoHoa.h"
#include <cmath>
#include <algorithm>

using namespace dohoa;

namespace {
const float TONG_THOI_GIAN_CHEM = 0.27f;    // Một nhát chém: lấy đà, chém, thu kiếm
const float THOI_GIAN_SAN_SANG = 1.0f;      // Sau nhát chém giữ kiếm trên tay thêm một lúc
const float PHAN_LAY_DA = 0.26f;            // Tỉ lệ thời gian của giai đoạn lấy đà
const float PHAN_CHEM_XONG = 0.60f;         // Hết giai đoạn chém nhanh

float Kep01(float p) { return std::max(0.f, std::min(1.f, p)); }
float Tron(float a, float b, float t) { return a + (b - a) * t; }
float DauRaMuot(float p) { p = Kep01(p); float q = 1.f - p; return 1.f - q * q * q; }       // Nhanh rồi chậm dần
float DauVaoRaMuot(float p) { p = Kep01(p); return p * p * (3.f - 2.f * p); }               // Mượt hai đầu

// Góc của lưỡi kiếm (radian, 0 là hướng thẳng ra trước, dương là chúi xuống) theo tiến độ đòn
void GocChem(int kieu, float u, float& gocHienTai, float& gocDau, float& gocCuoi, float gocSanSang) {
    gocDau = (kieu == 0) ? -2.15f : 1.25f;     // Điểm bắt đầu vung: kiếm giơ cao sau lưng, hoặc hạ thấp
    gocCuoi = (kieu == 0) ? 0.65f : -1.35f;    // Điểm kết thúc: chém tới trước hoặc hất lên cao
    if (u < PHAN_LAY_DA) {
        gocHienTai = Tron(gocSanSang, gocDau, DauRaMuot(u / PHAN_LAY_DA));
    } else if (u < PHAN_CHEM_XONG) {
        float p = (u - PHAN_LAY_DA) / (PHAN_CHEM_XONG - PHAN_LAY_DA);
        gocHienTai = Tron(gocDau, gocCuoi, DauRaMuot(p));
    } else {
        float q = (u - PHAN_CHEM_XONG) / (1.f - PHAN_CHEM_XONG);
        gocHienTai = Tron(gocCuoi, gocSanSang, DauVaoRaMuot(q));
    }
}

// Vẽ một thanh kiếm: tua đỏ, chuôi quấn dây, tay hộ vàng, lưỡi thép có sống giữa.
// tay là chỗ bàn tay cầm, goc là hướng lưỡi (đã tính cả lật trái phải qua s).
void VeThanhKiem(VA& va, Vector2f tay, float goc, float s, float dai, float doDuc, float t) {
    Vector2f d(std::cos(goc) * s, std::sin(goc));
    Vector2f n(-d.y, d.x);
    auto c = [&](Color m) { m.a = static_cast<std::uint8_t>(m.a * doDuc); return m; };

    // Tua đỏ buông từ đầu chuôi, lay động
    Vector2f dauChuoi = tay - d * 3.f;
    float lac = std::sin(t * 7.f) * 2.f;
    Vector2f giuaTua = dauChuoi - d * 5.f + n * lac;
    Vector2f cuoiTua = dauChuoi - d * 10.f + n * (lac * 1.7f);
    duong(va, dauChuoi, giuaTua, 1.4f, c(mau(190, 36, 32)));
    tam(va, giuaTua, cuoiTua + n * 1.6f, cuoiTua - n * 1.6f, c(mau(224, 54, 42)));

    // Chuôi quấn dây đỏ sẫm, có vòng vàng
    duong(va, tay - d * 2.5f, tay + d * 5.5f, 3.f, c(mau(92, 26, 22)));
    for (int k = 0; k < 3; ++k) {
        Vector2f p = tay + d * (0.4f + static_cast<float>(k) * 2.2f);
        duong(va, p - n * 1.6f, p + n * 1.6f, 1.1f, c(mau(222, 170, 62)));
    }
    Vector2f dau = tay - d * 3.2f;
    elip(va, dau.x, dau.y, 2.4f, 2.4f, c(mau(236, 184, 70)), 8);

    // Tay hộ vàng
    Vector2f g = tay + d * 5.8f;
    duong(va, g - n * 5.f, g + n * 5.f, 2.6f, c(mau(222, 170, 62)));
    elip(va, (g - n * 5.f).x, (g - n * 5.f).y, 1.8f, 1.8f, c(mau(252, 218, 112)), 6);
    elip(va, (g + n * 5.f).x, (g + n * 5.f).y, 1.8f, 1.8f, c(mau(252, 218, 112)), 6);

    // Lưỡi: viền tối, nửa sáng, nửa tối, sống giữa
    Vector2f g0 = g + d * 1.2f;
    Vector2f giua = g0 + d * (dai * 0.72f);
    Vector2f mui = g0 + d * dai;
    tu(va, g0 + n * 2.9f, g0 - n * 2.9f, giua - n * 2.0f, giua + n * 2.0f, c(mau(54, 64, 84)), c(mau(54, 64, 84)), c(mau(54, 64, 84)), c(mau(54, 64, 84)));
    tam(va, giua + n * 2.0f, giua - n * 2.0f, mui + d * 1.3f, c(mau(54, 64, 84)));
    tu(va, g0, g0 + n * 2.3f, giua + n * 1.5f, giua, c(mau(250, 253, 255)), c(mau(206, 218, 232)), c(mau(206, 218, 232)), c(mau(250, 253, 255)));
    tam(va, giua, giua + n * 1.5f, mui, c(mau(226, 235, 246)));
    tu(va, g0, giua, giua - n * 1.5f, g0 - n * 2.3f, c(mau(176, 190, 212)), c(mau(176, 190, 212)), c(mau(120, 136, 162)), c(mau(120, 136, 162)));
    tam(va, giua, mui, giua - n * 1.5f, c(mau(142, 156, 180)));
    duong(va, g0 + d * 1.5f, mui - d * 2.f, 0.9f, c(mau(255, 255, 255, 210)));
}

// Vệt chém hình lưỡi liềm: đầu sáng ở gocCuoi, đuôi mảnh và mờ dần về gocDau
void VeVetChem(VA& va, Vector2f tam0, float s, float gocDau, float gocCuoi, float banKinh, float doDuc) {
    const int seg = 18;
    auto vt = [&](float a, float r) { return Vector2f(tam0.x + std::cos(a) * r * s, tam0.y + std::sin(a) * r * 0.92f); };
    for (int i = 0; i < seg; ++i) {
        float t0 = static_cast<float>(i) / seg;
        float t1 = static_cast<float>(i + 1) / seg;
        float a0 = Tron(gocDau, gocCuoi, t0);
        float a1 = Tron(gocDau, gocCuoi, t1);
        float rTrong0 = Tron(banKinh * 0.97f, banKinh * 0.50f, t0);
        float rTrong1 = Tron(banKinh * 0.97f, banKinh * 0.50f, t1);
        float k0 = std::pow(t0, 1.6f) * doDuc;
        float k1 = std::pow(t1, 1.6f) * doDuc;
        Color ngoai0 = mau(255, 214, 110, 255.f * k0);
        Color ngoai1 = mau(255, 255, 235, 255.f * k1);
        Color trong0 = mau(255, 196, 80, 150.f * k0);
        Color trong1 = mau(255, 230, 150, 150.f * k1);
        tu(va, vt(a0, banKinh), vt(a1, banKinh), vt(a1, rTrong1), vt(a0, rTrong0), ngoai0, ngoai1, trong1, trong0);
        // Đường viền sáng mảnh ở mép ngoài cho vệt chém có nét
        duong(va, vt(a0, banKinh), vt(a1, banKinh), 1.5f, mau(255, 255, 240, 230.f * k1));
    }
}
}

NguoiChoi::NguoiChoi(sf::Vector2f viTriBanDau)
    : NhanVat(viTriBanDau, 160.f, 150, 40),
      theLuc(100.f),
      theLucToiDa(100.f),
      tocDoChay(250.f),
      dangChayNhanh(false),
      phamViTanCong(55.f),
      dangTanCong(false),
      thoiGianTanCong(0.f),
      kieuChem(0),
      thoiGianSanSang(0.f),
      huongNhin(1),
      thoiGianHoatAnh(0.f),
      nhipBuocChan(0.f),
      dangDiChuyen(false),
      heSoLat(1.f),
      coGioiHan(false),
      gioiHan({0.f, 0.f}, {0.f, 0.f}),
      danhSachVatCan(nullptr),
      coKiem(true),
      tuDo(6) {}

void NguoiChoi::Chay(bool kichHoat) {
    if (kichHoat && theLuc > 5.f) {
        dangChayNhanh = true;
    } else {
        dangChayNhanh = false;
    }
}

bool NguoiChoi::NhanVatPham(const VatPham& vp) {
    return tuDo.ThemVatPham(vp);
}

void NguoiChoi::XuLyPhim(float deltaTime) {
    if (daHySinh) return;

    sf::Vector2f huong(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        huong.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        huong.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        huong.x -= 1.f;
        huongNhin = -1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        huong.x += 1.f;
        huongNhin = 1;
    }

    // Chạy nhanh bằng nút LShift
    bool shiftNhan = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift);
    Chay(shiftNhan);

    // Chuẩn hóa vector di chuyển
    float doDai = std::sqrt(huong.x * huong.x + huong.y * huong.y);
    if (doDai > 0.0001f) {
        dangDiChuyen = true;
        huong.x /= doDai;
        huong.y /= doDai;

        float vanTocHienTai = dangChayNhanh ? tocDoChay : tocDo;
        sf::Vector2f buoc = huong * vanTocHienTai * deltaTime;

        // Đi từng trục một để khi chạm cây hoặc lều thì trượt dọc theo vật cản
        if (!KiemTraVaCham({viTri.x + buoc.x, viTri.y})) viTri.x += buoc.x;
        if (!KiemTraVaCham({viTri.x, viTri.y + buoc.y})) viTri.y += buoc.y;

        if (coGioiHan) {
            viTri.x = std::clamp(viTri.x, gioiHan.position.x, gioiHan.position.x + gioiHan.size.x);
            viTri.y = std::clamp(viTri.y, gioiHan.position.y, gioiHan.position.y + gioiHan.size.y);
        }

        if (dangChayNhanh) {
            theLuc -= 18.f * deltaTime;
            if (theLuc <= 0.f) {
                theLuc = 0.f;
                dangChayNhanh = false;
            }
        }
    } else {
        dangDiChuyen = false;
    }

    // Phím chuyển ô hotbar (1 -> 6)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) tuDo.SetODangChon(0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) tuDo.SetODangChon(1);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) tuDo.SetODangChon(2);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) tuDo.SetODangChon(3);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5)) tuDo.SetODangChon(4);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6)) tuDo.SetODangChon(5);

    // Tấn công bằng phím Space hoặc J
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J)) {
        TanCong();
    }
}

bool NguoiChoi::TanCong() {
    if (daHySinh || dangTanCong || !coKiem) return false;

    // Chém liên tiếp thì đổi kiểu chém (xuống rồi lên) cho đòn đánh có nhịp
    kieuChem = (thoiGianSanSang > 0.f) ? 1 - kieuChem : 0;
    thoiGianSanSang = THOI_GIAN_SAN_SANG;

    dangTanCong = true;
    thoiGianTanCong = TONG_THOI_GIAN_CHEM;
    return true;
}

sf::FloatRect NguoiChoi::GetBanChan(sf::Vector2f viTriXet) const {
    return sf::FloatRect({viTriXet.x - 11.f, viTriXet.y + 6.f}, {22.f, 12.f});
}

bool NguoiChoi::KiemTraVaCham(sf::Vector2f viTriXet) const {
    if (!danhSachVatCan) return false;
    sf::FloatRect chan = GetBanChan(viTriXet);
    for (const auto& vatCan : *danhSachVatCan) {
        if (chan.findIntersection(vatCan)) return true;
    }
    return false;
}

void NguoiChoi::HoiMau(int luong) {
    if (daHySinh) return;
    sucKhoe = std::min(sucKhoeToiDa, sucKhoe + luong);
}

sf::FloatRect NguoiChoi::GetHitBox() const {
    return sf::FloatRect({viTri.x - 14.f, viTri.y - 18.f}, {28.f, 36.f});
}

sf::FloatRect NguoiChoi::GetVungTanCong() const {
    // Đang lấy đà hoặc đang thu kiếm thì chưa trúng ai: vùng chém rỗng. Chỉ trúng đúng lúc lưỡi kiếm vung qua.
    if (dangTanCong && coKiem) {
        float u = 1.f - thoiGianTanCong / TONG_THOI_GIAN_CHEM;
        if (u < PHAN_LAY_DA - 0.02f || u > PHAN_CHEM_XONG + 0.08f) {
            return sf::FloatRect({viTri.x, viTri.y}, {0.f, 0.f});
        }
    }
    float tam = phamViTanCong * 1.2f;
    float xKiem = (huongNhin == 1) ? viTri.x : (viTri.x - tam);
    return sf::FloatRect({xKiem, viTri.y - 32.f}, {tam, 64.f});
}

void NguoiChoi::CapNhat(float deltaTime) {
    if (daHySinh) return;

    thoiGianHoatAnh += deltaTime;

    // Lật mượt hệ số hướng nhìn (-1 .. 1)
    float mucTieuLat = static_cast<float>(huongNhin);
    heSoLat += (mucTieuLat - heSoLat) * std::min(1.f, deltaTime * 16.f);

    // Hoạt ảnh bước chân khi di chuyển
    if (dangDiChuyen) {
        float tocDoBuoc = dangChayNhanh ? 18.f : 11.f;
        nhipBuocChan += deltaTime * tocDoBuoc;
    } else {
        nhipBuocChan *= 0.85f;
    }

    // Tự động hồi phục thể lực khi không chạy
    if (!dangChayNhanh && theLuc < theLucToiDa) {
        theLuc += 15.f * deltaTime;
        if (theLuc > theLucToiDa) theLuc = theLucToiDa;
    }

    if (thoiGianSanSang > 0.f && !dangTanCong) thoiGianSanSang -= deltaTime;

    // Cập nhật đòn đánh
    if (dangTanCong) {
        thoiGianTanCong -= deltaTime;
        if (thoiGianTanCong <= 0.f) {
            dangTanCong = false;
        }
    }
}

void NguoiChoi::Ve(sf::RenderWindow& window) {
    if (daHySinh) return;

    mangDinh.clear();

    const float x = viTri.x;
    const float y = viTri.y;
    const float s = heSoLat; // Hệ số co giãn ngang để lật mặt nhân vật
    const float absS = std::max(0.15f, std::abs(s));
    const float t = thoiGianHoatAnh;

    // 1. BÓNG ĐỔ DƯỚI CHÂN
    elip(mangDinh, x, y + 16.f, 15.f, 5.5f, mau(0, 0, 0, 85.f), 12);

    // ---- Tiến độ nhát chém và độ chồm người theo nhát chém ----
    const bool chem = dangTanCong && coKiem;
    const bool camKiem = coKiem && (chem || thoiGianSanSang > 0.f);    // Kiếm đang cầm trên tay (chưa tra vào vỏ)
    const float u = chem ? Kep01(1.f - thoiGianTanCong / TONG_THOI_GIAN_CHEM) : -1.f;
    float chom = 0.f;
    if (chem) {
        if (u < PHAN_LAY_DA) chom = Tron(0.f, -2.4f, DauRaMuot(u / PHAN_LAY_DA));
        else if (u < PHAN_CHEM_XONG) chom = Tron(-2.4f, 4.8f, DauRaMuot((u - PHAN_LAY_DA) / (PHAN_CHEM_XONG - PHAN_LAY_DA)));
        else chom = Tron(4.8f, 0.f, DauVaoRaMuot((u - PHAN_CHEM_XONG) / (1.f - PHAN_CHEM_XONG)));
    }
    const float xT = x + chom * s;                                     // Thân trên nghiêng theo nhát chém, chân giữ nguyên
    const float phaBuoc = std::sin(nhipBuocChan);

    // Điểm nhún người tự nhiên khi bước
    float doNhun = dangDiChuyen ? std::abs(phaBuoc) * 2.5f : std::sin(t * 3.f) * 0.8f;
    float yThan = y - 10.f - doNhun;
    float yDau = yThan - 11.f;

    Color kChanToi = mau(60, 42, 28);
    Color kChanSang = mau(92, 64, 42);
    Color kQuanLam = mau(26, 45, 80);
    Color kAoLam = mau(32, 68, 136);
    Color kAoLamToi = mau(20, 45, 96);
    Color kGiapDong = mau(212, 160, 54);
    Color kGiapSang = mau(255, 220, 110);
    Color kThatLung = mau(120, 36, 28);
    Color kDa = mau(255, 218, 185);
    Color kDaBong = mau(225, 185, 150);
    Color kToc = mau(30, 24, 20);
    Color kKhanDo = mau(205, 36, 36);
    Color kKhanSang = mau(245, 75, 60);

    // Vẽ một cánh tay hai đoạn có khuỷu, từ vai tới bàn tay
    auto veTay = [&](Vector2f vai, Vector2f tay, float lechKhuyu, Color ao) {
        Vector2f huong = tay - vai;
        float dai = std::max(1.f, std::sqrt(huong.x * huong.x + huong.y * huong.y));
        float dau = (s >= 0.f) ? 1.f : -1.f;
        Vector2f khuyu = (vai + tay) * 0.5f + Vector2f(-huong.y, huong.x) / dai * lechKhuyu * dau;
        duong(mangDinh, vai, khuyu, 4.4f, ao);
        duong(mangDinh, khuyu, tay, 3.8f, toi(ao, 0.85f));
        duong(mangDinh, tay - (tay - khuyu) * 0.25f, tay, 3.4f, kGiapDong);   // Bao cổ tay đồng
    };

    // 2. ÁO CHOÀNG ĐỎ PHÍA SAU LƯNG (bay theo bước chạy và nhát chém)
    {
        float lui = (dangDiChuyen ? 8.f : 2.5f) + std::max(0.f, -chom) * 1.2f;
        if (chem && u >= PHAN_LAY_DA && u < PHAN_CHEM_XONG) lui += 4.f;
        float ve = std::sin(t * 5.f + nhipBuocChan) * 2.5f;
        Vector2f vai0{xT - 7.f * s, yThan + 1.f};
        Vector2f vai1{xT + 2.f * s, yThan + 1.f};
        Vector2f gau1{xT + 3.f * s + ve * 0.5f, yThan + 23.f};
        Vector2f gau0{xT - (10.f + lui) * s + ve, yThan + 24.f};
        tu(mangDinh, vai0, vai1, gau1, gau0, mau(112, 26, 24), mau(130, 32, 28), mau(176, 44, 38), mau(196, 52, 42));
        duong(mangDinh, gau0, gau1, 1.6f, kGiapDong);
    }

    // 3. TAY SAU (tay không cầm kiếm), nằm sau thân
    {
        Vector2f vaiSau{xT - 6.f * s, yThan + 4.f};
        float luaTay = dangDiChuyen ? -phaBuoc * 5.f : std::sin(t * 3.f + 1.f) * 0.8f;
        Vector2f tay{xT - (8.f + luaTay * 0.45f) * s, yThan + 13.f};
        if (chem) tay = {xT - 11.f * s, yThan + 9.f};          // Giơ ra sau giữ thăng bằng khi chém
        duong(mangDinh, vaiSau, tay, 4.2f, kAoLamToi);
        elip(mangDinh, tay.x, tay.y, 2.7f * absS, 2.7f, kDaBong, 8);
    }

    // Vỏ kiếm nằm sau thân: chỉ lộ phần đầu vỏ thò ra phía sau lưng
    if (coKiem && !camKiem) {
        float lacVo = dangDiChuyen ? phaBuoc * 2.5f : 0.f;
        Vector2f g{xT + 6.f * s, yThan + 13.f};
        Vector2f dauVo{xT - (17.f + lacVo * 0.4f) * s, yThan + 27.f + lacVo * 0.3f};
        Vector2f huongVo = dauVo - g;
        float daiVo = std::max(1.f, std::sqrt(huongVo.x * huongVo.x + huongVo.y * huongVo.y));
        huongVo /= daiVo;
        duong(mangDinh, g, dauVo, 4.6f, mau(46, 30, 24));
        duong(mangDinh, g + huongVo, dauVo - huongVo, 2.2f, mau(96, 62, 42));
        duong(mangDinh, dauVo - huongVo * 5.f, dauVo, 5.f, kGiapDong);
        elip(mangDinh, dauVo.x, dauVo.y, 2.4f, 2.4f, kGiapSang, 8);
    }

    // 4. HAI CHÂN: ống quần, giáp ống chân bằng đồng, ủng da
    for (int lop = 0; lop < 2; ++lop) {
        float dau = (lop == 0) ? -1.f : 1.f;                    // 0 = chân sau, 1 = chân trước
        float buocPha = dangDiChuyen ? phaBuoc * dau : 0.f;
        float buocX = buocPha * 5.f + (chem ? dau * chom * 0.9f : 0.f);
        float nhac = dangDiChuyen ? std::max(0.f, -buocPha) * 3.5f : 0.f;   // Chân đang bước tới thì nhấc lên
        float legX = x + (dau * 4.5f + buocX) * s;
        float legY = y + 6.f - doNhun * 0.5f - nhac;
        Color giay = (lop == 0) ? kChanToi : kChanSang;
        chuNhatDoc(mangDinh, legX - 2.7f, legY, 5.4f, 6.f, kQuanLam, toi(kQuanLam, 0.8f));
        chuNhat(mangDinh, legX - 2.9f, legY + 5.f, 5.8f, 2.4f, toi(kGiapDong, lop == 0 ? 0.75f : 1.f));
        chuNhatDoc(mangDinh, legX - 2.6f, legY + 7.4f, 5.2f, 3.2f, giay, toi(giay, 0.7f));
        elip(mangDinh, legX + 2.f * s, legY + 10.6f, 4.8f * absS, 2.4f, giay, 8);
    }

    // 5. THÂN MÌNH: áo lam, cổ áo, giáp vai và giáp ngực bằng đồng, đai lưng
    tu(mangDinh,
       {xT - 9.f * s, yThan}, {xT + 9.f * s, yThan},
       {x + 11.f * s, yThan + 18.f}, {x - 11.f * s, yThan + 18.f},
       kAoLam, kAoLam, kAoLamToi, kAoLamToi);
    tam(mangDinh, {xT - 4.f * s, yThan}, {xT + 4.f * s, yThan}, {xT + 0.5f * s, yThan + 6.5f}, mau(238, 226, 200));

    // Giáp hộ tâm bằng đồng
    elip(mangDinh, xT + 1.f * s, yThan + 8.f, 6.5f * absS, 6.5f, kGiapDong, 12);
    elip(mangDinh, xT + 2.f * s, yThan + 7.f, 3.5f * absS, 3.5f, kGiapSang, 8);
    // Giáp vai
    elip(mangDinh, xT - 8.5f * s, yThan + 2.f, 4.4f * absS, 3.6f, toi(kGiapDong, 0.85f), 10);
    elip(mangDinh, xT + 8.5f * s, yThan + 2.f, 4.8f * absS, 3.8f, kGiapDong, 10);
    elip(mangDinh, xT + 9.f * s, yThan + 1.2f, 2.4f * absS, 1.6f, kGiapSang, 8);

    // Thắt lưng da và đai vải đỏ
    chuNhat(mangDinh, x - 10.f * s, yThan + 14.f, 20.f * s, 3.5f, kThatLung);
    chuNhat(mangDinh, x - 2.5f * s, yThan + 13.5f, 5.f * s, 4.5f, kGiapDong);
    float vatX = x + 3.f * s;
    float vatLac = std::sin(t * 5.f + nhipBuocChan) * 3.f * s;
    tu(mangDinh,
       {vatX - 2.f, yThan + 17.f}, {vatX + 2.f, yThan + 17.f},
       {vatX + 3.f + vatLac, yThan + 26.f}, {vatX - 1.f + vatLac, yThan + 26.f},
       kThatLung, kThatLung, mau(160, 45, 35), mau(160, 45, 35));

    // Chuôi kiếm nhô lên ở hông phía trước khi kiếm đã tra vào vỏ (phần vỏ vẽ phía sau thân, xem bên trên)
    Vector2f chuoiTraVao{xT + 10.f * s, yThan + 6.5f};
    if (coKiem && !camKiem) {
        Vector2f g{xT + 6.f * s, yThan + 13.f};
        duong(mangDinh, g, chuoiTraVao, 3.2f, mau(92, 26, 22));
        duong(mangDinh, {g.x - 2.6f * s, g.y + 1.2f}, {g.x + 2.6f * s, g.y - 1.2f}, 2.6f, kGiapDong);
        elip(mangDinh, chuoiTraVao.x, chuoiTraVao.y, 2.2f, 2.2f, kGiapSang, 8);
        duong(mangDinh, chuoiTraVao, {chuoiTraVao.x + 1.5f * s, chuoiTraVao.y + 6.f}, 1.4f, kKhanDo);
    }

    // 6. ĐẦU & KHĂN QUẤN ĐẠI VIỆT
    chuNhat(mangDinh, xT - 3.f * s, yThan - 3.f, 6.f * s, 4.f, kDaBong);                 // Cổ
    elip(mangDinh, xT - 6.5f * s, yDau + 1.f, 2.2f * absS, 2.8f, kDaBong, 8);            // Tai
    elip(mangDinh, xT + 1.f * s, yDau, 8.5f * absS, 9.5f, kDa, 14);                       // Mặt
    elip(mangDinh, xT + 5.5f * s, yDau + 3.f, 2.4f * absS, 1.5f, mau(240, 160, 140, 110), 8);   // Má hồng

    float matX = xT + 4.5f * s;
    elip(mangDinh, matX, yDau - 1.f, 1.7f, 1.9f, mau(20, 20, 20), 6);
    elip(mangDinh, matX + 0.6f * s, yDau - 1.8f, 0.7f, 0.7f, mau(255, 255, 255), 5);       // Ánh mắt
    duong(mangDinh, {matX - 2.6f * s, yDau - 4.2f}, {matX + 2.8f * s, yDau - 3.4f}, 1.5f, kToc);   // Lông mày
    duong(mangDinh, {matX - 0.4f * s, yDau + 4.2f}, {matX + 2.2f * s, yDau + 4.4f}, 1.0f, mau(150, 70, 60));  // Miệng

    // Khăn đỏ quấn đầu
    tu(mangDinh,
       {xT - 9.f * s, yDau - 7.f}, {xT + 9.f * s, yDau - 7.f},
       {xT + 9.5f * s, yDau - 2.f}, {xT - 9.5f * s, yDau - 2.f},
       kKhanSang, kKhanSang, kKhanDo, kKhanDo);
    duong(mangDinh, {xT - 9.f * s, yDau - 2.2f}, {xT + 9.5f * s, yDau - 2.2f}, 1.2f, toi(kKhanDo, 0.7f));
    elip(mangDinh, xT - 7.5f * s, yDau - 5.f, 4.f * absS, 4.5f, kToc, 8);                  // Búi tóc củ hành

    // Dải khăn đỏ bay phía sau
    float bayKhan = (dangDiChuyen ? -8.f : -3.f) * s + std::sin(t * 6.f) * 3.f;
    if (chem && u >= PHAN_LAY_DA && u < PHAN_CHEM_XONG) bayKhan -= 5.f * s;
    tu(mangDinh,
       {xT - 8.f * s, yDau - 4.f}, {xT - 7.f * s, yDau - 1.f},
       {xT - 17.f * s + bayKhan, yDau + 3.f}, {xT - 18.f * s + bayKhan, yDau},
       kKhanDo, kKhanDo, kKhanSang, kKhanSang);

    // 7. CÁNH TAY CẦM KIẾM, THANH KIẾM VÀ VỆT CHÉM
    Vector2f vai{xT + 6.f * s, yThan + 4.f};
    const float DAI_KIEM = 34.f;
    const float BAN_KINH_TAY = 9.f;
    float gocSanSang = -0.38f + std::sin(t * 2.5f) * 0.04f;

    if (chem) {
        float gocHienTai, gocDau, gocCuoi;
        GocChem(kieuChem, u, gocHienTai, gocDau, gocCuoi, gocSanSang);
        float huongChem = (gocCuoi > gocDau) ? 1.f : -1.f;
        bool dangVung = (u >= PHAN_LAY_DA && u < PHAN_CHEM_XONG + 0.04f);

        auto viTriTay = [&](float goc) { return Vector2f(vai.x + std::cos(goc) * BAN_KINH_TAY * s, vai.y + std::sin(goc) * BAN_KINH_TAY); };

        // Vệt chém lưỡi liềm: đầu sáng bám theo mũi kiếm, đuôi mờ dần
        if (u >= PHAN_LAY_DA) {
            float doDuc = 1.f;
            float gocDuoi = gocDau;
            float gocDauVet = gocHienTai;
            if (u >= PHAN_CHEM_XONG) {
                float q = Kep01((u - PHAN_CHEM_XONG) / 0.22f);
                doDuc = 1.f - q;
                gocDuoi = Tron(gocDau, gocCuoi, 0.75f * q);
                gocDauVet = gocCuoi;
            }
            if (doDuc > 0.02f && std::abs(gocDauVet - gocDuoi) > 0.05f) {
                VeVetChem(mangDinh, vai, s, gocDuoi, gocDauVet, BAN_KINH_TAY + DAI_KIEM + 9.f, doDuc);
            }
        }

        // Dư ảnh của lưỡi kiếm khi vung nhanh
        if (dangVung) {
            for (int k = 3; k >= 1; --k) {
                float gocMa = gocHienTai - huongChem * 0.15f * static_cast<float>(k);
                VeThanhKiem(mangDinh, viTriTay(gocMa), gocMa, s, DAI_KIEM, 0.30f / static_cast<float>(k), t);
            }
        }

        Vector2f tay = viTriTay(gocHienTai);
        veTay(vai, tay, 2.2f, kAoLam);
        VeThanhKiem(mangDinh, tay, gocHienTai, s, DAI_KIEM, 1.f, t);
        elip(mangDinh, tay.x, tay.y, 3.f * absS, 3.f, kDa, 8);

        // Ánh lóe ở mũi kiếm ngay lúc chém
        if (dangVung) {
            Vector2f mui = tay + Vector2f(std::cos(gocHienTai) * s, std::sin(gocHienTai)) * (DAI_KIEM + 7.f);
            elip(mangDinh, mui.x, mui.y, 5.5f, 5.5f, mau(255, 255, 255, 190), 10);
            elip(mangDinh, mui.x, mui.y, 9.f, 9.f, mau(255, 235, 170, 70), 12);
        }
    } else if (camKiem) {
        // Thế thủ sau nhát chém: kiếm chếch lên trước mặt
        Vector2f tay{vai.x + std::cos(gocSanSang) * BAN_KINH_TAY * s, vai.y + std::sin(gocSanSang) * BAN_KINH_TAY};
        veTay(vai, tay, 2.4f, kAoLam);
        VeThanhKiem(mangDinh, tay, gocSanSang, s, DAI_KIEM, 1.f, t);
        elip(mangDinh, tay.x, tay.y, 3.f * absS, 3.f, kDa, 8);
    } else if (coKiem) {
        // Kiếm đã tra vào vỏ: tay đặt lên chuôi
        veTay(vai, chuoiTraVao + Vector2f(0.5f * s, 1.5f), 2.f, kAoLam);
        elip(mangDinh, chuoiTraVao.x + 0.5f * s, chuoiTraVao.y + 1.5f, 3.f * absS, 3.f, kDa, 8);
    } else {
        // Chưa có kiếm: tay buông thõng, đung đưa theo bước
        float luaTay = dangDiChuyen ? phaBuoc * 5.f : std::sin(t * 3.f) * 0.8f;
        Vector2f tay{xT + (8.f + luaTay * 0.45f) * s, yThan + 13.f};
        veTay(vai, tay, 1.6f, kAoLam);
        elip(mangDinh, tay.x, tay.y, 3.f * absS, 3.f, kDa, 8);
    }

    // 8. THANH MÁU MINI TRÊN ĐẦU (Terraria Mini Health Bar)
    float rongThanh = 38.f;
    float caoThanh = 5.f;
    float yThanhMau = yDau - 18.f;
    float tiLeMau = std::clamp(static_cast<float>(sucKhoe) / static_cast<float>(sucKhoeToiDa), 0.0f, 1.0f);

    // Viền khung kim loại
    chuNhat(mangDinh, x - rongThanh * 0.5f - 1.5f, yThanhMau - 1.5f, rongThanh + 3.f, caoThanh + 3.f, mau(30, 30, 36, 220));
    // Nền tối
    chuNhat(mangDinh, x - rongThanh * 0.5f, yThanhMau, rongThanh, caoThanh, mau(45, 20, 20, 200));
    // Vạch máu đỏ rực
    if (tiLeMau > 0.01f) {
        chuNhat(mangDinh, x - rongThanh * 0.5f, yThanhMau, rongThanh * tiLeMau, caoThanh, mau(225, 30, 45));
        chuNhat(mangDinh, x - rongThanh * 0.5f, yThanhMau, rongThanh * tiLeMau, 1.5f, mau(255, 120, 130)); // Vệt sáng trên
    }

    window.draw(mangDinh);
}


