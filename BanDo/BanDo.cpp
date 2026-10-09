#include "BanDo.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
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

    // Doc kich thuoc map.
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

    // Doc tileset va animation.
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

        std::string value = getAttribute(line, "firstgid");

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

        // Tai anh tileset.
        std::string imageLine;

        while (std::getline(file, imageLine))
        {
            if (imageLine.find("<image ") != std::string::npos ||
                imageLine.find("</tileset>") != std::string::npos)
            {
                break;
            }
        }

        std::string imagePath = getAttribute(imageLine, "source");

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

        // Doc cac frame animation.
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

    // Doc cac tile tren layer.
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

        if (layerName == "Collision")
            continue;

        // Tim du lieu tile.
        while (std::getline(file, line))
        {
            if (line.find("<data") != std::string::npos)
                break;
        }

        // Doc tung hang tile.
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

                // Loai bo cac bit flip cua Tiled.
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

    // Doc cac Object Layer.
    file.clear();
    file.seekg(0);

    std::string objectLayerName;

    auto getAttribute = [](const std::string& source,
                           const std::string& attribute) -> std::string
    {
        std::regex pattern(attribute + "=\"([^\"]*)\"");
        std::smatch match;

        if (std::regex_search(source, match, pattern))
            return match[1].str();

        return "";
    };

    while (std::getline(file, line))
    {
        if (line.find("<objectgroup ") != std::string::npos)
        {
            objectLayerName = getAttribute(line, "name");
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

        const std::string objectLine = line;

        ObjectInfo obj{};
        obj.layer = objectLayerName;
        obj.name = getAttribute(objectLine, "name");

        std::string x = getAttribute(objectLine, "x");
        std::string y = getAttribute(objectLine, "y");
        std::string width = getAttribute(objectLine, "width");
        std::string height = getAttribute(objectLine, "height");

        obj.x = x.empty() ? 0.f : std::stof(x);
        obj.y = y.empty() ? 0.f : std::stof(y);
        obj.width = width.empty() ? 0.f : std::stof(width);
        obj.height = height.empty() ? 0.f : std::stof(height);

        // Doc noi dung object.
        std::string objectContent;

        if (objectLine.find("/>") == std::string::npos)
        {
            std::string childLine;

            while (std::getline(file, childLine))
            {
                objectContent += childLine + "\n";

                if (childLine.find("</object>") != std::string::npos)
                    break;
            }
        }

        // Doc properties cua coc co the nhat.
        if (obj.layer == "CollectibleStakes")
        {
            std::regex maRegex(
                R"regex(<property[^>]*name="maVatPham"[^>]*value="([^"]*)")regex");

            std::regex tenRegex(
                R"regex(<property[^>]*name="ten"[^>]*value="([^"]*)")regex");

            std::smatch propertyMatch;

            if (std::regex_search(
                    objectContent, propertyMatch, maRegex))
            {
                obj.maVatPham = std::stoi(propertyMatch[1].str());
            }

            if (std::regex_search(
                    objectContent, propertyMatch, tenRegex))
            {
                obj.name = propertyMatch[1].str();
            }
        }

        // Doc polygon neu object co polygon.
        if (objectContent.find("<polygon ") != std::string::npos)
        {
            std::regex polygonRegex(
                R"regex(<polygon[^>]*points="([^"]*)")regex");

            std::smatch pointsMatch;

            if (std::regex_search(
                    objectContent, pointsMatch, polygonRegex))
            {
                std::stringstream pointsStream(pointsMatch[1].str());
                std::string pointText;

                while (pointsStream >> pointText)
                {
                    const std::size_t comma = pointText.find(',');

                    if (comma == std::string::npos)
                        continue;

                    try
                    {
                        const float localX = std::stof(
                            pointText.substr(0, comma));

                        const float localY = std::stof(
                            pointText.substr(comma + 1));

                        obj.points.push_back({
                            obj.x + localX,
                            obj.y + localY
                        });
                    }
                    catch (const std::exception&)
                    {
                        std::cerr << "Khong doc duoc diem Polygon: "
                                  << pointText << '\n';
                    }
                }
            }
        }

        if (obj.layer == "RiverBounds" && obj.points.size() >= 3)
        {
            cacDinhSong = obj.points;
            coGioiHanSong = true;
        }
        else if (obj.layer == "RiverBounds")
        {
            std::cerr << "Canh bao: RiverBounds khong co Polygon hop le. "
                      << "So dinh doc duoc: "
                      << obj.points.size() << '\n';
        }

        objects.push_back(std::move(obj));
    }

    // Tao sprite cho cac object.
    for (std::size_t i = 0; i < objects.size(); ++i)
    {
        const auto& obj = objects[i];
        std::string imagePath;

        if (obj.layer == "Stakes")
        {
            imagePath = "Assets/Objects/Stakes.png";
        }
        else if (obj.layer == "CollectibleStakes")
        {
            imagePath = "Assets/Map/Terrain.png";
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

        if (obj.layer == "Stakes")
        {
            sprite->setTextureRect(
                sf::IntRect({0, 0}, {32, 64})
            );

            sprite->setOrigin({16.f, 64.f});
            sprite->setPosition({obj.x, obj.y});
        }
        else if (obj.layer == "CollectibleStakes")
        {
            sprite->setTextureRect(
                sf::IntRect({418, 174}, {30, 12})
            );

            sprite->setOrigin({15.f, 6.f});
            sprite->setPosition({obj.x, obj.y});
            sprite->setScale({1.5f, 1.5f});
        }
        else
        {
            sprite->setPosition({obj.x, obj.y});

            if (obj.width > 0.f && obj.height > 0.f)
            {
                const auto textureSize = texture->getSize();

                sprite->setScale({
                    obj.width / static_cast<float>(textureSize.x),
                    obj.height / static_cast<float>(textureSize.y)
                });
            }
        }

        objectSprites.push_back({
            std::move(texture),
            std::move(sprite),
            obj.layer,
            static_cast<int>(i)
        });
    }
}

// Cap nhat animation.
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

// Ve map.
void BanDo::Ve(sf::RenderWindow& window)
{
    for (const auto& tile : tiles)
        window.draw(tile.sprite);

    for (const auto& obj : objectSprites)
    {
        if (!obj.sprite)
            continue;

        if (obj.layer == "CollectibleStakes" &&
            obj.objectIndex >= 0 &&
            objects[obj.objectIndex].daThuThap)
        {
            continue;
        }

        window.draw(*obj.sprite);
    }
}

// Lay chieu rong map theo pixel.
int BanDo::layChieuRong() const
{
    return mapWidth * tileWidth;
}

// Lay chieu cao map theo pixel.
int BanDo::layChieuCao() const
{
    return mapHeight * tileHeight;
}

bool BanDo::namTrongSong(sf::Vector2f viTri) const
{
    if (!coGioiHanSong || cacDinhSong.size() < 3)
        return false;

    bool benTrong = false;

    for (std::size_t i = 0, j = cacDinhSong.size() - 1;
         i < cacDinhSong.size();
         j = i++)
    {
        const auto& a = cacDinhSong[i];
        const auto& b = cacDinhSong[j];

        if ((a.y > viTri.y) != (b.y > viTri.y))
        {
            const float giaoX =
                (b.x - a.x) * (viTri.y - a.y) /
                (b.y - a.y) + a.x;

            if (viTri.x < giaoX)
                benTrong = !benTrong;
        }
    }

    return benTrong;
}

void BanDo::DatTrangThaiCoc(bool hienCoc)
{
    for (auto& obj : objectSprites)
    {
        if (obj.layer != "Stakes" || !obj.sprite)
            continue;

        obj.sprite->setColor(
            hienCoc
                ? sf::Color::White
                : sf::Color::Transparent
        );
    }
}

bool BanDo::VaChamCoc(sf::FloatRect hitBoxThuyen) const
{
    for (const auto& obj : objectSprites)
    {
        if (obj.layer != "Stakes" || !obj.sprite)
            continue;

        // Bo qua coc dang chim.
        if (obj.sprite->getColor().a == 0)
            continue;

        if (obj.sprite->getGlobalBounds().findIntersection(hitBoxThuyen))
            return true;
    }

    return false;
}

bool BanDo::GanCocNhatDuoc(
    sf::Vector2f viTriNguoiChoi,
    float khoangCach
) const
{
    for (const auto& obj : objects)
    {
        if (obj.layer != "CollectibleStakes" ||
            obj.daThuThap ||
            obj.maVatPham <= 0)
        {
            continue;
        }

        sf::Vector2f tamCoc(
            obj.x + obj.width / 2.f,
            obj.y + obj.height / 2.f
        );

        sf::Vector2f delta = tamCoc - viTriNguoiChoi;

        const float khoangCachBinhPhuong =
            delta.x * delta.x + delta.y * delta.y;

        if (khoangCachBinhPhuong <= khoangCach * khoangCach)
            return true;
    }

    return false;
}

bool BanDo::NhatCocGanNhat(
    sf::Vector2f viTriNguoiChoi,
    int& maVatPham,
    std::string& tenVatPham
)
{
    ObjectInfo* cocGanNhat = nullptr;
    float khoangCachNhoNhat = 60.f * 60.f;

    for (auto& obj : objects)
    {
        if (obj.layer != "CollectibleStakes" ||
            obj.daThuThap ||
            obj.maVatPham <= 0)
        {
            continue;
        }

        sf::Vector2f tamCoc(
            obj.x + obj.width / 2.f,
            obj.y + obj.height / 2.f
        );

        sf::Vector2f delta = tamCoc - viTriNguoiChoi;

        const float d2 = delta.x * delta.x + delta.y * delta.y;

        if (d2 <= khoangCachNhoNhat)
        {
            khoangCachNhoNhat = d2;
            cocGanNhat = &obj;
        }
    }

    if (cocGanNhat == nullptr)
        return false;

    maVatPham = cocGanNhat->maVatPham;
    tenVatPham = cocGanNhat->name;

    return true;
}

void BanDo::DanhDauCocDaThuThap(int maVatPham)
{
    for (auto& obj : objects)
    {
        if (obj.layer == "CollectibleStakes" &&
            obj.maVatPham == maVatPham &&
            !obj.daThuThap)
        {
            obj.daThuThap = true;
            return;
        }
    }
}