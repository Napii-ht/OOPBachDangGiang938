#pragma once
#include <vector>
#include <string>
#include "NhiemVu.h"
enum class Hoigame
{
	Hoi1_Doanhtrai = 0,
	Hoi2_Trongrung = 1,
	Hoi3_Duoisong = 2,
	Hoi4_Cuabien = 3,
	Hoi5_Trensong = 4,
	HoanThanh = 5
};
class Quanlynhiemvu
{
private:
	vector<NhiemVu> dsNhiemvu;
	size_t ptuhoihientai;
public:
	Quanlynhiemvu();
	NhiemVu* Getnhiemvuhientai();
	Hoigame Gethoihientai() const;
	string Gettieudehoihientai() const;
	void Tangtiendo(int luong = 1);
	bool Kiemtrachuyenhoi();
	bool Hoanthanhgame() const;
	string GetthongtinUI()const;
};
