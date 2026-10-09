#include "Thuyen.h"

#include <algorithm>
#include <cmath>
#include <iostream>

Thuyen::Thuyen(
    sf::Vector2f viTriBanDau,
    float tocDoBanDau,
    int mauToiDa,
    sf::Color mauThanhMau
)
    : sucKhoe(std::max(0, mauToiDa)),
      sucKhoeToiDa(std::max(0, mauToiDa)),
      tocDo(std::max(0.f, tocDoBanDau)),
      daBiPhaHuy(mauToiDa <= 0),
      mauThanhMau(mauThanhMau)
{
    const std::array<std::string, 8> duongDan = {
        "Assets/Objects/Boats8Directions/thuyen_01_tren_trai.png",
        "Assets/Objects/Boats8Directions/thuyen_02_tren_cheo.png",
        "Assets/Objects/Boats8Directions/thuyen_03_tren_ngang.png",
        "Assets/Objects/Boats8Directions/thuyen_04_tren_phai.png",
        "Assets/Objects/Boats8Directions/thuyen_05_duoi_trai.png",
        "Assets/Objects/Boats8Directions/thuyen_06_duoi_cheo.png",
        "Assets/Objects/Boats8Directions/thuyen_07_duoi_ngang.png",
        "Assets/Objects/Boats8Directions/thuyen_08_duoi_phai.png"
    };

    for (std::size_t i = 0; i < duongDan.size(); ++i)
    {
        textures[i] = std::make_unique<sf::Texture>();

        if (!textures[i]->loadFromFile(duongDan[i]))
        {
            std::cerr << "Khong the tai anh thuyen: "
                      << duongDan[i] << '\n';

            textures[i].reset();
            continue;
        }

        sprites[i] =
            std::make_unique<sf::Sprite>(*textures[i]);

        const auto kichThuoc = textures[i]->getSize();
        const float tiLeThuNho = 0.20f;

        // Căn tâm để đổi hướng không làm thuyền nhảy vị trí.
        sprites[i]->setOrigin({
            kichThuoc.x / 2.f,
            kichThuoc.y / 2.f
        });

        sprites[i]->setScale({
            tiLeThuNho,
            tiLeThuNho
        });

        sprites[i]->setPosition(viTriBanDau);
    }

    thanhMauNen.setSize({48.f, 6.f});
    thanhMauNen.setFillColor(sf::Color(35, 35, 35, 220));

    thanhMauHienTai.setSize({48.f, 6.f});
    thanhMauHienTai.setFillColor(sf::Color::Green);

    CapNhatThanhMau(mauThanhMau);
}

bool Thuyen::HopLe() const
{
    return sprites[huongHienTai] != nullptr;
}

void Thuyen::DiChuyen(
    const sf::Vector2f& huong,
    float deltaTime
)
{
    if (daBiPhaHuy || deltaTime <= 0.f)
        return;

    const float doDai = std::sqrt(
        huong.x * huong.x + huong.y * huong.y
    );

    if (doDai <= 0.0001f)
        return;

    // Xác định sprite theo hướng di chuyển.
    if (huong.x == 0.f && huong.y < 0.f)
        huongHienTai = 0;
    else if (huong.x > 0.f && huong.y < 0.f)
        huongHienTai = 1;
    else if (huong.x > 0.f && huong.y == 0.f)
        huongHienTai = 2;
    else if (huong.x < 0.f && huong.y < 0.f)
        huongHienTai = 3;
    else if (huong.x == 0.f && huong.y > 0.f)
        huongHienTai = 4;
    else if (huong.x < 0.f && huong.y > 0.f)
        huongHienTai = 5;
    else if (huong.x < 0.f && huong.y == 0.f)
        huongHienTai = 6;
    else if (huong.x > 0.f && huong.y > 0.f)
        huongHienTai = 7;

    if (!sprites[huongHienTai])
        return;

    const sf::Vector2f huongDonVi(
        huong.x / doDai,
        huong.y / doDai
    );

    sprites[huongHienTai]->move(
        huongDonVi * tocDo * deltaTime
    );

    // Đồng bộ vị trí của cả 8 sprite.
    const sf::Vector2f viTri =
        sprites[huongHienTai]->getPosition();

    for (auto& sprite : sprites)
    {
        if (sprite)
            sprite->setPosition(viTri);
    }

    CapNhatThanhMau(mauThanhMau);
}

void Thuyen::SetViTri(sf::Vector2f viTri)
{
    for (auto& sprite : sprites)
    {
        if (sprite)
            sprite->setPosition(viTri);
    }

    CapNhatThanhMau(mauThanhMau);
}

sf::Vector2f Thuyen::GetViTri() const
{
    if (!sprites[huongHienTai])
        return {0.f, 0.f};

    return sprites[huongHienTai]->getPosition();
}

sf::FloatRect Thuyen::GetHitBox() const
{
    if (!sprites[huongHienTai])
        return {};

    return sprites[huongHienTai]->getGlobalBounds();
}

void Thuyen::NhanSatThuong(int satThuong)
{
    if (daBiPhaHuy || satThuong <= 0)
        return;

    sucKhoe = std::max(0, sucKhoe - satThuong);

    if (sucKhoe == 0)
    {
        daBiPhaHuy = true;
        return;
    }

    CapNhatThanhMau(mauThanhMau);
}

bool Thuyen::DaBiPhaHuy() const
{
    return daBiPhaHuy;
}

int Thuyen::GetMau() const
{
    return sucKhoe;
}

int Thuyen::GetMauToiDa() const
{
    return sucKhoeToiDa;
}

void Thuyen::CapNhatThanhMau(sf::Color mauThanhMau)
{
    if (!sprites[huongHienTai])
        return;

    const auto bounds =
        sprites[huongHienTai]->getGlobalBounds();

    const float tiLeMau =
        sucKhoeToiDa > 0
            ? static_cast<float>(sucKhoe) / sucKhoeToiDa
            : 0.f;

    constexpr float chieuRongThanhMau = 48.f;
    constexpr float chieuCaoThanhMau = 6.f;

    const sf::Vector2f viTriThanhMau(
        bounds.position.x +
            (bounds.size.x - chieuRongThanhMau) / 2.f,
        bounds.position.y - 12.f
    );

    thanhMauNen.setSize({
        chieuRongThanhMau,
        chieuCaoThanhMau
    });

    thanhMauNen.setPosition(viTriThanhMau);
    thanhMauNen.setFillColor(sf::Color(50, 50, 50));

    thanhMauHienTai.setPosition(viTriThanhMau);

    thanhMauHienTai.setSize({
        chieuRongThanhMau *
            std::clamp(tiLeMau, 0.f, 1.f),
        chieuCaoThanhMau
    });

    thanhMauHienTai.setFillColor(mauThanhMau);
}

void Thuyen::CapNhat(float deltaTime)
{
    if (daBiPhaHuy)
        return;

    (void)deltaTime;
    CapNhatThanhMau(mauThanhMau);
}

void Thuyen::Ve(sf::RenderWindow& window)
{
    if (daBiPhaHuy || !sprites[huongHienTai])
        return;

    window.draw(*sprites[huongHienTai]);
    window.draw(thanhMauNen);
    window.draw(thanhMauHienTai);
}