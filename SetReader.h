#ifndef SETREADER_H
#define SETREADER_H

#include <string>
#include <regex>
#include <filesystem>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <iterator>
#include <map>


class FabricType;


class Settings
{
public:
    Settings();

    bool read(const std::basic_string<char>& absoluteFilePath);

private:
    std::filesystem::path mAbsoluteFilePath;


    std::map<long long, std::tuple<std::string, long long, long long>> _c;
    bool parser_l1(const std::basic_string<char> &f, const std::basic_string<char> &pattern = "\\[(.*)\\]\\n");
    bool parser_l2(const std::basic_string<char> &f, const std::basic_string<char> & block_name, const std::basic_string<char> &pattern = "(.*)=(.*)\\n?");

    bool sub_block(std::basic_string<char>& _subblock, const std::basic_string<char> & block_name, const std::basic_string<char> &f);

    void example_settings();
    void print();

    // settings
    std::map<std::basic_string<char>, std::map<std::basic_string<char>,std::basic_string<char>>> d;       // d[group][name] = value

    friend class FabricType;
};

Settings::Settings()
{

}

bool Settings::read(const std::basic_string<char> &absoluteFilePath)
{
    mAbsoluteFilePath = std::filesystem::path(absoluteFilePath);
    if (!std::filesystem::exists(mAbsoluteFilePath)) {
        std::cout << "error. anavailable file: " << mAbsoluteFilePath << std::endl;
        return false;
    }

    std::basic_ifstream<char> istrm;
    istrm.open(mAbsoluteFilePath, std::ios_base::in | std::ios_base::ate);
    if (!istrm.is_open()) {
        std::cout << "file do not open: " << mAbsoluteFilePath << std::endl;
        return false;
    }

    auto _size = istrm.tellg();
    istrm.seekg(0, std::ios::beg);

    std::basic_string<char> _Out;
    _Out.resize(_size, 0x20);
    istrm.read(_Out.data(), _size);

    istrm.close();


    if (!parser_l1(_Out))                   // invalid l1 settings file
        return false;
    if (!parser_l2(_Out, "Data"))           // invalid l2 settings file
        return false;
    if (!parser_l2(_Out, "Processing"))     // invalid l2 settings file
        return false;

    return true;
}

bool Settings::parser_l1(const std::basic_string<char> &f, const std::basic_string<char> &pattern)
{
    std::basic_regex<char> rx(pattern);

    std::match_results<std::basic_string<char>::const_iterator> sm;
    std::match_results<std::basic_string<char>::const_iterator>::difference_type pos = 0;

    std::basic_string<char>::const_iterator begin = f.cbegin(), end = f.cend();

    int row_count = 0;
    while (std::regex_search(begin, end, sm, rx)) {

        _c[row_count] = { sm[1], sm.position(), sm[0].length() };       // [block_number] = { block_name, block_begin, block_size }

        ++row_count;
        pos = sm.position() + sm[0].length();
        begin = std::next(begin, pos);
    }

    if (_c.empty()) {
        std::cout << "Setttings file is empty." << std::endl;
        example_settings();
        return false;
    }

    if (auto it = std::find_if(_c.cbegin(), _c.cend(), [] (const std::pair<long long, std::tuple<std::string, long long, long long>>& p){ return std::get<0>(p.second) == "Data"; }); it == _c.cend()) {
        std::cerr << "no Data block in setting file, see: " << std::endl;
        example_settings();
        return false;
    }

    if (auto it = std::find_if(_c.cbegin(), _c.cend(), [] (const std::pair<long long, std::tuple<std::string, long long, long long>>& p){ return std::get<0>(p.second) == "Processing"; }); it == _c.cend()) {
        std::cerr << "no Processing block in setting file, see: " << std::endl;
        example_settings();
        return false;
    }

    return true;
}

bool Settings::parser_l2(const std::basic_string<char> &f, const std::basic_string<char> & block_name, const std::basic_string<char> &pattern)
{
    std::basic_string<char> sb;
    if (!sub_block(sb, block_name, f))
        return false;

    std::basic_string<char>::const_iterator begin = sb.cbegin(), end = sb.cend();

    std::match_results<std::basic_string<char>::const_iterator> sm;
    std::match_results<std::basic_string<char>::const_iterator>::difference_type pos = 0;

    bool ok = std::regex_search(begin, end, sm, std::basic_regex<char>(pattern));
    while (ok) {
        d[block_name][sm[1]] = sm[2];

        // next
        pos += sm[0].length();
        begin = std::next(sb.cbegin(), pos);

        // check
        ok = std::regex_search(begin, end, sm, std::basic_regex<char>(pattern));
    }


    // check data contains
    if (block_name == "Data") {
        if (!d["Data"].contains("type")) {
            std::cerr << "\nInvalid Data param in setting file, see: \"type\"" << std::endl;
            example_settings();
            return false;
        }
        if (!d["Data"].contains("data_in")) {
            std::cerr << "\nInvalid Data param in setting file, see: \"data_in\"" << std::endl;
            example_settings();
            return false;
        }
        if (!d["Data"].contains("data_out")) {
            std::cerr << "\nInvalid Data param in setting file, see: \"data_out\"" << std::endl;
            example_settings();
            return false;
        }
    }

    if (block_name == "Processing") {
        if (!d["Processing"].contains("multiplicator")) {
            std::cerr << "\nInvalid Processing param in setting file, see: \"multiplicator\"" << std::endl;
            example_settings();
            return false;
        }
        if (!d["Processing"].contains("logbase")) {
            std::cerr << "\nInvalid Processing param in setting file, see: \"logbase\"" << std::endl;
            example_settings();
            return false;
        }
        if (!d["Processing"].contains("power")) {
            std::cerr << "\nInvalid Processing param in setting file, see: \"power\"" << std::endl;
            example_settings();
            return false;
        }
        if (!d["Processing"].contains("average")) {
            std::cerr << "\nInvalid Processing param in setting file, see: \"average\"" << std::endl;
            example_settings();
            return false;
        }
    }

    return true;
}

bool Settings::sub_block(std::basic_string<char> &_Out, const std::basic_string<char> &block_name, const std::basic_string<char> &f)
{
    // [Block]
    auto it = std::find_if(_c.cbegin(), _c.cend(), [&block_name] (const std::pair<long long, std::tuple<std::string, long long, long long>>& p)
                           { return std::get<0>(p.second) == block_name; });

    // std::pair<const long long, std::tuple<std::string, long long, long long>> const* e = &(*it);

    long long n = 0;
    auto i = _c.cbegin();
    while (i != std::next(it)) {

        n += std::get<1>(i->second);
        n += std::get<2>(i->second);

        ++i;
    }

    // long long a = std::get<1>(it->second);    // begin block
    long long b = std::get<2>(it->second);    // size block


    std::basic_string<char>::const_iterator begin = std::next(f.cbegin(), n);       // a + b
    std::basic_string<char>::const_iterator end   = f.cend();

    if (auto y = std::next(it); y != _c.cend()) {
        // auto h = 1;
        end = std::next(f.cbegin(), std::get<1>(y->second) + std::get<2>(y->second) - b);
    }

    _Out = std::basic_string<char>(begin, end);

    return !_Out.empty();
}

void Settings::example_settings()
{
    std::cout << std::basic_string<char>(10,0x20) << "\nExample settings file content:" << std::endl;
    std::cout << std::basic_string<char>(15,0x20) << "[Data]" << std::endl
              << std::basic_string<char>(20,0x20) << "type=float" << std::endl
              << std::basic_string<char>(20,0x20) << "data_in=/path-to-data-file/filename.txt" << std::endl
              << std::basic_string<char>(20,0x20) << "data_out=/path-to-data-file/filename.txt" << std::endl
              << std::basic_string<char>(15,0x20) << "[Processing]" << std::endl
              << std::basic_string<char>(20,0x20) << "multiplicator=2.0" << std::endl
              << std::basic_string<char>(20,0x20) << "logbase=10" << std::endl
              << std::basic_string<char>(20,0x20) << "power=2.0" << std::endl
              << std::basic_string<char>(20,0x20) << "average=10" << std::endl
              << std::endl;
}

void Settings::print()
{
    std::cout << std::basic_string<char>(10,0x20) << "\nCurrent settings:" << std::endl;
    std::cout << std::basic_string<char>(15,0x20) << "[Data]" << std::endl
              << std::basic_string<char>(20,0x20) << "type=" << d.at("Data").at("type") << std::endl
              << std::basic_string<char>(20,0x20) << "data_in=" << d.at("Data").at("data_in") << std::endl
              << std::basic_string<char>(20,0x20) << "data_out=" << d.at("Data").at("data_out") << std::endl
              << std::basic_string<char>(15,0x20) << "[Processing]" << std::endl
              << std::basic_string<char>(20,0x20) << "multiplicator=" << d.at("Processing").at("multiplicator") << std::endl
              << std::basic_string<char>(20,0x20) << "logbase=" << d.at("Processing").at("logbase") << std::endl
              << std::basic_string<char>(20,0x20) << "power=" << d.at("Processing").at("power") << std::endl
              << std::basic_string<char>(20,0x20) << "average=" << d.at("Processing").at("average") << std::endl
              << std::endl;
}

#endif // SETREADER_H
