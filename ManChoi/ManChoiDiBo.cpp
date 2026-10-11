#include "ManChoiDiBo.h"
#include "../AmThanh/QuanLyAmThanh.h"
#include "../BanDo/DoHoa.h"
#include <algorithm>
#include <cmath>

using namespace dohoa;

namespace {
float KhoangCach(sf::Vector2f a, sf::Vector2f b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}
}

ManChoiDiBo::ManChoiDiBo(NguCanhGame& ngu, float rongTheGioi, float caoTheGioi, sf::Vector2f viTriBatDau)
    : ManChoi(ngu),
      window(ngu.window),
      nguoiChoi(viTriBatDau),
      quanLyGiaoDien(ngu.phongChu),
      thoiGian(0.f),
      thoiGianKhoaDieuKhien(0.f),
      chiSoCauThoai(0),
      thoiGianThongBao(0.f),
      viTriThongBao(370.f, 40.f),
      daKetThuc(false),
      ketQuaDuKien(KetQuaManChoi::THANG),
      cheDoThuyen(false),
      duocDieuKhienKhung(false),
      chiSoDiemGan(-1),
      coDiemGanKhoa(false),
      chiSoDiemKhoaGan(-1) {
    KhoiTaoThongSo(rongTheGioi, caoTheGioi, viTriBatDau);
}

void ManChoiDiBo::KhoiTaoThongSo(float rongTheGioi, float caoTheGioi, sf::Vector2f viTriBatDau) {
    kichThuocTheGioi = {rongTheGioi, caoTheGioi};
    mayAnh.DatTheGioi(rongTheGioi, caoTheGioi);
    mayAnh.DatNgayLapTuc(viTriBatDau);
    mayAnh.DatDoMuot(5.f);
    nguoiChoi.DatVatCan(&danhSachVatCan);
    dongGoiYDieuKhien = "W A S D: Đi    Shift: Chạy    E: Tương tác    Space hoặc J: Chém    Esc: Tạm dừng";
    quanLyNhiemVu.XoaToanBo();
}

// ---------------------------------------------------------------------------
//  Nền bản đồ
// ---------------------------------------------------------------------------
void ManChoiDiBo::BatDauVeNen() {
    (void)nenBanDo.resize({static_cast<unsigned int>(kichThuocTheGioi.x), static_cast<unsigned int>(kichThuocTheGioi.y)});
    nenBanDo.clear(sf::Color(60, 120, 60));
}

void ManChoiDiBo::KetThucVeNen() {
    nenBanDo.display();
}

void ManChoiDiBo::SonNenCo(sf::RenderTarget& dich, sf::FloatRect vung, std::uint32_t hat, sf::Color mauToi, sf::Color mauSang) {
    VA va(sf::PrimitiveType::Triangles);
    const float o = 24.f;

    // Màu mỗi đỉnh lấy theo nhiễu mượt nên cỏ chuyển sắc từ từ, không thành ô vuông
    auto mauTai = [&](float x, float y) {
        float n = nhieu(x * 0.012f, y * 0.012f, hat) * 0.65f + nhieu(x * 0.05f, y * 0.05f, hat + 7) * 0.35f;
        return pha(mauToi, mauSang, n);
    };

    for (float y = vung.position.y; y < vung.position.y + vung.size.y; y += o) {
        for (float x = vung.position.x; x < vung.position.x + vung.size.x; x += o) {
            tu(va, {x, y}, {x + o, y}, {x + o, y + o}, {x, y + o},
               mauTai(x, y), mauTai(x + o, y), mauTai(x + o, y + o), mauTai(x, y + o));
        }
    }

    // Cụm cỏ và hoa dại rải ngẫu nhiên
    Rng rng(hat * 2654435761u + 17u);
    int soCum = static_cast<int>(vung.size.x * vung.size.y / 900.f);
    for (int i = 0; i < soCum; ++i) {
        float x = vung.position.x + rng.f() * vung.size.x;
        float y = vung.position.y + rng.f() * vung.size.y;
        Color c = pha(toi(mauToi, 0.8f), sang(mauSang, 0.2f), rng.f());
        for (int b = 0; b < 3; ++b) {
            float lech = (static_cast<float>(b) - 1.f) * 3.f;
            tam(va, {x + lech - 1.2f, y}, {x + lech + 1.2f, y}, {x + lech * 1.6f, y - 6.f - rng.f() * 4.f}, c);
        }
        if (rng.f() < 0.07f) {
            Color hoa = (rng.irange(0, 2) == 0) ? mau(245, 240, 200) : (rng.irange(0, 1) == 0 ? mau(240, 200, 90) : mau(230, 130, 150));
            elip(va, x + 4.f, y - 7.f, 2.2f, 2.2f, hoa, 6);
        }
    }
    dich.draw(va);
}

void ManChoiDiBo::SonDuongDat(sf::RenderTarget& dich, const std::vector<sf::Vector2f>& diem, float rong) {
    VA va(sf::PrimitiveType::Triangles);

    // Viền tối mờ rồi mới tới lòng đường sáng, giúp đường có chiều sâu
    for (int lop = 0; lop < 2; ++lop) {
        float day = (lop == 0) ? rong + 10.f : rong;
        Color c = (lop == 0) ? mau(112, 88, 56, 120) : mau(184, 152, 104);
        for (size_t i = 0; i + 1 < diem.size(); ++i) duong(va, diem[i], diem[i + 1], day, c);
        for (const auto& d : diem) elip(va, d.x, d.y, day * 0.5f, day * 0.5f, c, 14);
    }

    // Sỏi nhỏ dọc đường
    Rng rng(static_cast<std::uint32_t>(rong * 31.f) + 5u);
    for (size_t i = 0; i + 1 < diem.size(); ++i) {
        sf::Vector2f d = diem[i + 1] - diem[i];
        float dai = std::sqrt(d.x * d.x + d.y * d.y);
        int soSoi = static_cast<int>(dai / 14.f);
        for (int k = 0; k < soSoi; ++k) {
            float t = rng.f();
            sf::Vector2f p = diem[i] + d * t + sf::Vector2f(rng.range(-rong * 0.4f, rong * 0.4f), rng.range(-rong * 0.4f, rong * 0.4f));
            elip(va, p.x, p.y, 1.6f + rng.f() * 1.6f, 1.2f + rng.f(), mau(140, 112, 76, 200), 6);
        }
    }
    dich.draw(va);
}

void ManChoiDiBo::SonSongBenPhai(sf::RenderTarget& dich, sf::FloatRect vungNuoc, std::uint32_t hat) {
    VA va(sf::PrimitiveType::Triangles);
    float x0 = vungNuoc.position.x;
    float y0 = vungNuoc.position.y;
    float cao = vungNuoc.size.y;
    float rong = vungNuoc.size.x;

    // Nước từ nông (sát bờ) sang sâu
    Color nong = mau(104, 184, 198);
    Color vua = mau(46, 118, 160);
    Color sau = mau(22, 70, 120);
    chuNhatNgang(va, x0, y0, rong * 0.4f, cao, nong, vua);
    chuNhatNgang(va, x0 + rong * 0.4f, y0, rong * 0.6f, cao, vua, sau);

    // Bờ cát lượn sóng và bọt sóng
    const float buoc = 20.f;
    for (float y = y0; y < y0 + cao; y += buoc) {
        auto bo = [&](float yy) { return x0 - 30.f + nhieu(yy * 0.02f, 3.f, hat) * 26.f; };
        float xa = bo(y), xb = bo(y + buoc);
        tu(va, {xa - 44.f, y}, {xa, y}, {xb, y + buoc}, {xb - 44.f, y + buoc},
           mau(206, 188, 140), mau(214, 198, 152), mau(214, 198, 152), mau(206, 188, 140));
        tu(va, {xa, y}, {xa + 12.f, y}, {xb + 12.f, y + buoc}, {xb, y + buoc},
           mau(236, 242, 240, 200), mau(236, 242, 240, 0), mau(236, 242, 240, 0), mau(236, 242, 240, 200));
    }

    // Lau sậy dọc bờ cát
    Rng rng(hat + 99u);
    for (int i = 0; i < static_cast<int>(cao / 26.f); ++i) {
        float y = y0 + rng.f() * cao;
        float x = x0 - 54.f + rng.f() * 24.f;
        int soCay = rng.irange(3, 5);
        for (int k = 0; k < soCay; ++k) {
            float lech = static_cast<float>(k - 2) * 4.f;
            duong(va, {x + lech, y}, {x + lech * 1.8f + 2.f, y - 28.f - rng.f() * 16.f}, 2.f, mau(120, 146, 76));
            elip(va, x + lech * 1.8f + 2.f, y - 34.f - rng.f() * 12.f, 2.2f, 6.f, mau(150, 110, 70), 6);
        }
    }
    dich.draw(va);
}

void ManChoiDiBo::VeAnhNuoc(sf::FloatRect vungNuoc) const {
    sf::FloatRect nhin = mayAnh.GetVungNhin();
    if (!nhin.findIntersection(vungNuoc)) return;

    VA va(sf::PrimitiveType::Triangles);
    int soDong = static_cast<int>(vungNuoc.size.y / 30.f);
    for (int i = 0; i < soDong; ++i) {
        float y = vungNuoc.position.y + static_cast<float>(i) * 30.f + std::sin(thoiGian * 1.2f + static_cast<float>(i)) * 4.f;
        float x = vungNuoc.position.x + std::fmod(static_cast<float>((i * 53) % 100) + thoiGian * (8.f + static_cast<float>(i % 4) * 3.f), vungNuoc.size.x);
        float dai = 18.f + static_cast<float>((i * 7) % 20);
        if (x + dai > vungNuoc.position.x + vungNuoc.size.x) continue;
        duong(va, {x, y}, {x + dai, y}, 1.6f, mau(220, 240, 250, 70));
    }
    window.draw(va);
}

// ---------------------------------------------------------------------------
//  Vật trang trí, NPC
// ---------------------------------------------------------------------------
int ManChoiDiBo::ThemVat(LoaiVat loai, sf::Vector2f viTri, float tiLe, int bienThe) {
    danhSachVat.emplace_back(loai, viTri, tiLe, bienThe);
    return static_cast<int>(danhSachVat.size()) - 1;
}

void ManChoiDiBo::CapNhatVatCan() {
    danhSachVatCan.clear();
    for (const auto& vat : danhSachVat) {
        sf::FloatRect khung = vat.GetKhungVaCham();
        if (khung.size.x > 0.f && khung.size.y > 0.f) danhSachVatCan.push_back(khung);
    }
}

NhanVatPhu* ManChoiDiBo::ThemNPC(sf::Vector2f viTri, LoaiNPC loai, const std::string& ten,
                                 const std::vector<std::string>& loiThoai, bool tuDongTroChuyen) {
    danhSachNPC.push_back(std::make_unique<NhanVatPhu>(viTri, loai, ten, loiThoai));
    NhanVatPhu* npc = danhSachNPC.back().get();

    // NPC chiếm một ô nhỏ để người chơi không đi xuyên qua
    danhSachVatCan.push_back(sf::FloatRect({viTri.x - 11.f, viTri.y + 4.f}, {22.f, 12.f}));

    if (tuDongTroChuyen) {
        DiemTuongTac diem;
        diem.viTri = viTri;
        diem.banKinh = 64.f;
        diem.goiY = "Trò chuyện";
        diem.docCao = 62.f;
        diem.hanhDong = [this, npc]() {
            BatDauHoiThoai({{npc->GetTen(), npc->LayLoiThoaiTiepTheo(), npc->GetLoai()}});
        };
        danhSachDiemTuongTac.push_back(diem);
    }
    return npc;
}

// ---------------------------------------------------------------------------
//  Hội thoại, thông báo
// ---------------------------------------------------------------------------
void ManChoiDiBo::BatDauHoiThoai(const std::vector<CauThoai>& cacCau, std::function<void()> khiXong) {
    if (cacCau.empty()) return;
    hangDoiThoai = cacCau;
    chiSoCauThoai = 0;
    khiThoaiXong = std::move(khiXong);
    quanLyGiaoDien.GetHopThoai().MoHopThoai(hangDoiThoai[0].nguoiNoi, hangDoiThoai[0].noiDung, hangDoiThoai[0].loai);
    nguoiChoi.DungLai();
}

bool ManChoiDiBo::DangHoiThoai() const {
    return const_cast<ManChoiDiBo*>(this)->quanLyGiaoDien.GetHopThoai().IsDangHienThi();
}

void ManChoiDiBo::HienThongBao(const std::string& noiDung, float thoiGianHien) {
    thongBao = noiDung;
    thoiGianThongBao = thoiGianHien;
}

void ManChoiDiBo::ThemChuBay(sf::Vector2f viTri, const std::string& noiDung, sf::Color mauChu) {
    danhSachChuBay.push_back({viTri, noiDung, mauChu, 0.f, 1.4f});
}

void ManChoiDiBo::BatDauCanhMoDau(const std::string& tieuDe, const std::string& loiDan, float thoiLuong) {
    quanLyCanhPhim.BatDauCanhTuyChinh(tieuDe, loiDan, thoiLuong);
}

void ManChoiDiBo::HoanThanhChuong(const std::string& tieuDe, const std::string& noiDung) {
    if (daKetThuc) return;
    daKetThuc = true;
    ketQuaDuKien = KetQuaManChoi::THANG;
    tieuDeKet = tieuDe;
    noiDungKet = noiDung;
    nguoiChoi.DungLai();
    nguCanh.amThanh.PhatTiengChienThang();
}

void ManChoiDiBo::BaoThatBai(const std::string& loiNhan) {
    if (daKetThuc) return;
    daKetThuc = true;
    ketQuaDuKien = KetQuaManChoi::THUA;
    tieuDeKet = "THẤT BẠI";
    noiDungKet = loiNhan.empty() ? "Chiến binh đã ngã xuống. Hãy thử lại: giữ khoảng cách, chém rồi lùi, đừng để bị vây." : loiNhan;
}

void ManChoiDiBo::DongBoTienDoVoiTuDo(int maVatPham) {
    NhiemVu* nv = quanLyNhiemVu.GetNhiemVuHienTai();
    if (!nv || nv->IsDaHoanThanh()) return;

    int dangCo = nguoiChoi.GetTuDo().GetSoLuongVatPham(maVatPham);
    int muc = std::min(dangCo, nv->GetMucTieu());
    if (muc > nv->GetTienDo()) quanLyNhiemVu.CapNhatTienDoNhiemVu(muc - nv->GetTienDo());
}

// ---------------------------------------------------------------------------
//  Sự kiện
// ---------------------------------------------------------------------------
void ManChoiDiBo::XuLySuKien(const sf::Event& suKien) {
    const auto* phim = suKien.getIf<sf::Event::KeyPressed>();
    if (!phim) return;

    bool phimXacNhan = (phim->code == sf::Keyboard::Key::Enter || phim->code == sf::Keyboard::Key::Space ||
                        phim->code == sf::Keyboard::Key::E || phim->code == sf::Keyboard::Key::J);

    // Màn kết chương: Enter hoặc Space để thoát
    if (daKetThuc) {
        if (phim->code == sf::Keyboard::Key::Enter || phim->code == sf::Keyboard::Key::Space) ketQua = ketQuaDuKien;
        return;
    }

    // Đang chiếu phim: bấm phím bất kỳ trong nhóm xác nhận để bỏ qua
    if (quanLyCanhPhim.IsDangChieuPhim()) {
        if (phimXacNhan) quanLyCanhPhim.KetThucCanh();
        return;
    }

    // Đang thoại: bấm để hiện hết chữ, bấm nữa để sang câu kế
    if (DangHoiThoai()) {
        if (!phimXacNhan) return;
        HopThoai& hop = quanLyGiaoDien.GetHopThoai();
        if (!hop.DaGoXong()) {
            hop.BoQuaHieuUngGo();
            return;
        }
        ++chiSoCauThoai;
        if (chiSoCauThoai < hangDoiThoai.size()) {
            const CauThoai& cau = hangDoiThoai[chiSoCauThoai];
            hop.MoHopThoai(cau.nguoiNoi, cau.noiDung, cau.loai);
        } else {
            hop.DongHopThoai();
            thoiGianKhoaDieuKhien = 0.25f;   // Tránh phím Space vừa bấm biến thành một nhát chém
            auto xong = std::move(khiThoaiXong);
            khiThoaiXong = nullptr;
            if (xong) xong();
        }
        return;
    }

    // Tương tác bằng E
    if (phim->code == sf::Keyboard::Key::E && thoiGianKhoaDieuKhien <= 0.f && !nguoiChoi.IsDaHySinh()) {
        if (chiSoDiemGan >= 0 && chiSoDiemGan < static_cast<int>(danhSachDiemTuongTac.size())) {
            auto hanhDong = danhSachDiemTuongTac[chiSoDiemGan].hanhDong;
            if (hanhDong) hanhDong();
        }
    }
}

// ---------------------------------------------------------------------------
//  Cập nhật
// ---------------------------------------------------------------------------
void ManChoiDiBo::CapNhat(float deltaTime) {
    thoiGian += deltaTime;
    quanLyCanhPhim.CapNhat(deltaTime);
    quanLyGiaoDien.CapNhat(deltaTime);
    quanLyNhiemVu.CapNhat(deltaTime);

    if (thoiGianThongBao > 0.f) thoiGianThongBao -= deltaTime;
    if (thoiGianKhoaDieuKhien > 0.f) thoiGianKhoaDieuKhien -= deltaTime;

    for (auto& vat : danhSachVat) vat.CapNhat(deltaTime);
    for (auto& npc : danhSachNPC) npc->CapNhat(deltaTime);

    for (auto& chu : danhSachChuBay) chu.tuoi += deltaTime;
    danhSachChuBay.erase(std::remove_if(danhSachChuBay.begin(), danhSachChuBay.end(),
                                        [](const ChuBay& c) { return c.tuoi >= c.toiDa; }),
                         danhSachChuBay.end());

    bool dangPhim = quanLyCanhPhim.IsDangChieuPhim();
    bool duocDieuKhien = !daKetThuc && !dangPhim && !DangHoiThoai() && thoiGianKhoaDieuKhien <= 0.f && !nguoiChoi.IsDaHySinh();

    duocDieuKhienKhung = duocDieuKhien;
    if (duocDieuKhien && !cheDoThuyen) nguoiChoi.XuLyPhim(deltaTime);
    else nguoiChoi.DungLai();
    nguoiChoi.CapNhat(deltaTime);

    // Điểm tương tác gần nhất
    chiSoDiemGan = -1;
    chiSoDiemKhoaGan = -1;
    if (duocDieuKhien) {
        float tot = 1e9f;
        float totKhoa = 1e9f;
        for (size_t i = 0; i < danhSachDiemTuongTac.size(); ++i) {
            const DiemTuongTac& d = danhSachDiemTuongTac[i];
            float kc = KhoangCach(nguoiChoi.GetViTri(), d.viTri);
            if (kc > d.banKinh) continue;
            bool mo = !d.dangMo || d.dangMo();
            if (mo && kc < tot) {
                tot = kc;
                chiSoDiemGan = static_cast<int>(i);
            } else if (!mo && !d.goiYKhiKhoa.empty() && kc < totKhoa) {
                totKhoa = kc;
                chiSoDiemKhoaGan = static_cast<int>(i);
            }
        }
    }

    if (!daKetThuc) {
        CapNhatRieng(deltaTime);
        if (nguoiChoi.IsDaHySinh()) BaoThatBai();
    }

    mayAnh.DatMucTieu(ViTriMayAnhTheoDoi());
    mayAnh.CapNhat(deltaTime);
}

// ---------------------------------------------------------------------------
//  Vẽ
// ---------------------------------------------------------------------------
void ManChoiDiBo::VeHopChu(const std::string& noiDung, unsigned int coChu, sf::Vector2f tam, sf::Color mauChu, sf::Color mauNen) const {
    sf::Text chu(nguCanh.phongChu, VanBan(noiDung), coChu);
    sf::FloatRect khung = chu.getLocalBounds();
    float rong = khung.size.x + 20.f;
    float cao = static_cast<float>(coChu) + 14.f;

    sf::RectangleShape nen({rong, cao});
    nen.setOrigin({rong * 0.5f, cao * 0.5f});
    nen.setPosition(tam);
    nen.setFillColor(mauNen);
    nen.setOutlineThickness(1.5f);
    nen.setOutlineColor(sf::Color(218, 165, 32, mauNen.a));
    window.draw(nen);

    chu.setFillColor(mauChu);
    chu.setOrigin({khung.position.x + khung.size.x * 0.5f, khung.position.y + khung.size.y * 0.5f});
    chu.setPosition(tam);
    window.draw(chu);
}

void ManChoiDiBo::Ve() {
    // ----- Thế giới (theo camera) -----
    window.setView(mayAnh.LayView(window));
    window.draw(sf::Sprite(nenBanDo.getTexture()));
    VeNenDong();

    sf::FloatRect nhin = mayAnh.GetVungNhin();
    std::vector<std::pair<float, std::function<void()>>> mucVe;

    for (const auto& vat : danhSachVat) {
        if (!nhin.findIntersection(vat.GetKhungHienThi())) continue;
        mucVe.push_back({vat.GetYSapXep(), [this, &vat]() { vat.Ve(window, thoiGian); }});
    }
    for (auto& npc : danhSachNPC) {
        NhanVatPhu* p = npc.get();
        mucVe.push_back({p->GetViTri().y, [p, this]() { p->Ve(window); }});
    }
    if (!cheDoThuyen) mucVe.push_back({nguoiChoi.GetViTri().y, [this]() { nguoiChoi.Ve(window); }});
    for (const auto& dich : heThongChienDau.GetDanhSachKeDich()) {
        KeDich* d = dich.get();
        if (d->IsDaBienMat()) continue;
        mucVe.push_back({d->GetViTri().y, [d, this]() { d->Ve(window); }});
    }
    ThemMucVeRieng(mucVe);

    std::stable_sort(mucVe.begin(), mucVe.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    for (auto& muc : mucVe) muc.second();

    VeTrongTheGioi();
    quanLyGiaoDien.VeHieuUngVaSatThuong(window);

    // Chữ nổi
    for (const auto& chu : danhSachChuBay) {
        float t = chu.tuoi / chu.toiDa;
        sf::Text text(nguCanh.phongChu, VanBan(chu.noiDung), 14);
        text.setFillColor(sf::Color(chu.mau.r, chu.mau.g, chu.mau.b, static_cast<std::uint8_t>(255.f * (1.f - t * t))));
        text.setOutlineThickness(2.f);
        text.setOutlineColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(200.f * (1.f - t))));
        sf::FloatRect kh = text.getLocalBounds();
        text.setOrigin({kh.position.x + kh.size.x * 0.5f, kh.position.y + kh.size.y * 0.5f});
        text.setPosition({chu.viTri.x, chu.viTri.y - 50.f * t});
        window.draw(text);
    }

    // ----- Giao diện (khung cố định 800x600) -----
    ApDungViewLogic(window);
    if (!cheDoThuyen) quanLyGiaoDien.VeHUD(window, nguoiChoi);
    quanLyNhiemVu.Ve(window, nguCanh.phongChu, {555.f, 20.f});
    VeThongTinPhu();

    // Gợi ý tương tác vẽ sau HUD (nhưng theo toạ độ thế giới) để không bị thanh máu hay túi đồ che
    window.setView(mayAnh.LayView(window));
    // Gợi ý tương tác treo trên đầu đối tượng
    if (!daKetThuc && !DangHoiThoai() && !quanLyCanhPhim.IsDangChieuPhim()) {
        if (chiSoDiemGan >= 0) {
            const DiemTuongTac& d = danhSachDiemTuongTac[chiSoDiemGan];
            float nhay = std::sin(thoiGian * 5.f) * 2.f;
            VeHopChu("[E] " + d.goiY, 12, {d.viTri.x, d.viTri.y - d.docCao + nhay}, sf::Color(255, 235, 140), sf::Color(20, 25, 35, 225));
        } else if (chiSoDiemKhoaGan >= 0) {
            const DiemTuongTac& d = danhSachDiemTuongTac[chiSoDiemKhoaGan];
            VeHopChu(d.goiYKhiKhoa, 11, {d.viTri.x, d.viTri.y - d.docCao}, sf::Color(190, 195, 205), sf::Color(20, 25, 35, 170));
        }
    }

    ApDungViewLogic(window);

    if (thoiGianThongBao > 0.f) {
        float mo = std::min(1.f, thoiGianThongBao * 2.f);
        VeHopChu(thongBao, 13, viTriThongBao, sf::Color(255, 255, 255, static_cast<std::uint8_t>(255.f * mo)),
                 sf::Color(20, 25, 35, static_cast<std::uint8_t>(215.f * mo)));
    }

    quanLyGiaoDien.GetHopThoai().Ve(window, nguCanh.phongChu);

    if (!quanLyCanhPhim.IsDangChieuPhim() && !DangHoiThoai() && !daKetThuc) {
        VeChuGiua(window, nguCanh.phongChu, dongGoiYDieuKhien, 11, {RONG_MAN_HINH * 0.5f, CAO_MAN_HINH - 14.f}, sf::Color(205, 215, 230), 1.5f, sf::Color(0, 0, 0, 200));
    }

    quanLyCanhPhim.VeHieuUngDienAnh(window, nguCanh.phongChu);

    // ----- Màn kết chương -----
    if (daKetThuc) {
        sf::RectangleShape toi({RONG_MAN_HINH, CAO_MAN_HINH});
        toi.setFillColor(sf::Color(0, 0, 0, 175));
        window.draw(toi);

        sf::RectangleShape khung({580.f, 330.f});
        khung.setOrigin({290.f, 165.f});
        khung.setPosition({RONG_MAN_HINH * 0.5f, CAO_MAN_HINH * 0.5f});
        khung.setFillColor(sf::Color(20, 25, 35, 245));
        khung.setOutlineThickness(3.f);
        khung.setOutlineColor(ketQuaDuKien == KetQuaManChoi::THANG ? sf::Color(255, 215, 0) : sf::Color(200, 60, 50));
        window.draw(khung);

        std::string dau = (ketQuaDuKien == KetQuaManChoi::THANG) ? "HOÀN THÀNH" : "";
        VeChuGiua(window, nguCanh.phongChu, dau, 12, {RONG_MAN_HINH * 0.5f, 160.f}, sf::Color(150, 205, 160));
        VeChuGiua(window, nguCanh.phongChu, tieuDeKet, 24,
                  {RONG_MAN_HINH * 0.5f, 200.f},
                  ketQuaDuKien == KetQuaManChoi::THANG ? sf::Color(255, 215, 0) : sf::Color(220, 60, 50), 2.f, sf::Color(100, 40, 20));

        if (ketQuaDuKien == KetQuaManChoi::THANG) {
            VeChuTrai(window, nguCanh.phongChu, "", 12, {RONG_MAN_HINH * 0.5f - 250.f, 245.f}, sf::Color(180, 200, 225));
        }
        VeChuTrai(window, nguCanh.phongChu, NgatDong(nguCanh.phongChu, noiDungKet, 13, 500.f), 13,
                  {RONG_MAN_HINH * 0.5f - 250.f, ketQuaDuKien == KetQuaManChoi::THANG ? 268.f : 250.f}, sf::Color::White);

        VeChuGiua(window, nguCanh.phongChu, "[ Enter hoặc Space để tiếp tục ]", 12, {RONG_MAN_HINH * 0.5f, 430.f}, sf::Color(180, 200, 220));
    }
}
