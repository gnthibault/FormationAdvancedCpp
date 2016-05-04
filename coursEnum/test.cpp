#include <iostream>
#include <cstdlib>
#include <vector>


enum class Color {blue, red, yellow};
enum class Feeling {happy, sad};

enum class perfEnum : unsigned char {ref0, ref1};

//g++ ./test.cpp -std=c++11 -o ./test
int main(int argc, char* argv[])
{
	Color c = Color::blue;

	//Erreur
	//Color c1 = Feeling::happy;
	//Erreur
	//Color c2 = 1;


	return EXIT_SUCCESS;
}
