#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../NhanVat/NguoiChoi.h"
#include "../ChienDau/HeThongChienDau.h"
#include "QuanLyTranDau.h"

// "Tong quan ly" cua game: giu cua so, vong lap chinh, nguoi choi, trang thai tran dau.
// Cac module cua ban khac (BanDo, KeDich, NhiemVu, GiaoDien...) se duoc gan vao day.
class QuanLyGame {
private:
    sf::RenderWindow cuaSo;
    sf::Clock dongHo;

    NguoiChoi nguoiChoi;
    QuanLyTranDau tranDau;
    HeThongChienDau heThongChienDau;     // cua Nhan: quan dich + AI + tran danh cuoi

    // ---- Phan THU NGHIEM (sau nay thay bang BanDo cua Yen / NhiemVu cua Trang) ----
    std::vector<sf::FloatRect> vatCan;       // vai vat can de thu va cham
    std::vector<sf::Vector2f>  viTriCoc;     // 3 coc go de thu nhat do
    std::vector<bool>          daNhatCoc;
    int soCocDaNhat;

    void XuLySuKien();
    void CapNhat(float dt);
    void Ve();

public:
    QuanLyGame();
    void Chay();      // vong lap game: XuLySuKien -> CapNhat -> Ve
};
