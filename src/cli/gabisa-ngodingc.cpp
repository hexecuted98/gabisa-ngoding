/**
 * @file gabisa-ngodingc.cpp
 * @brief Compiler CLI untuk bahasa gabisa-ngoding
 * 
 * Usage: gabisa-ngodingc [OPTIONS] FILE
 */

#include "utilitas/gabisa-ngoding.util.hpp"

int main(int argc, char* argv[]) {
    try {
        CETAK_INFO("=== GABISA-NGODING COMPILER ===");
        
        GabisaNgoding_Util util;
        
        if (argc < 2) {
            util.tampilkanMenu("gabisa-ngodingc");
            return 1;
        }
        
        std::string command = argv[1];
        
        if (command == "-h" || command == "--help") {
            std::cout << "gabisa-ngodingc - Compiler untuk bahasa gabisa-ngoding" << std::endl;
            std::cout << std::endl;
            std::cout << "Usage: gabisa-ngodingc [OPTIONS] INPUT_FILE" << std::endl;
            std::cout << std::endl;
            std::cout << "OPTIONS:" << std::endl;
            std::cout << "  -o, --output FILE    Tentukan file output" << std::endl;
            std::cout << "  -v, --version        Tampilkan versi" << std::endl;
            std::cout << "  -h, --help           Tampilkan bantuan" << std::endl;
            std::cout << std::endl;
            std::cout << "CONTOH:" << std::endl;
            std::cout << "  gabisa-ngodingc program.gng             # Compile ke program.gbc" << std::endl;
            std::cout << "  gabisa-ngodingc program.gng -o out.gbc  # Compile ke out.gbc" << std::endl;
            return 0;
        }
        else if (command == "-v" || command == "--version") {
            util.showVersion();
            return 0;
        }
        
        std::string inputFile;
        std::string outputFile;
        
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            
            if (arg == "-o" || arg == "--output") {
                if (i + 1 < argc) {
                    outputFile = argv[++i];
                } else {
                    CETAK_ERROR("Opsi -o membutuhkan parameter file output");
                    return 1;
                }
            }
            else if (arg[0] != '-') {
                inputFile = arg;
            }
        }
        
        if (inputFile.empty()) {
            CETAK_ERROR("File input diperlukan");
            return 1;
        }
        
        return util.compileFile(inputFile, outputFile) ? 0 : 1;
        
    } catch (const std::exception& e) {
        CETAK_ERROR("Error: " + std::string(e.what()));
        return 1;
    }
}