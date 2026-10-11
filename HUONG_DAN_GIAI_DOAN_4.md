# Bạch Đằng 938: Giai đoạn 4 (chương 6, màn kết, hoàn thiện menu)

Gói này là toàn bộ project (giai đoạn 0 đến 4 gộp lại), đủ 6 chương. Chép đè vào project rồi Include các file mới (danh sách ở cuối).
Cài đặt lần đầu vẫn cần: cờ `/utf-8`, Working Directory là `$(ProjectDir)`, có thư mục `TaiNguyen/`.

## Chương 6: Tổng phản công
Lòng sông đã cạn: bãi bùn lộ ra, 32 cọc nhô cao, 4 thuyền giặc mắc kẹt giữa bãi cọc (soái hạm ở giữa). Người chơi đi bộ và cận chiến.

1. Đi ra điểm tập kết giữa bãi bùn (có mũi tên chỉ hướng). Trống nổi lên, quân Đại Việt từ hai bờ đổ xuống.
2. Đợt 1: 6 lính Nam Hán nhảy xuống từ các thuyền. Quân ta yểm trợ nhưng đánh yếu.
3. Nghỉ 3 giây, hồi 60 máu. Đợt 2: 8 lính, nhanh và mạnh hơn.
4. Nghỉ, hồi 60 máu. Đợt 3: chủ tướng Lưu Hoằng Tháo (280 máu, có thanh máu riêng) cùng 4 vệ binh. Quân ta không đánh được chủ tướng, phải do người chơi hạ. Chủ tướng ngã thì cảnh chậm lại, lính còn lại tan rã.
5. Hiện thẻ lịch sử cuối chương.

Cọc và thân thuyền là vật cản cho cả người chơi, địch và quân ta. Chết thì về chọn chương để chơi lại.

## Màn kết và credits
Thắng chương 6 thì vào màn kết: 3 trang lời bạt lịch sử (tự chuyển trang, hoặc Enter), rồi danh sách nhóm thực hiện cuộn lên theo bảng phân công trong PDF. Esc để về menu bất cứ lúc nào.

## Hoàn thiện menu
- Màn chọn chương ghi "(đã xong)" cho chương đã hoàn thành và tự trỏ vào chương đang chơi dở.
- Hướng dẫn điều khiển trong menu cập nhật đủ cho chèo thuyền, khiêu khích (Q) và phát hiệu lệnh (Space).
- Bản chơi cũ đổi tên thành "Màn thử nghiệm (bản cũ)", giữ lại để tham khảo.

## Thay đổi ở phần lõi
- `ManChoi/ManChuong6`: kế thừa `ManChoiSong` nhưng ở chế độ đi bộ. Dùng lại `HeThongChienDau`, `KeDich`, thanh máu chủ tướng, `NhanVatPhu` (quân ta).
- `GiaoDien/ManHinhKetThuc`: lời bạt và credits.
- `QuanLyGame`: thêm trạng thái `KET_THUC`.

## Chú ý khi kiểm tra
- Các con số (6, 8 và 4 địch, máu, sát thương, hồi 60 máu) là thiết kế để dễ chơi, không phải số liệu lịch sử. Chỉnh ở các hằng số đầu `ManChuong6.h` và `ManChuong6.cpp`.
- Bot tự động đã thắng chương 6 và một ca đứng yên chịu đòn thì thua đúng. Cảm giác chơi thật cần nhóm thử bằng bàn phím.
- Thẻ lịch sử và lời bạt soạn theo sử liệu phổ thông, nhóm nên đối chiếu sách giáo khoa trước khi nộp.
- Âm thanh vẫn là tiếng tổng hợp, chưa có nhạc và lời đọc thật.

## File mới
`ManChoi/ManChuong6.h/.cpp`, `GiaoDien/ManHinhKetThuc.h/.cpp`.

## File đã sửa
`CotLoi/QuanLyGame.h/.cpp`, `GiaoDien/ManHinhChonChuong.h/.cpp`, `GiaoDien/MenuChinh.cpp`.

## Cập nhật: làm đẹp nhân vật, kiếm và chuỗi chém
Chỉ sửa `NhanVat/NguoiChoi.h` và `NhanVat/NguoiChoi.cpp`.

- Nhân vật: thêm cánh tay có khuỷu và bao cổ tay, giáp vai và giáp ngực bằng đồng, cổ áo, áo choàng đỏ bay theo bước chạy, giáp ống chân, ủng da, tai, má hồng, ánh mắt, miệng. Bước chân đã có nhấc chân và đạp chân thật (trước đây hai chân gần như đứng yên).
- Kiếm: vẽ lại thành thanh kiếm đầy đủ (tua đỏ, chuôi quấn dây có vòng vàng, tay hộ vàng, lưỡi thép có viền, nửa sáng nửa tối và sống giữa). Khi chưa rút thì tra trong vỏ đeo hông, chuôi nhô lên phía trước, tay đặt lên chuôi.
- Chuỗi chém chạy theo thời gian: lấy đà, chém nhanh, thu kiếm (0,27 giây). Thân người chồm tới theo nhát chém, tay sau giơ ra giữ thăng bằng, áo choàng và dải khăn bay ra sau. Vệt chém hình lưỡi liềm bám theo mũi kiếm, có dư ảnh lưỡi kiếm và ánh lóe ở mũi.
- Chém liên tiếp thì đổi qua lại giữa chém từ trên xuống và hất từ dưới lên. Sau nhát chém kiếm vẫn cầm trên tay 1 giây rồi tự tra vào vỏ.
- Đổi cách chơi một chút: vùng chém chỉ trúng đúng lúc lưỡi kiếm vung qua (khớp với hình vẽ, không còn trúng ngay lúc đang giơ kiếm), tầm chém từ 74 xuống 66 điểm ảnh cho khớp với hình kiếm. Nếu thấy chém hơi khó trúng, tăng `phamViTanCong` hoặc hệ số `1.2f` trong `GetVungTanCong()`.
- Kiểm tra lại chương 1 (luyện bù nhìn), chương 2 (đánh thám tử) và chương 6 (ba đợt địch cùng chủ tướng) bằng bot, đều vẫn thắng.
