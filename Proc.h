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

    void run();
    void print();

private:
    const std::vector<T> &d;

    //
    std::map<int,std::vector<T>*> out;
    std::vector<double> log_out;

    void _mult(T value = static_cast<T>(2.0));
    void _log();
    void _pow(T value = static_cast<T>(2.0));
    void _mean(size_t windwow = 10);



    Processing() = delete;
    Processing(const Processing&) = delete;
    Processing &operator=(const Processing&) = delete;
    Processing(const Processing&&) = delete;
    Processing &operator=(const Processing&&) = delete;
};



template<typename T>
Processing<T>::Processing(const std::vector<T> &d)
    : d(d)
{

}

template<typename T>
Processing<T>::~Processing()
{
    for (auto& _d : out) {
        if (_d.second)
            delete _d.second;
    }
    out.clear();
}

template<typename T>
void Processing<T>::run()
{
    out[o_mult] = new std::vector<T>(d.size(), T());
    // out[o_log]  = new std::vector<T>(d.size(), T());
    log_out.resize(d.size(), 0.0);
    out[o_pow]  = new std::vector<T>(d.size(), T());
    out[o_mean] = new std::vector<T>(d.size(), T());

     _mult();
     _log();
     _pow();
     _mean();

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
                  << std::right << std::setw(col_width) << std::fixed << std::setprecision(prec) << (*out[o_mult]).at(i);
        std::isnan(log_out.at(i))
            ? std::cout << std::right << std::setw(col_width) << "NaN"
            : std::cout << std::right << std::setw(col_width) << std::fixed << std::setprecision(log_prec) << log_out.at(i)
            ;
        std::cout << std::right << std::setw(col_width) << std::fixed << std::setprecision(prec) << (*out[o_pow]).at(i)
                  << std::right << std::setw(col_width) << std::fixed << std::setprecision(prec) << (*out[o_mean]).at(i)
                  << std::endl;
    }
}

template<typename T>
void Processing<T>::_mult(T value)
{
    for (size_t i = 0; i < d.size(); ++i) {
        (*out[o_mult])[i] = value * d.at(i);
    }
}

template<typename T>
void Processing<T>::_log()
{
    const T t = static_cast<double>(0.0);        // bound
    const T e = static_cast<double>(std::nan("exception"));


    for (size_t i = 0; i < d.size(); ++i) {

        // error handling
        errno = 0;
        std::feclearexcept(FE_ALL_EXCEPT);

        double v = std::log(d.at(i));

        log_out[i] = errno == 0
                         ? v
                         : std::nan("e")
            ;


        // (*out[o_log])[i] = d.at(i) > t
        //                        ? std::log(d.at(i))
        //                        : e
        //     ;

        // (*out[o_log])[i] = std::log(d.at(i));
    }
}

template<typename T>
void Processing<T>::_pow(T value)
{
    for (size_t i = 0; i < d.size(); ++i) {
        (*out[o_pow])[i] = std::pow(d[i], value);
    }
}

template<typename T>
void Processing<T>::_mean(size_t windwow)
{
    const T e = static_cast<T>(std::nan("?"));

    for (size_t i = 0; i < d.size(); ++i) {

        (*out[o_mean])[i] = i < windwow
            ? static_cast<T>(0.0)
                            : std::accumulate(std::prev(&d.at(i), windwow-1), &d.at(i), static_cast<T>(0.0), [] (T a, T b) { return std::move(a) + b; })
            ;
    }
}

#endif // PROC_H
