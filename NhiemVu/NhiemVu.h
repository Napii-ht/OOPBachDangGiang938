#pragma once

#include <string>
#include <SFML/Graphics.hpp>

enum class LoaiNhiemVu {
    KHAM_PHA,   // Di chuyển, gặp gỡ NPC (Ngô Quyền)
    THU_THAP,   // Thu thập cọc gỗ, tài nguyên
    XAY_DUNG,   // Cắm cọc ngầm tạo trận địa
    CHIEN_DAU   // Nhử địch, tổng phản công
};

class NhiemVu {
protected:
    int maNhiemVu;
    std::string tenNhiemVu;
    std::string moTa;
    LoaiNhiemVu loai;
    bool daKichHoat;
    bool daHoanThanh;
    int tienDo;
    int mucTieu;

public:
    NhiemVu(int id, const std::string& ten, const std::string& desc, LoaiNhiemVu loaiNv, int goal = 1);
    virtual ~NhiemVu() = default;

    virtual void CapNhatTienDo(int luong = 1);
    virtual bool KiemTraHoanThanh();

    int GetMaNhiemVu() const { return maNhiemVu; }
    std::string GetTenNhiemVu() const { return tenNhiemVu; }
    std::string GetMoTa() const { return moTa; }
    LoaiNhiemVu GetLoai() const { return loai; }
    bool IsDaKichHoat() const { return daKichHoat; }
    void SetDaKichHoat(bool val) { daKichHoat = val; }
    bool IsDaHoanThanh() const { return daHoanThanh; }
    int GetTienDo() const { return tienDo; }
    int GetMucTieu() const { return mucTieu; }

    std::string GetTienDoChu() const;
};

// Nhiệm vụ thu thập tài nguyên (Cọc gỗ)
class NhiemVuThuThap : public NhiemVu {
private:
    int maVatPhamMucTieu;

public:
    NhiemVuThuThap(int id, const std::string& ten, const std::string& desc, int maVatPham, int soLuong = 3);
    virtual ~NhiemVuThuThap() = default;
};

// Nhiệm vụ khám phá, đối thoại
class NhiemVuKhamPha : public NhiemVu {
public:
    NhiemVuKhamPha(int id, const std::string& ten, const std::string& desc);
    virtual ~NhiemVuKhamPha() = default;
};

// Nhiệm vụ chiến đấu tiêu diệt giặc
class NhiemVuChienDau : public NhiemVu {
public:
    NhiemVuChienDau(int id, const std::string& ten, const std::string& desc, int soQuanGiac = 5);
    virtual ~NhiemVuChienDau() = default;
};


