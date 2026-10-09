
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

        float x;
        float y;
        float width;
        float height;
    };

    struct ObjectSprite
    {
        std::unique_ptr<sf::Texture> texture;
        std::unique_ptr<sf::Sprite> sprite;
    };

    std::vector<TilesetInfo> tilesets;
    std::vector<TileInfo> tiles;
    std::vector<ObjectInfo> objects;
    std::vector<ObjectSprite> objectSprites;

    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;

    void loadMap(const std::string& path);

    const TilesetInfo* TimTileset(int gid) const;

public:
    BanDo();

    void capNhat(float deltaTime);
    void Ve(sf::RenderWindow& window);

    int layChieuRong() const;
    int layChieuCao() const;
};