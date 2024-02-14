#include "ini.h"

int main()
{
	Ini::Document* ini = new Ini::Document();
	ini = Ini::Load("file.ini");
	ini->write_ini_file("out.ini");
	return 0;
}