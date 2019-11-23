//
// Created by ward on 11/23/19.
//

#ifndef SIZE_CONTROLLER_H
#define SIZE_CONTROLLER_H

#include <boost/program_options.hpp>

class Controller {
public:
	Controller(int argc, char** argv);

	[[nodiscard]] int getReturnValue() const;

private:
	boost::program_options::variables_map vm;
	boost::program_options::options_description desc;
	int returnValue = 0;

	void checkOptions() const;

	void execute() const;

	void compress(const std::vector<std::string>& files) const;

	void extract(const std::vector<std::string>& files) const;

	void printHelp() const;
};


#endif //SIZE_CONTROLLER_H
