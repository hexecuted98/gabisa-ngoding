/**
 * @file gabisa-ngoding.ast.hpp
 * @brief Abstract Syntax Tree untuk bahasa 'gabisa-ngoding'
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
#include <string>
#include <sstream>
#include "../helper/helper.debug.hpp"
#include "../lexer/gabisa-ngoding.token.hpp"

/* ---------- Forward Declaration ---------- */
class GabisaNgoding_Parser;

/* ---------- Node dasar ---------- */
struct ASTNode
{
    virtual ~ASTNode() = default;
    virtual std::string ToString(int indent = 0) const = 0;
    virtual void Print(int indent = 0) const
    {
        std::cout << ToString(indent) << std::endl;
    }
};

/* ---------- Node Instruksi Tunggal ---------- */
struct InstructionNode : public ASTNode
{
    GabisaNgoding_TokenType token;
    std::string value;
    size_t baris;
    size_t kolom;

    explicit InstructionNode(GabisaNgoding_TokenType t, const std::string &v = "",
                             size_t b = 0, size_t k = 0)
        : token(t), value(v), baris(b), kolom(k) {}

    std::string ToString(int indent = 0) const override
    {
        std::string indentStr(indent, ' ');
        std::string namaToken = TokenToString(token);

        std::stringstream ss;
        ss << indentStr << namaToken;

        if (!value.empty())
        {
            ss << " ('" << value << "')";
        }

        if (baris > 0)
        {
            ss << " [baris " << baris << ", kolom " << kolom << "]";
        }

        return ss.str();
    }
};

/* ---------- Node Program (root node) ---------- */
struct ProgramNode : public ASTNode
{
    std::vector<std::unique_ptr<ASTNode>> instructions;

    std::string ToString(int indent = 0) const override
    {
        std::string indentStr(indent, ' ');
        std::stringstream ss;

        ss << indentStr << "PROGRAM:\n";
        for (const auto &instr : instructions)
        {
            ss << instr->ToString(indent + 2) << "\n";
        }

        return ss.str();
    }

    void AddInstruction(std::unique_ptr<ASTNode> instr)
    {
        instructions.push_back(std::move(instr));
    }
};

/* ---------- Node Loop (berisi daftar instruksi di dalamnya) ---------- */
struct LoopNode : public ASTNode
{
    std::vector<std::unique_ptr<ASTNode>> body;
    size_t baris;
    size_t kolom;

    LoopNode(size_t b = 0, size_t k = 0) : baris(b), kolom(k) {}

    std::string ToString(int indent = 0) const override
    {
        std::string indentStr(indent, ' ');
        std::stringstream ss;

        ss << indentStr << "HAI (loop) [baris " << baris << ", kolom " << kolom << "]:\n";

        if (body.empty())
        {
            ss << indentStr << "  (loop kosong)\n";
        }
        else
        {
            for (const auto &instr : body)
            {
                ss << instr->ToString(indent + 2) << "\n";
            }
        }

        ss << indentStr << "SEPUH (end loop)";
        return ss.str();
    }

    void AddInstruction(std::unique_ptr<ASTNode> instr)
    {
        body.push_back(std::move(instr));
    }
};