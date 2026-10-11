#include "ManChuong6.h"
#include "../AmThanh/QuanLyAmThanh.h"
#include "../BanDo/DoHoa.h"
#include "../CotLoi/TienIch.h"
#include <algorithm>
#include <cmath>

using namespace dohoa;

namespace {
const sf::Vector2f VI_TRI_THUYEN_BAT_DAU(400.f, 480.f);
const sf::Vector2f VI_TRI_DO_BO(980.f, 500.f);
const sf::Vector2f VI_TRI_SOAI_HAM(1280.f, 480.f);

float KhoangCach(sf::Vector2f a, sf::Vector2f b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

float Dai(sf::Vector2f v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

float NgauNhien(unsigned int& hat) {
    hat = hat * 1664525u + 1013904223u;
    return static_cast<float>((hat >> 8) & 0xFFFF) / 65535.f;
}
}

ManChuong6::ManChuong6(NguCanhGame& ngu)
    : ManChoiDiBo(ngu, 2000.f, 1000.f, VI_TRI_THUYEN_BAT_DAU),
      banDo(2000.f, 1000.f),
      giaiDoan(GiaiDoan::HOI_1_PHONG_HOA),
      demGiay(0.f),
      soThuyenDaDot(0),
      soLinhThuyenDaHa(0),
      soDichDaSinh(0),
      soDichDaTinhTienDo(0),
      daThoaiMoDau(false),
      chuTuongDaChet(false),
      thoiGianChamLai(0.f),
      dangNgamBan(false),
      lucKeoCung(0.f),
      thoiGianHoiNem(0.f) {

    cheDoThuyen = true;                  // Khởi đầu ở Hồi 1: điều khiển thuyền nhẹ
    viTriThongBao = {400.f, 40.f};
    dongGoiYDieuKhien = "W A S D: Lái thuyền    Chuột phải/F: Ném bình hỏa công    Giữ Chuột trái: Ngắm bắn cung";

    // Mực nước triều rút thấp, bãi cọc nhọn nhô lên
    banDo.GetSong().SetTrangThaiThuyTrieu(TrangThaiThuyTrieu::THAP);
    banDo.KhoiTaoBaiCocRong(880.f, 1600.f, 260.f, 680.f);
    banDo.KhoiTaoCocThuThap(0);

    // Thuyền nhẹ người chơi cơ động trên mặt sông
    thuyenTa = std::make_unique<ThuyenNguoiChoi>(VI_TRI_THUYEN_BAT_DAU);
    thuyenTa->DatGioiHan(sf::FloatRect({60.f, 240.f}, {1880.f, 500.f}));

    // Bốn chiến thuyền đồ sộ của Nam Hán mắc cạn giữa bãi cọc
    const sf::Vector2f viTriThuyen[] = {
        {1040.f, 360.f},
        {VI_TRI_SOAI_HAM.x, VI_TRI_SOAI_HAM.y}, // Soái hạm Lưu Hoằng Tháo
        {1480.f, 400.f},
        {1260.f, 620.f}
    };
    for (int i = 0; i < 4; ++i) {
        auto t = std::make_unique<ThuyenKeDich>(viTriThuyen[i], (i == 1));
        t->SetMacCoc(true);
        thuyenMacCan.push_back(std::move(t));
    }

    KhoiTaoHoi1();
    DatNhiemVu();

    // Nhạc nền trận chiến cuối
    nguCanh.amThanh.PhatNhacTranCuoi();

    BatDauCanhMoDau("CHƯƠNG 6: TỔNG PHẢN CÔNG ĐẠI THẮNG",
        "Nước triều rút nhanh, hạm đội Nam Hán đâm vỡ mạn vào bãi cọc ngầm. "
        "Chiến thuyền giặc mắc cạn giữa dòng. Quân ta dùng thuyền nhẹ cơ động áp sát, "
        "dùng hỏa công thiêu rụi hạm đội giặc và mở đường tổng phản công!", 6.f);
}

void ManChuong6::KhoiTaoHoi1() {
    soThuyenDaDot = 0;
    soLinhThuyenDaHa = 0;
    soDichDaTinhTienDo = 0;
    cacBinhDau.clear();
    cacMuiTen.clear();
    cacLinhThuyen.clear();
    cacTenDich.clear();
}

void ManChuong6::KhoiTaoHoi2() {
    cheDoThuyen = false;                 // Hồi 2: Người chơi trực tiếp đi bộ cận chiến
    dongGoiYDieuKhien = "W A S D: Di chuyển    Shift: Chạy    Space/J: Chém đao    Esc: Tạm dừng";
    soDichDaTinhTienDo = 0;

    nguoiChoi.SetViTri(VI_TRI_DO_BO);
    nguoiChoi.DatGioiHan({{60.f, 220.f}, {1880.f, 740.f}});

    DatVatCanMoi();
    SinhQuanTa();
    BatDauDot(GiaiDoan::HOI_2_DOT_1);
}

void ManChuong6::DatVatCanMoi() {
    danhSachVatCan.clear();
    for (const auto& coc : banDo.GetDanhSachCoc()) {
        sf::Vector2f p = coc.GetViTri();
        danhSachVatCan.push_back(sf::FloatRect({p.x - 7.f, p.y + 8.f}, {14.f, 16.f}));
    }
    for (const auto& t : thuyenMacCan) {
        sf::Vector2f p = t->GetViTri();
        danhSachVatCan.push_back(sf::FloatRect({p.x - 46.f, p.y - 12.f}, {92.f, 28.f}));
    }
}

void ManChuong6::DatNhiemVu() {
    quanLyNhiemVu.DatTieuDeBang("CHIẾN TRẬN BẠCH ĐẰNG");
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuChienDau>(NV_PHONG_HOA, "Hồi 1 - Hỏa công thiêu thuyền",
        "Lái thuyền áp sát, ném bình hỏa công (F / Chuột phải) thiêu rụi 4 chiến thuyền giặc mắc cạn.", 4));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuChienDau>(NV_XA_TIEN, "Hồi 1 - Xạ tiễn yểm trợ",
        "Giữ chuột trái để ngắm bắn cung, tiêu diệt 6 cung thủ địch trên boong thuyền.", 6));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuChienDau>(NV_DO_BO_CAN_CHIEN, "Hồi 2 - Đổ bộ cận chiến",
        "Đổ bộ lên bãi cọc, cùng nghĩa quân đánh tan 2 đợt hộ vệ Nam Hán tràn xuống.", SO_DICH_DOT_1 + SO_DICH_DOT_2));
    quanLyNhiemVu.ThemNhiemVu(std::make_unique<NhiemVuChienDau>(NV_QUYET_CHIEN_HOANG_THAO, "Hồi 2 - Sát cánh cùng Ngô Quyền",
        "Sát cánh cùng Ngô Quyền, quyết chiến chém gục Lưu Hoằng Tháo.", 1));
    quanLyNhiemVu.BatDau();
}

bool ManChuong6::CoVaCham(sf::Vector2f viTriChan) const {
    sf::FloatRect chan({viTriChan.x - 11.f, viTriChan.y + 6.f}, {22.f, 12.f});
    for (const auto& r : danhSachVatCan) {
        if (chan.findIntersection(r).has_value()) return true;
    }
    return false;
}

void ManChuong6::GiuNgoaiVatCan(sf::Vector2f viTriCu, NhanVat& nhanVat) const {
    sf::Vector2f moi = nhanVat.GetViTri();
    if (!CoVaCham(moi)) return;
    if (!CoVaCham({moi.x, viTriCu.y})) nhanVat.SetViTri({moi.x, viTriCu.y});
    else if (!CoVaCham({viTriCu.x, moi.y})) nhanVat.SetViTri({viTriCu.x, moi.y});
    else nhanVat.SetViTri(viTriCu);
}

sf::Vector2f ManChuong6::ChonViTriTrong(sf::Vector2f tam, float banKinh, unsigned int& hat) const {
    for (int thu = 0; thu < 60; ++thu) {
        float goc = NgauNhien(hat) * 6.2832f;
        float r = 25.f + NgauNhien(hat) * banKinh;
        sf::Vector2f p(tam.x + std::cos(goc) * r, tam.y + std::sin(goc) * r * 0.6f);
        p.x = std::clamp(p.x, 80.f, 1900.f);
        p.y = std::clamp(p.y, 250.f, 750.f);
        if (!CoVaCham(p)) return p;
    }
    return tam;
}

void ManChuong6::NemBinhDau(sf::Vector2f mucTieu) {
    if (thoiGianHoiNem > 0.f || !thuyenTa) return;
    thoiGianHoiNem = 0.55f;

    sf::Vector2f goc = thuyenTa->GetViTri();
    sf::Vector2f huong = mucTieu - goc;
    float dist = Dai(huong);
    if (dist < 1.f) return;

    huong /= dist;
    // Giới hạn trong tầm ném tối đa
    if (dist > TAM_NEM_TOI_DA) {
        dist = TAM_NEM_TOI_DA;
        mucTieu = goc + huong * dist;
    }

    // Thời gian bay tỉ lệ thuận mượt mà theo cự ly (0.42s đến 0.82s)
    float thoiGianBay = std::clamp(0.42f + (dist / TAM_NEM_TOI_DA) * 0.40f, 0.42f, 0.82f);
    float g = 520.f; // Gia tốc trọng lực ảo
    float v0z = 0.5f * g * thoiGianBay; // Vận tốc ném lên Z để chạm đất sau đúng thoiGianBay
    sf::Vector2f vXy = (mucTieu - goc) / thoiGianBay; // Vận tốc mặt phẳng để rơi chuẩn xác 100% vào mucTieu

    BinhHoaCong b;
    b.viTri = goc;
    b.vanToc = vXy;
    b.doCao = 0.f;
    b.vanTocZ = v0z;
    b.trongLuc = g;
    b.diemDich = mucTieu;
    b.daNo = false;
    b.thoiGianNo = 0.f;
    cacBinhDau.push_back(b);

    nguCanh.amThanh.PhatTiengChem(); // Tiếng vung tay ném
}

void ManChuong6::BanMuiTen(sf::Vector2f huong, float luc) {
    if (!thuyenTa) return;
    float d = Dai(huong);
    if (d < 1.f) return;
    huong /= d;

    MuiTenNguoiChoi m;
    m.viTri = thuyenTa->GetViTri() + huong * 22.f;
    m.vanToc = huong * (380.f + luc * 280.f);
    m.tuoi = 0.f;
    m.dangChay = true;
    cacMuiTen.push_back(m);

    nguCanh.amThanh.PhatTiengChem(); // Tiếng bật dây cung
    ThemChuBay(thuyenTa->GetViTri() + sf::Vector2f(0.f, -40.f), "Xạ tiễn!", sf::Color(255, 200, 80));
}

void ManChuong6::XuLySuKien(const sf::Event& suKien) {
    if (trinhChieuVideo.IsDangPhat()) {
        trinhChieuVideo.XuLySuKien(suKien);
        if (trinhChieuVideo.IsDaXong()) {
            KetThucChuong();
        }
        return;
    }

    ManChoiDiBo::XuLySuKien(suKien);

    if (giaiDoan == GiaiDoan::HOI_1_PHONG_HOA || giaiDoan == GiaiDoan::HOI_1_XA_TIEN) {
        if (const auto* mousePress = suKien.getIf<sf::Event::MouseButtonPressed>()) {
            sf::Vector2f chuotWorld = mayAnh.ChuotSangTheGioi(window, mousePress->position);
            if (mousePress->button == sf::Mouse::Button::Right) {
                NemBinhDau(chuotWorld);
            } else if (mousePress->button == sf::Mouse::Button::Left) {
                dangNgamBan = true;
                lucKeoCung = 0.f;
            }
        }
        if (const auto* mouseRelease = suKien.getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseRelease->button == sf::Mouse::Button::Left && dangNgamBan) {
                sf::Vector2f chuotWorld = mayAnh.ChuotSangTheGioi(window, mouseRelease->position);
                BanMuiTen(chuotWorld - thuyenTa->GetViTri(), lucKeoCung);
                dangNgamBan = false;
                lucKeoCung = 0.f;
            }
        }
        if (const auto* keyPress = suKien.getIf<sf::Event::KeyPressed>()) {
            if (keyPress->code == sf::Keyboard::Key::F) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                NemBinhDau(mayAnh.ChuotSangTheGioi(window, mousePos));
            }
        }
    }

    if (chuTuongDaChet) {
        if (const auto* keyPress = suKien.getIf<sf::Event::KeyPressed>()) {
            if (keyPress->code == sf::Keyboard::Key::V) {
                trinhChieuVideo.MoVideo("ChienThangBachDang.mpg", "ChienThangBachDang.ogg");
            }
        }
    }
}

void ManChuong6::CapNhatHoi1(float deltaTime) {
    if (thoiGianHoiNem > 0.f) thoiGianHoiNem -= deltaTime;

    // Người chơi điều khiển thuyền nhẹ
    if (thuyenTa) {
        thuyenTa->XuLyPhim();
        thuyenTa->CapNhat(deltaTime);
        nguoiChoi.SetViTri(thuyenTa->GetViTri()); // Để camera và điểm tương tác bám theo thuyền
    }

    // Đang ngắm bắn: tăng lực kéo cung
    if (dangNgamBan) {
        lucKeoCung = std::min(1.f, lucKeoCung + deltaTime * 1.8f);
    }

    // Cập nhật bình hỏa công
    for (auto& b : cacBinhDau) {
        if (!b.daNo) {
            b.viTri += b.vanToc * deltaTime;
            b.doCao += b.vanTocZ * deltaTime;
            b.vanTocZ -= b.trongLuc * deltaTime; // Trọng lực rơi
            if (b.doCao <= 0.f) {
                b.doCao = 0.f;
                b.viTri = b.diemDich; // Chốt hạ chính xác tuyệt đối tại điểm nhắm!
                b.daNo = true;
                b.thoiGianNo = 0.5f;

                quanLyGiaoDien.ThemHieuUngVaChamGo(b.viTri);
                quanLyGiaoDien.ThemHieuUngNuocBan(b.viTri);
                mayAnh.Rung(4.2f, 0.18f);

                // Kiểm tra trúng chiến thuyền giặc
                for (size_t i = 0; i < thuyenMacCan.size(); ++i) {
                    auto& tau = thuyenMacCan[i];
                    sf::Vector2f vtTau = tau->GetViTri();
                    float dx = std::abs(b.viTri.x - vtTau.x);
                    float dy = std::abs(b.viTri.y - vtTau.y);
                    bool trungThuyen = (dx < 75.f && dy < 35.f) || (KhoangCach(b.viTri, vtTau) < 95.f);
                    if (trungThuyen) {
                        if (!tau->IsDangChay()) {
                            tau->SetDangChay(true);
                            soThuyenDaDot++;
                            tau->NhanSatThuong(50);
                            nguCanh.amThanh.PhatTiengVaChamCoc();
                            ThemChuBay(tau->GetViTri() + sf::Vector2f(0.f, -50.f), "+HỎA CÔNG THIÊU RỤI!", sf::Color(255, 90, 40));
                            DongBoTienDo();

                            if (soThuyenDaDot >= static_cast<int>(thuyenMacCan.size()) && giaiDoan == GiaiDoan::HOI_1_PHONG_HOA) {
                                giaiDoan = GiaiDoan::HOI_1_XA_TIEN;
                                soDichDaTinhTienDo = 0;
                                soLinhThuyenDaHa = 0;
                                HienThongBao("Toàn bộ chiến hạm địch đã bốc cháy! Cung thủ địch xuất hiện trên boong!", 5.f);

                                // Sinh lính cung thủ trên boong tàu giặc
                                cacLinhThuyen.clear();
                                for (size_t tIdx = 0; tIdx < thuyenMacCan.size(); ++tIdx) {
                                    sf::Vector2f vt = thuyenMacCan[tIdx]->GetViTri();
                                    cacLinhThuyen.push_back({vt + sf::Vector2f(-32.f, -12.f), 40, 40, 1.0f, 0.f, 0.f, false, tIdx});
                                    cacLinhThuyen.push_back({vt + sf::Vector2f(32.f, -12.f), 40, 40, 1.7f, 0.f, 0.f, false, tIdx});
                                }
                            }
                        }
                        break;
                    }
                }

                // Sát thương lan lính địch trên boong nếu nổ gần
                for (auto& l : cacLinhThuyen) {
                    if (!l.daHySinh && KhoangCach(b.viTri, l.viTri) < 70.f) {
                        l.mau -= 40;
                        quanLyGiaoDien.ThemHieuUngChem(l.viTri);
                        quanLyGiaoDien.ThemSoSatThuong(l.viTri + sf::Vector2f(0.f, -15.f), 40);
                        if (l.mau <= 0) {
                            l.daHySinh = true;
                            soLinhThuyenDaHa++;
                            ThemChuBay(l.viTri + sf::Vector2f(0.f, -30.f), "Hỏa thiêu cung thủ!", sf::Color(255, 160, 40));
                            DongBoTienDo();
                        }
                    }
                }
            }
        } else {
            b.thoiGianNo -= deltaTime;
        }
    }
    cacBinhDau.erase(std::remove_if(cacBinhDau.begin(), cacBinhDau.end(), [](const BinhHoaCong& b) {
        return b.daNo && b.thoiGianNo <= 0.f;
    }), cacBinhDau.end());

    // Cập nhật mũi tên người chơi
    for (auto& m : cacMuiTen) {
        m.viTri += m.vanToc * deltaTime;
        m.tuoi += deltaTime;

        // Trúng lính địch trên boong thuyền
        for (auto& l : cacLinhThuyen) {
            if (!l.daHySinh && KhoangCach(m.viTri, l.viTri) < 28.f) {
                float tocDo = Dai(m.vanToc);
                int dame = (tocDo > 580.f) ? 40 : 25;
                l.mau -= dame;
                l.thoiGianBiThuong = 0.25f;
                quanLyGiaoDien.ThemHieuUngChem(l.viTri);
                quanLyGiaoDien.ThemSoSatThuong(l.viTri + sf::Vector2f(0.f, -15.f), dame);
                nguCanh.amThanh.PhatTiengTrungDon();
                m.tuoi = 99.f;
                if (l.mau <= 0) {
                    l.mau = 0;
                    l.daHySinh = true;
                    l.thoiGianChet = 0.f;
                    soLinhThuyenDaHa++;
                    ThemChuBay(l.viTri + sf::Vector2f(0.f, -30.f), "Hạ cung thủ!", sf::Color(255, 220, 100));
                    DongBoTienDo();
                }
                break;
            }
        }
    }
    cacMuiTen.erase(std::remove_if(cacMuiTen.begin(), cacMuiTen.end(), [](const MuiTenNguoiChoi& m) {
        return m.tuoi >= 2.5f;
    }), cacMuiTen.end());

    // Cập nhật hoạt ảnh và lính địch bắn trả
    for (auto& l : cacLinhThuyen) {
        if (l.thoiGianBiThuong > 0.f) l.thoiGianBiThuong -= deltaTime;
        if (l.daHySinh) {
            l.thoiGianChet += deltaTime;
            continue;
        }
        l.hoiBan -= deltaTime;
        if (l.hoiBan <= 0.f && thuyenTa) {
            l.hoiBan = 2.4f;
            sf::Vector2f huong = thuyenTa->GetViTri() - l.viTri;
            float d = Dai(huong);
            if (d > 1.f && d < 550.f) {
                cacTenDich.push_back({l.viTri, huong / d * 270.f, 0.f});
            }
        }
    }

    // Mũi tên địch bay trúng thuyền người chơi
    for (auto& td : cacTenDich) {
        td.viTri += td.vanToc * deltaTime;
        td.tuoi += deltaTime;
        if (thuyenTa && KhoangCach(td.viTri, thuyenTa->GetViTri()) < 28.f) {
            thuyenTa->NhanSatThuong(8);
            nguoiChoi.NhanSatThuong(8);
            quanLyGiaoDien.ThemHieuUngChem(thuyenTa->GetViTri());
            td.tuoi = 99.f;
            mayAnh.Rung(2.f, 0.1f);
        }
    }
    cacTenDich.erase(std::remove_if(cacTenDich.begin(), cacTenDich.end(), [](const MuiTenDich& td) {
        return td.tuoi >= 2.5f;
    }), cacTenDich.end());

    // Khi hạ đủ lính trên thuyền giặc: Chuyển sang Hồi 2
    if (giaiDoan == GiaiDoan::HOI_1_XA_TIEN && soLinhThuyenDaHa >= 6) {
        giaiDoan = GiaiDoan::CHUYEN_HOI;
        nguCanh.amThanh.PhatTiengTrongTrieuRut(); // Tiếng trống trận thúc giục triều rút dồn dập
        BatDauHoiThoai({
            {"Quân Tiên Phong", "Báo cáo! Lửa cháy ngút trời, thuyền giặc đã hoàn toàn tê liệt giữa bãi cọc!", LoaiNPC::LINH_DAI_VIET},
            {"Ngô Quyền", "Tốt lắm! Thừa thế địch hoảng loạn, toàn quân đổ bộ lên bãi cọc và mạn thuyền! Cận chiến giáp lá cà, bắt sống tướng giặc!", LoaiNPC::NGO_QUYEN}
        }, [this]() {
            KhoiTaoHoi2();
        });
    }
}

void ManChuong6::CapNhatHoi2(float deltaTime) {
    std::vector<sf::Vector2f> viTriCu;
    for (const auto& d : heThongChienDau.GetDanhSachKeDich()) viTriCu.push_back(d->GetViTri());

    heThongChienDau.CapNhat(deltaTime, nguoiChoi);

    const auto& danhSach = heThongChienDau.GetDanhSachKeDich();
    for (size_t i = 0; i < danhSach.size() && i < viTriCu.size(); ++i) {
        if (!danhSach[i]->IsDaBienMat()) GiuNgoaiVatCan(viTriCu[i], *danhSach[i]);
    }

    // Người chơi cận chiến
    int soTrung = heThongChienDau.XuLyPlayerTanCong(nguoiChoi.GetVungTanCong(), nguoiChoi.GetSatThuong(), nguoiChoi.IsDangTanCong());
    if (soTrung > 0) {
        sf::Vector2f diem = nguoiChoi.GetViTri() + sf::Vector2f(static_cast<float>(nguoiChoi.GetHuongNhin() * 30), 0.f);
        nguCanh.amThanh.PhatTiengTrungDon();
        quanLyGiaoDien.ThemHieuUngChem(diem);
        quanLyGiaoDien.ThemSoSatThuong(diem + sf::Vector2f(0.f, -20.f), nguoiChoi.GetSatThuong());
        mayAnh.Rung(2.8f, 0.12f);
    }

    // Kiểm tra chủ tướng tử trận
    if (!chuTuongDaChet && giaiDoan == GiaiDoan::HOI_2_NGO_QUYEN) {
        for (const auto& d : danhSach) {
            if (d->IsBoss() && d->IsDaHySinh()) {
                ChuTuongTuTran();
                break;
            }
        }
    }
}

void ManChuong6::SinhDichDot(int soLuong, int mau, int dame, float tocDo) {
    static unsigned int hat = 938u;
    for (int i = 0; i < soLuong; ++i) {
        const auto& tau = thuyenMacCan[static_cast<size_t>((soDichDaSinh + i) % thuyenMacCan.size())];
        sf::Vector2f p = ChonViTriTrong(tau->GetViTri() + sf::Vector2f(0.f, 30.f), 90.f, hat);
        heThongChienDau.ThemKeDich(p, tocDo + static_cast<float>(i % 3) * 6.f, mau, dame, LoaiKeDich::LINH_THUONG);
        quanLyGiaoDien.ThemHieuUngVaChamGo(p);
    }
    soDichDaSinh += soLuong;
}

void ManChuong6::SinhChuTuongVaNgoQuyen() {
    static unsigned int hat = 77u;
    sf::Vector2f pBoss = ChonViTriTrong(VI_TRI_SOAI_HAM + sf::Vector2f(0.f, 35.f), 45.f, hat);
    heThongChienDau.ThemKeDich(pBoss, 95.f, 320, 26, LoaiKeDich::TUONG_HOANG_THAO);

    for (int i = 0; i < SO_VE_BINH; ++i) {
        sf::Vector2f q = ChonViTriTrong(pBoss, 100.f, hat);
        heThongChienDau.ThemKeDich(q, 105.f, 85, 14, LoaiKeDich::LINH_THUONG);
    }

    // Chủ soái Ngô Quyền xuất trận sát cánh cùng người chơi
    sf::Vector2f pNgoQuyen = nguoiChoi.GetViTri() + sf::Vector2f(-40.f, 0.f);
    auto hinhNgoQuyen = std::make_unique<NhanVatPhu>(pNgoQuyen, LoaiNPC::NGO_QUYEN, "Ngô Quyền", std::vector<std::string>{"Theo ta giết giặc!"});
    quanTa.push_back({std::move(hinhNgoQuyen), 0.5f, true});

    ThemChuBay(pNgoQuyen + sf::Vector2f(0.f, -50.f), "NGÔ QUYỀN XUẤT TRẬN!", sf::Color(255, 215, 0));
    HienThongBao("Ngô Quyền đã đến! Hãy sát cánh cùng người hạ gục Lưu Hoằng Tháo!", 5.f);
}

void ManChuong6::SinhQuanTa() {
    static unsigned int hat = 5u;
    quanTa.clear();
    for (int i = 0; i < 6; ++i) {
        sf::Vector2f p(900.f + static_cast<float>(i) * 65.f, (i % 2 == 0) ? 360.f : 620.f);
        auto hinh = std::make_unique<NhanVatPhu>(p, LoaiNPC::LINH_DAI_VIET, "Quân Đại Việt", std::vector<std::string>{"Tiêu diệt quân Nam Hán!"});
        quanTa.push_back({std::move(hinh), NgauNhien(hat), false});
    }
}

void ManChuong6::CapNhatQuanTa(float deltaTime) {
    for (auto& linh : quanTa) {
        linh.hinh->CapNhat(deltaTime);
        linh.hoiDanh = std::max(0.f, linh.hoiDanh - deltaTime);

        // Tìm địch gần nhất (Ngô Quyền ưu tiên chém Boss nếu Boss xuất hiện)
        KeDich* gan = nullptr;
        float tot = 1e9f;
        for (const auto& d : heThongChienDau.GetDanhSachKeDich()) {
            if (d->IsDaBienMat()) continue;
            if (!linh.laNgoQuyen && d->IsBoss()) continue; // Lính thường không chạm boss
            float kc = KhoangCach(d->GetViTri(), linh.hinh->GetViTri());
            if (linh.laNgoQuyen && d->IsBoss()) kc *= 0.5f; // Ngô Quyền ưu tiên nhắm vào Hoằng Tháo
            if (kc < tot) {
                tot = kc;
                gan = d.get();
            }
        }
        if (!gan) continue;

        sf::Vector2f cu = linh.hinh->GetViTri();
        float cuLyChem = linh.laNgoQuyen ? 45.f : 36.f;
        if (tot > cuLyChem) {
            sf::Vector2f huong = gan->GetViTri() - cu;
            float d = Dai(huong);
            float toc = linh.laNgoQuyen ? 120.f : 90.f;
            if (d > 1.f) linh.hinh->SetViTri(cu + huong / d * toc * deltaTime);
            GiuNgoaiVatCan(cu, *linh.hinh);
        } else if (linh.hoiDanh <= 0.f) {
            int dame = linh.laNgoQuyen ? 18 : 8;
            gan->NhanSatThuong(dame);
            linh.hoiDanh = linh.laNgoQuyen ? 0.9f : 1.3f;
            quanLyGiaoDien.ThemHieuUngChem(gan->GetViTri());
            quanLyGiaoDien.ThemSoSatThuong(gan->GetViTri() + sf::Vector2f(0.f, -15.f), dame);
        }
    }
}

void ManChuong6::DongBoTienDo() {
    if (giaiDoan == GiaiDoan::HOI_1_PHONG_HOA) {
        if (soThuyenDaDot > soDichDaTinhTienDo) {
            quanLyNhiemVu.CapNhatTienDoNhiemVu(soThuyenDaDot - soDichDaTinhTienDo);
            soDichDaTinhTienDo = soThuyenDaDot;
        }
    } else if (giaiDoan == GiaiDoan::HOI_1_XA_TIEN) {
        if (soLinhThuyenDaHa > soDichDaTinhTienDo) {
            quanLyNhiemVu.CapNhatTienDoNhiemVu(soLinhThuyenDaHa - soDichDaTinhTienDo);
            soDichDaTinhTienDo = soLinhThuyenDaHa;
        }
    } else if (giaiDoan == GiaiDoan::HOI_2_DOT_1 || giaiDoan == GiaiDoan::HOI_2_DOT_2) {
        int tongChet = 0;
        for (const auto& d : heThongChienDau.GetDanhSachKeDich()) {
            if (d->IsDaHySinh() && !d->IsBoss()) tongChet++;
        }
        if (tongChet > soDichDaTinhTienDo) {
            quanLyNhiemVu.CapNhatTienDoNhiemVu(tongChet - soDichDaTinhTienDo);
            soDichDaTinhTienDo = tongChet;
        }
    }
}

void ManChuong6::KetThucDot() {
    nguoiChoi.HoiMau(60);
    ThemChuBay(nguoiChoi.GetViTri() + sf::Vector2f(0.f, -60.f), "+60 máu (Quân y tiếp tế)", sf::Color(130, 235, 150));
    nguCanh.amThanh.PhatTiengNhatDo();
    HienThongBao("Đợt này đã tan! Chuẩn bị lực lượng đối đầu đợt tiếp theo!", 3.5f);
}

void ManChuong6::ChuTuongTuTran() {
    chuTuongDaChet = true;
    thoiGianChamLai = 3.5f;              // Slow-motion điện ảnh
    mayAnh.Rung(8.f, 0.6f);
    nguCanh.amThanh.PhatTiengChienThang();
    quanLyNhiemVu.CapNhatTienDoNhiemVu(1);

    // Tiêu diệt toàn bộ tàn quân
    for (const auto& d : heThongChienDau.GetDanhSachKeDich()) {
        if (!d->IsDaHySinh() && !d->IsBoss()) d->Chet();
    }

    ThemChuBay(VI_TRI_SOAI_HAM + sf::Vector2f(0.f, -70.f), "LƯU HOẰNG THÁO TỬ TRẬN!", sf::Color(255, 215, 0));
    HienThongBao("Chiến dịch toàn thắng! Toàn bộ thủy quân Nam Hán đã bị chôn vùi!", 4.f);
}

void ManChuong6::KetThucChuong() {
    giaiDoan = GiaiDoan::KET_THUC;
    BatDauHoiThoai({
        {"Ngô Quyền", "Lưu Hoằng Tháo đã đền tội dưới đáy sông Bạch Đằng! Toàn bộ thủy quân Nam Hán đã bị chôn vùi!", LoaiNPC::NGO_QUYEN},
        {"Ngô Quyền", "Hơn một nghìn năm Bắc thuộc từ nay chấm dứt. Non sông gấm vóc Đại Việt muôn đời độc lập!", LoaiNPC::NGO_QUYEN}
    }, [this]() {
        HoanThanhChuong("ĐẠI THẮNG BẠCH ĐẰNG NĂM 938",
            "Chiến dịch Bạch Đằng toàn thắng rực rỡ. Lưu Hoằng Tháo bị tiêu diệt, "
            "vua Nam Hán khóc ròng thu tàn quân lui về nước. "
            "Chiến công vang dội chấm dứt hơn 1000 năm đô hộ phong kiến phương Bắc, "
            "mở ra kỷ nguyên độc lập tự chủ lâu dài cho dân tộc Việt Nam.");
    });
}

void ManChuong6::CapNhatRieng(float deltaTime) {
    banDo.CapNhat(deltaTime);

    // Cập nhật hiệu ứng quay chậm khi Lưu Hoằng Tháo ngã xuống
    float dt = deltaTime;
    if (thoiGianChamLai > 0.f) {
        thoiGianChamLai -= deltaTime;
        dt = deltaTime * 0.35f;
        if (thoiGianChamLai <= 0.f) {
            bool daMo = trinhChieuVideo.MoVideo("ChienThangBachDang.mpg", "ChienThangBachDang.ogg");
            if (!daMo) {
                KetThucChuong();
            }
            return;
        }
    }

    switch (giaiDoan) {
        case GiaiDoan::HOI_1_PHONG_HOA:
        case GiaiDoan::HOI_1_XA_TIEN:
            CapNhatHoi1(dt);
            break;

        case GiaiDoan::CHUYEN_HOI:
            break;

        case GiaiDoan::HOI_2_DOT_1:
            CapNhatHoi2(dt);
            CapNhatQuanTa(dt);
            DongBoTienDo();
            if (heThongChienDau.GetSoKeDichConLai() <= 0) {
                KetThucDot();
                giaiDoan = GiaiDoan::HOI_2_NGHI_1;
                demGiay = 0.f;
            }
            break;

        case GiaiDoan::HOI_2_NGHI_1:
            demGiay += dt;
            CapNhatQuanTa(dt);
            if (demGiay >= THOI_GIAN_NGHI) {
                BatDauDot(GiaiDoan::HOI_2_DOT_2);
            }
            break;

        case GiaiDoan::HOI_2_DOT_2:
            CapNhatHoi2(dt);
            CapNhatQuanTa(dt);
            DongBoTienDo();
            if (heThongChienDau.GetSoKeDichConLai() <= 0) {
                KetThucDot();
                giaiDoan = GiaiDoan::HOI_2_NGO_QUYEN;
                SinhChuTuongVaNgoQuyen();
            }
            break;

        case GiaiDoan::HOI_2_NGO_QUYEN:
            CapNhatHoi2(dt);
            CapNhatQuanTa(dt);
            break;

        case GiaiDoan::KET_THUC:
            break;
    }
}

void ManChuong6::BatDauDot(GiaiDoan dot) {
    giaiDoan = dot;
    demGiay = 0.f;
    if (dot == GiaiDoan::HOI_2_DOT_1) {
        SinhDichDot(SO_DICH_DOT_1, 65, 12, 85.f);
        HienThongBao("Đợt 1: Quân giặc từ các chiến thuyền mắc cạn tràn xuống!", 4.f);
    } else if (dot == GiaiDoan::HOI_2_DOT_2) {
        SinhDichDot(SO_DICH_DOT_2, 80, 15, 95.f);
        HienThongBao("Đợt 2: Hộ vệ Nam Hán tinh nhuệ đổ bộ! Cẩn thận đao khiên giặc!", 4.f);
    }
}

sf::Vector2f ManChuong6::ViTriMayAnhTheoDoi() const {
    if (cheDoThuyen && thuyenTa) {
        return thuyenTa->GetViTri();
    }
    return nguoiChoi.GetViTri();
}

void ManChuong6::VeNenDong() {
    banDo.Ve(window);
}

void ManChuong6::ThemMucVeRieng(std::vector<std::pair<float, std::function<void()>>>& danhSach) {
    // Vẽ chiến thuyền giặc mắc cạn
    for (auto& t : thuyenMacCan) {
        ThuyenKeDich* tau = t.get();
        danhSach.push_back({tau->GetViTri().y, [this, tau]() { tau->Ve(window); }});
    }

    // Hồi 1: Vẽ thuyền nhẹ người chơi
    if (cheDoThuyen && thuyenTa) {
        danhSach.push_back({thuyenTa->GetViTri().y, [this]() { thuyenTa->Ve(window); }});
    }

    // Hồi 2: Vẽ quân Đại Việt và Ngô Quyền
    for (auto& linh : quanTa) {
        NhanVatPhu* p = linh.hinh.get();
        danhSach.push_back({p->GetViTri().y, [this, p]() { p->Ve(window); }});
    }

    // Vẽ lính trên boong thuyền giặc ở Hồi 1
    for (const auto& l : cacLinhThuyen) {
        if (l.thoiGianChet < 0.8f) {
            // Độ sâu vẽ lớn hơn thuyền để lính được vẽ TRÊN MẶT BOONG THUYỀN!
            float ySauThuyen = 700.f;
            if (l.chiSoThuyen < thuyenMacCan.size()) {
                ySauThuyen = thuyenMacCan[l.chiSoThuyen]->GetViTri().y + 25.f;
            }
            danhSach.push_back({ySauThuyen, [this, &l]() {
                VeCungThuBoongTau(window, l);
            }});
        }
    }
}

void ManChuong6::VeTrongTheGioi() {
    // Tiền cảnh hoàng hôn từ BanDo: ánh nắng chiếu xiên, sương mù, lau sậy
    banDo.VeTienCanh(window);

    // Vẽ bình hỏa công bay và phát nổ
    for (const auto& b : cacBinhDau) {
        if (!b.daNo) {
            sf::Vector2f pos = b.viTri + sf::Vector2f(0.f, -b.doCao);
            sf::CircleShape binh(6.f);
            binh.setOrigin({6.f, 6.f});
            binh.setPosition(pos);
            binh.setFillColor(sf::Color(255, 120, 20));
            binh.setOutlineThickness(1.5f);
            binh.setOutlineColor(sf::Color(255, 230, 100));
            window.draw(binh);

            // Vệt khói lửa phía sau bình
            sf::CircleShape lua(4.f);
            lua.setOrigin({4.f, 4.f});
            lua.setPosition(pos - b.vanToc * 0.04f);
            lua.setFillColor(sf::Color(255, 60, 10, 160));
            window.draw(lua);
        }
    }

    // Vẽ mũi tên người chơi
    if (!cacMuiTen.empty()) {
        VA va(sf::PrimitiveType::Triangles);
        for (const auto& m : cacMuiTen) {
            float d = Dai(m.vanToc);
            if (d < 1.f) continue;
            sf::Vector2f h = m.vanToc / d;
            sf::Vector2f duoi = m.viTri - h * 20.f;
            duong(va, duoi, m.viTri, 2.2f, mau(255, 160, 40));
            sf::Vector2f vuong(-h.y, h.x);
            tam(va, m.viTri + h * 6.f, m.viTri + vuong * 3.f, m.viTri - vuong * 3.f, mau(255, 240, 180));
        }
        window.draw(va);
    }

    // Vẽ mũi tên địch
    if (!cacTenDich.empty()) {
        VA va(sf::PrimitiveType::Triangles);
        for (const auto& td : cacTenDich) {
            float d = Dai(td.vanToc);
            if (d < 1.f) continue;
            sf::Vector2f h = td.vanToc / d;
            sf::Vector2f duoi = td.viTri - h * 16.f;
            duong(va, duoi, td.viTri, 1.8f, mau(70, 50, 40));
            sf::Vector2f vuong(-h.y, h.x);
            tam(va, td.viTri + h * 5.f, td.viTri + vuong * 2.5f, td.viTri - vuong * 2.5f, mau(200, 200, 210));
        }
        window.draw(va);
    }

    // Vẽ đường ngắm và vùng nổ của bình hỏa công khi ở trên thuyền
    if (cheDoThuyen && thuyenTa && !dangNgamBan) {
        sf::Vector2f start = thuyenTa->GetViTri();
        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        sf::Vector2f mouseWorld = mayAnh.ChuotSangTheGioi(window, mousePixel);
        sf::Vector2f huong = mouseWorld - start;
        float dist = Dai(huong);
        if (dist > 1.f) {
            huong /= dist;
            sf::Vector2f dich = (dist > TAM_NEM_TOI_DA) ? (start + huong * TAM_NEM_TOI_DA) : mouseWorld;
            float culy = std::min(dist, TAM_NEM_TOI_DA);

            bool ganThuyenChuaChay = false;
            for (const auto& tau : thuyenMacCan) {
                if (!tau->IsDangChay()) {
                    sf::Vector2f vt = tau->GetViTri();
                    float dx = std::abs(dich.x - vt.x);
                    float dy = std::abs(dich.y - vt.y);
                    if ((dx < 75.f && dy < 35.f) || KhoangCach(dich, vt) < 95.f) {
                        ganThuyenChuaChay = true;
                        break;
                    }
                }
            }

            sf::Color mauVien = ganThuyenChuaChay ? sf::Color(255, 230, 40) : sf::Color(255, 140, 40, 200);
            sf::Color mauNen = ganThuyenChuaChay ? sf::Color(255, 200, 30, 50) : sf::Color(255, 90, 20, 35);

            // Vòng tròn vùng nổ tại điểm đích
            sf::CircleShape vungNo(36.f);
            vungNo.setOrigin({36.f, 36.f});
            vungNo.setPosition(dich);
            vungNo.setFillColor(mauNen);
            vungNo.setOutlineThickness(1.8f);
            vungNo.setOutlineColor(mauVien);
            window.draw(vungNo);

            // Tâm ngắm chữ thập
            sf::VertexArray tamNgam(sf::PrimitiveType::Lines, 4);
            tamNgam[0].position = dich + sf::Vector2f(-9.f, 0.f);
            tamNgam[0].color = mauVien;
            tamNgam[1].position = dich + sf::Vector2f(9.f, 0.f);
            tamNgam[1].color = mauVien;
            tamNgam[2].position = dich + sf::Vector2f(0.f, -9.f);
            tamNgam[2].color = mauVien;
            tamNgam[3].position = dich + sf::Vector2f(0.f, 9.f);
            tamNgam[3].color = mauVien;
            window.draw(tamNgam);

            // Đường vòng cung parabol tượng trưng từ thuyền tới đích
            const int SO_DIEM_CUNG = 12;
            for (int k = 1; k < SO_DIEM_CUNG; ++k) {
                float tDiem = static_cast<float>(k) / SO_DIEM_CUNG;
                sf::Vector2f pMatDat = start + (dich - start) * tDiem;
                float hMax = 35.f + (culy / TAM_NEM_TOI_DA) * 35.f;
                float z = 4.f * hMax * tDiem * (1.f - tDiem);
                sf::Vector2f pVe = pMatDat + sf::Vector2f(0.f, -z);

                sf::CircleShape hat(2.5f);
                hat.setOrigin({2.5f, 2.5f});
                hat.setPosition(pVe);
                hat.setFillColor(sf::Color(255, 170, 50, static_cast<std::uint8_t>(200 * (1.f - tDiem * 0.2f))));
                window.draw(hat);
            }
        }
    }

    // Vẽ đường ngắm bắn cung khi đang giữ chuột trái
    if (dangNgamBan && thuyenTa) {
        sf::Vector2f start = thuyenTa->GetViTri();
        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        sf::Vector2f mouseWorld = mayAnh.ChuotSangTheGioi(window, mousePixel);
        sf::Vector2f huong = mouseWorld - start;
        float d = Dai(huong);
        if (d > 1.f) {
            huong /= d;
            float maxLen = 100.f + lucKeoCung * 250.f;
            for (float s = 25.f; s < maxLen; s += 24.f) {
                sf::CircleShape dot(3.2f);
                dot.setOrigin({3.2f, 3.2f});
                dot.setPosition(start + huong * s);
                dot.setFillColor(sf::Color(255, 220, 50, static_cast<std::uint8_t>(220.f * (1.f - s / maxLen))));
                window.draw(dot);
            }
            // Tâm ngắm ở đầu đường đạn
            sf::CircleShape reticle(6.f);
            reticle.setOrigin({6.f, 6.f});
            reticle.setPosition(start + huong * maxLen);
            reticle.setFillColor(sf::Color::Transparent);
            reticle.setOutlineThickness(1.8f);
            reticle.setOutlineColor(sf::Color(255, 230, 80));
            window.draw(reticle);
        }
    }
}

void ManChuong6::VeThongTinPhu() {
    heThongChienDau.VeBossBar(window, nguCanh.phongChu);

    // Gợi ý điều khiển đặc biệt theo từng hồi
    if (giaiDoan == GiaiDoan::HOI_1_PHONG_HOA) {
        VeHopChu("HỒI 1 - HỎA CÔNG: Áp sát thuyền giặc, nhấn F hoặc Chuột phải để ném BÌNH HỎA CÔNG", 13,
                 {RONG_MAN_HINH * 0.5f, CAO_MAN_HINH - 35.f}, sf::Color(255, 220, 100), sf::Color(20, 20, 30, 210));
    } else if (giaiDoan == GiaiDoan::HOI_1_XA_TIEN) {
        std::string thongTin = "HỒI 1 - XẠ TIỄN: Tiêu diệt cung thủ địch (" + std::to_string(soLinhThuyenDaHa) + "/6) | Giữ Chuột trái kéo cung, nhả để bắn";
        VeHopChu(thongTin, 13,
                 {RONG_MAN_HINH * 0.5f, CAO_MAN_HINH - 35.f}, sf::Color(255, 220, 80), sf::Color(20, 20, 30, 210));

        // Mũi tên chỉ hướng nếu cung thủ nằm ngoài khung nhìn camera
        sf::Vector2f tamMayAnh = mayAnh.GetTam();
        float nuaRong = RONG_MAN_HINH * 0.5f;
        float nuaCao = CAO_MAN_HINH * 0.5f;
        sf::FloatRect vungNhin({tamMayAnh.x - nuaRong, tamMayAnh.y - nuaCao}, {RONG_MAN_HINH, CAO_MAN_HINH});

        for (const auto& l : cacLinhThuyen) {
            if (!l.daHySinh && !vungNhin.contains(l.viTri)) {
                sf::Vector2f huong = l.viTri - tamMayAnh;
                float d = Dai(huong);
                if (d > 1.f) {
                    huong /= d;
                    float cx = std::clamp(nuaRong + huong.x * (nuaRong - 45.f), 45.f, RONG_MAN_HINH - 45.f);
                    float cy = std::clamp(nuaCao + huong.y * (nuaCao - 55.f), 45.f, CAO_MAN_HINH - 55.f);

                    sf::CircleShape muiTenChi(6.f);
                    muiTenChi.setOrigin({6.f, 6.f});
                    muiTenChi.setPosition({cx, cy});
                    muiTenChi.setFillColor(sf::Color(255, 50, 30));
                    muiTenChi.setOutlineThickness(1.8f);
                    muiTenChi.setOutlineColor(sf::Color(255, 230, 80));
                    window.draw(muiTenChi);
                }
            }
        }
    }
}

void ManChuong6::VeCungThuBoongTau(sf::RenderWindow& window, const LinhTrenThuyen& l) {
    if (l.thoiGianChet >= 0.8f) return;

    float t = demGiay;
    sf::Vector2f pos = l.viTri;
    float flash = (l.thoiGianBiThuong > 0.f) ? 0.8f : 0.f;
    float alpha = l.daHySinh ? std::max(0.f, 1.f - l.thoiGianChet / 0.8f) : 1.f;
    if (alpha <= 0.f) return;

    // Bóng đổ trên sàn boong tàu
    sf::CircleShape bong(9.f);
    bong.setScale({1.3f, 0.55f});
    bong.setOrigin({9.f, 9.f});
    bong.setPosition(pos + sf::Vector2f(0.f, 14.f));
    bong.setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(90 * alpha)));
    window.draw(bong);

    // Hướng nhìn về phía thuyền ta
    float huongNhin = -1.f;
    if (thuyenTa) {
        huongNhin = (thuyenTa->GetViTri().x >= pos.x) ? 1.f : -1.f;
    }

    auto MauSang = [flash, alpha](sf::Color col) -> sf::Color {
        if (flash > 0.f) {
            return sf::Color(
                static_cast<std::uint8_t>(std::min(255.f, col.r + 255.f * flash)),
                static_cast<std::uint8_t>(std::min(255.f, col.g + 255.f * flash)),
                static_cast<std::uint8_t>(std::min(255.f, col.b + 255.f * flash)),
                static_cast<std::uint8_t>(col.a * alpha)
            );
        }
        return sf::Color(col.r, col.g, col.b, static_cast<std::uint8_t>(col.a * alpha));
    };

    sf::Color kSat = MauSang(sf::Color(75, 80, 95));       // Giáp sắt
    sf::Color kDo = MauSang(sf::Color(190, 40, 35));       // Viền đỏ Nam Hán
    sf::Color kVang = MauSang(sf::Color(225, 175, 45));    // Chi tiết vàng đồng
    sf::Color kDa = MauSang(sf::Color(230, 190, 155));     // Da mặt
    sf::Color kQuan = MauSang(sf::Color(45, 40, 48));      // Quần tối
    sf::Color kGo = MauSang(sf::Color(120, 75, 45));       // Cung gỗ

    // 1. Chân & ủng da
    sf::RectangleShape chanP({5.f, 10.f});
    chanP.setOrigin({2.5f, 0.f});
    chanP.setPosition(pos + sf::Vector2f(huongNhin * 3.f, 4.f));
    chanP.setFillColor(kQuan);
    window.draw(chanP);

    sf::RectangleShape chanT({5.f, 10.f});
    chanT.setOrigin({2.5f, 0.f});
    chanT.setPosition(pos + sf::Vector2f(-huongNhin * 4.f, 4.f));
    chanT.setFillColor(kQuan);
    window.draw(chanT);

    // 2. Thân mình / Áo giáp vảy
    sf::RectangleShape than({15.f, 16.f});
    than.setOrigin({7.5f, 8.f});
    than.setPosition(pos + sf::Vector2f(0.f, -1.f));
    than.setFillColor(kSat);
    than.setOutlineThickness(1.5f);
    than.setOutlineColor(kDo);
    window.draw(than);

    // Đai lưng đỏ
    sf::RectangleShape dai({16.f, 3.5f});
    dai.setOrigin({8.f, 1.75f});
    dai.setPosition(pos + sf::Vector2f(0.f, 3.5f));
    dai.setFillColor(kDo);
    window.draw(dai);

    // 3. Đầu & Khuôn mặt
    sf::CircleShape mat(6.f);
    mat.setOrigin({6.f, 6.f});
    mat.setPosition(pos + sf::Vector2f(0.f, -12.f));
    mat.setFillColor(kDa);
    window.draw(mat);

    // Mũ sắt Nam Hán với chóp nhọn
    sf::ConvexShape mu;
    mu.setPointCount(3);
    mu.setPoint(0, sf::Vector2f(-7.f, 0.f));
    mu.setPoint(1, sf::Vector2f(7.f, 0.f));
    mu.setPoint(2, sf::Vector2f(0.f, -10.f));
    mu.setPosition(pos + sf::Vector2f(0.f, -14.f));
    mu.setFillColor(kSat);
    mu.setOutlineThickness(1.2f);
    mu.setOutlineColor(kVang);
    window.draw(mu);

    // Lông mũ đỏ rực trên chóp mũ
    sf::RectangleShape longMu({3.f, 7.f});
    longMu.setOrigin({1.5f, 7.f});
    longMu.setPosition(pos + sf::Vector2f(0.f, -23.f));
    longMu.setFillColor(MauSang(sf::Color(230, 45, 30)));
    longMu.setRotation(sf::degrees(huongNhin * -15.f + std::sin(t * 8.f) * 10.f));
    window.draw(longMu);

    // 4. Cây cung chiến
    sf::Vector2f tayCung = pos + sf::Vector2f(huongNhin * 9.f, -2.f);
    float doCang = (l.hoiBan < 0.7f && !l.daHySinh) ? 1.4f : 1.0f;
    sf::VertexArray canhCung(sf::PrimitiveType::LineStrip, 5);
    canhCung[0].position = tayCung + sf::Vector2f(-huongNhin * 2.f, -14.f);
    canhCung[1].position = tayCung + sf::Vector2f(huongNhin * 5.f * doCang, -7.f);
    canhCung[2].position = tayCung + sf::Vector2f(huongNhin * 7.f * doCang, 0.f);
    canhCung[3].position = tayCung + sf::Vector2f(huongNhin * 5.f * doCang, 7.f);
    canhCung[4].position = tayCung + sf::Vector2f(-huongNhin * 2.f, 14.f);
    for (size_t i = 0; i < 5; ++i) canhCung[i].color = kGo;
    window.draw(canhCung);

    sf::VertexArray dayCung(sf::PrimitiveType::Lines, 2);
    dayCung[0].position = canhCung[0].position;
    dayCung[0].color = MauSang(sf::Color(220, 220, 220));
    dayCung[1].position = canhCung[4].position;
    dayCung[1].color = MauSang(sf::Color(220, 220, 220));
    window.draw(dayCung);

    // 5. Thanh máu mini & Ký hiệu mục tiêu trên đầu (khi còn sống)
    if (!l.daHySinh) {
        const float rongBar = 32.f;
        const float caoBar = 4.5f;
        sf::Vector2f gocBar = pos + sf::Vector2f(-rongBar * 0.5f, -34.f);

        // Nền đen
        sf::RectangleShape nenBar({rongBar, caoBar});
        nenBar.setPosition(gocBar);
        nenBar.setFillColor(sf::Color(20, 15, 20, 220));
        nenBar.setOutlineThickness(1.f);
        nenBar.setOutlineColor(sf::Color(230, 180, 40));
        window.draw(nenBar);

        // Máu hiện tại
        float tiLeMau = std::clamp(static_cast<float>(l.mau) / static_cast<float>(l.mauToiDa), 0.f, 1.f);
        sf::RectangleShape mauFill({(rongBar - 1.f) * tiLeMau, caoBar - 1.f});
        mauFill.setPosition(gocBar + sf::Vector2f(0.5f, 0.5f));
        mauFill.setFillColor(tiLeMau > 0.5f ? sf::Color(230, 50, 40) : sf::Color(255, 120, 20));
        window.draw(mauFill);

        // Biểu tượng mục tiêu nhấp nháy trên đầu (Diamond target)
        float nhapNhay = 0.5f + 0.5f * std::sin(t * 7.f);
        sf::ConvexShape tamDich;
        tamDich.setPointCount(4);
        tamDich.setPoint(0, sf::Vector2f(0.f, -5.f));
        tamDich.setPoint(1, sf::Vector2f(5.f, 0.f));
        tamDich.setPoint(2, sf::Vector2f(0.f, 5.f));
        tamDich.setPoint(3, sf::Vector2f(-5.f, 0.f));
        tamDich.setPosition(pos + sf::Vector2f(0.f, -44.f - nhapNhay * 3.f));
        tamDich.setFillColor(sf::Color(255, 50, 30, static_cast<std::uint8_t>(200 + 55 * nhapNhay)));
        tamDich.setOutlineThickness(1.2f);
        tamDich.setOutlineColor(sf::Color(255, 230, 80));
        window.draw(tamDich);
    }
}

void ManChuong6::CapNhat(float deltaTime) {
    if (trinhChieuVideo.IsDangPhat()) {
        trinhChieuVideo.CapNhat(deltaTime);
        if (trinhChieuVideo.IsDaXong()) {
            KetThucChuong();
        }
        return;
    }
    ManChoiDiBo::CapNhat(deltaTime);
}

void ManChuong6::Ve() {
    if (trinhChieuVideo.IsDangPhat()) {
        trinhChieuVideo.Ve(window, nguCanh.phongChu);
        return;
    }
    ManChoiDiBo::Ve();
}

