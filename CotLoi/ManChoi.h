#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "LuuGame.h"

class QuanLyAmThanh;

// Những thứ dùng chung cho mọi màn chơi, QuanLyGame tạo ra một lần rồi đưa cho từng màn
struct NguCanhGame {
    sf::RenderWindow& window;
    const sf::Font& phongChu;
    QuanLyAmThanh& amThanh;
    DuLieuLuu& duLieuLuu;
};

enum class KetQuaManChoi {
    DANG_CHOI,   // Màn vẫn đang chạy
    THANG,       // Hoàn thành màn
    THUA,        // Thua, quay về chọn chương
    THOAT        // Người chơi chủ động thoát màn
};

// Lớp cơ sở trừu tượng cho mỗi chương của game.
// Mỗi chương (bản đồ, cách chơi, nhiệm vụ riêng) kế thừa lớp này.
class ManChoi {
protected:
    NguCanhGame& nguCanh;
    KetQuaManChoi ketQua;

public:
    explicit ManChoi(NguCanhGame& ngu) : nguCanh(ngu), ketQua(KetQuaManChoi::DANG_CHOI) {}
    virtual ~ManChoi() = default;

    virtual void XuLySuKien(const sf::Event& suKien) = 0;
    virtual void CapNhat(float deltaTime) = 0;
    virtual void Ve() = 0;

    KetQuaManChoi GetKetQua() const { return ketQua; }
};
