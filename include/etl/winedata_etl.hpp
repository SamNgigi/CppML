#ifndef WINEDATA_ETL_HPP
#define WINEDATA_ETL_HPP

#include <vector>
#include <string>

class WineDataETL{

  std::string dataset = "";
  std::string delimiter = ",";
  bool header = false;

public:
  WineDataETL(std::string dataset, std::string delimiter, bool header): dataset(dataset), delimiter(delimiter), header(header) {}
  
  std::vector<std::vector<std::string>> readCSV();

};

#endif // WINEDATA_ETL_HPP
