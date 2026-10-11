#include "MayAnh.h"
#include "../CotLoi/TienIch.h"
#include <algorithm>
#include <cmath>

MayAnh::MayAnh()
    : tam({RONG_MAN_HINH * 0.5f, CAO_MAN_HINH * 0.5f}),
      mucTieu(tam),
      kichThuocTheGioi({RONG_MAN_HINH, CAO_MAN_HINH}),
      doMuot(6.f),
      zoom(1.f),
      thoiGianRung(0.f),
      thoiGianRungToiDa(0.f),
      bienDoRung(0.f),
      dongHo(0.f) {}

void MayAnh::DatTheGioi(float rong, float cao) {
    kichThuocTheGioi = {rong, cao};
    tam = KepTrongTheGioi(tam);
}

void MayAnh::DatNgayLapTuc(sf::Vector2f diem) {
    mucTieu = diem;
    tam = KepTrongTheGioi(diem);
}

void MayAnh::DatZoom(float giaTri) {
    zoom = std::clamp(giaTri, 0.5f, 3.f);
    tam = KepTrongTheGioi(tam);
}

void MayAnh::Rung(float bienDo, float thoiGian) {
    bienDoRung = bienDo;
    thoiGianRung = thoiGian;
    thoiGianRungToiDa = thoiGian;
}

sf::Vector2f MayAnh::KepTrongTheGioi(sf::Vector2f diem) const {
    float nuaRong = RONG_MAN_HINH / zoom * 0.5f;
    float nuaCao = CAO_MAN_HINH / zoom * 0.5f;

    // Thế giới nhỏ hơn màn hình thì giữ camera ở giữa thế giới
    float x = (kichThuocTheGioi.x <= nuaRong * 2.f) ? kichThuocTheGioi.x * 0.5f
                                                     : std::clamp(diem.x, nuaRong, kichThuocTheGioi.x - nuaRong);
    float y = (kichThuocTheGioi.y <= nuaCao * 2.f) ? kichThuocTheGioi.y * 0.5f
                                                    : std::clamp(diem.y, nuaCao, kichThuocTheGioi.y - nuaCao);
    return {x, y};
}

void MayAnh::CapNhat(float deltaTime) {
    dongHo += deltaTime;

    // Trượt dần về mục tiêu, độc lập với tốc độ khung hình
    float heSo = 1.f - std::exp(-doMuot * deltaTime);
    sf::Vector2f dich = KepTrongTheGioi(mucTieu);
    tam += (dich - tam) * heSo;

    if (thoiGianRung > 0.f) {
        thoiGianRung -= deltaTime;
        if (thoiGianRung < 0.f) thoiGianRung = 0.f;
    }
}

sf::View MayAnh::LayView(const sf::RenderWindow& window) const {
    sf::View view = TaoViewLogic(window);   // Giữ viewport chừa dải đen

    sf::Vector2f lechRung(0.f, 0.f);
    if (thoiGianRung > 0.f && thoiGianRungToiDa > 0.f) {
        float conLai = thoiGianRung / thoiGianRungToiDa;
        lechRung.x = std::sin(dongHo * 90.f) * bienDoRung * conLai;
        lechRung.y = std::cos(dongHo * 77.f) * bienDoRung * conLai;
    }

    view.setSize({RONG_MAN_HINH / zoom, CAO_MAN_HINH / zoom});
    view.setCenter(tam + lechRung);
    return view;
}

sf::Vector2f MayAnh::ChuotSangTheGioi(const sf::RenderWindow& window, sf::Vector2i pixel) const {
    return window.mapPixelToCoords(pixel, LayView(window));
}

sf::FloatRect MayAnh::GetVungNhin() const {
    float rong = RONG_MAN_HINH / zoom;
    float cao = CAO_MAN_HINH / zoom;
    return sf::FloatRect({tam.x - rong * 0.5f, tam.y - cao * 0.5f}, {rong, cao});
}
