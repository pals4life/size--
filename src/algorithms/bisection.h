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
		const auto settings = Settings(Settings::Flags::smart_productions | Settings::Flags::reserved);
		std::vector<Production> productions;
		productions.reserve(pairs.size() / 2);

		std::vector<Variable> variables(pairs.begin(), pairs.end());
		std::for_each(variables.begin(), variables.end() - 1, [](auto& elem) { elem += 256; });

		if (not odd) variables.back() += 256;
		else variables.back() >>= 8u;


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
			if (uneven) variables[(size / 2)] = variables.back();

			size = (size / 2) + uneven;

			startReached = size == 1;
		}

		variables.resize(size);

//		uint32_t offset = 0;
//		std::map<Production, std::pair<Variable, bool>> map;
//		std::vector<Variable> newVariables;
//
//		for (size_t i = 0; i < variables.size() / 2; ++i) {
//			const auto& char1 = variables.at(2 * i);
//			const auto& char2 = variables.at(2 * i + 1);
//			const Production production{char1, char2};
//
//			const auto varPointer = map.find(production);
//
//			if (varPointer == map.end()) {
//				Variable var = settings.offset(offset++);
//
//				map.emplace(production, std::make_pair(var, false));
//				productions.emplace_back(production);
//
//				newVariables.emplace_back(var);
//			} else {
//				(*varPointer).second.second = true;
//
//				newVariables.emplace_back((*varPointer).second.first);
//			}
//		}
//		if (variables.size() % 2 == 1) {
//			newVariables.emplace_back(variables.back());
//		}

		return std::make_tuple(settings, std::move(variables), std::move(productions));
	}
}