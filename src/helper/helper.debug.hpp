/**
 * @file helper.debug.hpp
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

#define DEBUG_AKTIF 0

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

#define RESET       "\033[00m"
#define HIJAU       "\033[32m"
#define BIRU        "\033[34m"
#define MERAH       "\033[31m"
#define KUNING      "\033[33m"

/* ---------- CETAK WAKTU ---------- */
inline std::string WaktuSekarang()
{
    using namespace std::chrono;
    auto sekarang = system_clock::now();
    std::time_t t = system_clock::to_time_t(sekarang);
    std::tm waktuLokal{};
#ifdef _WIN32
    localtime_s(&waktuLokal, &t);
#else
    localtime_r(&t, &waktuLokal);
#endif

    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S", &waktuLokal);
    return std::string(buffer);
}

/* ---------- CETAK PESAN INFO DAN ERROR ---------- */
#if DEBUG_AKTIF 
#define CETAK_INFO(fmt)     std::cout << BIRU   << "[" << WaktuSekarang() << "]" << HIJAU  << " INFO: "  << RESET << fmt << std::endl;
#define CETAK_WARN(fmt)     std::cout << BIRU   << "[" << WaktuSekarang() << "]" << KUNING << " WARN: "  << RESET << fmt << std::endl;
#define CETAK_ERROR(fmt)    std::cout << BIRU   << "[" << WaktuSekarang() << "]" << MERAH  << " ERROR: " << RESET << fmt << std::endl;
#else
#define CETAK_INFO(fmt) 
#define CETAK_WARN(fmt) 
#define CETAK_ERROR(fmt) 
#endif