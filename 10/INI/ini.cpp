#include <sstream>
#include "ini.h"
using namespace Ini;

Section::Section(std::string section_name) : name{ section_name } {};
void Section::add_var(std::string var_name, std::string var_value) {
	std::pair<std::string, std::string> p;
	vars.insert(std::make_pair(var_name, var_value));
};
Section* Document::AddSection(std::string name) {
	Section* new_section = new Section(name);
	sections.insert(std::make_pair(name, new_section));
	return new_section;
};
const Section& Document::GetSection(const std::string& name) const {
	return *sections.at(name);
};
size_t Document::SectionCount() const {
	return sections.size();
}
void Ini::Document::write_ini_file(std::string file_name){
	std::ofstream out(file_name);
	std::string sec_name;
	Section* sec = nullptr;
	for(auto kv : sections) {
		out << '[' << kv.first << "]\n";
		sec = kv.second;
		for (auto kv2 : sec->vars) {
			out << kv2.first << '=' << kv2.second << "\n";
		}
	}
};
Document* Ini::Load(std::string input){
	std::string a;
	Document* ini = new Document();
	std::ifstream in(input);
	Section* curr_section = nullptr;
	while (getline(in, a)) {
		std::stringstream str(a);
		std::string sec_name = "";
		char symbol;
		str >> symbol;
		if (symbol == ';' || symbol == '#')
			continue;
		if (symbol == '['){
			str >> symbol;
			while (symbol != ']' && symbol != '\n'){
				sec_name += symbol;
				str >> symbol;
			}
			Section* section = ini->AddSection(sec_name);
			curr_section = section;
			continue;
		}
		int i = 0;
		int length = a.size() - 1;
		std::string var_name = "";
		std::string var_value = "";
		while (symbol != '=') {
			var_name += symbol;
			str >> symbol;
			++i;
		}
		str >> symbol;
		while (symbol != '\n' && symbol != ';' && symbol != '#' && i < length) {
			var_value += symbol;
			str >> symbol;
			++i;
		}
		curr_section->add_var(var_name, var_value);
	}
	return ini;
}
