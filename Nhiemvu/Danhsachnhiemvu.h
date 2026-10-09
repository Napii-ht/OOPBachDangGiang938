#pragma once
#include <vector>
#include "NhiemVu.h"
using namespace std;
class KhoNhiemvu
{
public:
	static vector<NhiemVu>KhoitaodsNhiemvu()
	{
		vector<NhiemVu>ds;
		ds.push_back(NhiemVu
		("Nhận Binh khí chiến đấu",
			"Gặp Ngô Quyền để nhận lệnh ",
			"Ngô Quyền",
			1, // Mục tiêu: 1 lần nhận
			{
				"Lưu Hoằng Tháo dẫn đầu quân Nam Hán sắp tràn vào Tĩnh Hải Quân",
				"Ngươi mau nhận binh khí để sẵn sàng chiến đấu.",
				"Nhận xong binh khí, hãy mau ra bên sông giúp bà con dân làng"
			}
			)
		);
		ds.push_back(NhiemVu
		(
			"Đánh tàn quân và chế tác cọc gỗ",
			"Tiêu diệt tàn quân Kiều Công Tiễn đang cố gắng chống phá trong rừng, chặt 3 cây gỗ lớn va thu gom quặng sắt để bịt đầu cọc.",
			"Dân làng",
			5,
			{
				"Nguy rồi! Tàn quân của Kiều Công Tiễn vẫn còn lẫn khuất trong rừng để chống phá",
				"Khiến bà con không thể vào rừng chặt gỗ và thu nhặt quặng sắt",
				"Đầu tiên anh hãy dùng binh khí để tiêu diệt tàn quân Kiều Công Tiễn đang phục kich",
				"Sau đó hãy thu gom 3 thân gỗ lớn và quặng sắt để mài nhọn,rồi bịt sắt phần đã vót nhọn ở mỗi cây cọc trước khi triều lên "
			}
		));
		ds.push_back(NhiemVu
		(
			"Cắm cọc xuống sông Bạch Đằng",
			"Dùng dây thừng cố định và đóng 3 cọc gỗ bịt sắt xuống sông",
			"Dân làng",
			3,
			{
				"Tan quan da bi quet sach, coc go bit sat da gia cong xong xuoi!",
				"BAY GIO: Hay dung DAY THỪNG keo va DÓNG CHẶT 3 CỌC nay xuong long song Bach Dang!",
				"Phai xong xuoi truoc khi thuy trieu dang cao che khuat bai coc!"
			}
		));
		ds.push_back(NhiemVu
		(
			"Nhử giặc và né chướng ngại vật",
			"Chèo thuyền nhỏ ra cửa biển vừa đánh vừa lùi, khéo léo né mưa tên và đá ngầm trên sông.",
			"Ngô Quyền",
			1,
			{
				"Thủy triều dâng cao đã che khuất bãi cọc ngầm ",
				"Ngươi hãy mau lên thuyền ra cửa biển lừa giặc tiến sâu vào lòng sông!",
				"Cẩn thận:  Quân giặc trên sông sẽ bắn tên, dưới sông có đá ngầm. Hãy chèo thuyến khéo léo"
			}
		));
		ds.push_back(NhiemVu
		(
			"Tổng phản công ",
			"Thủy triều rút, thuyền giặc va vào bãi cọc ngầm. Dùng cung tên hạ quân giặc.",
			"Ngô Quyền",
			5,
			{
				"Thủy triều đã rút! Thuyền giặc va vào bãi cọc ngầm và trở nên hoảng loạn!",
				"Ngươi hãy dùng cung tên tiêu diệt toàn bộ quân Nam Hán!"
				"TOÀN QUÂN... TỔNG PHẢN CÔNG!",
			}
		));
		return ds;
	}
};