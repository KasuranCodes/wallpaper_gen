/* +----------------------------+
 * | Title:	    BitMapGen	    |
 * | Name: 	    Max McGill	    |
 * | Date:	    02.04.25	    |
 * +----------------------------+
 */

#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "bitmaps.h"
#include "image_to_ascii.h"

/* Colours */
#define BLACK           0
#define RED             1
#define GREEN           2
#define YELLOW          3
#define BLUE            4
#define MAGENTA         5
#define CYAN            6
#define WHITE           7
#define BRIGHTBLACK     60
#define BRIGHTRED       61
#define BRIGHTGREEN     62
#define BRIGHTYELLOW    63
#define BRIGHTBLUE      64
#define BRIGHTMAGENTA   65
#define BRIGHTCYAN      66
#define BRIGHTWHITE     67

/* Definitions */
#define IMAGEHEIGHT 38
#define IMAGEWIDTH 63


/* Global Assignables */
int height;
int width;


/* Function Prototypes */
void clear();
void setBack(int bg);
void setColr(int fg, int bg);
void setCsr(int x, int y);
void saveCsr();
void restoreCsr();
int colrGrb(const char* colr);


int main(int argc, char *argv[])
{

	struct winsize w;	
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	height = w.ws_row + 1;
	width = w.ws_col + 1;

	
	int xImage = width / 4 - (int)(IMAGEWIDTH / 2);
	int yImage = height / 2 - (int)(IMAGEHEIGHT / 2);
	int xImage2 = (width / 4 * 3) - (int)(IMAGEWIDTH / 2);
	int yImage2 = height / 2 - (int)(IMAGEHEIGHT / 2);
	
	int bk = BLACK;

	int fg = WHITE;
	int bg = BLACK;
	int fg2 = WHITE;
	int bg2 = BLACK;

	const char* input_file = "images/greenit.png";
	const char* input_file2 = "images/greenit.png";

	if (argc < 3)
	{
		printf("Error: No commands passed\n");
		printf("ix\t| int | Image x Coordinate\n");
		printf("iy\t| int | Image y Coordinate\n");
		printf("bk\t| str | Background Colour\n");
		printf("fg\t| str | Text Colour\n");
		printf("bg\t| str | Text Background Colour\n");
		printf("img\t| str | Image Path\n");
		printf("ix2\t| int | Image x Coordinate\n");
		printf("iy2\t| int | Image y Coordinate\n");
		printf("bk2\t| str | Background Colour\n");
		printf("fg2\t| str | Text Colour\n");
		printf("bg2\t| str | Text Background Colour\n");
		printf("img2\t| str | Image Path\n");
		return -1;
	}
	else
	{
		printf("argc = %d\n", argc);
	}

	for (int i = 1; i < argc; i += 2)
	{
		printf("i.%d: '%s' -> '%s'\n", i,  argv[i], argv[i + 1]);
			
		if (i == argc)
		{
			printf("Error: %s has no value\n", argv[i]);
			return -1;
		}
		else if (strcmp(argv[i], "ix") == 0)
		{	
			xImage = atoi(argv[i + 1]);
		}
		else if (strcmp(argv[i],"iy") == 0)
		{
			yImage = atoi(argv[i + 1]);
		}
		else if (strcmp(argv[i], "ix2") == 0)
		{	
			xImage2 = atoi(argv[i + 1]);
		}
		else if (strcmp(argv[i],"iy2") == 0)
		{
			yImage2 = atoi(argv[i + 1]);
		}
		else if (strcmp(argv[i], "bk") == 0)
		{
			bk = colrGrb(argv[i + 1]);
		}
		else if (strcmp(argv[i], "fg") == 0)
		{
			fg = colrGrb(argv[i + 1]);
		}
		else if (strcmp(argv[i], "bg") == 0)
		{
			bg = colrGrb(argv[i + 1]);
		}
		else if (strcmp(argv[i], "fg2") == 0)
		{
			fg2 = colrGrb(argv[i + 1]);
		}
		else if (strcmp(argv[i], "bg2") == 0)
		{
			bg2 = colrGrb(argv[i + 1]);
		}
		else if (strcmp(argv[i], "img") == 0)
		{
			input_file = argv[i + 1];
		}
		else if (strcmp(argv[i], "img2") == 0)
		{
			input_file2 = argv[i + 1];
		}


	}
	


	setBack(bk);



    // Load the image
    Image* img = load_image(input_file);
    Image* img2 = load_image(input_file2);
    if (!img) 
	{
        fprintf(stderr, "Failed to load image: %s\n", input_file);
        return 1;
    }
    
	img = resize_image(img, IMAGEWIDTH, IMAGEHEIGHT);
	img2 = resize_image(img2, IMAGEWIDTH, IMAGEHEIGHT);

    // Convert image to ASCII
    char* ascii_art = image_to_ascii(img, img->width, img->height);
    char* ascii_art2 = image_to_ascii(img2, img2->width, img2->height);
    if (!ascii_art) 
	{
        fprintf(stderr, "Failed to convert image to ASCII\n");
        free_image(img);
        free_image(img);
        return 1;
    }
	
	
	// Output to file
	FILE* file = fopen("output.txt", "w");
	FILE* file2 = fopen("output2.txt", "w");
	if (!file) 
	{
		fprintf(stderr, "Failed to open output file: %s\n", "output");
		free(ascii_art);
		free_image(img);
		free(ascii_art2);
		free_image(img2);
		return 1;
	}

	fprintf(file, "%s", ascii_art);
	fprintf(file2, "%s", ascii_art2);
	fclose(file);
	fclose(file2);



	file = fopen("output.txt", "r");
	file2 = fopen("output2.txt", "r");
	
	char line[256];
	int i = 0;
	setColr(fg, bg);
    while (fgets(line, sizeof(line), file)) {
		setCsr(xImage, yImage + i);
        printf("%s", line); 
		i++;
    }	
	char line2[256];
	i = 0;
	setColr(fg2, bg2);
    while (fgets(line2, sizeof(line2), file2)) {
		setCsr(xImage2, yImage2 + i);
        printf("%s", line2); 
		i++;
    }	


	setCsr(width, height);
	putchar(' ');
	
	fclose(file);
	fclose(file2);
	
	getchar();
	return 0;
}

void clear()
{
	printf("\e[1;1H\e[2J");
}

void setBack(int bg)
{
	saveCsr();
	clear();
	setColr(bg, bg);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			setCsr(x, y);
			printf(" ");
		}
	}
	restoreCsr();
}

void setColr(int fg, int bg)
{
	printf("\033[%i;%im", fg + 30, bg + 40);
}	

void setCsr(int x, int y)
{
	printf("\x1b[%i;%iH", y, x);
}

void saveCsr()
{
	/* Save Cursor */
	printf("\x1b[s");
}

void restoreCsr()
{
	/* Return Cursor */
	printf("\x1b[u");
}

int colrGrb(const char* colr)
{
    if (strcmp(colr, "black") == 0)
    {
        return BLACK;
    }
    if (strcmp(colr, "red") == 0)
    {
        return RED;
    }
    if (strcmp(colr, "green") == 0)
    {
        return GREEN;
    }
    if (strcmp(colr, "yellow") == 0)
    {
        return YELLOW;
    }
    if (strcmp(colr, "blue") == 0)
    {
        return BLUE;
    }
    if (strcmp(colr, "magenta") == 0)
    {
        return MAGENTA;
    }
    if (strcmp(colr, "cyan") == 0)
    {
        return CYAN;
    }
    if (strcmp(colr, "white") == 0)
    {
        return WHITE;
    }
    if (strcmp(colr, "brightblack") == 0)
    {
        return BRIGHTBLACK;
    }
    if (strcmp(colr, "brightred") == 0)
    {
        return BRIGHTRED;
    }
    if (strcmp(colr, "brightgreen") == 0)
    {
        return BRIGHTGREEN;
    }
    if (strcmp(colr, "brightyellow") == 0)
    {
        return BRIGHTYELLOW;
    }
    if (strcmp(colr, "brightblue") == 0)
    {
        return BRIGHTBLUE;
    }
    if (strcmp(colr, "brightmagenta") == 0)
    {
        return BRIGHTMAGENTA;
    }
    if (strcmp(colr, "brightcyan") == 0)
    {
        return BRIGHTCYAN;
    }
    if (strcmp(colr, "brightwhite") == 0)
    {
        return BRIGHTWHITE;
    }
    
    // Default return value if no color matches
    return BRIGHTMAGENTA;
}
