// emidump.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
const char* notes[12] = { "C-", "C#", "D-", "D#", "E-", "F-", "F#", "G-", "G#", "A-", "A#", "B-" };

const char* get_em_name(int nr)
{
	switch (nr)
	{
		case  2: return "BAR";
		case  3: return "BATTLE1";
		case  4: return "BATTLE2";
		case  5: return "BATTLE3";
		case  6: return "SHOP1";
		case  7: return "ENDING";
		case  8: return "HOUSE1";
		case  9: return "CEMETARY";
		case 10: return "GIRL";
		case 11: return "HOUSE2";
		case 12: return "TRAIN_ENGINEER";
		case 13: return "CASTLE";
		case 14: return "NUDE_BEACH";
		case 15: return "MENU";
		case 16: return "INTRO";
		case 17: return "SHOP2";
		case 18: return "SHOP3";
		case 19: return "GARBAGE1";
		case 20: return "FIST_BASE";
		case 21: return "CLUB10";
		case 22: return "UNKNOWN";
		case 23: return "POLICEHQ";
		case 24: return "RESEARCH_CENTER";
		case 25: return "HOUSE3";
		case 26: return "CREDITS";
		case 27: return "CENTRAL_COBRA";
		case 28: return "WEST_COBRA";
		case 29: return "EAST_COBRA";
		case 30: return "SOUTH_COBRA";
		case 31: return "BOSS_TALK";
		case 32: return "UNKNOWN2";
		case 33: return "VICTORY";
		case 34: return "FIGHT_WIN";
		case 35: return "GAMEOVER";
		case 36: return "MARRY";
		case 37: return "GARBAGE2";
		case 38: return "GARBAGE3";
		case 39: return "GARBAGE4";
		case 40: return "GARBAGE5";
		case 41: return "FALLING";
	}
	return "";
}

std::string& get_track_string(int channel, std::vector<std::string>& track_strings, int row)
{
	if (row < track_strings.size() + 1)
	{
		char buf[8];
		sprintf(buf, "%03d|", track_strings.size() % 64);
		std::string row_string = buf;
		for (int i = 0; i < channel; ++i)
			row_string += "    | ";
		track_strings.push_back(row_string);
	}
	return track_strings[row];
}

int main()
{
	for (int emi = 2; emi < 42; ++emi) // all
	//for (int emi = 9; emi < 10; ++emi)
	{
		FILE* fp;
		char in[64];
		sprintf(in, "%d.EM", emi);
		fopen_s(&fp, in, "rb");
		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		rewind(fp);
		unsigned char* bytes = new unsigned char[size];
		if (fp)
		{
			fread(bytes, 1, size, fp);
			fclose(fp);
		}

		std::vector<int> trackoffsets;
		for (int i = 0; i < size; ++i)
		{
			if (bytes[i] == 0xF8)
			{
				trackoffsets.push_back(i);
			}
		}
		trackoffsets.push_back(size);

		std::vector<std::string> track_strings;

		for (int i = 0; i < trackoffsets.size() - 1; ++i)
		{
			int begin = trackoffsets[i];
			int end = trackoffsets[i + 1];

			int rows = 0;
			int currentrow = 1;
			int current_row_string_nr = 0;
			for (int trkpos = begin; trkpos < end; ++trkpos)
			{

				unsigned char c = bytes[trkpos];
				if (c < 128)
				{
					char buffer[128];
					sprintf_s(buffer, "%s%d | ", notes[c % 12], c / 12);
					get_track_string(i, track_strings, current_row_string_nr++) += buffer;

					for (int row = 1; row < rows; ++row)
					{
						if (row % 4 == 0)
						{
							get_track_string(i, track_strings, current_row_string_nr++) += "-*- | ";
						}
						else
						{
							get_track_string(i, track_strings, current_row_string_nr++) += "--- | ";
						}
					}
				}
				else
				{
					switch (c)
					{
					case 0xF8: break; // Change track
					case 0x82: rows = 2; break;
					case 0x83: rows = 4; break;
					case 0x84: rows = 8; break;
					case 0x85: rows = 16; break;
					case 0x8B: rows = 24; break;

					case 0x8E: rows = 1; break; // hmmmmyeeesshh

					case 0x90: rows = 6; break; // hmmmmyeeesshh
					case 0x97: rows = 32; break; // hmmmmyeeesshh
					default:
					{
						//char buf[32];
						//sprintf(buf, "x%02X | ", c);
						//get_track_string(i, track_strings, current_row_string_nr++) += buf;
					}
					break;
					}
				}
			}
		}
		char out[64];
		sprintf_s(out, "%d - %s.txt", emi, get_em_name(emi));
		fopen_s(&fp, out, "w+");
		for (auto& string : track_strings)
		{
			fprintf(fp, "%s\n", string.c_str());
		}
		fclose(fp);
		delete[] bytes;
	}
    return 0;
}

