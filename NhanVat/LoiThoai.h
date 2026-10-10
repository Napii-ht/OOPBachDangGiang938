#pragma once
#include <iostream>
#include <vector>
#include "NhanVat.h"
using namespace std;
using namespace sf;
class LoiThoai
{
private:
	string tenNguoinoi;
	vector<string> danhsachcau;
	size_t ptuhientai;
public:
	LoiThoai(string ten="", vector<string> cau={}) :tenNguoinoi(ten), danhsachcau(cau), ptuhientai(0){}
	string Gettennguoinoi(){ return tenNguoinoi; }
	string Getcauhientai()
	{
		if (ptuhientai < danhsachcau.size())
		{
			return danhsachcau[ptuhientai];
		}
		return "";
	}
	bool Chuyencau()
	{
		if (ptuhientai + danhsachcau.size())
		{
			ptuhientai++;
			return true;
		}
		return false;
	}
	void Reset() { ptuhientai = 0; }
	bool Hetloithoai() { return ptuhientai >= danhsachcau.size(); }
	void Capnhatdanhsach(const vector<string>& caumoi)
	{
		danhsachcau = caumoi;
		ptuhientai = 0;
	}
};
