#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// Kích thước màn hình logic của game. Mọi thứ trên giao diện đều vẽ theo hệ tọa độ này,
// cửa sổ có thể phóng to thu nhỏ nhưng tỉ lệ luôn giữ 4:3 (có dải đen hai bên nếu cần).
constexpr float RONG_MAN_HINH = 800.f;
constexpr float CAO_MAN_HINH = 600.f;

// Đổi chuỗi UTF-8 (có dấu tiếng Việt) sang sf::String để sf::Text hiển thị đúng
sf::String VanBan(const std::string& utf8);

// Tìm đường dẫn tới file trong thư mục TaiNguyen.
// Ví dụ DuongDanTaiNguyen("PhongChu/arial.ttf") trả về "TaiNguyen/PhongChu/arial.ttf"
// (tự thử thêm ../ và ../../ phòng khi chạy từ thư mục build khác)
std::string DuongDanTaiNguyen(const std::string& tuongDoi);

// Nạp font mặc định của game (arialbd, arial rồi tới font của Windows)
bool TaiPhongChuMacDinh(sf::Font& font);

// View logic 800x600 giữ tỉ lệ, dùng để vẽ giao diện và đổi tọa độ chuột
sf::View TaoViewLogic(const sf::RenderWindow& window);
void ApDungViewLogic(sf::RenderWindow& window);
sf::Vector2f ToaDoChuot(const sf::RenderWindow& window, sf::Vector2i pixel);

// Vẽ nhanh một dòng chữ căn giữa tại viTri
void VeChuGiua(sf::RenderWindow& window, const sf::Font& font, const std::string& noiDung,
               unsigned int coChu, sf::Vector2f viTri, sf::Color mau,
               float doDayVien = 0.f, sf::Color mauVien = sf::Color::Black);

// Vẽ nhanh một đoạn chữ canh trái (xuống dòng bằng \n)
void VeChuTrai(sf::RenderWindow& window, const sf::Font& font, const std::string& noiDung,
               unsigned int coChu, sf::Vector2f viTri, sf::Color mau);

// Tự xuống dòng cho đoạn văn UTF-8 sao cho mỗi dòng không rộng quá rongToiDa (pixel)
std::string NgatDong(const sf::Font& font, const std::string& utf8, unsigned int coChu, float rongToiDa);
