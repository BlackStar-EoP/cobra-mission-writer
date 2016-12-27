#include "stdafx.h"
#include "png.h"
#include "stdio.h"
#include "stdlib.h"
#include <stdint.h>
#include <vector>
#include "chars.h"
#pragma comment(lib, "libpng15.lib")
#pragma comment(lib, "zlib.lib")

typedef unsigned int uint;
typedef unsigned char byte;

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

int get_file_size(const std::string& file);
class Color
{
public:
	Color()
		: m_r(0)
		, m_g(0)
		, m_b(0)
		, m_a(0)
	{
	}
	Color(byte r, byte g, byte b)
		: m_r(r)
		, m_g(g)
		, m_b(b)
		, m_a(0xFF)
	{

	}

	byte m_r;
	byte m_g;
	byte m_b;
	byte m_a;
};

Color get_color(byte c)
{
	switch (c)
	{
		case 0x00: 	return Color(0, 0, 0);
		case 0x66: return Color(195, 195, 0);  // - 195 195 0 
		case 0xEE: return Color(243, 243, 0);  // - 243 243 0 
		case 0xFF: return Color(243, 243, 243); // - 243 243 243
		case 0x77: return Color(113, 113, 113); // - 113 113 113
		case 0x33: return Color(243, 211, 162); // - 243 211 162
		case 0x22: return Color(195, 0, 0); // - 195 0 0
		case 0xAA: return Color(243, 0, 0); // - 243 0 0
		case 0x88: return Color(195, 195, 195); // - 195 195 195
		case 0x44: return Color(0, 178, 0); 
		case 0xCC: return Color(0, 243, 0);
		case 0x11: return Color(0, 0, 195); //- 0, 0, 195
		case 0x99: return Color(0, 0, 243); //- 0, 0, 243
		case 0x55: return Color(0, 195, 195);
		case 0xDD: return Color(0, 243, 243);
		case 0xBB: return Color(0, 243, 243);
		//case 0x    return Color(0, 243, 243);
		//case 0x40: return Color(255, 0, 255);
		//case 0xA0: return Color(0, 243, 243);
		//case 0x04: return Color(255, 128, 64);

	}
	
	return Color(0xFF, 0x00, 0xFF);
}

void dump_mcg_vol()
{
	int MULTIPLIER = 40;
	int width = 32 * MULTIPLIER;
	int height = 55840 / MULTIPLIER;
	uint bytes_per_pixel = 4;
	byte* buffer = new byte[32 * 55842 * 4];

	FILE* cm = nullptr;
	fopen_s(&cm, "mcg.vol", "rb");
	fread(buffer, 1, 32 * 55842, cm);
	fclose(cm);



	int code = 0;
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep row;

	// Open file for writing (binary mode)
	static int i = 0;
	char filename[512];
	char title[] = "Edge of Panic";
	while (i < 100000)
	{
		sprintf_s(filename, "screenshot-%d.png", ++i);
		fopen_s(&fp, filename, "r");
		if (fp != nullptr)
		{
			fclose(fp);
			continue;
		}
		else
			fopen_s(&fp, filename, "wb");
		break;
	}


	if (fp == NULL)
	{
		fprintf(stderr, "Could not open file %s for writing\n", filename);
		code = 1;
		goto finalise;
	}

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		fprintf(stderr, "Could not allocate write struct\n");
		code = 1;
		goto finalise;
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fprintf(stderr, "Could not allocate info struct\n");
		code = 1;
		goto finalise;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error during png creation\n");
		code = 1;
		goto finalise;
	}

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height,
		8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// Set title
	if (title != NULL) {
		png_text title_text;
		title_text.compression = PNG_TEXT_COMPRESSION_NONE;
		title_text.key = "Title";
		title_text.text = title;
		png_set_text(png_ptr, info_ptr, &title_text, 1);
	}

	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (4 bytes per pixel - RGBA)
	row = (png_bytep)malloc(4 * width * sizeof(png_byte));

	// Write image data
	uint pos = 0;
	uint rowpos = 0;
	//for (int y = height-1; y >= 0; --y)

	for (int y = 0; y < height; ++y)
	{
		int offset = 64 + (y * 32) + ((y / 32) * MULTIPLIER * 0x400);

		for (int m = 0; m < MULTIPLIER; ++m)
		{
			for (int x = 0; x < 32; ++x)
			{
				byte c = 0xBB;

				if (offset < 55842 * 32 * 4)
					c = buffer[offset + x];
				else
					printf("");

				Color color = get_color(c);
				row[rowpos++] = color.m_r;
				row[rowpos++] = color.m_g;
				row[rowpos++] = color.m_b;
				row[rowpos++] = color.m_a;
			}
			offset += 0x400;
		}
		png_write_row(png_ptr, row);
		rowpos = 0;
	}

	// End write
	png_write_end(png_ptr, NULL);

finalise:
	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL) free(row);

	delete[] buffer;
}

class Layout
{
public:
	enum LAYOUT_NUMBER
	{
		CHARSPRITES = 1,
		WEAPONSPRITES,
		CASTLE,
		AREA1,
		AREA2,
		AREA3,
		AREA4,
		AREA5,
		HOUSE,
		BAR,
		CORP,
		BRICK,
		LAB,
		SEASIDE
	};

	Layout() 
		: width(0)
		, height(0)
		, num_floors(0)
		, m_data(nullptr)
	{}

	Layout(byte* data, int datalength)
		: width(0)
		, height(0)
		, num_floors(0)
		, m_datalength(datalength)
	{
		m_data = new byte[datalength];
		memcpy(m_data, data, datalength);
	}

	~Layout()
	{
	//	delete[] m_data;
	}

	Color parse_color(uint16_t col)
	{
		byte r = (col >> 4) & 0x0F;
		byte g = col >> 8;
		byte b = col & 0x0F;
		return Color((r << 4) + (r >> 2), (g << 4) + (g >> 2), (b << 4) + (b >> 2));
	}

	void parse_data()
	{
		int pal = 0;
		for (int i = 32; i < 64; i += 2)
		{
			uint16_t color = (m_data[i + 1] << 8) | m_data[i];
			m_palette[pal++] = parse_color(color);
		}

		// 16
		// 18
		// 20
		width = m_data[16];
		height = m_data[18];
		num_floors = m_data[20];
		name = (char*)m_data + 22;
		m_layout = get_layout();
		description = (char*)m_data + 64;
		m_level_data_start = (m_data[11] << 8) | m_data[10];
		for (int i = 0; i < num_floors; ++i)
		{
			m_floors.push_back(m_data + m_level_data_start + (i * width * height));
		}
		int m_mask_data_start = m_level_data_start + (num_floors * width * height);
		for (int i = 0; i < num_floors; ++i)
		{
			m_masks.push_back(m_data + m_mask_data_start + (i * width * height));
		}

	}

	LAYOUT_NUMBER get_layout()
	{
		if (strcmp(name, "CASTLE") == 0) return CASTLE;
		if (strcmp(name, "AREA1") == 0) return AREA1;
		if (strcmp(name, "AREA2") == 0) return AREA2;
		if (strcmp(name, "AREA3") == 0) return AREA3;
		if (strcmp(name, "AREA4") == 0) return AREA4;
		if (strcmp(name, "AREA5") == 0) return AREA5;
		if (strcmp(name, "HOUSE") == 0) return HOUSE;
		if (strcmp(name, "BAR") == 0) return BAR;
		if (strcmp(name, "CORP") == 0) return CORP;
		if (strcmp(name, "BRICK") == 0) return BRICK;
		if (strcmp(name, "LAB") == 0) return LAB;
		if (strcmp(name, "SEASIDE") == 0) return SEASIDE;
	}


	void save_level_png()
	{
		static int filenr = 0;
		if (num_floors == 0)
			printf("");
		for (int i = 0; i < num_floors; ++i)
		{
			char filename[256];
			sprintf_s(filename, "./levels/%d_%s_%s_%d.png", filenr, name, description, i);

			for (uint pos = 0; pos < strlen(filename); ++pos)
			{
				if (filename[pos] == '|')
					filename[pos] = '_';
			}

			save_png(m_floors[i], m_masks[i], width, height, filename, description);

		}
		++filenr;
	}

	void save_png(byte* data, byte* maskdata, int width, int height, char* filename, char* title)
	{
		uint bytes_per_pixel = 4;

		int code = 0;
		FILE *fp;
		png_structp png_ptr = nullptr;
		png_infop info_ptr = nullptr;
		png_bytep row = nullptr;

		// Open file for writing (binary mode)
		fopen_s(&fp, filename, "wb+");
		if (fp == NULL)
		{
			fprintf(stderr, "Could not open file %s for writing\n", filename);
			code = 1;
			goto finalise;
		}

		// Initialize write structure
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL)
		{
			fprintf(stderr, "Could not allocate write struct\n");
			code = 1;
			goto finalise;
		}

		// Initialize info structure
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL)
		{
			fprintf(stderr, "Could not allocate info struct\n");
			code = 1;
			goto finalise;
		}

		// Setup Exception handling
		if (setjmp(png_jmpbuf(png_ptr))) {
			fprintf(stderr, "Error during png creation\n");
			code = 1;
			goto finalise;
		}

		png_init_io(png_ptr, fp);

		// Write header (8 bit colour depth)
		png_set_IHDR(png_ptr, info_ptr, width * 32, height * 32,
			8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

		// Set title
		if (title != NULL) {
			png_text title_text;
			title_text.compression = PNG_TEXT_COMPRESSION_NONE;
			title_text.key = "Title";
			title_text.text = title;
			png_set_text(png_ptr, info_ptr, &title_text, 1);
		}

		png_write_info(png_ptr, info_ptr);

		// Allocate memory for one row (4 bytes per pixel - RGBA)
		row = (png_bytep)malloc(4 * width * 32 * sizeof(png_byte));

		// Write image data
		uint pos = 0;
		uint rowpos = 0;
		//for (int y = height-1; y >= 0; --y)

		char tsf[256];
		sprintf_s(tsf, "MCG.VOL_/%d.SPR", m_layout);
		int tilefilesize = get_file_size(tsf);
		byte* buffer = new byte[tilefilesize];

		FILE* cm = nullptr;
		fopen_s(&cm, tsf, "rb");
		fread(buffer, 1, tilefilesize, cm);
		fclose(cm);
		byte* tile_table = buffer;

		for (int y = 0; y < height; ++y)
		{
			for (uint line = 0; line < 32; ++line)
			{
				for (int x = 0; x < width; ++x)
				{
					byte tile = data[y * width + x];
					byte mask = maskdata[y * width + x];
					int offset = (0x400 * tile) + (line * 32);
					byte* pixeloffset = tile_table + offset;
					for (uint pixel = 0; pixel < 32; ++pixel)
					{
						byte c = 0xBB;
						if (offset < tilefilesize)
							c = pixeloffset[pixel];

						//Color color = get_color(c);
						Color color = m_palette[c >> 4];
					#ifdef SHOW_TRIGGERS
						if (mask != 0)
						{
							color.m_r = 255 - color.m_r;
							color.m_g = 255 - color.m_g;
							color.m_b = 255 - color.m_b;
						}
					#endif 
#define RENDER_GRID
#ifdef RENDER_GRID
						if (pixel == 0 || line == 0)
						{
							color.m_r = 0;
							color.m_g = 0;
							color.m_b = 0;
						}
#endif 
					#ifdef DEBUG_HEX_CHARS
						// Debug hex chars! These are 3 by 5 character
						if (pixel < (CHAR_WIDTH*2) && line < CHAR_HEIGHT) // Tile byte
						{
							byte val = pixel < CHAR_WIDTH ? tile >> 4 : tile & 0x0F;
							char* char_array = get_char_data(val);
							int x = pixel % CHAR_WIDTH;
							int mult = 0xFF;
							int colval = char_array[line * CHAR_WIDTH + x] * mult;
							color = get_color(colval);
						}
						else if (pixel < (CHAR_WIDTH * 4) && line < CHAR_HEIGHT) // Mask byte
						{
							int realpixel = pixel - CHAR_WIDTH * 2;
							byte val = realpixel < CHAR_WIDTH ? mask >> 4 : mask & 0x0F;
							char* char_array = get_char_data(val);
							int x = pixel % CHAR_WIDTH;
							int mult = 0xEE;
							if (mask != 0)
								mult = 0xAA;
							color = get_color(char_array[line * CHAR_WIDTH + x] * mult);
						}
					#endif //  DEBUG_HEX_CHARS

						row[rowpos++] = color.m_r;
						row[rowpos++] = color.m_g;
						row[rowpos++] = color.m_b;
						row[rowpos++] = color.m_a;
					}
				}
				png_write_row(png_ptr, row);
				rowpos = 0;
			}
		}
		delete[] buffer;
		// End write
		png_write_end(png_ptr, NULL);

	finalise:
		if (fp != NULL) fclose(fp);
		if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
		if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		if (row != NULL) free(row);
	}

	void dump_level_data()
	{
		static int filenr = 0;
		if (num_floors == 0)
			printf("");
		for (int i = 0; i < num_floors; ++i)
		{
			FILE* fp = nullptr;
			char filename[256];
			sprintf_s(filename, "./levels/%d_%s_%s_%d.dat", filenr, name, description, i);
			
			for (uint pos = 0; pos < strlen(filename); ++pos)
			{
				if (filename[pos] == '|')
					filename[pos] = '_';
			}

			fopen_s(&fp, filename, "wb+");
			if (fp)
			{
				fwrite(m_floors[i], 1, width* height, fp);
				fclose(fp);
			}
			else
			{
				printf("");
			}

		}
		++filenr;
	}

	void save_chunks()
	{
		static int i = 0;
		int fileoffset = 0;
		// header 0x00 - 0x60
		save_chunk(fileoffset, 0x60, i, -1, "HEADER", false); fileoffset += 0x60;
		
		// blockchunk 0x60 - mapoffset
		save_chunk(fileoffset, m_level_data_start - fileoffset, i, -1, "UNKNOWN", false); fileoffset = m_level_data_start;
			
		// map
		int floorsize = width * height;
		for (int floor = 0; floor < num_floors; ++floor)
		{
			save_chunk(fileoffset, floorsize, i, floor, "FLOORDATA", false); fileoffset += floorsize;
		}
		// mask
		for (int floor = 0; floor < num_floors; ++floor)
		{
			save_chunk(fileoffset, floorsize, i, floor, "FLOORDATA", true); fileoffset += floorsize;
		}

		// footer
		save_chunk(fileoffset, m_datalength - fileoffset, i, -1, "FOOTER", false); fileoffset += floorsize;
		++i;
	}


	void save_chunk(uint start, uint length, int filenr, int floornr, char* blockname, bool mask)
	{
		FILE* fp = nullptr;
		char filename[256];
		
		if (mask)
			sprintf_s(filename, "./MED.VOL_/extract/%d_%d_%s %s%s_mask.dat", filenr, floornr, blockname, name, description);
		else
			sprintf_s(filename, "./MED.VOL_/extract/%d_%d_%s %s%s.dat", filenr, floornr, blockname, name, description);

		for (uint pos = 0; pos < strlen(filename); ++pos)
		{
			if (filename[pos] == '|')
				filename[pos] = '_';
		}

		fopen_s(&fp, filename, "wb+");
		if (fp)
		{
			fwrite(m_data + start, 1, length, fp);
			fclose(fp);
		}
	}

private:
	byte* m_data;
	int m_datalength;
	short m_level_data_start;
	char* name;
	LAYOUT_NUMBER m_layout;
	char* description;
	int width;
	int height;
	int num_floors;
	Color m_palette[16];
	std::vector<byte*> m_floors;
	std::vector<byte*> m_masks;
};

void parse_med_vol()
{
	std::vector<int> start_offsets;
	std::vector<Layout> layouts;
	byte* data = nullptr;
	FILE* fp = nullptr;
	int size = 0;
	fopen_s(&fp, "med.vol", "rb");
	//fopen_s(&fp, "areaheaders", "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		rewind(fp);
		data = new byte[size];
		fread(data, 1, size, fp);
		fclose(fp);
	}

	for (int i = 0; i < size-1; ++i)
	{
		if (data[i] == 'M' && data[i + 1] == 'D')
		{
			start_offsets.push_back(i);
		}
	}

	for (uint i = 0; i < start_offsets.size(); ++i)
	{
		int start_offset = 0;
		int block_length = 0;
		if (i == start_offsets.size()-1)
		{
			start_offset = start_offsets[i];
			block_length = size - start_offset;
		}
		else
		{
			start_offset = start_offsets[i];
			block_length = start_offsets[i + 1] - start_offset;
		}

		Layout l(data + start_offset, block_length);
		l.parse_data();
		//l.dump_level_data();
		l.save_level_png();
		//l.save_chunks();
		layouts.push_back(l);
		
	}

	delete[] data;
}

#include <Windows.h>

void dump_vol_file(const std::string& file, const std::string& extension)
{
	FILE* fp = nullptr;
	fopen_s(&fp, file.c_str(), "rb");
	if (!fp)
		return;

	fseek(fp, 0, SEEK_END);
	int filesize = ftell(fp);
	rewind(fp);
	std::string dir = std::string(file.begin(), file.end());
	dir += "_";
	CreateDirectory(std::wstring(dir.begin(), dir.end()).c_str(), 0);

	std::vector<uint> offsets;
	uint first_offset;
	fread(&first_offset, 1, 4, fp);
	offsets.push_back(first_offset);
	uint num_files = first_offset / 4;
	for (uint i = 0; i < num_files - 1; ++i) // Header with file offsets
	{
		uint offset = 0;
		fread(&offset, 1, 4, fp);
		if (offset != 0)
			offsets.push_back(offset);
		
		
		
	}
	for (int o = 1; o < offsets.size(); ++o)
	{
		int length = 0;
		//if (o == offsets.size() - 1)
		//{
		//	length = filesize - offsets[o];
		//}
		//else
		{
			length = offsets[o] - offsets[o-1];
		}

		if (length >= filesize)
		{
			continue;
		}

		if (length > 0)
		{
			byte* buffer = new byte[length];
			fread(buffer, 1, length, fp);

			char destfilename[256];
			sprintf_s(destfilename, "%s/%d.%s", dir.c_str(), o, extension.c_str());
			FILE* output = nullptr;
			fopen_s(&output, destfilename, "wb+");
			if (output)
			{
				fwrite(buffer, 1, length, output);
				fclose(output);
			}

			delete[] buffer;
		}
	}

	fclose(fp);

}

int get_file_size(const std::string& file)
{
	int size = 0;
	FILE* fp = nullptr;
	fopen_s(&fp, file.c_str(), "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fclose(fp);
	}

	return size;
}

void convert_mcg_to_png()
{
	std::vector<std::string> mcg_files;
	mcg_files.push_back("MCG.VOL_/1.GC");
	mcg_files.push_back("MCG.VOL_/2.GC");
	mcg_files.push_back("MCG.VOL_/3.GC");
	mcg_files.push_back("MCG.VOL_/4.GC");
	mcg_files.push_back("MCG.VOL_/5.GC");
	mcg_files.push_back("MCG.VOL_/6.GC");
	mcg_files.push_back("MCG.VOL_/7.GC");
	mcg_files.push_back("MCG.VOL_/8.GC");
	mcg_files.push_back("MCG.VOL_/9.GC");
	mcg_files.push_back("MCG.VOL_/10.GC");
	mcg_files.push_back("MCG.VOL_/11.GC");
	mcg_files.push_back("MCG.VOL_/12.GC");
	mcg_files.push_back("MCG.VOL_/13.GC");
	mcg_files.push_back("MCG.VOL_/14.GC");

	for (int i = 0; i < mcg_files.size(); ++i)
	{
		int size = get_file_size(mcg_files[i]);
		if (size <= 0)
			continue;

		int MULTIPLIER = 40;

		int num_tiles = size / 32 / 32;
		int num_cols = MULTIPLIER;
		int num_rows = num_tiles / num_cols;

		if (num_tiles % MULTIPLIER != 0)
			num_rows++;


		int width = 32 * num_cols;
		int height = 32 * num_rows;

		uint bytes_per_pixel = 4;
		byte* buffer = new byte[size];

		FILE* cm = nullptr;
		fopen_s(&cm, mcg_files[i].c_str(), "rb");
		fread(buffer, 1, size, cm);
		fclose(cm);



		int code = 0;
		FILE *fp = nullptr;
		png_structp png_ptr = nullptr;
		png_infop info_ptr = nullptr;
		png_bytep row = nullptr;

		// Open file for writing (binary mode)
		std::string output_filename = mcg_files[i] + std::string(".png");
		char title[] = "Edge of Panic";
		fopen_s(&fp, output_filename.c_str(), "wb");

		if (fp == NULL)
		{
			fprintf(stderr, "Could not open file %s for writing\n", output_filename.c_str());
			code = 1;
			goto finalise;
		}

		// Initialize write structure
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL)
		{
			fprintf(stderr, "Could not allocate write struct\n");
			code = 1;
			goto finalise;
		}

		// Initialize info structure
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL)
		{
			fprintf(stderr, "Could not allocate info struct\n");
			code = 1;
			goto finalise;
		}

		// Setup Exception handling
		if (setjmp(png_jmpbuf(png_ptr))) {
			fprintf(stderr, "Error during png creation\n");
			code = 1;
			goto finalise;
		}

		png_init_io(png_ptr, fp);

		// Write header (8 bit colour depth)
		png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

		// Set title
		if (title != NULL) {
			png_text title_text;
			title_text.compression = PNG_TEXT_COMPRESSION_NONE;
			title_text.key = "Title";
			title_text.text = title;
			png_set_text(png_ptr, info_ptr, &title_text, 1);
		}

		png_write_info(png_ptr, info_ptr);

		// Allocate memory for one row (4 bytes per pixel - RGBA)
		row = (png_bytep)malloc(4 * width * sizeof(png_byte));

		// Write image data
		uint pos = 0;
		uint rowpos = 0;
		//for (int y = height-1; y >= 0; --y)

		for (int y = 0; y < height; ++y)
		{
			int offset = (y * 32) + ((y / 32) * MULTIPLIER * 0x400);

			for (int m = 0; m < MULTIPLIER; ++m)
			{
				for (int x = 0; x < 32; ++x)
				{
					byte c = 0xBB;

					if (offset < size)
						c = buffer[offset + x];
					else
						printf("");

					Color color = get_color(c);
					row[rowpos++] = color.m_r;
					row[rowpos++] = color.m_g;
					row[rowpos++] = color.m_b;
					row[rowpos++] = color.m_a;
				}
				offset += 0x400;
			}
			png_write_row(png_ptr, row);
			rowpos = 0;
		}

		// End write
		png_write_end(png_ptr, NULL);

	finalise:
		if (fp != NULL) fclose(fp);
		if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
		if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		if (row != NULL) free(row);

		delete[] buffer;
	}
}


void convert_gc_to_png(const std::string& gcfilename)
{
	int size = get_file_size(gcfilename);
	if (size <= 0)
		return;

	int width = 590;
	int height = 360;

	uint bytes_per_pixel = 4;
	byte* buffer = new byte[size];

	FILE* cm = nullptr;
	fopen_s(&cm, gcfilename.c_str(), "rb");
	fread(buffer, 1, size, cm);
	fclose(cm);

	int code = 0;
	FILE *fp = nullptr;
	png_structp png_ptr = nullptr;
	png_infop info_ptr = nullptr;
	png_bytep row = nullptr;

	// Open file for writing (binary mode)
	std::string output_filename = gcfilename + std::string(".png");
	char title[] = "Edge of Panic";
	fopen_s(&fp, output_filename.c_str(), "wb");

	if (fp == NULL)
	{
		fprintf(stderr, "Could not open file %s for writing\n", output_filename.c_str());
		code = 1;
		goto finalise;
	}

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		fprintf(stderr, "Could not allocate write struct\n");
		code = 1;
		goto finalise;
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fprintf(stderr, "Could not allocate info struct\n");
		code = 1;
		goto finalise;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error during png creation\n");
		code = 1;
		goto finalise;
	}

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height,
		8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// Set title
	if (title != NULL) {
		png_text title_text;
		title_text.compression = PNG_TEXT_COMPRESSION_NONE;
		title_text.key = "Title";
		title_text.text = title;
		png_set_text(png_ptr, info_ptr, &title_text, 1);
	}

	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (4 bytes per pixel - RGBA)
	row = (png_bytep)malloc(4 * width * sizeof(png_byte));

	// Write image data
	uint pos = 0;
	uint rowpos = 0;
	//for (int y = height-1; y >= 0; --y)

	int position = 0;
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			byte c = 0xBB;

			if (position < size)
				c = buffer[position++];
			else
				printf("");

			Color color = Color(c,c,c);
			row[rowpos++] = color.m_r;
			row[rowpos++] = color.m_g;
			row[rowpos++] = color.m_b;
			row[rowpos++] = color.m_a;
		}
		png_write_row(png_ptr, row);
		rowpos = 0;
	}

	// End write
	png_write_end(png_ptr, NULL);

finalise:
	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL) free(row);

	delete[] buffer;
}

void dump_pcm_files()
{
	std::vector<std::string> pcm_files;
	pcm_files.push_back("PCM1.VOL");
	pcm_files.push_back("PCM2.VOL");
	pcm_files.push_back("PCM3.VOL");
	pcm_files.push_back("PCM4.VOL");
	pcm_files.push_back("PCM5.VOL");
	pcm_files.push_back("PCM6.VOL");

	for (int i = 0; i < pcm_files.size(); ++i)
	{
		dump_vol_file(pcm_files[i], "voc");
	}
}

class VOLEntry
{
public:
	VOLEntry(int size, byte* data)
		: m_size(size)
	{
		m_data = new byte[size];
		memcpy(m_data, data, size);
	}

	~VOLEntry()
	{
		//delete[] m_data;
	}

	int size() const
	{
		return m_size;
	}

	byte* data() const
	{
		return m_data;
	}
private:
	byte* m_data;
	int m_size;
};

class VOLFile
{
public:
	VOLFile(const std::string& file)
	{
		read_vol_file(file);
	}

	void read_vol_file(const std::string& file)
	{
		FILE* fp = nullptr;
		fopen_s(&fp, file.c_str(), "rb");
		if (!fp)
			return;

		fseek(fp, 0, SEEK_END);
		int filesize = ftell(fp);
		rewind(fp);

		std::vector<uint> offsets;
		uint first_offset;
		fread(&first_offset, 1, 4, fp);
		offsets.push_back(first_offset);
		uint num_files = first_offset / 4;
		for (int i = 0; i < num_files - 1; ++i) // Header with file offsets
		{
			uint offset = 0;
			fread(&offset, 1, 4, fp);
			if (offset != 0)
				offsets.push_back(offset);

		}
		for (int o = 1; o < offsets.size(); ++o)
		{
			int length = 0;
			{
				length = offsets[o] - offsets[o - 1];
			}

			if (length >= filesize)
			{
				continue;
			}

			if (length > 0)
			{
				byte* buffer = new byte[length];
				fread(buffer, 1, length, fp);
				m_entries.push_back(new VOLEntry(length, buffer));
				delete[] buffer;
			}
		}

		fclose(fp);

	}

	void write_vol_file()
	{
		FILE* fp;
		fopen_s(&fp, "PIC3_.VOL", "wb+");
		if (!fp) return;
		int offset = 0x60; // pic3.vol shit
		fwrite(&offset, sizeof(int), 1, fp);
		for (int i = 0; i < m_entries.size(); ++i)
		{
			offset += m_entries[i]->size();
			fwrite(&offset, sizeof(int), 1, fp);
		}
		int remain = (0x60 / 4) - m_entries.size() - 1;
		for (int i = 0; i < remain; ++i)
		{
			offset = 0;
			fwrite(&offset, sizeof(int), 1, fp);
		}
		for (int i = 0; i < m_entries.size(); ++i)
		{
			fwrite(m_entries[i]->data(), 1, m_entries[i]->size(), fp);
		}

		fclose(fp);
		
	}

	void swap(int a, int b)
	{
		VOLEntry* temp = m_entries[a];
		m_entries[a] = m_entries[b];
		m_entries[b] = temp;
	}

	void reverse()
	{
		std::vector<VOLEntry*> reversed;
		for (int i = m_entries.size() - 1; i >= 0; --i)
		{
			reversed.push_back(m_entries[i]);
		}
		m_entries = reversed;
	}

	std::vector<VOLEntry*> m_entries;
};

void dump_vol_files()
{
	dump_vol_file("CODE.VOL", "COD");
	dump_vol_file("CUT1.VOL", "GC");
	dump_vol_file("CUT2.VOL", "GC");
	dump_vol_file("CUT3.VOL", "GC");
	dump_vol_file("CUTA.VOL", "GC");
	dump_vol_file("DAT.VOL", "DAT");
	dump_vol_file("ECG.VOL", "SPR");
	dump_vol_file("EMI.VOL", "EM");
	dump_vol_file("ENM.VOL", "GC");
	dump_vol_file("ICP.VOL", "TXT");
	dump_vol_file("MAP.VOL", "GC");
	dump_vol_file("MCG.VOL", "SPR");
	dump_vol_file("MED.VOL", "MD");
	dump_vol_file("OPENING.VOL", "GC");
	dump_vol_file("PCM1.VOL", "VOC");
	dump_vol_file("PCM2.VOL", "VOC");
	dump_vol_file("PCM3.VOL", "VOC");
	dump_vol_file("PCM4.VOL", "VOC");
	dump_vol_file("PCM5.VOL", "VOC");
	dump_vol_file("PCM6.VOL", "VOC");
	dump_vol_file("PIC1.VOL", "GC");
	dump_vol_file("PIC2.VOL", "GC");
	dump_vol_file("PIC3.VOL", "GC");
	dump_vol_file("PICA.VOL", "GC");
}


void dmg_test()
{
	uint64_t sword_a = 0;
	uint64_t sword_b = 0;

	for (int i = 0; i < 100000000; ++i)
	{
		int sword_a_dmg = (rand() % 21) + 40; // 40 - 60 = 0 - 20 + 40
		int sword_b_dmg = (rand() % 41) + 30; // 30 - 70 = 0 - 40 + 30

		sword_a += sword_a_dmg;
		sword_b += sword_b_dmg;
	}

	printf("");

}

int main(int argc, char* argv[])
{
	//dmg_test();
	//dump_vol_files();
	parse_med_vol();
	//dump_pcm_files();
	//dump_vol_file("MED.VOL", "MD");
	//dump_vol_file("MAP.VOL", "GC");
	//dump_vol_file("PIC2.VOL", "GC");
	//dump_vol_file("PIC3.VOL", "GC");
	//dump_vol_file("DAT.VOL", "DAT");
	//dump_vol_file("ECG.VOL", "GFX");
	//dump_vol_file("EMI.VOL", "EM");
	//dump_vol_file("ENM.VOL", "GFX");
	//dump_vol_file("ICP.VOL", "ICP");
	//dump_vol_file("CODE.VOL", "COD");
	//dump_vol_file("CUT1.VOL", "GC");
	//dump_vol_file("CUT2.VOL", "GC");
	//dump_vol_file("CUT3.VOL", "GC");
	//dump_vol_file("MCG.VOL", "GC");
//	convert_mcg_to_png();
	//convert_gc_to_png("MED.VOL_/1.MD");
	//VOLFile v("PIC3.VOL");
	//v.reverse();
	//v.write_vol_file();


	//char buffer[1048576];

	//	uint32_t buf[256];
	//	FILE* f;
	//	fopen_s(&f, argv[1], "rb");
	//	uint16_t headersize;
	//	uint32_t entrycount;
	//	fseek(f, 6, SEEK_SET);
	//	fread(&headersize, 2, 1, f);
	//	fread(&entrycount, 4, 1, f);
	//	fseek(f, headersize, SEEK_SET);
	//	fread(buf, 4, 256, f);
	//	for (size_t n = 0; n < entrycount; n++) 
	//	{
	//		char fn[100];
	//		sprintf_s(fn, "%s_%zu", argv[1], n);
	//		FILE* of;
	//		fopen_s(&of, fn, "wb");
	//		fseek(f, buf[n], SEEK_SET);
	//		fread(buffer, 1, buf[n + 1] - buf[n], f);
	//		fwrite(buffer, 1, buf[n + 1] - buf[n], of);
	//		fclose(of);
	//	}
	//	fclose(f);

	return 0;
}