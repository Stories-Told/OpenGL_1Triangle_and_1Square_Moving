#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Used to read in .glsl txt files for vertex and fragment shaders
string readShaderSource(const char *filePath)
{
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";

	while (!fileStream.eof())
	{
		getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}
