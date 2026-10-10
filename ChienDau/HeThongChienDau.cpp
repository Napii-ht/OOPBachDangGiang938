#include "HeThongChienDau.h"
#include "../BanDo/BanDo.h"
#include <algorithm>

static sf::String VanBan(const std::string& utf8) {
    return sf::String::fromUtf8(utf8.begin(), utf8.end());
}

HeThongChienDau::HeThongChienDau()
    : soKeDichConLai(0),
      daKhoiTaoTranChien(false) {}

void HeThongChienDau::ThemKeDich(sf::Vector2f viTri, float tocDo, int mau, int dame, LoaiKeDich loai) {
    danhSachKeDich.push_back(std::make_unique<KeDich>(viTri, tocDo, mau, dame, 42.f, 260.f, loai));
    soKeDichConLai = static_cast<int>(danhSachKeDich.size());
}

void HeThongChienDau::TaoTranChienCuoi(int soLuongDich) {
    danhSachKeDich.clear();
    danhSachDaBiChemTrongNhatNay.clear();

    // 1. Tướng giặc Lưu Hoằng Thao xuất hiện làm Boss chính
    ThemKeDich({750.f, 320.f}, 95.f, 280, 25, LoaiKeDich::TUONG_HOANG_THAO);

    // 2. Quân tinh nhuệ hộ vệ xung quanh
    for (int i = 0; i < soLuongDich - 1; ++i) {
        float x = 600.f + static_cast<float>((i % 3) * 70);
        float y = 200.f + static_cast<float>((i / 3) * 110);
        ThemKeDich({x, y}, 105.f + static_cast<float>(i * 8), 75, 12, LoaiKeDich::LINH_THUONG);
    }

    soKeDichConLai = static_cast<int>(danhSachKeDich.size());
    daKhoiTaoTranChien = true;
}

void HeThongChienDau::TaoDoanThuyenNamHan(int soLuong) {
    danhSachThuyenDich.clear();

    // Soái hạm của Lưu Hoằng Thao
    danhSachThuyenDich.push_back(std::make_unique<ThuyenKeDich>(sf::Vector2f(1050.f, 340.f), true));

    // Các chiến thuyền hộ tống dàn hàng ngang tiến vào khúc sông
    for (int i = 1; i < soLuong; ++i) {
        float yPos = 210.f + static_cast<float>(i * 80);
        float xPos = 1080.f + static_cast<float>(i * 45);
        danhSachThuyenDich.push_back(std::make_unique<ThuyenKeDich>(sf::Vector2f(xPos, yPos), false));
    }
}

void HeThongChienDau::CapNhatHaiChien(float deltaTime, sf::Vector2f viTriThuyenPlayer, BanDo& banDo) {
    for (auto& thuyen : danhSachThuyenDich) {
        if (!thuyen->IsDaBiPhaHuy()) {
            if (!thuyen->IsDaMacCoc()) {
                heThongAI.CapNhatAIThuyen(*thuyen, viTriThuyenPlayer, deltaTime);
            }

            // Kiểm tra đâm va vào cọc ngầm Bạch Đằng
            if (banDo.VaChamCoc(thuyen->GetHitBox())) {
                thuyen->SetMacCoc(true);
            }

            thuyen->CapNhat(deltaTime);
        }
    }
}

void HeThongChienDau::VeHaiChien(sf::RenderWindow& window) {
    for (auto& thuyen : danhSachThuyenDich) {
        thuyen->Ve(window);
    }
}

int HeThongChienDau::GetSoThuyenConSong() const {
    int dem = 0;
    for (const auto& thuyen : danhSachThuyenDich) {
        if (!thuyen->IsDaBiPhaHuy()) dem++;
    }
    return dem;
}

bool HeThongChienDau::KiemTraTatCaThuyenDaBiPhaHuy() const {
    if (danhSachThuyenDich.empty()) return false;
    return GetSoThuyenConSong() == 0;
}

void HeThongChienDau::CapNhat(float deltaTime, NhanVat& player) {
    int demLinhConSong = 0;

    // Giữ khoảng cách giữa các lính giặc
    heThongAI.XuLyGianCachLinh(danhSachKeDich);

    for (auto& dich : danhSachKeDich) {
        if (!dich->IsDaHySinh()) {
            heThongAI.CapNhatAI(*dich, player.GetViTri(), deltaTime);
            dich->CapNhat(deltaTime);

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
    if (!playerDangChem) {
        danhSachDaBiChemTrongNhatNay.clear();
        return 0;
    }

    int soDichBiChem = 0;
    for (auto& dich : danhSachKeDich) {
        if (!dich->IsDaHySinh()) {
            if (vungTanCong.findIntersection(dich->GetHitBox()).has_value()) {
                if (danhSachDaBiChemTrongNhatNay.find(dich.get()) == danhSachDaBiChemTrongNhatNay.end()) {
                    dich->NhanSatThuong(satThuong);
                    danhSachDaBiChemTrongNhatNay.insert(dich.get());
                    soDichBiChem++;
                }
            }
        }
    }
    return soDichBiChem;
}

bool HeThongChienDau::KiemTraChienThang() const {
    return daKhoiTaoTranChien && (soKeDichConLai == 0);
}

void HeThongChienDau::XoaToanBo() {
    danhSachKeDich.clear();
    danhSachThuyenDich.clear();
    danhSachDaBiChemTrongNhatNay.clear();
    soKeDichConLai = 0;
    daKhoiTaoTranChien = false;
}

void HeThongChienDau::VeBossBar(sf::RenderWindow& window, const sf::Font& font) {
    // Tìm Boss Hoằng Thao trong danh sách lính
    const KeDich* boss = nullptr;
    for (const auto& dich : danhSachKeDich) {
        if (dich->IsBoss() && !dich->IsDaHySinh()) {
            boss = dich.get();
            break;
        }
    }

    if (!boss) return;

    // Thanh máu Boss Terraria uy nghi ở giữa đỉnh màn hình
    const float chieuRongThanh = 360.f;
    const float chieuCaoThanh = 16.f;
    const float x = 400.f;
    const float y = 28.f;

    sf::RectangleShape khungNen({chieuRongThanh + 8.f, chieuCaoThanh + 8.f});
    khungNen.setOrigin({(chieuRongThanh + 8.f) * 0.5f, (chieuCaoThanh + 8.f) * 0.5f});
    khungNen.setPosition({x, y});
    khungNen.setFillColor(sf::Color(20, 20, 25, 230));
    khungNen.setOutlineThickness(2.5f);
    khungNen.setOutlineColor(sf::Color(218, 165, 32));
    window.draw(khungNen);

    float tiLe = static_cast<float>(boss->GetSucKhoe()) / static_cast<float>(boss->GetSucKhoeToiDa());
    tiLe = std::clamp(tiLe, 0.0f, 1.0f);

    sf::RectangleShape vachMau({chieuRongThanh * tiLe, chieuCaoThanh});
    vachMau.setOrigin({chieuRongThanh * 0.5f, chieuCaoThanh * 0.5f});
    vachMau.setPosition({x - (chieuRongThanh * (1.f - tiLe) * 0.5f), y});
    vachMau.setFillColor(sf::Color(220, 30, 40));
    window.draw(vachMau);

    // Tên Boss
    sf::Text tenBoss(font, VanBan("LƯU HOẰNG THÁO - CHỦ TƯỚNG NAM HÁN"), 12);
    tenBoss.setFillColor(sf::Color(255, 230, 150));
    tenBoss.setOrigin({tenBoss.getLocalBounds().size.x * 0.5f, tenBoss.getLocalBounds().size.y * 0.5f});
    tenBoss.setPosition({x, y - 18.f});
    window.draw(tenBoss);
}
