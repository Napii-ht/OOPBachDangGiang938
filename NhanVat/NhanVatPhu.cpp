#include"NhanVatPhu.h"
#include <cmath>
NhanVatPhu::NhanVatPhu(float x, float y, string& tenNPC, float phamvi) :NhanVat(x, y, 0.0f, 100, 0), ten(tenNPC), Phamvituongtac(phamvi), Dangtuongtac(false)
{
	hinhDang.setFillColor(Color::Green);
}
void NhanVatPhu::CapNhat(float dt){}
//Tuongtac
bool NhanVatPhu::TuongTac(const Vector2f& ViTriNguoiChoi)
{
	float dx = viTri.x - ViTriNguoiChoi.x;
	float dy = viTri.y - ViTriNguoiChoi.y;
	float khoangcach = sqrt(dx*dx + dy * dy);
	if (khoangcach <= Phamvituongtac)
	{
		Dangtuongtac = true;
		return true;
	}
	else
	{
		Dangtuongtac = false;
		loithoai.Reset();
		return false;
	}
}
//Cau thoai tiep theo khi noi chuyem
void NhanVatPhu::NoiChuyen()
{
	if (Dangtuongtac)
		loithoai.Chuyencau();
}
//Hien thi loi thoai
void NhanVatPhu::HienThiLoiThoai(RenderWindow& cuaso, Font& font)
{
	if (!Dangtuongtac || loithoai.Hetloithoai()) return;
	RectangleShape khung(Vector2f{ 880.0f, 130.0f });
	khung.setFillColor(Color(15, 20, 30, 230));
	khung.setOutlineColor(Color(212, 175, 55));
	khung.setOutlineThickness(2.0f);
	khung.setPosition({ 200.0f, 560.0f });

	Text txtTen(font, String::fromUtf8(ten.begin(), ten.end()), 22);
	txtTen.setFillColor(Color::Yellow);
	txtTen.setPosition({ 220.0f, 570.0f });

	string cau = loithoai.Getcauhientai();
	Text txtNoidung(font, String::fromUtf8(cau.begin(), cau.end()), 17);
	txtNoidung.setFillColor(Color::Black);
	txtNoidung.setPosition({ 220.0f, 610.0f });

	cuaso.draw(khung);
	cuaso.draw(txtTen);
	cuaso.draw(txtNoidung);
}

void NhanVatPhu::CapNhapcauthoaitheoflow(const  vector<string>& caumoi)
{
	loithoai.Capnhatdanhsach(caumoi);
}
