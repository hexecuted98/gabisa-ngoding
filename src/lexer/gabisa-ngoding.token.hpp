/**
 * @file gabisa-ngoding.token.hpp
 *
 * @authors adjidev, espacioo
 */

#pragma once

/**
 * penjelasan token token dari bahasa program esoteric bernama 'gabisa-ngoding'
 *
 * bahasa gabisa ngoding adalah bahasa program esoteric mirip dengan brainfuck
 * tapi syntaxnya diganti dengan bahasa indonesia absurd
 *
 * buat yang belum tau syntax syntax bahasa program esoteric brainfuck dan 'gabisa-ngoding'
 * dibawah ini adalah tabel tabel perbedaan token token brainfuck dan 'gabisa-ngoding'
 *
 * TABEL - TABEL PERBEDAAN
 *
 * | BRAINFUCK |    GABISA NGODING     |        JENIS TOKEN      |
 * +-----------+-----------------------+-------------------------+
 * | TOKEN '[' | TOKEN 'HAI'           | LOOP START              |
 * +-----------+-----------------------+-------------------------+
 * | TOKEN ']' | TOKEN 'SEPUH'         | LOOP END                |
 * +-----------+-----------------------+-------------------------+
 * | TOKEN '+' | TOKEN  'GABISA'       | INCREMENT BYTE          |
 * +-----------+-----------------------+-------------------------+
 * | TOKEN '-' | TOKEN 'NGODING'       | DECREMENT BYTE          |
 * +-----------+-----------------------+-------------------------+
 * | TOKEN '>' | TOKEN 'AKU'           | POINTER INCREMENT       |
 * +-----------+-----------------------+-------------------------+
 * | TOKEN '<' | TOKEN 'PUH'           | POINTER DECREMENT       |
 * +-----------+-----------------------+-------------------------+
 * | TOKEN '.' | TOKEN 'AJARIN'        | OUTPUT                  |
 * +-----------+-----------------------+-------------------------+
 * | TOKEN ',' | TOKEN 'DONG'          | INPUT                   |
 * +-----------+-----------------------+-------------------------+
 * | TOKEN ';' | TOKEN 'KOMENTAR'      | KOMENTAR                |
 * +-----------+-----------------------+-------------------------+
 */

/* ---------- KOMPATIBILITAS ---------- */
#if __cplusplus < 201703L
#error "Maaf header bahasa gabisa-ngoding minimal butuh compiler C++17 Ke atas."
#endif

#ifndef __cplusplus
#error "Maaf header ini hanya mendukung C++ tidak mendukung C pastikan kamu mengkompilasikan pakai C++ compiler."
#endif

#include <string>
#include <unordered_map>
#include <stdexcept>

class GabisaNgoding_Lexer;

enum class GabisaNgoding_TokenType
{
    //---------- LOOP START & LOOP END ----------//
    GABISA_NGODING_TOKEN_HAI,   /* -> '[' */
    GABISA_NGODING_TOKEN_SEPUH, /* -> ']' */

    //---------- INCREMENT & DECREMENT BYTE ----------//
    GABISA_NGODING_TOKEN_GABISA,  /* -> '+' */
    GABISA_NGODING_TOKEN_NGODING, /* -> '-' */

    //---------- POINTER INCREMENT & DECREMENT -----------//
    GABISA_NGODING_TOKEN_AKU, /* -> '>' */
    GABISA_NGODING_TOKEN_PUH, /* -> '<' */

    //---------- INPUT & OUTPUT ----------//
    GABISA_NGODING_TOKEN_AJARIN, /* -> '.' */
    GABISA_NGODING_TOKEN_DONG,   /* -> ',' */

    //---------- LAINNYA/OTHERS ----------//
    GABISA_NGODING_TOKEN_KOMENTAR, /* -> ';' */

    //---------- TOKEN TAMBAHAN ----------//
    GABISA_NGODING_TOKEN_EOF,    /* END OF FILE */
    GABISA_NGODING_TOKEN_UNKNOWN /* TOKEN TAK DIKENAL */
};

/**
 * @brief konversikan token 'gabisa-ngoding' ke string
 */
inline std::string TokenToString(GabisaNgoding_TokenType token)
{
    switch (token)
    {
    case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_HAI:
        return "HAI"; // '['
    case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_SEPUH:
        return "SEPUH"; // ']'

    case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_GABISA:
        return "GABISA"; // '+'
    case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_NGODING:
        return "NGODING"; // '-'

    case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_AKU:
        return "AKU"; // '>'
    case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_PUH:
        return "PUH"; // '<'

    case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_AJARIN:
        return "AJARIN"; // '.'
    case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_DONG:
        return "DONG"; // ','

    case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_KOMENTAR:
        return "KOMENTAR"; // ';'

    case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_EOF:
        return "EOF";

    default:
        return "UNKNOWN_TOKEN";
    }
}

/**
 * @brief konversi sumber string 'gabisa-ngoding' ke jenis token 'gabisa-ngoding'
 */
inline GabisaNgoding_TokenType StringToToken(const std::string &str)
{
    static const std::unordered_map<std::string, GabisaNgoding_TokenType> tabelTokennya = {
        {"HAI", GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_HAI},
        {"SEPUH", GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_SEPUH},
        {"GABISA", GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_GABISA},
        {"NGODING", GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_NGODING},
        {"AKU", GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_AKU},
        {"PUH", GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_PUH},
        {"AJARIN", GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_AJARIN},
        {"DONG", GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_DONG},
        {"KOMENTAR", GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_KOMENTAR}};

    auto it = tabelTokennya.find(str);
    if (it != tabelTokennya.end())
        return it->second;

    throw std::runtime_error("Token '" + str + "' tidak dikenal");
}