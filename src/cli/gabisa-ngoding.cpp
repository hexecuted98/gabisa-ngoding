/**
 * @file gabisa-ngoding.cpp
 * @brief Runner/Interpreter CLI untuk bahasa gabisa-ngoding
 * 
 * Usage: gabisa-ngoding [OPTIONS] [FILE]
 */

#include "utilitas/gabisa-ngoding.util.hpp"

int main(int argc, char* argv[]) {
    try {
        CETAK_INFO("=== GABISA-NGODING RUNNER ===");
        
        GabisaNgoding_Util util;
        
        if (argc == 1) {
            std::cout << "gabisa-ngoding interactive mode" << std::endl;
            std::cout << "Ketik 'quit' atau 'exit' untuk keluar" << std::endl;
            std::cout << "Ketik 'help' untuk bantuan" << std::endl;
            std::cout << std::endl;
            
            std::string input;
            while (true) {
                std::cout << "gabisa-ngoding@cli> ";
                std::getline(std::cin, input);
                
                if (input == "quit" || input == "exit") {
                    break;
                }
                else if (input == "help") {
                    std::cout << "Perintah interaktif:" << std::endl;
                    std::cout << "  run FILE      - Jalankan file" << std::endl;
                    std::cout << "  compile FILE  - Compile file" << std::endl;
                    std::cout << "  version       - Tampilkan versi" << std::endl;
                    std::cout << "  help          - Tampilkan bantuan" << std::endl;
                    std::cout << "  quit/exit     - Keluar" << std::endl;
                }
                else if (input == "version") {
                    util.showVersion();
                }
                else if (input.find("run ") == 0) {
                    std::string filename = input.substr(4);
                    util.runFile(filename);
                    std::cout << "\n";
                }
                else if (input.find("compile ") == 0) {
                    std::string filename = input.substr(8);
                    util.compileFile(filename);
                }
                else if (!input.empty()) {
                    std::cout << "Perintah tidak dikenal: " << input << std::endl;
                }
            }
            
            return 0;
        }
        
        return util.parseArguments(argc, argv);
        
    } catch (const std::exception& e) {
        CETAK_ERROR("Error: " + std::string(e.what()));
        return 1;
    }
}