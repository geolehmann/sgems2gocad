#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <ostream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <typeinfo>
#include "OpenFileDialog.h"

std::vector<double>data;


int nx, ny, nz;
int cellx, celly, cellz;
double startx, starty, startz;
std::string name;

int main(int argc, char *argv[])
{
	std::cout << "SGEMS to SKUA-GOCAD Converter\n";
	std::cout << "==================================\n\n";
	std::cout << "(c) Christian Lehmann 2019\n\n\n";

	// Grid laden
	char filter_grid[] = "SGemS-Grid (*.grd)\0*.grd\0"; //Object Files(*.obj)\0*.obj\0Text Files(*.txt)\0*.txt\0All Files
	char title_grid[] = "Grid-Datei laden";
	std::string filename_grid = openDialog(title_grid, filter_grid);
	std::ifstream ifs_grid(filename_grid.c_str(), std::ifstream::in);
	if (!ifs_grid.good())
	{
		std::cout << "Error loading:" << filename_grid << " No file found!" << "\n";
		system("PAUSE");
		exit(0);
	}

	// Daten laden
	char filter_data[] = "SGemS Daten (*.*)\0*.*\0"; //Object Files(*.obj)\0*.obj\0Text Files(*.txt)\0*.txt\0All Files
	char title_data[] = "Daten laden";
	std::string filename_data = openDialog(title_data, filter_data);
	std::ifstream ifs_data(filename_data.c_str(), std::ifstream::in);
	if (!ifs_data.good())
	{
		std::cout << "Error loading:" << filename_data << " No file found!" << "\n";
		system("PAUSE");
		exit(0);
	}



	// Grid einlesen
	std::string  line;
	int linenumber = 0;
	while (!ifs_grid.eof() && std::getline(ifs_grid, line))
	{
		std::stringstream stringstream(line);
		
		if (linenumber == 0) {} // in erster Zeile ist nur Gridname
		if (linenumber == 1) { nx = std::stoi(line); }
		if (linenumber == 2) { ny = std::stoi(line); }
		if (linenumber == 3) { nz = std::stoi(line); }

		if (linenumber == 4) { cellx = std::stoi(line); }
		if (linenumber == 5) { celly = std::stoi(line); }
		if (linenumber == 6) { cellz = std::stoi(line); }

		if (linenumber == 7) { startx = std::stod(line); }
		if (linenumber == 8) { starty = std::stod(line); }
		if (linenumber == 9) { startz = std::stod(line); }
		linenumber++;
	}



	// Daten einlesen
	line = "";
	linenumber = 0;
	while (!ifs_data.eof() && std::getline(ifs_data, line))
	{
		std::stringstream stringstream(line);

		if (linenumber == 0) {}
		if (linenumber == 1) {}
		if (linenumber == 2) { name = line; }
		if (linenumber > 2) data.push_back(std::stod(line));

		linenumber++;
	}





	fprintf_s(stderr, "test2");

	// GOCAD ASCII erstellen
	std::cout << "Schreibe GOCAD - Datei...\n";

	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "%d.%m.%Y_%HUhr%M");
	auto str = oss.str();
	std::string xmlfilename = name + "_Export_";
	std::ofstream xmlfile;
	xmlfile.open(xmlfilename + str + ".vs");



	char header1[] = R"=====(GOCAD VSet 1 
HEADER {
*atoms*size: 4
name: )=====";

	xmlfile << header1 << name << std::endl;

	char header2[] = R"=====(}
GOCAD_ORIGINAL_COORDINATE_SYSTEM
NAME " Pdgm_Epic Local" 
PROJECTION Unknown
DATUM Unknown
AXIS_NAME X Y Z
AXIS_UNIT m m m
ZPOSITIVE Elevation
END_ORIGINAL_COORDINATE_SYSTEM
PROPERTIES )====="; 
xmlfile << header2 << name;

char header3[] = R"=====(
PROPERTY_CLASS_HEADER X {
kind: X
unit: m
pclip: 99
}
PROPERTY_CLASS_HEADER Y {
kind: Y
unit: m
pclip: 99
}
PROPERTY_CLASS_HEADER Z {
kind: Depth
unit: m
is_z: on
pclip: 99
}
PROPERTY_CLASS_HEADER )=====";
xmlfile << header3 << name;
	
char header4[] = R"=====(
kind: Real Number
unit: unitless
}
)=====";

xmlfile << header4;

	for (int i=0; i<data.size();i++)
	{
		double xneu, yneu, zneu;
		int blockx, blocky, blockz;

		blockz = 1 + (int)((i - 1) / (nx*ny));
		blocky = 1 + (int)((i - (blockz - 1)*nx*ny) / nx);
		blockx = i - (blockz - 1)*nx*ny - ((blocky - 1)*nx);


		xneu = startx + (blockx*cellx);
		yneu = starty + (blocky*celly);
		zneu = startz + (blockz*cellz);

		xmlfile << "PVRTX " << i << " " << std::to_string(xneu) << " " << std::to_string(yneu) << " " << std::to_string(zneu) << " " << std::to_string(data.at(i)) << std::endl;
	}

	xmlfile << "END";
}