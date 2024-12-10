// bitmapManager.cpp : This file contains the 'main' function. Program execution begins and ends there.

// How to run the code:
// clang++ bitmapManager.cpp -o bitmapManager
// ./bitmapManager

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_NAME_SIZE 100

typedef struct {
	unsigned char red;
	unsigned char green;
	unsigned char blue;

}Pixel;

typedef struct {
	char file_name[MAX_FILE_NAME_SIZE];
	unsigned int height;
	unsigned int width;
	unsigned int size;
	Pixel** pixels;

}RGB_Image;


// Main function Prototypes
void print_information_image(void);
void save_copy_image(void);
void change_luminosity_image(void);
void remove_channel_image(void);
void invert_image(void);
void quantize_image(void);
void flip_horizontal_image(void);
void crop_image(void);
void generate_and_plot_histogram(void);
void histogram_equalization(void);
void rotate_image(void);

// ROTATE Functions
void rotate_image_90(RGB_Image* image);
void rotate_image_180(RGB_Image* image);
void rotate_image_270(RGB_Image* image);

// FILE Functions
int load_image(RGB_Image*);
int save_image(RGB_Image);

// FREE function
void free_pixels(RGB_Image);

//REALLOC FUNCTION
void re_allocate_pixels(RGB_Image*, int, int);

// PIXEL FUNCTIONS
void invert_pixels(Pixel**, int, int);
void quantize_pixels(Pixel**, int, int, int);
void remove_red_pixels(Pixel**, int, int);
void remove_green_pixels(Pixel**, int, int);
void remove_blue_pixels(Pixel**, int, int);
void change_luminosity_pixels(Pixel**, int, int, int);
void flip_horizontal_pixels(Pixel**, int, int);

void removeBmp(char*);

int main()
{
	printf("\n\n ******************* Bitmap Manager *******************\n");
	int choice = 0;
	while (choice != -1) {
		printf("\n\n");
		printf("\n\t\t MAIN MENU");
		printf("\n\t Please press enter 0-7, or -1 to Quit");
		printf("\n");
		printf("\n\t 0 - Print image information");
		printf("\n\t 1 - Save copy of image");
		printf("\n\t 2 - Change luminosity of image");
		printf("\n\t 3 - Remove image channel");
		printf("\n\t 4 - Invert image colors");
		printf("\n\t 5 - Quantize image");
		printf("\n\t 6 - Flip image horizontally");
		printf("\n\t 7 - Crop image <EXPERIMENTAL>");
		printf("\n\t 8 - Generate and Plot Histogram");
		printf("\n\t 9 - Apply Histogram Equalization");
		printf("\n\t 10 - Rotate the image");
		printf("\n\t-1 - Quit");

		printf("\n\n\t Choice >> ");
		scanf("%d", &choice);
		switch (choice)
		{
		case 0:
			print_information_image();
			break;
		case 1:
			save_copy_image();
			break;
		case 2:
			change_luminosity_image();
			break;
		case 3:
			remove_channel_image();
			break;
		case 4:
			invert_image();
			break;
		case 5:
			quantize_image();
			break;
		case 6:
			flip_horizontal_image();
			break;
		case 7:
			crop_image();
			break;
		case 8:
			generate_and_plot_histogram();  // Generate and plot histogram automatically
    		break;
		case 9:
			histogram_equalization();  // Apply histogram equalization
    		break;
		case 10:
			rotate_image();  // Ask for the rotation angle and rotate accordingly
			break;
		default:
			continue;
		}
	}
}

int load_image(RGB_Image* image_ptr) {
	printf("\n Enter file name to load (without .bmp extension): ");
	scanf("%s", image_ptr->file_name);
	strcat(image_ptr->file_name, ".bmp");

	FILE* fptr = fopen(image_ptr->file_name, "rb");
	if (NULL == fptr) {
		printf("\n File cannot be opened.");
		return 1;
	}

	fseek(fptr, 2, SEEK_SET); // Skip first 2 bytes
	fread(&(image_ptr->size), 4, 1, fptr);
	fseek(fptr, 12, SEEK_CUR); // Skip next 12 bytes
	fread(&(image_ptr->width), 4, 1, fptr);
	fread(&(image_ptr->height), 4, 1, fptr);
	fseek(fptr, 28, SEEK_CUR); // Skip next 28 bytes

	// Dynamically allocating memory for the Pixel[][] array
	image_ptr->pixels = (Pixel**)malloc((image_ptr->height) * sizeof(Pixel*));
	int i, j;
	for (i = 0; i < (image_ptr->height); ++i)
		image_ptr->pixels[i] = (Pixel*)malloc((image_ptr->width) * sizeof(Pixel));

	//Initializing pixel array to 0
	for (i = 0; i < image_ptr->height; ++i)
		for (j = 0; j < image_ptr->width; ++j) {
			image_ptr->pixels[i][j].red = 0;
			image_ptr->pixels[i][j].green = 0;
			image_ptr->pixels[i][j].blue = 0;
		}

	//Reading data into pixel array

	for (i = 0; i < image_ptr->height; ++i)
		for (j = 0; j < image_ptr->width; ++j) {
			fread(&(image_ptr->pixels[i][j].blue), 1, 1, fptr);
			fread(&(image_ptr->pixels[i][j].green), 1, 1, fptr);
			fread(&(image_ptr->pixels[i][j].red), 1, 1, fptr);
		}

	fclose(fptr);
	printf("\n Image Loaded.\n\n");

	return 0;
}

int save_image(RGB_Image image) {

	char fileCopy[MAX_FILE_NAME_SIZE];
	strcpy(fileCopy, image.file_name);
	strcat(fileCopy, ".bmp");
	FILE* fptr = fopen(fileCopy, "wb");
	if (NULL == fptr) {
		printf("\n File cannot be saved.");
		return 1;
	}

	int data_size = image.size - 54;
	unsigned char bmp_header[] = {
			0x42,0x4D,
			image.size,image.size >> 8,  image.size >> 16,image.size >> 24,
			0x00,0x00,0x00,0x00,
			0x36,0x00,0x00,0x00,
			0x28,0x00,0x00,0x00,
			image.width,image.width >> 8,  image.width >> 16,image.width >> 24,
			image.height,image.height >> 8,  image.height >> 16,image.height >> 24,
			0x01,0x00,0x18,0x00,
			0x00,0x00,0x00,0x00,
			data_size,data_size >> 8,data_size >> 16,data_size >> 24,
			0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00
	};

	fwrite(bmp_header, 1, 54, fptr);
	int i, j;
	for (i = 0; i < image.height; ++i)
		for (j = 0; j < image.width; ++j) {
			fwrite(&(image.pixels[i][j].blue), 1, 1, fptr);
			fwrite(&(image.pixels[i][j].green), 1, 1, fptr);
			fwrite(&(image.pixels[i][j].red), 1, 1, fptr);
		}

	fclose(fptr);
	printf("\n Image Saved.\n\n");
	return 0;
}

void change_luminosity_pixels(Pixel** pixels, int height, int width, int luminosity_level) {
	int i, j;
	for (i = 0; i < height; ++i) {
		for (j = 0; j < width; ++j) {
			if (pixels[i][j].red + luminosity_level > 255)
				pixels[i][j].red = 255;
			else if (pixels[i][j].red + luminosity_level < 0)
				pixels[i][j].red = 0;
			else
				pixels[i][j].red += luminosity_level;

			if (pixels[i][j].green + luminosity_level > 255)
				pixels[i][j].green = 255;
			else if (pixels[i][j].green + luminosity_level < 0)
				pixels[i][j].green = 0;
			else
				pixels[i][j].green += luminosity_level;

			if (pixels[i][j].blue + luminosity_level > 255)
				pixels[i][j].blue = 255;
			else if (pixels[i][j].blue + luminosity_level < 0)
				pixels[i][j].blue = 0;
			else
				pixels[i][j].blue += luminosity_level;
		}
	}
}

void remove_red_pixels(Pixel** pixels, int height, int width) {
	int i, j;
	for (i = 0; i < height; ++i)
		for (j = 0; j < width; ++j)
			pixels[i][j].red = 0;

}

void remove_green_pixels(Pixel** pixels, int height, int width) {
	int i, j;
	for (i = 0; i < height; ++i)
		for (j = 0; j < width; ++j)
			pixels[i][j].green = 0;
}
void remove_blue_pixels(Pixel** pixels, int height, int width) {
	int i, j;
	for (i = 0; i < height; ++i)
		for (j = 0; j < width; ++j)
			pixels[i][j].blue = 0;

}

void invert_pixels(Pixel** pixels, int height, int width) {
	int i, j;
	for (i = 0; i < height; ++i)
		for (j = 0; j < width; ++j) {
			pixels[i][j].red ^= 0xFF;
			pixels[i][j].green ^= 0xFF;
			pixels[i][j].blue ^= 0xFF;
		}
}

void quantize_pixels(Pixel** pixels, int height, int width, int quantization_level) {

	int i, j;
	unsigned int bit_mask = 0;
	for (i = 0; i < quantization_level - 1; ++i) {
		bit_mask++;
		bit_mask = bit_mask << 1;
	}
	bit_mask++;

	quantization_level = bit_mask;

	for (i = 0; i < height; ++i)
		for (j = 0; j < width; ++j) {
			pixels[i][j].red &= quantization_level;
			pixels[i][j].green &= quantization_level;
			pixels[i][j].blue &= quantization_level;
		}
}

void flip_horizontal_pixels(Pixel** pixels, int height, int width) {
	Pixel** temp = (Pixel**)malloc(height * sizeof(Pixel*));
	int i;
	for (i = 0; i < height; ++i)
		temp[i] = (Pixel*)malloc(width * sizeof(Pixel));

	int j, k;
	for (i = 0; i < height; ++i)
		for (j = 0, k = width - 1; j < width; ++j, --k)
			temp[i][j] = pixels[i][k];

	for (i = 0; i < height; ++i) {
		pixels[i] = temp[i];
		temp[i] = NULL;
	}
}

void re_allocate_pixels(RGB_Image* image_ptr, int new_height, int new_width) {
	int pre_height = image_ptr->height;
	int pre_width = image_ptr->width;

	Pixel** temp = (Pixel**)malloc(new_height * sizeof(Pixel*));
	int i, j;
	for (i = 0; i < new_height; ++i)
		temp[i] = (Pixel*)malloc(new_width * sizeof(Pixel));

	for (i = 0; i < new_height; ++i) {
		for (j = 0; j < new_width; ++j) {
			if (i >= pre_height - 1 || j >= pre_width - 1) {
				temp[i][j].red = 0;
				temp[i][j].green = 0;
				temp[i][j].blue = 0;
			}
			else
				temp[i][j] = image_ptr->pixels[i][j];
		}
	}

	image_ptr->height = new_height;
	image_ptr->width = new_width;
	image_ptr->size = (new_height * new_width) + 54;

	image_ptr->pixels = (Pixel**)realloc(image_ptr->pixels, new_height);
	for (i = 0; i < new_height; ++i)
		image_ptr->pixels[i] = (Pixel*)malloc(new_width * sizeof(Pixel));

	for (i = 0; i < new_height; ++i) {
		image_ptr->pixels[i] = temp[i];
		temp[i] = NULL;
	}


}

void crop_image()
{
	RGB_Image image;
	int failedToLoad = load_image(&image);
	removeBmp(image.file_name);
	int sizes[] = { 100,200,300,400 };

	if (!failedToLoad) {
		int choice = 0;
		printf("\n Enter the size to crop (1-4):  <EXPERIMENTAL | UNSTABLE>\n"
			"\n 1. 100x100"
			"\n 2. 200x200"
			"\n 3. 300x300"
			"\n 4. 400x400\n > ");

		scanf("%d", &choice);
		switch (choice) {
		case 1:
			re_allocate_pixels(&image, sizes[0], sizes[0]);
			strcat(image.file_name, "_cropped_100");
			printf("\n Image cropped.\n");
			save_image(image);
			break;

		case 2:
			re_allocate_pixels(&image, sizes[1], sizes[1]);
			strcat(image.file_name, "_cropped_200");
			printf("\n Image cropped.\n");
			save_image(image);
			break;

		case 3:
			re_allocate_pixels(&image, sizes[2], sizes[2]);
			strcat(image.file_name, "_cropped_300");
			printf("\n Image cropped.\n");
			save_image(image);
			break;

		case 4:
			re_allocate_pixels(&image, sizes[3], sizes[3]);
			strcat(image.file_name, "_cropped_400");
			printf("\n Image cropped.\n");
			save_image(image);
			break;
		default:
			printf("\n Invalid choice.\n");
			return;
		}
		free_pixels(image);
	}
}

void generate_and_plot_histogram() {
    RGB_Image image;
    int failedToLoad = load_image(&image);  // Load the image
    if (failedToLoad) {
        printf("\n Failed to load image.\n");
        return;
    }

    // Initialize a histogram array for grayscale values (0-255)
    int histogram[256] = {0};

    // Compute the histogram
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            // Convert pixel to grayscale using a weighted sum
            unsigned char gray_value = 0.3 * image.pixels[i][j].red + 
                                       0.59 * image.pixels[i][j].green + 
                                       0.11 * image.pixels[i][j].blue;
            histogram[gray_value]++;
        }
    }

    // Save the histogram data to a CSV file (in the background)
    FILE* fptr = fopen("histogram_data.csv", "w");
    if (fptr == NULL) {
        printf("\n Error saving histogram data.\n");
        return;
    }
    fprintf(fptr, "Intensity,Count\n");
    for (int i = 0; i < 256; ++i) {
        fprintf(fptr, "%d,%d\n", i, histogram[i]);  // Save intensity and count
    }
    fclose(fptr);
    printf("\n Histogram data saved to 'histogram_data.csv'.\n");

    // Use gnuplot to automatically plot the histogram
    system("gnuplot -e \"set datafile separator ','; set title 'Image Histogram'; "
           "set xlabel 'Pixel Intensity'; set ylabel 'Pixel Count'; "
           "set style fill solid; plot 'histogram_data.csv' using 1:2 with boxes notitle; pause -1\"");
    printf("\n Histogram plotted using gnuplot.\n");

    // Clean up
    free_pixels(image);
}

void histogram_equalization() {
    RGB_Image image;
    int failedToLoad = load_image(&image);  // Load the image

    if (failedToLoad) {
        printf("\n Failed to load image.\n");
        return;
    }

    int histogram[256] = {0};  // Histogram for grayscale intensities
    int cdf[256] = {0};        // Cumulative distribution function (CDF)
    int total_pixels = image.width * image.height;

    // Step 1: Convert image to grayscale and compute the histogram
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            // Convert pixel to grayscale using a weighted sum
            unsigned char gray_value = 0.3 * image.pixels[i][j].red + 
                                       0.59 * image.pixels[i][j].green + 
                                       0.11 * image.pixels[i][j].blue;
            histogram[gray_value]++;
        }
    }

    // Step 2: Compute the CDF from the histogram
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; ++i) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }

    // Step 3: Normalize the CDF and map pixel intensities
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            unsigned char gray_value = 0.3 * image.pixels[i][j].red + 
                                       0.59 * image.pixels[i][j].green + 
                                       0.11 * image.pixels[i][j].blue;
            // Map the grayscale value to the new intensity based on the CDF
            unsigned char new_value = ((float)(cdf[gray_value] - cdf[0]) / (total_pixels - cdf[0])) * 255;

            // Apply the new intensity value to all channels (equalized grayscale)
            image.pixels[i][j].red = new_value;
            image.pixels[i][j].green = new_value;
            image.pixels[i][j].blue = new_value;
        }
    }

    // Save the result and free memory
    strcat(image.file_name, "_histogram_equalized");
    save_image(image);  // Save the modified image
    free_pixels(image);
    printf("\n Histogram equalization applied and image saved.\n");
}

void rotate_image_90(RGB_Image* image) {
    int new_height = image->width;
    int new_width = image->height;

    // Allocate memory for the rotated image
    Pixel** rotated_pixels = (Pixel**)malloc(new_height * sizeof(Pixel*));
    for (int i = 0; i < new_height; ++i) {
        rotated_pixels[i] = (Pixel*)malloc(new_width * sizeof(Pixel));
    }

    // Rotate by 90 degrees clockwise
    for (int i = 0; i < image->height; ++i) {
        for (int j = 0; j < image->width; ++j) {
            rotated_pixels[j][image->height - 1 - i] = image->pixels[i][j];
        }
    }

    // Replace original pixel data with rotated data
    free_pixels(*image);  // Free original pixels
    image->pixels = rotated_pixels;
    image->width = new_width;
    image->height = new_height;
    printf("\n Image rotated by 90 degrees.\n");
}

void rotate_image_180(RGB_Image* image) {
    // Rotate by 180 degrees
    for (int i = 0; i < image->height / 2; ++i) {
        for (int j = 0; j < image->width; ++j) {
            Pixel temp = image->pixels[i][j];
            image->pixels[i][j] = image->pixels[image->height - 1 - i][image->width - 1 - j];
            image->pixels[image->height - 1 - i][image->width - 1 - j] = temp;
        }
    }
    printf("\n Image rotated by 180 degrees.\n");
}

void rotate_image_270(RGB_Image* image) {
    int new_height = image->width;
    int new_width = image->height;

    // Allocate memory for the rotated image
    Pixel** rotated_pixels = (Pixel**)malloc(new_height * sizeof(Pixel*));
    for (int i = 0; i < new_height; ++i) {
        rotated_pixels[i] = (Pixel*)malloc(new_width * sizeof(Pixel));
    }

    // Rotate by 270 degrees clockwise (which is -90 degrees)
    for (int i = 0; i < image->height; ++i) {
        for (int j = 0; j < image->width; ++j) {
            rotated_pixels[image->width - 1 - j][i] = image->pixels[i][j];
        }
    }

    // Replace original pixel data with rotated data
    free_pixels(*image);  // Free original pixels
    image->pixels = rotated_pixels;
    image->width = new_width;
    image->height = new_height;
    printf("\n Image rotated by 270 degrees.\n");
}

void rotate_image() {
    RGB_Image image;
    int failedToLoad = load_image(&image);  // Load the image
    if (failedToLoad) {
        printf("\n Failed to load image.\n");
        return;
    }

    int choice = 0;
    printf("\n Choose the rotation angle:\n"
           "\n 1. Rotate 90 degrees"
           "\n 2. Rotate 180 degrees"
           "\n 3. Rotate 270 degrees\n > ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            rotate_image_90(&image);
            strcat(image.file_name, "_rotated_90");
            break;
        case 2:
            rotate_image_180(&image);
            strcat(image.file_name, "_rotated_180");
            break;
        case 3:
            rotate_image_270(&image);
            strcat(image.file_name, "_rotated_270");
            break;
        default:
            printf("\n Invalid choice.\n");
            free_pixels(image);  // Clean up memory
            return;
    }

    save_image(image);  // Save the modified image
    free_pixels(image); // Free the pixel memory
    printf("\n Image rotation saved.\n");
}

void flip_horizontal_image()
{
	RGB_Image image;
	int failedToLoad = load_image(&image);
	removeBmp(image.file_name);

	if (!failedToLoad) {
		flip_horizontal_pixels(image.pixels, image.height, image.width);
		strcat(image.file_name, "_flipped_horizontally");
		printf("\n Image flipped horizontally.\n\n");
		save_image(image);
		free_pixels(image);
	}
}

void quantize_image()
{
	RGB_Image image;
	int failedToLoad = load_image(&image);
	removeBmp(image.file_name);

	int quantization_level = -1;
	while (quantization_level < 0 || quantization_level > 7)
	{
		printf("\n Enter the quantization level (0-7): ");
		scanf("%d", &quantization_level);
	}

	if (!failedToLoad) {
		quantize_pixels(image.pixels, image.height, image.width, quantization_level);

		char temp[30];
		sprintf(temp, "_quantize_%d", quantization_level);
		strcat(image.file_name, temp);
		printf("\n Image quantized by a level of %d\n", quantization_level);
		save_image(image);
		free_pixels(image);
	}
}



void invert_image() {
	RGB_Image image;
	int failedToLoad = load_image(&image);
	removeBmp(image.file_name);

	if (!failedToLoad) {
		invert_pixels(image.pixels, image.height, image.width);
		strcat(image.file_name, "_inverted");
		printf("\n Image inverted.\n\n");
		save_image(image);
		free_pixels(image);
	}
}


void remove_channel_image() {
	RGB_Image image;
	int failedToLoad = load_image(&image);
	removeBmp(image.file_name);

	if (!failedToLoad) {
		int choice = 0;
		printf("\n Choose which channel to remove (1-3):\n"
			"\n 1. Red"
			"\n 2. Green"
			"\n 3. Blue\n > ");

		scanf("%d", &choice);
		switch (choice) {
		case 1:
			remove_red_pixels(image.pixels, image.height, image.width);
			strcat(image.file_name, "_red_channel_removed");
			printf("\n Red channel removed.\n");
			save_image(image);
			break;

		case 2:
			remove_green_pixels(image.pixels, image.height, image.width);
			strcat(image.file_name, "_green_channel_removed");
			printf("\n Green channel removed.\n");
			save_image(image);
			break;

		case 3:
			remove_blue_pixels(image.pixels, image.height, image.width);
			strcat(image.file_name, "_blue_channel_removed");
			printf("\n Blue channel removed.\n");
			save_image(image);
			break;
		default:
			printf("\n Invalid choice.\n");
			return;
		}
		free_pixels(image);
	}

}

void change_luminosity_image()
{
	RGB_Image image;
	int failedToLoad = load_image(&image);
	removeBmp(image.file_name);
	int luminosity_level = 0;
	printf("\n Enter the luminosity level: ");
	scanf("%d", &luminosity_level);

	if (!failedToLoad) {
		change_luminosity_pixels(image.pixels, image.height, image.width, luminosity_level);

		char temp[30];
		sprintf(temp, "_luminosity_%d", luminosity_level);
		strcat(image.file_name, temp);
		printf("\n Image luminosity changed by a level of %d\n", luminosity_level);
		save_image(image);
		free_pixels(image);
	}
}

void save_copy_image()
{
	RGB_Image image;
	int failedToLoad = load_image(&image);
	removeBmp(image.file_name);

	if (!failedToLoad) {
		strcat(image.file_name, "_copy");
		printf("\n Image Copied.\n\n");
		save_image(image);
		free_pixels(image);
	}
}

void print_information_image()
{
	RGB_Image image;
	int failedToLoad = load_image(&image);
	if (!failedToLoad) {
		printf("\n File name: %s"
			"\n Image height: %d"
			"\n Image Width: %d"
			"\n Image size: %lu",
			image.file_name, image.height, image.width, image.size);
	}

	free_pixels(image);
}

void free_pixels(RGB_Image image_ptr) {
	int i;
	for (i = 0; i < image_ptr.height; ++i)
		free(image_ptr.pixels[i]);
}

void removeBmp(char* withBmp) {
	withBmp[strlen(withBmp) - 4] = '\0';
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
