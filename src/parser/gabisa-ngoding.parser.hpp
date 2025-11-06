/**
 * @file gabisa-ngoding.parser.hpp
 * @brief Parser untuk bahasa 'gabisa-ngoding'
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
#include <memory>
#include <stdexcept>
#include <stack>
#include "./gabisa-ngoding.ast.hpp"
#include "../lexer/gabisa-ngoding.lexer.hpp"

class GabisaNgoding_Parser
{
private:
    const std::vector<Tokennya> &tokens;
    size_t posisi;
    std::stack<LoopNode *> loopStack;

    // Helper methods
    bool Habis() const
    {
        return posisi >= tokens.size() ||
               tokens[posisi].jenis == GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_EOF;
    }

    const Tokennya &Lihat() const
    {
        if (Habis())
        {
            throw std::runtime_error("Tidak ada token lagi");
        }
        return tokens[posisi];
    }

    const Tokennya &Ambil()
    {
        if (Habis())
        {
            throw std::runtime_error("Tidak ada token lagi");
        }
        return tokens[posisi++];
    }

    bool Cocok(GabisaNgoding_TokenType expected) const
    {
        if (Habis())
            return false;
        return Lihat().jenis == expected;
    }

    void Expect(GabisaNgoding_TokenType expected, const std::string &errorMsg)
    {
        if (!Cocok(expected))
        {
            const auto &token = Lihat();
            CETAK_ERROR(errorMsg + " di baris " + std::to_string(token.baris) +
                        ", kolom " + std::to_string(token.kolom) +
                        ". Dapat token: " + TokenToString(token.jenis));
            throw std::runtime_error(errorMsg);
        }
        Ambil();
    }

public:
    explicit GabisaNgoding_Parser(const std::vector<Tokennya> &toks)
        : tokens(toks), posisi(0) {}

    /**
     * @brief Parse sebuah loop (HAI ... SEPUH)
     */
    std::unique_ptr<LoopNode> ParseLoop()
    {
        const auto &tokenHAI = Ambil();
        CETAK_INFO("Memulai parsing loop di baris " + std::to_string(tokenHAI.baris));

        auto loopNode = std::make_unique<LoopNode>(tokenHAI.baris, tokenHAI.kolom);

        // Simpan state loop sebelumnya
        LoopNode *previousLoop = loopStack.empty() ? nullptr : loopStack.top();
        loopStack.push(loopNode.get());

        try
        {
            while (!Habis())
            {
                const auto &token = Lihat();

                switch (token.jenis)
                {
                case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_SEPUH:
                    CETAK_INFO("Menemukan SEPUH, mengakhiri loop");
                    Ambil(); // Konsumsi SEPUH
                    loopStack.pop();
                    return loopNode;

                case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_HAI:
                    loopNode->AddInstruction(ParseLoop());
                    break;

                default:
                    // Token instruksi biasa
                    if (IsInstructionToken(token.jenis))
                    {
                        const auto &instrToken = Ambil();
                        auto instrNode = std::make_unique<InstructionNode>(
                            instrToken.jenis, instrToken.sumberKode,
                            instrToken.baris, instrToken.kolom);
                        loopNode->AddInstruction(std::move(instrNode));
                    }
                    else
                    {
                        CETAK_WARN("Token tidak diharapkan dalam loop: " +
                                   TokenToString(token.jenis) + ", dilewati");
                        Ambil(); // Lewati token yang tidak diharapkan
                    }
                    break;
                }
            }

            // Jika sampai di sini, berarti SEPUH tidak ditemukan
            CETAK_ERROR("Loop tidak ditutup dengan SEPUH (mulai di baris " +
                        std::to_string(tokenHAI.baris) + ")");
            throw std::runtime_error("Loop tidak ditutup dengan SEPUH");
        }
        catch (...)
        {
            // Restore previous loop state
            loopStack.pop();
            if (previousLoop)
            {
                loopStack.push(previousLoop);
            }
            throw;
        }
    }

    /**
     * @brief Parse seluruh program
     */
    std::unique_ptr<ProgramNode> Parse()
    {
        CETAK_INFO("Memulai parsing program...");

        auto programNode = std::make_unique<ProgramNode>();
        size_t totalInstructions = 0;

        try
        {
            while (!Habis())
            {
                const auto &token = Lihat();

                switch (token.jenis)
                {
                case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_HAI:
                    programNode->AddInstruction(ParseLoop());
                    totalInstructions++;
                    break;

                case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_SEPUH:
                    CETAK_ERROR("SEPUH tanpa HAI di baris " + std::to_string(token.baris));
                    throw std::runtime_error("SEPUH tanpa HAI");

                default:
                    // Token instruksi biasa
                    if (IsInstructionToken(token.jenis))
                    {
                        const auto &instrToken = Ambil();
                        auto instrNode = std::make_unique<InstructionNode>(
                            instrToken.jenis, instrToken.sumberKode,
                            instrToken.baris, instrToken.kolom);
                        programNode->AddInstruction(std::move(instrNode));
                        totalInstructions++;
                    }
                    else
                    {
                        // Lewati token yang tidak relevan (seperti KOMENTAR, EOF, dll)
                        CETAK_WARN("Token dilewati: " + TokenToString(token.jenis));
                        Ambil();
                    }
                    break;
                }
            }

            CETAK_INFO("Parsing selesai! Total instruksi: " + std::to_string(totalInstructions));
            return programNode;
        }
        catch (const std::exception &e)
        {
            CETAK_ERROR("Error selama parsing: " + std::string(e.what()));
            throw;
        }
    }

private:
    /**
     * @brief Cek apakah token adalah instruksi yang valid
     */
    bool IsInstructionToken(GabisaNgoding_TokenType tokenType) const
    {
        switch (tokenType)
        {
        case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_GABISA:
        case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_NGODING:
        case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_AKU:
        case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_PUH:
        case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_AJARIN:
        case GabisaNgoding_TokenType::GABISA_NGODING_TOKEN_DONG:
            return true;
        default:
            return false;
        }
    }
};