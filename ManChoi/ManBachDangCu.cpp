
#include "ManBachDangCu.h"
#include "../CotLoi/TienIch.h"
#include <iostream>
#include <cmath>

ManBachDangCu::ManBachDangCu(NguCanhGame& ngu)
    : ManChoi(ngu),
      window(ngu.window),
      trangThaiMan(TrangThaiMan::DANG_CHOI),
      trangThaiTranDau(TrangThaiTranDau::CHUAN_BI),
      dangLaiThuyen(false),
      banDo(800.f, 600.f),
      quanLyGiaoDien(ngu.phongChu),
      thoiGianTrangThai(0.f),
      soCocDaCam(0),
      daThongBaoChienThang(false) {
    KhoiTaoGameMoi();
}

void ManBachDangCu::KhoiTaoGameMoi() {
    trangThaiMan = TrangThaiMan::DANG_CHOI;
    trangThaiTranDau = TrangThaiTranDau::CHUAN_BI;
    dangLaiThuyen = false;
    thoiGianTrangThai = 0.f;
    soCocDaCam = 0;
    daThongBaoChienThang = false;

    // Tạo Player trên bờ Bắc doanh trại Đại Việt
    nguoiChoi = std::make_unique<NguoiChoi>(sf::Vector2f(160.f, 95.f));

    // Thuyền chiến nhẹ Đại Việt neo ở mép nước
    thuyenNguoiChoi = std::make_unique<ThuyenNguoiChoi>(sf::Vector2f(220.f, 220.f));

    // Tạo các nhân vật phụ (NPCs)
    danhSachNPC.clear();

    // 1. Chủ soái Ngô Quyền
    std::vector<std::string> thoaiNgoQuyen = {
        "Hỡi các tướng sĩ, quân giặc Nam Hán cậy thuyền to muốn sang thôn tính nước ta!",
        "Ta đã sai quân dân chặt gỗ làm cọc nhọn bịt sắt, đóng ngầm dưới lòng sông.",
        "Hãy giúp quân ta thu thập đủ 3 cọc gỗ lớn trên bờ và cắm chặt ở trận địa!",
        "Đợi khi thủy triều hạ, thuyền giặc sẽ tiến thoái lưỡng nan, lúc đó ta tổng lực phản công!"
    };
    danhSachNPC.push_back(std::make_unique<NhanVatPhu>(
        sf::Vector2f(280.f, 90.f), LoaiNPC::NGO_QUYEN, "Ngô Quyền", thoaiNgoQuyen
    ));

    // 2. Dân làng hỗ trợ
    std::vector<std::string> thoaiDanLang = {
        "Chúng tôi ngày đêm đẽo gọt cọc gỗ lim để chuẩn bị trận địa giúp Ngô Quyền!",
        "Những cọc gỗ lớn đã được vận chuyển đến ven bờ sông, hãy mau lấy đi cắm!"
    };
    danhSachNPC.push_back(std::make_unique<NhanVatPhu>(
        sf::Vector2f(520.f, 85.f), LoaiNPC::DAN_LANG, "Bác Thợ Đẽo Cọc", thoaiDanLang
    ));

    // 3. Chiến binh Đại Việt
    std::vector<std::string> thoaiLinh = {
        "Phía hạ lưu quân mai phục đã sẵn sàng, chỉ chờ tín hiệu của Ngô Quyền!",
        "Kiếm này thề quyết bảo vệ sông núi bờ cõi!"
    };
    danhSachNPC.push_back(std::make_unique<NhanVatPhu>(
        sf::Vector2f(720.f, 95.f), LoaiNPC::LINH_DAI_VIET, "Tướng Tiên Phong", thoaiLinh
    ));

    // Khởi tạo bản đồ và chuỗi nhiệm vụ
    banDo.KhoiTaoBaiCocChuan();
    banDo.KhoiTaoCocThuThap(3);
    banDo.GetSong().SetTrangThaiThuyTrieu(TrangThaiThuyTrieu::CAO);

    quanLyNhiemVu.KhoiTaoChuoiNhiemVu();
    heThongChienDau.XoaToanBo();

    // Mở màn cảnh phim giới thiệu
    quanLyCanhPhim.BatDauCanh(LoaiCanhPhim::MO_DAU, 5.0f);
    nguCanh.amThanh.KichHoatNhipTrongTran(false);
}

void ManBachDangCu::XuLySuKien(const sf::Event& event) {
    if (trangThaiMan == TrangThaiMan::CHIEN_THANG || trangThaiMan == TrangThaiMan::THUA_TRAN) {
        if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Enter || key->code == sf::Keyboard::Key::Space) {
                // Báo cho QuanLyManChoi biết màn đã kết thúc
                ketQua = (trangThaiMan == TrangThaiMan::CHIEN_THANG) ? KetQuaManChoi::THANG : KetQuaManChoi::THUA;
            }
        }
        return;
    }

    // Đang trong trận đấu (DANG_CHOI)
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        // Tương tác hộp thoại
        if (quanLyGiaoDien.GetHopThoai().IsDangHienThi()) {
            if (key->code == sf::Keyboard::Key::Space || key->code == sf::Keyboard::Key::J || key->code == sf::Keyboard::Key::Enter) {
                if (!quanLyGiaoDien.GetHopThoai().DaGoXong()) {
                    quanLyGiaoDien.GetHopThoai().BoQuaHieuUngGo();
                } else {
                    quanLyGiaoDien.GetHopThoai().DongHopThoai();
                }
            }
            return;
        }

        // Phím tương tác [E]
        if (key->code == sf::Keyboard::Key::E) {
            XuLySuKienDoiThoai();
            XuLyNhatVaCamCoc();
        }

        // Tấn công bằng phím Space
        if (key->code == sf::Keyboard::Key::Space && !dangLaiThuyen) {
            if (nguoiChoi && nguoiChoi->TanCong()) {
                nguCanh.amThanh.PhatTiengChem();
            }
        }
    }
}

void ManBachDangCu::XuLySuKienDoiThoai() {
    if (!nguoiChoi || dangLaiThuyen) return;

    for (auto& npc : danhSachNPC) {
        if (npc->KiemTraPhamViTuongTac(nguoiChoi->GetViTri())) {
            std::string cauThoai = npc->LayLoiThoaiTiepTheo();
            quanLyGiaoDien.GetHopThoai().MoHopThoai(npc->GetTen(), cauThoai, npc->GetLoai());

            // Nếu nói chuyện với Ngô Quyền ở Nhiệm vụ 1 -> Hoàn thành Quest 1
            if (npc->GetLoai() == LoaiNPC::NGO_QUYEN && quanLyNhiemVu.GetChiSoNhiemVuHienTai() == 0) {
                quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
            }
            return;
        }
    }
}

void ManBachDangCu::XuLyNhatVaCamCoc() {
    if (!nguoiChoi) return;

    // 1. Nhặt cọc gỗ trên bờ
    if (quanLyNhiemVu.GetChiSoNhiemVuHienTai() == 1) {
        int nhat = banDo.KiemTraNhatCoc(nguoiChoi->GetViTri(), 55.f);
        if (nhat > 0) {
            nguCanh.amThanh.PhatTiengNhatDo();
            VatPham cocItem(1, "Cọc gỗ bịt sắt", "Cọc gỗ lim vót nhọn bọc thép, cắm trận địa",
                            LoaiVatPham::COC_GO, nhat, 10, sf::Color(139, 69, 19));
            nguoiChoi->NhanVatPham(cocItem);
            quanLyNhiemVu.CapNhatTienDoNhiemVu(nhat);
            quanLyGiaoDien.ThemSoSatThuong(nguoiChoi->GetViTri(), nhat, false);
            return;
        }
    }

    // 2. Cắm cọc gỗ xuống dòng sông
    if (quanLyNhiemVu.GetChiSoNhiemVuHienTai() == 2) {
        // Kiểm tra xem người chơi có đứng ở mép bờ sông (Y từ 120 đến 180 hoặc 510 đến 550)
        sf::Vector2f pos = nguoiChoi->GetViTri();
        if (pos.y >= 115.f && pos.y <= 190.f) {
            if (nguoiChoi->GetTuDo().XoaVatPham(1, 1)) {
                nguCanh.amThanh.PhatTiengVaChamCoc();
                banDo.ThemCocMoi({pos.x, pos.y + 35.f});
                quanLyGiaoDien.ThemHieuUngVaChamGo(pos);
                soCocDaCam++;
                quanLyNhiemVu.CapNhatTienDoNhiemVu(1);
            }
        }
    }

    // 3. Lên thuyền nhẹ chuẩn bị nhử địch (Nhiệm vụ 4)
    if (quanLyNhiemVu.GetChiSoNhiemVuHienTai() == 3 && !dangLaiThuyen) {
        float dx = nguoiChoi->GetViTri().x - thuyenNguoiChoi->GetViTri().x;
        float dy = nguoiChoi->GetViTri().y - thuyenNguoiChoi->GetViTri().y;
        if (std::sqrt(dx * dx + dy * dy) <= 65.f) {
            dangLaiThuyen = true;
            nguCanh.amThanh.PhatTiengNhatDo();
        }
    }
}

void ManBachDangCu::KiemTraChuyenGiaiDoanTranDau(float deltaTime) {
    thoiGianTrangThai += deltaTime;

    // Giai đoạn 1: Chuẩn bị trận địa cọc (Nhiệm vụ 1, 2, 3)
    if (trangThaiTranDau == TrangThaiTranDau::CHUAN_BI) {
        if (quanLyNhiemVu.GetChiSoNhiemVuHienTai() == 3) { // Vừa xong cắm cọc, chuẩn bị sang nhử giặc
            trangThaiTranDau = TrangThaiTranDau::QUAN_DICH_XUAT_HIEN;
            quanLyCanhPhim.BatDauCanh(LoaiCanhPhim::QUAN_DICH_XUAT_HIEN, 5.0f);
            heThongChienDau.TaoDoanThuyenNamHan(4);
            thoiGianTrangThai = 0.f;
        }
    }
    // Giai đoạn 2: Quân giặc xuất hiện -> Bắt đầu nhử địch
    else if (trangThaiTranDau == TrangThaiTranDau::QUAN_DICH_XUAT_HIEN) {
        if (!quanLyCanhPhim.IsDangChieuPhim() || thoiGianTrangThai >= 5.0f) {
            trangThaiTranDau = TrangThaiTranDau::NHU_DICH;
            dangLaiThuyen = true; // Cho người chơi lên thuyền nhẹ Đại Việt
            thoiGianTrangThai = 0.f;
        }
    }
    // Giai đoạn 3: Nhử địch trên sông
    else if (trangThaiTranDau == TrangThaiTranDau::NHU_DICH) {
        // Nhử địch trong khoảng 10 giây hoặc khi thuyền giặc lọt sâu vào bãi cọc
        if (thoiGianTrangThai >= 11.0f) {
            trangThaiTranDau = TrangThaiTranDau::THUY_TRIEU_XUONG;
            banDo.GetSong().BatDauTrieuRut(7.0f); // Bắt đầu triều rút
            quanLyCanhPhim.BatDauCanh(LoaiCanhPhim::THUY_TRIEU_RUT, 6.0f);
            nguCanh.amThanh.PhatTiengVaChamCoc();
            thoiGianTrangThai = 0.f;
        }
    }
    // Giai đoạn 4: Thủy triều rút & Thuyền giặc mắc cọc
    else if (trangThaiTranDau == TrangThaiTranDau::THUY_TRIEU_XUONG) {
        if (thoiGianTrangThai >= 6.5f) {
            trangThaiTranDau = TrangThaiTranDau::TAU_MAC_COC;
            thoiGianTrangThai = 0.f;
        }
    }
    else if (trangThaiTranDau == TrangThaiTranDau::TAU_MAC_COC) {
        if (thoiGianTrangThai >= 3.5f) {
            // Tàu giặc đã mắc kẹt hoàn toàn, bắt đầu Tổng Phản Công!
            trangThaiTranDau = TrangThaiTranDau::PHAN_CONG;
            dangLaiThuyen = false; // Player lên bờ / sàn bãi cọc giáp chiến
            nguoiChoi->SetViTri({250.f, 320.f});

            quanLyCanhPhim.BatDauCanh(LoaiCanhPhim::TONG_PHAN_CONG, 4.5f);
            nguCanh.amThanh.KichHoatNhipTrongTran(true);
            heThongChienDau.TaoTranChienCuoi(6); // Sinh lính và Boss Lưu Hoằng Thao
            quanLyNhiemVu.CapNhatTienDoNhiemVu(1); // Hoàn thành bước 4 chuyển sang 5
            thoiGianTrangThai = 0.f;
        }
    }
    // Giai đoạn 5: Tổng phản công tiêu diệt giặc
    else if (trangThaiTranDau == TrangThaiTranDau::PHAN_CONG) {
        if (heThongChienDau.KiemTraChienThang() && !daThongBaoChienThang) {
            daThongBaoChienThang = true;
            trangThaiTranDau = TrangThaiTranDau::CHIEN_THANG;
            trangThaiMan = TrangThaiMan::CHIEN_THANG;
            quanLyNhiemVu.CapNhatTienDoNhiemVu(6);
            nguCanh.amThanh.KichHoatNhipTrongTran(false);
            nguCanh.amThanh.PhatTiengChienThang();
            quanLyCanhPhim.BatDauCanh(LoaiCanhPhim::CHIEN_THANG_LICH_SU, 8.0f);
        }
    }
}

void ManBachDangCu::CapNhat(float deltaTime) {
    if (trangThaiMan == TrangThaiMan::CHIEN_THANG || trangThaiMan == TrangThaiMan::THUA_TRAN) {
        quanLyCanhPhim.CapNhat(deltaTime);
        quanLyGiaoDien.CapNhat(deltaTime);
        return;
    }

    // Đang chơi
    nguCanh.amThanh.CapNhat(deltaTime);
    quanLyCanhPhim.CapNhat(deltaTime);
    quanLyGiaoDien.CapNhat(deltaTime);
    quanLyNhiemVu.CapNhat(deltaTime);
    banDo.CapNhat(deltaTime);

    for (auto& npc : danhSachNPC) {
        npc->CapNhat(deltaTime);
    }

    // Điều khiển nhân vật hoặc thuyền
    if (dangLaiThuyen && thuyenNguoiChoi) {
        thuyenNguoiChoi->XuLyPhim();
        thuyenNguoiChoi->CapNhat(deltaTime);
    } else if (nguoiChoi) {
        nguoiChoi->XuLyPhim();
        nguoiChoi->CapNhat(deltaTime);

        // Kiểm tra người chơi hy sinh
        if (nguoiChoi->IsDaHySinh()) {
            trangThaiMan = TrangThaiMan::THUA_TRAN;
            return;
        }
    }

    // Xử lý hải chiến
    if (trangThaiTranDau == TrangThaiTranDau::NHU_DICH ||
        trangThaiTranDau == TrangThaiTranDau::THUY_TRIEU_XUONG ||
        trangThaiTranDau == TrangThaiTranDau::TAU_MAC_COC) {
        
        sf::Vector2f viTriMucTieu = dangLaiThuyen ? thuyenNguoiChoi->GetViTri() : nguoiChoi->GetViTri();
        heThongChienDau.CapNhatHaiChien(deltaTime, viTriMucTieu, banDo);
    }

    // Xử lý bộ chiến (Phản công)
    if (trangThaiTranDau == TrangThaiTranDau::PHAN_CONG && nguoiChoi) {
        heThongChienDau.CapNhat(deltaTime, *nguoiChoi);

        // Chém soLinhTrung giặc
        int soLinhTrung = heThongChienDau.XuLyPlayerTanCong(
            nguoiChoi->GetVungTanCong(),
            nguoiChoi->GetSatThuong(),
            nguoiChoi->IsDangTanCong()
        );

        if (soLinhTrung > 0) {
            nguCanh.amThanh.PhatTiengTrungDon();
            quanLyGiaoDien.ThemHieuUngChem(nguoiChoi->GetViTri() + sf::Vector2f(static_cast<float>(nguoiChoi->GetHuongNhin() * 30), 0.f));
            quanLyGiaoDien.ThemSoSatThuong(nguoiChoi->GetViTri() + sf::Vector2f(static_cast<float>(nguoiChoi->GetHuongNhin() * 30), -20.f), nguoiChoi->GetSatThuong());
            quanLyCanhPhim.KichHoatRungManHinh(4.f, 0.15f);
        }
    }

    // Kiểm tra chuyển bước dòng thời gian
    KiemTraChuyenGiaiDoanTranDau(deltaTime);
}

void ManBachDangCu::Ve() {
    // Lưu và thiết lập View với hiệu ứng rung chấn camera
    sf::View viewGoc = TaoViewLogic(window);
    sf::Vector2f offset = quanLyCanhPhim.GetDoLechCamera();
    sf::View viewRung = viewGoc;
    viewRung.move(offset);
    window.setView(viewRung);

    // 1. Vẽ bản đồ (bờ sông, nước sông Bạch Đằng, cọc ngầm, cảnh vật)
    banDo.Ve(window);

    // 2. Vẽ NPCs
    for (const auto& npc : danhSachNPC) {
        npc->Ve(window);
    }

    // 3. Vẽ hải chiến hoặc thuyền
    if (trangThaiTranDau == TrangThaiTranDau::NHU_DICH ||
        trangThaiTranDau == TrangThaiTranDau::THUY_TRIEU_XUONG ||
        trangThaiTranDau == TrangThaiTranDau::TAU_MAC_COC) {
        heThongChienDau.VeHaiChien(window);
    }

    if (thuyenNguoiChoi) {
        thuyenNguoiChoi->Ve(window);
    }

    // 4. Vẽ kẻ địch bộ chiến
    if (trangThaiTranDau == TrangThaiTranDau::PHAN_CONG) {
        heThongChienDau.Ve(window);
    }

    // 5. Vẽ người chơi (nếu không đang lái thuyền)
    if (!dangLaiThuyen && nguoiChoi) {
        nguoiChoi->Ve(window);
    }

    // 6. Vẽ các hạt hiệu ứng và số sát thương bay lên
    quanLyGiaoDien.VeHieuUngVaSatThuong(window);

    // 6.1. Vẽ tiền cảnh bản đồ (tia nắng hoàng hôn, cỏ lau tiền cảnh, sương ven bờ)
    banDo.VeTienCanh(window);

    // Khôi phục View gốc để vẽ HUD cố định
    window.setView(viewGoc);

    // 7. Giao diện HUD Terraria
    if (nguoiChoi) {
        quanLyGiaoDien.VeHUD(window, *nguoiChoi);
    }

    // Bảng nhiệm vụ Terraria
    quanLyNhiemVu.Ve(window, quanLyGiaoDien.GetPhongChu(), {555.f, 16.f});

    // Thanh máu Boss Lưu Hoằng Thao
    if (trangThaiTranDau == TrangThaiTranDau::PHAN_CONG) {
        heThongChienDau.VeBossBar(window, quanLyGiaoDien.GetPhongChu());
    }

    // Hộp thoại đối thoại NPC
    quanLyGiaoDien.GetHopThoai().Ve(window, quanLyGiaoDien.GetPhongChu());

    // Cắt cảnh điện ảnh (Letterboxing & Lời dẫn lịch sử)
    quanLyCanhPhim.VeHieuUngDienAnh(window, quanLyGiaoDien.GetPhongChu());

    // Màn hình thắng/thua
    if (trangThaiMan == TrangThaiMan::CHIEN_THANG) {
        quanLyGiaoDien.VeManHinhChienThang(window);
    } else if (trangThaiMan == TrangThaiMan::THUA_TRAN) {
        quanLyGiaoDien.VeManHinhGameOver(window);
    }
}


