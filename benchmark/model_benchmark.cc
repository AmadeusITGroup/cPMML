
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#include <chrono>
#include <iostream>
#include <unordered_map>

#include "cPMML.h"
#include "utils/csvreader.h"
#include "utils/utils.h"

int main(int argc, char **argv) {
  auto start = std::chrono::steady_clock::now();
  cpmml::Model model(argv[1]);
  auto end = std::chrono::steady_clock::now();
  double elapsed_load = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

  CSVReader reader(argv[2]);
  double n_predict = 0;
  std::unordered_map<std::string, std::string> sample;
  double elapsed_predict = 0;
  while ((sample = reader.read()).size() > 0) {
    try {
      start = std::chrono::steady_clock::now();
      model.predict(sample);
      end = std::chrono::steady_clock::now();
      elapsed_predict += (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
      n_predict++;
    } catch (const cpmml::Exception &exception) {
    }
  }

  CSVReader reader_score(argv[2]);
  double n_score = 0;
  double elapsed_score = 0;
  while ((sample = reader_score.read()).size() > 0) {
    try {
      start = std::chrono::steady_clock::now();
      model.score(sample);
      end = std::chrono::steady_clock::now();
      elapsed_score += (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
      n_score++;
    } catch (const cpmml::Exception &exception) {
    }
  }

  std::cout << "\tLoading time: " << std::setprecision(2) << std::fixed << elapsed_load / 1000 / 1000 << " ms";
  std::cout << "\tScoring time: " << std::setprecision(2) << std::fixed << elapsed_score / n_score / 1000 << " us";
  std::cout << "\tScoring TPS: " << format_int((int)1e9 / (elapsed_score / n_score));
  std::cout << "\tPredict time: " << std::setprecision(2) << std::fixed << elapsed_predict / n_predict / 1000 << " us";
  std::cout << "\tPredict TPS: " << format_int((int)1e9 / (elapsed_predict / n_predict)) << std::endl;

  return 0;
}
