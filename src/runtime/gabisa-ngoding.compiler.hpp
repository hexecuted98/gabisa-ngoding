/**
 * @file gabisa-ngoding.compiler.hpp
 * @brief Compiler yang diperbaiki untuk bahasa 'gabisa-ngoding'
 */

#pragma once

/* ---------- KOMPATIBILITAS ---------- */
#if __cplusplus < 201703L
#error "Maaf header bahasa gabisa-ngoding minimal butuh compiler C++17 Ke atas."
#endif

#ifndef __cplusplus
#error "Maaf header ini hanya mendukung C++ tidak mendukung C pastikan kamu mengkompilasikan pakai C++ compiler."
#endif

#include <vector>
#include <stack>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include "../helper/helper.debug.hpp"
#include "../lexer/gabisa-ngoding.token.hpp"

class GabisaNgoding_Compiler
{
private:
    struct LoopInfo
    {
        size_t startPos;
        size_t jumpPos;
    };

public:
    /**
     * @brief Compile token 'gabisa-ngoding' langsung ke bytecode sederhana
     */
    std::vector<uint8_t> Compile(const std::vector<Tokennya> &tokens)
    {
        CETAK_INFO("Memulai kompilasi dari tokens...");
        std::vector<uint8_t> bytecode;
        std::stack<size_t> loopStack;

        for (const auto &token : tokens)
        {
            switch (token.jenis)
            {
            case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_AKU:     // >
                bytecode.push_back(0x01);
                break;

            case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_PUH:     // <
                bytecode.push_back(0x02);
                break;

            case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_GABISA:  // +
                bytecode.push_back(0x03);
                break;

            case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_NGODING: // -
                bytecode.push_back(0x04);
                break;

            case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_AJARIN:  // .
                bytecode.push_back(0x05);
                break;

            case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_DONG:    // ,
                bytecode.push_back(0x06);
                break;

            case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_HAI:     // [
                bytecode.push_back(0x07);
                loopStack.push(bytecode.size() - 1);
                break;

            case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_SEPUH:   // ]
                if (loopStack.empty())
                {
                    CETAK_ERROR("SEPUH tanpa HAI di baris " + std::to_string(token.baris));
                    throw std::runtime_error("SEPUH tanpa HAI");
                }

                {
                    size_t haiPos = loopStack.top();
                    loopStack.pop();

                    bytecode.push_back(0x08);
                }
                break;

            case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_EOF:
                break;

            default:
                CETAK_WARN("Token tidak dikompilasi: " + TokenToString(token.jenis));
                break;
            }
        }

        if (!loopStack.empty())
        {
            CETAK_ERROR("HAI tidak ditutup dengan SEPUH");
            throw std::runtime_error("HAI tidak ditutup dengan SEPUH");
        }

        CETAK_INFO("Kompilasi selesai! Ukuran bytecode: " + std::to_string(bytecode.size()) + " bytes");

        // std::cout << "Bytecode hasil kompilasi: ";
        // for (size_t i = 0; i < bytecode.size(); i++)
        // {
        //     std::cout << std::hex << static_cast<int>(bytecode[i]) << " ";
        // }
        // std::cout << std::dec << std::endl;

        return bytecode;
    }
};