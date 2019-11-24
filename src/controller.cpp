//
// Created by ward on 11/23/19.
//

#include <iostream>
#include "controller.h"
#include "pal.h"

using namespace boost::program_options;

Controller::Controller(int argc, char** argv) : desc(options_description("options")) {
	desc.add_options()
			("help,h", "produce this message")
			("verbose,v", "be verbose")
			("output,o", value<std::filesystem::path>(), "output file (optional)")
			("create,c", value<Algorithm>(), "create a new archive with a specified algorithm: (TODO list algorithms)")
			("extract,x", "extract file(s) from an archive");

	options_description hidden;
	hidden.add_options()
			("files", value<std::vector<std::filesystem::path>>(&files), "input file(s)");

	options_description combined;
	combined.add(desc).add(hidden);

	positional_options_description pos;
	pos.add("files", -1);

	try {
		store(command_line_parser(argc, argv).options(combined).positional(pos).run(), vm);
		notify(vm);

		if (vm.count("help")) {
			printHelp();
			return;
		}

		checkOptions();
	} catch (const error& ex) {
		returnValue += 1;
		std::cout << std::endl << ex.what() << "\n\n";
		printHelp();
		return;
	}

	execute();
}

void Controller::checkOptions() {
	if (!(vm.count("create") ^ vm.count("extract"))) throw error("need to specify creation or extraction");
	if (!vm.count("files")) throw error("need at least one file to run on");
	if (files.size() > 1) {
		tar = true;
		if(vm.count("extract")) throw error("only one pal archive can be extracted");
	}
	for (const auto& file: files) {
		if (!std::filesystem::exists(file)) throw error("file '" + file.string() + "' is not found");
		if (std::filesystem::is_directory(file)) tar = true;
	}
}

void Controller::execute() {
	if (vm.count("create")) {
		compress();
	} else if (vm.count("extract")) {
		extract();
	}
}

int Controller::getReturnValue() const {
	return returnValue;
}

void Controller::printHelp() const {
	std::cout << "Usage: SIZE-- [options] <files>\n";
	std::cout << desc;
}

void Controller::compress() {
	auto temp = std::filesystem::temp_directory_path() / "tempsizeminmin.tar";
	auto in = files[0];
	auto out = std::filesystem::path(in.string() + ".pal");

	if (tar) {
		std::string command = "tar -cf " + temp.string();
		for (const auto& file: files) {
			command += " " + file.string();
		}
		command += " >/dev/null 2>&1";
		returnValue += system(command.c_str());
		in = temp.string();
	}

	if (vm.count("output")) out = vm["output"].as<std::filesystem::path>();
	pal::encode(in, out, vm["create"].as<Algorithm>());
}

void Controller::extract() const {
	auto temp = std::filesystem::temp_directory_path() / "tempsizeminmin.tar";
	auto in = files[0];
	auto out = in;
	out.replace_extension("");

	if (vm.count("output")) out = vm["output"].as<std::filesystem::path>();
	pal::encode(in, out, vm["create"].as<Algorithm>());

	if (tar) {
		std::string command = "tar -xf " + temp.string();
		command += " >/dev/null 2>&1";
		returnValue += system(command.c_str());
		in = temp.string();
	}
}

