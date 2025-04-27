#include "LevelInit.h"

void InitPresetLevelsVert(int& levelNum, TileMap* levels[LEVEL_Max])
{
	string x1[] = {
		"L-----------------H",
		"|*...............*|",
		"|.L-H.L-H.L-----H.|",
		"|.| |.| |.|     |.|",
		"|.K-J.K-J.K-----J.|",
		"|.................|",
		"|.L-H.L-----H.L-H.|",
		"|.K-J.K-----J.K-J.|",
		"|.................|",
		"K---H.L-H L-H.L---J",
		"    |.|LJ KH|.|    ",
		"----J.||   ||.K----",
		"......||   ||......",
		"----H.||   ||.L----",
		"    |.|KH LJ|.|    ",
		"L---J.K-J K-J.K---H",
		"|.................|",
		"|.L-H.L-H.L-H.L-H.|",
		"|.| |.u-J.u-J.| |.|",
		"|.K-J.|...|...| |.|",
		"|.....|.--J.L-J |.|",
		"|.L-H.|.....|   |.|",
		"|.K-J.K--.--w---J.|",
		"|*...............*|",
		"K-----------------J"
	};
	TileMap* tm1 = new TileMap(SIZE_OF_ARR(x1), x1);
	tm1->GateToLeft = Vec2(0, 12);
	tm1->GateToRight = Vec2(18, 12);
	tm1->PlayerStartPos[0] = Vec2(9, 21);
	tm1->PlayerStartPos[1] = Vec2(9, 5);
	tm1->GhostStartPos[0] = Vec2(8, 11);
	tm1->GhostStartPos[1] = Vec2(10, 11);
	tm1->GhostStartPos[2] = Vec2(8, 13);
	tm1->GhostStartPos[3] = Vec2(10, 13);
	tm1->GhostHomeZone[0] = Vec2(3, 3);
	tm1->GhostHomeZone[1] = Vec2(3, 18);
	tm1->GhostHomeZone[2] = Vec2(13, 3);
	tm1->GhostHomeZone[3] = Vec2(14, 21);
	tm1->PlayerTextScorePos[0] = Vec2(0, 10);
	tm1->PlayerTextScorePos[1] = Vec2(0, 14);
	tm1->PlayerTextExtraPos[0] = Vec2(15, 10);
	tm1->PlayerTextExtraPos[1] = Vec2(15, 14);
	tm1->MessagePos = Vec2(11, 3);
	levels[levelNum++] = tm1;

	string x2[] = {
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
	TileMap* tm2 = new TileMap(SIZE_OF_ARR(x2), x2);
	tm2->GateToLeft = Vec2(0, 12);
	tm2->GateToRight = Vec2(18, 12);
	tm2->GateToTop = Vec2(11, 0);
	tm2->GateToBottom = Vec2(7, 24);
	tm2->PlayerStartPos[0] = Vec2(9, 21);
	tm2->PlayerStartPos[1] = Vec2(9, 5);
	tm2->GhostStartPos[0] = Vec2(8, 11);
	tm2->GhostStartPos[1] = Vec2(10, 11);
	tm2->GhostStartPos[2] = Vec2(8, 13);
	tm2->GhostStartPos[3] = Vec2(10, 13);
	tm2->GhostHomeZone[0] = Vec2(5, 3);
	tm2->GhostHomeZone[1] = Vec2(6, 15);
	tm2->GhostHomeZone[2] = Vec2(3, 19);
	tm2->GhostHomeZone[3] = Vec2(13, 19);
	tm2->PlayerTextScorePos[0] = Vec2(0, 10);
	tm2->PlayerTextScorePos[1] = Vec2(0, 14);
	tm2->PlayerTextExtraPos[0] = Vec2(15, 10);
	tm2->PlayerTextExtraPos[1] = Vec2(15, 14);
	tm2->MessagePos = Vec2(3, 3);
	levels[levelNum++] = tm2;
}

void LevelInit::InitPresetLevels(int& levelNum, TileMap* levels[LEVEL_Max])
{
	string x1[] = {
		"L--------H |.| L--------H",
		"|*.......| |.| |.......*|",
		"|.L-H.LH.| |.| |.L-H.LH.|",
		"|.| |.||.K-J.K-J.| |.||.|",
		"|.K-J.KJ.........K-J.KJ.|",
		"|........L-----H........|",
		"|.L-H.LH.|     |.Lx---H.|",
		"|.| |.||.|L---H|.||...|.|",
		"|.K-J.||.KJ   KJ.KJ.|.|.|",
		"|.....||.       ....|...|",
		"|.L-H.||.LH   LH.Lx-J.|.|",
		"|.| |.||.|K---J|.||...|.|",
		"|.| |.KJ.|     |.KJ.L-v.|",
		"|.| |....K-----J....| |.|",
		"|.| |.LH.........L--J |.|",
		"|.| |.||.L-H.L-H.|    |.|",
		"|.K-J.KJ.| |.| |.K----J.|",
		"|*.......| |.| |.......*|",
		"K--------J |.| K--------J",
	};

	TileMap* tm1 = new TileMap(SIZE_OF_ARR(x1), x1);
	tm1->GateToTop = Vec2(12, 0);
	tm1->GateToBottom = Vec2(12, 18);
	tm1->PlayerStartPos[0] = Vec2(21, 9);
	tm1->PlayerStartPos[1] = Vec2(5, 9);
	tm1->GhostStartPos[0] = Vec2(11, 8);
	tm1->GhostStartPos[1] = Vec2(11, 10);
	tm1->GhostStartPos[2] = Vec2(13, 8);
	tm1->GhostStartPos[3] = Vec2(13, 10);
	tm1->GhostHomeZone[0] = Vec2(3, 3);
	tm1->GhostHomeZone[1] = Vec2(18, 3);
	tm1->GhostHomeZone[2] = Vec2(3, 13);
	tm1->GhostHomeZone[3] = Vec2(21, 14);
	tm1->PlayerTextScorePos[0] = Vec2(10, 6);
	tm1->PlayerTextScorePos[1] = Vec2(18, 15);
	tm1->PlayerTextExtraPos[0] = Vec2(10, 12);
	// tm1->PlayerTextExtraPos[1] = Vec2(15, 14);
	tm1->MessagePos = Vec2(10, 12);
	levels[levelNum++] = tm1;

	string x2[] = {
		"L---------H|.|L---------H",
		"|.........KJ.KJ.........|",
		"|.L--H.LH.......LH.L--H.|",
		"|.|  |.|K--H.L--J|.|  |.|",
		"|.K--J.K---J.K---J.K--J.|",
		"|.......................|",
		"K--H.L-H.L-H L-H.L------J",
		"   |.| |.|LJ KH|.|       ",
		"---J.K-J.||   ||.K-------",
		".........||   ||.........",
		"-------H.||   ||.L-H.L---",
		"       |.|KH LJ|.| |.|   ",
		"L------J.K-J K-J.K-J.K--H",
		"|.......................|",
		"|.L--H.L---H.L---H.L--H.|",
		"|.|  |.|L--J.K--H|.|  |.|",
		"|.K--J.KJ.......KJ.K--J.|",
		"|.........LH.LH.........|",
		"K---------J|.|K---------J"
	};

	TileMap* tm2 = new TileMap(SIZE_OF_ARR(x2), x2);
	tm2->GateToLeft = Vec2(0, 9);
	tm2->GateToRight = Vec2(24, 9);
	tm2->GateToTop = Vec2(12, 0);
	tm2->GateToBottom = Vec2(12, 18);
	tm2->PlayerStartPos[0] = Vec2(6, 5);
	tm2->PlayerStartPos[1] = Vec2(18, 14);
	tm2->GhostStartPos[0] = Vec2(11, 8);
	tm2->GhostStartPos[1] = Vec2(13, 8);
	tm2->GhostStartPos[2] = Vec2(11, 10);
	tm2->GhostStartPos[3] = Vec2(13, 10);
	tm2->GhostHomeZone[0] = Vec2(3, 3);
	tm2->GhostHomeZone[1] = Vec2(3, 16);
	tm2->GhostHomeZone[2] = Vec2(21, 3);
	tm2->GhostHomeZone[3] = Vec2(21, 16);
	tm2->PlayerTextScorePos[0] = Vec2(18,7);
	tm2->PlayerTextScorePos[1] = Vec2(0, 11);
	tm2->PlayerTextExtraPos[0] = Vec2(0, 11);
	tm2->PlayerTextExtraPos[1] = Vec2(0, 11);
	tm2->MessagePos = Vec2(0, 11);
	levels[levelNum++] = tm2;
}

/*

	string x22[] = {
		"L--------H |.| L--------H",
		"|........| |.| |........|",
		"|.L-H.LH.| |.| |.L----H.|",
		"|.| |.||.| |.| |.K----J.|",
		"|.| |.||.K-J.K-J.... ...|",
		"|.| |.Kw.........L--H.L-J",
		"|.| |..|.L-----H.K--w.K--",
		"|.K-J..|.|L---H|....|....",
		"|......|.KJ   KJ.LH.|.L--",
		"K-H.LH.|.       .||.|.K-H",
		"--J.||.|.LH   LH.xJ.|...|",
		"....KJ.|.|K---J|.|..|.L-w",
		"--H....|.K-----J.|.Lw.K-w",
		"L-J..L-w.........|.KJ...|",
		"|....| |.L-H.L-H.|....L-w",
		"|.LH.| |.| |.| |.x--H.| |",
		"|.KJ.K-J.| |.| |.K--J.K-w",
		"|........| |.| |........|",
		"K--------J |.| K--------J"
	};

*/