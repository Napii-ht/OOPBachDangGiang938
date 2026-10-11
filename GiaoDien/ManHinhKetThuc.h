#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "CanhNenMenu.h"

// Màn kết thúc game sau chương 6: lời bạt lịch sử rồi danh sách nhóm thực hiện cuộn lên.
// Bấm Enter, Space hoặc chuột để chuyển nhanh, Esc để về menu ngay.
class ManHinhKetThuc {
private:
    enum class Buoc {
        LOI_BAT,
        CREDITS,
        XONG
    };

    CanhNenMenu canhNen;
    Buoc buoc;
    float thoiGian;
    float viTriCuon;
    size_t chiSoTrang;
    bool daXong;

    std::vector<std::string> cacTrangLoiBat;
    std::vector<std::pair<std::string, std::string>> dongCredits;   // Cặp (tiêu đề, nội dung) cho mỗi dòng

    void ChuyenBuoc();

public:
    ManHinhKetThuc();

    void Mo();
    void XuLySuKien(const sf::Event& suKien);
    void CapNhat(float deltaTime);
    void Ve(sf::RenderWindow& window, const sf::Font& font) const;

    bool DaXong() const { return daXong; }
};
