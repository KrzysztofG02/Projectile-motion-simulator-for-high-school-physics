#include "ToFile_t.h"

#include <iostream>

using namespace std;


gs::ToFile_t::ToFile_t(std::string const &fileName,
                       std::pair<mpf_class, mpf_class> const &timeRange,
                       size_t const intervalsCount,
                       std::vector<std::function<mpf_class (mpf_class const &)>> const &functions,
                       std::vector<std::string> const &headers,
                       bool const timeColumn,
                       short const precision,
                       char const separator,
                       char const point):
    mFileName(fileName),
    mTimeRange(timeRange),
    mIntervalsCount(ToFile_t::correctIntervalsCount(intervalsCount)),
    mFunctions(functions),
    mHeaders(headers),
    mTimeColumn(timeColumn),
    mPrecision(precision),
    mSeparator(separator), mPoint(point),

    mStrFormat(("%." + (mPrecision < 0 ? "" : to_string(mPrecision)) + "Ff")),
    mBuffer(nullptr) {}


void gs::ToFile_t::operator()()
{
    ofstream file(mFileName);

    if(!file.is_open()) {
        cerr << "Can't open file: " << mFileName << '\n';
        return;
    }

    this->makeFile(file);

    file.close();
}

void gs::ToFile_t::setTimeRange(std::pair<mpf_class, mpf_class> const &timeRange)
{
    mTimeRange = timeRange;
}

void gs::ToFile_t::setPrecision(short const precision)
{
    mPrecision = precision;
    mStrFormat = string("%." + (mPrecision < 0 ? "" : to_string(mPrecision)) + "Ff");
}

void gs::ToFile_t::setIntervalsCount(size_t const intervalsCount)
{
    mIntervalsCount = ToFile_t::correctIntervalsCount(intervalsCount);
}


constexpr size_t gs::ToFile_t::correctIntervalsCount(size_t const intervalsCount)
{
    return intervalsCount < 2 ? 2 : (intervalsCount > 10000 ? 10000 : intervalsCount);
}


void gs::ToFile_t::makeFile(std::ofstream &file)
{
    this->makeHeadRow(file);
    this->makeMidRows(file);
    this->makeLastRow(file);
}

void gs::ToFile_t::makeHeadRow(std::ofstream &file)
{
    size_t i = 0;

    if(mTimeColumn)
    {
        file << "t" << mSeparator;
    }

    for(; i < min(mFunctions.size(), mHeaders.size()); ++i)
    {
        file << mHeaders[i] << mSeparator;
    }

    for(; i < mFunctions.size(); ++i)
    {
        file << 'H' << to_string(i + 1) << mSeparator;
    }

    file << '\n';
}

void gs::ToFile_t::makeMidRows(std::ofstream &file)
{
    size_t const &&intervalCountWithoutLast = mIntervalsCount - 1;

    for(size_t i = 0; i < intervalCountWithoutLast; ++i)
    {
        mpf_class const &&interval = (mTimeRange.second - mTimeRange.first) / mpf_class(intervalCountWithoutLast) * mpf_class(i);

        this->makeMidRow(file, interval);
    }
}

void gs::ToFile_t::makeMidRow(std::ofstream &file, mpf_class const &interval)
{
    size_t j = 0;
    char const * const &&cStrFormat = mStrFormat.c_str();

    if(mTimeColumn)
    {
        gmp_asprintf(&mBuffer, cStrFormat, interval);
        gs::replaceFirstFitChar(mBuffer, '.', mPoint);
        file << mBuffer << mSeparator;
        free(mBuffer);
    }

    for(; j < mFunctions.size(); ++j)
    {
        gmp_asprintf(&mBuffer, cStrFormat, mFunctions[j](interval));
        gs::replaceFirstFitChar(mBuffer, '.', mPoint);
        file << mBuffer << mSeparator;
        free(mBuffer);
    }

    for(; j < max(mFunctions.size(), mHeaders.size()); ++j)
    {
        file << mSeparator;
    }

    file << '\n';
}

void gs::ToFile_t::makeLastRow(std::ofstream &file)
{
    size_t i = 0;
    char const * const &&cStrFormat = mStrFormat.c_str();

    if(mTimeColumn)
    {
        gmp_asprintf(&mBuffer, cStrFormat, mTimeRange.second);
        gs::replaceFirstFitChar(mBuffer, '.', mPoint);
        file << mBuffer << mSeparator;
        free(mBuffer);
    }

    for(; i < mFunctions.size(); ++i)
    {
        gmp_asprintf(&mBuffer, cStrFormat, mFunctions[i](mTimeRange.second));
        gs::replaceFirstFitChar(mBuffer, '.', mPoint);
        file << mBuffer << mSeparator;
        free(mBuffer);
    }

    for(; i < max(mFunctions.size(), mHeaders.size()); ++i)
    {
        file << mSeparator;
    }
}
