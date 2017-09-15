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
// NOTE: p = dummy char variable, t = input file ppm type,
//       w = dummy char variable for traversing white space
int p, t, w, target_ppm_type, width, height, max_color_value;
Pixel* pixmap1d;

// Function converts either P3 or P6 type ppm files to a P3 type ppm file
// Params - outfile: the new file to write to
// Returns - Nothing
void convertP3(char* outfile) {
	// detect which file type is being converted to P3
	// writing ASCII data
	FILE* file_handle_out = fopen(outfile, "w");
	if (t == 3) {
		// copy header data
		unsigned char space = ' ';
		// write magic number
		fwrite(&p, sizeof(char), 1, file_handle_out);
		fwrite(&t, sizeof(char), 1, file_handle_out);
		fwrite(&space, sizeof(char), 1, file_handle_out);
		// write width
		fwrite(&width, sizeof(char), 1, file_handle_out);
		fwrite(&space, sizeof(char), 1, file_handle_out);
		// write height
		fwrite(&height, sizeof(char), 1, file_handle_out);
		fwrite(&space, sizeof(char), 1, file_handle_out);
		// write maxval
		fwrite(&max_color_value, sizeof(char), 1, file_handle_out);
		fwrite(&space, sizeof(char), 1, file_handle_out);
		// write raster
		fwrite(&pixmap1d, sizeof(Pixel), width*height, file_handle_out);
		// close file
		fclose(file_handle_out);
		// done
		exit(0);
	}
	/*
	else {
	// convert to P6 (ASCII to binary)
	FILE* file_handle_out = fopen(argv[3], "wb");
	// copy header data
	// write magic number
	fwrite(p, sizeof(char), 1, file_handle_out);
	fwrite(t, sizeof(char), 1, file_handle_out);
	fwrite(space, sizeof(char), 1, file_handle_out);
	// write width
	fwrite(width, sizeof(char), 1, file_handle_out);
	fwrite(space, sizeof(char), 1, file_handle_out);
	// write height
	fwrite(height, sizeof(char), 1, file_handle_out);
	fwrite(space, sizeof(char), 1, file_handle_out);
	// write maxval
	fwrite(max_color_value, sizeof(char), 1, file_handle_out);
	fwrite(space, sizeof(char), 1, file_handle_out);
	// write raster
	}
	*/
}


// Main function performs error checking, reads in data from file, and decides
// which conversion function to run based on that file.
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
	// store the target file type in target_ppm_type
	target_ppm_type = atoi(argv[1]);
	// always read binary when working on windows
	// TODO: figure our if this is necessary now that I'm developing on a linux machine
	FILE* file_handle_in = fopen(argv[2], "rb"); // open file for reading
	// read in file header
	p = fgetc(file_handle_in); // store first character in dummy variable
	t = fgetc(file_handle_in); // store MAGIC NUMBER in t
	// travel through whitespace until a new character is reached
	// NOTE: fgetc advances the cursor each time it grabs a character
	// TODO: factor out traversal function
	w = fgetc(file_handle_in);
	while (w == ' ' || w == '\n') {
		printf("whitespace\n");
		if (w == '#') {
			while (w != '\n') {
				w = fgetc(file_handle_in);
			}
 		}
		w = fgetc(file_handle_in);
	}
	// next character will be file width
	width = w;
	// repeat travel process untill height value is reached
	w = fgetc(file_handle_in);
	while (w == ' ' || w == '\n') {
		if (w == '#') {
			while (w != '\n') {
				w = fgetc(file_handle_in);
			}
		}
		w = fgetc(file_handle_in);
	}
	height = w;
	// repeat travel process until maxval character is reached
	w = fgetc(file_handle_in);
	while (w == ' ' || w == '\n') {
		w = fgetc(file_handle_in);
		if (w == '#') {
			while (w != '\n') {
				w = fgetc(file_handle_in);
			}
		}
	}
	max_color_value = w;
	if (max_color_value > 255) {
		fprintf(stderr, "Error: file is not on a 255 color scale.");
		return 1;
	}
	// repeat travel process until pixel data is reached
	w = fgetc(file_handle_in);
	while (w == ' ' || w == '\n') {
		if (w == '#') {
			while (w != '\n') {
				w = fgetc(file_handle_in);
			}
		}
		w = fgetc(file_handle_in);
	}
	// w will be the first byte of pixel data
	// unget the first character
	w = ungetc(w, file_handle_in);
	// allocate memory based on file size
	pixmap1d = malloc(sizeof(Pixel)*width*height);
	// declare variables for color values
	int r_value, g_value, b_value;
	// read data into buffer
	switch (t) {
		case 3:
			// P3, data stored in ASCII, rgb

			for (int i = 0; i < (width*height); i += 1) {
				fscanf(file_handle_in, "%d" "%d" "%d", &r_value, &g_value, &b_value);
				pixmap1d[i].r = r_value;
				pixmap1d[i].g = g_value;
				pixmap1d[i].b = b_value;
			}
			break;
		case 6:
			// P6, binary data, rgb
			for (int i = 0; i < (width*height); i += 1) {
				fscanf(file_handle_in, "%d" "%d" "%d", &r_value, &g_value, &b_value);
				pixmap1d[i].r = r_value;
				pixmap1d[i].g = g_value;
				pixmap1d[i].b = b_value;
			}
	}
	// jump to appropriate routine
	// argument supplied in command line determines the format to be converted TO
	// outfile is passed into function
	switch (target_ppm_type) {
	case 3:
		convertP3(argv[3]);
		break;
	case 6:
		// TODO: function that converts P3 and P6 to P6
		break;
	default:
		fprintf(stderr, "Error: Invalid file.");
	}

	return 1;
}
