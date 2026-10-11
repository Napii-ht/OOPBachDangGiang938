
#pragma once

#include <SFML/Graphics.hpp>

class Coc {
private:
    sf::Vector2f viTri;
    float chieuRong;
    float chieuCao;
    bool daCam;            // true nếu đã được cắm xuống lòng sông
    int satThuongVaCham;   // Sát thương gây ra khi thuyền địch đâm phải
    bool daPhatNoi;        // Đã kích hoạt va chạm gây sát thương chưa

public:
    Coc(sf::Vector2f pos, float rong = 16.f, float cao = 55.f, bool cam = true);
    ~Coc() = default;

    sf::Vector2f GetViTri() const { return viTri; }
    void SetViTri(sf::Vector2f pos) { viTri = pos; }
    bool IsDaCam() const { return daCam; }
    void SetDaCam(bool c) { daCam = c; }
    int GetSatThuong() const { return satThuongVaCham; }
    bool IsDaPhatNoi() const { return daPhatNoi; }
    void SetDaPhatNoi(bool val) { daPhatNoi = val; }

    sf::FloatRect GetHitBox() const;

    // Vẽ cọc phụ thuộc vào độ sâu thủy triều (tiLeThuyTrieu: 1.0 = ngập hoàn toàn, 0.0 = cọc lộ hoàn toàn)
    void Ve(sf::RenderWindow& window, float tiLeThuyTrieu = 0.0f);
};


