#include "LuuGame.h"
#include <algorithm>
#include <fstream>
#include <sstream>

bool LuuGame::Tai(DuLieuLuu& duLieu, const std::string& tenFile) {
    std::ifstream file(tenFile);
    if (!file.is_open()) return false;

    std::string dong;
    while (std::getline(file, dong)) {
        size_t viTriBang = dong.find('=');
        if (viTriBang == std::string::npos) continue;

        std::string khoa = dong.substr(0, viTriBang);
        std::string giaTri = dong.substr(viTriBang + 1);

        int so = 0;
        try {
            so = std::stoi(giaTri);
        } catch (...) {
            continue; // Dòng hỏng thì bỏ qua
        }

        if (khoa == "chuongDaMo") duLieu.chuongDaMo = so;
        else if (khoa == "chuongHoanThanh") duLieu.chuongHoanThanh = so;
        else if (khoa == "amLuong") duLieu.amLuong = so;
        else if (khoa == "toanManHinh") duLieu.toanManHinh = (so != 0);
    }

    // Chặn giá trị bậy trong file bị sửa tay
    duLieu.chuongDaMo = std::clamp(duLieu.chuongDaMo, 1, 6);
    duLieu.chuongHoanThanh = std::clamp(duLieu.chuongHoanThanh, 0, 6);
    duLieu.amLuong = std::clamp(duLieu.amLuong, 0, 100);
    return true;
}

bool LuuGame::Luu(const DuLieuLuu& duLieu, const std::string& tenFile) {
    std::ofstream file(tenFile);
    if (!file.is_open()) return false;

    file << "chuongDaMo=" << duLieu.chuongDaMo << "\n";
    file << "chuongHoanThanh=" << duLieu.chuongHoanThanh << "\n";
    file << "amLuong=" << duLieu.amLuong << "\n";
    file << "toanManHinh=" << (duLieu.toanManHinh ? 1 : 0) << "\n";
    return true;
}

void LuuGame::GhiNhanHoanThanh(DuLieuLuu& duLieu, int maChuong, int soChuongToiDa) {
    if (maChuong < 1) return; // Màn thử nghiệm không tính tiến độ
    duLieu.chuongHoanThanh = std::max(duLieu.chuongHoanThanh, maChuong);
    duLieu.chuongDaMo = std::min(soChuongToiDa, std::max(duLieu.chuongDaMo, maChuong + 1));
}
