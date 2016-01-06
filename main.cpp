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

myint atoisimple(const char *b)
{
    myint res = 0; // Initialize result
    int sgn = 1;

    // Skip whitespace
    // TODO: recognize not only spaces
    for (; *b == ' '; ++b);

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
    // TODO: recognize not only spaces
    for (; *b == ' '; ++b);

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
    // TODO: recognize not only spaces
    for (; *b == ' '; ++b);

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
    // TODO: recognize not only spaces
    for (; *b == ' '; ++b);

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
        const char* e = 0;

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

unsigned long N = 10000000UL;
const unsigned WORD_COUNT = 10000;
const unsigned MAX_WORD_LEN = 8;

bool unit_test()
{
    assert(fastatoiaddunr("-10") == atoi("-10"));
    assert(fastatoiadd("-10") == atoi("-10"));
    assert(fastatoi("-10") == atoi("-10"));
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
    bool digits_only = false;

    for (size_t L = 1; L <= MAX_WORD_LEN; ++L)
    {

      Name2Time best;
      std::vector<std::string> v;
      generate_add(v, WORD_COUNT, L, digits_only);


      std::cout << "Digits per number: " << L << " The string contains"<< (digits_only ? " digits only" : " whitespace and sign" )<< std::endl; 
      std::cout << "Int size in bytes:" << sizeof(myint) << std::endl;
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

      if (argc > 1) 
      {
        std::ofstream f(argv[1], std::ios::app | std::ios::out);
        if (!f.fail())
        {
          for (auto& kv : best)
          {
            f << L << "," << (digits_only ? "DO" : "WS" ) << "," << kv.first << "," << kv.second << std::endl;
          }        
        }
      }
      else
      {
        std::cout << "No output file param" << std::endl;
      }
    
    }

    return 0;
}



