/**
 * @file gabisa-ngoding.cpp
 * @brief Runner/Interpreter CLI untuk bahasa gabisa-ngoding
 *
 * Usage: gabisa-ngoding [OPTIONS] [FILE]
 */

#include "../utilitas/gabisa-ngoding.util.hpp"
#include "../utilitas/gabisa-ngoding.converter.hpp"

int main(int argc, char* argv[]) {
    try {
        CETAK_INFO("=== GABISA-NGODING RUNNER ===");

        GabisaNgoding_Util util;

        if (argc == 1) {
            std::cout << "gabisa-ngoding interactive mode" << std::endl;
            std::cout << "github : github.com/hexecuted98" << std::endl;
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
                    std::cout << "  run FILE            - Jalankan file" << std::endl;
                    std::cout << "  compile FILE        - Compile file" << std::endl;
                    std::cout << "  version             - Tampilkan versi" << std::endl;
                    std::cout << "  string [TEXT]       - Konversi string ke gabisa-ngoding" << std::endl;
                    std::cout << "  bf-to-gng [CODE]    - Brainfuck ke gabisa-ngoding" << std::endl;
                    std::cout << "  help                - Tampilkan bantuan ini" << std::endl;
                    std::cout << "  quit/exit           - Keluar" << std::endl;
                }
                else if (input == "version") {
                    util.showVersion();
                }
                else if (input.rfind("run ", 0) == 0) {
                    std::string filename = input.substr(4);
                    util.runFile(filename);
                }
                else if (input.rfind("compile ", 0) == 0) {
                    std::string filename = input.substr(8);
                    util.compileFile(filename);
                }
                else if (input.rfind("string ", 0) == 0) {
                    std::string str = input.substr(7);
                    if (str.empty()) {
                        std::cout << "Masukkan teks yang ingin dikonversi:" << std::endl;
                        std::getline(std::cin, str);
                    }
                    stringToGabisaNgoding(str);
                }
                else if (input.rfind("bf-to-gng ", 0) == 0) {
                    std::string bfInput = input.substr(10);
                    if (bfInput.empty()) {
                        std::cout << "Masukkan kode Brainfuck yang ingin dikonversi:" << std::endl;
                        std::getline(std::cin, bfInput);
                    }
                    brainfuckToGabisaNgoding(bfInput);
                }
                else if (!input.empty()) {
                    std::cout << "Perintah tidak dikenal: " << input << std::endl;
                    std::cout << "Ketik 'help' untuk daftar perintah." << std::endl;
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
