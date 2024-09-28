#ifndef UTILS_H
#define UTILS_H

#include <cstring>
#include <fstream>
#include "hftree.h"
#include <queue>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>
#include <stack>
#include <cmath>
#include <bitset>


namespace utils
{
    std::string getFileName(const std::string &fileName, const std::string &suffix)
    {
        size_t final_pos = fileName.find_last_of("/\\");
        size_t suffix_pos = fileName.find_last_of('.');
        return final_pos == std::string::npos ? "./output/" + fileName.substr(0, suffix_pos) + suffix : "./output/" + fileName.substr(final_pos + 1, suffix_pos - final_pos - 1) + suffix;
    }

    std::string parseText(const std::string &input)
    {
        std::string content;
        // TODO: Your code here
        std::ifstream file(input);
        if (file.is_open())
        {
            char c;
            while (file.get(c))
            {
                content += c;
            }
            file.close();
        }
        return content;
    }

    void output(const std::string &output, const std::string &data)
    {
        // TODO: Your code here
//        if (data.find('\n') != std::string::npos)
        std::string extension = output.substr(output.find_last_of(".") + 1);
        if (extension == "huffzip")
        {
//            std::cout << "data:" << data << std::endl;
//            std::cout << "!!" << std::endl;
            std::ofstream outputFile(output, std::ios::binary);
            if (outputFile.is_open())
            {
                std::string byteString;
                for (int i = 0; i < data.size(); i += 8)
                {
                    std::string byte = data.substr(i, 8);
                    uint8_t byteValue = 0;
                    for (int j = 0; j < 8; j++)
                    {
                        byteValue <<= 1;
                        if (byte[j] == '1')
                            byteValue |= 1;
                    }
                    outputFile.write(reinterpret_cast<const char *>(&byteValue), sizeof(byteValue));
                }
                outputFile.close();
            }
            else {
                std::cerr << "Failed to open output file" << std::endl;
            }
//            std::ofstream outputFile1("3.huffzip", std::ios::binary);
//            if (outputFile.is_open())
//            {
//                std::string byteString;
//                for (int i = 0; i < data.size(); i += 8)
//                {
//                    std::string byte = data.substr(i, 8);
//                    uint8_t byteValue = 0;
//                    for (int j = 0; j < 8; j++)
//                    {
//                        byteValue <<= 1;
//                        if (byte[j] == '1')
//                            byteValue |= 1;
//                    }
//                    outputFile.write(reinterpret_cast<const char *>(&byteValue), sizeof(byteValue));
//                }
//                outputFile.close();
//            }
//            else {
//                std::cerr << "Failed to open output file" << std::endl;
//            }
        }
        else {
//            std::cout << "???" << std::endl;
            std::ofstream file(output);
            if (file.is_open())
            {
//                std::cout << "hello" << std::endl;
                file.write(data.c_str(), data.size());
                file.close();
            }
        }
    }

    std::string codingTable2String(const std::map<std::string, std::string> &codingTable)
    {
        std::string result = "";
        // TODO: Your code here
        for (auto it = codingTable.begin(); it != codingTable.end(); it++)
        {
            result += it->first + " " + it->second + "\n";
        }
//        std::cout << "codingTable:" << std::endl;
//        for (auto it = codingTable.begin(); it != codingTable.end(); it++)
//        {
//            std::cout << it->first << ": " << it->second << std::endl;
//        }
//        std::cout << "result: " << result << std::endl;
        return result;
    }

    void loadCodingTable(const std::string &input, std::map<std::string, std::string> &codingTable)
    {
        // TODO: Your code here
        std::ifstream file(input);
        if (file.is_open())
        {
            char buffer[2];
            while (file.read(buffer, sizeof(buffer)))
            {
                if (file.peek() == '0' || file.peek() == '1')
                {
                    std::string key(buffer, 1); // 只取buffer的第一个字符作为key
                    std::string value;
                    std::string plus(buffer, 2);
                    value += plus;
                    std::getline(file, value); // 读取0或1后的所有字符作为value
                    codingTable[key] = value;
                }
                else if (file.peek() == ' ')
                {
                    file.ignore(1); // 忽略空格
                    std::string key(buffer, sizeof(buffer));
                    std::string value;
                    std::getline(file, value); // 读取空格后的所有字符作为value
                    codingTable[key] = value;
                }
            }
            file.close();
        }
//        std::cout << "codingTable:" << std::endl;
//        for (auto it = codingTable.begin(); it != codingTable.end(); it++)
//        {
//            std::cout << it->first << ": " << it->second << std::endl;
//        }
    }

    std::string compress(const std::map<std::string, std::string> &codingTable, const std::string &text)
    {
//        std::cout << "codingTable:" << std::endl;
//        for (auto it = codingTable.begin(); it != codingTable.end(); it++)
//        {
//            std::cout << it->first << ": " << it->second << std::endl;
//        }
//        std::cout << "text: " << text << std::endl;
        std::string result;
        // TODO: Your code here
        std::string bitstream;
        std::string currentCode;
        for (size_t i = 0; i < text.size(); ++i) {
//            std::cout << "text[i]: " << text[i] << std::endl;
//            std::cout << "i: " << i << std::endl;
//            currentCode += text[i];
//            std::cout << "currentCode:" << currentCode << std::endl;
//            if (codingTable.count(currentCode) > 0) {
//                bitstream += codingTable.at(currentCode);
//                currentCode.clear();
//                std::cout << "bitstream: " << bitstream << std::endl;
//            }
            if (i + 1 < text.size())
            {
                std::string doubleChar = text.substr(i, 2);
                if (codingTable.count(doubleChar) > 0)
                {
                    bitstream += codingTable.at(doubleChar);
//                    std::cout << "currentCode:" << doubleChar << " " << codingTable.at(doubleChar) << std::endl;
                    i += 1;
                    continue;
                }
            }
            // 如果两个字符的匹配失败，那么就匹配一个字符
            std::string singleChar = text.substr(i, 1);
            if (codingTable.count(singleChar) > 0)
            {
                bitstream += codingTable.at(singleChar);
//                std::cout << "currentCode:" << singleChar << " " << codingTable.at(singleChar) << std::endl;
                // i += 1;
            }
        }
//        if (codingTable.count(" i") > 0) {
//            std::cout<<"YESYESYES"<<std::endl;
//        }
//        if (codingTable.find(" i") != codingTable.end()) {
//            std::cout << "YESYESYES" << std::endl;
//        }
//        std::cout << "bitstream: " << bitstream << std::endl;
        uint64_t effectiveBits = bitstream.size();
//        std::cout << "effectiveBits: " << effectiveBits << std::endl;
        for (int i = 0; i < 8; ++i)
        {
            result += static_cast<char>((effectiveBits >> (i * 8)) & 0xFF);
        }
        while (bitstream.size() % 8 != 0) {
            bitstream += '0';
        }
        uint64_t decimal = 0;
        int len = bitstream.length();
        for (int i = 0; i < len; i++) {
            if (bitstream[i] == '1') {
                decimal += pow(2, len - i - 1);
            } else {
                decimal += 0;
            }
        }
        for (int i = bitstream.size()/8 - 1; i >= 0; i--)
        {
            result += static_cast<char>((decimal >> (i * 8)) & 0xFF);
        }

        std::string ans = "";
        std::stack<int> binaryEffectiveBits;
//        std::cout << "bitstream: " << bitstream << std::endl;
//        std::cout << "effectiveBits: " << effectiveBits << std::endl;
        while (effectiveBits > 0)
        {
            binaryEffectiveBits.push(effectiveBits % 2);
            effectiveBits /= 2;
        }
        std::string binaryStr = "";
        while (!binaryEffectiveBits.empty())
        {
            binaryStr += std::to_string(binaryEffectiveBits.top());
            binaryEffectiveBits.pop();
        }
//        std::cout << "binaryEE: " << binaryStr << std::endl;
        while (binaryStr.size() != 64)
        {
            binaryStr = "0" + binaryStr;
        }
//        std::cout << "binaryEE00: " << binaryStr << std::endl;
        std::string reverse = "";
        int jj = binaryStr.size() / 8;
        for (int i = 0; i < jj; i++)
        {
            reverse += binaryStr.substr(binaryStr.length() - 8);
            binaryStr.erase(binaryStr.length() - 8);
        }
//        std::cout << "reverse: " << reverse << std::endl;
        ans = reverse + bitstream;
//        std::cout << "ans: " << ans << std::endl;
//        std::string aniya = "";
//        for (int i = 0; i < ans.size(); i++){
//            aniya = static_cast<char>((ans >> i) & 0x1) + aniya;
//        }
//        std::cout << "aniya: " << aniya << std::endl;
//        std::ofstream outputFile1("3.huffzip", std::ios::binary);
//        if (outputFile1.is_open())
//        {
//            outputFile1.write(aniya.c_str(), aniya.size());
//            outputFile1.close();
//        }
//        std::ofstream outputFile("4.huffzip", std::ios::binary);
//        if (outputFile.is_open())
//        {
//            std::string byteString;
//            for (int i = 0; i < ans.size(); i += 8)
//            {
//                std::string byte = ans.substr(i, 8);
//                uint8_t byteValue = 0;
//                for (int j = 0; j < 8; j++)
//                {
//                    byteValue <<= 1;
//                    if (byte[j] == '1')
//                        byteValue |= 1;
//                }
//                outputFile.write(reinterpret_cast<const char *>(&byteValue), sizeof(byteValue));
//            }
//            outputFile.close();
//        }
        result = ans;
//        std::cout << "result: " << result << std::endl;
        return result;
    }
};

#endif