// program converts between p3 and p6 type .ppm images
// By: Kristoffer Schindele
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// struct representing pixel
typedef struct Pixel {
	unsigned char r, g, b;
} Pixel;

// declare variables
// NOTE: p = dummy char variable, t = input file ppm type,
//       w = dummy char variable for traversing white space
int p, t, target_ppm_type, width, height, max_color_value;
Pixel* pixmap1d; // one dimensional array to hold pixel data
Pixel test_pixel;
unsigned char space = ' ';
unsigned char newline = '\n';
unsigned char hash = '#';

// Function traverses white space until a character that is not a space, newline,
// or a comment line
// Params - file_to_read: a pointer to a file handle to be read
// Returns - NOTHING
void traverse_whitespace(FILE* file_to_read) {
	// Initialize with the first character in the file
	int current_char = fgetc(file_to_read);
	// Traverse the file, ignoring whitespace and newlines
	while (current_char == ' ' || current_char == '\n' || current_char == '#') {
		// If hash, skip the entire line
		if (current_char == '#') {
			current_char = fgetc(file_to_read); // Initialize loop with next char
			while (current_char != '\n') { // While not newline
				current_char = fgetc(file_to_read);
				// current_char now holds newline, break out of loop
			}
		}
		current_char = fgetc(file_to_read); // Get the next char
	}
	// Unget the last non-whitespace character
	ungetc(current_char, file_to_read);
}

// Function converts either P3 or P6 type ppm files to a P3 type ppm file
// Params - outfile: the new file to write to
// Returns - NOTHING
void convert_to_p3(char* outfile) {
	printf("Converting to P3 . . .\n");
	// initialize a file for writing ASCII data
	FILE* file_handle_out = fopen(outfile, "w");
	// copy header data
	fwrite(&p, sizeof(char), 1, file_handle_out); // write P
	fprintf(file_handle_out, "%d", 3); // write magic number
	fwrite(&newline, sizeof(char), 1, file_handle_out);
	fwrite(&hash, sizeof(char), 1, file_handle_out); // write a hash
	fwrite(&newline, sizeof(char), 1, file_handle_out);
	fprintf(file_handle_out, "%d", width); // write width
	fwrite(&space, sizeof(char), 1, file_handle_out);
	fprintf(file_handle_out, "%d", height); // write height
	fwrite(&newline, sizeof(char), 1, file_handle_out);
	fprintf(file_handle_out, "%d", max_color_value); // write maxval
	fwrite(&newline, sizeof(char), 1, file_handle_out);
	// write each pixel from the raster
	for (int i = 0; i < width*height; i += 1) {
		fprintf(file_handle_out, "%d", pixmap1d[i].r); // write red value
		fwrite(&newline, sizeof(char), 1, file_handle_out);
		fprintf(file_handle_out, "%d", pixmap1d[i].g); // write green value
		fwrite(&newline, sizeof(char), 1, file_handle_out);
		fprintf(file_handle_out, "%d", pixmap1d[i].b); // write blue value
		fwrite(&newline, sizeof(char), 1, file_handle_out);
	}
	// close file
	fclose(file_handle_out);
}

// Function converts either P3 or P6 type ppm files to a P6 type ppm file
// Params - outfile: the new file to write to
// Returns - NOTHING
void convert_to_p6(char* outfile) {
	printf("Converting to P6 . . .\n");
	// initialize file for writing binary data
	FILE* file_handle_out = fopen(outfile, "wb");
	// var to hold magic number for writing purposes
	int t = 6;
	// copy header data
	fwrite(&p, sizeof(char), 1, file_handle_out); // write P
	fprintf(file_handle_out, "%d", 6); // write magic number
	fwrite(&newline, sizeof(char), 1, file_handle_out);
	fwrite(&hash, sizeof(char), 1, file_handle_out); // write a hash
	fwrite(&newline, sizeof(char), 1, file_handle_out);
	fprintf(file_handle_out, "%d", width); // write width
	fwrite(&space, sizeof(char), 1, file_handle_out);
	fprintf(file_handle_out, "%d", height); // write height
	fwrite(&newline, sizeof(char), 1, file_handle_out);
	fprintf(file_handle_out, "%d", max_color_value); // write maxval
	fwrite(&newline, sizeof(char), 1, file_handle_out);
	// write each pixel from the raster
	for (int i = 0; i < width*height; i += 1) {
		fwrite(&pixmap1d[i].r, sizeof(char), 1, file_handle_out); // write red value
		//fwrite(&newline, sizeof(char), 1, file_handle_out);
		fwrite(&pixmap1d[i].g, sizeof(char), 1, file_handle_out); // write green value
		//fwrite(&newline, sizeof(char), 1, file_handle_out);
		fwrite(&pixmap1d[i].b, sizeof(char), 1, file_handle_out); // write blue value
		//fwrite(&newline, sizeof(char), 1, file_handle_out);
	}
	// close file
	fclose(file_handle_out);
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
	// always read binary
	FILE* file_handle_in = fopen(argv[2], "rb"); // open file for reading
	// read in file header
	p = fgetc(file_handle_in); // store first character in dummy variable
	fscanf(file_handle_in, "%d", &t); // store MAGIC NUMBER in t
	printf("Input file type: %d\n", t);
	// traverse white space until width value is reached
	traverse_whitespace(file_handle_in);
	// store the width value
	fscanf(file_handle_in, "%d", &width);
	printf("Image width: %d\n", width);
	// traverse white space until height value is reached
	traverse_whitespace(file_handle_in);
	// store the height value
	fscanf(file_handle_in, "%d", &height);
	printf("Image height: %d\n", height);
	// traverse white space until color max val is reached
	traverse_whitespace(file_handle_in);
	// store the max color value
	fscanf(file_handle_in, "%d", &max_color_value);
	printf("Max color value: %d\n", max_color_value);
	if (max_color_value > 255) {
		fprintf(stderr, "Error: file is not on a 255 color scale.");
		return 1;
	}
	// traverse white space until pixel data is reached
	traverse_whitespace(file_handle_in);
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
				fread(&pixmap1d[i], sizeof(Pixel), 1, file_handle_in);
			}
	}
	// ppm data is now stored in pixmap1d

	// switch based on target ppm type
	switch (target_ppm_type) {
	case 3:
		convert_to_p3(argv[3]);
		break;
	case 6:
		convert_to_p6(argv[3]);
		break;
	default:
		fprintf(stderr, "Error: Invalid file.");
	}
	// close the input file before exiting
	fclose(file_handle_in);
	return 0;
}
