#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>
#include <optional>
#include <string>

#include "BanDo/BanDo.h"
#include "DoiTuong/TuDo.h"
#include "DoiTuong/VatPham.h"
#include "DoiTuong/ThuyTrieu.h"
#include "NhanVat/NguoiChoi.h"
#include "NhanVat/NhanVatPhu.h"
#include "ChienDau/HeThongChienDau.h"
#include "Nhiemvu/Quanlynhiemvu.h"
#include "Thuyen/Thuyen.h"
#include "CotLoi/QuanLyTranDau.h"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({1200, 700}),
        "Bach Dang Giang 938 - Hop nhat tat ca phan he"
    );
    window.setFramerateLimit(60);

    // 1. Bản đồ & Thủy triều
    BanDo banDo;
    ThuyTrieu thuyTrieu;
    thuyTrieu.BatDauRutNuoc();

    // 2. Nhân vật người chơi
    NguoiChoi nguoiChoi(350.f, 320.f);

    // 3. Nhân vật phụ (NPC) & Hệ thống hội thoại
    std::string tenNgoQuyen = "Ngô Quyền";
    std::string tenDanLang = "Dân làng";
    NhanVatPhu npcNgoQuyen(200.f, 220.f, tenNgoQuyen, 85.f);
    NhanVatPhu npcDanLang(480.f, 220.f, tenDanLang, 85.f);

    npcNgoQuyen.CapNhapcauthoaitheoflow({
        "Lưu Hoằng Tháo dẫn đầu chiến thuyền Nam Hán sắp tràn vào cửa biển!",
        "Ngươi hãy ra bờ sông phối hợp cùng bà con cắm cọc sắt chuẩn bị trận địa!",
        "Đợi khi thủy triều rút cạn, toàn quân ta sẽ tổng phản công tiêu diệt giặc!"
    });

    npcDanLang.CapNhapcauthoaitheoflow({
        "Bà con đã đẵn gỗ và bịt sắt sẵn sàng đầu cọc rồi!",
        "Anh hãy mau nhặt các cọc gỗ đóng chặt xuống lòng sông trước khi triều lên!"
    });

    // 4. Hệ thống nhiệm vụ 5 Hồi game
    Quanlynhiemvu qlNhiemVu;

    // 5. Quản lý trạng thái trận đấu
    QuanLyTranDau tranDau;

    // 6. Thuyền người chơi 8 hướng (Thủy chiến)
    Thuyen thuyenPlayer({550.f, 340.f}, 170.f, 200, sf::Color::Cyan);
    bool dangLaiThuyen = false;

    // 7. Hệ thống chiến đấu (Bộ binh & Hải chiến)
    HeThongChienDau heThongChienDau;
    // Khởi tạo lính Nam Hán tuần tra trên bộ
    heThongChienDau.ThemKeDich({700.f, 260.f}, 95.f, 60, 10);
    heThongChienDau.ThemKeDich({840.f, 350.f}, 95.f, 60, 10);
    heThongChienDau.ThemKeDich({620.f, 480.f}, 95.f, 60, 10);

    // Khởi tạo hạm đội chiến thuyền giặc Nam Hán
    heThongChienDau.TaoDoanThuyenNamHan(4);

    // 8. Tải phông chữ hiển thị
    sf::Font font;
    const bool fontDaTai = font.openFromFile("Assets/Fonts/BeVietnamPro-Regular.ttf");

    // Các thành phần giao diện HUD
    sf::Text thongBao(font);
    sf::Text thongBaoNhat(font);
    sf::Text huongDan(font);
    sf::Text thongTinNhiemVu(font);

    sf::RectangleShape nenThanhTienDo;
    sf::RectangleShape thanhTienDo;

    float thoiGianThongBao = 0.f;
    std::string noiDungThongBao;

    if (fontDaTai)
    {
        thongBao.setCharacterSize(20);
        thongBao.setFillColor(sf::Color::White);
        thongBao.setOutlineColor(sf::Color::Black);
        thongBao.setOutlineThickness(2.f);
        thongBao.setPosition({24.f, 15.f});

        thongTinNhiemVu.setCharacterSize(16);
        thongTinNhiemVu.setFillColor(sf::Color(255, 230, 120));
        thongTinNhiemVu.setOutlineColor(sf::Color::Black);
        thongTinNhiemVu.setOutlineThickness(1.5f);
        thongTinNhiemVu.setPosition({24.f, 42.f});

        thongBaoNhat.setCharacterSize(17);
        thongBaoNhat.setFillColor(sf::Color(255, 220, 140));
        thongBaoNhat.setOutlineColor(sf::Color::Black);
        thongBaoNhat.setOutlineThickness(2.f);
        thongBaoNhat.setPosition({24.f, 98.f});

        nenThanhTienDo.setSize({240.f, 14.f});
        nenThanhTienDo.setPosition({24.f, 74.f});
        nenThanhTienDo.setFillColor(sf::Color(45, 45, 45));
        nenThanhTienDo.setOutlineColor(sf::Color::White);
        nenThanhTienDo.setOutlineThickness(1.f);

        thanhTienDo.setSize({0.f, 14.f});
        thanhTienDo.setPosition({24.f, 74.f});
        thanhTienDo.setFillColor(sf::Color(218, 174, 82));

        huongDan.setCharacterSize(13);
        huongDan.setFillColor(sf::Color(220, 230, 245));
        huongDan.setOutlineColor(sf::Color::Black);
        huongDan.setOutlineThickness(1.5f);
        huongDan.setPosition({24.f, 670.f});
        huongDan.setString("WASD: Di chuyen/Lai thuyen | Shift: Chay | Space: Chem | E: Thoai/Nhat coc | T: Len/Xuong thuyen | P: Boss Hoang Thao");
    }
    else
    {
        std::cerr << "Khong the tai font! Kiem tra Assets/Fonts/BeVietnamPro-Regular.ttf\n";
    }

    sf::Clock clock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                // Phím P: Triệu hồi Boss Lưu Hoằng Thao & Chuyển trạng thái tổng phản công
                if (key->code == sf::Keyboard::Key::P)
                {
                    heThongChienDau.TaoTranChienCuoi(5);
                    tranDau.ChuyenTrangThai(TrangThaiTranDau::PHAN_CONG);
                    qlNhiemVu.Tangtiendo(1);
                    noiDungThongBao = "Chu tuong Luu Hoang Thao da xuat hien!";
                    thoiGianThongBao = 3.f;
                }

                // Phím T: Chuyển đổi giữa Đi bộ và Lái thuyền nhẹ
                if (key->code == sf::Keyboard::Key::T)
                {
                    dangLaiThuyen = !dangLaiThuyen;
                    if (dangLaiThuyen)
                    {
                        thuyenPlayer.SetViTri(nguoiChoi.GetViTri());
                        noiDungThongBao = "Da len thuyen! Dieu khien thuyen 8 huong bang WASD.";
                    }
                    else
                    {
                        nguoiChoi.SetViTri(thuyenPlayer.GetViTri());
                        noiDungThongBao = "Da xuong thuyen, tiep tuc hanh trinh tren bo.";
                    }
                    thoiGianThongBao = 2.5f;
                }

                // Phím E: Tương tác nói chuyện với NPC nếu đang ở gần
                if (key->code == sf::Keyboard::Key::E)
                {
                    if (npcNgoQuyen.Isdangtuongtac())
                    {
                        npcNgoQuyen.NoiChuyen();
                        qlNhiemVu.Tangtiendo(1);
                    }
                    else if (npcDanLang.Isdangtuongtac())
                    {
                        npcDanLang.NoiChuyen();
                    }
                }
            }
        }

        if (!window.isOpen())
            break;

        const float deltaTime = clock.restart().asSeconds();

        // 1. Cập nhật nhân vật & điều khiển
        if (dangLaiThuyen)
        {
            // Lái thuyền 8 hướng
            thuyenPlayer.XuLyDieuKhien(deltaTime);
            thuyenPlayer.CapNhat(deltaTime);

            // Hải chiến: Đoàn thuyền địch bám đuổi thuyền ta và va chạm cọc ngầm
            heThongChienDau.CapNhatHaiChien(deltaTime, thuyenPlayer.GetViTri(), banDo);
        }
        else
        {
            // Đi bộ trên cạn
            nguoiChoi.XuLyPhim();
            nguoiChoi.CapNhat(deltaTime);

            // Kiểm tra tương tác với NPC
            npcNgoQuyen.TuongTac(nguoiChoi.GetViTri());
            npcDanLang.TuongTac(nguoiChoi.GetViTri());

            // Giao tranh bộ binh: Lính địch bám đuổi & nhận sát thương
            heThongChienDau.CapNhat(deltaTime, nguoiChoi);
            heThongChienDau.XuLyPlayerTanCong(
                nguoiChoi.GetVungTanCong(),
                nguoiChoi.GetSatThuong(),
                nguoiChoi.IsDangTanCong()
            );

            // Nhặt cọc bằng phím E
            if (nguoiChoi.IsDaBamTuongTac())
            {
                int maVatPham = 0;
                std::string tenVatPham;

                if (banDo.GanCocNhatDuoc(nguoiChoi.GetViTri()) &&
                    banDo.NhatCocGanNhat(nguoiChoi.GetViTri(), maVatPham, tenVatPham))
                {
                    if (nguoiChoi.NhatVatPham(maVatPham, tenVatPham))
                    {
                        banDo.DanhDauCocDaThuThap(maVatPham);
                        qlNhiemVu.Tangtiendo(1);

                        noiDungThongBao = nguoiChoi.DaThuThapDu3Coc()
                            ? "Da thu thap du 3 coc go!"
                            : "Da nhat duoc 1 coc go!";
                        thoiGianThongBao = 2.5f;
                    }
                    else
                    {
                        noiDungThongBao = "Tui do da day!";
                        thoiGianThongBao = 2.5f;
                    }
                }
            }
        }

        // 2. Cập nhật thủy triều, bản đồ và trạng thái trận đấu
        thuyTrieu.CapNhat(deltaTime);
        banDo.CapNhatThuyTrieu(thuyTrieu.GetTiLeRutNuoc());
        banDo.capNhat(deltaTime);
        tranDau.CapNhat(deltaTime);

        // Đếm ngược thời gian hiển thị thông báo
        if (thoiGianThongBao > 0.f)
        {
            thoiGianThongBao -= deltaTime;
            if (thoiGianThongBao <= 0.f)
            {
                thoiGianThongBao = 0.f;
                noiDungThongBao.clear();
            }
        }

        // 3. Cập nhật thanh tiến độ thu thập cọc
        const int soCocDaNhat = nguoiChoi.GetSoLuongCoc();
        const int sucChuaToiDa = 3;
        float tiLeTienDo = (sucChuaToiDa > 0)
            ? static_cast<float>(soCocDaNhat) / static_cast<float>(sucChuaToiDa)
            : 0.f;
        tiLeTienDo = std::clamp(tiLeTienDo, 0.f, 1.f);
        thanhTienDo.setSize({240.f * tiLeTienDo, 14.f});

        // 4. VẼ ĐỒ HỌA RA MÀN HÌNH
        window.clear(sf::Color(25, 30, 45));

        // Vẽ bản đồ Tiled map Bạch Đằng
        banDo.Ve(window);

        // Vẽ NPC
        npcNgoQuyen.Ve(window);
        npcDanLang.Ve(window);

        if (dangLaiThuyen)
        {
            // Vẽ hạm đội thuyền giặc Nam Hán
            heThongChienDau.VeHaiChien(window);
            // Vẽ thuyền người chơi
            thuyenPlayer.Ve(window);
        }
        else
        {
            // Vẽ lính bộ binh Nam Hán
            heThongChienDau.Ve(window);
            // Vẽ người chơi
            nguoiChoi.Ve(window);
        }

        window.setView(window.getDefaultView());

        // Hiển thị hộp thoại khi nói chuyện với NPC
        if (fontDaTai)
        {
            npcNgoQuyen.HienThiLoiThoai(window, font);
            npcDanLang.HienThiLoiThoai(window, font);

            // Vẽ thanh máu Boss Lưu Hoằng Thao (nếu Boss đã xuất hiện)
            heThongChienDau.VeBossBar(window, font);

            // Cập nhật thông tin HUD
            thongBao.setString("Coc Bach Dang: " + std::to_string(soCocDaNhat) + "/" + std::to_string(sucChuaToiDa) +
                               (dangLaiThuyen ? " | [DANG LAI THUYEN]" : " | [DI BO]"));
            if (soCocDaNhat >= sucChuaToiDa)
                thongBao.setFillColor(sf::Color(150, 255, 150));
            else
                thongBao.setFillColor(sf::Color::White);

            thongTinNhiemVu.setString("Hoi: " + qlNhiemVu.Gettieudehoihientai() + " | " + qlNhiemVu.GetthongtinUI());
            thongBaoNhat.setString(noiDungThongBao);

            window.draw(thongBao);
            window.draw(thongTinNhiemVu);
            window.draw(nenThanhTienDo);
            window.draw(thanhTienDo);

            if (!noiDungThongBao.empty())
                window.draw(thongBaoNhat);

            window.draw(huongDan);
        }

        window.display();
    }

    return 0;
}