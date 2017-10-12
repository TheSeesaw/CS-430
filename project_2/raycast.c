// program takes in a CSV file and creates an image based on the drawn objects
// By: Kristoffer Schindele
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// GLOBAL VARIABLES

Pixel *pixmap1d;
unsigned char space = ' ';
unsigned char newline = '\n';
unsigned char hash = '#';


// STRUCT DEFINITIONS

// struct representing pixel
typedef struct Pixel {
	unsigned char r, g, b;
} Pixel;

// struct representing a shape
// NOTE: all shapes share the same definition,
// but some fields are unused
typedef struct Shape {
	int type; // 1 = camera, 2 = sphere, 3 = plane
  int x_pos, y_pos, z_pos;
	int color_arr[3]; // 0 = r, 1 = g, 2 = b
  int radius;
  // TODO: implement plane primitives
} Shape;


// FUNCTION DECLARATIONS

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


// MAIN FUNCTION

// Main function performs error checking, reads in data from scene file, and
// passes scene object to a file writer.
int main(int argc, char *argv[]) {
	// argument checking
	if (argc != 5) {
		fprintf(stderr, "Error: Invalid number of arguments.");
		return 1;
	}
	else if (argv[3] == NULL || argv[4] == NULL) {
		fprintf(stderr, "Error reading or writing file.");
		return 1;
	}

	// always read binary
	FILE* file_handle_in = fopen(argv[3], "rb"); // open file for reading
	// capture width and height values
	int img_width = argv[1];
	int img_height = argv[2];
	// allocate memory for pixel buffer
	pixmap1d = malloc(sizeof(Pixel)*img_width*img_height);	
	// begin reading shape data

	// skip any whitespace before the start of the line
	traverse_whitespace(file_handle_in);

	// close the input file before exiting
	fclose(file_handle_in);
	return 0;
}
