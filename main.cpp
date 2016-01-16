// main.cpp

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <cassert>

typedef long myint;

// this definition does not work properly for tabs, newlines etc.
//#define isspace_fast(x) ((x) == ' ')

// "C" locale compatible definition
#define isspace_fast(x) ((x) == ' ' || (x) == '\t' || (x) == '\n' || (x) == '\v' || (x) == '\f' || (x) == '\r')

myint atoisimple(const char *b)
{
    myint res = 0; // Initialize result
    int sgn = 1;

    // Skip whitespace
    for (; isspace_fast(*b); ++b);

    if (*b == '-')
    {
        sgn = -1;
        ++b;
    } 
    else if (*b == '+')
    {
        ++b;
    }

    // Iterate through all characters of input string and update result
    for (; *b != 0; ++b)
    {
        if (*b < '0' || *b > '9') 
        {
            return sgn * res;
        }
        res = res * 10 + *b - '0';
    }

    // return result.
    return sgn * res;
}

myint fastatoi(const char *b)
{
    myint res = 0; // Initialize result
    int sgn = 1;

    // Skip whitespace
    for (; isspace_fast(*b); ++b);

    if (*b == '-')
    {
        sgn = -1;
        ++b;
    } 
    else if (*b == '+')
    {
        ++b;
    }

    // Iterate through all characters of input string and update result
    for (; *b != 0; ++b)
    {
        unsigned d = static_cast<unsigned>(*b) - '0';
        if (d > 9) 
        {
            return sgn * res;
        }
        res = res * 10 + d;
    }

    // return result.
    return sgn * res;
}

myint fastatoiadd(const char *b)
{
    myint res = 0; // Initialize result

    // Skip whitespace
    for (; isspace_fast(*b); ++b);

    if (*b == '-')
    {
        ++b;
        // Iterate through all characters of input string and update result
        for (; ; ++b)
        {
            unsigned d = static_cast<unsigned>(*b) - '0';
            if (d > 9) 
            {
                return res;
            }
            res = res * 10 - d;
        }

    } 
    else if (*b == '+')
    {
        ++b;
    }
    
    // Iterate through all characters of input string and update result
    for (; ; ++b)
    {
        unsigned d = static_cast<unsigned>(*b) - '0';
        if (d > 9) 
        {
            return res;
        }
        res = res * 10 + d;
    }

    //  unreachable
    return res;
}

myint fastatoiaddunr(const char *b)
{
    myint res = 0; // Initialize result

    // Skip whitespace
    for (; isspace_fast(*b); ++b);

    if (*b == '-')
    {
        ++b;
        unsigned d = static_cast<unsigned>(*b) - '0';
        if (d > 9) 
        {
            return 0;
        }
        res = -static_cast<myint>(d);
        ++b;
        
        // Iterate through all characters of input string and update result
        for (; ; ++b)
        {
            unsigned d = static_cast<unsigned>(*b) - '0';
            if (d > 9) 
            {
                return res;
            }
            res = res * 10 - d;
        }

    } 
    else if (*b == '+')
    {
        ++b;
    }
    
    unsigned d = static_cast<unsigned>(*b) - '0';
    if (d > 9) 
    {
        return 0;
    }
    res = d;
    ++b;
    
    // Iterate through all characters of input string and update result
    for (; ; ++b)
    {
        unsigned d = static_cast<unsigned>(*b) - '0';
        if (d > 9) 
        {
            return res;
        }
        res = res * 10 + d;
    }

    //  unreachable
    return res;
}


myint zero(const char *b)
{
    volatile myint res = (*b == 0); // Initialize result

    // return result.
    return res;
}

typedef std::map<std::string, unsigned long long> Name2Time;

template <typename F>
void test(unsigned long rep, 
          F f, 
          const std::vector<std::string>& v, 
          Name2Time& best, 
          const std::string& name)
{
    try
    {
        const char* b = 0;

        myint r = 0;
        auto et0 = std::chrono::system_clock::now();
        std::clock_t t0 = std::clock();
        for (unsigned int i = 0; i < rep; ++i)
        {
            unsigned int ii = i % v.size();
            b = v[ii].c_str();
            r += f(b);
        }
        std::clock_t t = std::clock();
        auto et = std::chrono::system_clock::now() - et0;
        
        std::cout << name << ":" << std::setw(18) << r << "," <<
                     std::setw(10) << t - t0 << ", " << 
                     std::setw(10) << et.count() << std::endl;
        if (name.length() > 0)
        {
            std::string ename = "e." + name;
            std::string cname = "c." + name;
            auto bt = best[ename];
            if (bt == 0 || bt > et.count()) { best[ename] = et.count(); }
            bt = best[cname];
            if (bt == 0 || bt > t - t0) { best[cname] = t - t0; }
        }
    }
    catch(std::exception& exc)
    {
        std::cout << "ERROR" << std::endl;
    }
}


void unit_test()
{
    std::vector<myint (*)(const char *)> v = { atoisimple, fastatoi, fastatoiadd, fastatoiaddunr };
    std::vector<std::string> sv = { "0", " 1", " 2", "\t 3", "4x", "5%", 
                                    "  10  ", " 99.", "100", "12345", "999999",
                                    "-1", " -2x", "   +123456+", " -345", "\n\v8I",
                                    " fake", "values", " +", "    zero  "};
    for (auto pf : v)
    {
       for (const auto s: sv)
       {
           assert((*pf)(s.c_str()) == atoi(s.c_str()));
       }
    }
}

void generate_add(std::vector<std::string>& v, int count, int length, bool digits_only)
{
    for (unsigned i = 0; i < count; ++i)
    {
        std::string s;
        for (unsigned j = 0; j < length; ++j)
        {
            s += '0' + (i + j * 7) % 10;
        }

        int mode = digits_only ? 0 : (i % 5);


        switch (mode)
        {
          case 0:
             v.push_back(s);
             break;
          case 1:
             v.push_back('-' + s);
             break;
          case 2:
              v.push_back('+' + s);
             break;
          case 3:
             v.push_back("    -" + s + ")))");
             break;
          case 4:
             v.push_back("    +" + s + ".12");
             break;
        }

    }
}

int main(int argc, char* argv[])
{
    unsigned MAX_WORD_LEN = 8;
    unsigned long N = 10000000UL;
    unsigned WORD_COUNT = 10000;
    bool digits_only = false;
    std::string out_filename = "";

    std::vector<std::string> args(argv, argv + argc);
    int i = 1;
    while (i < args.size())
    {
        if (args[i] == "-t" || args[i] == "--unittest")
        {
            unit_test();
            return 0;
        }
        if (args[i] == "-d" || args[i] == "--digitsonly")
        {
            digits_only = true;
        }
        if ((args[i] == "-l" || args[i] == "--length") && 
            i < args.size()) 
        {
            if ((MAX_WORD_LEN = atoi(args[i + 1].c_str())) > 20 || MAX_WORD_LEN == 0) 
            {
                std::cerr << "Invalid length" << std::endl;
                return 1;
            }
            i++;
        }       
        if ((args[i] == "-w" || args[i] == "--words") && 
            i < args.size())
        {
            if ((WORD_COUNT = atol(args[i + 1].c_str())) > 1000000 || WORD_COUNT == 0) 
            {
                std::cerr << "Invalid word count" << std::endl;
                return 1;
            }
            i++;
        }
        if ((args[i] == "-n" || args[i] == "--numloops") && 
            i < args.size())
        {
            if ((N = atol(args[i + 1].c_str())) > 1000000000000UL || N == 0) 
            {
                std::cerr << "Invalid number of loops" << std::endl;
                return 1;
            }
            i++;
        }
        if ((args[i] == "-o" || args[i] == "--output") && 
            i < args.size())
        {
            out_filename = args[i + 1];
            i++;
        }

        i++;
    }

    for (size_t L = 1; L <= MAX_WORD_LEN; ++L)
    {

        Name2Time best;
        std::vector<std::string> v;
        generate_add(v, WORD_COUNT, L, digits_only);


        std::cout << "Digits per number: " << L << " The string contains"
                  << (digits_only ? " digits only" : " whitespace and sign" ) << std::endl; 
        std::cout << "Int size in bytes:" << sizeof(myint) << std::endl;
        std::cout << "Loops:" << N << std::endl;
        std::cout << "Words:" << WORD_COUNT << std::endl;
        std::cout << "Ramp up:" << std::endl;
        test(N, atoi, v, best, "");

        for (int i = 0; i < 3; ++i) 
        {
            test(N, zero, v, best,            "zero..........");
            test(N, atoi, v, best,            "atoi..........");
            test(N, atoisimple, v, best,      "atoisimple....");
            test(N, fastatoi, v, best,        "fastatoi......");
            test(N, fastatoiadd, v, best,     "fastatoiadd...");
            test(N, fastatoiaddunr, v, best,  "fastatoiaddunr");
        }

        for (auto& kv : best)
        {
            std::cout << "Best: " << kv.first << " : " << std::setw(11) << kv.second << std::endl;
        }

        if (out_filename.length() > 0) 
        {
            std::ofstream f(out_filename.c_str(), std::ios::app | std::ios::out);
            if (!f.fail())
            {
                if (L == 1)
                {
                    // header
                    f << "\"Length\",\"Digits only\"" ;
                    for (auto& kv : best)
                    {
                         f << ",\"" << kv.first << "\""; 
                    }
                    f << std::endl;
                }
                f << L << "," << (digits_only ? 1 : 0);

                for (auto& kv : best)
                {
                    f << "," << kv.second;
                }
                f << std::endl;
            }
        }
        else
        {
          std::cout << "No output file param" << std::endl;
        }
    
    }

    return 0;
}



