#include "DoiThuyenGiac.h"
#include "../BanDo/DoHoa.h"
#include <algorithm>
#include <cmath>

using namespace dohoa;

namespace {
const float TOC_DO_TIEN_VAO = 55.f;
const float TOC_DO_BO_CHAY = 118.f;
const float TOC_DO_MUI_TEN = 310.f;
const float Y_NUOC_TREN = 315.f;
const float Y_NUOC_DUOI = 725.f;
const float TAM_Y_LUONG_CHINH = 530.f;   // Khi hoảng loạn, thuyền dồn vào luồng giữa sông

float Dai(sf::Vector2f v) { return std::sqrt(v.x * v.x + v.y * v.y); }
}

DoiThuyenGiac::DoiThuyenGiac()
    : cheDo(CheDo::TIEN_VAO),
      doHang(60.f),
      hoiKhieuKhich(0.f),
      thoiGianTangToc(0.f),
      hoiBan(2.f),
      hoiVaCham(0.f),
      xDungDoXet(1850.f),
      heSoTocCoBan(1.f),
      daToiDiemDung(false),
      heSoTocChung(1.f),
      nhipBan(2.4f) {}

void DoiThuyenGiac::ThemThuyen(sf::Vector2f viTri, bool soChiHuy) {
    cacThuyen.push_back(std::make_unique<ThuyenKeDich>(viTri, soChiHuy));
    size_t i = cacThuyen.size() - 1;
    tocDoDuoi.push_back(soChiHuy ? 72.f : 92.f - 4.f * static_cast<float>(i % 3));
    phaLac.push_back(static_cast<float>(i) * 1.9f);
}

bool DoiThuyenGiac::KhieuKhich() {
    if (hoiKhieuKhich > 0.f || cheDo == CheDo::HOANG_LOAN) return false;
    doHang = std::min(100.f, doHang + 35.f);
    thoiGianTangToc = 3.f;
    hoiKhieuKhich = 7.f;
    if (cheDo == CheDo::TIEN_VAO && daToiDiemDung) cheDo = CheDo::DUOI;
    return true;
}

float DoiThuyenGiac::KhoangCachGanNhat(sf::Vector2f viTri) const {
    float tot = 1e9f;
    for (const auto& t : cacThuyen) {
        if (t->IsDaBiPhaHuy()) continue;
        tot = std::min(tot, Dai(t->GetViTri() - viTri));
    }
    return tot;
}

int DoiThuyenGiac::ChiSoThuyenGanNhat(sf::Vector2f viTri) const {
    int chiSo = -1;
    float tot = 1e9f;
    for (size_t i = 0; i < cacThuyen.size(); ++i) {
        if (cacThuyen[i]->IsDaBiPhaHuy() || cacThuyen[i]->IsDaMacCoc()) continue;
        float kc = Dai(cacThuyen[i]->GetViTri() - viTri);
        if (kc < tot) {
            tot = kc;
            chiSo = static_cast<int>(i);
        }
    }
    return chiSo;
}

int DoiThuyenGiac::SoThuyenMacCoc() const {
    int dem = 0;
    for (const auto& t : cacThuyen) if (t->IsDaMacCoc()) ++dem;
    return dem;
}

int DoiThuyenGiac::SoThuyenDaThoat(float xThoat) const {
    int dem = 0;
    for (const auto& t : cacThuyen) if (!t->IsDaMacCoc() && t->GetViTri().x > xThoat) ++dem;
    return dem;
}

void DoiThuyenGiac::TachCacThuyenChongNhau() {
    // Hai thuyền sát nhau thì đẩy lệch theo chiều dọc để không chồng lên nhau
    for (size_t a = 0; a < cacThuyen.size(); ++a) {
        for (size_t b = a + 1; b < cacThuyen.size(); ++b) {
            if (cacThuyen[a]->IsDaMacCoc() || cacThuyen[b]->IsDaMacCoc()) continue;
            sf::Vector2f pa = cacThuyen[a]->GetViTri();
            sf::Vector2f pb = cacThuyen[b]->GetViTri();
            float dx = std::abs(pa.x - pb.x);
            float dy = pb.y - pa.y;
            if (dx < 100.f && std::abs(dy) < 44.f) {
                float day = (dy >= 0.f ? 1.f : -1.f) * 0.5f;
                cacThuyen[a]->SetViTri({pa.x, pa.y - day});
                cacThuyen[b]->SetViTri({pb.x, pb.y + day});
            }
        }
    }
}

KetQuaDoiThuyen DoiThuyenGiac::CapNhat(float deltaTime, ThuyenNguoiChoi& nguoi, std::vector<Coc>& coc, float tiLeTrieu) {
    KetQuaDoiThuyen ketQua;

    hoiKhieuKhich = std::max(0.f, hoiKhieuKhich - deltaTime);
    thoiGianTangToc = std::max(0.f, thoiGianTangToc - deltaTime);
    hoiVaCham = std::max(0.f, hoiVaCham - deltaTime);
    hoiBan = std::max(0.f, hoiBan - deltaTime);

    for (auto& t : cacThuyen) t->CapNhat(deltaTime);

    sf::Vector2f viTriNguoi = nguoi.GetViTri();
    float heSoToc = ((thoiGianTangToc > 0.f) ? 1.4f : 1.f) * heSoTocCoBan;

    // ------------------------------------------------------------------
    //  Từng chế độ di chuyển
    // ------------------------------------------------------------------
    if (cheDo == CheDo::TIEN_VAO) {
        bool tatCaDen = true;
        for (auto& t : cacThuyen) {
            if (t->IsDaMacCoc()) continue;
            if (t->GetViTri().x > xDungDoXet) {
                t->DatTocDo(TOC_DO_TIEN_VAO);
                t->DuoiTheo({t->GetViTri().x - 1000.f, t->GetViTri().y}, deltaTime);
                tatCaDen = false;
            }
        }
        if (tatCaDen) daToiDiemDung = true;
    } else if (cheDo == CheDo::DUOI) {
        for (size_t i = 0; i < cacThuyen.size(); ++i) {
            ThuyenKeDich& t = *cacThuyen[i];
            if (t.IsDaMacCoc() || t.IsDaBiPhaHuy()) continue;
            t.DatTocDo(tocDoDuoi[i] * heSoToc * heSoTocChung);
            t.DuoiTheo(viTriNguoi, deltaTime);
        }
    } else {
        // Hoảng loạn: dồn vào luồng giữa sông rồi chạy ra biển
        for (size_t i = 0; i < cacThuyen.size(); ++i) {
            ThuyenKeDich& t = *cacThuyen[i];
            if (t.IsDaMacCoc() || t.IsDaBiPhaHuy()) continue;
            float yMucTieu = TAM_Y_LUONG_CHINH + std::sin(phaLac[i]) * 40.f;
            t.DatTocDo(TOC_DO_BO_CHAY + 8.f * static_cast<float>(i % 3));
            t.DuoiTheo({t.GetViTri().x + 1000.f, yMucTieu}, deltaTime);
        }
    }

    TachCacThuyenChongNhau();
    for (auto& t : cacThuyen) {
        sf::Vector2f p = t->GetViTri();
        if (p.y < Y_NUOC_TREN) t->SetViTri({p.x, Y_NUOC_TREN});
        if (p.y > Y_NUOC_DUOI) t->SetViTri({p.x, Y_NUOC_DUOI});
    }

    // ------------------------------------------------------------------
    //  Va chạm với cọc (chỉ gây hại khi triều đã rút thấp)
    // ------------------------------------------------------------------
    for (auto& t : cacThuyen) {
        if (t->IsDaMacCoc()) continue;
        for (auto& c : coc) {
            if (t->KiemTraVaChamCoc(c, tiLeTrieu)) {
                ketQua.viTriMacCoc.push_back(t->GetViTri());
                break;
            }
        }
    }

    if (cheDo == CheDo::HOANG_LOAN) {
        cacMuiTen.clear();
        return ketQua;
    }

    // ------------------------------------------------------------------
    //  Độ hăng: giặc chỉ đuổi tiếp nếu thuyền nhử ở khoảng cách vừa phải
    // ------------------------------------------------------------------
    if (cheDo == CheDo::DUOI) {
        float kc = KhoangCachGanNhat(viTriNguoi);
        if (kc > 480.f) doHang -= 16.f * deltaTime;
        else if (kc >= 130.f) doHang += 9.f * deltaTime;
        else doHang += 4.f * deltaTime;
        doHang = std::max(0.f, std::min(100.f, doHang));
    }

    // ------------------------------------------------------------------
    //  Bắn tên từ thuyền gần nhất
    // ------------------------------------------------------------------
    int gan = ChiSoThuyenGanNhat(viTriNguoi);
    if (cheDo == CheDo::DUOI && gan >= 0 && hoiBan <= 0.f) {
        sf::Vector2f nguon = cacThuyen[static_cast<size_t>(gan)]->GetViTri() + sf::Vector2f(0.f, -16.f);
        float kc = Dai(viTriNguoi - nguon);
        if (kc > 120.f && kc < 450.f) {
            // Ngắm trước một chút theo hướng thuyền người chơi đang đi
            sf::Vector2f diem = viTriNguoi + nguoi.GetVanToc() * (kc / TOC_DO_MUI_TEN);
            sf::Vector2f huong = diem - nguon;
            float d = Dai(huong);
            if (d > 1.f) {
                cacMuiTen.push_back({nguon, huong / d * TOC_DO_MUI_TEN, 0.f});
                ketQua.giacBanTen = true;
                hoiBan = nhipBan;
            }
        }
    }

    for (auto& m : cacMuiTen) {
        m.viTri += m.van * deltaTime;
        m.tuoi += deltaTime;
    }

    // Tên trúng thuyền người chơi
    for (auto& m : cacMuiTen) {
        if (m.tuoi < 2.f && Dai(m.viTri - viTriNguoi) < 22.f && !nguoi.IsDaBiPhaHuy()) {
            nguoi.NhanSatThuong(9);
            m.tuoi = 99.f;
            ++ketQua.soMuiTenTrung;
        }
    }
    cacMuiTen.erase(std::remove_if(cacMuiTen.begin(), cacMuiTen.end(), [](const MuiTen& m) { return m.tuoi >= 1.8f; }), cacMuiTen.end());

    // ------------------------------------------------------------------
    //  Thuyền giặc đâm vào thuyền nhẹ
    // ------------------------------------------------------------------
    if (hoiVaCham <= 0.f && cheDo == CheDo::DUOI && !nguoi.IsDaBiPhaHuy()) {
        for (auto& t : cacThuyen) {
            if (t->IsDaMacCoc()) continue;
            if (t->GetHitBox().findIntersection(nguoi.GetHitBox()).has_value()) {
                nguoi.NhanSatThuong(22);
                // Đẩy thuyền nhẹ ra xa để không bị đâm liên tục
                sf::Vector2f huong = nguoi.GetViTri() - t->GetViTri();
                float d = Dai(huong);
                if (d > 1.f) nguoi.SetViTri(nguoi.GetViTri() + huong / d * 40.f);
                hoiVaCham = 1.2f;
                ketQua.thuyenBiDam = true;
                break;
            }
        }
    }

    return ketQua;
}

void DoiThuyenGiac::VeMuiTen(sf::RenderWindow& window) const {
    if (cacMuiTen.empty()) return;
    VA va(sf::PrimitiveType::Triangles);
    for (const auto& m : cacMuiTen) {
        float d = Dai(m.van);
        if (d < 1.f) continue;
        sf::Vector2f h = m.van / d;
        sf::Vector2f duoi = m.viTri - h * 18.f;
        duong(va, duoi, m.viTri, 1.8f, mau(70, 46, 28));
        sf::Vector2f vuong(-h.y, h.x);
        tam(va, m.viTri + h * 5.f, m.viTri + vuong * 2.6f, m.viTri - vuong * 2.6f, mau(200, 205, 215));
        duong(va, duoi, duoi - h * 2.f + vuong * 2.5f, 1.2f, mau(220, 60, 50));
        duong(va, duoi, duoi - h * 2.f - vuong * 2.5f, 1.2f, mau(220, 60, 50));
    }
    window.draw(va);
}
