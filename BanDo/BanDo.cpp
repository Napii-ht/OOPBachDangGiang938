
#include "BanDo.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <regex>
#include <map>
#include <utility>

BanDo::BanDo()
    : mapWidth(0),
      mapHeight(0),
      tileWidth(32),
      tileHeight(32)
{
    loadMap("Assets/Map/BachDang.tmx");
}

const BanDo::TilesetInfo* BanDo::TimTileset(int gid) const
{
    const TilesetInfo* result = nullptr;

    for (const auto& tileset : tilesets)
    {
        if (gid >= tileset.firstGid &&
            (result == nullptr ||
             tileset.firstGid > result->firstGid))
        {
            result = &tileset;
        }
    }

    return result;
}

void BanDo::loadMap(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Khong the mo map: " << path << '\n';
        return;
    }

    std::string line;

    // 1. Doc kich thuoc map
    while (std::getline(file, line))
    {
        if (line.find("<map ") == std::string::npos)
            continue;

        std::size_t widthPos = line.find("width=\"");
        std::size_t heightPos = line.find("height=\"");

        if (widthPos != std::string::npos)
        {
            widthPos += 7;

            mapWidth = std::stoi(
                line.substr(
                    widthPos,
                    line.find('"', widthPos) - widthPos
                )
            );
        }

        if (heightPos != std::string::npos)
        {
            heightPos += 8;

            mapHeight = std::stoi(
                line.substr(
                    heightPos,
                    line.find('"', heightPos) - heightPos
                )
            );
        }

        break;
    }

    // 2. Doc tileset va animation
    std::map<int, TileAnimation> animations;

    file.clear();
    file.seekg(0);

    while (std::getline(file, line))
    {
        if (line.find("<tileset ") == std::string::npos)
            continue;

        TilesetInfo info{};

        info.firstGid = 0;
        info.tileCount = 0;
        info.columns = 1;
        info.tileWidth = 32;
        info.tileHeight = 32;

        auto getAttribute = [](const std::string& source,
                               const std::string& attribute) -> std::string
        {
            std::regex pattern(attribute + "=\"([^\"]*)\"");
            std::smatch match;

            if (std::regex_search(source, match, pattern))
                return match[1].str();

            return "";
        };

        std::string value;

        value = getAttribute(line, "firstgid");
        if (!value.empty())
            info.firstGid = std::stoi(value);

        value = getAttribute(line, "tilecount");
        if (!value.empty())
            info.tileCount = std::stoi(value);

        value = getAttribute(line, "columns");
        if (!value.empty())
            info.columns = std::stoi(value);

        value = getAttribute(line, "tilewidth");
        if (!value.empty())
            info.tileWidth = std::stoi(value);

        value = getAttribute(line, "tileheight");
        if (!value.empty())
            info.tileHeight = std::stoi(value);

        // Tim anh tileset
        std::string imageLine;

        while (std::getline(file, imageLine))
        {
            if (imageLine.find("<image ") != std::string::npos ||
                imageLine.find("</tileset>") != std::string::npos)
            {
                break;
            }
        }

        std::string imagePath =
            getAttribute(imageLine, "source");

        if (imagePath.empty())
        {
            std::cerr << "Khong tim thay anh cua tileset.\n";
            continue;
        }

        const std::string fullPath = "Assets/Map/" + imagePath;

        if (!info.texture.loadFromFile(fullPath))
        {
            std::cerr << "Khong the tai tileset: "
                      << fullPath << '\n';
            continue;
        }

        // Doc animation
        int currentTileId = -1;
        TileAnimation currentAnimation;
        bool readingAnimation = false;

        while (std::getline(file, line))
        {
            if (line.find("</tileset>") != std::string::npos)
                break;

            if (line.find("<tile id=\"") != std::string::npos)
            {
                value = getAttribute(line, "id");
                currentTileId = value.empty() ? -1 : std::stoi(value);

                currentAnimation.frames.clear();
                readingAnimation = false;
            }

            if (line.find("<animation>") != std::string::npos)
            {
                currentAnimation.frames.clear();
                readingAnimation = true;
            }

            if (readingAnimation &&
                line.find("<frame ") != std::string::npos)
            {
                std::string tileIdValue =
                    getAttribute(line, "tileid");

                std::string durationValue =
                    getAttribute(line, "duration");

                if (!tileIdValue.empty() &&
                    !durationValue.empty())
                {
                    currentAnimation.frames.push_back({
                        std::stoi(tileIdValue),
                        std::stoi(durationValue)
                    });
                }
            }

            if (line.find("</animation>") != std::string::npos)
            {
                if (currentTileId >= 0 &&
                    !currentAnimation.frames.empty())
                {
                    const int gid =
                        info.firstGid + currentTileId;

                    animations[gid] = currentAnimation;
                }

                readingAnimation = false;
            }
        }

        tilesets.push_back(std::move(info));
    }

    // 3. Doc cac tile tren layer
    file.clear();
    file.seekg(0);

    while (std::getline(file, line))
    {
        if (line.find("<layer ") == std::string::npos)
            continue;

        std::string layerName;
        std::smatch match;
        std::regex nameRegex(R"regex(name="([^"]*)")regex");

        if (std::regex_search(line, match, nameRegex))
            layerName = match[1].str();

        // Khong render layer Collision
        if (layerName == "Collision")
            continue;

        // Tim du lieu tile
        while (std::getline(file, line))
        {
            if (line.find("<data") != std::string::npos)
                break;
        }

        // Doc tung hang tile
        for (int y = 0; y < mapHeight; ++y)
        {
            if (!std::getline(file, line))
                break;

            std::stringstream stream(line);
            std::string value;

            for (int x = 0; x < mapWidth; ++x)
            {
                if (!std::getline(stream, value, ','))
                    break;

                if (value.empty())
                    continue;

                const unsigned long long rawGid =
                    std::stoull(value);

                // Loai bo cac bit flip cua Tiled
                const unsigned int gid =
                    static_cast<unsigned int>(
                        rawGid & 0x1FFFFFFF
                    );

                if (gid == 0)
                    continue;

                const TilesetInfo* tileset = TimTileset(gid);

                if (tileset == nullptr)
                    continue;

                const int localId = gid - tileset->firstGid;

                if (localId < 0 ||
                    localId >= tileset->tileCount)
                {
                    continue;
                }

                sf::Sprite sprite(tileset->texture);

                int frameTileId = localId;
                bool animated = false;
                TileAnimation animation;

                auto animationIt = animations.find(gid);

                if (animationIt != animations.end() &&
                    !animationIt->second.frames.empty())
                {
                    animated = true;
                    animation = animationIt->second;
                    frameTileId = animation.frames[0].tileId;
                }

                const int tileX =
                    frameTileId % tileset->columns;

                const int tileY =
                    frameTileId / tileset->columns;

                sprite.setTextureRect(
                    sf::IntRect(
                        {
                            tileX * tileset->tileWidth,
                            tileY * tileset->tileHeight
                        },
                        {
                            tileset->tileWidth,
                            tileset->tileHeight
                        }
                    )
                );

                sprite.setPosition({
                    static_cast<float>(x * tileWidth),
                    static_cast<float>(y * tileHeight)
                });

                tiles.push_back({
                    std::move(sprite),
                    tileset,
                    localId,
                    animated,
                    std::move(animation),
                    0,
                    0.f
                });
            }
        }
    }

    // 4. Doc cac Object Layer
    file.clear();
    file.seekg(0);

    std::string objectLayerName;

    while (std::getline(file, line))
    {
        if (line.find("<objectgroup ") != std::string::npos)
        {
            std::smatch match;
            std::regex nameRegex(R"regex(name="([^"]*)")regex");

            objectLayerName.clear();

            if (std::regex_search(line, match, nameRegex))
                objectLayerName = match[1].str();

            continue;
        }

        if (line.find("</objectgroup>") != std::string::npos)
        {
            objectLayerName.clear();
            continue;
        }

        if (objectLayerName.empty() ||
            line.find("<object ") == std::string::npos)
        {
            continue;
        }

        auto getAttribute = [&](const std::string& attribute) -> std::string
        {
            std::regex pattern(attribute + "=\"([^\"]*)\"");
            std::smatch match;

            if (std::regex_search(line, match, pattern))
                return match[1].str();

            return "";
        };

        ObjectInfo obj{};

        obj.layer = objectLayerName;
        obj.name = getAttribute("name");

        std::string x = getAttribute("x");
        std::string y = getAttribute("y");
        std::string width = getAttribute("width");
        std::string height = getAttribute("height");

        obj.x = x.empty() ? 0.f : std::stof(x);
        obj.y = y.empty() ? 0.f : std::stof(y);
        obj.width = width.empty() ? 0.f : std::stof(width);
        obj.height = height.empty() ? 0.f : std::stof(height);

        objects.push_back(std::move(obj));
    }

    // 5. Tao sprite cho cac object
    for (const auto& obj : objects)
    {
        std::string imagePath;

        if (obj.layer == "Stakes")
        {
            imagePath = "Assets/Objects/Stakes.png";
        }
        else if (obj.layer == "Boats")
        {
            imagePath = "Assets/Objects/Boats.png";
        }
        else
        {
            continue;
        }

        auto texture = std::make_unique<sf::Texture>();

        if (!texture->loadFromFile(imagePath))
        {
            std::cerr << "Khong the tai anh object: "
                      << imagePath << '\n';
            continue;
        }

        auto sprite = std::make_unique<sf::Sprite>(*texture);

        // Vi tri object trong Tiled tinh theo pixel
        sprite->setPosition({obj.x, obj.y});

        // Dieu chinh kich thuoc sprite theo object
        if (obj.width > 0.f && obj.height > 0.f)
        {
            const auto textureSize = texture->getSize();

            sprite->setScale({
                obj.width / static_cast<float>(textureSize.x),
                obj.height / static_cast<float>(textureSize.y)
            });
        }

        objectSprites.push_back({
            std::move(texture),
            std::move(sprite)
        });
    }
}

// Cap nhat animation
void BanDo::capNhat(float deltaTime)
{
    const float deltaMilliseconds = deltaTime * 1000.f;

    for (auto& tile : tiles)
    {
        if (!tile.animated || tile.animation.frames.empty())
            continue;

        tile.elapsedTime += deltaMilliseconds;

        while (true)
        {
            const AnimationFrame& currentFrame =
                tile.animation.frames[tile.currentFrame];

            if (tile.elapsedTime < currentFrame.duration)
                break;

            tile.elapsedTime -= currentFrame.duration;

            tile.currentFrame++;

            if (tile.currentFrame >=
                static_cast<int>(tile.animation.frames.size()))
            {
                tile.currentFrame = 0;
            }

            const AnimationFrame& nextFrame =
                tile.animation.frames[tile.currentFrame];

            const int tileX =
                nextFrame.tileId % tile.tileset->columns;

            const int tileY =
                nextFrame.tileId / tile.tileset->columns;

            tile.sprite.setTextureRect(
                sf::IntRect(
                    {
                        tileX * tile.tileset->tileWidth,
                        tileY * tile.tileset->tileHeight
                    },
                    {
                        tile.tileset->tileWidth,
                        tile.tileset->tileHeight
                    }
                )
            );
        }
    }
}

// Ve map
void BanDo::Ve(sf::RenderWindow& window)
{
    for (const auto& tile : tiles)
        window.draw(tile.sprite);

    for (const auto& obj : objectSprites)
        window.draw(*obj.sprite);
}

// Lay chieu rong map theo pixel
int BanDo::layChieuRong() const
{
    return mapWidth * tileWidth;
}

// Lay chieu cao map theo pixel
int BanDo::layChieuCao() const
{
    return mapHeight * tileHeight;
}