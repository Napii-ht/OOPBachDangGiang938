#include "KeDich.h"
#include <cmath>
#include <algorithm>

KeDich::KeDich(sf::Vector2f viTriBanDau, float tocDo, int mau, int dame, float tamDanh, float tamPhatHien)
    : NhanVat(viTriBanDau, tocDo, mau, dame),
      tamDanh(tamDanh),
      tamPhatHien(tamPhatHien),
      trangThaiHienTai(TrangThaiKeDich::DUNG_YEN),
      thoiGianHoiChieu(1.0f),      // Mặc định 1 giây mới chém 1 phát
      thoiGianTroiQua(1.0f) {      // Vào trận là sẵn sàng chém luôn
    
    // Tạo hình tròn đỏ đại diện cho lính Nam Hán
    hinhDang.setRadius(16.f);
    hinhDang.setOrigin({16.f, 16.f});
    hinhDang.setFillColor(sf::Color(220, 50, 40));
    hinhDang.setOutlineThickness(2.f);
    hinhDang.setOutlineColor(sf::Color(120, 20, 20));
    hinhDang.setPosition(viTri);

    // Thanh máu đen làm nền nằm trên đầu
    thanhMauNen.setSize({32.f, 5.f});
    thanhMauNen.setFillColor(sf::Color(50, 50, 50, 180));
    thanhMauNen.setOutlineThickness(1.f);
    thanhMauNen.setOutlineColor(sf::Color::Black);

    // Vạch máu đỏ thể hiện lượng máu còn lại
    thanhMauHienTai.setSize({32.f, 5.f});
    thanhMauHienTai.setFillColor(sf::Color::Red);
}

bool KeDich::PhatHienNguoiChoi(sf::Vector2f viTriPlayer) const {
    if (daHySinh) return false;
    // Tính khoảng cách xem player có đi vào vùng quan sát không
    float dx = viTriPlayer.x - viTri.x;
    float dy = viTriPlayer.y - viTri.y;
    return (dx * dx + dy * dy) <= (tamPhatHien * tamPhatHien);
}

void KeDich::DuoiTheo(sf::Vector2f viTriPlayer, float deltaTime) {
    if (daHySinh) return;

    float dx = viTriPlayer.x - viTri.x;
    float dy = viTriPlayer.y - viTri.y;
    float khoangCach = std::sqrt(dx * dx + dy * dy);

    // Nếu đã đứng sát mặt rồi thì dừng lại chém, không chạy nữa
    if (khoangCach <= tamDanh) {
        trangThaiHienTai = TrangThaiKeDich::TAN_CONG;
        return;
    }

    // Chuẩn hóa vector hướng: chia cho khoảng cách để độ dài = 1
    // Nhờ đó tốc độ chạy của lính luôn đều đặn, không bị giật hay bay quá nhanh
    if (khoangCach > 0.0001f) {
        sf::Vector2f huong(dx / khoangCach, dy / khoangCach);
        DiChuyen(huong, deltaTime);
        trangThaiHienTai = TrangThaiKeDich::DUOI_THEO;
    }
}

bool KeDich::CoTheTanCong(sf::Vector2f viTriPlayer) const {
    if (daHySinh) return false;
    float dx = viTriPlayer.x - viTri.x;
    float dy = viTriPlayer.y - viTri.y;
    float khoangCach = std::sqrt(dx * dx + dy * dy);
    // Vừa phải đứng trong tầm đánh, vừa phải hết thời gian chờ giữa 2 đòn
    return (khoangCach <= tamDanh) && (thoiGianTroiQua >= thoiGianHoiChieu);
}

bool KeDich::TanCong(NhanVat& mucTieu) {
    if (daHySinh) return false;

    if (thoiGianTroiQua >= thoiGianHoiChieu) {
        mucTieu.NhanSatThuong(satThuong);
        thoiGianTroiQua = 0.0f; // Chém xong phải chờ hồi chiêu
        trangThaiHienTai = TrangThaiKeDich::TAN_CONG;
        return true;
    }
    return false;
}

void KeDich::NhanSatThuong(int damage) {
    NhanVat::NhanSatThuong(damage);
    if (sucKhoe <= 0) {
        Chet();
    } else {
        trangThaiHienTai = TrangThaiKeDich::BI_THUONG;
    }
}

void KeDich::Chet() {
    HySinh(); // Báo cho class cha biết là đã chết
    trangThaiHienTai = TrangThaiKeDich::DA_CHET;
}

void KeDich::CapNhat(float deltaTime) {
    if (daHySinh) return;

    // Tính giờ hồi chiêu đòn đánh
    thoiGianTroiQua += deltaTime;

    // Cho hình tròn chạy theo tọa độ viTri mới
    hinhDang.setPosition(viTri);

    // Tính chiều dài vạch máu theo % máu còn lại
    float tiLeMau = static_cast<float>(sucKhoe) / static_cast<float>(sucKhoeToiDa);
    tiLeMau = std::clamp(tiLeMau, 0.0f, 1.0f);
    thanhMauHienTai.setSize({32.f * tiLeMau, 5.f});

    // Neo thanh máu nằm cao hơn đầu con lính 26 pixel
    sf::Vector2f viTriThanhMau(viTri.x - 16.f, viTri.y - 26.f);
    thanhMauNen.setPosition(viTriThanhMau);
    thanhMauHienTai.setPosition(viTriThanhMau);
}

void KeDich::Ve(sf::RenderWindow& window) {
    if (daHySinh) return;

    // Vẽ con lính đỏ và cây máu của nó
    window.draw(hinhDang);
    window.draw(thanhMauNen);
    window.draw(thanhMauHienTai);
}
