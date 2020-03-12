
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_MULTIPLEMODELMETHOD_H
#define CPMML_MULTIPLEMODELMETHOD_H

#include <string>
#include <unordered_map>
#include <vector>

#ifdef MULTITHREADING
#include <omp.h>
#endif

#include "core/internal_score.h"
#include "segment.h"
#include "utils/utils.h"

/**
 * @class MultipleModelMethod
 *
 * Class reresenting <a
 * href="http://dmg.org/pmml/v4-4/MultipleModels.html#xsdType_MULTIPLE-MODEL-METHOD">PMML
 * MULTIPLE-MODEL-METHOD</a>.
 *
 * For instance:
 * - majorityVote
 * - weightedAverage
 * - modelChain
 */
class MultipleModelMethod {
 public:
  enum class MultipleModelMethodType {
    MAJORITY_VOTE,
    WEIGHTED_MAJORITY_VOTE,
    AVERAGE,
    WEIGHTED_AVERAGE,
    //    MEDIAN,
    //    MAX,
    SUM,
    //    SELECT_FIRST,
    //    SELECT_ALL,
    MODEL_CHAIN
  };

  MultipleModelMethodType value;
  std::function<std::unique_ptr<InternalScore>(const Sample &, const std::vector<Segment> &)> function;

  MultipleModelMethod() = default;

  explicit MultipleModelMethod(const std::string &multiplemodelmethod, const MiningFunction &mining_function)
      : value(from_string(multiplemodelmethod)), function(to_function(multiplemodelmethod, mining_function)) {}

  static MultipleModelMethodType from_string(const std::string &multiplemodelmethod) {
    static const std::unordered_map<std::string, MultipleModelMethodType> multiplemodelmethod_converter = {
        {"majorityvote", MultipleModelMethodType::MAJORITY_VOTE},
        {"weightedmajorityvote", MultipleModelMethodType::WEIGHTED_MAJORITY_VOTE},
        {"average", MultipleModelMethodType::AVERAGE},
        {"weightedaverage", MultipleModelMethodType::WEIGHTED_AVERAGE},
        //        {"median", MultipleModelMethodType::MEDIAN},
        //        {"max", MultipleModelMethodType::MAX},
        {"sum", MultipleModelMethodType::SUM},
        //        {"selectfirst", MultipleModelMethodType::SELECT_FIRST},
        //        {"selectall", MultipleModelMethodType::SELECT_ALL},
        {"modelchain", MultipleModelMethodType::MODEL_CHAIN}};

    try {
      return multiplemodelmethod_converter.at(::to_lower(multiplemodelmethod));
    } catch (const std::out_of_range &e) {
      throw cpmml::ParsingException(multiplemodelmethod + " not supported");
    }
  }

  static std::function<std::unique_ptr<InternalScore>(const Sample &, const std::vector<Segment> &)> to_function(
      const std::string &multiplemodelmethod, const MiningFunction &mining_function) {
    switch (from_string(multiplemodelmethod)) {
      case MultipleModelMethodType::MAJORITY_VOTE:
        return majority_vote;
      case MultipleModelMethodType::WEIGHTED_MAJORITY_VOTE:
        return weighted_majority_vote;
      case MultipleModelMethodType::AVERAGE:
        switch (mining_function.value) {
          case MiningFunction::MiningFunctionType::CLASSIFICATION:
            return classification_average;
          case MiningFunction::MiningFunctionType::REGRESSION:
            return regression_average;
        }
      case MultipleModelMethodType::WEIGHTED_AVERAGE:
        return classification_weighted_average;
      case MultipleModelMethodType::SUM:
        return sum;
      case MultipleModelMethodType::MODEL_CHAIN:
        return model_chain;
    }

    throw cpmml::ParsingException(multiplemodelmethod + " not supported");
  }

#ifndef MULTITHREADING

  static std::unique_ptr<InternalScore> majority_vote(const Sample &sample, const std::vector<Segment> &ensemble) {
    std::unordered_map<std::string, double> probabilities;  // zero initialized

    std::string score;
    for (const auto &segment : ensemble)
      if (segment.predicate(sample)) probabilities[segment.predict(sample)] += 1.0 / ensemble.size();

    double max_prob = 0;
    for (const auto &probability : probabilities) {
      if (max_prob > 0.5) break;

      if (probability.second > max_prob && probability.first != "") {
        max_prob = probability.second;
        score = probability.first;
      }
    }

    return make_unique<InternalScore>(score, probabilities);
  }

#else
  inline static std::unique_ptr<InternalScore> majority_vote(const Sample &sample,
                                                             const std::vector<Segment> &ensemble) {
    std::unordered_map<std::string, double> probabilities;  // zero initialized
    std::unordered_map<std::string, double> tmp_probabilities[NUM_THREADS];

    std::string score;
#pragma omp parallel for if (ensemble.size() > 25) default(shared) num_threads(NUM_THREADS)
    for (auto i = 0u; i < ensemble.size(); i++)
      if (ensemble[i].predicate(sample))
        tmp_probabilities[omp_get_thread_num()][ensemble[i].predict(sample)] += 1.0 / ensemble.size();

    for (auto i = 0u; i < NUM_THREADS; i++)
      for (const auto &pair : tmp_probabilities[i]) probabilities[pair.first] += pair.second;

    double max_prob = 0;
    for (const auto &probability : probabilities) {
      if (max_prob > 0.5) break;

      if (probability.second > max_prob && probability.first != "") {
        max_prob = probability.second;
        score = probability.first;
      }
    }

    return make_unique<InternalScore>(score, probabilities);
  }

#endif

  inline static std::unique_ptr<InternalScore> weighted_majority_vote(const Sample &sample,
                                                                      const std::vector<Segment> &ensemble) {
    std::unordered_map<std::string, double> probabilities;  // zero initialized

    for (const auto &segment : ensemble)
      if (segment.predicate(sample)) probabilities[segment.predict(sample)] += 1.0 * segment.weight / ensemble.size();

    double max_prob = 0;
    std::string score;
    double winning_threshold = 1.0 / ensemble[0].model->target_field.n_values;
    for (const auto &probability : probabilities) {
      if (max_prob > winning_threshold) break;

      if (probability.second > max_prob && probability.first != "") {
        max_prob = probability.second;
        score = probability.first;
      }
    }

    return make_unique<InternalScore>(score, probabilities);
  }

  inline static std::unique_ptr<InternalScore> classification_average(const Sample &sample,
                                                                      const std::vector<Segment> &ensemble) {
    std::unique_ptr<InternalScore> first_score(ensemble[0].score(sample));
    std::unordered_map<std::string, double> probabilities = first_score->probabilities;

    for (auto i = 1u; i < ensemble.size(); i++)
      if (ensemble[i].predicate(sample)) {
        std::unique_ptr<InternalScore> tmp_score(ensemble[i].score(sample));
        for (const auto &probability : tmp_score->probabilities) probabilities[probability.first] += probability.second;
      }

    for (const auto &probability : probabilities)
      probabilities[probability.first] = probability.second / ensemble.size();

    double max_prob = 0;
    std::string score;
    double winning_threshold = 1.0;  // / ensemble[0].model->target_field.n_values;
    for (const auto &probability : probabilities) {
      if (max_prob >= winning_threshold) break;

      if (probability.second > max_prob && probability.first != "") {
        max_prob = probability.second;
        score = probability.first;
      }
    }

    return make_unique<InternalScore>(score, probabilities);
  }

#ifndef MULTITHREADING
  static std::unique_ptr<InternalScore> regression_average(const Sample &sample, const std::vector<Segment> &ensemble) {
    double score = 0;
    double count = 0;

    for (const auto &segment : ensemble)
      if (segment.predicate(sample)) {
        count++;
        score += to_double(segment.predict(sample));
      }

    score /= count;

    return make_unique<InternalScore>(score);
  }
#else
  inline static std::unique_ptr<InternalScore> regression_average(const Sample &sample,
                                                                  const std::vector<Segment> &ensemble) {
    double score = 0;
    double scores[NUM_THREADS];
    double count = 0;

    for (auto i = 0u; i < NUM_THREADS; i++) scores[i] = 0;

#pragma omp parallel for if (ensemble.size() > 25) default(shared) num_threads(NUM_THREADS)
    for (auto i = 0u; i < ensemble.size(); i++)
      if (ensemble[i].predicate(sample)) {
        count++;
        scores[omp_get_thread_num()] += to_double(ensemble[i].predict(sample));
      }

    for (auto i = 0u; i < NUM_THREADS; i++) score += scores[i];

    score /= count;

    return make_unique<InternalScore>(score);
  }
#endif

  inline static std::unique_ptr<InternalScore> classification_weighted_average(const Sample &sample,
                                                                               const std::vector<Segment> &ensemble) {
    std::unique_ptr<InternalScore> first_score(ensemble[0].score(sample));
    std::unordered_map<std::string, double> probabilities = first_score->probabilities;

    for (auto i = 1u; i < ensemble.size(); i++)
      if (ensemble[i].predicate(sample)) {
        std::unique_ptr<InternalScore> tmp_score(ensemble[i].score(sample));
        for (const auto &probability : tmp_score->probabilities)
          probabilities[probability.first] += probability.second * ensemble[i].weight;
      }

    for (const auto &probability : probabilities)
      probabilities[probability.first] = probability.second / ensemble.size();

    double max_prob = 0;
    std::string score;
    double winning_threshold = 1.0 / ensemble[0].model->target_field.n_values;
    for (const auto &probability : probabilities) {
      if (max_prob >= winning_threshold) break;

      if (probability.second > max_prob && probability.first != "") {
        max_prob = probability.second;
        score = probability.first;
      }
    }

    return make_unique<InternalScore>(score, probabilities);
  }

#ifndef MULTITHREADING

  inline static std::unique_ptr<InternalScore> sum(const Sample &sample, const std::vector<Segment> &ensemble) {
    double score = 0;

    for (const auto &segment : ensemble)
      if (segment.predicate(sample)) score += std::unique_ptr<InternalScore>(segment.score(sample))->double_score;

    return make_unique<InternalScore>(score);
  }

#else
  inline static std::unique_ptr<InternalScore> sum(const Sample &sample, const std::vector<Segment> &ensemble) {
    double score = 0;
    double scores[NUM_THREADS];

    for (auto i = 0u; i < NUM_THREADS; i++) scores[i] = 0;

#pragma omp parallel for if (ensemble.size() > 25) default(shared) num_threads(NUM_THREADS)
    for (auto i = 0u; i < ensemble.size(); i++)
      if (ensemble[i].predicate(sample))
        scores[omp_get_thread_num()] += std::unique_ptr<InternalScore>(ensemble[i].score(sample))->double_score;

    for (auto i = 0u; i < NUM_THREADS; i++) score += scores[i];

    return make_unique<InternalScore>(score);
  }
#endif

  inline static std::unique_ptr<InternalScore> model_chain(const Sample &sample, const std::vector<Segment> &ensemble) {
    Sample tmp_sample = sample;
    bool first = true;

    for (auto i = 0u; i < ensemble.size() - 1; i++)
      if (ensemble[i].predicate(tmp_sample)) {
        if (first) {
          ensemble[i].model->augment_first(tmp_sample);
          first = false;
        } else {
          ensemble[i].model->augment(tmp_sample);
        }
      }

    return ensemble.back().model->augment_last(tmp_sample);
  }
};

#endif
