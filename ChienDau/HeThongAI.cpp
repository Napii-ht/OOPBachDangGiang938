#include "HeThongAI.h"
#include <cmath>

float HeThongAI::TinhKhoangCach(sf::Vector2f p1, sf::Vector2f p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

sf::Vector2f HeThongAI::TinhHuong(sf::Vector2f nguon, sf::Vector2f dich) {
    float dx = dich.x - nguon.x;
    float dy = dich.y - nguon.y;
    float dist = std::sqrt(dx * dx + dy * dy);
    if (dist > 0.0001f) {
        return sf::Vector2f(dx / dist, dy / dist);
    }
    return sf::Vector2f(0.f, 0.f);
}

void HeThongAI::CapNhatAI(KeDich& keDich, sf::Vector2f viTriPlayer, float deltaTime) {
    if (keDich.IsDaHySinh()) return;

    if (keDich.PhatHienNguoiChoi(viTriPlayer)) {
        keDich.DuoiTheo(viTriPlayer, deltaTime);
    } else {
        keDich.SetTrangThai(TrangThaiKeDich::DUNG_YEN);
    }
}

void HeThongAI::XuLyGianCachLinh(std::vector<std::unique_ptr<KeDich>>& danhSachDich, float khoangCachToiThieu) {
    for (size_t i = 0; i < danhSachDich.size(); ++i) {
        if (danhSachDich[i]->IsDaHySinh()) continue;

        for (size_t j = i + 1; j < danhSachDich.size(); ++j) {
            if (danhSachDich[j]->IsDaHySinh()) continue;

            sf::Vector2f p1 = danhSachDich[i]->GetViTri();
            sf::Vector2f p2 = danhSachDich[j]->GetViTri();
            float dist = TinhKhoangCach(p1, p2);

            if (dist < khoangCachToiThieu && dist > 0.001f) {
                sf::Vector2f dayHuong = TinhHuong(p1, p2);
                float dayXa = (khoangCachToiThieu - dist) * 0.5f;

                danhSachDich[i]->SetViTri(p1 - dayHuong * dayXa);
                danhSachDich[j]->SetViTri(p2 + dayHuong * dayXa);
            }
        }
    }
}

void HeThongAI::CapNhatAIThuyen(ThuyenKeDich& thuyen, sf::Vector2f viTriThuyenPlayer, float deltaTime) {
    thuyen.DuoiTheo(viTriThuyenPlayer, deltaTime);
}
