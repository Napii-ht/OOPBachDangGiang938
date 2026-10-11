#include "NhanVatPhu.h"
#include "../BanDo/DoHoa.h"
#include <cmath>

using namespace dohoa;

NhanVatPhu::NhanVatPhu(sf::Vector2f viTriBanDau, LoaiNPC loaiNpc, const std::string& tenNpc,
                       const std::vector<std::string>& hoiThoai)
    : NhanVat(viTriBanDau, 0.f, 200, 0),
      ten(tenNpc),
      loai(loaiNpc),
      danhSachLoiThoai(hoiThoai),
      chiSoLoiThoaiHienTai(0),
      phamViTuongTac(68.f),
      dangTuongTac(false),
      thoiGianHoatAnh(0.f) {}

bool NhanVatPhu::KiemTraPhamViTuongTac(sf::Vector2f viTriPlayer) const {
    float dx = viTriPlayer.x - viTri.x;
    float dy = viTriPlayer.y - viTri.y;
    return (dx * dx + dy * dy) <= (phamViTuongTac * phamViTuongTac);
}

std::string NhanVatPhu::LayLoiThoaiTiepTheo() {
    if (danhSachLoiThoai.empty()) return "...";
    
    std::string cauThoai = danhSachLoiThoai[chiSoLoiThoaiHienTai];
    chiSoLoiThoaiHienTai = (chiSoLoiThoaiHienTai + 1) % static_cast<int>(danhSachLoiThoai.size());
    return cauThoai;
}

void NhanVatPhu::ResetLoiThoai() {
    chiSoLoiThoaiHienTai = 0;
}

void NhanVatPhu::CapNhat(float deltaTime) {
    thoiGianHoatAnh += deltaTime;
}

void NhanVatPhu::Ve(sf::RenderWindow& window) {
    mangDinh.clear();

    const float x = viTri.x;
    const float y = viTri.y;
    const float t = thoiGianHoatAnh;

    // 1. VÒNG HÀO QUANG TƯƠNG TÁC DƯỚI ĐẤT (Interactive Ripple)
    {
        float pha = std::fmod(t * 0.8f, 1.f);
        float rGoc = phamViTuongTac * 0.45f;
        float rRong = rGoc + pha * (phamViTuongTac * 0.4f);
        float alpha = (1.f - pha) * 90.f;
        vong(mangDinh, x, y + 16.f, rRong, rRong * 0.35f, 1.6f, mau(255, 230, 110, alpha), 24);
        elip(mangDinh, x, y + 16.f, 16.f, 6.f, mau(0, 0, 0, 85.f), 12); // Bóng đổ
    }

    if (loai == LoaiNPC::NGO_QUYEN) {
        // =========================================================================
        //  CHỦ SOÁI NGÔ QUYỀN - PHONG THÁI ANH HÙNG DÂN TỘC
        // =========================================================================
        Color kAoBao = mau(86, 26, 112);
        Color kAoBaoToi = mau(48, 14, 66);
        Color kAoChoang = mau(196, 30, 30);
        Color kAoChoangToi = mau(120, 18, 18);
        Color kVangKim = mau(255, 215, 0);
        Color kVangDong = mau(212, 160, 54);
        Color kDa = mau(248, 212, 180);
        Color kRau = mau(36, 28, 24);

        // Áo choàng đỏ vương giả phía sau lưng tung bay nhẹ
        float gioBay = std::sin(t * 2.2f) * 4.f;
        tu(mangDinh,
           {x - 12.f, y - 10.f}, {x + 12.f, y - 10.f},
           {x + 16.f + gioBay, y + 19.f}, {x - 16.f + gioBay, y + 19.f},
           kAoChoang, kAoChoang, kAoChoangToi, kAoChoangToi);

        // Đôi ủng da đen viền vàng
        chuNhatDoc(mangDinh, x - 8.f, y + 11.f, 6.f, 6.f, mau(40, 30, 25), mau(20, 15, 12));
        chuNhatDoc(mangDinh, x + 2.f, y + 11.f, 6.f, 6.f, mau(40, 30, 25), mau(20, 15, 12));

        // Thân áo bào tím thêu viền vàng
        tu(mangDinh,
           {x - 11.f, y - 12.f}, {x + 11.f, y - 12.f},
           {x + 13.f, y + 12.f}, {x - 13.f, y + 12.f},
           kAoBao, kAoBao, kAoBaoToi, kAoBaoToi);

        // Đai ngọc thắt lưng hoàng gia & dải ngọc bội
        chuNhat(mangDinh, x - 12.f, y + 2.f, 24.f, 4.f, kVangKim);
        elip(mangDinh, x, y + 4.f, 3.5f, 3.5f, mau(70, 180, 110), 8); // Ngọc bích trung tâm
        chuNhatDoc(mangDinh, x - 2.f, y + 6.f, 4.f, 9.f, kVangDong, kAoBao);

        // Cầu vai giáp mạ vàng kim uy dũng
        elip(mangDinh, x - 12.f, y - 8.f, 5.5f, 4.5f, kVangKim, 8);
        elip(mangDinh, x + 12.f, y - 8.f, 5.5f, 4.5f, kVangKim, 8);

        // Thanh kiếm lệnh chỉ huy bên hông trái
        duong(mangDinh, {x - 12.f, y - 2.f}, {x - 18.f, y + 16.f}, 3.5f, kVangDong);
        duong(mangDinh, {x - 9.f, y - 4.f}, {x - 15.f, y}, 2.f, kVangKim); // Chuôi kiếm

        // Cổ áo viền gấm vàng
        tu(mangDinh, {x - 5.f, y - 12.f}, {x + 5.f, y - 12.f}, {x, y - 7.f}, {x, y - 7.f}, kVangKim, kVangKim, kVangKim, kVangKim);

        // Đầu & Khuôn mặt uy nghiêm
        float yDau = y - 22.f;
        elip(mangDinh, x, yDau, 9.5f, 10.5f, kDa, 14);

        // Mắt rồng kiên nghị & Lông mày tướng quân
        elip(mangDinh, x - 3.5f, yDau - 1.f, 1.6f, 1.6f, mau(20, 20, 20), 6);
        elip(mangDinh, x + 3.5f, yDau - 1.f, 1.6f, 1.6f, mau(20, 20, 20), 6);
        duong(mangDinh, {x - 6.f, yDau - 4.f}, {x - 1.5f, yDau - 3.f}, 1.6f, kRau);
        duong(mangDinh, {x + 1.5f, yDau - 3.f}, {x + 6.f, yDau - 4.f}, 1.6f, kRau);

        // Chòm râu tướng quân anh dũng
        tam(mangDinh, {x - 4.f, yDau + 5.f}, {x + 4.f, yDau + 5.f}, {x, yDau + 14.f}, kRau);

        // Mũ trụ đại soái mạ vàng với cánh phượng hoàng
        tu(mangDinh,
           {x - 11.f, yDau - 6.f}, {x + 11.f, yDau - 6.f},
           {x + 10.f, yDau - 12.f}, {x - 10.f, yDau - 12.f},
           kVangKim, kVangKim, kVangDong, kVangDong);
        // Đỉnh mũ vút cao
        tam(mangDinh, {x - 5.f, yDau - 12.f}, {x + 5.f, yDau - 12.f}, {x, yDau - 20.f}, kVangKim);
        elip(mangDinh, x, yDau - 14.f, 2.5f, 2.5f, mau(220, 30, 30), 8); // Viên ngọc đỏ đỉnh mũ

    } else if (loai == LoaiNPC::LINH_DAI_VIET) {
        // =========================================================================
        //  Binh Lính ĐẠI VIỆT - TRƯỜNG THƯƠNG SÁNG LOÁNG
        // =========================================================================
        Color kGiapLam = mau(36, 75, 140);
        Color kDong = mau(195, 145, 52);
        Color kDa = mau(240, 205, 172);

        // Chân ủng
        chuNhatDoc(mangDinh, x - 6.f, y + 10.f, 5.f, 6.f, mau(45, 35, 25), mau(25, 20, 15));
        chuNhatDoc(mangDinh, x + 1.f, y + 10.f, 5.f, 6.f, mau(45, 35, 25), mau(25, 20, 15));

        // Thân giáp sắt viền đồng
        tu(mangDinh,
           {x - 9.f, y - 10.f}, {x + 9.f, y - 10.f},
           {x + 11.f, y + 10.f}, {x - 11.f, y + 10.f},
           kGiapLam, kGiapLam, mau(22, 50, 95), mau(22, 50, 95));

        // Tấm gương hộ tâm
        elip(mangDinh, x, y - 1.f, 5.f, 5.f, kDong, 8);

        // Đầu & Nón chiến bọc đồng
        float yDau = y - 19.f;
        elip(mangDinh, x, yDau, 8.f, 8.5f, kDa, 12);
        // Nón trụ nhọn bọc đồng
        tam(mangDinh, {x - 10.f, yDau - 4.f}, {x + 10.f, yDau - 4.f}, {x, yDau - 16.f}, kDong);
        chuNhat(mangDinh, x - 11.f, yDau - 4.f, 22.f, 2.5f, mau(140, 100, 35));

        // Ngọn trường thương sừng sững bên cạnh
        float xGiao = x + 14.f;
        duong(mangDinh, {xGiao, y - 36.f}, {xGiao, y + 16.f}, 2.8f, mau(110, 70, 40)); // Cán gỗ lim
        // Mũi thép hình lá nhọn hoắt
        tam(mangDinh, {xGiao - 3.5f, y - 36.f}, {xGiao + 3.5f, y - 36.f}, {xGiao, y - 50.f}, mau(235, 245, 255));
        // Dải tua rua đỏ bay dưới mũi thương
        float ruaBay = std::sin(t * 4.f) * 3.f;
        tu(mangDinh,
           {xGiao - 3.f, y - 36.f}, {xGiao + 3.f, y - 36.f},
           {xGiao + 5.f + ruaBay, y - 26.f}, {xGiao - 1.f + ruaBay, y - 26.f},
           mau(215, 30, 30), mau(215, 30, 30), mau(160, 20, 20), mau(160, 20, 20));

    } else {
        // =========================================================================
        //  BÁC THỢ ĐẼO CỌC (DÂN LÀNG) - MỘC MẠC, CHÂN CHẤT
        // =========================================================================
        Color kAoNau = mau(125, 78, 44);
        Color kAoNauToi = mau(85, 52, 28);
        Color kDa = mau(235, 195, 160);

        // Chân quấn xà cạp mộc
        chuNhatDoc(mangDinh, x - 6.f, y + 9.f, 5.f, 6.f, mau(60, 50, 40), mau(40, 30, 25));
        chuNhatDoc(mangDinh, x + 1.f, y + 9.f, 5.f, 6.f, mau(60, 50, 40), mau(40, 30, 25));

        // Áo nâu sồng xắn tay
        tu(mangDinh,
           {x - 9.f, y - 9.f}, {x + 9.f, y - 9.f},
           {x + 10.f, y + 9.f}, {x - 10.f, y + 9.f},
           kAoNau, kAoNau, kAoNauToi, kAoNauToi);

        // Đầu & Khăn nâu buộc đầu
        float yDau = y - 18.f;
        elip(mangDinh, x, yDau, 8.f, 8.5f, kDa, 12);
        chuNhat(mangDinh, x - 8.5f, yDau - 8.f, 17.f, 4.5f, mau(70, 50, 36));

        // Chiếc rìu đẽo cọc cầm trên tay
        float xRiu = x + 12.f;
        duong(mangDinh, {xRiu, y - 16.f}, {xRiu - 2.f, y + 10.f}, 2.5f, mau(130, 85, 50));
        // Lưỡi rìu sắt vát cong
        tu(mangDinh,
           {xRiu - 1.f, y - 16.f}, {xRiu + 8.f, y - 20.f},
           {xRiu + 9.f, y - 11.f}, {xRiu - 1.f, y - 11.f},
           mau(200, 210, 220), mau(235, 245, 255), mau(150, 160, 170), mau(150, 160, 170));

        // Mảnh phoi bào gỗ rơi rải rác dưới đất
        for (int i = 0; i < 4; ++i) {
            float fx = x - 14.f + static_cast<float>(i * 8);
            float fy = y + 12.f + static_cast<float>((i % 2) * 3);
            elip(mangDinh, fx, fy, 2.8f, 1.2f, mau(175, 125, 75), 6);
        }
    }

    window.draw(mangDinh);
}


