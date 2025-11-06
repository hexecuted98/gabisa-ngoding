/**
 * @file gabisa-ngoding.util.hpp
 * @brief Utilitas CLI untuk compiler dan runner gabisa-ngoding
 * 
 * @authors adjidev, espacioo
 */

#pragma once

#define MAJOR_VERSION 1
#define PATCH_VERSION 0
#define MINOR_VERSION 0
#define AUTHORS "adjidev, espacioo"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include "../helper/helper.debug.hpp"
#include "../lexer/gabisa-ngoding.lexer.hpp"
#include "../runtime/gabisa-ngoding.compiler.hpp"
#include "../runtime/gabisa-ngoding.vm.hpp"
#include "../utilitas/gabisa-ngoding.converter.hpp"

/* ---------- KOMPATIBILITAS ---------- */
#if __cplusplus < 201703L
#error "Maaf header bahasa gabisa-ngoding minimal butuh compiler C++17 Ke atas."
#endif

#ifndef __cplusplus
#error "Maaf header ini hanya mendukung C++ tidak mendukung C pastikan kamu mengkompilasikan pakai C++ compiler."
#endif

namespace fs = std::filesystem;

class GabisaNgoding_Util {
private:
    std::string version = "1.0.0";
    std::string author = "adjidev, espacioo";

public:
    /**
     * @brief Compile file gabisa-ngoding ke bytecode
     */
    bool compileFile(const std::string& inputFile, const std::string& outputFile = "") {
        try {
            CETAK_INFO("Mengompilasi file: " + inputFile);
            
            if (!fs::exists(inputFile)) {
                CETAK_ERROR("File tidak ditemukan: " + inputFile);
                return false;
            }
            
            std::ifstream file(inputFile);
            if (!file.is_open()) {
                CETAK_ERROR("Gagal membuka file: " + inputFile);
                return false;
            }
            
            std::string sourceCode((std::istreambuf_iterator<char>(file)), 
                                 std::istreambuf_iterator<char>());
            file.close();
            
            std::string outputPath = outputFile;
            if (outputPath.empty()) {
                outputPath = fs::path(inputFile).stem().string() + ".gbc";
            }
            
            CETAK_INFO("Melakukan lexing...");
            GabisaNgoding_Lexer lexer(sourceCode);
            lexer.Jalankan();
            
            CETAK_INFO("Melakukan kompilasi...");
            GabisaNgoding_Compiler compiler;
            auto bytecode = compiler.Compile(lexer.AmbilToken());
            
            std::ofstream outFile(outputPath, std::ios::binary);
            if (!outFile.is_open()) {
                CETAK_ERROR("Gagal membuat file output: " + outputPath);
                return false;
            }
            
            outFile.write(reinterpret_cast<const char*>(bytecode.data()), bytecode.size());
            outFile.close();
            
            CETAK_INFO("Kompilasi berhasil! File output: " + outputPath);
            CETAK_INFO("Ukuran bytecode: " + std::to_string(bytecode.size()) + " bytes");
            
            return true;
            
        } catch (const std::exception& e) {
            CETAK_ERROR("Error selama kompilasi: " + std::string(e.what()));
            return false;
        }
    }
    
    /**
     * @brief Jalankan file bytecode gabisa-ngoding
     */
    bool runFile(const std::string& inputFile) {
        try {
            CETAK_INFO("Menjalankan file: " + inputFile);
            
            if (!fs::exists(inputFile)) {
                CETAK_ERROR("File tidak ditemukan: " + inputFile);
                return false;
            }
            
            std::string ext = fs::path(inputFile).extension().string();
            if (ext != ".gng" && ext != ".gbc") {
                CETAK_WARN("Ekstensi file tidak biasa: " + ext);
            }
            
            GabisaNgoding_VM vm;
            
            if (ext == ".gbc") {
                vm.muatFile(inputFile);
            } else {
                std::string tempBytecode = "gabisa-ngoding.cache.gbc";
                if (!compileFile(inputFile, tempBytecode)) {
                    return false;
                }
                vm.muatFile(tempBytecode);
                
                fs::remove(tempBytecode);
            }
            
            CETAK_INFO("Memulai eksekusi...");
            vm.jalankan();
            
            return true;
            
        } catch (const std::exception& e) {
            CETAK_ERROR("Error selama eksekusi: " + std::string(e.what()));
            return false;
        }
    }
    
    /**
     * @brief Compile dan jalankan langsung
     */
    bool compileAndRun(const std::string& inputFile) {
        try {
            CETAK_INFO("Compile & Run: " + inputFile);
            
            if (!fs::exists(inputFile)) {
                CETAK_ERROR("File tidak ditemukan: " + inputFile);
                return false;
            }
            
            std::ifstream file(inputFile);
            std::string sourceCode((std::istreambuf_iterator<char>(file)), 
                                 std::istreambuf_iterator<char>());
            file.close();
            
            GabisaNgoding_Lexer lexer(sourceCode);
            lexer.Jalankan();
            
            GabisaNgoding_Compiler compiler;
            auto bytecode = compiler.Compile(lexer.AmbilToken());
            
            GabisaNgoding_VM vm;
            vm.muatBytecode(bytecode);
            vm.jalankan();
            
            return true;
            
        } catch (const std::exception& e) {
            CETAK_ERROR("Error: " + std::string(e.what()));
            return false;
        }
    }
    
    /**
     * @brief Tampilkan informasi versi
     */
    void showVersion() {
        std::cout << "gabisa-ngoding " << version << std::endl;
        std::cout << "Dibuat oleh: " << author << std::endl;
        std::cout << "Bahasa pemrograman esoteric berbasis Brainfuck" << std::endl;
    }
    
    /**
     * @brief Tampilkan bantuan penggunaan
     */
    void tampilkanMenu(const std::string& programName) {
        std::cout << "Penggunaan: " << programName << " [OPTIONS] [FILE]" << std::endl;
        std::cout << std::endl;
        std::cout << "OPTIONS:" << std::endl;
        std::cout << "  -c, --compile FILE    Compile file gabisa-ngoding ke bytecode" << std::endl;
        std::cout << "  -o, --output FILE     Tentukan file output untuk kompilasi" << std::endl;
        std::cout << "  -r, --run FILE        Jalankan file bytecode atau sumber" << std::endl;
        std::cout << "  -x, --execute FILE    Compile dan jalankan langsung" << std::endl;
        std::cout << "  -v, --version         Tampilkan informasi versi" << std::endl;
        std::cout << "  -h, --help            Tampilkan bantuan ini" << std::endl;
        std::cout << std::endl;
        std::cout << "CONTOH:" << std::endl;
        std::cout << "  " << programName << " -c contoh.gng             # Compile ke contoh.gbc" << std::endl;
        std::cout << "  " << programName << " -c contoh.gng -o out.gbc  # Compile ke out.gbc" << std::endl;
        std::cout << "  " << programName << " -r contoh.gbc             # Jalankan bytecode" << std::endl;
        std::cout << "  " << programName << " -x contoh.gng             # Compile & run langsung" << std::endl;
        std::cout << "  " << programName << " contoh.gng                # Jalankan file sumber" << std::endl;
        std::cout << "EKSTENSI FILE:" << std::endl;
        std::cout << "  .gng  - File sumber gabisa-ngoding" << std::endl;
        std::cout << "  .gbc  - File bytecode gabisa-ngoding" << std::endl;
    }
    
    /**
     * @brief Parse command line arguments
     */
    int parseArguments(int argc, char* argv[]) {
        if (argc < 2) {
            tampilkanMenu(argv[0]);
            return 1;
        }
        
        std::string command = argv[1];
        
        if (command == "-h" || command == "--help") {
            tampilkanMenu(argv[0]);
            return 0;
        }
        else if (command == "-v" || command == "--version") {
            showVersion();
            return 0;
        }
        else if (command == "-c" || command == "--compile") {
            if (argc < 3) {
                CETAK_ERROR("File input diperlukan untuk kompilasi");
                tampilkanMenu(argv[0]);
                return 1;
            }
            
            std::string inputFile = argv[2];
            std::string outputFile = "";
            
            for (int i = 3; i < argc; i++) {
                std::string arg = argv[i];
                if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
                    outputFile = argv[++i];
                }
            }
            
            return compileFile(inputFile, outputFile) ? 0 : 1;
        }
        else if (command == "-r" || command == "--run") {
            if (argc < 3) {
                CETAK_ERROR("File input diperlukan untuk dijalankan");
                tampilkanMenu(argv[0]);
                return 1;
            }
            
            std::string inputFile = argv[2];
            return runFile(inputFile) ? 0 : 1;
        }
        else if (command == "-x" || command == "--execute") {
            if (argc < 3) {
                CETAK_ERROR("File input diperlukan untuk dijalankan");
                tampilkanMenu(argv[0]);
                return 1;
            }
            
            std::string inputFile = argv[2];
            return compileAndRun(inputFile) ? 0 : 1;
        }
        else {
            return runFile(command) ? 0 : 1;
        }
    }
};