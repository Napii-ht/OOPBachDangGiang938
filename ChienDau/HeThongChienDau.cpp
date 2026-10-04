#include "HeThongChienDau.h"

HeThongChienDau::HeThongChienDau() : soKeDichConLai(0), daKhoiTaoTranChien(false) {}

void HeThongChienDau::ThemKeDich(sf::Vector2f viTri, float tocDo, int mau, int dame) {
    danhSachKeDich.push_back(std::make_unique<KeDich>(viTri, tocDo, mau, dame));
    soKeDichConLai = static_cast<int>(danhSachKeDich.size());
}

void HeThongChienDau::TaoTranChienCuoi(int soLuongDich) {
    XoaToanBo();

    // Rải quân lính Nam Hán dàn trận trên bờ sông Bạch Đằng
    for (int i = 0; i < soLuongDich; ++i) {
        float x = 200.f + static_cast<float>(i % 3) * 180.f;
        float y = 100.f + static_cast<float>(i / 3) * 120.f;
        ThemKeDich({x, y}, 80.f + static_cast<float>(i * 10), 80, 12);
    }

    soKeDichConLai = soLuongDich;
    daKhoiTaoTranChien = true;
}

void HeThongChienDau::CapNhat(float deltaTime, NhanVat& player) {
    int demLinhConSong = 0;

    for (auto& dich : danhSachKeDich) {
        if (!dich->IsDaHySinh()) {
            // Cho AI suy nghĩ và ra lệnh cho con lính
            heThongAI.CapNhatAI(*dich, player.GetViTri(), deltaTime);

            // Cập nhật vị trí và hoạt ảnh
            dich->CapNhat(deltaTime);

            // Đủ điều kiện thì chém player
            if (dich->CoTheTanCong(player.GetViTri())) {
                dich->TanCong(player);
            }

            demLinhConSong++;
        }
    }

    soKeDichConLai = demLinhConSong;
}

void HeThongChienDau::Ve(sf::RenderWindow& window) {
    for (auto& dich : danhSachKeDich) {
        if (!dich->IsDaHySinh()) {
            dich->Ve(window);
        }
    }
}

int HeThongChienDau::XuLyPlayerTanCong(const sf::FloatRect& vungTanCong, int satThuong, bool playerDangChem) {
    // Nếu player không còn vung kiếm nữa (hạ kiếm xuống) thì reset danh sách để đòn sau chém lại được
    if (!playerDangChem) {
        danhSachDaBiChemTrongNhatNay.clear();
        return 0;
    }

    int soDichTrúngDon = 0;

    for (auto& dich : danhSachKeDich) {
        if (!dich->IsDaHySinh()) {
            // SFML 3: dùng findIntersection kiểm tra va chạm vùng kiếm với hitbox lính
            if (vungTanCong.findIntersection(dich->GetHitBox()).has_value()) {
                // Kiểm tra xem con lính này đã bị dính đòn trong nhát chém này chưa
                // Nếu CHƯA dính thì mới trừ máu (chỉ trừ đúng 1 lần/nhát chém)
                if (danhSachDaBiChemTrongNhatNay.find(dich.get()) == danhSachDaBiChemTrongNhatNay.end()) {
                    dich->NhanSatThuong(satThuong);
                    danhSachDaBiChemTrongNhatNay.insert(dich.get()); // Đánh dấu là đã dính đòn rồi
                    soDichTrúngDon++;
                }
            }
        }
    }

    return soDichTrúngDon;
}

bool HeThongChienDau::KiemTraChienThang() const {
    // Thắng trận khi đã mở trận và diệt sạch lính
    return daKhoiTaoTranChien && (soKeDichConLai == 0);
}

void HeThongChienDau::XoaToanBo() {
    danhSachKeDich.clear();
    danhSachDaBiChemTrongNhatNay.clear();
    soKeDichConLai = 0;
    daKhoiTaoTranChien = false;
}
