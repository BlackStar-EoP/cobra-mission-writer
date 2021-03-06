int CHAR_WIDTH = 5;
int CHAR_HEIGHT = 5;


char char_0[] {
0, 1, 1, 0, 0,
1, 0, 0, 1, 0,
1, 0, 0, 1, 0,
1, 0, 0, 1, 0,
0, 1, 1, 0, 0,
};
char char_1[] {
0, 0, 1, 0, 0,
0, 1, 1, 0, 0,
0, 0, 1, 0, 0,
0, 0, 1, 0, 0,
0, 1, 1, 1, 0,
};

char char_2[] {
0, 1, 1, 0, 0,
1, 0, 0, 1, 0,
0, 0, 1, 0, 0,
0, 1, 0, 0, 0,
1, 1, 1, 1, 0,
};

char char_3[] {
0, 1, 1, 0, 0,
1, 0, 0, 1, 0,
0, 0, 1, 0, 0,
1, 0, 0, 1, 0,
0, 1, 1, 0, 0,
};
char char_4[] {
1, 0, 0, 1, 0,
1, 0, 0, 1, 0,
1, 1, 1, 1, 0,
0, 0, 0, 1, 0,
0, 0, 0, 1, 0,
};
char char_5[] {
1, 1, 1, 1, 0,
1, 0, 0, 0, 0,
1, 1, 1, 0, 0,
0, 0, 0, 1, 0,
1, 1, 1, 0, 0,
};
char char_6[] {
0, 1, 1, 1, 0,
1, 0, 0, 0, 0,
1, 1, 1, 0, 0,
1, 0, 0, 1, 0,
0, 1, 1, 0, 0,
};
char char_7[] {
1, 1, 1, 1, 0,
0, 0, 0, 1, 0,
0, 0, 1, 0, 0,
0, 1, 0, 0, 0,
0, 1, 0, 0, 0,
};
char char_8[] {
0, 1, 1, 0, 0,
1, 0, 0, 1, 0,
0, 1, 1, 0, 0,
1, 0, 0, 1, 0,
0, 1, 1, 0, 0,
};
char char_9[] {
0, 1, 1, 0, 0,
1, 0, 0, 1, 0,
0, 1, 1, 1, 0,
0, 0, 0, 1, 0,
0, 1, 1, 0, 0,
};
char char_a[] {
0, 1, 1, 0, 0,
1, 0, 0, 1, 0,
1, 1, 1, 1, 0,
1, 0, 0, 1, 0,
1, 0, 0, 1, 0,
};
char char_b[] {
1, 1, 1, 0, 0,
1, 0, 0, 1, 0,
1, 1, 1, 0, 0,
1, 0, 0, 1, 0,
1, 1, 1, 0, 0,
};
char char_c[] {
0, 1, 1, 0, 0,
1, 0, 0, 1, 0,
1, 0, 0, 0, 0,
1, 0, 0, 1, 0,
0, 1, 1, 0, 0,
};
char char_d[] {
1, 1, 1, 0, 0,
1, 0, 0, 1, 0,
1, 0, 0, 1, 0,
1, 0, 0, 1, 0,
1, 1, 1, 0, 0,
};
char char_e[] {
1, 1, 1, 1, 0,
1, 0, 0, 0, 0,
1, 1, 1, 1, 0,
1, 0, 0, 0, 0,
1, 1, 1, 1, 0,
};
char char_f[] {
1, 1, 1, 1, 0,
1, 0, 0, 0, 0,
1, 1, 1, 1, 0,
1, 0, 0, 0, 0,
1, 0, 0, 0, 0,
};

static char* arrays[16] = { char_0,
char_1,
char_2,
char_3,
char_4,
char_5,
char_6,
char_7,
char_8,
char_9,
char_a,
char_b,
char_c,
char_d,
char_e,
char_f };


char* get_char_data(unsigned char value)
{
	if (value > 15) 
		return nullptr;
	return arrays[value];
}

char* gc_files[266] =
{
	"CUT1.VOL_\\1.GC",
	"CUT1.VOL_\\10.GC",
	"CUT1.VOL_\\11.GC",
	"CUT1.VOL_\\12.GC",
	"CUT1.VOL_\\13.GC",
	"CUT1.VOL_\\14.GC",
	"CUT1.VOL_\\15.GC",
	"CUT1.VOL_\\16.GC",
	"CUT1.VOL_\\17.GC",
	"CUT1.VOL_\\18.GC",
	"CUT1.VOL_\\19.GC",
	"CUT1.VOL_\\2.GC",
	"CUT1.VOL_\\20.GC",
	"CUT1.VOL_\\21.GC",
	"CUT1.VOL_\\22.GC",
	"CUT1.VOL_\\23.GC",
	"CUT1.VOL_\\24.GC",
	"CUT1.VOL_\\25.GC",
	"CUT1.VOL_\\26.GC",
	"CUT1.VOL_\\27.GC",
	"CUT1.VOL_\\28.GC",
	"CUT1.VOL_\\29.GC",
	"CUT1.VOL_\\3.GC",
	"CUT1.VOL_\\30.GC",
	"CUT1.VOL_\\31.GC",
	"CUT1.VOL_\\32.GC",
	"CUT1.VOL_\\33.GC",
	"CUT1.VOL_\\34.GC",
	"CUT1.VOL_\\35.GC",
	"CUT1.VOL_\\36.GC",
	"CUT1.VOL_\\37.GC",
	"CUT1.VOL_\\38.GC",
	"CUT1.VOL_\\39.GC",
	"CUT1.VOL_\\4.GC",
	"CUT1.VOL_\\5.GC",
	"CUT1.VOL_\\6.GC",
	"CUT1.VOL_\\7.GC",
	"CUT1.VOL_\\8.GC",
	"CUT1.VOL_\\9.GC",
	"CUT2.VOL_\\1.GC",
	"CUT2.VOL_\\10.GC",
	"CUT2.VOL_\\11.GC",
	"CUT2.VOL_\\12.GC",
	"CUT2.VOL_\\13.GC",
	"CUT2.VOL_\\14.GC",
	"CUT2.VOL_\\15.GC",
	"CUT2.VOL_\\16.GC",
	"CUT2.VOL_\\17.GC",
	"CUT2.VOL_\\18.GC",
	"CUT2.VOL_\\19.GC",
	"CUT2.VOL_\\2.GC",
	"CUT2.VOL_\\20.GC",
	"CUT2.VOL_\\21.GC",
	"CUT2.VOL_\\22.GC",
	"CUT2.VOL_\\23.GC",
	"CUT2.VOL_\\24.GC",
	"CUT2.VOL_\\25.GC",
	"CUT2.VOL_\\26.GC",
	"CUT2.VOL_\\27.GC",
	"CUT2.VOL_\\28.GC",
	"CUT2.VOL_\\29.GC",
	"CUT2.VOL_\\3.GC",
	"CUT2.VOL_\\30.GC",
	"CUT2.VOL_\\31.GC",
	"CUT2.VOL_\\32.GC",
	"CUT2.VOL_\\33.GC",
	"CUT2.VOL_\\34.GC",
	"CUT2.VOL_\\35.GC",
	"CUT2.VOL_\\36.GC",
	"CUT2.VOL_\\37.GC",
	"CUT2.VOL_\\38.GC",
	"CUT2.VOL_\\39.GC",
	"CUT2.VOL_\\4.GC",
	"CUT2.VOL_\\40.GC",
	"CUT2.VOL_\\41.GC",
	"CUT2.VOL_\\42.GC",
	"CUT2.VOL_\\43.GC",
	"CUT2.VOL_\\44.GC",
	"CUT2.VOL_\\45.GC",
	"CUT2.VOL_\\5.GC",
	"CUT2.VOL_\\6.GC",
	"CUT2.VOL_\\7.GC",
	"CUT2.VOL_\\8.GC",
	"CUT2.VOL_\\9.GC",
	"CUT3.VOL_\\1.GC",
	"CUT3.VOL_\\2.GC",
	"CUT3.VOL_\\3.GC",
	"CUT3.VOL_\\4.GC",
	"CUT3.VOL_\\5.GC",
	"CUT3.VOL_\\6.GC",
	"CUTA.VOL_\\1.GC",
	"CUTA.VOL_\\10.GC",
	"CUTA.VOL_\\11.GC",
	"CUTA.VOL_\\15.GC",
	"CUTA.VOL_\\18.GC",
	"CUTA.VOL_\\2.GC",
	"CUTA.VOL_\\3.GC",
	"CUTA.VOL_\\30.GC",
	"CUTA.VOL_\\33.GC",
	"CUTA.VOL_\\34.GC",
	"CUTA.VOL_\\36.GC",
	"CUTA.VOL_\\37.GC",
	"CUTA.VOL_\\38.GC",
	"CUTA.VOL_\\4.GC",
	"CUTA.VOL_\\44.GC",
	"CUTA.VOL_\\46.GC",
	"CUTA.VOL_\\47.GC",
	"CUTA.VOL_\\48.GC",
	"CUTA.VOL_\\5.GC",
	"CUTA.VOL_\\58.GC",
	"CUTA.VOL_\\6.GC",
	"CUTA.VOL_\\65.GC",
	"CUTA.VOL_\\7.GC",
	"CUTA.VOL_\\73.GC",
	"CUTA.VOL_\\78.GC",
	"CUTA.VOL_\\79.GC",
	"CUTA.VOL_\\8.GC",
	"CUTA.VOL_\\80.GC",
	"CUTA.VOL_\\9.GC",
	"ENM.VOL_\\1.GC",
	"ENM.VOL_\\10.GC",
	"ENM.VOL_\\11.GC",
	"ENM.VOL_\\12.GC",
	"ENM.VOL_\\13.GC",
	"ENM.VOL_\\14.GC",
	"ENM.VOL_\\15.GC",
	"ENM.VOL_\\16.GC",
	"ENM.VOL_\\17.GC",
	"ENM.VOL_\\18.GC",
	"ENM.VOL_\\19.GC",
	"ENM.VOL_\\2.GC",
	"ENM.VOL_\\20.GC",
	"ENM.VOL_\\21.GC",
	"ENM.VOL_\\22.GC",
	"ENM.VOL_\\23.GC",
	"ENM.VOL_\\24.GC",
	"ENM.VOL_\\25.GC",
	"ENM.VOL_\\26.GC",
	"ENM.VOL_\\27.GC",
	"ENM.VOL_\\28.GC",
	"ENM.VOL_\\29.GC",
	"ENM.VOL_\\3.GC",
	"ENM.VOL_\\30.GC",
	"ENM.VOL_\\31.GC",
	"ENM.VOL_\\32.GC",
	"ENM.VOL_\\33.GC",
	"ENM.VOL_\\34.GC",
	"ENM.VOL_\\35.GC",
	"ENM.VOL_\\4.GC",
	"ENM.VOL_\\5.GC",
	"ENM.VOL_\\6.GC",
	"ENM.VOL_\\7.GC",
	"ENM.VOL_\\8.GC",
	"ENM.VOL_\\9.GC",
	"MAP.VOL_\\1.GC",
	"MAP.VOL_\\10.GC",
	"MAP.VOL_\\11.GC",
	"MAP.VOL_\\12.GC",
	"MAP.VOL_\\16.GC",
	"MAP.VOL_\\2.GC",
	"MAP.VOL_\\3.GC",
	"MAP.VOL_\\4.GC",
	"MAP.VOL_\\5.GC",
	"MAP.VOL_\\6.GC",
	"MAP.VOL_\\7.GC",
	"MAP.VOL_\\8.GC",
	"MAP.VOL_\\9.GC",
	"OPENING.VOL_\\1.GC",
	"OPENING.VOL_\\10.GC",
	"OPENING.VOL_\\11.GC",
	"OPENING.VOL_\\12.GC",
	"OPENING.VOL_\\13.GC",
	"OPENING.VOL_\\14.GC",
	"OPENING.VOL_\\15.GC",
	"OPENING.VOL_\\16.GC",
	"OPENING.VOL_\\17.GC",
	"OPENING.VOL_\\18.GC",
	"OPENING.VOL_\\19.GC",
	"OPENING.VOL_\\2.GC",
	"OPENING.VOL_\\3.GC",
	"OPENING.VOL_\\4.GC",
	"OPENING.VOL_\\5.GC",
	"OPENING.VOL_\\6.GC",
	"OPENING.VOL_\\7.GC",
	"OPENING.VOL_\\8.GC",
	"OPENING.VOL_\\9.GC",
	"PIC1.VOL_\\1.GC",
	"PIC1.VOL_\\10.GC",
	"PIC1.VOL_\\11.GC",
	"PIC1.VOL_\\12.GC",
	"PIC1.VOL_\\13.GC",
	"PIC1.VOL_\\14.GC",
	"PIC1.VOL_\\15.GC",
	"PIC1.VOL_\\16.GC",
	"PIC1.VOL_\\17.GC",
	"PIC1.VOL_\\2.GC",
	"PIC1.VOL_\\3.GC",
	"PIC1.VOL_\\4.GC",
	"PIC1.VOL_\\5.GC",
	"PIC1.VOL_\\6.GC",
	"PIC1.VOL_\\7.GC",
	"PIC1.VOL_\\8.GC",
	"PIC1.VOL_\\9.GC",
	"PIC2.VOL_\\1.GC",
	"PIC2.VOL_\\10.GC",
	"PIC2.VOL_\\11.GC",
	"PIC2.VOL_\\12.GC",
	"PIC2.VOL_\\13.GC",
	"PIC2.VOL_\\14.GC",
	"PIC2.VOL_\\15.GC",
	"PIC2.VOL_\\16.GC",
	"PIC2.VOL_\\17.GC",
	"PIC2.VOL_\\2.GC",
	"PIC2.VOL_\\3.GC",
	"PIC2.VOL_\\4.GC",
	"PIC2.VOL_\\5.GC",
	"PIC2.VOL_\\6.GC",
	"PIC2.VOL_\\7.GC",
	"PIC2.VOL_\\8.GC",
	"PIC2.VOL_\\9.GC",
	"PIC3.VOL_\\1.GC",
	"PIC3.VOL_\\10.GC",
	"PIC3.VOL_\\11.GC",
	"PIC3.VOL_\\12.GC",
	"PIC3.VOL_\\13.GC",
	"PIC3.VOL_\\14.GC",
	"PIC3.VOL_\\15.GC",
	"PIC3.VOL_\\16.GC",
	"PIC3.VOL_\\17.GC",
	"PIC3.VOL_\\2.GC",
	"PIC3.VOL_\\3.GC",
	"PIC3.VOL_\\4.GC",
	"PIC3.VOL_\\5.GC",
	"PIC3.VOL_\\6.GC",
	"PIC3.VOL_\\7.GC",
	"PIC3.VOL_\\8.GC",
	"PIC3.VOL_\\9.GC",
	"PICA.VOL_\\1.GC",
	"PICA.VOL_\\10.GC",
	"PICA.VOL_\\11.GC",
	"PICA.VOL_\\12.GC",
	"PICA.VOL_\\13.GC",
	"PICA.VOL_\\14.GC",
	"PICA.VOL_\\15.GC",
	"PICA.VOL_\\16.GC",
	"PICA.VOL_\\17.GC",
	"PICA.VOL_\\18.GC",
	"PICA.VOL_\\19.GC",
	"PICA.VOL_\\2.GC",
	"PICA.VOL_\\20.GC",
	"PICA.VOL_\\21.GC",
	"PICA.VOL_\\22.GC",
	"PICA.VOL_\\23.GC",
	"PICA.VOL_\\24.GC",
	"PICA.VOL_\\25.GC",
	"PICA.VOL_\\26.GC",
	"PICA.VOL_\\27.GC",
	"PICA.VOL_\\28.GC",
	"PICA.VOL_\\29.GC",
	"PICA.VOL_\\3.GC",
	"PICA.VOL_\\4.GC",
	"PICA.VOL_\\5.GC",
	"PICA.VOL_\\6.GC",
	"PICA.VOL_\\7.GC",
	"PICA.VOL_\\8.GC",
	"PICA.VOL_\\9.GC",
};