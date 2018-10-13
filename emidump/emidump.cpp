// emidump.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <assert.h>
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


class EMI_File
{
public:

	void save_s3m(const std::string& filename)
	{
//			Scream Tracker 3.01 BETA File Formats And Mixing Info
//			==================================================== =
//
//			This document finally containts the OFFICIAL information on s3m format and
//			much more.There might be some errors though I've even checked this a few
//			times, so if something seems weird, don't just blindly believe it but think
//			first if it could be just a typo or something.
//
//
//			---------------------------------------------------------------------------- -
//			What is the S3M file format ?
//			What is the samplefile format ?
//			What is the adlib instrument format ?
//
//
//			The first table describes the S3M header.All other blocks are
//			pointer to by pointers, so in theory they could be anywhere in
//			the file.However, the practical standard order is :
//		-header
//			- instruments in order
//			- patterns in order
//			- samples in order
//
//			Next the instrument header is described.It is stored to S3M
//			for each instrument and also saved to the start of all samples
//				saved from ST3.Same header is also used by Advance Digiplayer.
//
//				The third part is the description of the packed pattern format.
//
//
//				S3M Module header
//				0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
//				+ -- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0000: | Song name, max 28 chars(end with NUL(0)) |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0010 : |                                               |1Ah | Typ | x | x |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0020 : | OrdNum | InsNum | PatNum | Flags | Cwt / v | Ffv | 'S' | 'C' | 'R' | 'M' |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0030 : | g.v | i.s | i.t | m.v | x | x | x | x | x | x | x | x | x | x | Special |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0040 : | Channel settings for 32 channels, 255 = unused, +128 = disabled |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0050 : |                                                               |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0060 : | Orders; length = OrdNum(should be even) |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				xxx1: | Parapointers to instruments; length = InsNum * 2 |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				xxx2: | Parapointers to patterns; length = PatNum * 2 |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				xxx1 = 70h + orders
//				xxx2 = 70h + orders + instruments * 2
//
//				Parapointers to file offset Y is(Y - Offset of file header) / 16.
//				You could think of parapointers as segments relative to the
//				start of the S3M file.
//
//				Type = File type : 16 = ST3 module
//				Ordnum = Number of orders in file(should be even!)
//				Insnum = Number of instruments in file
//				Patnum = Number of patterns in file
//				Cwt / v = Created with tracker / version : &0xfff = version, >> 12 = tracker
//				ST3.00 : 0x1300
//				ST3.01 : 0x1301
//				Ffv = File format version;
//		1 = old version used long ago(samples signed)
//			2 = standard(samples unsigned)
//			Flags = [These are old flags for Ffv1.Not supported in ST3.01
//			| +1:st2vibrato
//			| +2 : st2tempo
//			| +4 : amigaslides
//			| +32 : enable filter / sfx with sb
//			]
//			+ 8: 0vol optimizations
//			Automatically turn off looping notes whose volume
//			is zero for >2 note rows.
//			+ 16: amiga limits
//			Disallow any notes that go beond the amiga hardware
//			limits(like amiga does).This means that sliding
//			up stops at B#5 etc.Also affects some minor amiga
//			compatibility issues.
//			+ 128: special custom data in file
//			Special = pointer to special custom data(not used by ST3.01)
//			g.v = global volume(see next section)
//			m.v = master volume(see next section) 7 lower bits
//			bit 8 : stereo(1) / mono(0)
//			i.s = initial speed(command A)
//			i.t = initial tempo(command T)
//
//			Channel settings :
//		bit 8 : channel enabled
//			bit 0 - 7 : channel type
//			0..7 : Left Sample Channel 1 - 8
//			8..15 : Right Sample Channel 1 - 8
//			16..31 : Adlib channels(9 melody + 5 drums)
//
//			Global volume directly divides the volume numbers used.So
//			if the module has a note with volume 48 and master volume
//				is 32, the note will be played with volume 24. This affects
//				both Gravis & SoundBlasters.
//
//				Master volume only affects the SoundBlaster.It controls
//				the amount of sample multiplication(see mixing section
//					of this doc).The bigger the value the bigger the output
//				volume(and thus quality) will be.However if the value
//				is too big, the mixer may have to clip the output to
//				fit the 8 bit output stream.The default value works
//				pretty well.Note that in stereo, the mastermul is
//				internally multiplied by 11 / 8 inside the player since
//				there is generally more room in the output stream.
//
//				Order list lists the order in which to play the patterns. 255 = --
//				is the end of tune mark and 254 = ++is just a marker that is
//				skipped.
//
//
//
//				Digiplayer / ST3 samplefileformat
//				0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
//				+ -- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0000: | [T] | Dos filename(12345678.ABC) | MemSeg |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0010 : | Length | HI : leng | LoopBeg | HI : LBeg | LoopEnd | HI : Lend | Vol | x | [P] | [F] |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0020 : | C2Spd | HI : C2sp | x | x | x | x | Int : Gp | Int : 512 | Int : lastused |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0030 : | Sample name, 28 characters max... (incl.NUL) |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0040 : | ...sample name... | 'S' | 'C' | 'R' | 'S' |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				xxxx : sampledata
//
//				Length / LoopBegin / LoopEnd are all 32 bit parameters although
//				ST3 only support file sizes up to 64, 000 bytes.Files bigger
//				than that are clipped to 64, 000 bytes when loaded to ST3.NOTE
//				that LoopEnd points to one byte AFTER the end of the sample,
//				so LoopEnd = 100 means that byte 99.9999 (fixed)is the last one
//				played.
//				C2Spd = Herz for middle C.ST3 only uses lower 16 bits.
//				Vol = Default volume 0..64
//				Memseg = Pointer to sampledata
//				Inside a sample or S3M, MemSeg tells the parapointer to
//				the actual sampledata.In files all 24 bits are used.
//				In memory the value points to the actual sample segment
//				or Fxxx if sample is in EMS under handle xxx.In memory
//				the first memseg byte is overwritten with 0 to create
//				the dos filename terminator nul.
//				Int:Gp = Internal : Address of sample in gravis memory / 32
//				(only used while module in memory)
//				Int : 512 = Internal : flags for soundblaster loop expansion
//					  (only used while module in memory)
//				Int : las = Internal : last used position(only works with sb)
//				(only used while module in memory)
//				[T]ype   1 = Sample, 2 = adlib melody, 3 += adlib drum(see below for
//					adlib structure)
//				[F]lags, +1 = loop on
//				+ 2 = stereo(after Length bytes for LEFT channel,
//					another Length bytes for RIGHT channel)
//				+ 4 = 16 - bit sample(intel LO - HI byteorder)
//				(+2 / +4 not supported by ST3.01)
//				[P]ack   0 = unpacked, 1 = DP30ADPCM packing(not used by ST3.01)
//
//
//
//				adlib instrument format
//				0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
//				+ -- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0000: | [T] | Dos filename(12345678.123) | 00h | 00h | 00h |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0010 : | D00 | D01 | D02 | D03 | D04 | D05 | D06 | D07 | D08 | D09 | D0A | D0B | Vol | Dsk | x | x |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0020 : | C2Spd | HI : C2sp | x | x | x | x | x | x | x | x | x | x | x | x |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0030 : | Sample name, 28 characters max... (incl.NUL) |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0040 : | ...sample name... | 'S' | 'C' | 'R' | 'I' |
//				+-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//
//				[T]ype = 2 : amel 3 : abd 4 : asnare 5 : atom 6 : acym 7 : ahihat
//				C2Spd = 'Herz' for middle C.ST3 only uses lower 16 bits.
//				Actually this is a modifier since there is no
//				clear frequency for adlib instruments.It scales
//				the note freq sent to adlib.
//				D00..D0B contains the adlib instrument specs packed like this :
//				modulator : carrier :
//				D00 = [freq.muliplier] + [? scale env.] * 16 + [? sustain] * 32 + = D01
//				[? pitch vib] * 64 + [? vol.vib] * 128
//				D02 = [63 - volume] + [levelscale & 1] * 128 + [l.s. & 2] * 64 = D03
//				D04 = [attack] * 16 + [decay] = D05
//				D06 = [15 - sustain] * 16 + [release] = D07
//				D08 = [wave select] = D09
//				D0A = [modulation feedback] * 2 + [? additive synthesis]
//				D0B = unused
//
//
//
//				packed pattern format
//				0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
//				+ -- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//				0000: | Length | packed data, see below...
//				+ -- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//
//				Length = length of packed pattern
//
//				Unpacked pattern is always 32 channels by 64 rows.Below
//				is the unpacked format st uses for reference:
//		Unpacked Internal memoryformat for patterns(not used in files) :
//			NOTE : each channel takes 320 bytes, rows for each channel are
//			sequential, so one unpacked pattern takes 10K.
//			byte 0 - Note; hi = oct, lo = note, 255 = empty note,
//			254 = key off(used with adlib, with samples stops smp)
//			byte 1 - Instrument; 0 = ..
//			byte 2 - Volume; 255 = ..
//			byte 3 - Special command; 255 = ..
//			byte 4 - Command info;
//
//		Packed data consits of following entries :
//	BYTE:what  0 = end of row
//		& 31 = channel
//		& 32 = follows;  BYTE:note, BYTE : instrument
//		& 64 = follows;  BYTE:volume
//		& 128 = follows; BYTE:command, BYTE : info
//
//		So to unpack, first read one byte.If it's zero, this row is
//		done(64 rows in entire pattern).If nonzero, the channel
//		this entry belongs to is in BYTE AND 31. Then if bit 32
//		is set, read NOTE and INSTRUMENT(2 bytes).Then if bit
//		64 is set read VOLUME(1 byte).Then if bit 128 is set
//		read COMMAND and INFO(2 bytes).
//
//		For information on commands / how st3 plays them, see the
//		manual.
//
//
//		---------------------------------------------------------------------------- -
//		What is the Stmik300old format ?
//		What is the STIMPORT file format ?
//		What is the SIMPLEXFILE format ?
//
//		The old stmik 300 (never published because it has no
//			usable interface) want's that the samples in the module
//		are pre - extended for soundblaster.This means that
//		samples have an extra 512 bytes of loop after their
//		loopend(or silence if no loop).The save option for
//		old stmik does this extension.Otherwise the fileformat
//		is the same as for a normal S3M.
//
//		STIMPORT file format is supposed to be an easy way of
//		imputting weird data to st3.That is, it should be
//		easy to convert something to STIMPORT format.The format
//		goes like this:
//
//		 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
//			 + -- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//			 0000 : | 'S' | 'T' | 'I' | 'M' | 'P' | 'O' | 'R' | 'T' | i.s | x | x | x | x | x | x | x |
//			 +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//			 0010 : | Notedata...
//			 + -- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//			 xxxx : | Instruments...
//			 + -- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +-- - +
//
//			 There are no patterns or orders, just a continuos note stream
//			 of following structures :
//		 0    1    2    3    4
//			 + ---- + ---- + ---- + ---- + ---- +
//			 | Chan | Note | Inst | Cmnd | Info |
//			 +---- + ---- + ---- + ---- + ---- +
//			 Chan is the channel number for the note.Value 0 stands
//			 for next row.Value 255 stands for end of note stream.
//				 Note / Inst / Cmnd / Info are like in the unpacked memory pattern
//				 (see previous section)
//
//				 After the notedata there can be instruments(optional)
//				 Every instrument consits of the following block :
//		 0    1    2    3    4
//			 + ---- + ---- + ---- + ---- + ---- + ---- + ---- + ---- + ---------------- -
//			 | Inst | Loop | LoopBeg | LoopEnd | Length | Sampledata...
//			 + ---- + ---- + ---- + ---- + ---- + ---- + ---- + ---- + ---------------- -
//			 Inst = number of instrument to load(0 = end of instruments)
//			 Loop = 1 = loop enabled
//			 LoopBeg / LoopEnd / Length = guess
//			 Sampledata = raw sampledata for Length bytes.
//
//
//			 SIMPLEXFORMAT has been used for some adlib musics.Dunno
//			 if it's useful to you, but the st3 saves it like this:
//
//				 First is stores the 32 first instruments(80 bytes each,
//					 just like in a S3M).Then it stores the raw notedata
//				 for the entire song.This means it goes through the
//					 order list and saves all the patererns in that order
//					 unpacked.The resulting file will be BIG.The idea
//					 is that it should be fairly easy to convert to something
//					 from this.
//
//
//					 ---------------------------------------------------------------------------- -
//					 What is C2SPD ?
//					 How to calculate the note frequencies like ST3 ?
//					 How does ST3 mix depending on master volume ?
//
//
//					 Finetuning(C2SPD) is actually the frequency in herz for
//					 the note C4.Why is it C2SPD ? Well, originally in ST2
//					 the middle note was C2 and the name stuck.Later in ST3
//					 the middle note was raised to C4 for more octaves... So
//					 actually C2SPD should be called C4SPD...
//
//
//					 Table for note frequencies used by ST3 :
//
//	 note:  C    C#   D    D#   E    F    F#   G    G#   A    A#   B
//		 period : 1712, 1616, 1524, 1440, 1356, 1280, 1208, 1140, 1076, 1016, 0960, 0907
//
//		 middle octave is 4.
//
//		 8363 * 16 * (period(NOTE) >> octave(NOTE))
//		 note_st3period = --------------------------------------------
//		 middle_c_finetunevalue(INSTRUMENT)
//
//		 note_amigaperiod = note_st3period / 4
//
//		 note_herz = 14317056 / note_st3period
//
//		 Note that ST3 uses period values that are 4 times larger than the
//		 amiga to allow for extra fine slides(which are 4 times finer
//			 than normal fine slides).
//
//
//		 How ST3 mixes :
//
//			1) volumetable is created in the following way :
//
//	> volumetable[volume][sampledata] = volume*(sampledata - 128) / 64;
//
//NOTE: sampledata in memory is unsigned in ST3, so the - 128 in the
//	formula converts it so that the volumetable output is signed.
//
//	2) postprocessing table is created with this pseudocode:
//
//		> z = mastervol & 127;
//		> if (z<0x10) z = 0x10;
//		> c = 2048 * 16 / z;
//		> a = (2048 - c) / 2;
//		> b = a + c;
//		>                     { 0, if x < a
//		> posttable[x + 1024] = { (x - a) * 256 / (b - a)  , if a <= x < b
//		>                     { 255              , if x > b
//
//			3) mixing the samples
//
//			output = 1024
//			for i = 0 to number of channels
//				output += volumetable[volume*globalvolume / 64][sampledata];
//		next
//			realoutput = posttable[output]
//
//
//			This is how the mixing is done in theory.In practice it's a bit
//			different for speed reasons, but the result is the same.
//
//
//			---------------------------------------------------------------------------- -
//			That's it. If there are any more questions, that's too bad : -) If you have
//			problems with the S3M format, try to contact somone who already supports
//			it(there is quite a lot of support for the S3M already, so that shouldn't
//				be too hard...) Good luck for reading / writing Scream Tracker files.

	}

private:
};

class Track
{
public:
	Track(unsigned char* data, uint16_t length)
		: data_(data)
		, length_(length)
	{
		build_track_strings();
	}

	std::string get_track_byte_str(uint16_t pos) const
	{
		if (pos >= length_)
			return "-----|-";
		char buf[32];
		sprintf(buf, "0x%02X | ", data_[pos]);
		return buf;
	}

	std::string get_track_string(size_t pos) const
	{
		if (pos >= track_strings_.size())
			return "-xxxx|-";
		return track_strings_[pos];
	}

	size_t track_string_size() const
	{
		return track_strings_.size();
	}

private:
	void build_track_strings()
	{
//#define DEBUG_CONTROL_COMMANDS
//#define DEBUG_SPEED
		uint8_t rows = 0;
		for (uint16_t pos = 0; pos < length_; ++pos)
		{
			unsigned char value = data_[pos];
			
#ifdef DEBUG_CONTROL_COMMANDS
			switch (value)
			{
			// 2 byte control command
			case 0xE4:
			case 0xE7:
			case 0xE8:
			case 0xFB:
			case 0xFD:
			case 0xF7:
			case 0xFA:
			case 0xFC:
				++pos;
				continue;

			// 1 byte control command
			case 0xCD:
			case 0xCF:
			case 0xDC:
			case 0xDD:
			case 0xCE:
			case 0xF8: // Loop command?
				continue;

			// Speed/tempo change
			case 0x81: rows = 2; continue;
			case 0x82: rows = 4; continue;
			case 0x83: rows = 8; continue;
			case 0x84: rows = 16; continue;
			case 0x85: rows = 32; continue;
			case 0x86: rows = 64; continue;
			case 0x89: rows = 12; continue;
			case 0x90: rows = 6; continue;
			case 0x8B: rows = 48; continue;
			case 0x8E: rows = 3; continue;
				
			}
#endif


			// notes between 0x00 and 0x7F (?)
			if (value >= 0 && value <= 0x7F)
			{
				if (value == 0)
					track_strings_.push_back("-DUM-|-");
				else
				{
					char buf[32];
					if (value / 12 < 10)
						sprintf_s(buf, "%s%d  | ", notes[value % 12], value / 12);
					else
						sprintf_s(buf, "%s%d | ", notes[value % 12], value / 12);

					track_strings_.push_back(buf);
				}
#ifdef DEBUG_SPEED
				for (uint8_t r = 1; r < rows; ++r)
				{
					track_strings_.push_back("-----|-");
				}

#endif
			}
			else
			{
				char buf[32];
				sprintf(buf, "0x%02X | ", data_[pos]);
				track_strings_.push_back(buf);
			}
		}
	}
private:
	unsigned char* data_;
	uint16_t length_;
	std::vector<std::string> track_strings_;
};

int main()
{
	//for (int emi = 2; emi < 42; ++emi) // all
	int emi = 3;
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
		unsigned char* track = bytes;
		// header
		char header[3];
		header[0] = bytes[0];
		header[1] = bytes[1];
		header[2] = bytes[2];
		bytes += 3;

		uint8_t num_tracks = bytes[0]; bytes += sizeof(uint8_t);
		assert(num_tracks == 6);
		uint16_t unknown1 = *(uint16_t*) bytes; bytes += sizeof(uint16_t); 
		assert(unknown1 == 0 || unknown1 == 768);
		uint16_t header_size = *(uint16_t*)bytes; bytes += sizeof(uint16_t);
		assert(header_size == 0x1C);
		uint16_t total_header_size1 = *(uint16_t*)bytes; bytes += sizeof(uint16_t);
		uint16_t total_header_size2 = *(uint16_t*)bytes; bytes += sizeof(uint16_t);
		assert(total_header_size1 == total_header_size2);
		uint16_t file_size = *(uint16_t*)bytes; bytes += sizeof(uint16_t);
		assert(file_size == size);
		uint16_t unknown6 = *(uint16_t*)bytes; bytes += sizeof(uint16_t);

		std::vector<uint16_t> trackoffsets;
		for (uint8_t track = 0; track < num_tracks; ++track)
		{
			trackoffsets.push_back(*(uint16_t*)bytes);
			bytes += sizeof(uint16_t);
		}
		trackoffsets.push_back(size);

		//uint16_t title_length = total_header_size1 - header_size - 2;
		//char* title = new char[title_length];
		//memcpy(title, track + header_size + 1, title_length);

		std::vector<Track> tracks;
		for (int i = 0; i < trackoffsets.size() - 1; ++i)
		{
			uint16_t begin = trackoffsets[i];
			uint16_t end = trackoffsets[i + 1];
			if (begin == 0 || end == 0)
				continue;
			uint16_t tracklength = end - begin;
			unsigned char* trackbytes = new unsigned char[tracklength];
			
			memcpy(trackbytes, track + begin, tracklength);
			tracks.push_back(Track(trackbytes, tracklength));
		}

		size_t max_track_length = 0;
		for (const Track& t : tracks)
		{
			if (t.track_string_size() > max_track_length)
				max_track_length = t.track_string_size();
		}

		std::vector<std::string> track_strings;
		for (size_t i = 0; i < max_track_length; ++i)
		{
			char rownr[16];
			sprintf(rownr, "%04i|", i);
			std::string row = rownr;
			for (const Track& t : tracks)
			{
				row += t.get_track_string(i);
			}
			track_strings.push_back(row);
		}

		//for (int i = 0; i < trackoffsets.size() - 1; ++i)
		//{
		//	int begin = trackoffsets[i];
		//	int end = trackoffsets[i + 1];

		//	int rows = 0;
		//	int currentrow = 1;
		//	int current_row_string_nr = 0;
		//	for (int trkpos = begin; trkpos < end; ++trkpos)
		//	{

		//		unsigned char c = bytes[trkpos];
		//		if (c < 128)
		//		{
		//			char buffer[128];
		//			sprintf_s(buffer, "%s%d | ", notes[c % 12], c / 12);
		//			get_track_string(i, track_strings, current_row_string_nr++) += buffer;

		//			for (int row = 1; row < rows; ++row)
		//			{
		//				if (row % 4 == 0)
		//				{
		//					get_track_string(i, track_strings, current_row_string_nr++) += "-*- | ";
		//				}
		//				else
		//				{
		//					get_track_string(i, track_strings, current_row_string_nr++) += "--- | ";
		//				}
		//			}
		//		}
		//		else
		//		{
		//			/*
		//			switch (c)
		//			{
		//			case 0xF8: break; // Change track
		//			case 0x82: rows = 2; break;
		//			case 0x83: rows = 4; break;
		//			case 0x84: rows = 8; break;
		//			case 0x85: rows = 16; break;
		//			case 0x8B: rows = 24; break;

		//			case 0x8E: rows = 1; break; // hmmmmyeeesshh

		//			case 0x90: rows = 6; break; // hmmmmyeeesshh
		//			case 0x97: rows = 32; break; // hmmmmyeeesshh
		//			default:
		//			{
		//				//char buf[32];
		//				//sprintf(buf, "x%02X | ", c);
		//				//get_track_string(i, track_strings, current_row_string_nr++) += buf;
		//			}
		//			break;
		//			}*/
		//			switch (c)
		//			{
		//			case 0xF8: break; // Change track
		//			case 0x81: rows = 2; break;
		//			case 0x82: rows = 4; break;
		//			case 0x83: rows = 8; break;
		//			case 0x84: rows = 16; break;
		//			case 0x85: rows = 32; break;
		//			case 0x8B: rows = 48; break;

		//			case 0x8E: rows = 1; break; // hmmmmyeeesshh

		//			case 0x90: rows = 6; break; // hmmmmyeeesshh
		//			case 0x97: rows = 32; break; // hmmmmyeeesshh
		//			default:
		//			{
		//				//char buf[32];
		//				//sprintf(buf, "x%02X | ", c);
		//				//get_track_string(i, track_strings, current_row_string_nr++) += buf;
		//			}
		//			break;
		//			}
		//		}
		//	}
		//}
		char out[64];
		sprintf_s(out, "%d - %s.txt", emi, get_em_name(emi));
		fopen_s(&fp, out, "w+");
		for (auto& string : track_strings)
		{
			fprintf(fp, "%s\n", string.c_str());
		}
		fclose(fp);
		delete[] track;
	}
    return 0;
}

