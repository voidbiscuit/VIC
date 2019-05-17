
#include "FreenectExampleModded/FreenectExampleModded.h"
#include "VisualEyes/VisualEyes.h"
#include <conio.h>

using namespace std;

// Data
string
splash = \
"\n                              "\
"\n       Timothy Nicholls       "\
"\n      Visual Intelligence     "\
"\n                              "\
"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n",
menu = \
"\n                              "\
"\n     Visual Intelligence      "\
"\n                              "\
"\n  F - Freenect Example        "\
"\n  V - VisualEyes              "\
"\n                              "\
"\n  Q - Quit                    "\
"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"\
"";

// Values
char key = ' ';
// Functions
void Clear() { for (int _ = 0; _ < 100; _++) printf("\n"); }

void Splash() {
	Clear();
	cout << splash;
	_sleep(1000);
}




// Main
int main(int argc, char * argv[])
{
	// Init Data
	string filepath = "F:/Work/Year 3/Visual Intelligence/";
	FreenectExampleModded freenectExampleModded = FreenectExampleModded(filepath+"images/", filepath + "Set1/");
	VisualEyes visualEyes = VisualEyes(filepath + "images/", "obj", "__", 20);

	// Splash
	Splash();

	// Program Loop
	while (key != 'Q') {
		Clear();
		cout << menu;
		key = _getche();
		key = key > 'Z' ? key - ' ' : key;
		Clear();
		switch (key) {
		case 'F':
			freenectExampleModded.Start();
			break;
		case 'V':
			visualEyes.Start();
			break;
		default:
			break;
		};
	}

}