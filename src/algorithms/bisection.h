//============================================================================
// @name        : none.h
// @author      : Thomas Dooms
// @date        : 11/16/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include <vector>
#include <algorithm>
#include <tuple>

#include "../util/variable.h"
#include "../util/production.h"
#include "../util/settings.h"

namespace algorithm::bisection {

	std::tuple<Settings, std::vector<Variable>, std::vector<Production>>
	compress(const std::vector<uint16_t>& pairs, bool odd) {
		const auto settings = Settings();
		std::vector<Production> productions;
		productions.reserve(pairs.size() / 2);

		std::vector<Variable> variables(pairs.begin(), pairs.end());
		std::for_each(variables.begin(), variables.end() - 1, [](auto& elem) { elem += 256; });

		if (not odd) variables.back() += 256;

		bool startReached = false;
		std::unordered_map<Production, Variable> map;
		map.reserve(pairs.size());

		uint32_t offset = 0;
		size_t size = variables.size();

		while (not startReached) {
			bool uneven = size % 2 != 0;

			for (size_t i = 0; i < size / 2; ++i) {
				const auto pair = map.try_emplace(Production{variables[2 * i], variables[2 * i + 1]},
				                                  settings.offset(offset));
				if (pair.second) {
					++offset;
					productions.emplace_back((*pair.first).first);
				}

				variables[i] = (*pair.first).second;
			}
			if (uneven) variables[(size / 2)] = variables[size - 1];

			size = (size / 2) + uneven;

			startReached = size == 1;
		}

		variables.resize(size);

		return std::make_tuple(settings, std::move(variables), std::move(productions));
	}

}

namespace algorithm::optimisedBisection {

	std::tuple<Settings, std::vector<Variable>, std::vector<Production>>
	compress(const std::vector<uint16_t>& pairs, bool odd) {
		const auto settings = Settings();

		std::vector<Production> productions;
		std::unordered_map<Production, Variable> map;
		std::vector<bool> used;
		productions.reserve(pairs.size() / 2);
		map.reserve(pairs.size() / 2);
		used.reserve(pairs.size() / 2);

		std::vector<Variable> variables(pairs.begin(), pairs.end());
		std::for_each(variables.begin(), variables.end() - 1, [](auto& elem) { elem += 256; });
		if (not odd) variables.back() += 256;

		uint32_t offset = 0;
		uint32_t previousLevelBegin = 0;
		uint32_t levelBegin = settings.begin();
		size_t size = variables.size();

		while (levelBegin != previousLevelBegin) {
			size_t index = 0;

			for (size_t i = 0; i < size / 2; ++i) {
				const auto& var1 = variables[2 * i];
				const auto& var2 = variables[2 * i + 1];

				// enkel variabelen van vorige level die kunnen herhaald worden
				if (size != variables.size()) {
					if (var1 < previousLevelBegin || var2 < previousLevelBegin ||
					    !used[var1 - settings.begin()] || !used[var2 - settings.begin()]) {
						variables[index++] = var1;
						variables[index++] = var2;
						continue;
					}
				}

				const auto pair = map.try_emplace(Production{var1, var2}, settings.offset(offset));
				if (pair.second) {
					++offset;
					productions.emplace_back((*pair.first).first);
					used.emplace_back(false);
				} else {
					used[(*pair.first).second - settings.begin()] = true;
				}
				variables[index++] = (*pair.first).second;
			}
			if (size % 2 == 1) variables[index++] = variables[size - 1];

			size = index;
			previousLevelBegin = levelBegin;
			levelBegin = settings.offset(offset);
		}

		variables.resize(size);

		return std::make_tuple(settings, std::move(variables), std::move(productions));
	}

}