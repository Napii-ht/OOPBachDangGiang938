
#pragma once

#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <string>

class QuanLyAmThanh {
private:
    sf::SoundBuffer bufferChem;
    sf::SoundBuffer bufferTrungDon;
    sf::SoundBuffer bufferVaChamCoc;
    sf::SoundBuffer bufferTrongTran;
    sf::SoundBuffer bufferNhatDo;
    sf::SoundBuffer bufferChienThang;

    std::unique_ptr<sf::Sound> soundChem;
    std::unique_ptr<sf::Sound> soundTrungDon;
    std::unique_ptr<sf::Sound> soundVaChamCoc;
    std::unique_ptr<sf::Sound> soundTrongTran;
    std::unique_ptr<sf::Sound> soundNhatDo;
    std::unique_ptr<sf::Sound> soundChienThang;

    // Nhạc nền đọc từ file trong TaiNguyen/Nhac (.ogg, .wav, .mp3). Thiếu file thì im lặng.
    std::unique_ptr<sf::Music> nhacNen;
    std::string tenNhacDangPhat;

    bool amThanhSanSang;
    bool nhipTrongDangBat;
    float demNhipTrong;

    void TongHopAmThanh();

public:
    QuanLyAmThanh();
    ~QuanLyAmThanh() = default;

    void CapNhat(float deltaTime);

    void PhatTiengChem();
    void PhatTiengTrungDon();
    void PhatTiengVaChamCoc();
    void PhatTiengTrongTran();
    void PhatTiengNhatDo();
    void PhatTiengChienThang();

    void KichHoatNhipTrongTran(bool bat);
    void PhatNhacTranCuoi();
    void PhatTiengTrongTrieuRut();

    // Phát nhạc nền theo tên file. Trả về false nếu không tìm thấy file.
    // Nếu bài đó đang phát rồi thì giữ nguyên, không phát lại từ đầu.
    bool PhatNhacNen(const std::string& tenFile, bool lap = true);
    void DungNhacNen();

    // Âm lượng chung của toàn game, từ 0 đến 100
    void DatAmLuongChung(int phanTram);
};


