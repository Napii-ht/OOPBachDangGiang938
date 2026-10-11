#include "VatTrangTri.h"
#include "DoHoa.h"
#include <cmath>

using namespace dohoa;

VatTrangTri::VatTrangTri(LoaiVat loaiVat, sf::Vector2f viTriChan, float tiLeVe, int bienTheVe)
    : loai(loaiVat),
      viTri(viTriChan),
      tiLe(tiLeVe),
      bienThe(bienTheVe),
      daDung(false),
      thoiGianRung(0.f),
      thoiGianDaDung(0.f) {}

void VatTrangTri::CapNhat(float deltaTime) {
    if (thoiGianRung > 0.f) thoiGianRung = std::max(0.f, thoiGianRung - deltaTime);
    thoiGianDaDung += deltaTime;
}

sf::FloatRect VatTrangTri::GetKhungVaCham() const {
    const float x = viTri.x;
    const float y = viTri.y;
    const float s = tiLe;
    switch (loai) {
        case LoaiVat::CAY:
        case LoaiVat::CAY_LIM:
            if (loai == LoaiVat::CAY_LIM && daDung) return sf::FloatRect({x - 6.f * s, y - 6.f * s}, {12.f * s, 8.f * s});
            return sf::FloatRect({x - 9.f * s, y - 10.f * s}, {18.f * s, 14.f * s});
        case LoaiVat::DA:         return sf::FloatRect({x - 18.f * s, y - 12.f * s}, {36.f * s, 16.f * s});
        case LoaiVat::LEU: {
            float k = (bienThe == 2) ? 1.5f : 1.f;
            return sf::FloatRect({x - 40.f * s * k, y - 18.f * s}, {80.f * s * k, 20.f * s});
        }
        case LoaiVat::CHOI_CANH:  return sf::FloatRect({x - 26.f * s, y - 14.f * s}, {52.f * s, 16.f * s});
        case LoaiVat::DONG_LUA:   return sf::FloatRect({x - 13.f * s, y - 8.f * s}, {26.f * s, 12.f * s});
        case LoaiVat::GIA_VU_KHI: return sf::FloatRect({x - 32.f * s, y - 10.f * s}, {64.f * s, 12.f * s});
        case LoaiVat::BU_NHIN:    return sf::FloatRect({x - 11.f * s, y - 8.f * s}, {22.f * s, 12.f * s});
        case LoaiVat::HANG_RAO:   return sf::FloatRect({x - 40.f * s, y - 8.f * s}, {80.f * s, 10.f * s});
        case LoaiVat::THUNG:      return sf::FloatRect({x - 12.f * s, y - 10.f * s}, {24.f * s, 12.f * s});
        case LoaiVat::LO_REN:     return sf::FloatRect({x - 44.f * s, y - 18.f * s}, {110.f * s, 20.f * s});
        case LoaiVat::XE_BO:      return sf::FloatRect({x - 40.f * s, y - 14.f * s}, {80.f * s, 18.f * s});
        case LoaiVat::COT_CO:     return sf::FloatRect({x - 7.f * s, y - 6.f * s}, {14.f * s, 9.f * s});
        case LoaiVat::THUYEN_NEO: return sf::FloatRect({x - 12.f * s, y - 30.f * s}, {24.f * s, 60.f * s});
        case LoaiVat::BUI:
        case LoaiVat::DONG_SAT:
        default:                  return sf::FloatRect({0.f, 0.f}, {0.f, 0.f});
    }
}

sf::FloatRect VatTrangTri::GetKhungHienThi() const {
    float r = 110.f * tiLe * ((loai == LoaiVat::LEU && bienThe == 2) ? 1.5f : 1.f);
    return sf::FloatRect({viTri.x - r, viTri.y - r * 1.3f}, {r * 2.f, r * 1.5f});
}

void VatTrangTri::Ve(sf::RenderWindow& window, float thoiGian) const {
    VA va(sf::PrimitiveType::Triangles);
    const float x = viTri.x;
    const float y = viTri.y;
    const float s = tiLe;
    const float seed = static_cast<float>(bienThe) * 1.7f + x * 0.013f;

    float rung = 0.f;
    if (thoiGianRung > 0.f) rung = std::sin(thoiGian * 70.f) * 3.f * s * std::min(1.f, thoiGianRung * 3.f);

    switch (loai) {
        case LoaiVat::CAY:
        case LoaiVat::CAY_LIM: {
            bool lim = (loai == LoaiVat::CAY_LIM);
            elip(va, x, y + 1.f, 28.f * s, 9.f * s, mau(0, 0, 0, 70));   // Bóng

            if (lim && daDung) {
                // Chỉ còn gốc cây
                chuNhat(va, x - 8.f * s, y - 9.f * s, 16.f * s, 10.f * s, mau(105, 62, 40));
                elip(va, x, y - 9.f * s, 8.f * s, 3.5f * s, mau(185, 140, 95));
                elip(va, x, y - 9.f * s, 4.5f * s, 1.8f * s, mau(150, 105, 65));
                break;
            }

            Color than = lim ? mau(112, 58, 40) : mau(98, 68, 44);
            float rongThan = lim ? 12.f : 10.f;
            chuNhat(va, x - rongThan * 0.5f * s, y - 40.f * s, rongThan * s, 42.f * s, than);
            chuNhat(va, x - rongThan * 0.5f * s, y - 40.f * s, rongThan * 0.3f * s, 42.f * s, toi(than, 0.75f));
            if (lim) {
                for (int i = 0; i < 4; ++i) {
                    float yy = y - (10.f + static_cast<float>(i) * 8.f) * s;
                    duong(va, {x - 4.f * s, yy}, {x + 3.f * s, yy - 2.f * s}, 1.3f, toi(than, 0.6f));
                }
            }

            float lac = std::sin(thoiGian * 1.1f + seed) * 1.6f * s + rung;
            Color tanToi = lim ? mau(28, 76, 46) : mau(46, 118, 52);
            Color tanVua = lim ? mau(38, 98, 58) : mau(62, 146, 66);
            Color tanSang = lim ? mau(60, 128, 74) : mau(98, 178, 86);
            float cao = lim ? 74.f : 62.f;
            elip(va, x + lac * 0.5f, y - (cao - 6.f) * s, 32.f * s, 27.f * s, tanToi);
            elip(va, x - 15.f * s + lac, y - (cao - 14.f) * s, 22.f * s, 18.f * s, tanVua);
            elip(va, x + 16.f * s + lac, y - (cao - 12.f) * s, 22.f * s, 18.f * s, tanVua);
            elip(va, x - 3.f * s + lac, y - (cao + 10.f) * s, 20.f * s, 15.f * s, tanSang);
            if (!lim) {
                // Vài chấm sáng để tán cây có khối
                elip(va, x + 10.f * s + lac, y - (cao + 4.f) * s, 6.f * s, 4.f * s, mau(130, 205, 105, 190), 8);
            }
            break;
        }

        case LoaiVat::BUI: {
            elip(va, x, y, 18.f * s, 6.f * s, mau(0, 0, 0, 55));
            elip(va, x - 8.f * s, y - 8.f * s, 12.f * s, 9.f * s, mau(48, 112, 56));
            elip(va, x + 8.f * s, y - 7.f * s, 12.f * s, 9.f * s, mau(58, 128, 62));
            elip(va, x, y - 13.f * s, 11.f * s, 8.f * s, mau(82, 156, 80));
            if (bienThe % 3 == 0) {
                for (int i = 0; i < 4; ++i) {
                    elip(va, x - 9.f * s + static_cast<float>(i) * 6.f * s, y - (9.f + static_cast<float>(i % 2) * 5.f) * s, 1.8f * s, 1.8f * s, mau(220, 70, 70), 6);
                }
            }
            break;
        }

        case LoaiVat::DA: {
            elip(va, x, y + 2.f, 24.f * s, 7.f * s, mau(0, 0, 0, 60));
            elip(va, x, y - 7.f * s, 19.f * s, 12.f * s, mau(112, 116, 124));
            elip(va, x - 4.f * s, y - 10.f * s, 11.f * s, 7.f * s, mau(146, 150, 158));
            elip(va, x + 8.f * s, y - 4.f * s, 8.f * s, 5.f * s, mau(92, 96, 104));
            break;
        }

        case LoaiVat::LEU: {
            float k = (bienThe == 2) ? 1.5f : 1.f;
            Color vai = (bienThe == 0) ? mau(214, 192, 148) : (bienThe == 1 ? mau(104, 130, 92) : mau(150, 40, 38));
            Color vien = (bienThe == 2) ? mau(230, 190, 70) : toi(vai, 0.6f);
            float W = 46.f * s * k;
            float H = 62.f * s * k;
            elip(va, x, y + 2.f, W * 1.15f, 10.f * s, mau(0, 0, 0, 70));
            tam(va, {x - W, y}, {x, y - H}, {x + W, y}, vai);
            tam(va, {x, y - H}, {x + W, y}, {x + W * 0.1f, y}, toi(vai, 0.8f));
            tam3(va, {x - W * 0.28f, y}, {x, y - H * 0.55f}, {x + W * 0.28f, y}, mau(30, 22, 20), mau(18, 12, 12), mau(30, 22, 20));
            duong(va, {x - W, y}, {x, y - H}, 2.2f, vien);
            duong(va, {x + W, y}, {x, y - H}, 2.2f, vien);
            // Cọc cờ và lá cờ phấp phới ở đỉnh
            duong(va, {x, y - H}, {x, y - H - 16.f * s * k}, 2.f, mau(90, 60, 40));
            float phap = std::sin(thoiGian * 3.f + seed) * 3.f * s;
            tam(va, {x, y - H - 16.f * s * k}, {x + 18.f * s * k + phap, y - H - 12.f * s * k}, {x, y - H - 7.f * s * k}, mau(190, 36, 30));
            if (bienThe == 2) {
                // Dải vàng chính giữa lều chỉ huy
                chuNhat(va, x - 2.f * s, y - H * 0.45f - 8.f, 4.f * s, 8.f, mau(235, 195, 70));
            }
            break;
        }

        case LoaiVat::CHOI_CANH: {
            elip(va, x, y + 2.f, 36.f * s, 9.f * s, mau(0, 0, 0, 65));
            Color go = mau(120, 84, 52);
            duong(va, {x - 24.f * s, y}, {x - 19.f * s, y - 74.f * s}, 4.f * s, go);
            duong(va, {x + 24.f * s, y}, {x + 19.f * s, y - 74.f * s}, 4.f * s, go);
            duong(va, {x - 24.f * s, y - 4.f * s}, {x + 19.f * s, y - 40.f * s}, 2.5f * s, toi(go, 0.85f));
            duong(va, {x + 24.f * s, y - 4.f * s}, {x - 19.f * s, y - 40.f * s}, 2.5f * s, toi(go, 0.85f));
            chuNhat(va, x - 30.f * s, y - 82.f * s, 60.f * s, 9.f * s, toi(go, 0.9f));
            for (int i = 0; i < 6; ++i) {
                chuNhat(va, x - 28.f * s + static_cast<float>(i) * 11.f * s, y - 98.f * s, 3.f * s, 16.f * s, go);
            }
            tam(va, {x - 36.f * s, y - 98.f * s}, {x, y - 124.f * s}, {x + 36.f * s, y - 98.f * s}, mau(150, 52, 40));
            float phap = std::sin(thoiGian * 3.f + seed) * 3.f * s;
            duong(va, {x, y - 124.f * s}, {x, y - 138.f * s}, 2.f, mau(90, 60, 40));
            tam(va, {x, y - 138.f * s}, {x + 16.f * s + phap, y - 134.f * s}, {x, y - 128.f * s}, mau(190, 36, 30));
            break;
        }

        case LoaiVat::DONG_LUA: {
            float nhay = 0.5f + 0.5f * std::sin(thoiGian * 11.f + seed);
            float nhay2 = 0.5f + 0.5f * std::sin(thoiGian * 17.f + seed * 2.f);
            elip(va, x, y - 6.f * s, (46.f + 6.f * nhay) * s, (30.f + 4.f * nhay) * s, mau(255, 170, 70, 26));   // Quầng sáng
            elip(va, x, y + 1.f, 16.f * s, 5.f * s, mau(0, 0, 0, 80));
            for (int i = 0; i < 7; ++i) {
                float a = 2.f * PI * static_cast<float>(i) / 7.f;
                elip(va, x + std::cos(a) * 11.f * s, y - 3.f * s + std::sin(a) * 4.f * s, 3.8f * s, 3.f * s, mau(118, 118, 126), 8);
            }
            duong(va, {x - 9.f * s, y - 2.f * s}, {x + 9.f * s, y - 6.f * s}, 3.f * s, mau(84, 56, 36));
            duong(va, {x + 9.f * s, y - 2.f * s}, {x - 9.f * s, y - 6.f * s}, 3.f * s, mau(96, 64, 40));
            float cao = (16.f + 5.f * nhay) * s;
            tam(va, {x - 8.f * s, y - 5.f * s}, {x + 8.f * s, y - 5.f * s}, {x + (nhay2 - 0.5f) * 5.f * s, y - 5.f * s - cao}, mau(230, 90, 30, 235));
            tam(va, {x - 5.f * s, y - 5.f * s}, {x + 5.f * s, y - 5.f * s}, {x + (nhay - 0.5f) * 4.f * s, y - 5.f * s - cao * 0.7f}, mau(255, 170, 50));
            tam(va, {x - 2.5f * s, y - 5.f * s}, {x + 2.5f * s, y - 5.f * s}, {x, y - 5.f * s - cao * 0.4f}, mau(255, 235, 140));
            break;
        }

        case LoaiVat::GIA_VU_KHI: {
            elip(va, x, y + 1.f, 38.f * s, 7.f * s, mau(0, 0, 0, 60));
            Color go = mau(116, 80, 50);
            duong(va, {x - 30.f * s, y}, {x - 30.f * s, y - 50.f * s}, 4.f * s, go);
            duong(va, {x + 30.f * s, y}, {x + 30.f * s, y - 50.f * s}, 4.f * s, go);
            duong(va, {x - 32.f * s, y - 38.f * s}, {x + 32.f * s, y - 38.f * s}, 3.f * s, toi(go, 0.9f));
            duong(va, {x - 32.f * s, y - 20.f * s}, {x + 32.f * s, y - 20.f * s}, 3.f * s, toi(go, 0.9f));
            for (int i = 0; i < 4; ++i) {   // Giáo dựng thẳng
                float xx = x - 21.f * s + static_cast<float>(i) * 11.f * s;
                duong(va, {xx, y - 14.f * s}, {xx + 1.f, y - 66.f * s}, 1.8f * s, mau(140, 104, 70));
                tam(va, {xx - 2.8f * s, y - 62.f * s}, {xx + 2.8f * s, y - 62.f * s}, {xx + 0.5f, y - 76.f * s}, mau(210, 218, 228));
            }
            elip(va, x + 14.f * s, y - 28.f * s, 9.f * s, 9.f * s, mau(150, 40, 36));
            elip(va, x + 14.f * s, y - 28.f * s, 3.f * s, 3.f * s, mau(235, 195, 70), 8);
            break;
        }

        case LoaiVat::BU_NHIN: {
            elip(va, x, y + 1.f, 16.f * s, 5.f * s, mau(0, 0, 0, 60));
            float lac = rung * 1.2f;
            duong(va, {x, y}, {x + lac * 0.3f, y - 44.f * s}, 4.f * s, mau(112, 78, 48));
            duong(va, {x - 18.f * s + lac, y - 34.f * s}, {x + 18.f * s + lac, y - 34.f * s}, 3.5f * s, mau(124, 88, 54));
            elip(va, x + lac * 0.6f, y - 28.f * s, 12.f * s, 15.f * s, mau(206, 176, 98));
            elip(va, x + lac * 0.6f, y - 28.f * s, 8.f * s, 10.f * s, mau(176, 40, 36));
            elip(va, x + lac * 0.6f, y - 28.f * s, 4.f * s, 5.f * s, mau(240, 232, 214));
            elip(va, x + lac, y - 52.f * s, 7.f * s, 8.f * s, mau(196, 164, 112));
            break;
        }

        case LoaiVat::HANG_RAO: {
            for (int i = 0; i < 5; ++i) {
                float xx = x - 36.f * s + static_cast<float>(i) * 18.f * s;
                duong(va, {xx, y}, {xx, y - 26.f * s}, 4.f * s, mau(150, 124, 72));
                tam(va, {xx - 2.f * s, y - 26.f * s}, {xx + 2.f * s, y - 26.f * s}, {xx, y - 31.f * s}, mau(150, 124, 72));
            }
            duong(va, {x - 40.f * s, y - 18.f * s}, {x + 40.f * s, y - 18.f * s}, 3.f * s, mau(124, 100, 58));
            duong(va, {x - 40.f * s, y - 8.f * s}, {x + 40.f * s, y - 8.f * s}, 3.f * s, mau(124, 100, 58));
            break;
        }

        case LoaiVat::THUNG: {
            elip(va, x, y + 1.f, 15.f * s, 5.f * s, mau(0, 0, 0, 60));
            chuNhat(va, x - 11.f * s, y - 22.f * s, 22.f * s, 22.f * s, mau(132, 92, 58));
            elip(va, x, y - 22.f * s, 11.f * s, 4.f * s, mau(158, 116, 76));
            chuNhat(va, x - 11.f * s, y - 16.f * s, 22.f * s, 2.f * s, mau(84, 70, 60));
            chuNhat(va, x - 11.f * s, y - 7.f * s, 22.f * s, 2.f * s, mau(84, 70, 60));
            break;
        }

        case LoaiVat::LO_REN: {
            float nhay = 0.5f + 0.5f * std::sin(thoiGian * 9.f);
            elip(va, x, y + 2.f, 56.f * s, 10.f * s, mau(0, 0, 0, 70));
            elip(va, x - 6.f * s, y - 20.f * s, 56.f * s, 40.f * s, mau(255, 150, 60, static_cast<float>(20 + 14 * nhay)));
            // Thân lò đắp đá
            chuNhatDoc(va, x - 40.f * s, y - 46.f * s, 66.f * s, 46.f * s, mau(128, 126, 134), mau(92, 90, 100));
            chuNhat(va, x - 44.f * s, y - 50.f * s, 74.f * s, 7.f * s, mau(150, 148, 156));
            chuNhat(va, x + 6.f * s, y - 78.f * s, 14.f * s, 30.f * s, mau(112, 110, 118));   // Ống khói
            // Miệng lò đỏ rực
            tam3(va, {x - 22.f * s, y}, {x - 22.f * s, y - 26.f * s}, {x + 8.f * s, y - 26.f * s}, mau(250, 120, 30), mau(255, 190, 70), mau(255, 190, 70));
            chuNhatDoc(va, x - 22.f * s, y - 26.f * s, 30.f * s, 26.f * s, mau(255, 190 + 30 * nhay, 70), mau(220, 70, 24));
            // Khói
            for (int i = 0; i < 3; ++i) {
                float t = std::fmod(thoiGian * 0.6f + static_cast<float>(i) * 0.33f, 1.f);
                elip(va, x + 13.f * s + std::sin(t * 6.f) * 6.f * s, y - 80.f * s - t * 40.f * s, (5.f + t * 8.f) * s, (4.f + t * 6.f) * s, mau(150, 150, 156, 130.f * (1.f - t)));
            }
            // Đe bên phải lò
            chuNhat(va, x + 44.f * s, y - 14.f * s, 20.f * s, 14.f * s, mau(58, 58, 66));
            tam(va, {x + 36.f * s, y - 28.f * s}, {x + 76.f * s, y - 28.f * s}, {x + 64.f * s, y - 14.f * s}, mau(76, 76, 86));
            tam(va, {x + 36.f * s, y - 28.f * s}, {x + 64.f * s, y - 14.f * s}, {x + 44.f * s, y - 14.f * s}, mau(62, 62, 72));
            chuNhat(va, x + 38.f * s, y - 31.f * s, 36.f * s, 4.f * s, mau(110, 110, 120));
            break;
        }

        case LoaiVat::DONG_SAT: {
            elip(va, x, y + 1.f, 22.f * s, 6.f * s, mau(0, 0, 0, 60));
            if (daDung) {
                // Đã lấy hết: còn vệt đen cháy xém
                elip(va, x, y - 2.f, 16.f * s, 5.f * s, mau(48, 44, 42, 150));
                break;
            }
            for (int h = 0; h < 3; ++h) {
                for (int i = 0; i < 4 - h; ++i) {
                    float xx = x - (18.f - static_cast<float>(h) * 5.f) * s + static_cast<float>(i) * 10.f * s;
                    float yy = y - (4.f + static_cast<float>(h) * 6.f) * s;
                    chuNhat(va, xx - 6.f * s, yy - 4.f * s, 14.f * s, 5.f * s, mau(108, 118, 138));
                    chuNhat(va, xx - 6.f * s, yy - 4.f * s, 14.f * s, 1.8f * s, mau(176, 188, 208));
                }
            }
            float lap = 0.5f + 0.5f * std::sin(thoiGian * 3.f + seed);
            elip(va, x + 8.f * s, y - 20.f * s, 2.5f * s, 2.5f * s, mau(255, 255, 255, 120.f + 100.f * lap), 6);   // Ánh kim loại
            break;
        }

        case LoaiVat::XE_BO: {
            elip(va, x, y + 2.f, 46.f * s, 9.f * s, mau(0, 0, 0, 65));
            chuNhat(va, x - 36.f * s, y - 30.f * s, 72.f * s, 14.f * s, mau(140, 98, 60));
            chuNhat(va, x - 36.f * s, y - 30.f * s, 72.f * s, 3.f * s, mau(168, 124, 80));
            duong(va, {x - 36.f * s, y - 24.f * s}, {x - 58.f * s, y - 20.f * s}, 3.5f * s, mau(112, 78, 48));   // Càng xe
            if (bienThe == 1) {
                for (int i = 0; i < 6; ++i) {   // Cọc nhọn bịt sắt xếp trên xe
                    float xx = x - 30.f * s + static_cast<float>(i) * 11.f * s;
                    duong(va, {xx, y - 28.f * s}, {xx + 4.f * s, y - 52.f * s}, 5.f * s, mau(120, 66, 44));
                    tam(va, {xx + 1.f * s, y - 51.f * s}, {xx + 7.f * s, y - 51.f * s}, {xx + 5.f * s, y - 64.f * s}, mau(176, 188, 208));
                }
            }
            for (int b = 0; b < 2; ++b) {
                float xx = x - 22.f * s + static_cast<float>(b) * 44.f * s;
                elip(va, xx, y - 10.f * s, 11.f * s, 11.f * s, mau(86, 58, 36));
                elip(va, xx, y - 10.f * s, 7.f * s, 7.f * s, mau(146, 106, 66));
                elip(va, xx, y - 10.f * s, 2.5f * s, 2.5f * s, mau(70, 50, 34), 8);
            }
            break;
        }

        case LoaiVat::COT_CO: {
            elip(va, x, y + 1.f, 18.f * s, 6.f * s, mau(0, 0, 0, 60));
            for (int i = 0; i < 5; ++i) {
                float a = 2.f * PI * static_cast<float>(i) / 5.f;
                elip(va, x + std::cos(a) * 9.f * s, y - 2.f * s + std::sin(a) * 3.f * s, 4.f * s, 3.f * s, mau(120, 120, 128), 8);
            }
            duong(va, {x, y - 2.f * s}, {x, y - 118.f * s}, 4.5f * s, mau(120, 84, 52));
            elip(va, x, y - 120.f * s, 4.f * s, 4.f * s, mau(235, 195, 70), 8);
            float phap = std::sin(thoiGian * 2.6f + seed) * 4.f * s;
            float phap2 = std::sin(thoiGian * 2.6f + seed + 1.f) * 4.f * s;
            tu(va, {x + 2.f * s, y - 114.f * s}, {x + 44.f * s + phap, y - 110.f * s}, {x + 44.f * s + phap2, y - 80.f * s}, {x + 2.f * s, y - 84.f * s},
               mau(200, 40, 34), mau(170, 30, 26), mau(150, 24, 22), mau(190, 36, 30));
            elip(va, x + 22.f * s + phap * 0.5f, y - 97.f * s, 7.f * s, 7.f * s, mau(240, 200, 80), 10);
            break;
        }

        case LoaiVat::THUYEN_NEO: {
            // Thuyền đặt dọc theo bờ sông, mũi hướng lên trên, nhấp nhô theo sóng
            float nhun = std::sin(thoiGian * 1.4f + seed) * 2.f * s;
            float cy = y + nhun;
            elip(va, x, cy, 24.f * s, 50.f * s, mau(20, 60, 90, 70));                    // Bóng nước
            vong(va, x, cy, 22.f * s, 46.f * s, 2.f, mau(235, 245, 250, 90), 20);         // Gợn sóng quanh thân
            // Thân thuyền
            tu(va, {x - 11.f * s, cy - 40.f * s}, {x + 11.f * s, cy - 40.f * s}, {x + 16.f * s, cy + 36.f * s}, {x - 16.f * s, cy + 36.f * s},
               mau(150, 100, 60), mau(130, 84, 50), mau(104, 66, 40), mau(112, 72, 44));
            tam(va, {x - 11.f * s, cy - 40.f * s}, {x + 11.f * s, cy - 40.f * s}, {x, cy - 56.f * s}, mau(168, 116, 70));
            chuNhat(va, x - 14.f * s, cy - 8.f * s, 28.f * s, 3.f * s, mau(92, 58, 36));
            chuNhat(va, x - 14.f * s, cy + 14.f * s, 28.f * s, 3.f * s, mau(92, 58, 36));
            // Lòng thuyền, cột buồm và lá cờ đỏ
            chuNhat(va, x - 8.f * s, cy - 36.f * s, 16.f * s, 66.f * s, mau(76, 50, 32));
            duong(va, {x, cy - 4.f * s}, {x, cy - 78.f * s}, 3.f * s, mau(104, 72, 46));
            float phap = std::sin(thoiGian * 3.f + seed) * 3.f * s;
            tam(va, {x, cy - 78.f * s}, {x + 16.f * s + phap, cy - 74.f * s}, {x, cy - 67.f * s}, mau(190, 36, 30));
            // Buồm cuộn vào cột
            chuNhat(va, x - 12.f * s, cy - 42.f * s, 24.f * s, 4.f * s, mau(214, 192, 148));
            break;
        }
    }

    window.draw(va);
}
