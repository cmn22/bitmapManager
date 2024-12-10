# Bitmap Manager

## Overview
Bitmap Manager is a C++ project designed to manipulate and process bitmap (.bmp) images. It provides a variety of functionalities to modify, analyze, and enhance images. From basic transformations like rotation and cropping to advanced operations such as histogram equalization and color channel manipulations, this tool demonstrates powerful image processing capabilities.

---

## Features
1. **Image Information**:
   - Displays the dimensions, size, and file name of the image.

2. **Image Manipulation**:
   - Save a copy of the image.
   - Change the luminosity of the image.
   - Remove specific color channels (red, green, blue).
   - Invert image colors.
   - Quantize the image to reduce color depth.
   - Flip the image horizontally.
   - Rotate the image (90°, 180°, 270°).

3. **Image Enhancement**:
   - Generate and plot a histogram to visualize pixel intensity distribution.
   - Apply histogram equalization to enhance image contrast.

4. **Image Cropping**:
   - Crop the image to predefined sizes (100x100, 200x200, etc.).

---

## How to Run the Program
1. **Compile the Program**:
   - Use `clang++` to compile the program:
     ```bash
     clang++ bitmapManager.cpp -o bitmapManager
     ```

2. **Run the Program**:
   - Execute the compiled program:
     ```bash
     ./bitmapManager
     ```

3. **Follow the On-Screen Menu**:
   - The program provides a user-friendly menu to perform various operations. Enter the number corresponding to your desired operation.

---

## Dependencies
- **C++ Compiler**: Ensure you have `clang++` installed.
- **gnuplot**:
  - Used to plot histograms.
  - Install using Homebrew (macOS):
    ```bash
    brew install gnuplot
    ```
  - Ensure gnuplot is added to your system's PATH.

---

## Menu Options
1. **Print Image Information**:
   - Displays metadata of the loaded image.
2. **Save Copy of Image**:
   - Creates a duplicate of the original image.
3. **Change Luminosity**:
   - Increases or decreases the brightness of the image by a user-defined value.
4. **Remove Color Channel**:
   - Removes the red, green, or blue channel from the image.
5. **Invert Colors**:
   - Inverts all pixel colors.
6. **Quantize Image**:
   - Reduces the number of colors in the image, creating a simplified version.
7. **Flip Horizontally**:
   - Mirrors the image horizontally.
8. **Crop Image**:
   - Crops the image to one of the predefined sizes.
9. **Generate and Plot Histogram**:
   - Calculates and visualizes the image's grayscale intensity distribution.
10. **Apply Histogram Equalization**:
    - Enhances the contrast of the image.
11. **Rotate Image**:
    - Rotates the image by 90°, 180°, or 270° clockwise.

---

## Key Algorithms
1. **Histogram Equalization**:
   - Adjusts image contrast by redistributing pixel intensity values using a cumulative distribution function (CDF).

2. **Image Rotation**:
   - Rearranges pixels to rotate the image by a specified angle.

3. **Cropping**:
   - Dynamically reallocates memory to create a smaller, cropped version of the image.

4. **Histogram Generation**:
   - Converts the image to grayscale and calculates pixel intensity frequencies.

---

## Challenges Faced
1. **Compiler Selection**:
   - Initially faced issues with compiler compatibility. Switched to `clang++` for better support on macOS.
2. **gnuplot Integration**:
   - Ensuring seamless generation and plotting of histograms using external tools.
3. **Memory Management**:
   - Managing dynamic memory for large pixel arrays and avoiding memory leaks.
4. **Bitmap File Handling**:
   - Correctly reading and writing bitmap headers and pixel data.
5. **OpenCV Setup**:
   - Tried to integrate OpenCV for advanced features like GUI-based cropping but faced configuration issues on macOS.

---

## Future Enhancements
1. **GUI Integration**:
   - Use libraries like Qt or OpenCV for a graphical user interface.
2. **Support for Additional Formats**:
   - Extend support for PNG, JPEG, and other image formats.
3. **Batch Processing**:
   - Add functionality to process multiple images simultaneously.
4. **Real-Time Editing**:
   - Enable real-time image editing using interactive tools.

---

## Example Output
### Input
- A `.bmp` image with the following properties:
  - Dimensions: 200x200 pixels
  - File Size: 120 KB

### Operations and Results
1. **Histogram Equalization**:
   - Original image: Low contrast.
   - Result: Enhanced contrast with more visible details.
2. **Rotation**:
   - Original image: Upright.
   - Result: Rotated 90° clockwise.
3. **Generate and Plot Histogram**:
   - A bar chart showing the pixel intensity distribution.

---

## Contribution
Feel free to contribute to this project. Fork the repository, make your changes, and submit a pull request. Suggestions and enhancements are always welcome!

---

## License
This project is licensed under the MIT License.

---

## Acknowledgments
Special thanks to:
- **gnuplot** for histogram plotting.
- The open-source community for supporting C++ and image processing tools.
