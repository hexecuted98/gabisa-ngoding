/**
 * @file gabisa-ngoding.memory.hpp
 * @brief Memory manager untuk VM gabisa-ngoding
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
#include <cstdint>
#include <iostream>
#include <stdexcept>

class GabisaNgoding_Memory
{
private:
    std::vector<uint8_t> tape;
    size_t pointer;
    const size_t size;

public:
    explicit GabisaNgoding_Memory(size_t size = 30000)
        : tape(size, 0), pointer(0), size(size) {}

    void kanan()
    {
        if (pointer >= size - 1)
        {
            throw std::runtime_error("Pointer melebihi batas memory (kanan)");
        }
        pointer++;
    }

    void kiri()
    {
        if (pointer == 0)
        {
            throw std::runtime_error("Pointer melebihi batas memory (kiri)");
        }
        pointer--;
    }

    void tambah() { tape[pointer]++; }
    void kurang() { tape[pointer]--; }

    void cetak() const
    {
        std::cout << static_cast<char>(tape[pointer]);
        std::cout.flush();
    }

    void input()
    {
        char c;
        if (!std::cin.get(c))
        {
            tape[pointer] = 0;
        }
        else
        {
            tape[pointer] = static_cast<uint8_t>(c);
        }
    }

    uint8_t get() const { return tape[pointer]; }
    size_t getPointer() const { return pointer; }
    size_t getSize() const { return size; }

    void debugPrint(size_t range = 10) const
    {
        std::cout << "Memory Debug [pointer=" << pointer << "]: ";
        size_t start = (pointer >= range) ? pointer - range : 0;
        size_t end = (pointer + range < size) ? pointer + range : size - 1;

        for (size_t i = start; i <= end; i++)
        {
            if (i == pointer)
            {
                std::cout << "[" << static_cast<int>(tape[i]) << "] ";
            }
            else
            {
                std::cout << static_cast<int>(tape[i]) << " ";
            }
        }
        std::cout << std::endl;
    }

    void reset()
    {
        std::fill(tape.begin(), tape.end(), 0);
        pointer = 0;
    }
};