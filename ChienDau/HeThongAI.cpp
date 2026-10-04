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
    float kc = std::sqrt(dx * dx + dy * dy);
    // Tránh lỗi chia cho 0 nếu 2 nhân vật đứng trùng tọa độ nhau
    if (kc > 0.0001f) {
        return sf::Vector2f(dx / kc, dy / kc);
    }
    return sf::Vector2f(0.f, 0.f);
}

void HeThongAI::CapNhatAI(KeDich& keDich, sf::Vector2f viTriPlayer, float deltaTime) {
    // Chết rồi thì dừng nghỉ ngơi, không cần tính toán hành vi nữa
    if (keDich.IsDaHySinh()) {
        keDich.SetTrangThai(TrangThaiKeDich::DA_CHET);
        return;
    }

    float khoangCach = TinhKhoangCach(keDich.GetViTri(), viTriPlayer);

    // TH1: Áp sát trong tầm chém -> Chuyển sang trạng thái chém
    if (khoangCach <= keDich.GetTamDanh()) {
        keDich.SetTrangThai(TrangThaiKeDich::TAN_CONG);
    }
    // TH2: Ở trong tầm mắt nhìn thấy -> Lao tới đuổi theo
    else if (khoangCach <= keDich.GetTamPhatHien()) {
        keDich.DuoiTheo(viTriPlayer, deltaTime);
    }
    // TH3: Ở xa quá không thấy -> Đứng yên cảnh giác
    else {
        keDich.SetTrangThai(TrangThaiKeDich::DUNG_YEN);
    }
}
