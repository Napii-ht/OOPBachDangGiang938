
#include "QuanLyAmThanh.h"
#include "../CotLoi/TienIch.h"
#include <cmath>
#include <filesystem>
#include <cstdlib>

QuanLyAmThanh::QuanLyAmThanh()
    : amThanhSanSang(false),
      nhipTrongDangBat(false),
      demNhipTrong(0.f) {
    TongHopAmThanh();
}

void QuanLyAmThanh::TongHopAmThanh() {
    const unsigned int sampleRate = 44100;
    const std::vector<sf::SoundChannel> channelMap = { sf::SoundChannel::Mono };

    // 1. Tiếng chém kiếm gió (Swoosh) - 0.15s
    {
        unsigned int numSamples = static_cast<unsigned int>(sampleRate * 0.15f);
        std::vector<std::int16_t> samples(numSamples);
        for (unsigned int i = 0; i < numSamples; ++i) {
            float t = static_cast<float>(i) / sampleRate;
            float env = std::sin(3.14159f * (t / 0.15f));
            float noise = static_cast<float>((rand() % 2000) - 1000) / 1000.f;
            float freq = 600.f - t * 2500.f;
            float sine = std::sin(2.f * 3.14159f * freq * t);
            samples[i] = static_cast<std::int16_t>(18000.f * env * (0.6f * noise + 0.4f * sine));
        }
        if (bufferChem.loadFromSamples(samples.data(), samples.size(), 1, sampleRate, channelMap)) {
            soundChem = std::make_unique<sf::Sound>(bufferChem);
        }
    }

    // 2. Tiếng trúng đòn chém (Hit / Flesh impact) - 0.12s
    {
        unsigned int numSamples = static_cast<unsigned int>(sampleRate * 0.12f);
        std::vector<std::int16_t> samples(numSamples);
        for (unsigned int i = 0; i < numSamples; ++i) {
            float t = static_cast<float>(i) / sampleRate;
            float env = std::exp(-28.f * t);
            float sine = std::sin(2.f * 3.14159f * (180.f - t * 800.f) * t);
            float noise = static_cast<float>((rand() % 2000) - 1000) / 1000.f;
            samples[i] = static_cast<std::int16_t>(26000.f * env * (0.7f * sine + 0.3f * noise));
        }
        if (bufferTrungDon.loadFromSamples(samples.data(), samples.size(), 1, sampleRate, channelMap)) {
            soundTrungDon = std::make_unique<sf::Sound>(bufferTrungDon);
        }
    }

    // 3. Tiếng tàu đâm cọc nứt vỡ gỗ (Wood crash & Water surge) - 0.45s
    {
        unsigned int numSamples = static_cast<unsigned int>(sampleRate * 0.45f);
        std::vector<std::int16_t> samples(numSamples);
        for (unsigned int i = 0; i < numSamples; ++i) {
            float t = static_cast<float>(i) / sampleRate;
            float env = std::exp(-7.f * t);
            float crack = static_cast<float>((rand() % 2000) - 1000) / 1000.f;
            float boom = std::sin(2.f * 3.14159f * 90.f * t);
            samples[i] = static_cast<std::int16_t>(28000.f * env * (0.6f * crack + 0.4f * boom));
        }
        if (bufferVaChamCoc.loadFromSamples(samples.data(), samples.size(), 1, sampleRate, channelMap)) {
            soundVaChamCoc = std::make_unique<sf::Sound>(bufferVaChamCoc);
        }
    }

    // 4. Tiếng trống trận rền vang (War drum kick) - 0.38s
    {
        unsigned int numSamples = static_cast<unsigned int>(sampleRate * 0.38f);
        std::vector<std::int16_t> samples(numSamples);
        for (unsigned int i = 0; i < numSamples; ++i) {
            float t = static_cast<float>(i) / sampleRate;
            float env = std::exp(-9.f * t);
            float freq = 85.f * std::exp(-12.f * t) + 45.f;
            float drum = std::sin(2.f * 3.14159f * freq * t);
            samples[i] = static_cast<std::int16_t>(30000.f * env * drum);
        }
        if (bufferTrongTran.loadFromSamples(samples.data(), samples.size(), 1, sampleRate, channelMap)) {
            soundTrongTran = std::make_unique<sf::Sound>(bufferTrongTran);
        }
    }

    // 5. Tiếng nhặt vật phẩm (Chime) - 0.18s
    {
        unsigned int numSamples = static_cast<unsigned int>(sampleRate * 0.18f);
        std::vector<std::int16_t> samples(numSamples);
        for (unsigned int i = 0; i < numSamples; ++i) {
            float t = static_cast<float>(i) / sampleRate;
            float env = std::exp(-14.f * t);
            float tone1 = std::sin(2.f * 3.14159f * 587.f * t); // D5
            float tone2 = std::sin(2.f * 3.14159f * 880.f * t); // A5
            samples[i] = static_cast<std::int16_t>(20000.f * env * (tone1 * 0.5f + tone2 * 0.5f));
        }
        if (bufferNhatDo.loadFromSamples(samples.data(), samples.size(), 1, sampleRate, channelMap)) {
            soundNhatDo = std::make_unique<sf::Sound>(bufferNhatDo);
        }
    }

    // 6. Tiếng chiến thắng hùng tráng (Victory fanfare) - 0.8s
    {
        unsigned int numSamples = static_cast<unsigned int>(sampleRate * 0.8f);
        std::vector<std::int16_t> samples(numSamples);
        for (unsigned int i = 0; i < numSamples; ++i) {
            float t = static_cast<float>(i) / sampleRate;
            float freq = 440.f; // A4
            if (t > 0.2f && t <= 0.4f) freq = 554.f; // C#5
            else if (t > 0.4f && t <= 0.6f) freq = 659.f; // E5
            else if (t > 0.6f) freq = 880.f; // A5
            float env = std::exp(-3.5f * (t - std::floor(t / 0.2f) * 0.2f));
            float s = std::sin(2.f * 3.14159f * freq * t);
            samples[i] = static_cast<std::int16_t>(22000.f * env * s);
        }
        if (bufferChienThang.loadFromSamples(samples.data(), samples.size(), 1, sampleRate, channelMap)) {
            soundChienThang = std::make_unique<sf::Sound>(bufferChienThang);
        }
    }

    amThanhSanSang = true;
}

void QuanLyAmThanh::CapNhat(float deltaTime) {
    if (nhipTrongDangBat) {
        demNhipTrong += deltaTime;
        if (demNhipTrong >= 0.75f) { // Nhịp trống trận dồn dập mỗi 0.75s
            demNhipTrong = 0.f;
            PhatTiengTrongTran();
        }
    }
}

void QuanLyAmThanh::PhatTiengChem() {
    if (soundChem) soundChem->play();
}

void QuanLyAmThanh::PhatTiengTrungDon() {
    if (soundTrungDon) soundTrungDon->play();
}

void QuanLyAmThanh::PhatTiengVaChamCoc() {
    if (soundVaChamCoc) soundVaChamCoc->play();
}

void QuanLyAmThanh::PhatTiengTrongTran() {
    if (soundTrongTran) soundTrongTran->play();
}

void QuanLyAmThanh::PhatTiengNhatDo() {
    if (soundNhatDo) soundNhatDo->play();
}

void QuanLyAmThanh::PhatTiengChienThang() {
    if (soundChienThang) soundChienThang->play();
}

void QuanLyAmThanh::KichHoatNhipTrongTran(bool bat) {
    nhipTrongDangBat = bat;
    if (bat) demNhipTrong = 0.f;
}

void QuanLyAmThanh::PhatNhacTranCuoi() {
    if (!PhatNhacNen("TranChienCuoi.ogg") &&
        !PhatNhacNen("TranChienCuoi.mp3") &&
        !PhatNhacNen("NhacChuong6.ogg") &&
        !PhatNhacNen("NhacChuong6.mp3")) {
        // Fallback: nếu chưa có file nhạc riêng, vẫn tiếp tục không lỗi
    }
}

void QuanLyAmThanh::PhatTiengTrongTrieuRut() {
    PhatTiengTrongTran();
    KichHoatNhipTrongTran(true);
}



bool QuanLyAmThanh::PhatNhacNen(const std::string& tenFile, bool lap) {
    if (nhacNen && tenNhacDangPhat == tenFile &&
        nhacNen->getStatus() == sf::SoundSource::Status::Playing) {
        return true;
    }

    // Kiểm tra file trước để thiếu nhạc thì im lặng, không in lỗi ra console
    std::string duongDan = DuongDanTaiNguyen("Nhac/" + tenFile);
    std::error_code loiFile;
    if (!std::filesystem::exists(duongDan, loiFile)) return false;

    auto nhacMoi = std::make_unique<sf::Music>();
    if (!nhacMoi->openFromFile(duongDan)) {
        return false;
    }

    nhacNen = std::move(nhacMoi);
    nhacNen->setLooping(lap);
    nhacNen->setVolume(70.f);
    nhacNen->play();
    tenNhacDangPhat = tenFile;
    return true;
}

void QuanLyAmThanh::DungNhacNen() {
    if (nhacNen) nhacNen->stop();
    tenNhacDangPhat.clear();
}

void QuanLyAmThanh::DatAmLuongChung(int phanTram) {
    if (phanTram < 0) phanTram = 0;
    if (phanTram > 100) phanTram = 100;
    sf::Listener::setGlobalVolume(static_cast<float>(phanTram));
}
