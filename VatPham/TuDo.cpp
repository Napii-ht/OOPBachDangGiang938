
#include "TuDo.h"
#include "../CotLoi/TienIch.h"
#include <iostream>

TuDo::TuDo(int kichThuoc) : sucChua(kichThuoc), oDangChon(0) {}

bool TuDo::ThemVatPham(const VatPham& vp) {
    // Tìm xem đã có vật phẩm này chưa để gộp số lượng
    for (auto& item : danhSachVatPham) {
        if (item.GetMaVatPham() == vp.GetMaVatPham()) {
            item.TangSoLuong(vp.GetSoLuong());
            return true;
        }
    }

    if (static_cast<int>(danhSachVatPham.size()) < sucChua) {
        danhSachVatPham.push_back(vp);
        return true;
    }

    return false; // Túi đồ đã đầy
}

bool TuDo::XoaVatPham(int maVatPham, int soLuong) {
    for (auto it = danhSachVatPham.begin(); it != danhSachVatPham.end(); ++it) {
        if (it->GetMaVatPham() == maVatPham) {
            if (it->GetSoLuong() >= soLuong) {
                it->GiamSoLuong(soLuong);
                if (it->GetSoLuong() <= 0) {
                    danhSachVatPham.erase(it);
                    if (oDangChon >= static_cast<int>(danhSachVatPham.size()) && oDangChon > 0) {
                        oDangChon = static_cast<int>(danhSachVatPham.size()) - 1;
                    }
                }
                return true;
            }
            return false;
        }
    }
    return false;
}

bool TuDo::KiemTraVatPham(int maVatPham, int soLuong) const {
    return GetSoLuongVatPham(maVatPham) >= soLuong;
}

int TuDo::GetSoLuongVatPham(int maVatPham) const {
    for (const auto& item : danhSachVatPham) {
        if (item.GetMaVatPham() == maVatPham) {
            return item.GetSoLuong();
        }
    }
    return 0;
}

void TuDo::SetODangChon(int index) {
    if (index >= 0 && index < sucChua) {
        oDangChon = index;
    }
}

const VatPham* TuDo::GetVatPhamDangChon() const {
    if (oDangChon >= 0 && oDangChon < static_cast<int>(danhSachVatPham.size())) {
        return &danhSachVatPham[oDangChon];
    }
    return nullptr;
}

void TuDo::VeHotbar(sf::RenderWindow& window, const sf::Font& font, sf::Vector2f viTriGoc) {
    const float oKichThuoc = 46.f;
    const float khoangCach = 6.f;

    for (int i = 0; i < sucChua; ++i) {
        sf::Vector2f pos(viTriGoc.x + static_cast<float>(i) * (oKichThuoc + khoangCach), viTriGoc.y);

        // Khung viền ô đồ kiểu Terraria (Gỗ sẫm và viền vàng kim)
        sf::RectangleShape khung({oKichThuoc, oKichThuoc});
        khung.setPosition(pos);
        
        if (i == oDangChon) {
            khung.setFillColor(sf::Color(65, 80, 115, 220));
            khung.setOutlineThickness(2.5f);
            khung.setOutlineColor(sf::Color(255, 215, 0)); // Viền vàng chọn
        } else {
            khung.setFillColor(sf::Color(35, 40, 55, 190));
            khung.setOutlineThickness(1.5f);
            khung.setOutlineColor(sf::Color(90, 105, 130, 200));
        }
        window.draw(khung);

        // Hiển thị phím số tắt (1, 2, 3...)
        sf::Text soO(font, VanBan(std::to_string(i + 1)), 12);
        soO.setFillColor(sf::Color(180, 190, 210));
        soO.setPosition({pos.x + 4.f, pos.y + 2.f});
        window.draw(soO);

        // Nếu ô có vật phẩm, vẽ biểu tượng và số lượng
        if (i < static_cast<int>(danhSachVatPham.size())) {
            const auto& item = danhSachVatPham[i];
            item.VeBieuTuong(window, {pos.x + oKichThuoc * 0.5f, pos.y + oKichThuoc * 0.5f}, oKichThuoc * 0.8f);

            // Số lượng góc dưới phải
            if (item.GetSoLuong() > 1) {
                sf::Text soLuongText(font, VanBan(std::to_string(item.GetSoLuong())), 13);
                soLuongText.setFillColor(sf::Color::White);
                soLuongText.setOutlineThickness(1.f);
                soLuongText.setOutlineColor(sf::Color::Black);
                soLuongText.setPosition({pos.x + oKichThuoc - 18.f, pos.y + oKichThuoc - 17.f});
                window.draw(soLuongText);
            }
        }
    }

    // Hiển thị tên vật phẩm đang chọn phong cách Terraria
    const VatPham* itemChon = GetVatPhamDangChon();
    if (itemChon) {
        sf::Text tenText(font, VanBan(itemChon->GetTen()), 11);
        tenText.setFillColor(sf::Color(255, 225, 120));
        tenText.setOutlineThickness(1.2f);
        tenText.setOutlineColor(sf::Color(20, 20, 20, 220));
        tenText.setPosition({viTriGoc.x + 2.f, viTriGoc.y + oKichThuoc + 4.f});
        window.draw(tenText);
    }
}


