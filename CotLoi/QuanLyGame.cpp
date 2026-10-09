#include "QuanLyGame.h"
#include <cmath>
#include <iostream>
#include <optional>
#include <string>

static float KhoangCach(const sf::Vector2f& a, const sf::Vector2f& b)
{
    float dx = a.x - b.x, dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

QuanLyGame::QuanLyGame()
    : cuaSo(sf::VideoMode({1280u, 720u}), "Bach Dang 938"),
      nguoiChoi(640.f, 360.f),
      soCocDaNhat(0)
{
    cuaSo.setFramerateLimit(60);

    // Gioi han ban do = ca cua so; vai vat can thu nghiem
    nguoiChoi.DatGioiHanBanDo(sf::FloatRect({0.f, 0.f}, {1280.f, 720.f}));
    vatCan.push_back(sf::FloatRect({300.f, 200.f}, {160.f, 90.f}));
    vatCan.push_back(sf::FloatRect({850.f, 380.f}, {70.f, 220.f}));
    nguoiChoi.DatDanhSachVatCan(&vatCan);

    // 3 coc go de thu E + NhatVatPham
    viTriCoc = { {200.f, 520.f}, {1000.f, 150.f}, {640.f, 640.f} };
    daNhatCoc.assign(viTriCoc.size(), false);

    // Moc noi: khi nguoi choi nhat do -> (sau nay goi tuDo.ThemVatPham(...) cua Yen)
    nguoiChoi.DatKhiNhatVatPham([this](int ma, const std::string& ten) {
        ++soCocDaNhat;
        std::cout << "Nhat duoc vat pham #" << ma << " (" << ten << ") -> " << soCocDaNhat << "/3\n";
    });

    // Vi du dang ky "lang nghe" khi doi trang thai (Ngan, Yen, Nhan se lam tuong tu)
    tranDau.DangKyLangNghe([this](TrangThaiTranDau cu, TrangThaiTranDau moi) {
        std::cout << "Trang thai tran dau: " << TenTrangThai(cu) << " -> " << TenTrangThai(moi) << "\n";
        if (moi == TrangThaiTranDau::PHAN_CONG)
            heThongChienDau.TaoTranChienCuoi(5);     // Nhan: sinh 5 linh Nam Han khi tong phan cong
    });
}

void QuanLyGame::XuLySuKien()
{
    while (const std::optional sk = cuaSo.pollEvent()) {
        if (sk->is<sf::Event::Closed>()) cuaSo.close();

        if (const auto* phim = sk->getIf<sf::Event::KeyPressed>()) {
            if (phim->code == sf::Keyboard::Key::Escape) cuaSo.close();
            if (phim->code == sf::Keyboard::Key::H) nguoiChoi.NhanSatThuong(10);   // thu mat mau
            if (phim->code == sf::Keyboard::Key::N) tranDau.TiepTheo();            // thu sang trang thai ke tiep
            if (phim->code == sf::Keyboard::Key::P) tranDau.ChuyenTrangThai(TrangThaiTranDau::PHAN_CONG);   // thu: nhay thang toi tran danh cuoi
        }
    }
}

void QuanLyGame::CapNhat(float dt)
{
    nguoiChoi.XuLyPhim();
    nguoiChoi.CapNhat(dt);
    tranDau.CapNhat(dt);

    // Bam E gan coc -> nhat coc
    if (nguoiChoi.IsDaBamTuongTac()) {
        for (std::size_t i = 0; i < viTriCoc.size(); ++i) {
            if (!daNhatCoc[i] && KhoangCach(nguoiChoi.GetViTri(), viTriCoc[i]) < 60.f) {
                daNhatCoc[i] = true;
                nguoiChoi.NhatVatPham(1, "Coc go");
                break;
            }
        }
    }

    // Du 3 coc -> sang buoc tiep theo (sau nay do NhiemVu cua Trang quyet dinh)
    if (soCocDaNhat >= 3 && tranDau.Is(TrangThaiTranDau::CHUAN_BI))
        tranDau.ChuyenTrangThai(TrangThaiTranDau::QUAN_DICH_XUAT_HIEN);

    // Tong phan cong: quan dich duoi + danh nguoi choi; nguoi choi chem tru mau dich
    if (tranDau.Is(TrangThaiTranDau::PHAN_CONG)) {
        heThongChienDau.CapNhat(dt, nguoiChoi);
        int soDichTrung = heThongChienDau.XuLyPlayerTanCong(nguoiChoi.GetVungTanCong(),
                                                             nguoiChoi.GetSatThuong(),
                                                             nguoiChoi.IsDangTanCong());
        (void)soDichTrung;   // Ngan: if (soDichTrung > 0) phat am thanh chem trung

        if (heThongChienDau.KiemTraChienThang())
            tranDau.ChuyenTrangThai(TrangThaiTranDau::CHIEN_THANG);
    }

    if (nguoiChoi.IsDaHySinh()) {
        cuaSo.setTitle("Bach Dang 938 - NGUOI CHOI DA HY SINH");
    } else {
        cuaSo.setTitle(std::string("Bach Dang 938 | ") + TenTrangThai(tranDau.GetTrangThai()) +
                       " | HP " + std::to_string(nguoiChoi.GetSucKhoe()) +
                       " | The luc " + std::to_string(static_cast<int>(nguoiChoi.GetTheLuc())) +
                       " | Coc " + std::to_string(soCocDaNhat) + "/3" +
                       (tranDau.Is(TrangThaiTranDau::PHAN_CONG)
                            ? " | Dich con " + std::to_string(heThongChienDau.GetSoKeDichConLai()) : ""));
    }
}

void QuanLyGame::Ve()
{
    cuaSo.clear(sf::Color(30, 60, 90));

    for (const sf::FloatRect& vc : vatCan) {
        sf::RectangleShape o(vc.size);
        o.setPosition(vc.position);
        o.setFillColor(sf::Color(110, 80, 50));
        cuaSo.draw(o);
    }
    for (std::size_t i = 0; i < viTriCoc.size(); ++i) {
        if (daNhatCoc[i]) continue;
        sf::RectangleShape coc({14.f, 40.f});
        coc.setOrigin({7.f, 20.f});
        coc.setPosition(viTriCoc[i]);
        coc.setFillColor(sf::Color(200, 170, 110));
        cuaSo.draw(coc);
    }

    heThongChienDau.Ve(cuaSo);
    nguoiChoi.Ve(cuaSo);
    cuaSo.display();
}

void QuanLyGame::Chay()
{
    while (cuaSo.isOpen()) {
        float dt = dongHo.restart().asSeconds();
        XuLySuKien();
        CapNhat(dt);
        Ve();
    }
}
