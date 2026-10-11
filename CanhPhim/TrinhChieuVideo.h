#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <vector>

struct plm_t;

// Trình chiếu video điện ảnh tích hợp trực tiếp trong SFML
// Không cần bất kỳ trình phát video ngoài nào (Windows Media Player, VLC...)
// Chạy mượt mà đa nền tảng, đưa lên GitHub người khác clone về vẫn xem được 100%
class TrinhChieuVideo {
private:
    plm_t* plm;
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> sprite;
    std::vector<std::uint8_t> bufferRGBA;
    sf::Music nhac;
    bool dangPhat;
    bool daXong;
    float thoiGianTroi;
    float thoiLuong;
    unsigned int rongVideo;
    unsigned int caoVideo;

    std::string TimDuongDanTep(const std::string& tenTep) const;

public:
    TrinhChieuVideo();
    ~TrinhChieuVideo();

    bool MoVideo(const std::string& tenVideoMpg = "ChienThangBachDang.mpg",
                 const std::string& tenAudioOgg = "ChienThangBachDang.ogg");
    void XuLySuKien(const sf::Event& suKien);
    void CapNhat(float deltaTime);
    void Ve(sf::RenderWindow& window, const sf::Font& font);
    void Dung();

    void CapNhatKhungHinh(void* framePtr);

    bool IsDangPhat() const { return dangPhat; }
    bool IsDaXong() const { return daXong; }
};

