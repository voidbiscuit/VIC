#include "FenVIC/FenVIC.h"
#include "VisuII/VisuII.h"
#include <conio.h>
#define cls(); system("cls");

using namespace std;

// Main
int main(int argc, char * argv[])
{
	// Init Data
	string filepath = "F:/Work/Year 3/Visual Intelligence/";
	FenVIC _FenVIC = FenVIC(filepath + "images/", filepath + "Set1/");
	VisuII _VisuII = VisuII(filepath + "images/", "obj", "__", 20);
	char key = -1;

	// Splash
	cls();
	cout <<
		"\n                              "\
		"\n           CX004763           "\
		"\n      Visual Intelligence     "\
		"\n                              "\
		;
	_sleep(1000);
	cls();
	// VIC
	while (key != 'Q') {
		cls();
		cout <<
			"\n                              "\
			"\n     Visual Intelligence      "\
			"\n                              "\
			"\n  F - FenVIC (Freenect)       "\
			"\n  V - VisuII (ex-Project)     "\
			"\n                              "\
			"\n  Q - Quit                    "\
			"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"\
			"";
		key = _getche();
		//key = 'F';
		key = key > 'Z' ? key - ' ' : key;
		cls();
		switch (key) {
		case 'F':
			_FenVIC.Start();
			break;
		case 'V':
			_VisuII.Start();
			break;
		default:
			break;
		};
	}

}