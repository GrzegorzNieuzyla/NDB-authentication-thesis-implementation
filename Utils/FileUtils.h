#pragma once
#include <string>
#include <vector>
#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
class FileUtils
{
public:
    struct CsvFileData
    {
        std::string algorithm;
        int length = 0;
        int count = 0;
        double probabilityRatio = 0;
        std::vector<double> probabilityRatios;
        double recordCountRatio = 0;
        int specifiedBits = 0;
        static std::string GetCsvDataHeader()
        {
            return "#Algorithm;l;c;q;p;r;k";
        }
        std::string GetCsvRow()
        {
            using boost::adaptors::transformed;
            using boost::algorithm::join;
            std::vector<std::string> probabilities;
            std::transform(probabilityRatios.begin(), probabilityRatios.end(), std::back_inserter(probabilities), [](double v){return std::to_string(v);});
            return (boost::format("%1%;%2%;%3%;%4%;%5%;%6%;%7%")
                    % algorithm % length
                    % count % probabilityRatio
                    % join(probabilities, ",")
                    % recordCountRatio % specifiedBits).str();
        }
    };
};
