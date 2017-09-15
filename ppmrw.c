// CS430_Project1.cpp : Defines the entry point for the console application.
//
// By: Kristoffer Schindele
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// struct representing pixel
typedef struct Pixel {
	unsigned char r, g, b, a;
} Pixel;

// declare variables
int p, t, w, trg, width, height, max_val;
Pixel* pixmap1d;

// convert to p3 function

void convertP3(char* outfile) {
	// detect which file type is being converted to P3
	// writing ASCII data
	FILE* fho = fopen(outfile, "w");
	if (t == 3) {
		// copy header data
		unsigned char space = ' ';
		// write magic number
		fwrite(&p, sizeof(char), 1, fho);
		fwrite(&t, sizeof(char), 1, fho);
		fwrite(&space, sizeof(char), 1, fho);
		// write width
		fwrite(&width, sizeof(char), 1, fho);
		fwrite(&space, sizeof(char), 1, fho);
		// write height
		fwrite(&height, sizeof(char), 1, fho);
		fwrite(&space, sizeof(char), 1, fho);
		// write maxval
		fwrite(&max_val, sizeof(char), 1, fho);
		fwrite(&space, sizeof(char), 1, fho);
		// write raster
		fwrite(&pixmap1d, sizeof(Pixel), width*height, fho);
		// close file
		fclose(fho);
		// done
		exit(0);
	}
	/*
	else {
	// convert to P6 (ASCII to binary)
	FILE* fho = fopen(argv[3], "wb");
	// copy header data
	// write magic number
	fwrite(p, sizeof(char), 1, fho);
	fwrite(t, sizeof(char), 1, fho);
	fwrite(space, sizeof(char), 1, fho);
	// write width
	fwrite(width, sizeof(char), 1, fho);
	fwrite(space, sizeof(char), 1, fho);
	// write height
	fwrite(height, sizeof(char), 1, fho);
	fwrite(space, sizeof(char), 1, fho);
	// write maxval
	fwrite(max_val, sizeof(char), 1, fho);
	fwrite(space, sizeof(char), 1, fho);
	// write raster
	}
	*/
}

int main(int argc, char *argv[]) {

	// argument checking
	if (argc != 4) {
		fprintf(stderr, "Error: Invalid number of arguments.");
		return 1;
	}
	else if (argv[2] == NULL || argv[3] == NULL) {
		fprintf(stderr, "Error reading or writing file.");
		return 1;
	}
	// store the target file type in trg
	trg = atoi(argv[1]);
	// always read binary when working on windows
	FILE* fhi = fopen(argv[2], "rb"); // open file for reading
	// check file header for type
	// read in file header
	// store first character in dummy variable
	p = fgetc(fhi);
	// store MAGIC NUMBER in t
	t = fgetc(fhi);
	// travel through whitespace until a new character is reached
	w = fgetc(fhi);
	while (w == ' ' || w == '\n') {
		printf("banana");
		if (w == '#') {
			while (w != '\n') {
				w = fgetc(fhi);
			}
 		}
		w = fgetc(fhi);
	}
	// next character will be file width
	width = w;
	// repeat travel process untill height value is reached
	w = fgetc(fhi);
	while (w == ' ' || w == '\n') {
		if (w == '#') {
			while (w != '\n') {
				w = fgetc(fhi);
			}
		}
		w = fgetc(fhi);
	}
	height = w;
	// repeat travel process until maxval character is reached
	w = fgetc(fhi);
	while (w == ' ' || w == '\n') {
		w = fgetc(fhi);
		if (w == '#') {
			while (w != '\n') {
				w = fgetc(fhi);
			}
		}
	}
	max_val = w;
	if (max_val > 255) {
		fprintf(stderr, "Error: file is not on a 255 color scale.");
		return 1;
	}
	// repeat travel process until pixel data is reached
	w = fgetc(fhi);
	while (w == ' ' || w == '\n') {
		if (w == '#') {
			while (w != '\n') {
				w = fgetc(fhi);
			}
		}
		w = fgetc(fhi);
	}
	// w will be the first byte of pixel data
	// unget the first character
	w = ungetc(w, fhi);
	// allocate memory based on file size
	pixmap1d = malloc(sizeof(Pixel)*width*height);
	// read data into buffer
	switch (t) {
	case 3:
		//P3, data stored in ASCII, rgb
		int r_value;
		int g_value;
		int b_value;
		for (int i = 0; i < (width*height); i += 1) {
			fscanf(fhi, "%d" "%d" "%d", &r_value, &g_value, &b_value);
			pixmap1d[i].r = r_value;
			pixmap1d[i].g = g_value;
			pixmap1d[i].b = b_value;
		}
		break;
		/*
	case 6:
		// P6, binary data, rgb
		int r_value;
		int g_value;
		int b_value;
		for (int i = 0; i < (width*height); i += 1) {
			fscanf(fhi, "%d" "%d" "%d", &r_value, &g_value, &b_value);
			pixmap1d[i].r = r_value;
			pixmap1d[i].g = g_value;
			pixmap1d[i].b = b_value;
		}
		*/
	}
	// jump to appropriate routine
	// argument supplied in command line determines the format to be converted TO
	// outfile is passed into function
	switch (trg) {
	case 3:
		convertP3(argv[3]);
		break;
	default:
		fprintf(stderr, "Error: Invalid file.");
	}

	return 1;
}
