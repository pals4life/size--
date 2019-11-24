//
// Created by ward on 11/23/19.
//

#include "type.h"
#include <algorithm>

std::istream& operator>>(std::istream& in, Algorithm& algorithm) {
	std::string token;
	in >> token;
	std::transform(token.begin(), token.end(), token.begin(), ::tolower);
	if (token == "0" || token == "none")
		algorithm = Algorithm::none;
	else if (token == "1" || token == "bisection")
		algorithm = Algorithm::bisection;
	else if (token == "2" || token == "bisection++" || token == "bisectionplusplus")
		algorithm = Algorithm::bisectionPlusPlus;
	else if (token == "3" || token == "bisection++++" || token == "bisectionplusplusPlusPlus")
		algorithm = Algorithm::bisectionPlusPlusPlusPlus;
	else if (token == "4" || token == "repair" || token == "re-pair")
		algorithm = Algorithm::repair;
	else if (token == "5" || token == "sequitur")
		algorithm = Algorithm::sequitur;
	else if (token == "6" || token == "sequential")
		algorithm = Algorithm::sequential;
	else
		in.setstate(std::ios_base::failbit);
	return in;
}