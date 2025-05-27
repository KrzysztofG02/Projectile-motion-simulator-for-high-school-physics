#ifndef TOFILE_H_INCLUDED
#define TOFILE_H_INCLUDED

#include "StringFunctions.h"

#include <fstream>
#include <functional>
#include <gmpxx.h>
#include <utility>
#include <vector>


using namespace std;


namespace gs
{

class ToFile_t final
{
private:
    std::string mFileName;
    std::pair<mpf_class, mpf_class> mTimeRange;
    size_t mIntervalsCount;
    std::vector<std::function<mpf_class (mpf_class const &)>> mFunctions;
    std::vector<std::string> mHeaders;
    bool mTimeColumn;
    short mPrecision; //precision < 0 --> max available precision
    char mSeparator;
    char mPoint;

    std::string mStrFormat;
    char *mBuffer;

public:
    ToFile_t(string const &fileName = {},
             std::pair<mpf_class, mpf_class> const &timeRange = {},
             size_t const intervalsCount = {},
             std::vector<std::function<mpf_class (mpf_class const &)>> const &functions = {},
             std::vector<std::string> const &headers = {},
             bool const timeColumn = true,
             short const precision = -1,
             char const separator = ';',
             char const point = '.');


public:
    void operator()();
    void setTimeRange(std::pair<mpf_class, mpf_class> const &timeRange);
    void setPrecision(short const precision);
    void setIntervalsCount(size_t const intervalsCount);


private:
    static constexpr size_t correctIntervalsCount(size_t const intervalsCount);

    void makeFile(std::ofstream &file);
        void makeHeadRow(std::ofstream &file);
        void makeMidRows(std::ofstream &file);
            void makeMidRow(std::ofstream &file, mpf_class const &interval);
        void makeLastRow(std::ofstream &file);
};

}

#endif // TOFILE_H_INCLUDED
