#ifndef DATAREADER_H
#define DATAREADER_H

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <iterator>
#include <fstream>
#include <regex>
#include <map>
#include <limits>
#include <type_traits>
#include <typeinfo>
#include <optional>

#include "Proc.h"


template <typename Ty>
class Data
{
public:
    enum ErrorData : int
    {
        undef = -1,         // undef error
        match,              // match row error
        format,             // type format error
        invalid_argument,   // convert arg invalid
        out_of_range,       // out of type range
        runtime_error,      // common exception
    };

    Data(const std::string &absoluteFilePath);
    bool read();
    void calc(const std::string &multiplicator, const std::string &logbase, const std::string &power, const std::string &average);

private:
    std::vector<Ty> mData;
    const std::filesystem::path mAbsoluteFilePath;
    Processing<Ty> *mProc;


    bool exists() const;

    bool read(std::basic_string<char> &_Out);

    // parsers
    bool parser(std::basic_string<char>::const_iterator begin, std::basic_string<char>::const_iterator end);
    std::optional<std::pair<int, std::basic_string<char>>> parser_type(const std::basic_string<char> &_Value);
    std::basic_string<char> type_pattern() const;

    // covert
    std::optional<std::pair<int, std::basic_string<char>>> stonum(Ty& _Data, const std::basic_string<char> &_Str);
    std::optional<Ty> stonum(const std::basic_string<char> &_Str);

    // errors
    std::map<size_t, std::pair<int, std::basic_string<char>>> eMsg;       // [row] = {error_id, error_msg}
    std::basic_string<char> error_str(ErrorData e) const;
    void print_error() const;

    Data() = delete;
    Data(const Data&) = delete;
    Data &operator=(const Data&) = delete;
    Data(const Data&&) = delete;
    Data &operator=(const Data&&) = delete;
};



template<typename Ty>
std::basic_string<char> Data<Ty>::error_str(ErrorData e) const
{
    std::string er_str;

    switch (e) {
    case match:
        er_str = "match";
        break;
    case format:
        er_str = "format";
        break;
    case invalid_argument:
        er_str = "invalid_argument";
        break;
    case out_of_range:
        er_str = "out_of_range";
        break;
    case runtime_error:
        er_str = "runtime_error";
        break;
    default:
        er_str = "undef";
        break;
    }

    std::basic_string<char> s(20, 0x20);
    std::copy(er_str.cbegin(), er_str.cend(), std::next(s.begin(), 3));
    return s;
}

template<typename Ty>
void Data<Ty>::print_error() const
{
    if (eMsg.empty())
        return;

    std::cout << std::string(5, 0x20) << "File reading error:  " << mAbsoluteFilePath << std::endl;
              // <<  "fix incorrect data in file: " << mAbsoluteFilePath.generic_string() << ".  " << ;


    for (auto const& v : eMsg) {
        std::cout << std::string(10, 0x20) << "row: "  << std::right << std::setw(5) << v.first << "     "
                  << std::left << " error:" <<  error_str((ErrorData)v.second.first) << "     " << v.second.second
                  << " " << std::endl;
    }
    std::cout << std::endl << std::endl  << std::string(5, 0x20) << "Correct the Data and Restart the Programm !!!" << std::endl;
}

template<typename Ty>
Data<Ty>::Data(const std::string &absoluteFilePath)
    : mAbsoluteFilePath(absoluteFilePath)
    , mProc(new Processing<Ty>(mData))
{
    // std::cout << "read of Data with type[" <<  typeid(Ty).hash_code() << "]: " << std::string(typeid(Ty).name()) << std::endl;
}

template<typename Ty>
bool Data<Ty>::read()
{
    if (!exists())
        return false;

    // read file as utf-8 char type
    std::basic_string<char> _Out;
    if (!read(_Out))
        return false;

    // parsing file
    return parser(_Out.cbegin(), _Out.cend());
}

template<typename Ty>
void Data<Ty>::calc(const std::string &multiplicator, const std::string &logbase, const std::string &power, const std::string &average)
{
    bool ok = true;
    int row_count = 0;

    Ty _multiplicator;
    if (auto r = parser_type(multiplicator); r.has_value()) {    // check type pattern
        eMsg[row_count] = r.value();
        ++row_count;
    }
    if (auto r = stonum(_multiplicator, multiplicator); r.has_value()) {
        eMsg[row_count] = r.value();
        ++row_count;
        ok = false;
    }

    Ty _logbase;
    if (auto r = parser_type(logbase); r.has_value()) {    // check type pattern
        eMsg[row_count] = r.value();
        ++row_count;
        ok = false;
    }
    if (auto r = stonum(_logbase, logbase); r.has_value()) {
        eMsg[row_count] = r.value();
        ++row_count;
        ok = false;
    }

    Ty _power;
    if (auto r = parser_type(power); r.has_value()) {    // check type pattern
        eMsg[row_count] = r.value();
        ++row_count;
        ok = false;
    }
    if (auto r = stonum(_power, power); r.has_value()) {
        eMsg[row_count] = r.value();
        ++row_count;
        ok = false;
    }
    size_t _average = 0ull;
    try {
        _average = std::stoull(average);
    }
    catch(std::invalid_argument const& ex) {
        std::cout << typeid(*this).name() << "(" << this << ") average: invalid_argument" << std::endl;
    }
    catch(std::out_of_range const& ex) {
        std::cout << typeid(*this).name() << "(" << this << ") average: out_of_range" << std::endl;
    }

    print_error();

    if (eMsg.empty()) {
        mProc->run(_multiplicator, _logbase, _power, _average);
    }
}

template<typename Ty>
bool Data<Ty>::exists() const
{
    // file exist
    if (!std::filesystem::exists(mAbsoluteFilePath)) {
        std::cout << "file not exist: " << mAbsoluteFilePath.generic_string() << std::endl;
        return false;
    }
    return true;
}

template<typename Ty>
bool Data<Ty>::read(std::basic_string<char> &_Out)
{
    _Out.clear();

    std::basic_ifstream<char> istrm;
    istrm.open(mAbsoluteFilePath, std::ios_base::in | std::ios_base::ate);
    if (!istrm.is_open()) {
        std::cout << "file do not open: " << mAbsoluteFilePath.generic_string() << std::endl;
        return false;
    }

    auto _size = istrm.tellg();
    istrm.seekg(0, std::ios::beg);

    _Out.resize(_size, 0x20);
    istrm.read(_Out.data(), _size);

    istrm.close();

    if (_Out.empty()) {
        std::cout << "file is empty: " << mAbsoluteFilePath.generic_string() << std::endl;
        return false;
    }

    return true;
}

template<typename Ty>
bool Data<Ty>::parser(std::basic_string<char>::const_iterator begin, std::basic_string<char>::const_iterator end)
{
    std::basic_regex<char> rx("(.*)\\n");
    std::match_results<std::basic_string<char>::const_iterator> sm;

    size_t row_count = 0;
    Ty _Data;

    while (std::regex_search(begin, end, sm, rx)) {

        // next row
        ++row_count;
        begin = std::next(begin, sm[0].length());

        // get row
        std::basic_string<char> mRow = sm[1];       // row data
        if (mRow.empty()) {                         // skip empty row
            continue;
        }

        // trim
        mRow.erase(mRow.find_last_not_of(0x20)+1);
        mRow.erase(0, mRow.find_first_not_of(0x20));

        // check type pattern
        if (auto r = parser_type(mRow); r.has_value()) {
            eMsg[row_count] = r.value();
            continue;
        }

        // convert to number
        if (auto r = stonum(_Data, mRow); r.has_value()) {
            eMsg[row_count] = r.value();
        } else {
            mData.push_back(_Data);         // add data to container
        }

    }

    print_error();

    return eMsg.empty();
}

template<typename Ty>
std::optional<std::pair<int, std::basic_string<char>>> Data<Ty>::parser_type(const std::basic_string<char> &_Value)
{
    std::basic_regex<char> _rex(type_pattern());
    std::match_results<std::basic_string<char>::const_iterator> sm;
    std::basic_string<char>::const_iterator begin = _Value.cbegin(), end = _Value.end();

    // ======================== (sucsess metric's) ============================

    // 1. no match by pattern
    if (!std::regex_search(begin, end, sm, _rex)) {
        return std::pair<int, std::basic_string<char>>(match, "no match data");
    }

    // 2. more then One number (multi Type in single row)
    if (std::next(begin, sm[0].length()) != end) {
        return std::pair<int, std::basic_string<char>>(format, std::basic_string<char>("Change value:   ") + "\"" + _Value + "\"");
    }

    // 3. sucsess
    return std::optional<std::pair<int, std::basic_string<char>>>();
}

template<typename Ty>
std::basic_string<char> Data<Ty>::type_pattern() const
{
    // short
    if (std::is_same<Ty, short>::value)
        return "([+-]?[0-9]*)";
    // unsigned short
    if (std::is_same<Ty, unsigned short>::value)
        return "([+-]?[0-9]*)";

    if (std::is_same<Ty, long>::value || std::is_same<Ty, int>::value)
        return "([+-]?[0-9]*)";
    if (std::is_same<Ty, long long>::value || std::is_same<Ty, long int>::value)
        return "([+-]?[0-9]*)";
    if (std::is_same<Ty, unsigned long>::value || std::is_same<Ty, unsigned int>::value)
        return "([+]?[0-9]*)";
    if (std::is_same<Ty, unsigned long long>::value || std::is_same<Ty, unsigned long int>::value)
        return "([+]?[0-9]*)";

    if (std::is_same<Ty, float>::value)
        return "[+-]?[0-9]*\\.?[0-9]*";
    if (std::is_same<Ty, double>::value)
        return "[+-]?[0-9]*\\.?[0-9]*";
    if (std::is_same<Ty, long double>::value)
        return "[+-]?[0-9]*\\.?[0-9]*";

    std::cout << std::left << std::setw(15) << __func__ << "     undef_type: " << std::setw(10) << std::right << typeid(Ty).name()/* << ".        Set def_type <int>."*/ << std::endl;
    return "([+-]?[0-9]*)";
}

template<typename Ty>
std::optional<std::pair<int, std::basic_string<char>>> Data<Ty>::stonum(Ty &_Data, const std::basic_string<char> &_Str)
{
    try{
        std::optional<Ty> v = stonum(_Str);
        if (v.has_value()) {
            _Data = v.value();
            return std::optional<std::pair<int, std::basic_string<char>>>();
        } else {
            return std::make_pair(out_of_range, std::basic_string<char>("Change value:   \"") + _Str + "\" to range [" + std::to_string(std::numeric_limits<Ty>::min()) + ", " + std::to_string(std::numeric_limits<Ty>::max()) + "]");
        }
    }
    catch(std::invalid_argument const& ex) {
        return std::make_pair(invalid_argument, std::basic_string<char>("Change value:   ") + _Str);
    }
    catch(std::out_of_range const& ex) {
        return std::make_pair(out_of_range, std::basic_string<char>("Change value:   \"") + _Str + "\" to range [" + std::to_string(std::numeric_limits<Ty>::min()) + ", " + std::to_string(std::numeric_limits<Ty>::max()) + "]");
    }
}

template<typename Ty>
std::optional<Ty> Data<Ty>::stonum(const std::basic_string<char> &_Str)
{
    if (std::is_same<Ty, short>::value) {
        int value = std::stoi(_Str);
        return (value > static_cast<int>(std::numeric_limits<Ty>::min()) - 1 && value < static_cast<int>(std::numeric_limits<Ty>::max()) + 1)
                   ? std::optional<Ty>(static_cast<short>(value))
                   : std::optional<Ty>()        // out_of_range
        ;
    }
    if (std::is_same<Ty, unsigned short>::value) {
        unsigned long value = std::stoul(_Str);
        return (value > (static_cast<unsigned long>(std::numeric_limits<Ty>::min()) - 1ul) &&
                value < (static_cast<unsigned long>(std::numeric_limits<Ty>::max()) + 1))
                   ? std::optional<Ty>(static_cast<unsigned short>(value))
                   : std::optional<Ty>()        // out_of_range
        ;

    }

    if (std::is_same<Ty, int>::value)
        return std::stoi(_Str);
    if (std::is_same<Ty, long>::value)
        return std::stol(_Str);
    if (std::is_same<Ty, long long>::value || std::is_same<Ty, long int>::value)
        return std::stoll(_Str);
    if (std::is_same<Ty, unsigned long>::value || std::is_same<Ty, unsigned int>::value)
        return std::stoul(_Str);
    if (std::is_same<Ty, unsigned long long>::value || std::is_same<Ty, unsigned long int>::value)
        return std::stoull(_Str);
    if (std::is_same<Ty, float>::value)
        return std::stof(_Str);
    if (std::is_same<Ty, double>::value)
        return std::stod(_Str);
    if (std::is_same<Ty, long double>::value)
        return std::stold(_Str);
    ;

    std::cout << std::left << std::setw(15) << __func__ << "     undef_type: " << std::setw(10) << std::right << typeid(Ty).name()/* << ".        Set def_type <int>."*/ << std::endl;
    return std::stoi(_Str);
}

#endif // DATAREADER_H
