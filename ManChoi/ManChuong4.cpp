#include "ManChuong4.h"
#include "../AmThanh/QuanLyAmThanh.h"
#include "../BanDo/DoHoa.h"
#include <algorithm>
#include <cmath>

using namespace dohoa;

namespace {
const sf::Vector2f VI_TRI_THUYEN_BAN_DAU(1600.f, 520.f);
const float X_DUNG_DO_XET = 1880.f;
}

ManChuong4::ManChuong4(NguCanhGame& ngu)
    : ManChoiSong(ngu, VI_TRI_THUYEN_BAN_DAU),
      soThuyenDaQua(0),
      daThoaiMoDau(false),
      daBaoKetThuc(false) {
    viTriThongBao = {430.f, 520.f};
    dongGoiYDieuKhien = "W A S D: Chèo thuyền    Q: Khiêu khích    Esc: Tạm dừng";

    DungBoSong();
    CamTatCaCoc();               // Bãi cọc đã dựng xong ở chương 3
    tiLeTrieu = 1.f;             // Triều cao: cọc chìm dưới nước
    song.DatTiLeThuyTrieu(1.f);
    thuyen->DatCheDoMuot(true);

    // Hạm đội Nam Hán vào từ phía biển (bên phải), chủ tướng đi giữa
    doi.ThemThuyen({2200.f, 430.f}, false);
    doi.ThemThuyen({2330.f, 545.f}, true);
    doi.ThemThuyen({2240.f, 650.f}, false);
    doi.ThemThuyen({2420.f, 470.f}, false);
    doi.DatDiemDungDoXet(X_DUNG_DO_XET);
    doi.DatCheDo(DoiThuyenGiac::CheDo::TIEN_VAO);
    daQuaBaiCoc.assign(SO_THUYEN_GIAC, false);

    DatNhiemVu();

    BatDauCanhMoDau("CHƯƠNG 4: NHỬ ĐỊCH VÀO SÂU",
        "Nước triều đang lên cao, che kín bãi cọc dưới lòng sông. Hạm đội Nam Hán tiến vào cửa sông Bạch Đằng. "
        "Thuyền nhẹ của Ngô Quyền ra khiêu chiến rồi giả thua, dẫn giặc ngược dòng.", 6.f);
}

void ManChuong4::DatNhiemVu() {
    quanLyNhiemVu.DatTieuDeBang("NHIỆM VỤ CHƯƠNG 4");
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuKhamPha>(NV_KHIEU_CHIEN, "Khiêu chiến",
        "Chèo về phía hạm đội Nam Hán ở cửa biển rồi nhấn Q để hô khiêu chiến."));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVu>(NV_NHU_GIAC, "Dẫn giặc qua bãi cọc",
        "Chèo ngược sông về phía tây. Giữ giặc cách thuyền từ 130 đến 480 bước: gần quá thì bị đâm và bắn, xa quá thì giặc nghi ngờ rút lui. Nhấn Q khi giặc hết hăng.",
        LoaiNhiemVu::KHAM_PHA, SO_THUYEN_GIAC));
    quanLyNhiemVu.BatDau();
}

void ManChuong4::XuLySuKien(const sf::Event& suKien) {
    ManChoiSong::XuLySuKien(suKien);

    if (const auto* phim = suKien.getIf<sf::Event::KeyPressed>()) {
        if (phim->code == sf::Keyboard::Key::Q && duocDieuKhienKhung && !daKetThuc && thoiGianKhoaDieuKhien <= 0.f) {
            KhieuKhich();
        }
    }
}

void ManChuong4::KhieuKhich() {
    if (doi.GetCheDo() == DoiThuyenGiac::CheDo::TIEN_VAO && !doi.DaToiDiemDung()) {
        HienThongBao("Giặc còn đang tiến vào cửa sông, chờ chúng tới gần", 3.f);
        return;
    }
    if (!doi.KhieuKhich()) return;

    ThemChuBay(thuyen->GetViTri() + sf::Vector2f(0.f, -55.f), "Giặc kia, lại đây!", sf::Color(255, 220, 120));
    nguCanh.amThanh.PhatTiengTrongTran();
    mayAnh.Rung(2.f, 0.15f);
}

void ManChuong4::KiemTraThuyenQuaBaiCoc() {
    const auto& cacThuyen = doi.GetThuyen();
    for (size_t i = 0; i < cacThuyen.size(); ++i) {
        if (daQuaBaiCoc[i]) continue;
        if (cacThuyen[i]->GetViTri().x < X_MEP_TAY_BAI_COC) {
            daQuaBaiCoc[i] = true;
            ++soThuyenDaQua;
            ThemChuBay(cacThuyen[i]->GetViTri() + sf::Vector2f(0.f, -50.f),
                       "Qua bãi cọc " + std::to_string(soThuyenDaQua) + "/" + std::to_string(SO_THUYEN_GIAC), sf::Color(150, 235, 160));
            nguCanh.amThanh.PhatTiengNhatDo();
            if (MaNhiemVuHienTai() == NV_NHU_GIAC) quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
        }
    }
}

void ManChuong4::XuLyKetQuaDoi(const KetQuaDoiThuyen& ketQua) {
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
}

void ManChuong4::CapNhatRieng(float deltaTime) {
    song.CapNhat(deltaTime);

    if (!daThoaiMoDau && !quanLyCanhPhim.IsDangChieuPhim()) {
        daThoaiMoDau = true;
        BatDauHoiThoai({
            {"Ngô Quyền", "Nước triều đang lên cao, che kín bãi cọc. Giặc không thể thấy gì dưới lòng sông.", LoaiNPC::NGO_QUYEN},
            {"Ngô Quyền", "Hạm đội Nam Hán đã vào cửa sông. Ngươi lái thuyền nhẹ ra khiêu chiến, rồi giả thua, dẫn chúng ngược dòng qua khỏi bãi cọc.", LoaiNPC::NGO_QUYEN},
            {"Ngô Quyền", "Giữ giặc cách thuyền vừa phải: gần quá thì bị đâm và bắn, xa quá thì giặc ngờ vực mà rút lui. Khi giặc hết hăng, hô khiêu chiến để kéo chúng lại.", LoaiNPC::NGO_QUYEN}
        });
    }

    bool dangKichBan = DangHoiThoai() || quanLyCanhPhim.IsDangChieuPhim() || daBaoKetThuc;

    CapNhatThuyenSong(deltaTime, duocDieuKhienKhung && !daBaoKetThuc, {0.f, 0.f}, 1.f);

    if (!dangKichBan) {
        KetQuaDoiThuyen ketQua = doi.CapNhat(deltaTime, *thuyen, cocDaCam, tiLeTrieu);
        XuLyKetQuaDoi(ketQua);

        // Giặc hăng đuổi theo rồi thì nhiệm vụ khiêu chiến xong
        if (MaNhiemVuHienTai() == NV_KHIEU_CHIEN && doi.GetCheDo() == DoiThuyenGiac::CheDo::DUOI) {
            quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
            HienThongBao("Giặc đã đuổi theo! Dẫn chúng ngược sông, qua khỏi bãi cọc", 6.f);
        }
        KiemTraThuyenQuaBaiCoc();

        if (thuyen->IsDaBiPhaHuy()) {
            BaoThatBai("Thuyền nhẹ đã bị phá hủy. Hãy thử lại: né tên bằng cách đổi hướng liên tục, đừng để thuyền giặc áp sát.");
            return;
        }
        if (doi.GetCheDo() == DoiThuyenGiac::CheDo::DUOI && doi.MatHung()) {
            BaoThatBai("Giặc ngờ vực nên không đuổi theo nữa. Hãy thử lại: đừng chèo quá xa, và hô khiêu chiến khi thanh độ hăng xuống thấp.");
            return;
        }
    }

    // Cả hạm đội đã qua bãi cọc
    if (!daBaoKetThuc && soThuyenDaQua >= SO_THUYEN_GIAC) {
        daBaoKetThuc = true;
        BatDauHoiThoai({
            {"Ngô Quyền", "Cả bốn chiến thuyền đã qua bãi cọc. Giặc đã vào sâu trong lòng sông rồi.", LoaiNPC::NGO_QUYEN},
            {"Ngô Quyền", "Thuyền nhẹ rút vào lau sậy. Quân ta đã vào vị trí hai bên bờ. Giờ chỉ còn chờ nước rút.", LoaiNPC::NGO_QUYEN}
        }, [this]() {
            HoanThanhChuong("CHƯƠNG 4: NHỬ ĐỊCH VÀO SÂU",
                "Theo sử cũ, Ngô Quyền cho thuyền nhẹ ra khiêu chiến rồi giả thua rút chạy để nhử quân Nam Hán đuổi theo vào sâu trong sông, "
                "đúng lúc nước triều đang lên và che kín bãi cọc. Nhờ vậy thuyền giặc không hay biết gì về cái bẫy dưới lòng sông.");
        });
    }
}

void ManChuong4::ThemMucVeRieng(std::vector<std::pair<float, std::function<void()>>>& danhSach) {
    ThemMucVeSong(danhSach);
    for (const auto& t : doi.GetThuyen()) {
        ThuyenKeDich* tau = t.get();
        danhSach.push_back({tau->GetViTri().y, [this, tau]() { tau->Ve(window); }});
    }
    danhSach.push_back({99999.f, [this]() { doi.VeMuiTen(window); }});
}

void ManChuong4::VeBangThongTin() const {
    const sf::Vector2f goc(12.f, 12.f);
    sf::RectangleShape khung({262.f, 112.f});
    khung.setPosition(goc);
    khung.setFillColor(sf::Color(15, 20, 28, 225));
    khung.setOutlineThickness(2.f);
    khung.setOutlineColor(sf::Color(218, 165, 32));
    window.draw(khung);

    // Máu thuyền nhẹ
    float tiLeMau = static_cast<float>(thuyen->GetSucKhoe()) / static_cast<float>(thuyen->GetSucKhoeToiDa());
    VeChuTrai(window, nguCanh.phongChu, "Thuyền nhẹ: " + std::to_string(thuyen->GetSucKhoe()) + " / " + std::to_string(thuyen->GetSucKhoeToiDa()), 12,
              {goc.x + 12.f, goc.y + 8.f}, sf::Color::White);
    VeThanh(goc.x + 12.f, goc.y + 28.f, 238.f, tiLeMau, tiLeMau > 0.35f ? sf::Color(80, 190, 100) : sf::Color(220, 70, 60), sf::Color(20, 30, 44));

    // Độ hăng của giặc
    float h = doi.GetDoHang();
    bool thap = h < 25.f;
    VeChuTrai(window, nguCanh.phongChu, "Độ hăng của giặc", 12, {goc.x + 12.f, goc.y + 44.f}, thap ? sf::Color(255, 150, 120) : sf::Color(255, 235, 150));
    VeThanh(goc.x + 12.f, goc.y + 64.f, 238.f, h / 100.f, thap ? sf::Color(230, 80, 60) : sf::Color(240, 190, 70), sf::Color(20, 30, 44));

    // Khiêu khích
    float hoi = doi.GetHoiKhieuKhich();
    std::string trangThai = (hoi <= 0.f) ? "Q: Khiêu khích (sẵn sàng)" : "Q: Khiêu khích (còn " + std::to_string(static_cast<int>(std::ceil(hoi))) + " giây)";
    VeChuTrai(window, nguCanh.phongChu, trangThai, 12, {goc.x + 12.f, goc.y + 82.f}, hoi <= 0.f ? sf::Color(150, 235, 160) : sf::Color(170, 180, 195));
}

void ManChuong4::VeThongTinPhu() {
    VeBangThongTin();

    std::vector<sf::Vector2f> diemDich;
    for (const auto& t : doi.GetThuyen()) diemDich.push_back(t->GetViTri());
    VeMinimapSong(diemDich);
}
