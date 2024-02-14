#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <istream>
#include <iostream>
namespace Ini {
	class Section {
	public:
		Section(std::string section_name);
		void add_var(std::string var_name, std::string var_value);
		std::unordered_map<std::string, std::string> vars;
	private:
		std::string name;
	};

	class Document {
	public:
		Document() = default;
		Section* AddSection(std::string name);
		const Section& GetSection(const std::string& name) const;
		size_t SectionCount() const;
		void write_ini_file(std::string file_name);
	private:
		std::unordered_map<std::string, Section*> sections;
	};
	Document* Load(std::string input);
}