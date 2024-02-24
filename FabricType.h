#ifndef FABRICTYPE_H
#define FABRICTYPE_H

#include "DataReader.h"

#include "SetReader.h"
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

    void run();

private:
    const std::basic_string<char> mAbsoluteFilePath;        // path-to-ini-file
    Settings mSetting;                                      // ini file

    void create_types();



    std::basic_string<char> mType;
    std::map<std::basic_string<char>, size_t> mTypes;
    bool check_type(const std::basic_string<char>& name);
};




FabricType::FabricType(const std::basic_string<char> &absoluteFilePath)
    : mAbsoluteFilePath(absoluteFilePath), mType("")
{
    create_types();
}

FabricType::~FabricType()
{
    std::cout << "\n" << this << "Finished ...";
}

void FabricType::run()
{
    if (!mSetting.read(mAbsoluteFilePath))
        return;

    auto type_name = mSetting.d["Data"]["type"];
    auto data_path = mSetting.d["Data"]["data_path"];

    std::cout << "\n" << typeid(*this).name() << "(" << this << ", " << type_name << ")" << "  running ... "<< std::string(50, '+') <<  "\n\n";

    if (!check_type(type_name))
        return;

    mSetting.print();

    switch (mTypes.at(mType)) {

    case it_short: {
        Data<short> d(data_path);
        if (d.read())
            d.calc(mSetting.d["Processing"]["multiplicator"], mSetting.d["Processing"]["logbase"], mSetting.d["Processing"]["power"], mSetting.d["Processing"]["average"]);
    }
    break;
    case it_unsigned_short:
    {
        Data<unsigned short> d(data_path);
        if (d.read())
            d.calc(mSetting.d["Processing"]["multiplicator"], mSetting.d["Processing"]["logbase"], mSetting.d["Processing"]["power"], mSetting.d["Processing"]["average"]);
    }
    break;

    case it_int:
    {
        Data<int> d(data_path);
        if (d.read())
            d.calc(mSetting.d["Processing"]["multiplicator"], mSetting.d["Processing"]["logbase"], mSetting.d["Processing"]["power"], mSetting.d["Processing"]["average"]);
    }
    break;
    case it_unsigned_int:
    {
        Data<unsigned int> d(data_path);
        if (d.read())
            d.calc(mSetting.d["Processing"]["multiplicator"], mSetting.d["Processing"]["logbase"], mSetting.d["Processing"]["power"], mSetting.d["Processing"]["average"]);
    }
    break;


    case it_long:
    {
        Data<long> d(data_path);
        if (d.read())
            d.calc(mSetting.d["Processing"]["multiplicator"], mSetting.d["Processing"]["logbase"], mSetting.d["Processing"]["power"], mSetting.d["Processing"]["average"]);
    }
    break;
    case it_unsigned_long:
    {
        Data<unsigned long> d(data_path);
        if (d.read())
            d.calc(mSetting.d["Processing"]["multiplicator"], mSetting.d["Processing"]["logbase"], mSetting.d["Processing"]["power"], mSetting.d["Processing"]["average"]);
    }
    break;

    case it_long_long:
    {
        Data<long long> d(data_path);
        if (d.read())
            d.calc(mSetting.d["Processing"]["multiplicator"], mSetting.d["Processing"]["logbase"], mSetting.d["Processing"]["power"], mSetting.d["Processing"]["average"]);
    }
    break;
    case it_unsigned_long_long:
    {
        Data<unsigned long long> d(data_path);
        if (d.read())
            d.calc(mSetting.d["Processing"]["multiplicator"], mSetting.d["Processing"]["logbase"], mSetting.d["Processing"]["power"], mSetting.d["Processing"]["average"]);
    }
    break;


    case it_int16_t:
    {
        Data<int16_t> d(data_path);
        if (d.read())
            d.calc(mSetting.d["Processing"]["multiplicator"], mSetting.d["Processing"]["logbase"], mSetting.d["Processing"]["power"], mSetting.d["Processing"]["average"]);
    }
    break;
    case it_uint16_t:
    {
        Data<uint16_t> d(data_path);
        if (d.read())
            d.calc(mSetting.d["Processing"]["multiplicator"], mSetting.d["Processing"]["logbase"], mSetting.d["Processing"]["power"], mSetting.d["Processing"]["average"]);
    }
    break;


    case it_int32_t:
    {
        Data<int32_t> d(data_path);
        if (d.read())
            d.calc(mSetting.d["Processing"]["multiplicator"], mSetting.d["Processing"]["logbase"], mSetting.d["Processing"]["power"], mSetting.d["Processing"]["average"]);
    }
    break;
    case it_uint32_t:
    {
        Data<uint32_t> d(data_path);
        if (d.read())
            d.calc(mSetting.d["Processing"]["multiplicator"], mSetting.d["Processing"]["logbase"], mSetting.d["Processing"]["power"], mSetting.d["Processing"]["average"]);
    }
    break;


    case it_int64_t:
    {
        Data<int64_t> d(data_path);
        if (d.read())
            d.calc(mSetting.d["Processing"]["multiplicator"], mSetting.d["Processing"]["logbase"], mSetting.d["Processing"]["power"], mSetting.d["Processing"]["average"]);
    }
    break;
    case it_uint64_t:
    {
        Data<uint64_t> d(data_path);
        if (d.read())
            d.calc(mSetting.d["Processing"]["multiplicator"], mSetting.d["Processing"]["logbase"], mSetting.d["Processing"]["power"], mSetting.d["Processing"]["average"]);
    }
    break;


    case it_float:
    {
        Data<float> d(data_path);
        if (d.read())
            d.calc(mSetting.d["Processing"]["multiplicator"], mSetting.d["Processing"]["logbase"], mSetting.d["Processing"]["power"], mSetting.d["Processing"]["average"]);
    }
    break;
    case it_double:
    {
        Data<double> d(data_path);
        if (d.read())
            d.calc(mSetting.d["Processing"]["multiplicator"], mSetting.d["Processing"]["logbase"], mSetting.d["Processing"]["power"], mSetting.d["Processing"]["average"]);
    }
    break;


    default:
        break;
    }

    // exit
    std::cout << "\n" << typeid(*this).name() << "(" << this << ", " << type_name << ")" << "  finished ... " << std::string(50, '+') << "\n\n\n\n";
}

void FabricType::create_types()
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
