#include "ManChuong1.h"
#include "../AmThanh/QuanLyAmThanh.h"
#include "../BanDo/DoHoa.h"
#include <algorithm>
#include <cmath>

using namespace dohoa;

namespace {
const float RONG_THE_GIOI = 1500.f;
const float CAO_THE_GIOI = 1000.f;
const sf::FloatRect VUNG_SONG({1380.f, 0.f}, {120.f, 1000.f});

// Khoảng cách từ điểm tới đoạn thẳng, dùng để không đặt cây lên đường đi
float KhoangCachDenDoan(sf::Vector2f p, sf::Vector2f a, sf::Vector2f b) {
    sf::Vector2f ab = b - a;
    float dai2 = ab.x * ab.x + ab.y * ab.y;
    float t = (dai2 < 0.001f) ? 0.f : ((p.x - a.x) * ab.x + (p.y - a.y) * ab.y) / dai2;
    t = std::max(0.f, std::min(1.f, t));
    sf::Vector2f g = a + ab * t;
    return std::sqrt((p.x - g.x) * (p.x - g.x) + (p.y - g.y) * (p.y - g.y));
}
}

ManChuong1::ManChuong1(NguCanhGame& ngu)
    : ManChoiDiBo(ngu, RONG_THE_GIOI, CAO_THE_GIOI, {170.f, 880.f}),
      khuDoanhTrai({340.f, 200.f}, {880.f, 560.f}),
      daHienHuongDan(false),
      daNhacDiLuyen(false) {
    nguoiChoi.SetCoKiem(false);                             // Chưa có kiếm, phải ra giá binh khí lấy
    nguoiChoi.DatGioiHan({{30.f, 50.f}, {1320.f, 920.f}});  // Không lội xuống sông

    DungBanDo();
    DatNhanVatVaTuongTac();
    DatNhiemVu();

    BatDauCanhMoDau("CHƯƠNG 1: HỘI KIẾN NGÔ QUYỀN",
        "Năm 938, Giao Châu vừa qua cơn biến loạn. Kiều Công Tiễn đã bị diệt, nhưng thủy quân Nam Hán đang vượt biển kéo sang. "
        "Tại doanh trại Từ Lương Xâm, Ngô Quyền triệu tập tướng sĩ.", 6.f);
}

void ManChuong1::DungBanDo() {
    // ----- Nền tĩnh -----
    const std::vector<std::vector<sf::Vector2f>> cacDuong = {
        {{150.f, 880.f}, {420.f, 830.f}, {640.f, 790.f}, {780.f, 745.f}, {780.f, 560.f}},
        {{780.f, 745.f}, {900.f, 820.f}, {1000.f, 860.f}, {1100.f, 850.f}},
        {{780.f, 520.f}, {620.f, 500.f}},
        {{780.f, 520.f}, {940.f, 520.f}},
        {{780.f, 520.f}, {780.f, 440.f}}
    };

    BatDauVeNen();
    SonNenCo(nenBanDo, {{0.f, 0.f}, {RONG_THE_GIOI, CAO_THE_GIOI}}, 3, sf::Color(54, 108, 52), sf::Color(98, 156, 76));

    // Sân doanh trại: đất nén, viền mờ dần
    {
        VA va(sf::PrimitiveType::Triangles);
        elip(va, 780.f, 480.f, 440.f, 290.f, mau(168, 140, 96, 38), 40);
        elip(va, 780.f, 480.f, 400.f, 260.f, mau(168, 140, 96, 52), 40);
        elip(va, 780.f, 480.f, 350.f, 225.f, mau(168, 140, 96, 66), 40);
        nenBanDo.draw(va);
    }
    for (const auto& duong : cacDuong) SonDuongDat(nenBanDo, duong, duong.size() <= 2 ? 36.f : 54.f);
    SonSongBenPhai(nenBanDo, VUNG_SONG, 5);
    KetThucVeNen();

    // ----- Công trình trong doanh trại -----
    ThemVat(LoaiVat::LEU, {780.f, 350.f}, 1.f, 2);   // Lều chỉ huy của Ngô Quyền
    ThemVat(LoaiVat::LEU, {520.f, 330.f}, 1.f, 0);
    ThemVat(LoaiVat::LEU, {1040.f, 330.f}, 1.f, 1);
    ThemVat(LoaiVat::LEU, {540.f, 600.f}, 1.f, 1);
    ThemVat(LoaiVat::LEU, {1020.f, 610.f}, 1.f, 0);
    ThemVat(LoaiVat::DONG_LUA, {690.f, 530.f});
    ThemVat(LoaiVat::DONG_LUA, {880.f, 530.f});
    ThemVat(LoaiVat::CHOI_CANH, {1220.f, 420.f});
    ThemVat(LoaiVat::GIA_VU_KHI, {610.f, 480.f});
    ThemVat(LoaiVat::COT_CO, {700.f, 712.f});
    ThemVat(LoaiVat::COT_CO, {860.f, 712.f});
    ThemVat(LoaiVat::THUNG, {440.f, 680.f});
    ThemVat(LoaiVat::THUNG, {470.f, 676.f});
    ThemVat(LoaiVat::THUNG, {1130.f, 676.f});
    ThemVat(LoaiVat::THUNG, {660.f, 300.f});
    ThemVat(LoaiVat::THUNG, {900.f, 300.f});

    // Hàng rào bắc và nam, chừa cổng ở giữa phía nam
    for (int i = 0; i < 10; ++i) ThemVat(LoaiVat::HANG_RAO, {420.f + static_cast<float>(i) * 80.f, 235.f});
    const float rangNam[] = {440.f, 520.f, 600.f, 670.f, 890.f, 970.f, 1050.f, 1130.f};
    for (float x : rangNam) ThemVat(LoaiVat::HANG_RAO, {x, 725.f});

    // Bãi tập phía nam doanh trại
    chiSoBuNhin.push_back(ThemVat(LoaiVat::BU_NHIN, {940.f, 870.f}));
    chiSoBuNhin.push_back(ThemVat(LoaiVat::BU_NHIN, {1040.f, 890.f}));
    chiSoBuNhin.push_back(ThemVat(LoaiVat::BU_NHIN, {1140.f, 870.f}));

    // Thuyền neo bên bờ sông
    ThemVat(LoaiVat::THUYEN_NEO, {1425.f, 260.f}, 1.f, 1);
    ThemVat(LoaiVat::THUYEN_NEO, {1440.f, 640.f}, 1.1f, 2);
    ThemVat(LoaiVat::THUYEN_NEO, {1415.f, 880.f}, 1.f, 3);

    // ----- Cây cối, bụi, đá rải ngoài doanh trại -----
    Rng rng(938u);
    std::vector<sf::Vector2f> daDat;
    int soCay = 0;
    for (int thu = 0; thu < 2600 && soCay < 78; ++thu) {
        sf::Vector2f p(rng.range(40.f, 1340.f), rng.range(70.f, CAO_THE_GIOI - 30.f));

        // Bỏ qua chỗ trong doanh trại, bãi tập, chỗ xuất phát
        sf::FloatRect khuLoai({300.f, 170.f}, {960.f, 650.f});
        sf::FloatRect khuTap({880.f, 780.f}, {340.f, 190.f});
        if (khuLoai.contains(p) || khuTap.contains(p)) continue;
        if (std::hypot(p.x - 170.f, p.y - 880.f) < 130.f) continue;

        bool gan = false;
        for (const auto& duong : cacDuong) {
            for (size_t i = 0; i + 1 < duong.size(); ++i) {
                if (KhoangCachDenDoan(p, duong[i], duong[i + 1]) < 62.f) gan = true;
            }
        }
        for (const auto& q : daDat) {
            if (std::hypot(p.x - q.x, p.y - q.y) < 62.f) gan = true;
        }
        if (gan) continue;

        daDat.push_back(p);
        ++soCay;
        if (soCay % 5 == 0) ThemVat(LoaiVat::DA, p, 0.8f + rng.f() * 0.5f);
        else if (soCay % 3 == 0) ThemVat(LoaiVat::BUI, p, 0.9f + rng.f() * 0.4f, rng.irange(0, 5));
        else ThemVat(LoaiVat::CAY, p, 0.85f + rng.f() * 0.4f, rng.irange(0, 5));
    }

    CapNhatVatCan();
}

void ManChuong1::DatNhiemVu() {
    quanLyNhiemVu.DatTieuDeBang("NHIỆM VỤ CHƯƠNG 1");
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuKhamPha>(NV_VAO_DOANH_TRAI, "Vào doanh trại",
        "Đi theo con đường đất vào doanh trại Từ Lương Xâm. Dùng W A S D để đi, giữ Shift để chạy."));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuKhamPha>(NV_HOI_KIEN, "Hội kiến Ngô Quyền",
        "Đến gần Ngô Quyền ở lều lớn giữa doanh trại và nhấn E để nói chuyện."));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuThuThap>(NV_NHAN_KIEM, "Nhận kiếm",
        "Ra giá binh khí phía tây doanh trại, nhấn E để lấy kiếm.", ID_KIEM, 1));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuChienDau>(NV_LUYEN_KIEM, "Luyện kiếm",
        "Ra bãi tập phía nam, chém bù nhìn bằng Space hoặc J. Cần 6 nhát trúng.", 6));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuKhamPha>(NV_BAO_CAO, "Báo lại Ngô Quyền",
        "Quay về lều chỉ huy, nhấn E để báo với Ngô Quyền."));
    quanLyNhiemVu.BatDau();
}

void ManChuong1::DatNhanVatVaTuongTac() {
    // Những người có thể trò chuyện tự do, mỗi lần nhấn E đổi sang câu khác
    ThemNPC({730.f, 690.f}, LoaiNPC::LINH_DAI_VIET, "Tướng Tiên Phong", {
        "Chào mừng đến doanh trại. Dùng W A S D để di chuyển, giữ Shift để chạy, nhưng chạy nhiều sẽ mất thể lực.",
        "Ngô Quyền đang ở lều lớn giữa doanh trại. Cứ theo đường đất đi lên là thấy.",
        "Nhấn E khi đứng gần người hoặc đồ vật để tương tác. Nhấn Esc nếu cần tạm dừng."});
    ThemNPC({700.f, 575.f}, LoaiNPC::LINH_DAI_VIET, "Lính Thủ Doanh", {
        "Nghe nói nước sông Bạch Đằng lên xuống rất mạnh. Ngô Quyền đang tính toán theo con nước.",
        "Thuyền giặc to và nhiều, nhưng thuyền to thì khó xoay trở nơi nước cạn."});
    ThemNPC({470.f, 490.f}, LoaiNPC::DAN_LANG, "Bác Thợ Đẽo Cọc", {
        "Dân làng chúng tôi chặt gỗ lim đẽo cọc suốt ngày đêm. Gỗ lim cứng, lại chìm nước, làm cọc là hợp nhất.",
        "Cọc phải bịt sắt nhọn thì mới xuyên nổi đáy thuyền giặc đấy."});

    // Ngô Quyền: nói gì tùy theo bước nhiệm vụ đang làm
    NhanVatPhu* ngoQuyen = ThemNPC({780.f, 420.f}, LoaiNPC::NGO_QUYEN, "Ngô Quyền", {"..."}, false);
    (void)ngoQuyen;

    DiemTuongTac diemNgoQuyen;
    diemNgoQuyen.viTri = {780.f, 420.f};
    diemNgoQuyen.banKinh = 72.f;
    diemNgoQuyen.goiY = "Hội kiến";
    diemNgoQuyen.docCao = 70.f;
    diemNgoQuyen.hanhDong = [this]() {
        int ma = MaNhiemVuHienTai();

        if (ma == NV_HOI_KIEN) {
            BatDauHoiThoai({
                {"Ngô Quyền", "Ngươi đến rồi. Ta là Ngô Quyền, con rể và là tướng của Dương Đình Nghệ.", LoaiNPC::NGO_QUYEN},
                {"Ngô Quyền", "Kiều Công Tiễn giết Dương Đình Nghệ, cướp quyền, rồi còn sai sứ sang Nam Hán cầu viện. Hắn đã bị diệt, nhưng quân Nam Hán vẫn kéo sang.", LoaiNPC::NGO_QUYEN},
                {"Ngô Quyền", "Thám báo cho hay: Lưu Hoằng Tháo, con vua Nam Hán, đem thủy quân vượt biển tiến vào cửa sông Bạch Đằng.", LoaiNPC::NGO_QUYEN},
                {"Ngô Quyền", "Thuyền giặc lớn, quân ta ít. Muốn thắng phải dựa vào sức nước: đóng cọc nhọn bịt sắt xuống lòng sông, chờ triều rút.", LoaiNPC::NGO_QUYEN},
                {"Ngô Quyền", "Việc đầu tiên: ra giá binh khí phía tây lấy kiếm, rồi ra bãi tập cho quen tay. Việc lớn còn nhiều.", LoaiNPC::NGO_QUYEN}
            }, [this]() {
                quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
                HienThongBao("Ra giá binh khí ở phía tây doanh trại để lấy kiếm");
            });
        } else if (ma == NV_BAO_CAO) {
            BatDauHoiThoai({
                {"Ngô Quyền", "Tay kiếm đã vững. Tốt lắm.", LoaiNPC::NGO_QUYEN},
                {"Ngô Quyền", "Giờ hãy vào rừng lim ven sông, đốn gỗ cho thợ rèn làm cọc. Cọc phải đủ cứng, đầu bịt sắt, để xuyên thủng đáy thuyền giặc.", LoaiNPC::NGO_QUYEN},
                {"Ngô Quyền", "Nhớ rằng cả trận đánh nằm ở con nước. Đi đi, mọi người đang chờ.", LoaiNPC::NGO_QUYEN}
            }, [this]() {
                quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
                HoanThanhChuong("CHƯƠNG 1: HỘI KIẾN NGÔ QUYỀN",
                    "Năm 937, Kiều Công Tiễn giết Dương Đình Nghệ để cướp quyền rồi cầu cứu nhà Nam Hán. "
                    "Ngô Quyền, vốn là tướng và con rể của Dương Đình Nghệ, kéo quân ra Bắc diệt Kiều Công Tiễn, "
                    "rồi chuẩn bị đón đánh thủy quân Nam Hán.");
            });
        } else if (ma < NV_HOI_KIEN) {
            BatDauHoiThoai({{"Ngô Quyền", "Cứ vào doanh trại, ta đang chờ.", LoaiNPC::NGO_QUYEN}});
        } else if (ma == NV_NHAN_KIEM) {
            BatDauHoiThoai({{"Ngô Quyền", "Giá binh khí ở phía tây doanh trại. Lấy kiếm rồi hãy quay lại.", LoaiNPC::NGO_QUYEN}});
        } else {
            BatDauHoiThoai({{"Ngô Quyền", "Ra bãi tập phía nam, chém trúng sáu nhát rồi hãy về báo ta.", LoaiNPC::NGO_QUYEN}});
        }
    };
    ThemDiemTuongTac(diemNgoQuyen);

    // Giá binh khí: chỉ nhận kiếm được khi tới bước "Nhận kiếm"
    DiemTuongTac diemGia;
    diemGia.viTri = {610.f, 515.f};
    diemGia.banKinh = 70.f;
    diemGia.goiY = "Lấy kiếm";
    diemGia.goiYKhiKhoa = "Giá binh khí";
    diemGia.docCao = 90.f;
    diemGia.dangMo = [this]() { return MaNhiemVuHienTai() == NV_NHAN_KIEM; };
    diemGia.hanhDong = [this]() {
        nguoiChoi.NhanVatPham(VatPham(ID_KIEM, "Kiếm Đại Việt", "Thanh kiếm sắt do thợ doanh trại rèn", LoaiVatPham::VU_KHI, 1, 1, sf::Color(200, 210, 225)));
        nguoiChoi.SetCoKiem(true);
        nguCanh.amThanh.PhatTiengNhatDo();
        ThemChuBay({610.f, 440.f}, "+ Kiếm Đại Việt", sf::Color(255, 230, 120));
        quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
        HienThongBao("Nhấn Space hoặc J để chém. Ra bãi tập phía nam luyện kiếm", 6.f);
    };
    ThemDiemTuongTac(diemGia);
}

void ManChuong1::XuLyChemBuNhin() {
    if (!nguoiChoi.IsDangTanCong()) {
        daChamTrongNhatNay.clear();
        return;
    }

    sf::FloatRect vungChem = nguoiChoi.GetVungTanCong();
    for (int chiSo : chiSoBuNhin) {
        if (daChamTrongNhatNay.count(chiSo)) continue;

        sf::Vector2f p = danhSachVat[chiSo].GetViTri();
        sf::FloatRect thanBuNhin({p.x - 16.f, p.y - 62.f}, {32.f, 66.f});
        if (!vungChem.findIntersection(thanBuNhin)) continue;

        daChamTrongNhatNay.insert(chiSo);
        danhSachVat[chiSo].Rung(0.4f);
        quanLyGiaoDien.ThemHieuUngVaChamGo({p.x, p.y - 28.f});
        nguCanh.amThanh.PhatTiengTrungDon();

        if (MaNhiemVuHienTai() == NV_LUYEN_KIEM) {
            ThemChuBay({p.x, p.y - 70.f}, "Trúng!", sf::Color(255, 220, 90));
            quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
            if (MaNhiemVuHienTai() == NV_BAO_CAO) HienThongBao("Luyện kiếm xong. Về báo Ngô Quyền ở lều lớn", 5.f);
        }
    }
}

void ManChuong1::CapNhatRieng(float deltaTime) {
    (void)deltaTime;

    // Gợi ý điều khiển sau khi phim mở đầu kết thúc
    if (!daHienHuongDan && !quanLyCanhPhim.IsDangChieuPhim()) {
        daHienHuongDan = true;
        HienThongBao("W A S D để di chuyển, giữ Shift để chạy", 6.f);
    }

    // Bước vào doanh trại thì hoàn thành nhiệm vụ đầu
    if (MaNhiemVuHienTai() == NV_VAO_DOANH_TRAI && khuDoanhTrai.contains(nguoiChoi.GetViTri())) {
        quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
        HienThongBao("Hãy đến gặp Ngô Quyền ở lều lớn giữa doanh trại", 6.f);
    }

    XuLyChemBuNhin();
}

void ManChuong1::VeNenDong() {
    VeAnhNuoc(VUNG_SONG);
}
