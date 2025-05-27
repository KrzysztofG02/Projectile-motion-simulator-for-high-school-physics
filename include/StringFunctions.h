#ifndef STRINGFUNCTIONS_H_INCLUDED
#define STRINGFUNCTIONS_H_INCLUDED

#include <string>


namespace gs
{

std::string makeNumberStr(unsigned const value, //works with numbers > 0
                          short const expOf10 = 0,
                          std::string const &prefix = "",
                          unsigned short const plusScientificNotationTreshold = 0,
                          //Minimal output's positive index of 10 at which scientific notation starts being used. 0 = never
                          unsigned short const minusScientificNotationTreshold = 0,
                          //Minimal output's negative index of 10 at which scientific notation starts being used. 0 = never
                          std::string const &point = ",");

void replaceFirstFitChar(char *str, char const oldChar, char const newChar);

void replaceSubstr(std::string &input, std::string const &oldSub, std::string const &newSub);

std::string getNumWithThousandsSeparator(char const *str, char sep10 = '.');

}

#endif // STRINGFUNCTIONS_H_INCLUDED
