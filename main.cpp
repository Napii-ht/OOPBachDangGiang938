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

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({1200, 700}),
        "Bach Dang Giang 938"
    );

    BanDo banDo;
    NguoiChoi nguoiChoi(400.f, 300.f);
    ThuyTrieu thuyTrieu;
    thuyTrieu.BatDauRutNuoc();

    sf::Font font;
    const bool fontDaTai =
        font.openFromFile("Assets/Fonts/BeVietnamPro-Regular.ttf");

    sf::Text thongBao(font);
    sf::Text thongBaoNhat(font);

    sf::RectangleShape nenThanhTienDo;
    sf::RectangleShape thanhTienDo;

    float thoiGianThongBao = 0.f;
    std::string noiDungThongBao;

    if (fontDaTai)
    {
        thongBao.setCharacterSize(22);
        thongBao.setFillColor(sf::Color::White);
        thongBao.setOutlineColor(sf::Color::Black);
        thongBao.setOutlineThickness(2.f);
        thongBao.setPosition({24.f, 20.f});

        thongBaoNhat.setCharacterSize(18);
        thongBaoNhat.setFillColor(sf::Color(255, 230, 160));
        thongBaoNhat.setOutlineColor(sf::Color::Black);
        thongBaoNhat.setOutlineThickness(2.f);
        thongBaoNhat.setPosition({24.f, 94.f});

        nenThanhTienDo.setSize({240.f, 16.f});
        nenThanhTienDo.setPosition({24.f, 58.f});
        nenThanhTienDo.setFillColor(sf::Color(55, 55, 55));
        nenThanhTienDo.setOutlineColor(sf::Color::White);
        nenThanhTienDo.setOutlineThickness(1.f);

        thanhTienDo.setSize({0.f, 16.f});
        thanhTienDo.setPosition({24.f, 58.f});
        thanhTienDo.setFillColor(sf::Color(218, 174, 82));
    }
    else
    {
        std::cerr
            << "Khong the tai font! Kiem tra duong dan font.\n";
    }

    sf::Clock clock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (!window.isOpen())
            break;

        const float deltaTime = clock.restart().asSeconds();

        thuyTrieu.CapNhat(deltaTime);
        banDo.CapNhatThuyTrieu(
            thuyTrieu.GetTiLeRutNuoc()
        );

        // Cap nhat ban do
        banDo.capNhat(deltaTime);

        // Nhat coc bang ham tuong tac cua NguoiChoi
        if (nguoiChoi.IsDaBamTuongTac())
        {
            int maVatPham = 0;
            std::string tenVatPham;

            if (banDo.GanCocNhatDuoc(nguoiChoi.GetViTri()) &&
                banDo.NhatCocGanNhat(
                    nguoiChoi.GetViTri(),
                    maVatPham,
                    tenVatPham))
            {
                if (nguoiChoi.NhatVatPham(maVatPham, tenVatPham))
                {
                    banDo.DanhDauCocDaThuThap(maVatPham);

                    noiDungThongBao =
                        nguoiChoi.DaThuThapDu3Coc()
                            ? "Da thu thap du 3 coc!"
                            : "Da nhat coc!";

                    thoiGianThongBao = 2.5f;
                }
                else
                {
                    noiDungThongBao = "Tui do da day!";
                    thoiGianThongBao = 2.5f;
                }
            }
            else
            {
                noiDungThongBao = "Khong co coc nao o gan!";
                thoiGianThongBao = 2.f;
            }
        }

        // Cập nhật thời gian hiển thị thông báo.
        if (thoiGianThongBao > 0.f)
        {
            thoiGianThongBao -= deltaTime;

            if (thoiGianThongBao <= 0.f)
            {
                thoiGianThongBao = 0.f;
                noiDungThongBao.clear();
            }
        }

        // Cập nhật thanh tiến độ thu thập.
        const int soCocDaNhat = nguoiChoi.GetSoLuongCoc();
        const int sucChuaToiDa = 3;

        float tiLeTienDo = 0.f;

        if (sucChuaToiDa > 0)
        {
            tiLeTienDo =
                static_cast<float>(soCocDaNhat) /
                static_cast<float>(sucChuaToiDa);
        }

        tiLeTienDo = std::clamp(tiLeTienDo, 0.f, 1.f);

        thanhTienDo.setSize({
            240.f * tiLeTienDo,
            16.f
        });

        // Ve ban do
        banDo.Ve(window);
        nguoiChoi.Ve(window);

        window.setView(window.getDefaultView());

        // Vẽ HUD.
        if (fontDaTai)
        {
            thongBao.setString(
                "Coc Bach Dang: " +
                std::to_string(soCocDaNhat) + "/" +
                std::to_string(sucChuaToiDa)
            );

            if (soCocDaNhat >= sucChuaToiDa)
            {
                thongBao.setFillColor(
                    sf::Color(150, 255, 150)
                );
            }
            else
            {
                thongBao.setFillColor(sf::Color::White);
            }

            thongBaoNhat.setString(noiDungThongBao);

            window.draw(thongBao);
            window.draw(nenThanhTienDo);
            window.draw(thanhTienDo);

            if (!noiDungThongBao.empty())
            {
                window.draw(thongBaoNhat);
            }
        }

        window.display();
    }

    return 0;
}