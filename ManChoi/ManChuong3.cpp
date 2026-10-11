#include "ManChuong3.h"
#include "../AmThanh/QuanLyAmThanh.h"
#include "../BanDo/DoHoa.h"
#include <algorithm>
#include <cmath>

using namespace dohoa;

namespace {
const sf::Vector2f VI_TRI_THUYEN_BAN_DAU(400.f, 360.f);
const sf::Vector2f VI_TRI_NGO_QUYEN(330.f, 275.f);
const sf::Vector2f DONG_CHAY(14.f, 0.f);   // Nước xuôi ra biển
}

ManChuong3::ManChuong3(NguCanhGame& ngu)
    : ManChoiSong(ngu, VI_TRI_THUYEN_BAN_DAU),
      soCocTrenThuyen(0),
      soCocTrongKho(SO_O),
      soDaCam(0),
      thoiGianConLai(THOI_GIAN_TOI_DA),
      daThoaiMoDau(false),
      daBaoGiac(false),
      giaiDoan(GiaiDoan::CHOI),
      thoiGianCanhTrieu(0.f) {
    viTriThongBao = {430.f, 520.f};   // Giữa phía dưới, tránh bảng thời gian ở góc trên trái
    dongGoiYDieuKhien = "W A S D: Chèo thuyền    E: Bốc cọc hoặc cắm cọc    Esc: Tạm dừng";

    song.DatTiLeThuyTrieu(0.f);

    // Hạm đội Nam Hán đậu xa ngoài cửa biển, chỉ để làm cảnh
    thuyenGiac.push_back(std::make_unique<ThuyenKeDich>(sf::Vector2f(2120.f, 430.f), false));
    thuyenGiac.push_back(std::make_unique<ThuyenKeDich>(sf::Vector2f(2200.f, 540.f), true));
    thuyenGiac.push_back(std::make_unique<ThuyenKeDich>(sf::Vector2f(2140.f, 650.f), false));

    DungBoSong();
    DatTuongTac();
    DatNhiemVu();

    BatDauCanhMoDau("CHƯƠNG 3: DỰNG BÃI CỌC",
        "Cửa sông Bạch Đằng, giữa hai bờ Vũ Yên và Đình Vũ. Nước triều đang xuống thấp. "
        "Ngô Quyền ra lệnh đóng bãi cọc ngầm giữa lòng sông trước khi hạm đội Nam Hán tới.", 6.f);
}

void ManChuong3::DatNhiemVu() {
    quanLyNhiemVu.DatTieuDeBang("NHIỆM VỤ CHƯƠNG 3");
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuKhamPha>(NV_NHAN_LENH, "Nhận lệnh Ngô Quyền",
        "Nghe Ngô Quyền dặn cách đóng bãi cọc giữa lòng sông."));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuKhamPha>(NV_BOC_COC, "Bốc cọc lên thuyền",
        "Chèo thuyền tới đầu bến, nhấn E để bốc cọc. Mỗi chuyến chở tối đa 8 cọc."));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVu>(NV_CAM_COC, "Cắm bãi cọc giữa sông",
        "Chèo ra khu có vòng sáng giữa sông, nhấn E để cắm vào ô gần nhất. Nước cuốn thuyền ra biển, nhớ giữ thuyền cho vững. Cần ít nhất 24 cọc.",
        LoaiNhiemVu::XAY_DUNG, SO_COC_TOI_THIEU));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuKhamPha>(NV_BAO_CAO, "Báo cáo Ngô Quyền",
        "Chèo về đầu bến, lên sát chỗ Ngô Quyền và nhấn E. Cắm thêm cọc cho kín nếu còn thời gian."));
    quanLyNhiemVu.BatDau();
}

int ManChuong3::ChiSoOGanNhat(sf::Vector2f viTri, float banKinhToiDa) const {
    int tot = -1;
    float khoangTot = banKinhToiDa;
    for (size_t i = 0; i < cacO.size(); ++i) {
        if (cacO[i].daCam) continue;
        float dx = cacO[i].viTri.x - viTri.x;
        float dy = cacO[i].viTri.y - viTri.y;
        float kc = std::sqrt(dx * dx + dy * dy);
        if (kc < khoangTot) {
            khoangTot = kc;
            tot = static_cast<int>(i);
        }
    }
    return tot;
}

void ManChuong3::DatTuongTac() {
    // Ngô Quyền đứng ở đầu cầu bến
    ThemNPC(VI_TRI_NGO_QUYEN, LoaiNPC::NGO_QUYEN, "Ngô Quyền", {"..."}, false);

    DiemTuongTac baoCao;
    baoCao.viTri = VI_TRI_NGO_QUYEN;
    baoCao.banKinh = 62.f;
    baoCao.goiY = "Báo cáo Ngô Quyền";
    baoCao.goiYKhiKhoa = "Ngô Quyền";
    baoCao.docCao = 70.f;
    baoCao.dangMo = [this]() { return MaNhiemVuHienTai() == NV_BAO_CAO && giaiDoan == GiaiDoan::CHOI; };
    baoCao.hanhDong = [this]() {
        int sao = SoSao();
        std::string nhanXet;
        if (sao >= 3) nhanXet = "Bãi cọc kín đủ cả hàng, thuyền giặc khó mà lách qua.";
        else if (sao == 2) nhanXet = "Khá vững. Còn vài kẽ hở nhưng đủ dùng.";
        else nhanXet = "Tạm đủ. Giặc có thể lách qua kẽ hở, ta phải bù bằng mưu nhử.";

        BatDauHoiThoai({
            {"Ngô Quyền", "Bãi cọc đã dựng xong: " + std::to_string(soDaCam) + " trên " + std::to_string(SO_O) + " ô. " + nhanXet, LoaiNPC::NGO_QUYEN},
            {"Ngô Quyền", "Giờ chỉ còn chờ triều lên che kín đầu cọc. Hạm đội Nam Hán sẽ không thấy gì cho tới khi nước rút.", LoaiNPC::NGO_QUYEN}
        }, [this]() { BatDauCanhTrieuLen(); });
    };
    ThemDiemTuongTac(baoCao);

    // Bốc cọc ở đầu bến
    DiemTuongTac boc;
    boc.viTri = DIEM_BOC_COC;
    boc.banKinh = 62.f;
    boc.goiY = "Bốc cọc";
    boc.goiYKhiKhoa = "Kho cọc";
    boc.docCao = 50.f;
    boc.dangMo = [this]() {
        return MaNhiemVuHienTai() >= NV_BOC_COC && giaiDoan == GiaiDoan::CHOI &&
               soCocTrenThuyen < SUC_CHUA && soCocTrongKho > 0;
    };
    boc.hanhDong = [this]() { BocCoc(); };
    ThemDiemTuongTac(boc);

    // Mỗi ô cọc là một điểm tương tác: ô nào gần thuyền nhất sẽ hiện gợi ý
    for (size_t i = 0; i < cacO.size(); ++i) {
        DiemTuongTac o;
        o.viTri = cacO[i].viTri;
        o.banKinh = 40.f;
        o.goiY = "Cắm cọc";
        o.docCao = 62.f;
        o.dangMo = [this, i]() { return !cacO[i].daCam && soCocTrenThuyen > 0 && giaiDoan == GiaiDoan::CHOI && MaNhiemVuHienTai() >= NV_CAM_COC; };
        o.hanhDong = [this, i]() { CamCoc(static_cast<int>(i)); };
        ThemDiemTuongTac(o);
    }
}

void ManChuong3::BocCoc() {
    int them = std::min(SUC_CHUA - soCocTrenThuyen, soCocTrongKho);
    if (them <= 0) return;

    soCocTrenThuyen += them;
    soCocTrongKho -= them;
    thoiGianKhoaDieuKhien = 0.35f;
    nguCanh.amThanh.PhatTiengNhatDo();
    ThemChuBay(thuyen->GetViTri() + sf::Vector2f(0.f, -50.f), "+" + std::to_string(them) + " cọc", sf::Color(255, 220, 140));

    if (MaNhiemVuHienTai() == NV_BOC_COC) {
        quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
        HienThongBao("Chèo ra khu có vòng sáng giữa sông, nhấn E để cắm cọc", 6.f);
    }
}

void ManChuong3::CamCoc(int chiSoO) {
    if (chiSoO < 0 || chiSoO >= static_cast<int>(cacO.size()) || cacO[chiSoO].daCam) return;

    cacO[chiSoO].daCam = true;
    sf::Vector2f p = cacO[chiSoO].viTri;
    cocDaCam.emplace_back(sf::Vector2f(p.x, p.y - 22.f));
    --soCocTrenThuyen;
    ++soDaCam;
    thoiGianKhoaDieuKhien = 0.3f;

    quanLyGiaoDien.ThemHieuUngNuocBan(p);
    mayAnh.Rung(2.f, 0.1f);
    nguCanh.amThanh.PhatTiengVaChamCoc();

    if (MaNhiemVuHienTai() == NV_CAM_COC) {
        quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
        if (MaNhiemVuHienTai() == NV_BAO_CAO) {
            HienThongBao("Đủ cọc tối thiểu. Cắm thêm cho kín hoặc về báo Ngô Quyền", 7.f);
        }
    }
}

int ManChuong3::SoSao() const {
    if (soDaCam >= SO_O) return 3;
    if (soDaCam >= 28) return 2;
    if (soDaCam >= SO_COC_TOI_THIEU) return 1;
    return 0;
}

void ManChuong3::CapNhatTrieu() {
    // Nước đứng yên ở mức thấp, tới những giây cuối thì dâng dần
    if (giaiDoan == GiaiDoan::CHOI) {
        float t = 1.f - std::min(1.f, thoiGianConLai / THOI_GIAN_NUOC_DANG);
        tiLeTrieu = 0.5f * t * t;
    }
    song.DatTiLeThuyTrieu(tiLeTrieu);
}

void ManChuong3::BatDauCanhTrieuLen() {
    if (giaiDoan == GiaiDoan::TRIEU_LEN) return;
    giaiDoan = GiaiDoan::TRIEU_LEN;
    thoiGianCanhTrieu = 0.f;
    quanLyCanhPhim.BatDauCanhTuyChinh("THỦY TRIỀU DÂNG",
        "Nước lên che kín bãi cọc. Từ mặt sông, không ai thấy dưới lòng sông có gì. Chỉ khi nước rút, bãi cọc mới lộ ra.", 7.f);
}

void ManChuong3::KetThucChuong() {
    int sao = SoSao();
    std::string danhGia = "Bãi cọc: " + std::to_string(soDaCam) + " trên " + std::to_string(SO_O) + " ô, " + std::to_string(sao) + " sao.\n";
    HoanThanhChuong("CHƯƠNG 3: DỰNG BÃI CỌC",
        danhGia + "Thủy triều ở cửa sông Bạch Đằng lên xuống rất mạnh, đó là yếu tố then chốt của kế sách. "
        "Theo sử cũ, cọc được đóng ngầm dưới lòng sông: nước lên thì che khuất, nước rút thì lộ ra chặn thuyền giặc.");
}

void ManChuong3::CapNhatRieng(float deltaTime) {
    song.CapNhat(deltaTime);
    for (auto& t : thuyenGiac) t->CapNhat(deltaTime);

    if (!daThoaiMoDau && !quanLyCanhPhim.IsDangChieuPhim()) {
        daThoaiMoDau = true;
        BatDauHoiThoai({
            {"Ngô Quyền", "Nước triều đang xuống thấp. Đây là lúc đóng cọc.", LoaiNPC::NGO_QUYEN},
            {"Ngô Quyền", "Bãi cọc đặt giữa lòng sông, cắm so le từng hàng cho khít để thuyền giặc không lách qua được.", LoaiNPC::NGO_QUYEN},
            {"Ngô Quyền", "Mỗi chuyến thuyền chở được tám cọc. Cần cắm ít nhất hai mươi bốn cọc, càng đủ càng tốt, trước khi nước dâng.", LoaiNPC::NGO_QUYEN},
            {"Ngô Quyền", "Hai bên bờ, quân ta đã mai phục sẵn trong rừng. Nước sông cuốn ra biển rất mạnh, giữ thuyền cho vững. Đi đi.", LoaiNPC::NGO_QUYEN}
        }, [this]() {
            quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
            HienThongBao("Chèo thuyền tới đầu bến, nhấn E để bốc cọc", 6.f);
        });
    }

    if (giaiDoan == GiaiDoan::CHOI) {
        CapNhatThuyenSong(deltaTime, duocDieuKhienKhung, duocDieuKhienKhung ? DONG_CHAY : sf::Vector2f(0.f, 0.f),
                          1.f - 0.03f * static_cast<float>(soCocTrenThuyen));

        // Đồng hồ chỉ chạy khi người chơi đã nhận lệnh và đang được điều khiển
        if (MaNhiemVuHienTai() >= NV_BOC_COC && duocDieuKhienKhung) {
            thoiGianConLai -= deltaTime;
            if (thoiGianConLai <= 20.f && !daBaoGiac) {
                daBaoGiac = true;
                HienThongBao("Trống báo: thuyền giặc đã thấy ở cửa biển!", 6.f);
                nguCanh.amThanh.PhatTiengTrongTran();
            }
            if (thoiGianConLai <= 0.f) {
                thoiGianConLai = 0.f;
                if (soDaCam >= SO_COC_TOI_THIEU) {
                    BatDauCanhTrieuLen();
                } else {
                    BaoThatBai("Nước đã dâng mà bãi cọc mới có " + std::to_string(soDaCam) + " trên " + std::to_string(SO_COC_TOI_THIEU) +
                               " cọc cần thiết. Hãy thử lại: chở đủ 8 cọc mỗi chuyến và đừng để dòng nước cuốn thuyền đi quá xa.");
                }
            }
        }

        // Hạm đội giặc từ từ tiến lại gần khi sắp hết giờ
        if (thoiGianConLai < 90.f && MaNhiemVuHienTai() >= NV_BOC_COC) {
            for (auto& t : thuyenGiac) {
                sf::Vector2f p = t->GetViTri();
                if (p.x > 1840.f) t->SetViTri({p.x - 6.f * deltaTime, p.y});
            }
        }
        CapNhatTrieu();
    } else {
        // Cảnh nước dâng: mực nước lên hết, camera lùi ra cho thấy cả bãi cọc
        thoiGianCanhTrieu += deltaTime;
        float t = std::min(1.f, thoiGianCanhTrieu / 6.f);
        tiLeTrieu = std::max(tiLeTrieu, t);
        song.DatTiLeThuyTrieu(tiLeTrieu);
        mayAnh.DatZoom(1.f - 0.28f * std::min(1.f, thoiGianCanhTrieu / 3.f));
        thuyen->DungLai();
        thuyen->CapNhat(deltaTime);

        if (!quanLyCanhPhim.IsDangChieuPhim() && thoiGianCanhTrieu > 0.5f) {
            tiLeTrieu = 1.f;
            song.DatTiLeThuyTrieu(1.f);
            KetThucChuong();
        }
    }
}

sf::Vector2f ManChuong3::ViTriMayAnhTheoDoi() const {
    if (giaiDoan == GiaiDoan::TRIEU_LEN) return TAM_BAI_COC;
    // Nhìn lệch về phía trước thuyền một chút cho dễ thấy chỗ sắp tới
    sf::Vector2f van = thuyen->GetVanToc();
    return thuyen->GetViTri() + sf::Vector2f(van.x * 0.35f, van.y * 0.35f);
}

void ManChuong3::ThemMucVeRieng(std::vector<std::pair<float, std::function<void()>>>& danhSach) {
    // Vòng sáng đánh dấu bãi cọc và các ô còn trống (vẽ ngay trên mặt nước)
    danhSach.push_back({-1000.f, [this]() {
        VA va(sf::PrimitiveType::Triangles);
        float nhay = 0.5f + 0.5f * std::sin(thoiGian * 3.f);
        float tl = 1.f - tiLeTrieu;   // Nước dâng thì vòng mờ đi
        int gan = ChiSoOGanNhat(thuyen->GetViTri(), 46.f);

        // Khung bãi cọc
        const float x0 = 950.f, y0 = 418.f, x1 = 1480.f, y1 = 640.f;
        Color vien = mau(255, 215, 0, static_cast<float>(80 + 50 * nhay) * tl);
        duong(va, {x0, y0}, {x1, y0}, 2.f, vien);
        duong(va, {x1, y0}, {x1, y1}, 2.f, vien);
        duong(va, {x1, y1}, {x0, y1}, 2.f, vien);
        duong(va, {x0, y1}, {x0, y0}, 2.f, vien);

        for (size_t i = 0; i < cacO.size(); ++i) {
            if (cacO[i].daCam) continue;
            bool lanCan = (static_cast<int>(i) == gan);
            Color c = lanCan ? mau(255, 240, 150, 230.f * tl) : mau(255, 230, 140, (60.f + 40.f * nhay) * tl);
            float r = lanCan ? 13.f : 9.f;
            elip(va, cacO[i].viTri.x, cacO[i].viTri.y, r, r * 0.45f, c, 16);
        }
        window.draw(va);
    }});

    // Cầu bến
    danhSach.push_back({0.f, [this]() { window.draw(mangBen); }});

    // Cọc đã cắm, chìm dần khi nước dâng
    for (auto& coc : cocDaCam) {
        Coc* c = &coc;
        danhSach.push_back({c->GetViTri().y + 22.f, [this, c]() { c->Ve(window, tiLeTrieu); }});
    }

    // Thuyền người chơi và cọc xếp trên thuyền
    danhSach.push_back({thuyen->GetViTri().y, [this]() { thuyen->Ve(window); }});
    if (soCocTrenThuyen > 0) {
        danhSach.push_back({thuyen->GetViTri().y + 0.1f, [this]() {
            VA va(sf::PrimitiveType::Triangles);
            sf::Vector2f p = thuyen->GetViTri();
            for (int k = 0; k < soCocTrenThuyen; ++k) {
                float x = p.x - 24.f + static_cast<float>(k) * 6.5f;
                float y = p.y - 4.f - static_cast<float>(k % 2) * 3.f;
                duong(va, {x, y}, {x + 3.f, y - 26.f}, 4.f, mau(122, 68, 42));
                tam(va, {x + 1.f, y - 25.f}, {x + 5.f, y - 25.f}, {x + 3.5f, y - 33.f}, mau(186, 198, 216));
            }
            window.draw(va);
        }});
    }

    // Hạm đội giặc ngoài cửa biển
    for (auto& t : thuyenGiac) {
        ThuyenKeDich* tau = t.get();
        danhSach.push_back({tau->GetViTri().y, [this, tau]() { tau->Ve(window); }});
    }
}

void ManChuong3::VeBangThongTin() const {
    const sf::Vector2f goc(12.f, 12.f);
    sf::RectangleShape khung({262.f, 118.f});
    khung.setPosition(goc);
    khung.setFillColor(sf::Color(15, 20, 28, 225));
    khung.setOutlineThickness(2.f);
    khung.setOutlineColor(sf::Color(218, 165, 32));
    window.draw(khung);

    auto thanh = [&](float y, float tiLe, sf::Color mau, sf::Color nenMau) {
        sf::RectangleShape nenThanh({238.f, 10.f});
        nenThanh.setPosition({goc.x + 12.f, y});
        nenThanh.setFillColor(nenMau);
        nenThanh.setOutlineThickness(1.f);
        nenThanh.setOutlineColor(sf::Color(90, 100, 120));
        window.draw(nenThanh);
        sf::RectangleShape day({238.f * std::max(0.f, std::min(1.f, tiLe)), 10.f});
        day.setPosition({goc.x + 12.f, y});
        day.setFillColor(mau);
        window.draw(day);
    };

    // Thời gian còn lại trước khi nước dâng
    int giay = static_cast<int>(std::ceil(thoiGianConLai));
    std::string gio = std::to_string(giay / 60) + ":" + (giay % 60 < 10 ? "0" : "") + std::to_string(giay % 60);
    bool gap = thoiGianConLai < 45.f;
    VeChuTrai(window, nguCanh.phongChu, "Nước dâng sau: " + gio, 12, {goc.x + 12.f, goc.y + 8.f}, gap ? sf::Color(255, 150, 120) : sf::Color(255, 235, 150));
    thanh(goc.y + 28.f, thoiGianConLai / THOI_GIAN_TOI_DA, gap ? sf::Color(230, 90, 60) : sf::Color(70, 170, 220), sf::Color(20, 30, 44));

    // Số cọc đã cắm so với mức tối thiểu và tổng số ô
    VeChuTrai(window, nguCanh.phongChu, "Bãi cọc: " + std::to_string(soDaCam) + " / " + std::to_string(SO_O) + "   (cần " + std::to_string(SO_COC_TOI_THIEU) + ")", 12,
              {goc.x + 12.f, goc.y + 46.f}, soDaCam >= SO_COC_TOI_THIEU ? sf::Color(150, 235, 160) : sf::Color::White);
    thanh(goc.y + 66.f, static_cast<float>(soDaCam) / static_cast<float>(SO_O), soDaCam >= SO_COC_TOI_THIEU ? sf::Color(120, 210, 120) : sf::Color(230, 190, 70), sf::Color(20, 30, 44));
    // Vạch mốc 24 cọc
    sf::RectangleShape moc({2.f, 14.f});
    moc.setPosition({goc.x + 12.f + 238.f * static_cast<float>(SO_COC_TOI_THIEU) / static_cast<float>(SO_O) - 1.f, goc.y + 64.f});
    moc.setFillColor(sf::Color(255, 255, 255, 200));
    window.draw(moc);

    VeChuTrai(window, nguCanh.phongChu, "Cọc trên thuyền: " + std::to_string(soCocTrenThuyen) + " / " + std::to_string(SUC_CHUA) +
              "     Kho còn: " + std::to_string(soCocTrongKho), 12, {goc.x + 12.f, goc.y + 90.f}, sf::Color(205, 215, 230));
}

void ManChuong3::VeThongTinPhu() {
    VeBangThongTin();
    VeMinimapSong({});
}
