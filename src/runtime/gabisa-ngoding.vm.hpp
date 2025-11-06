/**
 * @file gabisa-ngoding.vm.hpp
 * @brief Virtual Machine yang diperbaiki dengan fungsi muatFile
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
#include <fstream>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <stack>
#include <filesystem>
#include "../helper/helper.debug.hpp"
#include "./gabisa-ngoding.memory.hpp"
#include "../lexer/gabisa-ngoding.lexer.hpp"
#include "./gabisa-ngoding.compiler.hpp"

namespace fs = std::filesystem;

class GabisaNgoding_VM
{
private:
    GabisaNgoding_Memory memory;
    std::vector<uint8_t> bytecode;
    size_t pc;
    bool running;
    std::stack<size_t> loopStack;

    /**
     * @brief Cari pasangan SEPUH untuk HAI di posisi tertentu
     */
    size_t findMatchingSepuh(size_t startPos)
    {
        int depth = 1;
        for (size_t i = startPos + 1; i < bytecode.size(); i++)
        {
            if (bytecode[i] == 0x07)
            { //HAI
                depth++;
            }
            else if (bytecode[i] == 0x08)
            { //SEPUH
                depth--;
                if (depth == 0)
                {
                    return i;
                }
            }
        }
        throw std::runtime_error("Tidak menemukan SEPUH yang cocok");
    }

    /**
     * @brief Cari pasangan HAI untuk SEPUH di posisi tertentu
     */
    size_t findMatchingHai(size_t startPos)
    {
        int depth = 1;
        for (size_t i = startPos - 1; i >= 0; i--)
        {
            if (bytecode[i] == 0x08)
            { //SEPUH
                depth++;
            }
            else if (bytecode[i] == 0x07)
            { //HAI
                depth--;
                if (depth == 0)
                {
                    return i;
                }
            }
        }
        throw std::runtime_error("Tidak menemukan HAI yang cocok");
    }

    /**
     * @brief Muat file bytecode biner (.gbc)
     */
    bool muatFileBytecode(const std::string &filename)
    {
        CETAK_INFO("Memuat file bytecode: " + filename);

        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            CETAK_ERROR("Gagal membuka file bytecode: " + filename);
            return false;
        }

        size_t fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        bytecode.resize(fileSize);
        if (!file.read(reinterpret_cast<char *>(bytecode.data()), fileSize))
        {
            CETAK_ERROR("Gagal membaca file bytecode: " + filename);
            return false;
        }
        file.close();

        CETAK_INFO("Berhasil memuat " + std::to_string(fileSize) + " bytes bytecode");
        return true;
    }

    /**
     * @brief Muat file sumber gabisa-ngoding (.gng) dan compile
     */
    bool muatFileSumber(const std::string &filename)
    {
        CETAK_INFO("Memuat file sumber: " + filename);

        std::ifstream file(filename);
        if (!file.is_open())
        {
            CETAK_ERROR("Gagal membuka file sumber: " + filename);
            return false;
        }

        std::string sourceCode((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
        file.close();

        CETAK_INFO("Ukuran file sumber: " + std::to_string(sourceCode.length()) + " karakter");

        try
        {

            GabisaNgoding_Lexer lexer(sourceCode);
            lexer.Jalankan();

            GabisaNgoding_Compiler compiler;
            bytecode = compiler.Compile(lexer.AmbilToken());

            CETAK_INFO("Berhasil mengompilasi ke " + std::to_string(bytecode.size()) + " bytes bytecode");
            return true;
        }
        catch (const std::exception &e)
        {
            CETAK_ERROR("Error selama kompilasi: " + std::string(e.what()));
            return false;
        }
    }

    /**
     * @brief Validasi bytecode yang dimuat
     */
    bool validasiBytecode()
    {
        if (bytecode.empty())
        {
            CETAK_ERROR("Bytecode kosong");
            return false;
        }

        int loopBalance = 0;
        for (uint8_t instr : bytecode)
        {
            if (instr == 0x07)
                loopBalance++; //HAI
            if (instr == 0x08)
                loopBalance--; //SEPUH
        }

        if (loopBalance != 0)
        {
            CETAK_ERROR("Loop tidak seimbang! HAI: " + std::to_string(loopBalance > 0 ? loopBalance : -loopBalance) +
                        " lebih banyak dari SEPUH");
            return false;
        }

        for (size_t i = 0; i < bytecode.size(); i++)
        {
            uint8_t instr = bytecode[i];
            if (instr < 0x01 || instr > 0x08)
            {
                CETAK_WARN("Instruksi tidak valid ditemukan di posisi " + std::to_string(i) +
                           ": 0x" + std::to_string(static_cast<int>(instr)));
            }
        }

        CETAK_INFO("Validasi bytecode berhasil");
        return true;
    }

public:
    explicit GabisaNgoding_VM(size_t memSize = 30000)
        : memory(memSize), pc(0), running(false) {}

    /**
     * @brief Muat file (bytecode atau sumber) ke VM
     */
    bool muatFile(const std::string &filename)
    {
        try
        {
            CETAK_INFO("Memuat file: " + filename);

            if (!fs::exists(filename))
            {
                CETAK_ERROR("File tidak ditemukan: " + filename);
                return false;
            }

            reset();

            std::string extension = fs::path(filename).extension().string();
            bool success = false;

            if (extension == ".gbc" || extension == ".bin")
            {
                success = muatFileBytecode(filename);
            }
            else if (extension == ".gng" || extension == ".txt")
            {
                success = muatFileSumber(filename);
            }
            else
            {
                CETAK_WARN("Ekstensi file tidak dikenali: " + extension + ", mencoba deteksi otomatis");

                std::ifstream file(filename, std::ios::binary);
                if (file.is_open())
                {
                    file.seekg(0, std::ios::end);
                    size_t size = file.tellg();
                    file.seekg(0, std::ios::beg);

                    std::vector<uint8_t> buffer(size);
                    file.read(reinterpret_cast<char *>(buffer.data()), size);
                    file.close();

                    bool isBytecode = true;
                    for (uint8_t b : buffer)
                    {
                        if (b < 0x01 || b > 0x08)
                        {
                            isBytecode = false;
                            break;
                        }
                    }

                    if (isBytecode && size > 0)
                    {
                        CETAK_INFO("File terdeteksi sebagai bytecode");
                        bytecode = buffer;
                        success = true;
                    }
                    else
                    {
                        CETAK_INFO("File terdeteksi sebagai sumber teks");
                        std::string sourceCode(buffer.begin(), buffer.end());
                        GabisaNgoding_Lexer lexer(sourceCode);
                        lexer.Jalankan();
                        GabisaNgoding_Compiler compiler;
                        bytecode = compiler.Compile(lexer.AmbilToken());
                        success = true;
                    }
                }
            }

            if (success)
            {
                if (!validasiBytecode())
                {
                    CETAK_ERROR("Validasi bytecode gagal");
                    return false;
                }

                CETAK_INFO("File berhasil dimuat, total instruksi: " + std::to_string(bytecode.size()));
                return true;
            }
            else
            {
                CETAK_ERROR("Gagal memuat file: " + filename);
                return false;
            }
        }
        catch (const std::exception &e)
        {
            CETAK_ERROR("Error selama memuat file: " + std::string(e.what()));
            return false;
        }
    }

    /**
     * @brief Muat bytecode langsung dari memory
     */
    void muatBytecode(const std::vector<uint8_t> &code)
    {
        bytecode = code;
        CETAK_INFO("Memuat bytecode langsung, ukuran: " + std::to_string(bytecode.size()) + " bytes");

        if (!validasiBytecode())
        {
            CETAK_WARN("Bytecode tidak valid, mungkin menyebabkan error selama eksekusi");
        }

        reset();
    }

    /**
     * @brief Muat source code langsung dan compile
     */
    bool muatSourceCode(const std::string &sourceCode)
    {
        CETAK_INFO("Memuat source code langsung, panjang: " + std::to_string(sourceCode.length()) + " karakter");

        try
        {

            GabisaNgoding_Lexer lexer(sourceCode);
            lexer.Jalankan();

            GabisaNgoding_Compiler compiler;
            bytecode = compiler.Compile(lexer.AmbilToken());

            CETAK_INFO("Berhasil mengompilasi ke " + std::to_string(bytecode.size()) + " bytes bytecode");

            if (!validasiBytecode())
            {
                CETAK_WARN("Bytecode tidak valid, mungkin menyebabkan error selama eksekusi");
            }

            reset();
            return true;
        }
        catch (const std::exception &e)
        {
            CETAK_ERROR("Error selama kompilasi: " + std::string(e.what()));
            return false;
        }
    }

    /**
     * @brief Jalankan VM dengan implementasi loop yang benar
     */
    void jalankan()
    {
        if (bytecode.empty())
        {
            CETAK_ERROR("Tidak ada bytecode yang dimuat");
            throw std::runtime_error("Tidak ada bytecode yang dimuat");
        }

        CETAK_INFO("Memulai eksekusi VM...");
        running = true;
        pc = 0;

        size_t stepCount = 0;
        const size_t maxSteps = 1000000; 

        while (running && pc < bytecode.size() && stepCount < maxSteps)
        {
            uint8_t instruksi = bytecode[pc];

            switch (instruksi)
            {
            case 0x01: //AKU -> pointer++
                memory.kanan();
                pc++;
                break;

            case 0x02: //PUH -> pointer--
                memory.kiri();
                pc++;
                break;

            case 0x03: //GABISA -> tambah byte
                memory.tambah();
                pc++;
                break;

            case 0x04: //NGODING -> kurang byte
                memory.kurang();
                pc++;
                break;

            case 0x05: //AJARIN -> output
                memory.cetak();
                pc++;
                break;

            case 0x06: //DONG -> input
                memory.input();
                pc++;
                break;

            case 0x07: //HAI -> loop start
                if (memory.get() == 0)
                {
                    pc = findMatchingSepuh(pc) + 1;
                }
                else
                {
                    pc++;
                }
                break;

            case 0x08: //SEPUH -> loop end
                if (memory.get() != 0)
                {
                    pc = findMatchingHai(pc);
                }
                else
                {
                    pc++;
                }
                break;

            default:
                CETAK_ERROR("Instruksi tidak dikenal: 0x" +
                            std::to_string(static_cast<int>(instruksi)) +
                            " di PC=" + std::to_string(pc));
                pc++;
                break;
            }

            stepCount++;

            if (stepCount >= maxSteps)
            {
                CETAK_ERROR("Eksekusi terlalu lama, mungkin infinite loop");
                debugState();
                throw std::runtime_error("Eksekusi melebihi batas maksimum");
            }
        }

        if (pc >= bytecode.size())
        {
            CETAK_INFO("Eksekusi selesai secara normal");
        }
        else if (running)
        {
            CETAK_WARN("Eksekusi dihentikan sebelum selesai");
        }

        CETAK_INFO("Total langkah: " + std::to_string(stepCount));
    }

    /**
     * @brief Reset VM state
     */
    void reset()
    {
        memory.reset();
        pc = 0;
        running = false;
        while (!loopStack.empty())
            loopStack.pop();
    }

    /**
     * @brief Dapatkan state memory untuk debugging
     */
    void debugState() const
    {
        std::cout << "VM State - PC: " << pc << "/" << bytecode.size();
        std::cout << ", Memory Pointer: " << memory.getPointer();
        std::cout << ", Value: " << static_cast<int>(memory.get()) << std::endl;
        memory.debugPrint();
    }

    /**
     * @brief Dapatkan statistik eksekusi
     */
    void tampilkanStatistik() const
    {
        std::cout << "\n=== STATISTIK EKSEKUSI ===" << std::endl;
        std::cout << "Ukuran bytecode: " << bytecode.size() << " instruksi" << std::endl;
        std::cout << "Program Counter: " << pc << "/" << bytecode.size() << std::endl;
        std::cout << "Memory pointer: " << memory.getPointer() << std::endl;
        std::cout << "Nilai saat ini: " << static_cast<int>(memory.get()) << std::endl;
    }

    /**
     * @brief Hentikan eksekusi
     */
    void berhenti()
    {
        running = false;
    }

    /**
     * @brief Cek apakah VM sedang berjalan
     */
    bool isRunning() const
    {
        return running;
    }

    /**
     * @brief Dapatkan bytecode yang dimuat
     */
    const std::vector<uint8_t> &getBytecode() const
    {
        return bytecode;
    }

    /**
     * @brief Dapatkan akses ke memory (untuk testing/debugging)
     */
    GabisaNgoding_Memory &getMemory()
    {
        return memory;
    }
};