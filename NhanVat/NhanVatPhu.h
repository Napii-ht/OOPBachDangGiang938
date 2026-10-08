#pragma once
#include "NhanVat.h"
#include "LoiThoai.h"
#include<string>
class NhanVatPhu :public NhanVat
{
protected:
	string ten;
	LoiThoai loithoai;
	float Phamvituongtac;
	bool Dangtuongtac;
public:
	NhanVatPhu(float x, float y, string& tenNPC, float Phamvi = 80.0f);
	virtual ~NhanVatPhu() = default;
	void CapNhat(float dt) override;
	virtual bool TuongTac(const Vector2f& ViTriNguoiChoi);
	virtual void NoiChuyen();
	virtual void HienThiLoiThoai(RenderWindow& cuaso, Font& font);
	string Getten()const { return ten; }
	bool Isdangtuongtac() { return Dangtuongtac; }
	void CapNhapcauthoaitheoflow(const vector<string>& caumoi);
};