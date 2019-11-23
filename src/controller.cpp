//
// Created by ward on 11/23/19.
//

#include <iostream>
#include "controller.h"
#include "pal.h"

using namespace boost::program_options;

Controller::Controller(int argc, char** argv) : desc(options_description("Options")) {
	desc.add_options()
			("help,h", "produce this message")
			("create,c", "create a new archive")
			("separate,s", "compress each file into a separate archive if multiple files are given")
			("algorithm,a", value<std::string>(), "specify a compression algorithm (TODO list algorithms)")
			("optimum,o", "try all algorithms and keep the most compressed archive")
			("verbose,v", "be verbose")
			("extract,x", "extract file(s) from an archive");

	options_description hidden;
	hidden.add_options()
			("files", value<std::vector<std::string>>(), "input file(s)");

	options_description combined;
	combined.add(desc).add(hidden);

	positional_options_description pos;
	pos.add("files", -1);

	try {
		store(command_line_parser(argc, argv).options(combined).positional(pos).run(), vm);
		notify(vm);

		checkOptions();
	} catch (const error& ex) {
		returnValue = 1;
		std::cout << ex.what() << "\n\n";
		printHelp();
		return;
	}

	execute();
}

void Controller::checkOptions() const {
	if (!vm.count("files")) throw error("need at least one file to run on");
	if (!(vm.count("create") ^ vm.count("extract"))) throw error("need to specify creation or extraction");
}

void Controller::execute() const {
	if (vm.count("help")) {
		printHelp();
		return;
	}

	auto files = vm["files"].as<std::vector<std::string>>();

	if (vm.count("create")) {
		compress(files);
	} else if (vm.count("extract")) {
		extract(files);
	}
}

int Controller::getReturnValue() const {
	return returnValue;
}

void Controller::printHelp() const {
	std::cout << desc;
}

void Controller::compress(const std::vector<std::string>& files) const {
	if (vm.count("separate") || files.size() == 1) {
		for (const auto& file: files) {
		}
	}
}

void Controller::extract(const std::vector<std::string>& files) const {

}

