
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CSVREADER_H
#define CSVREADER_H

#include <fstream>
#include <sstream>
#include <vector>
#include "utils.h"

#define MAX_LINE_LENGTH 8192

/**
 * @class CSVReader
 *
 * Class implementing a simple csv file reader.
 *
 * Though it is used for benchmarking and testing of cPMML, it is not part of
 * its core or of its API, since it's best-effort and not directly involved in
 * model scoring.
 */
class CSVReader {
 public:
  explicit CSVReader(const std::string &filename) : file(filename) {
    this->file.getline(line, MAX_LINE_LENGTH);
    std::stringstream line_stream(line);
    std::string field;
    while (getline(line_stream, field, ',')) this->header.push_back(remove_all(remove_all(field, '\r'), '"'));
  }

  std::unordered_map<std::string, std::string> read() {
    std::unordered_map<std::string, std::string> result;
    line[0] = 0;
    this->file.getline(line, MAX_LINE_LENGTH);
    std::stringstream line_stream(line);
    std::string field;

    if (!getline(line_stream, field, ',')) return result;
    for (unsigned int i = 0; i < this->header.size(); i++, getline(line_stream, field, ','))
      result[header[i]] = remove_all(remove_all(field, '\r'), '"');

    return result;
  }

  std::string read_raw() {
    this->file.getline(line, MAX_LINE_LENGTH);
    return line;
  }

 private:
  std::ifstream file;
  std::vector<std::string> header;
  char line[MAX_LINE_LENGTH];
};

#endif
