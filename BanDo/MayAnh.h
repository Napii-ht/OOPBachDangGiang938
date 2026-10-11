#pragma once

#include <SFML/Graphics.hpp>

// Camera cho các chương có bản đồ lớn hơn màn hình: bám theo mục tiêu mượt mà,
// không đi ra ngoài mép bản đồ, có rung màn hình và zoom.
class MayAnh {
private:
    sf::Vector2f tam;           // Tâm camera hiện tại
    sf::Vector2f mucTieu;       // Điểm camera đang đuổi theo
    sf::Vector2f kichThuocTheGioi;
    float doMuot;               // Càng lớn camera càng bám sát
    float zoom;                 // 1 = bình thường, lớn hơn 1 = phóng to

    float thoiGianRung;
    float thoiGianRungToiDa;
    float bienDoRung;
    float dongHo;

    sf::Vector2f KepTrongTheGioi(sf::Vector2f diem) const;

public:
    MayAnh();

    void DatTheGioi(float rong, float cao);
    void DatMucTieu(sf::Vector2f diem) { mucTieu = diem; }
    void DatNgayLapTuc(sf::Vector2f diem);   // Nhảy thẳng tới điểm, không trượt
    void DatDoMuot(float giaTri) { doMuot = giaTri; }
    void DatZoom(float giaTri);

    void Rung(float bienDo, float thoiGian);
    void CapNhat(float deltaTime);

    // View để vẽ thế giới, đã giữ đúng tỉ lệ khung hình của cửa sổ
    sf::View LayView(const sf::RenderWindow& window) const;

    // Đổi vị trí chuột sang tọa độ thế giới
    sf::Vector2f ChuotSangTheGioi(const sf::RenderWindow& window, sf::Vector2i pixel) const;

    sf::Vector2f GetTam() const { return tam; }
    sf::FloatRect GetVungNhin() const;
    float GetZoom() const { return zoom; }
};
