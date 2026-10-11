#include "NhiemVu.h"

NhiemVu::NhiemVu(int id, const std::string& ten, const std::string& desc, LoaiNhiemVu loaiNv, int goal)
    : maNhiemVu(id),
      tenNhiemVu(ten),
      moTa(desc),
      loai(loaiNv),
      daKichHoat(false),
      daHoanThanh(false),
      tienDo(0),
      mucTieu(goal) {}

void NhiemVu::CapNhatTienDo(int luong) {
    tienDo += luong;
    if (tienDo >= mucTieu) {
        tienDo = mucTieu;
        daHoanThanh = true;
    }
}

bool NhiemVu::KiemTraHoanThanh() {
    if (tienDo >= mucTieu) {
        daHoanThanh = true;
    }
    return daHoanThanh;
}

std::string NhiemVu::GetTienDoChu() const {
    if (daHoanThanh) return "[ Hoàn thành ]";
    return std::to_string(tienDo) + "/" + std::to_string(mucTieu);
}

// ----------------- DERIVED QUEST CLASSES -----------------

NhiemVuThuThap::NhiemVuThuThap(int id, const std::string& ten, const std::string& desc, int maVatPham, int soLuong)
    : NhiemVu(id, ten, desc, LoaiNhiemVu::THU_THAP, soLuong),
      maVatPhamMucTieu(maVatPham) {}

NhiemVuKhamPha::NhiemVuKhamPha(int id, const std::string& ten, const std::string& desc)
    : NhiemVu(id, ten, desc, LoaiNhiemVu::KHAM_PHA, 1) {}

NhiemVuChienDau::NhiemVuChienDau(int id, const std::string& ten, const std::string& desc, int soQuanGiac)
    : NhiemVu(id, ten, desc, LoaiNhiemVu::CHIEN_DAU, soQuanGiac) {}


