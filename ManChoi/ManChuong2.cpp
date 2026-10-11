#include "ManChuong2.h"
#include "../AmThanh/QuanLyAmThanh.h"
#include "../BanDo/DoHoa.h"
#include <algorithm>
#include <cmath>
#include <iterator>

using namespace dohoa;

namespace {
const float RONG_THE_GIOI = 1800.f;
const float CAO_THE_GIOI = 1200.f;
const sf::FloatRect VUNG_SONG({1660.f, 0.f}, {140.f, 1200.f});
const sf::Vector2f VI_TRI_LO_REN(1150.f, 520.f);
const sf::Vector2f VI_TRI_XE_BO(1500.f, 700.f);

float KhoangCachDenDoan(sf::Vector2f p, sf::Vector2f a, sf::Vector2f b) {
    sf::Vector2f ab = b - a;
    float dai2 = ab.x * ab.x + ab.y * ab.y;
    float t = (dai2 < 0.001f) ? 0.f : ((p.x - a.x) * ab.x + (p.y - a.y) * ab.y) / dai2;
    t = std::max(0.f, std::min(1.f, t));
    sf::Vector2f g = a + ab * t;
    return std::sqrt((p.x - g.x) * (p.x - g.x) + (p.y - g.y) * (p.y - g.y));
}
}

ManChuong2::ManChuong2(NguCanhGame& ngu)
    : ManChoiDiBo(ngu, RONG_THE_GIOI, CAO_THE_GIOI, {250.f, 1000.f}),
      chiSoXeBo(-1),
      chiSoLoRen(-1),
      dangRen(false),
      tienDoRen(0.f),
      demTiaLuaRen(0.f),
      daThoaiMoDau(false),
      daSinhThamTu(false),
      soThamTuBanDau(0) {
    nguoiChoi.DatGioiHan({{30.f, 50.f}, {1560.f, 1110.f}});

    DungBanDo();
    DatTuongTac();
    DatNhiemVu();

    BatDauCanhMoDau("CHƯƠNG 2: CHUẨN BỊ TRẬN ĐỊA",
        "Muốn thắng thuyền lớn, phải có cọc nhọn cắm dưới lòng sông. Ngô Quyền sai người vào rừng lim đốn gỗ, "
        "rèn sắt bịt đầu cọc, rồi chở ra bờ sông chờ con nước.", 6.f);
}

void ManChuong2::DungBanDo() {
    // Các đường mòn: đường lớn từ chỗ xuất phát ra lò rèn và bờ sông, đường nhỏ vào rừng lim
    const std::vector<std::vector<sf::Vector2f>> cacDuong = {
        {{250.f, 1000.f}, {520.f, 950.f}, {760.f, 890.f}, {980.f, 850.f}, {1110.f, 700.f}, {1150.f, 590.f}},
        {{1110.f, 700.f}, {1300.f, 720.f}, {1500.f, 745.f}, {1600.f, 760.f}},
        {{420.f, 960.f}, {390.f, 790.f}, {430.f, 620.f}, {520.f, 450.f}, {600.f, 340.f}, {580.f, 230.f}},
        {{980.f, 850.f}, {1040.f, 960.f}}
    };

    BatDauVeNen();
    SonNenCo(nenBanDo, {{0.f, 0.f}, {RONG_THE_GIOI, CAO_THE_GIOI}}, 11, sf::Color(44, 98, 50), sf::Color(86, 144, 70));

    // Nền rừng sẫm hơn: nhiều lớp elip trong suốt phủ phía tây bắc để bóng cây có chiều sâu
    {
        VA va(sf::PrimitiveType::Triangles);
        elip(va, 480.f, 420.f, 560.f, 420.f, mau(18, 60, 34, 34), 40);
        elip(va, 460.f, 400.f, 480.f, 350.f, mau(18, 60, 34, 40), 40);
        elip(va, 440.f, 380.f, 380.f, 280.f, mau(18, 60, 34, 46), 40);
        // Khoảng sân đất quanh lò rèn
        elip(va, 1160.f, 560.f, 230.f, 150.f, mau(160, 132, 92, 44), 36);
        elip(va, 1160.f, 560.f, 190.f, 120.f, mau(160, 132, 92, 58), 36);
        nenBanDo.draw(va);
    }
    for (const auto& duong : cacDuong) SonDuongDat(nenBanDo, duong, duong.size() <= 2 ? 34.f : 46.f);
    SonSongBenPhai(nenBanDo, VUNG_SONG, 8);
    KetThucVeNen();

    // ----- Khu lò rèn -----
    chiSoLoRen = ThemVat(LoaiVat::LO_REN, VI_TRI_LO_REN);
    ThemVat(LoaiVat::DONG_LUA, {1010.f, 470.f});
    ThemVat(LoaiVat::LEU, {1010.f, 400.f}, 1.f, 1);
    ThemVat(LoaiVat::LEU, {1330.f, 480.f}, 1.f, 0);
    ThemVat(LoaiVat::THUNG, {1230.f, 600.f});
    ThemVat(LoaiVat::THUNG, {1255.f, 606.f});

    // ----- Kho sắt phía nam -----
    chiSoDongSat.push_back(ThemVat(LoaiVat::DONG_SAT, {1000.f, 1040.f}, 1.1f, 0));
    chiSoDongSat.push_back(ThemVat(LoaiVat::DONG_SAT, {1130.f, 1000.f}, 1.1f, 1));
    chiSoDongSat.push_back(ThemVat(LoaiVat::DONG_SAT, {1100.f, 1110.f}, 1.1f, 2));
    ThemVat(LoaiVat::THUNG, {900.f, 1060.f});
    ThemVat(LoaiVat::THUNG, {925.f, 1070.f});
    ThemVat(LoaiVat::LEU, {1230.f, 1040.f}, 0.9f, 1);

    // ----- Bờ sông: xe bò và thuyền neo -----
    chiSoXeBo = ThemVat(LoaiVat::XE_BO, VI_TRI_XE_BO, 1.1f, 0);
    ThemVat(LoaiVat::THUYEN_NEO, {1715.f, 380.f}, 1.1f, 1);
    ThemVat(LoaiVat::THUYEN_NEO, {1730.f, 930.f}, 1.f, 2);
    ThemVat(LoaiVat::COT_CO, {1590.f, 640.f});

    // ----- Rừng lim: 10 cây lim đốn được, đặt tay để chắc chắn tới được -----
    const sf::Vector2f viTriLim[] = {
        {190.f, 230.f}, {320.f, 150.f}, {450.f, 300.f}, {640.f, 170.f}, {760.f, 310.f},
        {240.f, 470.f}, {380.f, 520.f}, {620.f, 450.f}, {760.f, 600.f}, {180.f, 700.f}
    };
    Rng rng(938u);
    for (const auto& p : viTriLim) {
        chiSoCayLim.push_back(ThemVat(LoaiVat::CAY_LIM, p, 1.05f + rng.f() * 0.15f, rng.irange(0, 4)));
        soNhatDaChat.push_back(0);
    }

    // ----- Cây thường và bụi rải khắp bản đồ, dày hơn ở rừng phía tây -----
    std::vector<sf::Vector2f> daDat(std::begin(viTriLim), std::end(viTriLim));
    int soCay = 0;
    for (int thu = 0; thu < 4000 && soCay < 120; ++thu) {
        bool trongRung = (soCay < 70);
        sf::Vector2f p = trongRung ? sf::Vector2f(rng.range(40.f, 900.f), rng.range(70.f, 880.f))
                                   : sf::Vector2f(rng.range(40.f, 1600.f), rng.range(70.f, CAO_THE_GIOI - 30.f));

        sf::FloatRect khuLoai({880.f, 330.f}, {520.f, 420.f});   // Sân lò rèn
        sf::FloatRect khuLoaiXe({1380.f, 600.f}, {260.f, 200.f});
        sf::FloatRect khuKho({840.f, 920.f}, {500.f, 340.f});     // Kho sắt phía nam (mở rộng để không bị tán cây che)
        if (khuLoai.contains(p) || khuLoaiXe.contains(p) || khuKho.contains(p)) continue;
        if (std::hypot(p.x - 250.f, p.y - 1000.f) < 120.f) continue;

        bool gan = false;
        // Tuyệt đối không sinh cây/bụi gần các đống sắt
        for (int chiSo : chiSoDongSat) {
            sf::Vector2f vtSat = danhSachVat[chiSo].GetViTri();
            if (std::hypot(p.x - vtSat.x, p.y - vtSat.y) < 150.f) gan = true;
        }
        for (const auto& duong : cacDuong) {
            for (size_t i = 0; i + 1 < duong.size(); ++i) {
                if (KhoangCachDenDoan(p, duong[i], duong[i + 1]) < 58.f) gan = true;
            }
        }
        for (const auto& q : daDat) {
            if (std::hypot(p.x - q.x, p.y - q.y) < (trongRung ? 64.f : 80.f)) gan = true;
        }
        if (gan) continue;

        daDat.push_back(p);
        ++soCay;
        if (soCay % 7 == 0) ThemVat(LoaiVat::DA, p, 0.8f + rng.f() * 0.5f);
        else if (soCay % 4 == 0) ThemVat(LoaiVat::BUI, p, 0.9f + rng.f() * 0.4f, rng.irange(0, 5));
        else ThemVat(LoaiVat::CAY, p, 0.9f + rng.f() * 0.4f, rng.irange(0, 5));
    }

    CapNhatVatCan();
}

void ManChuong2::DatNhiemVu() {
    quanLyNhiemVu.DatTieuDeBang("NHIỆM VỤ CHƯƠNG 2");
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuThuThap>(NV_DON_GO, "Đốn gỗ lim",
        "Vào rừng lim phía tây. Đứng gần cây lim có vòng sáng, nhấn E để chặt, mỗi cây ba nhát.", ID_GO_LIM, SO_COC_CAN));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuThuThap>(NV_LAY_SAT, "Lấy thanh sắt",
        "Ra kho sắt phía nam, nhấn E để lấy sắt. Mỗi đống được hai thanh.", ID_THANH_SAT, SO_COC_CAN));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVu>(NV_REN_COC, "Rèn cọc bịt sắt",
        "Mang gỗ và sắt tới lò rèn phía bắc, nhấn E để rèn từng cọc. Mỗi cọc cần một gỗ và một sắt.", LoaiNhiemVu::XAY_DUNG, SO_COC_CAN));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuKhamPha>(NV_CHAT_COC, "Chất cọc lên xe bò",
        "Đẩy cọc ra xe bò gần bờ sông phía đông, nhấn E để chất lên xe."));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuChienDau>(NV_DIET_THAM_TU, "Diệt thám tử Nam Hán",
        "Thám tử địch lảng vảng quanh xe cọc. Chém chúng bằng Space hoặc J, đừng để chúng báo tin về.", SO_THAM_TU));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuKhamPha>(NV_BAO_TIN, "Báo tin Tướng Tiên Phong",
        "Nói chuyện với Tướng Tiên Phong bên xe bò."));
    quanLyNhiemVu.BatDau();
}

void ManChuong2::DatTuongTac() {
    // ----- NPC -----
    ThemNPC({330.f, 960.f}, LoaiNPC::DAN_LANG, "Bác Thợ Đẽo Cọc", {
        "Gỗ lim vừa cứng vừa nặng, lại chìm nước, làm cọc là hợp nhất. Cây nào có vòng sáng quanh gốc là chặt được.",
        "Mỗi cây phải chặt ba nhát mới ngã. Đừng nản, trận này cần rất nhiều cọc.",
        "Nhớ mang sắt tới lò rèn. Cọc không bịt sắt thì chỉ như que củi."});
    ThemNPC({1085.f, 570.f}, LoaiNPC::DAN_LANG, "Thợ Rèn", {
        "Có đủ gỗ lim và thanh sắt là ta rèn được cọc ngay. Mỗi cọc phải đập sắt cho ôm chặt đầu gỗ.",
        "Đầu cọc phải nhọn và bịt sắt kỹ, để thuyền lớn của giặc đâm vào là thủng đáy."});

    // Tướng Tiên Phong: nói chuyện kết chương khi tới bước báo tin
    ThemNPC({1440.f, 745.f}, LoaiNPC::LINH_DAI_VIET, "Tướng Tiên Phong", {"..."}, false);
    DiemTuongTac diemTuong;
    diemTuong.viTri = {1440.f, 745.f};
    diemTuong.banKinh = 70.f;
    diemTuong.goiY = "Trò chuyện";
    diemTuong.docCao = 62.f;
    diemTuong.hanhDong = [this]() {
        int ma = MaNhiemVuHienTai();
        if (ma == NV_BAO_TIN) {
            BatDauHoiThoai({
                {"Tướng Tiên Phong", "Giỏi lắm! Đám thám tử đã bị diệt, tin tức về bãi cọc không lọt ra ngoài.", LoaiNPC::LINH_DAI_VIET},
                {"Tướng Tiên Phong", "Số cọc này sẽ được chở ra cửa sông. Ngô Quyền cần người cắm chúng xuống lòng sông lúc nước triều thấp.", LoaiNPC::LINH_DAI_VIET},
                {"Tướng Tiên Phong", "Nghỉ lấy sức. Việc khó nhất còn ở phía trước.", LoaiNPC::LINH_DAI_VIET}
            }, [this]() {
                quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
                HoanThanhChuong("CHƯƠNG 2: CHUẨN BỊ TRẬN ĐỊA",
                    "Theo sử cũ, Ngô Quyền cho đóng cọc gỗ đầu bịt sắt nhọn dưới lòng sông Bạch Đằng. "
                    "Khi thủy triều lên, cọc bị nước che khuất. Khi nước rút, cọc nhô lên chặn đường thuyền giặc. "
                    "Vị trí cụ thể của bãi cọc năm 938 chưa được xác định chắc chắn trong sử liệu.");
            });
        } else if (ma == NV_CHAT_COC) {
            BatDauHoiThoai({{"Tướng Tiên Phong", "Xe bò đã sẵn sàng. Chất đủ sáu cọc lên xe là chúng ta có thể đưa ra cửa sông.", LoaiNPC::LINH_DAI_VIET}});
        } else {
            BatDauHoiThoai({{"Tướng Tiên Phong", "Cứ làm cho xong phần việc của ngươi. Ta canh giữ xe cọc ở đây.", LoaiNPC::LINH_DAI_VIET}});
        }
    };
    ThemDiemTuongTac(diemTuong);

    // ----- Chặt cây lim -----
    for (size_t k = 0; k < chiSoCayLim.size(); ++k) {
        int chiSoVat = chiSoCayLim[k];
        sf::Vector2f p = danhSachVat[chiSoVat].GetViTri();

        DiemTuongTac cay;
        cay.viTri = p;
        cay.banKinh = 52.f;
        cay.goiY = "Chặt cây lim";
        cay.goiYKhiKhoa = "Cây lim";
        cay.docCao = 108.f;
        cay.dangMo = [this, chiSoVat]() {
            return MaNhiemVuHienTai() == NV_DON_GO && !danhSachVat[chiSoVat].IsDaDung();
        };
        cay.hanhDong = [this, k, chiSoVat, p]() {
            nguoiChoi.TanCong();
            thoiGianKhoaDieuKhien = 0.3f;   // Một nhát chặt mất chút thời gian
            danhSachVat[chiSoVat].Rung(0.35f);
            quanLyGiaoDien.ThemHieuUngVaChamGo({p.x, p.y - 30.f});
            nguCanh.amThanh.PhatTiengVaChamCoc();
            ++soNhatDaChat[k];

            if (soNhatDaChat[k] >= SO_NHAT_CHAT) {
                danhSachVat[chiSoVat].DatDaDung(true);
                CapNhatVatCan();
                nguoiChoi.NhanVatPham(VatPham(ID_GO_LIM, "Gỗ lim", "Gỗ lim cứng, chìm nước, hợp làm cọc", LoaiVatPham::NHIEM_VU, 1, 99, sf::Color(130, 70, 48)));
                nguCanh.amThanh.PhatTiengNhatDo();
                ThemChuBay({p.x, p.y - 60.f}, "+1 Gỗ lim", sf::Color(255, 210, 140));
                DongBoTienDoVoiTuDo(ID_GO_LIM);
                if (MaNhiemVuHienTai() == NV_LAY_SAT) HienThongBao("Đủ gỗ rồi. Ra kho sắt phía nam lấy thanh sắt", 5.f);
            } else {
                ThemChuBay({p.x, p.y - 60.f}, std::to_string(soNhatDaChat[k]) + " / " + std::to_string(SO_NHAT_CHAT), sf::Color(255, 255, 255));
            }
        };
        ThemDiemTuongTac(cay);
    }

    // ----- Lấy sắt -----
    for (int chiSoVat : chiSoDongSat) {
        sf::Vector2f p = danhSachVat[chiSoVat].GetViTri();
        DiemTuongTac sat;
        sat.viTri = p;
        sat.banKinh = 56.f;
        sat.goiY = "Lấy thanh sắt";
        sat.goiYKhiKhoa = "Kho sắt";
        sat.docCao = 54.f;
        sat.dangMo = [this, chiSoVat]() {
            return MaNhiemVuHienTai() == NV_LAY_SAT && !danhSachVat[chiSoVat].IsDaDung();
        };
        sat.hanhDong = [this, chiSoVat, p]() {
            danhSachVat[chiSoVat].DatDaDung(true);
            nguoiChoi.NhanVatPham(VatPham(ID_THANH_SAT, "Thanh sắt", "Sắt để bịt đầu cọc", LoaiVatPham::NHIEM_VU, 2, 99, sf::Color(176, 188, 208)));
            nguCanh.amThanh.PhatTiengNhatDo();
            ThemChuBay({p.x, p.y - 50.f}, "+2 Thanh sắt", sf::Color(190, 215, 255));
            DongBoTienDoVoiTuDo(ID_THANH_SAT);
            if (MaNhiemVuHienTai() == NV_REN_COC) HienThongBao("Đủ nguyên liệu. Mang tới lò rèn phía bắc", 5.f);
        };
        ThemDiemTuongTac(sat);
    }

    // ----- Lò rèn -----
    DiemTuongTac loRen;
    loRen.viTri = {VI_TRI_LO_REN.x + 10.f, VI_TRI_LO_REN.y + 36.f};
    loRen.banKinh = 78.f;
    loRen.goiY = "Rèn cọc";
    loRen.goiYKhiKhoa = "Lò rèn";
    loRen.docCao = 96.f;
    loRen.dangMo = [this]() {
        return MaNhiemVuHienTai() == NV_REN_COC && !dangRen &&
               nguoiChoi.GetTuDo().KiemTraVatPham(ID_GO_LIM, 1) && nguoiChoi.GetTuDo().KiemTraVatPham(ID_THANH_SAT, 1);
    };
    loRen.hanhDong = [this]() { BatDauRen(); };
    ThemDiemTuongTac(loRen);

    // ----- Xe bò -----
    DiemTuongTac xe;
    xe.viTri = {VI_TRI_XE_BO.x, VI_TRI_XE_BO.y + 30.f};
    xe.banKinh = 80.f;
    xe.goiY = "Chất cọc lên xe";
    xe.goiYKhiKhoa = "Xe bò chở cọc";
    xe.docCao = 96.f;
    xe.dangMo = [this]() {
        return MaNhiemVuHienTai() == NV_CHAT_COC && nguoiChoi.GetTuDo().KiemTraVatPham(ID_COC, SO_COC_CAN);
    };
    xe.hanhDong = [this]() {
        nguoiChoi.GetTuDo().XoaVatPham(ID_COC, SO_COC_CAN);
        danhSachVat[chiSoXeBo].SetBienThe(1);   // Xe hiện cọc nhọn xếp trên thùng
        nguCanh.amThanh.PhatTiengNhatDo();
        HienThongBao("Đã chất " + std::to_string(SO_COC_CAN) + " cọc lên xe bò", 4.f);
        quanLyNhiemVu.CapNhatTienDoNhiemVu(1);

        // Vừa chất xong thì thám tử Nam Hán xuất hiện
        BatDauHoiThoai({
            {"Tướng Tiên Phong", "Coi chừng! Có thám tử Nam Hán rình quanh đây. Không để chúng chạy về báo tin!", LoaiNPC::LINH_DAI_VIET}
        }, [this]() { SinhThamTu(); });
    };
    ThemDiemTuongTac(xe);
}

void ManChuong2::BatDauRen() {
    dangRen = true;
    tienDoRen = 0.f;
    demTiaLuaRen = 0.f;
    thoiGianKhoaDieuKhien = 1.0f;
    nguoiChoi.DungLai();
}

void ManChuong2::CapNhatRen(float deltaTime) {
    if (!dangRen) return;

    tienDoRen += deltaTime;
    thoiGianKhoaDieuKhien = std::max(thoiGianKhoaDieuKhien, 0.1f);   // Đứng yên cho tới khi rèn xong

    // Tia lửa bắn ra và tiếng búa đập theo nhịp
    demTiaLuaRen += deltaTime;
    if (demTiaLuaRen >= 0.28f) {
        demTiaLuaRen = 0.f;
        sf::Vector2f dePos(VI_TRI_LO_REN.x + 56.f, VI_TRI_LO_REN.y - 20.f);
        quanLyGiaoDien.ThemHieuUngChem(dePos);
        nguCanh.amThanh.PhatTiengVaChamCoc();
        danhSachVat[chiSoLoRen].Rung(0.1f);
    }

    if (tienDoRen >= 1.3f) {
        dangRen = false;
        thoiGianKhoaDieuKhien = 0.15f;
        nguoiChoi.GetTuDo().XoaVatPham(ID_GO_LIM, 1);
        nguoiChoi.GetTuDo().XoaVatPham(ID_THANH_SAT, 1);
        nguoiChoi.NhanVatPham(VatPham(ID_COC, "Cọc bịt sắt", "Cọc gỗ lim đầu nhọn bịt sắt, dùng cắm lòng sông", LoaiVatPham::COC_GO, 1, 99, sf::Color(139, 69, 19)));
        nguCanh.amThanh.PhatTiengNhatDo();
        ThemChuBay({VI_TRI_LO_REN.x, VI_TRI_LO_REN.y - 100.f}, "+1 Cọc bịt sắt", sf::Color(255, 220, 130));
        quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
        if (MaNhiemVuHienTai() == NV_CHAT_COC) HienThongBao("Đủ sáu cọc. Ra xe bò ngoài bờ sông để chất lên", 5.f);
    }
}

void ManChuong2::SinhThamTu() {
    daSinhThamTu = true;
    const sf::Vector2f viTri[SO_THAM_TU] = {{1330.f, 800.f}, {1420.f, 860.f}, {1300.f, 640.f}};
    for (const auto& p : viTri) heThongChienDau.ThemKeDich(p, 96.f, 55, 9, LoaiKeDich::LINH_THUONG);
    soThamTuBanDau = SO_THAM_TU;
    mayAnh.Rung(4.f, 0.4f);
    nguCanh.amThanh.PhatTiengTrongTran();
}

void ManChuong2::CapNhatChienDau(float deltaTime) {
    if (!daSinhThamTu) return;

    heThongChienDau.CapNhat(deltaTime, nguoiChoi);

    int soTrung = heThongChienDau.XuLyPlayerTanCong(nguoiChoi.GetVungTanCong(), nguoiChoi.GetSatThuong(), nguoiChoi.IsDangTanCong());
    if (soTrung > 0) {
        sf::Vector2f diem = nguoiChoi.GetViTri() + sf::Vector2f(static_cast<float>(nguoiChoi.GetHuongNhin() * 30), 0.f);
        nguCanh.amThanh.PhatTiengTrungDon();
        quanLyGiaoDien.ThemHieuUngChem(diem);
        quanLyGiaoDien.ThemSoSatThuong(diem + sf::Vector2f(0.f, -20.f), nguoiChoi.GetSatThuong());
        mayAnh.Rung(2.5f, 0.12f);
    }

    // Số thám tử đã hạ chính là tiến độ nhiệm vụ
    NhiemVu* nv = quanLyNhiemVu.GetNhiemVuHienTai();
    if (nv && MaNhiemVuHienTai() == NV_DIET_THAM_TU) {
        int daHa = soThamTuBanDau - heThongChienDau.GetSoKeDichConLai();
        if (daHa > nv->GetTienDo()) quanLyNhiemVu.CapNhatTienDoNhiemVu(daHa - nv->GetTienDo());
        if (MaNhiemVuHienTai() == NV_BAO_TIN) {
            nguoiChoi.HoiMau(60);
            HienThongBao("Quân y băng bó vết thương. Báo tin cho Tướng Tiên Phong", 5.f);
        }
    }
}

void ManChuong2::CapNhatRieng(float deltaTime) {
    if (!daThoaiMoDau && !quanLyCanhPhim.IsDangChieuPhim()) {
        daThoaiMoDau = true;
        BatDauHoiThoai({
            {"Bác Thợ Đẽo Cọc", "Ngô Quyền sai ta lo phần cọc. Cần sáu cọc lim bịt sắt cho đợt đầu tiên.", LoaiNPC::DAN_LANG},
            {"Bác Thợ Đẽo Cọc", "Rừng lim ở phía bắc đường mòn. Cây nào có vòng sáng quanh gốc thì đứng gần, nhấn E để chặt. Chặt đủ sáu cây rồi ra kho sắt.", LoaiNPC::DAN_LANG}
        });
    }

    CapNhatRen(deltaTime);
    CapNhatChienDau(deltaTime);
}

void ManChuong2::VeNenDong() {
    VeAnhNuoc(VUNG_SONG);
}

void ManChuong2::ThemMucVeRieng(std::vector<std::pair<float, std::function<void()>>>& danhSach) {
    if (!dangRen) return;

    // Thanh tiến độ rèn cọc treo trên lò
    danhSach.push_back({99999.f, [this]() {
        float tiLe = std::min(1.f, tienDoRen / 1.3f);
        sf::Vector2f tam(VI_TRI_LO_REN.x, VI_TRI_LO_REN.y - 110.f);
        sf::RectangleShape nen({90.f, 10.f});
        nen.setOrigin({45.f, 5.f});
        nen.setPosition(tam);
        nen.setFillColor(sf::Color(20, 20, 26, 230));
        nen.setOutlineThickness(1.5f);
        nen.setOutlineColor(sf::Color(218, 165, 32));
        window.draw(nen);

        sf::RectangleShape day({90.f * tiLe, 10.f});
        day.setOrigin({45.f, 5.f});
        day.setPosition(tam);
        day.setFillColor(sf::Color(250, 140, 40));
        window.draw(day);
        VeChuGiua(window, nguCanh.phongChu, "Đang rèn cọc...", 11, {tam.x, tam.y - 16.f}, sf::Color(255, 230, 150), 1.5f, sf::Color(0, 0, 0, 200));
    }});
}
