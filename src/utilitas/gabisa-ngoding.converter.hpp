/**
 * @file gabisa-ngoding.translator.hpp
 * @brief Alat penerjemah untuk bahasa gabisa-ngoding
 *
 * @authors adjidev, espacioo
 */

#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include "../helper/helper.debug.hpp"

/* ---------- KOMPATIBILITAS ---------- */
#if __cplusplus < 201703L
#error "Maaf header bahasa gabisa-ngoding minimal butuh compiler C++17 Ke atas."
#endif

#ifndef __cplusplus
#error "Maaf header ini hanya mendukung C++ tidak mendukung C pastikan kamu mengkompilasikan pakai C++ compiler."
#endif

// MARK: BRAINFUCK TO GABISA-NGODING
inline void brainfuckToGabisaNgoding(const std::string &sumber)
{
    CETAK_INFO("Memulai terjemahan Brainfuck -> gabisa-ngoding...");

    std::unordered_map<char, std::string> translator = {
        {'>', "AKU"},
        {'<', "PUH"},
        {'+', "GABISA"},
        {'-', "NGODING"},
        {'.', "AJARIN"},
        {',', "DONG"},
        {'[', "HAI"},
        {']', "SEPUH"}};

    std::stringstream hasil;
    size_t lineLength = 0;
    size_t totalInstructions = 0;

    for (char c : sumber)
    {
        if (translator.find(c) != translator.end())
        {
            std::string token = translator[c];
            hasil << token << " ";
            lineLength += token.length() + 1;
            totalInstructions++;

            if (lineLength > 80)
            {
                hasil << "\n";
                lineLength = 0;
            }
        }
    }

    std::cout << "\n=== HASIL TERJEMAHAN Brainfuck -> gabisa-ngoding ===" << std::endl;
    std::cout << "Total instruksi: " << totalInstructions << std::endl;
    std::cout << "Kode gabisa-ngoding:\n"
              << std::endl;
    std::cout << hasil.str() << std::endl;
}

// MARK: STRING TO GABISA-NGODING
inline void stringToGabisaNgoding(const std::string &sumber)
{
    CETAK_INFO("Memulai terjemahan String -> gabisa-ngoding...");

    if (sumber.empty())
    {
        CETAK_WARN("String sumber kosong!");
        return;
    }

    std::stringstream hasil;
    hasil << "; Kode untuk output: \"" << sumber << "\"\n";

    int currentValue = 0;

    for (char c : sumber)
    {
        int targetValue = static_cast<unsigned char>(c);
        int difference = targetValue - currentValue;

        if (difference == 0)
        {
            hasil << "AJARIN ; '" << c << "' (ASCII " << targetValue << ")\n";
            continue;
        }

        if (std::abs(difference) <= 128)
        {
            if (difference > 0)
            {
                hasil << "; Set ke '" << c << "' (ASCII " << targetValue << ") dari " << currentValue << "\n";
                for (int i = 0; i < difference; i++)
                {
                    hasil << "GABISA ";
                }
                hasil << "AJARIN\n";
            }
            else
            {
                hasil << "; Set ke '" << c << "' (ASCII " << targetValue << ") dari " << currentValue << "\n";
                for (int i = 0; i < -difference; i++)
                {
                    hasil << "NGODING ";
                }
                hasil << "AJARIN\n";
            }
        }
        else
        {
            hasil << "; Reset dan set ke '" << c << "' (ASCII " << targetValue << ")\n";
            if (currentValue != 0)
            {
                for (int i = 0; i < currentValue; i++)
                {
                    hasil << "NGODING ";
                }
                hasil << "\n";
            }
            for (int i = 0; i < targetValue; i++)
            {
                hasil << "GABISA ";
            }
            hasil << "AJARIN\n";
        }

        currentValue = targetValue;
    }

    std::cout << "\n=== HASIL TERJEMAHAN String -> gabisa-ngoding ===" << std::endl;
    std::cout << "String: \"" << sumber << "\" (" << sumber.length() << " karakter)" << std::endl;
    std::cout << "Kode gabisa-ngoding:\n"
              << std::endl;
    std::cout << hasil.str() << std::endl;
}

// MARK: GABISA-NGODING TO BRAINFUCK
inline void gabisaNgodingToBrainfuck(const std::string &sumber)
{
    CETAK_INFO("Memulai terjemahan gabisa-ngoding -> Brainfuck...");

    std::unordered_map<std::string, char> translator = {
        {"AKU", '>'},
        {"PUH", '<'},
        {"GABISA", '+'},
        {"NGODING", '-'},
        {"AJARIN", '.'},
        {"DONG", ','},
        {"HAI", '['},
        {"SEPUH", ']'}};

    std::stringstream hasil;
    std::stringstream currentWord;
    size_t totalInstructions = 0;
    size_t lineLength = 0;
    bool inComment = false;

    for (char c : sumber)
    {
        if (c == ';')
        {
            inComment = true;
            continue;
        }

        if (c == '\n')
        {
            inComment = false;
            continue;
        }

        if (inComment)
        {
            continue;
        }

        if (std::isspace(c))
        {
            std::string word = currentWord.str();
            if (!word.empty())
            {
                if (translator.find(word) != translator.end())
                {
                    char bfChar = translator[word];
                    hasil << bfChar;
                    lineLength++;
                    totalInstructions++;

                    if (lineLength >= 80)
                    {
                        hasil << "\n";
                        lineLength = 0;
                    }
                }
                currentWord.str("");
            }
        }
        else
        {
            currentWord << c;
        }
    }

    std::string lastWord = currentWord.str();
    if (!lastWord.empty() && translator.find(lastWord) != translator.end())
    {
        char bfChar = translator[lastWord];
        hasil << bfChar;
        totalInstructions++;
    }

    std::cout << "\n=== HASIL TERJEMAHAN gabisa-ngoding -> Brainfuck ===" << std::endl;
    std::cout << "Total instruksi: " << totalInstructions << std::endl;
    std::cout << "Kode Brainfuck:\n"
              << std::endl;
    std::cout << hasil.str() << std::endl;
}

// MARK: BEAUTIFY
inline void beautifyGabisaNgoding(const std::string &sumber)
{
    CETAK_INFO("Memulai beautify kode gabisa-ngoding...");

    std::unordered_map<std::string, std::string> explanations = {
        {"AKU", "pointer++"},
        {"PUH", "pointer--"},
        {"GABISA", "byte++"},
        {"NGODING", "byte--"},
        {"AJARIN", "output"},
        {"DONG", "input"},
        {"HAI", "loop start"},
        {"SEPUH", "loop end"}};

    std::stringstream hasil;
    std::stringstream currentWord;
    size_t indentLevel = 0;
    bool inComment = false;
    size_t lineNumber = 1;

    hasil << "1: ";

    for (char c : sumber)
    {
        if (c == ';')
        {
            inComment = true;
            hasil << ";";
            continue;
        }

        if (c == '\n')
        {
            if (inComment)
            {
                inComment = false;
            }
            lineNumber++;
            hasil << "\n"
                  << lineNumber << ": ";
            continue;
        }

        if (inComment)
        {
            hasil << c;
            continue;
        }

        if (std::isspace(c))
        {
            std::string word = currentWord.str();
            if (!word.empty())
            {
                if (word == "SEPUH")
                {
                    indentLevel = (indentLevel > 0) ? indentLevel - 1 : 0;
                }

                if (word == "HAI" || word == "SEPUH")
                {
                    hasil << std::string(indentLevel * 2, ' ');
                }

                hasil << word;

                if (explanations.find(word) != explanations.end())
                {
                    hasil << " ; " << explanations[word];
                }

                hasil << " ";

                if (word == "HAI")
                {
                    indentLevel++;
                }

                currentWord.str("");
            }
            else
            {
                hasil << " ";
            }
        }
        else
        {
            currentWord << c;
        }
    }

    std::string lastWord = currentWord.str();
    if (!lastWord.empty())
    {
        if (explanations.find(lastWord) != explanations.end())
        {
            hasil << lastWord << " ; " << explanations[lastWord];
        }
        else
        {
            hasil << lastWord;
        }
    }

    std::cout << "\n=== BEAUTIFY gabisa-ngoding ===" << std::endl;
    std::cout << "Kode yang diformat:\n"
              << std::endl;
    std::cout << hasil.str() << std::endl;
}

// MARK: ANALYZER
inline void analyzeGabisaNgoding(const std::string &sumber)
{
    CETAK_INFO("Memulai analisis kode gabisa-ngoding...");

    std::unordered_map<std::string, std::string> tokenNames = {
        {"AKU", "Pointer Right (>)"},
        {"PUH", "Pointer Left (<)"},
        {"GABISA", "Increment (+)"},
        {"NGODING", "Decrement (-)"},
        {"AJARIN", "Output (.)"},
        {"DONG", "Input (,)"},
        {"HAI", "Loop Start ([)"},
        {"SEPUH", "Loop End (])"}};

    std::unordered_map<std::string, int> tokenCount;
    std::stringstream currentWord;
    bool inComment = false;
    int totalTokens = 0;
    int loopDepth = 0;
    int maxLoopDepth = 0;

    for (char c : sumber)
    {
        if (c == ';')
        {
            inComment = true;
            continue;
        }

        if (c == '\n')
        {
            inComment = false;
            continue;
        }

        if (inComment)
        {
            continue;
        }

        if (std::isspace(c))
        {
            std::string word = currentWord.str();
            if (!word.empty())
            {
                if (tokenNames.find(word) != tokenNames.end())
                {
                    tokenCount[word]++;
                    totalTokens++;

                    if (word == "HAI")
                    {
                        loopDepth++;
                        if (loopDepth > maxLoopDepth)
                        {
                            maxLoopDepth = loopDepth;
                        }
                    }
                    else if (word == "SEPUH")
                    {
                        loopDepth--;
                    }
                }
                currentWord.str("");
            }
        }
        else
        {
            currentWord << c;
        }
    }

    std::string lastWord = currentWord.str();
    if (!lastWord.empty() && tokenNames.find(lastWord) != tokenNames.end())
    {
        tokenCount[lastWord]++;
        totalTokens++;
    }

    std::cout << "\n=== ANALISIS KODE gabisa-ngoding ===" << std::endl;
    std::cout << "Total token: " << totalTokens << std::endl;
    std::cout << "Kedalaman loop maksimum: " << maxLoopDepth << std::endl;
    std::cout << "\nDistribusi Token:" << std::endl;

    for (const auto &[token, count] : tokenCount)
    {
        std::cout << "  " << tokenNames[token] << ": " << count
                  << " (" << (count * 100.0 / totalTokens) << "%)" << std::endl;
    }

    int loopBalance = (tokenCount["HAI"] - tokenCount["SEPUH"]);
    if (loopBalance != 0)
    {
        CETAK_ERROR("Loop tidak seimbang! HAI: " + std::to_string(tokenCount["HAI"]) +
                    ", SEPUH: " + std::to_string(tokenCount["SEPUH"]));
    }
    else
    {
        CETAK_INFO("Loop seimbang ✓");
    }
}