#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// Danh sách nút dọc điều khiển được bằng bàn phím (W/S, mũi tên, Enter, Space) và chuột.
// Dùng chung cho menu chính, menu tạm dừng, chọn chương, cài đặt.
class DanhSachNut {
private:
    std::vector<std::string> danhSachNhan;
    std::vector<bool> danhSachKhoa;
    int chon;
    sf::Vector2f tamTren;   // Tâm của nút đầu tiên
    float rongNut;
    float caoNut;
    float khoangCach;       // Khoảng cách giữa tâm hai nút liền nhau
    unsigned int coChu;
    float thoiGian;

    int TimNutDuoiChuot(sf::Vector2f toaDo) const;

public:
    DanhSachNut();

    void DatDanhSach(const std::vector<std::string>& nhan);
    void DatNhan(int chiSo, const std::string& nhan);
    void DatKhoa(int chiSo, bool khoa);
    void DatBoCuc(sf::Vector2f tamNutDau, float rong, float cao, float khoang, unsigned int co = 14);
    void DatChon(int chiSo);

    // Trả về chỉ số nút được kích hoạt (Enter, Space hoặc click), -1 nếu chưa có
    int XuLySuKien(const sf::Event& suKien, const sf::RenderWindow& window);
    void CapNhat(float deltaTime) { thoiGian += deltaTime; }
    void Ve(sf::RenderWindow& window, const sf::Font& font) const;

    int GetChon() const { return chon; }
    int GetSoNut() const { return static_cast<int>(danhSachNhan.size()); }
    bool IsKhoa(int chiSo) const;
};
