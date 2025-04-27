// INFO2 HKA
// Rotate Pac Man Levels

#include <iostream>
using namespace std;

int main()
{
	string level[] = {
		"L--------H|.|L----H",
		"|........||.||....|",
		"|.L----H.KJ.KJ.LH.|",
		"|.|    |.......KJ.|",
		"|.K----J.L-H......|",
		"|........K-J.L--H.|",
		"|.L--H.......|  |.|",
		"|.K--w-------w--J.|",
		"|.................|",
		"K---H.L-H L-H.L---J",
		"    |.|LJ KH|.|    ",
		"----J.||   ||.K----",
		"......||   ||......",
		"----H.||   ||.L----",
		"    |.|KH LJ|.|    ",
		"L---J.K-J K-J.K---H",
		"|.................|",
		"|.LH.LH.L-x----xH.|",
		"|.||.||.K-J....||.|",
		"|.||.||.....LH.||.|",
		"|.|| Kw-----wJ.KJ.|",
		"|.||..............|",
		"|.KJ.LH.LH.LH.L-H.|",
		"|....||.||.||.| |.|",
		"K----J|.|K-ww-w-w-J"
	};

	// replacements
	string repSrc = "-|KH";
	string repDst = "|-HK";

	// output dimensions
	for (int ri = 0; ri < 19; ri++)
	{
		string row = "";
		for (int ci = 0; ci < 25; ci++)
		{
			char p = level[ci][ri];

			int i = repSrc.find(p);
			if (i >= 0)
				p = repDst[i];

			row += p;
		}
		cout << "\"" << row << "\"," << endl;
	}
}

