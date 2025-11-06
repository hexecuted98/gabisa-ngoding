/**
 * @file gabisa-ngoding.lexer.hpp
 * @brief Lexer untuk bahasa esoteric 'gabisa-ngoding'
 *
 * @authors adjidev, espacioo
 */

#pragma once

/* ---------- KOMPATIBILITAS ---------- */
#if __cplusplus < 201703L
#error "Maaf header bahasa gabisa-ngoding minimal butuh compiler C++17 Ke atas."
#endif

#ifndef __cplusplus
#error "Maaf header ini hanya mendukung C++ tidak mendukung C pastikan kamu mengkompilasikan pakai C++ compiler."
#endif

#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <stdexcept>

#include "../helper/helper.debug.hpp"
#include "./gabisa-ngoding.token.hpp"

/**
 * @struct Tokennya
 * @brief Menyimpan token hasil lexer beserta posisi sumber
 */
struct Tokennya
{
    GabisaNgoding_TokenType jenis;
    std::string sumberKode;
    size_t baris;
    size_t kolom;

    Tokennya(GabisaNgoding_TokenType j, const std::string &src, size_t b, size_t k)
        : jenis(j), sumberKode(src), baris(b), kolom(k) {}
};

/**
 * @class GabisaNgoding_Lexer
 * @brief Lexer untuk memecah kode sumber bahasa 'gabisa-ngoding' menjadi token-token.
 */
class GabisaNgoding_Lexer
{
private:
    std::string kode;
    std::vector<Tokennya> daftarToken;
    size_t posisi;
    size_t baris;
    size_t kolom;

    /**
     * @brief Ambil karakter saat ini
     */
    char karakterSekarang() const
    {
        if (posisi >= kode.length())
            return '\0';
        return kode[posisi];
    }

    /**
     * @brief Maju ke karakter berikutnya
     */
    void maju()
    {
        if (posisi < kode.length())
        {
            if (karakterSekarang() == '\n')
            {
                baris++;
                kolom = 1;
            }
            else
            {
                kolom++;
            }
            posisi++;
        }
    }

    /**
     * @brief Lewati spasi dan karakter whitespace
     */
    void lewatiSpasi()
    {
        while (std::isspace(karakterSekarang()))
        {
            maju();
        }
    }

    /**
     * @brief Baca sebuah kata (token)
     */
    std::string bacaKata()
    {
        std::string kata;
        size_t mulaiKolom = kolom;

        while (karakterSekarang() != '\0' && !std::isspace(karakterSekarang()))
        {
            kata += karakterSekarang();
            maju();
        }

        return kata;
    }

    /**
     * @brief Baca komentar sampai akhir baris
     */
    void bacaKomentar()
    {
        size_t mulaiBaris = baris;
        size_t mulaiKolom = kolom;

        maju();

        while (karakterSekarang() != '\0' && karakterSekarang() != '\n')
        {
            maju();
        }

        CETAK_WARN("Komentar ditemukan di baris " + std::to_string(mulaiBaris) +
                   ", kolom " + std::to_string(mulaiKolom));
    }

public:
    /**
     * @brief Constructor lexer
     */
    GabisaNgoding_Lexer(const std::string &sumber)
        : kode(sumber), posisi(0), baris(1), kolom(1) {}

    /**
     * @brief Mulai proses tokenisasi dari kode sumber
     */
    void Jalankan()
    {
        daftarToken.clear();

        CETAK_INFO("Memulai proses tokenisasi...");

        while (karakterSekarang() != '\0')
        {
            lewatiSpasi();

            if (karakterSekarang() == '\0')
                break;

            size_t mulaiBaris = baris;
            size_t mulaiKolom = kolom;

            if (karakterSekarang() == ';')
            {
                bacaKomentar();
                continue;
            }

            std::string kata = bacaKata();

            if (!kata.empty())
            {
                try
                {
                    GabisaNgoding_TokenType jenisToken = StringToToken(kata);
                    daftarToken.emplace_back(jenisToken, kata, mulaiBaris, mulaiKolom);

                    CETAK_INFO("Token ditemukan: " + TokenToString(jenisToken) +
                               " ('" + kata + "') di baris " +
                               std::to_string(mulaiBaris) + ", kolom " +
                               std::to_string(mulaiKolom));
                }
                catch (const std::exception &e)
                {
                    CETAK_ERROR("Token tidak dikenal di baris " +
                                std::to_string(mulaiBaris) + ", kolom " +
                                std::to_string(mulaiKolom) + ": '" + kata + "'");

                    daftarToken.emplace_back(
                        GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_UNKNOWN,
                        kata, mulaiBaris, mulaiKolom);
                }
            }
        }

        daftarToken.emplace_back(
            GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_EOF,
            "", baris, kolom);

        CETAK_INFO("Tokenisasi selesai! Total token: " + std::to_string(daftarToken.size()));
    }

    /**
     * @brief Ambil semua token hasil lexer
     */
    const std::vector<Tokennya> &AmbilToken() const
    {
        return daftarToken;
    }

    /**
     * @brief Cetak semua token hasil lexer
     */
    void CetakToken() const
    {
        CETAK_INFO("Daftar Token:");
        for (const auto &token : daftarToken)
        {
            std::cout << BIRU
                      << "[Baris " << token.baris
                      << ", Kolom " << token.kolom
                      << "] Token: " << TokenToString(token.jenis);

            if (!token.sumberKode.empty() &&
                token.jenis != GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_EOF)
            {
                std::cout << " (\"" << token.sumberKode << "\")";
            }

            std::cout << RESET << std::endl;
        }
    }

    /**
     * @brief Reset lexer untuk kode sumber baru
     */
    void Reset(const std::string &kodeBaru)
    {
        kode = kodeBaru;
        daftarToken.clear();
        posisi = 0;
        baris = 1;
        kolom = 1;
    }
};