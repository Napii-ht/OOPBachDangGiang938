
#include "VatPham.h"
#include "../BanDo/DoHoa.h"
#include <algorithm>

using namespace dohoa;

VatPham::VatPham(int id, const std::string& tenVatPham, const std::string& moTaVatPham,
                 LoaiVatPham loaiItem, int sl, int maxSl, sf::Color mau)
    : maVatPham(id),
      ten(tenVatPham),
      moTa(moTaVatPham),
      loai(loaiItem),
      soLuong(sl),
      soLuongToiDa(maxSl),
      mauBieuTuong(mau) {}

bool VatPham::GiamSoLuong(int sl) {
    if (soLuong >= sl) {
        soLuong -= sl;
        return true;
    }
    return false;
}

void VatPham::VeBieuTuong(sf::RenderWindow& window, sf::Vector2f viTri, float kichThuoc) const {
    sf::VertexArray va(sf::PrimitiveType::Triangles);

    const float x = viTri.x;
    const float y = viTri.y;
    const float s = kichThuoc * 0.035f;

    if (loai == LoaiVatPham::COC_GO) {
        // CỌC GỖ LIM BỊT SẮT (Vát nghiêng 25 độ)
        // Bóng mờ
        elip(va, x + 2.f, y + 2.f, 12.f * s, 4.f * s, mau(0, 0, 0, 70));

        // Thân gỗ lim sẫm
        tu(va,
           {x - 4.f * s, y - 8.f * s}, {x + 4.f * s, y - 8.f * s},
           {x + 5.f * s, y + 12.f * s}, {x - 5.f * s, y + 12.f * s},
           mau(138, 90, 50), mau(172, 122, 72), mau(96, 60, 34), mau(68, 40, 22));

        // Đai sắt bọc quanh cổ cọc
        chuNhat(va, x - 5.f * s, y - 9.f * s, 10.f * s, 3.f * s, mau(70, 78, 88));
        chuNhat(va, x - 5.f * s, y - 9.f * s, 10.f * s, 1.2f * s, mau(180, 195, 210));

        // Mũi nhọn bọc sắt hình tam giác
        tam3(va,
             {x, y - 18.f * s},
             {x - 6.f * s, y - 9.f * s},
             {x + 6.f * s, y - 9.f * s},
             mau(255, 255, 255), mau(160, 175, 190), mau(100, 115, 130));

        // Tia sáng lấp lánh ở đỉnh mũi sắt
        elip(va, x, y - 18.f * s, 2.5f * s, 2.5f * s, mau(255, 255, 255, 220), 8);

    } else if (loai == LoaiVatPham::VU_KHI) {
        // THANH BẢO KIẾM ĐẠI VIỆT
        // Lưỡi kiếm thép sắc bén chéo góc
        Vector2f pChuoi{x - 8.f * s, y + 8.f * s};
        Vector2f pMui{x + 12.f * s, y - 12.f * s};

        duongTan(va, pChuoi, pMui, 4.f * s, 2.5f * s, mau(210, 225, 240), mau(255, 255, 255));
        duongTan(va, pChuoi, pMui, 1.5f * s, 1.f * s, mau(255, 255, 255), mau(255, 255, 255));

        // Chuôi kiếm vàng đúc hoa văn
        duong(va, pChuoi, {pChuoi.x - 6.f * s, pChuoi.y + 6.f * s}, 3.5f * s, mau(218, 165, 32));
        // Đai chắn tay
        duong(va, {pChuoi.x - 3.5f * s, pChuoi.y - 3.5f * s}, {pChuoi.x + 3.5f * s, pChuoi.y + 3.5f * s}, 2.5f * s, mau(255, 215, 0));
        // Tua rua đỏ son
        duong(va, {pChuoi.x - 6.f * s, pChuoi.y + 6.f * s}, {pChuoi.x - 10.f * s, pChuoi.y + 11.f * s}, 1.8f * s, mau(220, 30, 30));

    } else if (loai == LoaiVatPham::HOI_MAU) {
        // BÌNH DƯỢC THẢO SINH LỰC
        // Đáy bình thủy tinh tròn
        elip(va, x, y + 2.f * s, 9.f * s, 9.f * s, mau(220, 25, 45), 14);
        elip(va, x - 2.5f * s, y - 1.f * s, 3.5f * s, 4.5f * s, mau(255, 130, 145), 8); // Vệt sáng thủy tinh
        // Cổ bình
        chuNhat(va, x - 3.f * s, y - 8.f * s, 6.f * s, 4.f * s, mau(180, 215, 235, 200));
        // Nút bần gỗ
        chuNhat(va, x - 2.5f * s, y - 11.f * s, 5.f * s, 3.f * s, mau(150, 100, 55));

    } else {
        // VẬT PHẨM LỆNH KỲ / KHÁC
        chuNhat(va, x - 8.f * s, y - 8.f * s, 16.f * s, 16.f * s, mauBieuTuong);
        chuNhat(va, x - 7.f * s, y - 7.f * s, 14.f * s, 14.f * s, sang(mauBieuTuong, 0.3f));
    }

    window.draw(va);
}


