#ifndef FABRICTYPE_H
#define FABRICTYPE_H

#include "DataReader.h"

#include <bits/types.h>

class FabricType
{
public:
    enum T : size_t
    {
        it_short,
        it_unsigned_short,
        it_int,
        it_unsigned_int,
        it_long,
        it_unsigned_long,
        it_long_long,
        it_unsigned_long_long,
        it_int16_t,
        it_uint16_t,
        it_int32_t,
        it_uint32_t,
        it_int64_t,
        it_uint64_t,
        it_float,
        it_double,
    };

    FabricType(const std::basic_string<char> &absoluteFilePath);
    ~FabricType();

    void run(const std::basic_string<char> &type_name);

private:
    const std::basic_string<char> mAbsoluteFilePath;
    std::basic_string<char> mType;
    std::map<std::basic_string<char>, size_t> mTypes;
    bool check_type(const std::basic_string<char>& name);
};




FabricType::FabricType(const std::basic_string<char> &absoluteFilePath)
    : mAbsoluteFilePath(absoluteFilePath), mType("")
{
    mTypes["short"]             = it_short;
    mTypes["unsigned short"]    = it_unsigned_short;

    mTypes["int"]               = it_int;
    mTypes["unsigned int"]      = it_unsigned_int;

    mTypes["long"]              = it_long;
    mTypes["unsigned long"]     = it_unsigned_long;

    mTypes["long long"]         = it_long_long;
    mTypes["unsigned long long"]= it_unsigned_long_long;


    mTypes["int16_t"]           = it_int16_t;
    mTypes["uint16_t"]          = it_uint16_t;

    mTypes["int32_t"]           = it_int32_t;
    mTypes["uint32_t"]          = it_uint32_t;

    mTypes["int64_t"]           = it_int64_t;
    mTypes["uint64_t"]          = it_uint64_t;


    mTypes["float"]             = it_float;
    mTypes["double"]            = it_double;

}

FabricType::~FabricType()
{
    std::cout << "\n" << this << "Finished ...";
}

void FabricType::run(const std::basic_string<char> &type_name)
{
    std::cout << "\n" << typeid(*this).name() << "(" << this << ", " << type_name << ")" << "  running ... "<< std::string(50, '+') <<  "\n\n";

    if (!check_type(type_name))
        return;

    // bool ok = false;

    switch (mTypes.at(mType)) {

    case it_short: {
        Data<short> d(mAbsoluteFilePath);
        if (d.read())
            d.calc();
    }
    break;
    case it_unsigned_short:
    {
        Data<unsigned short> d(mAbsoluteFilePath);
        if (d.read())
            d.calc();
    }
    break;

    case it_int:
    {
        Data<int> d(mAbsoluteFilePath);
        if (d.read())
            d.calc();
    }
    break;
    case it_unsigned_int:
    {
        Data<unsigned int> d(mAbsoluteFilePath);
        if (d.read())
            d.calc();
    }
    break;


    case it_long:
    {
        Data<long> d(mAbsoluteFilePath);
        if (d.read())
            d.calc();
    }
    break;
    case it_unsigned_long:
    {
        Data<unsigned long> d(mAbsoluteFilePath);
        if (d.read())
            d.calc();
    }
    break;

    case it_long_long:
    {
        Data<long long> d(mAbsoluteFilePath);
        if (d.read())
            d.calc();
    }
    break;
    case it_unsigned_long_long:
    {
        Data<unsigned long long> d(mAbsoluteFilePath);
        if (d.read())
            d.calc();
    }
    break;


    case it_int16_t:
    {
        Data<int16_t> d(mAbsoluteFilePath);
        if (d.read())
            d.calc();
    }
    break;
    case it_uint16_t:
    {
        Data<uint16_t> d(mAbsoluteFilePath);
        if (d.read())
            d.calc();
    }
    break;


    case it_int32_t:
    {
        Data<int32_t> d(mAbsoluteFilePath);
        if (d.read())
            d.calc();
    }
    break;
    case it_uint32_t:
    {
        Data<uint32_t> d(mAbsoluteFilePath);
        if (d.read())
            d.calc();
    }
    break;


    case it_int64_t:
    {
        Data<int64_t> d(mAbsoluteFilePath);
        if (d.read())
            d.calc();
    }
    break;
    case it_uint64_t:
    {
        Data<uint64_t> d(mAbsoluteFilePath);
        if (d.read())
            d.calc();
    }
    break;


    case it_float:
    {
        Data<float> d(mAbsoluteFilePath);
        if (d.read())
            d.calc();
    }
    break;
    case it_double:
    {
        Data<double> d(mAbsoluteFilePath);
        if (d.read())
            d.calc();
    }
    break;


    default:
        break;
    }

    // exit
    std::cout << "\n" << typeid(*this).name() << "(" << this << ", " << type_name << ")" << "  finished ... " << std::string(50, '+') << "\n\n\n\n";
}

bool FabricType::check_type(const std::basic_string<char>& name)
{
    if (mTypes.contains(name)) {
        mType = name;
        return true;
    }


    std::cout << std::string(5, 0x20) << "Unavailable name of data type: \"" << name << "\"." << std::endl
              << std::string(10, 0x20) << "available name types: " << std::endl;
    for (auto const& t : mTypes)
        std::cout << std::string(15, 0x20) << t.first << std::endl;
    std::cout << std::endl;

    std::cout << "\n" << typeid(*this).name() << "(" << this << ", " << name << ")" << "  finished ... " << std::string(50, '+') << "\n\n\n\n";

    return false;
}

#endif // FABRICTYPE_H
