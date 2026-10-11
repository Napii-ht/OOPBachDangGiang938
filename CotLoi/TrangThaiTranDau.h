#pragma once

// Các giai đoạn của trận Bạch Đằng theo dòng thời gian trong PDF phân công.
// Dùng chung cho những chương có diễn biến trận đánh.
enum class TrangThaiTranDau {
    CHUAN_BI,              // Gặp Ngô Quyền, thu thập và cắm cọc gỗ
    QUAN_DICH_XUAT_HIEN,   // Thuyền giặc Nam Hán tiến vào cửa sông
    NHU_DICH,              // Lên thuyền nhẹ nhử giặc vào bãi cọc
    THUY_TRIEU_XUONG,      // Thủy triều bắt đầu rút mạnh
    TAU_MAC_COC,           // Thuyền giặc đâm trúng cọc, mắc cạn
    PHAN_CONG,             // Quân Đại Việt đổ ra tổng phản công
    CHIEN_THANG            // Toàn thắng lịch sử
};
