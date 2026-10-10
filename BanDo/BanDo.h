#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <string>
#include <vector>

class BanDo
{
private:
    struct TilesetInfo
    {
        int firstGid;
        int tileCount;
        int columns;
        int tileWidth;
        int tileHeight;

        sf::Texture texture;
    };

    struct AnimationFrame
    {
        int tileId;
        int duration;
    };

    struct TileAnimation
    {
        std::vector<AnimationFrame> frames;
    };

    struct TileInfo
    {
        sf::Sprite sprite;
        const TilesetInfo* tileset;
        int originalLocalId;

        bool animated;
        TileAnimation animation;

        int currentFrame;
        float elapsedTime;
    };

    struct ObjectInfo
    {
        std::string layer;
        std::string name;
        std::vector<sf::Vector2f> points;

        float x;
        float y;
        float width;
        float height;

        int maVatPham = 0;
        bool daThuThap = false;
    };

    struct ObjectSprite
    {
        std::unique_ptr<sf::Texture> texture;
        std::unique_ptr<sf::Sprite> sprite;

        std::string layer;
        int objectIndex = -1;
    };

    std::vector<TilesetInfo> tilesets;
    std::vector<TileInfo> tiles;
    std::vector<ObjectInfo> objects;
    std::vector<ObjectSprite> objectSprites;
    std::vector<sf::Vector2f> cacDinhSong;

    bool coGioiHanSong = false;

    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;

    float tiLeRutNuocHienTai = 0.f;

    void loadMap(const std::string& path);

    const TilesetInfo* TimTileset(int gid) const;

public:
    BanDo();

    void capNhat(float deltaTime);
    void Ve(sf::RenderWindow& window);

    int layChieuRong() const;
    int layChieuCao() const;
    
    bool namTrongSong(sf::Vector2f viTri) const;

    void CapNhatThuyTrieu(float tiLeRutNuoc);
    bool VaChamCoc(sf::FloatRect hitBoxThuyen) const;

    void DatTrangThaiCoc(bool hienCoc);
    bool GanCocNhatDuoc(
        sf::Vector2f viTriNguoiChoi,
        float khoangCach = 60.f
    ) const;

    bool NhatCocGanNhat(
        sf::Vector2f viTriNguoiChoi,
        int& maVatPham,
        std::string& tenVatPham
    );

    void DanhDauCocDaThuThap(int maVatPham);
};