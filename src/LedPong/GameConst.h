#pragma once

// see: https://stackoverflow.com/questions/12042549/define-constant-variables-in-c-header

// GAME

const int GAME_FrameRate = 60;

const int GAME_Max_Player = 2;
const int GAME_Max_Ghost = 4;

// 96:128 = 3:4
const int WALL_Xdim = 2 * 64;
const int WALL_Ydim = 3 * 32;

const int WALL_Xgap = 1;
const int WALL_Ygap = 1;

const int WALL_XpixSize = 4;
const int WALL_YpixSize = 4;

const int LEVEL_Max = 10;

const int GAME_Promille_to_spawn_fruit = 10;
const int GAME_Spawned_item_base_frames = 30;
const int GAME_Spawned_item_frame_range = 120;

const int GAME_Switch_Text_Frames = 45;

const int GAME_Message_Max_Char = 5;
const int GAME_Message_Life_time_frames = 45;

const double GAME_PacMan_Phase_per_Frame = 0.14; // Google Pac Man -> 0.16

const int GAME_Frames_of_death = 1.6 * GAME_FrameRate;

const int GAME_Max_Highscore = 100;