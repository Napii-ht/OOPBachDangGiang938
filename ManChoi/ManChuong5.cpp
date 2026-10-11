#include "ManChuong5.h"
#include "../AmThanh/QuanLyAmThanh.h"
#include "../BanDo/DoHoa.h"
#include <algorithm>
#include <cmath>

using namespace dohoa;

namespace {
const sf::Vector2f VI_TRI_THUYEN_BAN_DAU(560.f, 520.f);
const float X_THOAT_RA_BIEN = 2060.f;   // Thuyền chạy qua mốc này mà không mắc cọc là đã thoát
}

ManChuong5::ManChuong5(NguCanhGame& ngu)
    : ManChoiSong(ngu, VI_TRI_THUYEN_BAN_DAU),
      giaiDoan(GiaiDoan::GIU_CHAN),
      demTenMaiPhuc(0.f),
      daThoaiMoDau(false),
      hieuLenhDungLuc(false),
      hieuLenhSom(false),
      daBaoCuaSoMo(false),
      thoiGianHoangLoan(0.f),
      thoiGianChamLai(0.f),
      soMacCocDaBao(0) {
    viTriThongBao = {430.f, 520.f};
    dongGoiYDieuKhien = "W A S D: Chèo thuyền    Q: Khiêu khích    Space: Phát hiệu lệnh    Esc: Tạm dừng";

    DungBoSong();
    CamTatCaCoc();
    tiLeTrieu = 1.f;
    song.DatTiLeThuyTrieu(1.f);
    thuyen->DatGioiHan({{40.f, 300.f}, {1100.f, 420.f}});   // Ở lại phía tây bãi cọc

    // Hạm đội Nam Hán đã vào sâu, đang bám theo thuyền nhử ở phía tây bãi cọc
    doi.ThemThuyen({880.f, 430.f}, false);
    doi.ThemThuyen({820.f, 540.f}, true);
    doi.ThemThuyen({890.f, 640.f}, false);
    doi.ThemThuyen({760.f, 480.f}, false);
    doi.DatCheDo(DoiThuyenGiac::CheDo::DUOI);
    doi.DatDoHang(70.f);
    doi.DatHeSoTocChung(0.65f);   // Nước rút mạnh làm thuyền giặc nặng nề hơn
    doi.DatNhipBan(3.2f);
    doi.DatHeSoTocCoBan(0.6f);   // Giặc dè dặt hơn, đủ chỗ để người chơi vòng quanh giữ chân

    DatNhiemVu();

    BatDauCanhMoDau("CHƯƠNG 5: TRIỀU RÚT",
        "Hạm đội Nam Hán đã vào sâu trong sông. Nước triều bắt đầu rút. Từ hai bờ, quân Đại Việt nín thở chờ hiệu lệnh. "
        "Chỉ khi nước đủ thấp, bãi cọc mới trở thành cái bẫy chết người.", 6.f);
}

void ManChuong5::DatNhiemVu() {
    quanLyNhiemVu.DatTieuDeBang("NHIỆM VỤ CHƯƠNG 5");
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuKhamPha>(NV_GIU_CHAN, "Giữ chân hạm đội giặc",
        "Chèo trong vùng phía tây bãi cọc, kéo giặc ở lại và né tên. Đợi thanh mực nước chuyển xanh. Nhấn Q khi giặc hết hăng."));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuKhamPha>(NV_PHAT_HIEU_LENH, "Phát hiệu lệnh đúng lúc",
        "Khi thanh mực nước ở vùng xanh, nhấn Space để đánh trống lệnh. Sớm quá thì cọc còn chìm, muộn quá thì giặc nhìn thấy cọc."));
    quanLyNhiemVu.BatDau();
}

void ManChuong5::XuLySuKien(const sf::Event& suKien) {
    ManChoiSong::XuLySuKien(suKien);

    const auto* phim = suKien.getIf<sf::Event::KeyPressed>();
    if (!phim || !duocDieuKhienKhung || daKetThuc || thoiGianKhoaDieuKhien > 0.f) return;

    if (phim->code == sf::Keyboard::Key::Q && giaiDoan == GiaiDoan::GIU_CHAN) {
        if (doi.KhieuKhich()) {
            ThemChuBay(thuyen->GetViTri() + sf::Vector2f(0.f, -55.f), "Giặc kia, lại đây!", sf::Color(255, 220, 120));
            nguCanh.amThanh.PhatTiengTrongTran();
        }
    } else if (phim->code == sf::Keyboard::Key::Space && giaiDoan == GiaiDoan::GIU_CHAN) {
        PhatHieuLenh();
    }
}

void ManChuong5::PhatHieuLenh() {
    nguCanh.amThanh.PhatTiengTrongTran();
    mayAnh.Rung(4.f, 0.4f);
    giaiDoan = GiaiDoan::HOANG_LOAN;
    thoiGianHoangLoan = 0.f;
    doi.DatCheDo(DoiThuyenGiac::CheDo::HOANG_LOAN);

    if (TrongCuaSo()) {
        hieuLenhDungLuc = true;
        if (MaNhiemVuHienTai() == NV_GIU_CHAN) quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
        if (MaNhiemVuHienTai() == NV_PHAT_HIEU_LENH) quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
        HienThongBao("Trống lệnh vang lên! Giặc hoảng loạn tháo chạy ra biển", 5.f);
    } else {
        hieuLenhSom = (tiLeTrieu > MUC_NUOC_THAP_NHAT_DUNG);
        HienThongBao(hieuLenhSom ? "Nước còn cao, cọc vẫn chìm sâu..." : "Nước đã rút hết, giặc thấy cả cọc...", 5.f);
    }
}

void ManChuong5::XuLyKetQuaDoi(const KetQuaDoiThuyen& ketQua) {
    sf::Vector2f p = thuyen->GetViTri();
    if (ketQua.soMuiTenTrung > 0) {
        quanLyGiaoDien.ThemHieuUngNuocBan(p);
        ThemChuBay(p + sf::Vector2f(18.f, -40.f), "-" + std::to_string(9 * ketQua.soMuiTenTrung), sf::Color(255, 110, 90));
        nguCanh.amThanh.PhatTiengTrungDon();
        mayAnh.Rung(3.f, 0.15f);
    }
    if (ketQua.thuyenBiDam) {
        quanLyGiaoDien.ThemHieuUngNuocBan(p);
        ThemChuBay(p + sf::Vector2f(-18.f, -40.f), "-22", sf::Color(255, 90, 70));
        nguCanh.amThanh.PhatTiengVaChamCoc();
        mayAnh.Rung(6.f, 0.3f);
    }

    // Thuyền giặc đâm vào cọc: nước bắn, mảnh gỗ, rung màn hình, tiếng va chạm
    for (const auto& v : ketQua.viTriMacCoc) {
        ++soMacCocDaBao;
        quanLyGiaoDien.ThemHieuUngNuocBan(v);
        quanLyGiaoDien.ThemHieuUngVaChamGo(v);
        quanLyGiaoDien.ThemHieuUngNuocBan(v + sf::Vector2f(20.f, 10.f));
        ThemChuBay(v + sf::Vector2f(0.f, -50.f), "Mắc cọc!", sf::Color(255, 220, 110));
        nguCanh.amThanh.PhatTiengVaChamCoc();
        mayAnh.Rung(7.f, 0.4f);
        if (soMacCocDaBao == 1) thoiGianChamLai = 1.6f;   // Slow motion khi thuyền đầu tiên đâm cọc
    }
}

void ManChuong5::CapNhatTenMaiPhuc(float deltaTime) {
    for (auto& t : cacTenMaiPhuc) t.tuoi += deltaTime;
    cacTenMaiPhuc.erase(std::remove_if(cacTenMaiPhuc.begin(), cacTenMaiPhuc.end(),
                                       [](const TenMaiPhuc& t) { return t.tuoi >= t.toiDa; }),
                        cacTenMaiPhuc.end());

    // Có thuyền mắc cọc thì quân hai bờ bắn tên vào
    std::vector<sf::Vector2f> mucTieu;
    for (const auto& t : doi.GetThuyen()) if (t->IsDaMacCoc()) mucTieu.push_back(t->GetViTri());
    if (mucTieu.empty()) return;

    demTenMaiPhuc -= deltaTime;
    while (demTenMaiPhuc <= 0.f) {
        demTenMaiPhuc += 0.07f;
        static unsigned int bamNgau = 7u;
        bamNgau = bamNgau * 1664525u + 1013904223u;
        float r1 = static_cast<float>((bamNgau >> 8) & 0xFFFF) / 65535.f;
        bamNgau = bamNgau * 1664525u + 1013904223u;
        float r2 = static_cast<float>((bamNgau >> 8) & 0xFFFF) / 65535.f;
        bamNgau = bamNgau * 1664525u + 1013904223u;
        float r3 = static_cast<float>((bamNgau >> 8) & 0xFFFF) / 65535.f;

        bool bacBo = (r1 < 0.5f);
        sf::Vector2f dau(960.f + r2 * 560.f, bacBo ? 255.f : 765.f);
        sf::Vector2f dich = mucTieu[static_cast<size_t>(r3 * static_cast<float>(mucTieu.size())) % mucTieu.size()];
        dich += sf::Vector2f((r2 - 0.5f) * 50.f, (r1 - 0.5f) * 20.f);
        cacTenMaiPhuc.push_back({dau, dich, 0.f, 0.9f + r3 * 0.3f});
    }
}

void ManChuong5::CapNhatGiuChan(float deltaTime) {
    // Nước rút đều. Thuyền người chơi vẫn điều khiển được.
    tiLeTrieu = std::max(0.f, tiLeTrieu - TIEN_DO_RUT * deltaTime);
    song.DatTiLeThuyTrieu(tiLeTrieu);

    CapNhatThuyenSong(deltaTime, duocDieuKhienKhung, {0.f, 0.f}, 1.f);
    KetQuaDoiThuyen ketQua = doi.CapNhat(deltaTime, *thuyen, cocDaCam, tiLeTrieu);
    XuLyKetQuaDoi(ketQua);

    if (TrongCuaSo() && !daBaoCuaSoMo) {
        daBaoCuaSoMo = true;
        HienThongBao("Nước đã đủ thấp! Nhấn Space để phát hiệu lệnh", 8.f);
        nguCanh.amThanh.PhatTiengNhatDo();
        if (MaNhiemVuHienTai() == NV_GIU_CHAN) quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
    }

    if (thuyen->IsDaBiPhaHuy()) {
        BaoThatBai("Thuyền nhử bị phá hủy nên hạm đội giặc không còn bị giữ chân. Hãy thử lại: né tên liên tục và đừng để giặc áp sát.");
        giaiDoan = GiaiDoan::KET_THUC;
        return;
    }
    if (doi.MatHung()) {
        BaoThatBai("Giặc ngờ vực nên rút lui ra biển trước khi nước rút đủ thấp. Hãy thử lại: giữ giặc cách thuyền 130 đến 480 bước và hô khiêu chiến khi thanh độ hăng xuống thấp.");
        giaiDoan = GiaiDoan::KET_THUC;
        return;
    }
    if (tiLeTrieu < MUC_NUOC_QUA_MUON) {
        BaoThatBai("Nước đã rút hết, bãi cọc lộ ra nên giặc nhìn thấy mà tránh. Hãy thử lại: phát hiệu lệnh khi thanh mực nước còn ở vùng xanh.");
        giaiDoan = GiaiDoan::KET_THUC;
    }
}

void ManChuong5::CapNhatHoangLoan(float deltaTime) {
    thoiGianHoangLoan += deltaTime;

    // Nước tiếp tục rút nhanh cho cảnh phim: cọc nhô cao dần
    if (hieuLenhDungLuc) tiLeTrieu = std::max(0.f, tiLeTrieu - 0.12f * deltaTime);
    song.DatTiLeThuyTrieu(tiLeTrieu);

    CapNhatThuyenSong(deltaTime, duocDieuKhienKhung, {0.f, 0.f}, 1.f);
    KetQuaDoiThuyen ketQua = doi.CapNhat(deltaTime, *thuyen, cocDaCam, tiLeTrieu);
    XuLyKetQuaDoi(ketQua);
    CapNhatTenMaiPhuc(deltaTime);

    int soMac = doi.SoThuyenMacCoc();
    int soThoat = doi.SoThuyenDaThoat(X_THOAT_RA_BIEN);

    if (hieuLenhDungLuc) {
        // Đủ số thuyền mắc cọc, hoặc đã hết thuyền còn chạy được thì kết luận
        bool hetChay = (soMac + soThoat >= SO_THUYEN_GIAC);
        if ((soMac >= SO_THUYEN_MAC_COC_DAT && thoiGianHoangLoan > 3.5f) || hetChay || thoiGianHoangLoan > 14.f) {
            if (soMac >= SO_THUYEN_MAC_COC_DAT) KetThucThanhCong();
            else {
                BaoThatBai("Chỉ có " + std::to_string(soMac) + " thuyền mắc cọc, còn lại chạy thoát ra biển. Hãy thử lại: giữ giặc ở phía tây bãi cọc trước khi phát hiệu lệnh.");
                giaiDoan = GiaiDoan::KET_THUC;
            }
        }
    } else if (soThoat >= SO_THUYEN_GIAC - 1 || thoiGianHoangLoan > 12.f) {
        if (hieuLenhSom) {
            BaoThatBai("Hiệu lệnh quá sớm: nước còn cao, cọc chìm sâu nên thuyền giặc lướt qua an toàn. Hãy chờ thanh mực nước chuyển xanh rồi mới nhấn Space.");
        } else {
            BaoThatBai("Hiệu lệnh quá muộn: nước đã rút hết, giặc thấy cọc nên chạy theo lối an toàn. Hãy nhấn Space khi thanh mực nước còn ở vùng xanh.");
        }
        giaiDoan = GiaiDoan::KET_THUC;
    }
}

void ManChuong5::KetThucThanhCong() {
    giaiDoan = GiaiDoan::KET_THUC;
    BatDauHoiThoai({
        {"Ngô Quyền", "Thuyền giặc mắc cứng vào cọc rồi! Quân hai bờ, tiến lên!", LoaiNPC::NGO_QUYEN},
        {"Ngô Quyền", "Cọc nhọn đã đâm thủng đáy thuyền. Bây giờ là lúc tổng phản công.", LoaiNPC::NGO_QUYEN}
    }, [this]() {
        HoanThanhChuong("CHƯƠNG 5: TRIỀU RÚT",
            "Theo sử cũ, khi thủy triều rút, thuyền giặc vướng vào cọc nhọn bịt sắt đóng ngầm dưới lòng sông. "
            "Quân Đại Việt từ hai bên bờ đồng loạt xông ra đánh. Quân Nam Hán hoảng loạn, thuyền va vào nhau, Lưu Hoằng Tháo bị giết.");
    });
}

void ManChuong5::CapNhatRieng(float deltaTime) {
    if (!daThoaiMoDau && !quanLyCanhPhim.IsDangChieuPhim()) {
        daThoaiMoDau = true;
        BatDauHoiThoai({
            {"Ngô Quyền", "Nước đã bắt đầu rút. Hạm đội giặc đang ở phía tây bãi cọc, bám theo thuyền nhử của ngươi.", LoaiNPC::NGO_QUYEN},
            {"Ngô Quyền", "Giữ chân giặc ở đó, đừng để chúng quay ra biển sớm. Cọc chỉ đâm thủng được thuyền khi nước rút xuống thấp.", LoaiNPC::NGO_QUYEN},
            {"Ngô Quyền", "Nhìn thanh mực nước: khi nó chuyển sang vùng xanh, nhấn Space để đánh trống lệnh. Sớm quá hay muộn quá đều hỏng việc.", LoaiNPC::NGO_QUYEN}
        });
    }

    bool dangKichBan = DangHoiThoai() || quanLyCanhPhim.IsDangChieuPhim();
    if (giaiDoan == GiaiDoan::KET_THUC) {
        song.CapNhat(deltaTime);
        CapNhatTenMaiPhuc(deltaTime);
        thuyen->DungLai();
        return;
    }

    // Slow motion ngắn khi thuyền giặc đầu tiên mắc cọc
    float dt = deltaTime;
    if (thoiGianChamLai > 0.f) {
        thoiGianChamLai -= deltaTime;
        dt = deltaTime * 0.4f;
    }
    song.CapNhat(dt);

    if (dangKichBan) {
        thuyen->DungLai();
        return;
    }

    if (giaiDoan == GiaiDoan::GIU_CHAN) CapNhatGiuChan(dt);
    else CapNhatHoangLoan(dt);
}

sf::Vector2f ManChuong5::ViTriMayAnhTheoDoi() const {
    if (giaiDoan == GiaiDoan::HOANG_LOAN || giaiDoan == GiaiDoan::KET_THUC) {
        // Cảnh phim: nhìn bao quát bãi cọc, nơi thuyền giặc đâm vào
        return TAM_BAI_COC + sf::Vector2f(120.f, 0.f);
    }
    return ManChoiSong::ViTriMayAnhTheoDoi();
}

void ManChuong5::ThemMucVeRieng(std::vector<std::pair<float, std::function<void()>>>& danhSach) {
    ThemMucVeSong(danhSach);
    for (const auto& t : doi.GetThuyen()) {
        ThuyenKeDich* tau = t.get();
        danhSach.push_back({tau->GetViTri().y, [this, tau]() { tau->Ve(window); }});
    }
    danhSach.push_back({99999.f, [this]() { doi.VeMuiTen(window); }});
}

void ManChuong5::VeTrongTheGioi() {
    // Tên từ hai bờ bắn xuống thuyền giặc mắc cọc, bay theo đường cong
    if (cacTenMaiPhuc.empty()) return;
    VA va(sf::PrimitiveType::Triangles);
    for (const auto& t : cacTenMaiPhuc) {
        float u = std::min(1.f, t.tuoi / t.toiDa);
        auto diem = [&](float uu) {
            sf::Vector2f p = t.dau + (t.cuoi - t.dau) * uu;
            p.y -= 90.f * 4.f * uu * (1.f - uu);   // Đường cong parabol
            return p;
        };
        sf::Vector2f p0 = diem(u);
        sf::Vector2f p1 = diem(std::max(0.f, u - 0.06f));
        duong(va, p1, p0, 1.8f, mau(80, 52, 30));
        sf::Vector2f h = p0 - p1;
        float d = std::sqrt(h.x * h.x + h.y * h.y);
        if (d > 0.5f) {
            h /= d;
            sf::Vector2f vuong(-h.y, h.x);
            tam(va, p0 + h * 4.f, p0 + vuong * 2.2f, p0 - vuong * 2.2f, mau(210, 215, 225));
        }
    }
    window.draw(va);
}

void ManChuong5::VeThanhNuoc() const {
    // Thanh đo mực nước nằm dọc bên trái, có vùng xanh là vùng nên phát hiệu lệnh
    const float x = 280.f;
    const float y = 14.f;
    const float cao = 112.f;
    const float rong = 16.f;

    sf::RectangleShape nen({rong, cao});
    nen.setPosition({x, y});
    nen.setFillColor(sf::Color(20, 30, 44, 235));
    nen.setOutlineThickness(2.f);
    nen.setOutlineColor(sf::Color(218, 165, 32));
    window.draw(nen);

    // Mực 1.0 ở đỉnh, 0.0 ở đáy
    auto yTheoMuc = [&](float muc) { return y + cao * (1.f - muc); };

    sf::RectangleShape nuoc({rong - 4.f, cao * tiLeTrieu});
    nuoc.setPosition({x + 2.f, yTheoMuc(tiLeTrieu)});
    nuoc.setFillColor(sf::Color(60, 140, 200, 220));
    window.draw(nuoc);

    sf::RectangleShape vungXanh({rong + 10.f, yTheoMuc(MUC_NUOC_QUA_MUON) - yTheoMuc(MUC_NUOC_THAP_NHAT_DUNG)});
    vungXanh.setPosition({x - 5.f, yTheoMuc(MUC_NUOC_THAP_NHAT_DUNG)});
    float nhay = TrongCuaSo() ? 0.6f + 0.4f * std::sin(thoiGian * 8.f) : 0.45f;
    vungXanh.setFillColor(sf::Color(90, 230, 120, static_cast<std::uint8_t>(255.f * nhay * 0.55f)));
    vungXanh.setOutlineThickness(1.5f);
    vungXanh.setOutlineColor(sf::Color(120, 255, 150));
    window.draw(vungXanh);

    sf::RectangleShape kim({rong + 18.f, 3.f});
    kim.setOrigin({0.f, 1.5f});
    kim.setPosition({x - 9.f, yTheoMuc(tiLeTrieu)});
    kim.setFillColor(sf::Color(255, 255, 255));
    window.draw(kim);

    // Nhãn đặt bên phải thanh đo, chỗ trống giữa bảng thông tin và bảng nhiệm vụ
    VeChuTrai(window, nguCanh.phongChu, "Mực nước", 11, {x + 26.f, y + 2.f}, sf::Color(210, 220, 235));
    VeChuTrai(window, nguCanh.phongChu, "Vùng xanh: cọc\nđâm thủng thuyền", 10, {x + 26.f, y + 20.f}, sf::Color(150, 235, 170));
}

void ManChuong5::VeBangThongTin() const {
    const sf::Vector2f goc(12.f, 12.f);
    sf::RectangleShape khung({262.f, 112.f});
    khung.setPosition(goc);
    khung.setFillColor(sf::Color(15, 20, 28, 225));
    khung.setOutlineThickness(2.f);
    khung.setOutlineColor(sf::Color(218, 165, 32));
    window.draw(khung);

    float tiLeMau = static_cast<float>(thuyen->GetSucKhoe()) / static_cast<float>(thuyen->GetSucKhoeToiDa());
    VeChuTrai(window, nguCanh.phongChu, "Thuyền nhử: " + std::to_string(thuyen->GetSucKhoe()) + " / " + std::to_string(thuyen->GetSucKhoeToiDa()), 12,
              {goc.x + 12.f, goc.y + 8.f}, sf::Color::White);
    VeThanh(goc.x + 12.f, goc.y + 28.f, 238.f, tiLeMau, tiLeMau > 0.35f ? sf::Color(80, 190, 100) : sf::Color(220, 70, 60), sf::Color(20, 30, 44));

    float h = doi.GetDoHang();
    bool thap = h < 25.f;
    VeChuTrai(window, nguCanh.phongChu, "Độ hăng của giặc", 12, {goc.x + 12.f, goc.y + 44.f}, thap ? sf::Color(255, 150, 120) : sf::Color(255, 235, 150));
    VeThanh(goc.x + 12.f, goc.y + 64.f, 238.f, h / 100.f, thap ? sf::Color(230, 80, 60) : sf::Color(240, 190, 70), sf::Color(20, 30, 44));

    std::string dong;
    sf::Color mau = sf::Color(170, 180, 195);
    if (giaiDoan == GiaiDoan::GIU_CHAN) {
        if (TrongCuaSo()) { dong = "SPACE: PHÁT HIỆU LỆNH!"; mau = sf::Color(150, 255, 170); }
        else if (tiLeTrieu > MUC_NUOC_THAP_NHAT_DUNG) dong = "Chờ nước rút... cọc còn chìm";
        else dong = "Quá muộn, giặc thấy cọc!";
    } else if (giaiDoan == GiaiDoan::HOANG_LOAN) {
        dong = "Thuyền mắc cọc: " + std::to_string(doi.SoThuyenMacCoc()) + " / " + std::to_string(SO_THUYEN_GIAC);
        mau = sf::Color(255, 220, 130);
    }
    VeChuTrai(window, nguCanh.phongChu, dong, 12, {goc.x + 12.f, goc.y + 82.f}, mau);
}

void ManChuong5::VeThongTinPhu() {
    VeBangThongTin();
    VeThanhNuoc();

    std::vector<sf::Vector2f> diemDich;
    for (const auto& t : doi.GetThuyen()) diemDich.push_back(t->GetViTri());
    VeMinimapSong(diemDich);
}
