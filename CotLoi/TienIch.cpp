#include "TienIch.h"
#include <filesystem>

sf::String VanBan(const std::string& utf8) {
    return sf::String::fromUtf8(utf8.begin(), utf8.end());
}

std::string DuongDanTaiNguyen(const std::string& tuongDoi) {
    static std::string tienTo;
    static bool daTim = false;

    if (!daTim) {
        daTim = true;
        const char* cacTienTo[] = { "", "../", "../../", "../../../" };
        for (const char* t : cacTienTo) {
            std::error_code loi;
            if (std::filesystem::exists(std::string(t) + "TaiNguyen", loi)) {
                tienTo = t;
                break;
            }
        }
    }
    return tienTo + "TaiNguyen/" + tuongDoi;
}

bool TaiPhongChuMacDinh(sf::Font& font) {
    const std::string cacFont[] = {
        DuongDanTaiNguyen("PhongChu/arialbd.ttf"),
        DuongDanTaiNguyen("PhongChu/arial.ttf"),
        "C:/Windows/Fonts/arialbd.ttf",
        "C:/Windows/Fonts/arial.ttf"
    };
    for (const auto& duongDan : cacFont) {
        if (font.openFromFile(duongDan)) return true;
    }
    return false;
}

sf::View TaoViewLogic(const sf::RenderWindow& window) {
    sf::View view(sf::FloatRect({0.f, 0.f}, {RONG_MAN_HINH, CAO_MAN_HINH}));

    sf::Vector2u kichThuoc = window.getSize();
    if (kichThuoc.x == 0 || kichThuoc.y == 0) return view;

    float tiLeCuaSo = static_cast<float>(kichThuoc.x) / static_cast<float>(kichThuoc.y);
    float tiLeLogic = RONG_MAN_HINH / CAO_MAN_HINH;

    if (tiLeCuaSo > tiLeLogic) {
        // Cửa sổ rộng hơn 4:3, chừa dải đen hai bên trái phải
        float rong = tiLeLogic / tiLeCuaSo;
        view.setViewport(sf::FloatRect({(1.f - rong) * 0.5f, 0.f}, {rong, 1.f}));
    } else {
        // Cửa sổ cao hơn 4:3, chừa dải đen trên dưới
        float cao = tiLeCuaSo / tiLeLogic;
        view.setViewport(sf::FloatRect({0.f, (1.f - cao) * 0.5f}, {1.f, cao}));
    }
    return view;
}

void ApDungViewLogic(sf::RenderWindow& window) {
    window.setView(TaoViewLogic(window));
}

sf::Vector2f ToaDoChuot(const sf::RenderWindow& window, sf::Vector2i pixel) {
    return window.mapPixelToCoords(pixel, TaoViewLogic(window));
}

void VeChuGiua(sf::RenderWindow& window, const sf::Font& font, const std::string& noiDung,
               unsigned int coChu, sf::Vector2f viTri, sf::Color mau,
               float doDayVien, sf::Color mauVien) {
    sf::Text chu(font, VanBan(noiDung), coChu);
    chu.setFillColor(mau);
    if (doDayVien > 0.f) {
        chu.setOutlineThickness(doDayVien);
        chu.setOutlineColor(mauVien);
    }
    sf::FloatRect khung = chu.getLocalBounds();
    chu.setOrigin({khung.position.x + khung.size.x * 0.5f, khung.position.y + khung.size.y * 0.5f});
    chu.setPosition(viTri);
    window.draw(chu);
}

void VeChuTrai(sf::RenderWindow& window, const sf::Font& font, const std::string& noiDung,
               unsigned int coChu, sf::Vector2f viTri, sf::Color mau) {
    sf::Text chu(font, VanBan(noiDung), coChu);
    chu.setFillColor(mau);
    chu.setPosition(viTri);
    window.draw(chu);
}

std::string NgatDong(const sf::Font& font, const std::string& utf8, unsigned int coChu, float rongToiDa) {
    std::string ketQua;
    std::string dongHienTai;
    std::string tu;

    // Hàm phụ: thêm một từ vào dòng hiện tại, nếu quá rộng thì xuống dòng
    auto themTu = [&](const std::string& tuMoi) {
        std::string thu = dongHienTai.empty() ? tuMoi : dongHienTai + " " + tuMoi;
        sf::Text phep(font, VanBan(thu), coChu);
        if (!dongHienTai.empty() && phep.getLocalBounds().size.x > rongToiDa) {
            ketQua += dongHienTai + "\n";
            dongHienTai = tuMoi;
        } else {
            dongHienTai = thu;
        }
    };

    for (size_t i = 0; i <= utf8.size(); ++i) {
        char c = (i < utf8.size()) ? utf8[i] : '\0';
        if (c == ' ' || c == '\n' || c == '\0') {
            if (!tu.empty()) {
                themTu(tu);
                tu.clear();
            }
            if (c == '\n') {
                ketQua += dongHienTai + "\n";
                dongHienTai.clear();
            }
        } else {
            tu += c;
        }
    }
    ketQua += dongHienTai;
    return ketQua;
}
