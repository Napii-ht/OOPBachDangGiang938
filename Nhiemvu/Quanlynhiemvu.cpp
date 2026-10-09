#include "Quanlynhiemvu.h"
#include"Danhsachnhiemvu.h"
Quanlynhiemvu::Quanlynhiemvu() :ptuhoihientai(0)
{
	dsNhiemvu = KhoNhiemvu::KhoitaodsNhiemvu();
}
NhiemVu* Quanlynhiemvu::Getnhiemvuhientai()
{
	if (ptuhoihientai < dsNhiemvu.size())
	{
		return&dsNhiemvu[ptuhoihientai];
	}
	return nullptr;
}
Hoigame Quanlynhiemvu::Gethoihientai() const
{
	if (ptuhoihientai < 5)
		return static_cast<Hoigame>(ptuhoihientai);
	return Hoigame::HoanThanh;
}
string Quanlynhiemvu::Gettieudehoihientai()const
{
	switch (Gethoihientai())
	{
	case Hoigame::Hoi1_Doanhtrai:
		return "Hồi 1: Nhận lệnh";
	case Hoigame::Hoi2_Trongrung:
		return "Hồi 2: Chuẩn bị";
	case Hoigame::Hoi3_Duoisong:
		return "Hồi 3: Bãi cọc ngầm";
	case Hoigame::Hoi4_Cuabien:
		return "Hồi 4: Dụ binh";
	case Hoigame::Hoi5_Trensong:
		return "Hồi 5: Tổng phản công";
	default:
		return "Chiến thẳng";
	}
}
void Quanlynhiemvu::Tangtiendo(int luong)
{
	NhiemVu* nv = Getnhiemvuhientai();
	if (nv)
	{
		nv->Capnhattiendo(luong);
		Kiemtrachuyenhoi();
	}
}
bool Quanlynhiemvu::Kiemtrachuyenhoi()
{
	NhiemVu* nv = Getnhiemvuhientai();
	if (nv && nv->HoanThanh())
	{
		if (ptuhoihientai + 1 < dsNhiemvu.size())
		{
			ptuhoihientai++;
			return true;
		}
	}
	return false;
}
bool Quanlynhiemvu::Hoanthanhgame()const
{
	return ptuhoihientai >= dsNhiemvu.size();
}
string Quanlynhiemvu::GetthongtinUI()const
{
	if (ptuhoihientai < dsNhiemvu.size())
	{
		return dsNhiemvu[ptuhoihientai].GetchuoihienthiUI();
	}
	return "Đã hoàn thành tất cả nhiệm vụ";
}