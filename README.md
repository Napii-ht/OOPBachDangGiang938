# OOP Bạch Đằng Giang 938

Dự án trò chơi lịch sử tái hiện chiến thắng Bạch Đằng năm 938 của Ngô Quyền, xây dựng bằng C++ và thư viện đồ họa SFML.

## Cấu trúc dự án
- `AmThanh/`: Quản lý hiệu ứng âm thanh và nhạc nền.
- `BanDo/`: Quản lý bản đồ sông nước, bãi cọc và các đối tượng địa hình.
- `CanhPhim/`: Bộ giải mã và trình chiếu video cắt cảnh chiến thắng (`pl_mpeg`).
- `ChienDau/`: Hệ thống chiến đấu cận chiến, bắn cung, hỏa công và trí tuệ nhân tạo (AI).
- `CotLoi/`: Vòng lặp trò chơi chính, quản lý màn chơi, lưu trữ trạng thái.
- `GiaoDien/`: Giao diện người dùng, trình đơn chính, màn hình chọn chương, màn hình kết thúc.
- `ManChoi/`: Các màn chơi theo chương (Chương 1 đến Chương 6 và màn Bạch Đằng lịch sử).
- `NhanVat/`: Quản lý nhân vật người chơi, đồng minh và kẻ địch (kể cả chủ tướng Lưu Hoằng Tháo).
- `NhiemVu/`: Hệ thống theo dõi mục tiêu và nhiệm vụ.
- `TaiNguyen/`: Hình ảnh, phông chữ, âm thanh và video tài nguyên.
- `VatPham/`: Các vật phẩm tương tác trong trò chơi.

## Hướng dẫn xây dựng (Build)
Yêu cầu:
- Trình biên dịch C++ hỗ trợ C++17 trở lên (MSVC, GCC hoặc Clang).
- CMake 3.16 trở lên.
- Thư viện SFML 2.5+ (hoặc SFML 3.0 tùy theo cấu hình).

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```
