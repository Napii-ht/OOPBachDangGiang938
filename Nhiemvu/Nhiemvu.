#pragma once
#include <vector>
#include <string>
using namespace std;
class NhiemVu
{
private:
	string tenNhiemvu;
	string mota;
	bool dahoanthanh;
	int Tiendo;
	int Muctieu;

	string Tennguoigiao;
	vector<string>dsLoithoai;
public: 
	NhiemVu(string ten, string motachitiet, string npc, int muctieuyeucau, vector<string> thoai): tenNhiemvu(ten), mota(motachitiet), Tennguoigiao(npc), dahoanthanh(false), Tiendo(0), Muctieu(muctieuyeucau), dsLoithoai(thoai) {}
	void Capnhattiendo(int luong = 1)
	{
		if (dahoanthanh) return;
		Tiendo += luong;
		if (Tiendo >= Muctieu)
		{
			Tiendo = Muctieu;
			dahoanthanh = true;
		}
	}
	bool HoanThanh() const { return dahoanthanh; }
	string Gettennhiemvu() const { return tenNhiemvu; }
	string Getmota() const { return mota; }
	string Gettennguoigiao() const { return Tennguoigiao; }
	int Gettiendo() const { return Tiendo; }
	int Getmuctieu() const { return Muctieu; }
	const vector<string>& Getdsloithoai() const { return dsLoithoai; }
	string GetchuoihienthiUI() const { return tenNhiemvu + ": " + to_string(Tiendo) + "/ " + to_string(Muctieu); }
};
