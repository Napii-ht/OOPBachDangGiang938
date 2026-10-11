#include "QuanLyGame.h"
#include "TienIch.h"
#include "../ManChoi/ManBachDangCu.h"
#include "../ManChoi/ManChuong1.h"
#include "../ManChoi/ManChuong2.h"
#include "../ManChoi/ManChuong3.h"
#include "../ManChoi/ManChuong4.h"
#include "../ManChoi/ManChuong5.h"
#include "../ManChoi/ManChuong6.h"
#include <iostream>

namespace {
const char* TIEU_DE_CUA_SO = "Bạch Đằng 938";
}

QuanLyGame::QuanLyGame(sf::RenderWindow& win)
    : window(win),
      nguCanh{win, phongChu, amThanh, duLieuLuu},
      daNapFont(false),
      trangThai(TrangThaiGame::MENU),
      trangThaiTruocCaiDat(TrangThaiGame::MENU),
      trangThaiSauKhiThoatMan(TrangThaiGame::CHON_CHUONG) {
    daNapFont = TaiPhongChuMacDinh(phongChu);
    if (!daNapFont) {
        std::cerr << "Khong tim thay font. Hay dat arial.ttf va arialbd.ttf vao TaiNguyen/PhongChu/\n";
        return;
    }

    LuuGame::Tai(duLieuLuu);
    ApDungAmLuong();
    if (duLieuLuu.toanManHinh) ApDungToanManHinh(true);

    DangKyCacChuong();
    ChuyenSangMenu();
}

void QuanLyGame::DangKyCacChuong() {
    // Sáu chương chính theo dàn ý. daCoSan = false nghĩa là chương chưa được làm.
    quanLyManChoi.DangKyChuong({1, "Hội kiến Ngô Quyền", "Doanh trại Từ Lương Xâm trên đồng cỏ ven sông",
        "Đi bộ khám phá doanh trại, trò chuyện với Ngô Quyền và tướng sĩ, nhận kiếm và luyện kiếm trên bãi tập.",
        true, [](NguCanhGame& ngu) { return std::unique_ptr<ManChoi>(new ManChuong1(ngu)); }});
    quanLyManChoi.DangKyChuong({2, "Chuẩn bị trận địa", "Rừng lim và bãi cỏ ven sông",
        "Đốn gỗ lim trong rừng, lấy sắt ở kho, rèn cọc bịt sắt rồi chất lên xe. Dọc đường có thám tử Nam Hán rình rập.",
        true, [](NguCanhGame& ngu) { return std::unique_ptr<ManChoi>(new ManChuong2(ngu)); }});
    quanLyManChoi.DangKyChuong({3, "Dựng bãi cọc", "Cửa sông Bạch Đằng, hai bờ Vũ Yên và Đình Vũ",
        "Chèo thuyền chở cọc ra lòng sông lúc triều thấp, cắm vào từng ô của bãi cọc trước khi nước dâng. Dòng nước cuốn thuyền ra biển.",
        true, [](NguCanhGame& ngu) { return std::unique_ptr<ManChoi>(new ManChuong3(ngu)); }});
    quanLyManChoi.DangKyChuong({4, "Nhử địch vào sâu", "Sông Bạch Đằng lúc triều cao",
        "Lái thuyền nhẹ ra khiêu chiến rồi giả thua. Giữ khoảng cách vừa đủ để hạm đội Nam Hán đuổi theo qua bãi cọc, né tên và thuyền chiến.",
        true, [](NguCanhGame& ngu) { return std::unique_ptr<ManChoi>(new ManChuong4(ngu)); }});
    quanLyManChoi.DangKyChuong({5, "Triều rút", "Lòng sông Bạch Đằng, cảnh phim",
        "Giữ chân hạm đội giặc cho tới khi nước rút đủ thấp rồi đánh trống phát hiệu lệnh. Thuyền giặc đâm vào bãi cọc, quân hai bờ bắn tên.",
        true, [](NguCanhGame& ngu) { return std::unique_ptr<ManChoi>(new ManChuong5(ngu)); }});
    quanLyManChoi.DangKyChuong({6, "Tổng phản công", "Bãi bùn và hai bờ sông",
        "Đi bộ và cận chiến trên bãi bùn khi nước đã rút hết: đánh tan ba đợt quân Nam Hán, hạ Lưu Hoằng Tháo.",
        true, [](NguCanhGame& ngu) { return std::unique_ptr<ManChoi>(new ManChuong6(ngu)); }});

    // Màn thử nghiệm: bản chơi cũ gói lại, chơi được ngay
    quanLyManChoi.DangKyChuong({0, "Màn thử nghiệm (bản cũ)", "Một khung sông duy nhất, cỏ hai bờ",
        "Toàn bộ trận đánh trong một màn như bản trước. Dùng để demo trong lúc các chương mới đang được làm.",
        true, [](NguCanhGame& ngu) { return std::unique_ptr<ManChoi>(new ManBachDangCu(ngu)); }});
}

void QuanLyGame::ApDungToanManHinh(bool bat) {
    if (bat) {
        window.create(sf::VideoMode::getDesktopMode(), VanBan(TIEU_DE_CUA_SO), sf::Style::None, sf::State::Fullscreen);
    } else {
        window.create(sf::VideoMode({800, 600}), VanBan(TIEU_DE_CUA_SO), sf::Style::Default, sf::State::Windowed);
        window.setMinimumSize(sf::Vector2u(400, 300));
    }
    window.setFramerateLimit(60);
    duLieuLuu.toanManHinh = bat;
}

void QuanLyGame::ApDungAmLuong() {
    amThanh.DatAmLuongChung(duLieuLuu.amLuong);
}

void QuanLyGame::LuuTienDo() {
    LuuGame::Luu(duLieuLuu);
}

void QuanLyGame::ChuyenSangMenu() {
    trangThai = TrangThaiGame::MENU;
    amThanh.PhatNhacNen("nhac_menu.ogg");   // Chưa có file thì im lặng
}

void QuanLyGame::ChuyenSangChonChuong() {
    trangThai = TrangThaiGame::CHON_CHUONG;
    manHinhChonChuong.DatDuLieu(quanLyManChoi.GetDanhSachChuong(), duLieuLuu);
}

void QuanLyGame::BatDauChuong(int chiSo) {
    amThanh.DungNhacNen();
    if (quanLyManChoi.BatDauChuong(chiSo, nguCanh)) {
        trangThai = TrangThaiGame::DANG_CHOI;
        trangThaiSauKhiThoatMan = TrangThaiGame::CHON_CHUONG;
    }
}

void QuanLyGame::XuLyKhiManKetThuc() {
    KetQuaManChoi ketQua = KetQuaManChoi::THOAT;
    int maChuong = 0;
    if (!quanLyManChoi.LayKetQuaKhiDong(ketQua, maChuong)) return;

    if (ketQua == KetQuaManChoi::THANG) {
        LuuGame::GhiNhanHoanThanh(duLieuLuu, maChuong);
        LuuTienDo();

        // Thắng chương cuối thì chiếu lời bạt và credits
        if (maChuong >= 6) {
            amThanh.KichHoatNhipTrongTran(false);
            manHinhKetThuc.Mo();
            trangThai = TrangThaiGame::KET_THUC;
            trangThaiSauKhiThoatMan = TrangThaiGame::CHON_CHUONG;
            amThanh.PhatNhacNen("nhac_menu.ogg");
            return;
        }
    }

    amThanh.KichHoatNhipTrongTran(false);
    if (trangThaiSauKhiThoatMan == TrangThaiGame::MENU) {
        ChuyenSangMenu();
    } else {
        ChuyenSangChonChuong();
        amThanh.PhatNhacNen("nhac_menu.ogg");
    }
    trangThaiSauKhiThoatMan = TrangThaiGame::CHON_CHUONG;
}

void QuanLyGame::XuLySuKien(const sf::Event& suKien) {
    // Phím chung: F11 bật tắt toàn màn hình
    if (const auto* phim = suKien.getIf<sf::Event::KeyPressed>()) {
        if (phim->code == sf::Keyboard::Key::F11) {
            ApDungToanManHinh(!duLieuLuu.toanManHinh);
            LuuTienDo();
            return;
        }
    }

    switch (trangThai) {
        case TrangThaiGame::MENU: {
            switch (menuChinh.XuLySuKien(suKien, window)) {
                case LuaChonMenu::CHOI:
                    ChuyenSangChonChuong();
                    break;
                case LuaChonMenu::CAI_DAT:
                    trangThaiTruocCaiDat = TrangThaiGame::MENU;
                    manHinhCaiDat.Mo(duLieuLuu);
                    trangThai = TrangThaiGame::CAI_DAT;
                    break;
                case LuaChonMenu::THOAT:
                    window.close();
                    break;
                default:
                    break;
            }
            break;
        }

        case TrangThaiGame::CHON_CHUONG: {
            int ketQua = manHinhChonChuong.XuLySuKien(suKien, window);
            if (ketQua == ManHinhChonChuong::QUAY_LAI) {
                trangThai = TrangThaiGame::MENU;
            } else if (ketQua >= 0) {
                BatDauChuong(ketQua);
            }
            break;
        }

        case TrangThaiGame::CAI_DAT: {
            HanhDongCaiDat hanhDong = manHinhCaiDat.XuLySuKien(suKien, window, duLieuLuu);
            if (hanhDong == HanhDongCaiDat::DOI_AM_LUONG) {
                ApDungAmLuong();
            } else if (hanhDong == HanhDongCaiDat::DOI_TOAN_MAN_HINH) {
                ApDungToanManHinh(duLieuLuu.toanManHinh);
            } else if (hanhDong == HanhDongCaiDat::QUAY_LAI) {
                LuuTienDo();
                trangThai = trangThaiTruocCaiDat;
            }
            break;
        }

        case TrangThaiGame::DANG_CHOI: {
            // Esc hoặc mất tiêu điểm cửa sổ thì tạm dừng
            bool bamEsc = false;
            if (const auto* phim = suKien.getIf<sf::Event::KeyPressed>()) {
                bamEsc = (phim->code == sf::Keyboard::Key::Escape);
            }
            if ((bamEsc || suKien.is<sf::Event::FocusLost>()) && !quanLyManChoi.DangChuyenCanh()) {
                menuTamDung.MoMenu();
                trangThai = TrangThaiGame::TAM_DUNG;
                break;
            }
            quanLyManChoi.XuLySuKien(suKien);
            break;
        }

        case TrangThaiGame::KET_THUC: {
            manHinhKetThuc.XuLySuKien(suKien);
            if (manHinhKetThuc.DaXong()) ChuyenSangMenu();
            break;
        }

        case TrangThaiGame::TAM_DUNG: {
            switch (menuTamDung.XuLySuKien(suKien, window)) {
                case LuaChonTamDung::TIEP_TUC:
                    trangThai = TrangThaiGame::DANG_CHOI;
                    break;
                case LuaChonTamDung::CAI_DAT:
                    trangThaiTruocCaiDat = TrangThaiGame::TAM_DUNG;
                    manHinhCaiDat.Mo(duLieuLuu);
                    trangThai = TrangThaiGame::CAI_DAT;
                    break;
                case LuaChonTamDung::VE_MENU:
                    trangThaiSauKhiThoatMan = TrangThaiGame::MENU;
                    trangThai = TrangThaiGame::DANG_CHOI;
                    quanLyManChoi.YeuCauThoat();
                    break;
                default:
                    break;
            }
            break;
        }
    }
}

void QuanLyGame::CapNhat(float deltaTime) {
    switch (trangThai) {
        case TrangThaiGame::MENU:
            canhNen.CapNhat(deltaTime);
            menuChinh.CapNhat(deltaTime);
            break;
        case TrangThaiGame::CHON_CHUONG:
            canhNen.CapNhat(deltaTime);
            manHinhChonChuong.CapNhat(deltaTime);
            break;
        case TrangThaiGame::CAI_DAT:
            canhNen.CapNhat(deltaTime);
            manHinhCaiDat.CapNhat(deltaTime);
            break;
        case TrangThaiGame::DANG_CHOI:
            amThanh.CapNhat(deltaTime);
            quanLyManChoi.CapNhat(deltaTime);
            XuLyKhiManKetThuc();
            break;
        case TrangThaiGame::TAM_DUNG:
            menuTamDung.CapNhat(deltaTime);
            break;
        case TrangThaiGame::KET_THUC:
            manHinhKetThuc.CapNhat(deltaTime);
            if (manHinhKetThuc.DaXong()) ChuyenSangMenu();
            break;
    }
}

void QuanLyGame::Ve() {
    window.clear(sf::Color::Black);   // Dải đen khi cửa sổ không đúng tỉ lệ 4:3

    bool veTrenCanhPause = (trangThai == TrangThaiGame::TAM_DUNG) ||
                           (trangThai == TrangThaiGame::CAI_DAT && trangThaiTruocCaiDat == TrangThaiGame::TAM_DUNG);

    if (trangThai == TrangThaiGame::KET_THUC) {
        ApDungViewLogic(window);
        manHinhKetThuc.Ve(window, phongChu);
    } else if (trangThai == TrangThaiGame::DANG_CHOI || veTrenCanhPause) {
        quanLyManChoi.Ve(window);   // Màn chơi (đứng yên nếu đang tạm dừng)
        ApDungViewLogic(window);
        if (trangThai == TrangThaiGame::TAM_DUNG) menuTamDung.Ve(window, phongChu);
        if (trangThai == TrangThaiGame::CAI_DAT) manHinhCaiDat.Ve(window, phongChu, duLieuLuu);
    } else {
        ApDungViewLogic(window);
        canhNen.Ve(window);
        if (trangThai == TrangThaiGame::MENU) menuChinh.Ve(window, phongChu);
        if (trangThai == TrangThaiGame::CHON_CHUONG) manHinhChonChuong.Ve(window, phongChu);
        if (trangThai == TrangThaiGame::CAI_DAT) manHinhCaiDat.Ve(window, phongChu, duLieuLuu);
    }

    window.display();
}
