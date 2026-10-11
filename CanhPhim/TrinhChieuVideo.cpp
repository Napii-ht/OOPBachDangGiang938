#include "TrinhChieuVideo.h"
#define PL_MPEG_IMPLEMENTATION
#include "pl_mpeg.h"
#include "../CotLoi/TienIch.h"

#include <filesystem>
#include <algorithm>
#include <iostream>

static void OnVideoFrame(plm_t* /*self*/, plm_frame_t* frame, void* user) {
    auto* trinhChieu = static_cast<TrinhChieuVideo*>(user);
    if (trinhChieu && frame) {
        trinhChieu->CapNhatKhungHinh(frame);
    }
}

TrinhChieuVideo::TrinhChieuVideo()
    : plm(nullptr)
    , dangPhat(false)
    , daXong(false)
    , thoiGianTroi(0.f)
    , thoiLuong(0.f)
    , rongVideo(0)
    , caoVideo(0) {
}

TrinhChieuVideo::~TrinhChieuVideo() {
    Dung();
}

std::string TrinhChieuVideo::TimDuongDanTep(const std::string& tenTep) const {
    const char* cacTienTo[] = { "", "../", "../../", "../../../" };
    std::error_code ec;
    for (const char* t : cacTienTo) {
        std::string thu1 = std::string(t) + "TaiNguyen/Video/" + tenTep;
        if (std::filesystem::exists(thu1, ec)) return thu1;
        std::string thu2 = std::string(t) + "TaiNguyen/" + tenTep;
        if (std::filesystem::exists(thu2, ec)) return thu2;
        std::string thu3 = std::string(t) + tenTep;
        if (std::filesystem::exists(thu3, ec)) return thu3;
    }
    return "";
}

bool TrinhChieuVideo::MoVideo(const std::string& tenVideoMpg, const std::string& tenAudioOgg) {
    Dung();

    std::string duongDanMpg = TimDuongDanTep(tenVideoMpg);
    if (duongDanMpg.empty()) {
        std::cerr << "[TrinhChieuVideo] Khong tim thay tep video: " << tenVideoMpg << std::endl;
        return false;
    }

    plm = plm_create_with_filename(duongDanMpg.c_str());
    if (!plm) {
        std::cerr << "[TrinhChieuVideo] Khong the khoi tao pl_mpeg cho: " << duongDanMpg << std::endl;
        return false;
    }

    plm_set_audio_enabled(plm, FALSE);
    plm_set_video_decode_callback(plm, OnVideoFrame, this);
    plm_set_loop(plm, FALSE);

    rongVideo = static_cast<unsigned int>(plm_get_width(plm));
    caoVideo = static_cast<unsigned int>(plm_get_height(plm));
    thoiLuong = static_cast<float>(plm_get_duration(plm));

    if (rongVideo == 0 || caoVideo == 0) {
        Dung();
        return false;
    }

    if (!texture.resize({rongVideo, caoVideo})) {
        Dung();
        return false;
    }
    sprite = std::make_unique<sf::Sprite>(texture);
    bufferRGBA.assign(rongVideo * caoVideo * 4, 255);

    // Giải mã khung hình đầu tiên để có hình ảnh ngay lập tức
    plm_frame_t* frameDau = plm_decode_video(plm);
    if (frameDau) {
        CapNhatKhungHinh(frameDau);
    }

    // Tải và phát âm thanh đồng bộ nếu có
    std::string duongDanOgg = TimDuongDanTep(tenAudioOgg);
    if (!duongDanOgg.empty()) {
        if (nhac.openFromFile(duongDanOgg)) {
            nhac.setLooping(false);
            nhac.play();
        }
    }

    dangPhat = true;
    daXong = false;
    thoiGianTroi = 0.f;
    return true;
}

void TrinhChieuVideo::CapNhatKhungHinh(void* framePtr) {
    auto* frame = static_cast<plm_frame_t*>(framePtr);
    if (!frame || bufferRGBA.empty() || rongVideo == 0) return;
    plm_frame_to_rgba(frame, bufferRGBA.data(), static_cast<int>(rongVideo * 4));
    texture.update(bufferRGBA.data());
}

void TrinhChieuVideo::XuLySuKien(const sf::Event& suKien) {
    if (!dangPhat) return;

    if (const auto* keyPress = suKien.getIf<sf::Event::KeyPressed>()) {
        if (keyPress->code == sf::Keyboard::Key::Space ||
            keyPress->code == sf::Keyboard::Key::Escape ||
            keyPress->code == sf::Keyboard::Key::Enter) {
            Dung();
            daXong = true;
        }
    }
}

void TrinhChieuVideo::CapNhat(float deltaTime) {
    if (!dangPhat || !plm) return;

    thoiGianTroi += deltaTime;

    double tick = static_cast<double>(deltaTime);
    if (nhac.getStatus() == sf::Music::Status::Playing) {
        double thoiGianAudio = static_cast<double>(nhac.getPlayingOffset().asSeconds());
        double thoiGianVideo = plm_get_time(plm);
        double doLech = thoiGianAudio - thoiGianVideo;
        if (doLech > 0.0) {
            tick = doLech;
        } else {
            tick = 0.0;
        }
    }

    if (tick > 0.0) {
        if (tick > 0.2) tick = 0.2;
        plm_decode(plm, tick);
    }

    if (plm_has_ended(plm) || (thoiLuong > 0.f && thoiGianTroi >= thoiLuong + 0.5f)) {
        Dung();
        daXong = true;
    }
}

void TrinhChieuVideo::Ve(sf::RenderWindow& window, const sf::Font& font) {
    if (!dangPhat) return;

    sf::Vector2f kichThuocCuaSo(window.getSize());
    if (kichThuocCuaSo.x <= 0.f || kichThuocCuaSo.y <= 0.f) return;

    // Thiết lập góc nhìn theo đúng kích thước pixel thực tế của cửa sổ (hỗ trợ phóng to / toàn màn hình)
    sf::View viewToanManHinh(sf::FloatRect({0.f, 0.f}, kichThuocCuaSo));
    window.setView(viewToanManHinh);

    // Nền đen điện ảnh bao phủ toàn bộ màn hình
    sf::RectangleShape nen(kichThuocCuaSo);
    nen.setFillColor(sf::Color::Black);
    window.draw(nen);

    // Căn giữa và co dãn tỷ lệ khung hình
    if (sprite && rongVideo > 0 && caoVideo > 0) {
        float tiLeX = kichThuocCuaSo.x / static_cast<float>(rongVideo);
        float tiLeY = kichThuocCuaSo.y / static_cast<float>(caoVideo);
        float tiLeChung = std::min(tiLeX, tiLeY);

        float rongThuc = static_cast<float>(rongVideo) * tiLeChung;
        float caoThuc = static_cast<float>(caoVideo) * tiLeChung;

        sprite->setScale({tiLeChung, tiLeChung});
        sprite->setPosition({(kichThuocCuaSo.x - rongThuc) * 0.5f, (kichThuocCuaSo.y - caoThuc) * 0.5f});
        window.draw(*sprite);
    }

    // Dòng chữ hướng dẫn bỏ qua video căn giữa đáy màn hình
    sf::Text textGoiY(font, VanBan("Nhấn [Phím cách] hoặc [Esc] để bỏ qua"), 13);
    textGoiY.setFillColor(sf::Color(200, 205, 215, 185));
    sf::FloatRect bounds = textGoiY.getLocalBounds();
    textGoiY.setOrigin({bounds.position.x + bounds.size.x * 0.5f, bounds.position.y + bounds.size.y * 0.5f});
    textGoiY.setPosition({kichThuocCuaSo.x * 0.5f, kichThuocCuaSo.y - 30.f});
    window.draw(textGoiY);
}

void TrinhChieuVideo::Dung() {
    if (nhac.getStatus() == sf::Music::Status::Playing) {
        nhac.stop();
    }
    if (plm) {
        plm_destroy(plm);
        plm = nullptr;
    }
    dangPhat = false;
}
