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

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <shellapi.h>
#endif
#include <cstdlib>

bool PhatVideoNgoai(const std::string& tenVideo) {
    const char* cacTienTo[] = { "", "../", "../../", "../../../" };
    std::string duongDanTimThay = "";
    std::error_code ec;

    // 1. Tìm theo tên chỉ định
    for (const char* t : cacTienTo) {
        std::string thu1 = std::string(t) + "TaiNguyen/Video/" + tenVideo;
        if (std::filesystem::exists(thu1, ec)) { duongDanTimThay = thu1; break; }
        std::string thu2 = std::string(t) + "TaiNguyen/" + tenVideo;
        if (std::filesystem::exists(thu2, ec)) { duongDanTimThay = thu2; break; }
    }

    // 2. Nếu chưa thấy, tìm bất kỳ file .mp4 nào trong thư mục Video
    if (duongDanTimThay.empty()) {
        for (const char* t : cacTienTo) {
            std::string thuMucVideo = std::string(t) + "TaiNguyen/Video";
            if (std::filesystem::exists(thuMucVideo, ec)) {
                for (const auto& entry : std::filesystem::directory_iterator(thuMucVideo, ec)) {
                    if (entry.is_regular_file() && entry.path().extension() == ".mp4") {
                        duongDanTimThay = entry.path().string();
                        break;
                    }
                }
            }
            if (!duongDanTimThay.empty()) break;
        }
    }

    if (duongDanTimThay.empty()) return false;

    // Chuẩn hóa sang đường dẫn tuyệt đối với dấu gạch chéo ngược Windows (\)
    std::filesystem::path pAbs = std::filesystem::absolute(duongDanTimThay);
    std::string duongDanChuan = pAbs.lexically_normal().string();
    for (char& c : duongDanChuan) {
        if (c == '/') c = '\\';
    }

#ifdef _WIN32
    HINSTANCE res = ShellExecuteA(NULL, "open", duongDanChuan.c_str(), NULL, NULL, SW_SHOWNORMAL);
    if (reinterpret_cast<INT_PTR>(res) > 32) return true;

    // Dự phòng gọi lệnh start của Windows shell
    std::string lenhStart = "start \"\" \"" + duongDanChuan + "\"";
    return std::system(lenhStart.c_str()) == 0;
#else
    std::string lenhLinux = "xdg-open \"" + duongDanChuan + "\" &";
    return std::system(lenhLinux.c_str()) == 0;
#endif
}
