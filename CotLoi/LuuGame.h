#pragma once

#include <string>

// Dữ liệu được lưu giữa các lần chơi (file luu_game.txt cạnh file chạy game)
struct DuLieuLuu {
    int chuongDaMo = 1;        // Chương cao nhất đã mở khóa (1 đến 6)
    int chuongHoanThanh = 0;   // Chương cao nhất đã hoàn thành
    int amLuong = 80;          // Âm lượng chung 0 đến 100
    bool toanManHinh = false;  // Có bật toàn màn hình không
};

class LuuGame {
public:
    // Trả về true nếu đọc được file. Nếu không có file thì giữ nguyên giá trị mặc định.
    static bool Tai(DuLieuLuu& duLieu, const std::string& tenFile = "luu_game.txt");
    static bool Luu(const DuLieuLuu& duLieu, const std::string& tenFile = "luu_game.txt");

    // Ghi nhận hoàn thành một chương: mở khóa chương kế tiếp
    static void GhiNhanHoanThanh(DuLieuLuu& duLieu, int maChuong, int soChuongToiDa = 6);
};
