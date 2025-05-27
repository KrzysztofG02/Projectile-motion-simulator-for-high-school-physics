#include "StringFunctions.h"

#include <algorithm>
#include <cstring>

using namespace std;


std::string gs::makeNumberStr(unsigned const value, //works with numbers > 0
                              short const expOf10,
                              std::string const &prefix,
                              unsigned short const plusScientificNotationTreshold,
                              //Minimal output's positive index of 10 at which scientific notation starts being used. 0 = never
                              unsigned short const minusScientificNotationTreshold,
                              //Minimal output's negative index of 10 at which scientific notation starts being used. 0 = never
                              std::string const &point)
{
    if(value == 0)
    {
        return string("0");
    }

    string result(to_string(value));
    unsigned short const len = result.length();

    if(expOf10 + static_cast<short>(len) <= -static_cast<short>(minusScientificNotationTreshold))       //0 < outputValue <= 10^(-mSNT)
    {
        size_t const lastNonZeroIdx = result.find_last_not_of('0');

        result = result.substr(0, 1) + (lastNonZeroIdx != 0 ? point : "") + result.substr(1, lastNonZeroIdx) + "e-" + to_string(1 - len - expOf10);
    }
    else if(expOf10 < 1 - static_cast<short>(len))                                                      //10^(-mSNT) < outputValue < 1
    {
        result = "0" + point + string(-len - expOf10, '0') + result.substr(0, result.find_last_not_of('0') + 1);
    }
    else if(expOf10 < 0)                                                                                //1 <= outputValue < 10^(pSNT) && outputValue contains a point
    {
        size_t const &&pointIdx = len - static_cast<size_t>(-expOf10);
        size_t const lastNonZeroIdx = result.find_last_not_of('0');

        result = result.substr(0, pointIdx) + (lastNonZeroIdx >= pointIdx ? point + result.substr(pointIdx, lastNonZeroIdx + 1 - pointIdx) : "");
    }
    else if(expOf10 < plusScientificNotationTreshold - static_cast<short>(len))                         //1 <= outputValue < 10^(pSNT) && outputValue contains no point
    {
        result += string(expOf10, '0');
    }
    else if(size_t const &lastNonZeroDigitIdx = result.find_last_not_of('0'); lastNonZeroDigitIdx != 0) //outputValue >= 10^(pSNT) && outputValue contains a point
    {
        result = result.substr(0, 1) + point + result.substr(1, lastNonZeroDigitIdx) + 'e' + to_string(len + expOf10 - 1);
    }
    else                                                                                                //outputValue >= 10^(pSNT) && outputValue contains no point
    {
        result = result.substr(0, 1) + 'e' + to_string(len + expOf10 - 1);
    }

    result = prefix + result;   //optional prefix, e.g. "-" or "+"

    return result;
}

void gs::replaceFirstFitChar(char *str, char const oldChar, char const newChar)
{
    if(oldChar != newChar)
    {
        if(char * const &&pos = find(str, str + strlen(str), oldChar); *pos == oldChar)
        {
            *pos = newChar;
        }
    }
}

void gs::replaceSubstr(std::string &input, std::string const &oldSub, std::string const &newSub)
{
    if(size_t const &&pos = input.find(oldSub); pos != std::string::npos)
    {
        input.replace(pos, oldSub.size(), newSub);
    }
}

std::string gs::getNumWithThousandsSeparator(char const *c_str, char sep10)
{
    if(!c_str || *c_str == '\0')
    {
        return string{};
    }

    string const str(c_str);
    int limit;

    if(size_t const pointPos = str.find(sep10); pointPos != string::npos)
    {
        limit = pointPos;
    }
    else
    {
        limit = str.size();
    }

    bool const lessThan0 = str[0] == '-';
    string result = string(str.size() + (limit - 1 - static_cast<int>(lessThan0)) / 3, ' ');

    result.shrink_to_fit();
    string::iterator result_it = result.end() - 1;
    int i = str.size() - 1;

    for(; i >= limit; --i, --result_it)
    {
        *result_it = str[i];
    }

    for(unsigned short k = 0; i >= 0 + static_cast<int>(lessThan0); ++k, --i, --result_it)
    {
        if(k == 3)
        {
            --result_it;
            k = 0;
        }

        *result_it = str[i];
    }

    if(lessThan0)
    {
        result[0] = '-';
    }

    return result;
}
