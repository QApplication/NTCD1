#ifndef PROC_H
#define PROC_H

#include <vector>
#include <map>
#include <cmath>
#include <numeric>
#include <iostream>
#include <iomanip>

#include <cerrno>
#include <cfenv>
#include <cstring>


template <typename T>
class Processing
{
public:
    enum Oprt: int
    {
        o_mult,
        o_log,
        o_pow,
        o_mean,
    };

    Processing(const std::vector<T> &d);
    ~Processing();

    void run(T multiplicator, T logbase, T power, size_t average);
    void print();

private:
    const std::vector<T> &d;
    std::vector<T> out_mul, out_pow, out_avr;
    std::vector<double> out_log;

    void _mult(T value = static_cast<T>(2.0));
    void _log(T logbase = static_cast<T>(10.0));
    void _pow(T value = static_cast<T>(2.0));
    void _mean(size_t windwow = 10);

    Processing() = delete;
    Processing(const Processing&) = delete;
    Processing &operator=(const Processing&) = delete;
    Processing(const Processing&&) = delete;
    Processing &operator=(const Processing&&) = delete;

    template<typename Ty> friend class Data;
};



template<typename T>
Processing<T>::Processing(const std::vector<T> &d)
    : d(d)
{

}

template<typename T>
Processing<T>::~Processing()
{

}

template<typename T>
void Processing<T>::run(T multiplicator, T logbase, T power, size_t average)
{
    out_mul.resize(d.size(), static_cast<T>(0.0));
    _mult(multiplicator);

    out_log.resize(d.size(), 0.0);
    _log(logbase);

    out_pow.resize(d.size(), static_cast<T>(0.0));
    _pow(power);

    out_avr.resize(d.size(), static_cast<T>(0.0));
    _mean(average);

    print();
}

template<typename T>
void Processing<T>::print()
{
    int col_width = 15;
    int prec = 0;
    int log_prec = 6;

    if (std::is_same<T, float>::value) {
        col_width += 5;
        prec = 6;
    }
    if (std::is_same<T, double>::value) {
        col_width += 10;
        prec = 6;
    }


    // header
    std::cout << std::string(10, 0x20)
              << std::right << std::setw(col_width) << "Data"
              << std::right << std::setw(col_width) << "Mult"
              << std::right << std::setw(col_width) << "Log"
              << std::right << std::setw(col_width) << "Pow"
              << std::right << std::setw(col_width) << "Mean"
              << std::endl << std::string(6*col_width, '=') << std::endl;

    // data
    for (size_t i = 0; i < d.size(); ++i) {
        std::cout << std::string(10, 0x20)
                  << std::right << std::setw(col_width) << std::fixed << std::setprecision(prec) << d.at(i)
                  << std::right << std::setw(col_width) << std::fixed << std::setprecision(prec) << out_mul.at(i);

        std::isnan(out_log.at(i))
            ? std::cout << std::right << std::setw(col_width) << "NaN"
            : std::cout << std::right << std::setw(col_width) << std::fixed << std::setprecision(log_prec) << out_log.at(i)
            ;

        std::isnan(out_pow.at(i))
            ? std::cout << std::right << std::setw(col_width) << "NaN"
            : std::cout << std::right << std::setw(col_width) << std::fixed << std::setprecision(prec) << out_pow.at(i)
            ;

        std::cout << std::right << std::setw(col_width) << std::fixed << std::setprecision(prec) << out_avr.at(i)
                  << std::endl;
    }
}

template<typename T>
void Processing<T>::_mult(T value)
{
    for (size_t i = 0; i < d.size(); ++i) {
        out_mul[i] = value * d.at(i);
    }
}

template<typename T>
void Processing<T>::_log(T logbase)
{
    for (size_t i = 0; i < d.size(); ++i) {

        // error handling
        errno = 0;
        std::feclearexcept(FE_ALL_EXCEPT);

        if (double _D = std::log(logbase); errno == 0) {

            double _N = std::log(d.at(i));
            out_log[i] = (errno == 0)
                             ? _N / _D
                             : std::nan(std::to_string(errno).c_str())
                ;
        } else {
            out_log[i] = std::nan("e");
        }
    }
}

template<typename T>
void Processing<T>::_pow(T value)
{
    std::feclearexcept(FE_ALL_EXCEPT);

    for (size_t i = 0; i < d.size(); ++i) {

        errno = 0;
        auto b2 = std::pow(d[i], value);

        if (errno == 0) {
            out_pow[i] = b2;
        } else {
            // std::cout << b2 << "  " << d[i] << "  " << errno << "  " << std::strerror(errno) << std::endl;
            out_pow[i] = std::nan(std::to_string(errno).c_str() );
        }

    }
}

template<typename T>
void Processing<T>::_mean(size_t windwow)
{
    const T e = static_cast<T>(std::nan("?"));

    for (size_t i = 0; i < d.size(); ++i) {

        out_avr[i] = i < windwow
            ? static_cast<T>(0.0)
                         : static_cast<T>((double)std::accumulate(std::prev(&d.at(i), windwow-1), &d.at(i), static_cast<T>(0.0), [] (T a, T b) { return std::move(a) + b; })
                                          / (double)windwow)
            ;
    }
}

#endif // PROC_H
