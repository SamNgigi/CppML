#include "etl/winedata_etl.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

std::vector<std::vector<std::string>> WineDataETL::readCSV(){
  
  std::vector<std::vector<std::string>> data {{""}};

  std::ifstream file(dataset);

  if(!file.is_open()){
    throw std::runtime_error("Unable to open file: " + dataset);
  }


  return data;
}
