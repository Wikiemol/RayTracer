#ifndef COLORBUFFER_HPP
#define COLORBUFFER_HPP

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
class ColorBuffer {
public:
    ColorBuffer(unsigned w, unsigned h);
    ColorBuffer(unsigned w, unsigned h, unsigned *bgColor);
    ~ColorBuffer();
    void clearBuffer();
    unsigned getHeight();
    unsigned getWidth();
    unsigned *getColorAt(unsigned a, unsigned b);
    unsigned *getBackgroundColor();
    unsigned *getStrokeColor();
    void setBackgroundColor(unsigned r, unsigned g, unsigned b);
    void setColorAt(unsigned a, unsigned b);
    void setStrokeColor(unsigned r, unsigned g, unsigned b);
    string writeToFile(string fileName, string fileExtension);
private:
    unsigned width;
    unsigned height;
    unsigned **colorBuffer;
    unsigned *backgroundColor;
    unsigned *strokeColor;
};

/* w will be the width of the image created by the colorBuffer
 * h will be the height of the image created by the colorBuffer
 * The actual colorBuffer array will have width w * h and
 * height 3 (representing r g and b values respectively).
 */
ColorBuffer::ColorBuffer(unsigned w, unsigned h) {
    width = w;
    height = h;
    colorBuffer = new unsigned*[width * height];
    backgroundColor = new unsigned[3];
    backgroundColor[0] = 0;
    backgroundColor[1] = 0;
    backgroundColor[2] = 0;

    strokeColor = new unsigned[3];
    strokeColor[0] = 255;
    strokeColor[1] = 255;
    strokeColor[2] = 255;

    clearBuffer();
}

/* bgColor will be the rgb value that the buffer will be cleared to */
ColorBuffer::ColorBuffer(unsigned w, unsigned h, unsigned *bgColor) {
    width = w;
    height = h;
    colorBuffer = new unsigned*[width * height];
    backgroundColor = new unsigned[3];
    backgroundColor[0] = bgColor[0];
    backgroundColor[1] = bgColor[1];
    backgroundColor[2] = bgColor[2];
    clearBuffer();
}

/* cleans up the pointers used in the class */
ColorBuffer::~ColorBuffer() {
    for (unsigned i = 0; i < width * height; i++) {
        delete colorBuffer[i];
        colorBuffer[i] = NULL;
    }

    delete colorBuffer;
    colorBuffer = NULL;

    delete backgroundColor;
    backgroundColor = NULL;

    delete strokeColor;
    strokeColor = NULL;
}

/* sets all rgb values in the color buffer to
 * the background color.
 */
void ColorBuffer::clearBuffer() {
    for (unsigned i = 0; i < width * height; i++) {
        colorBuffer[i] = new unsigned[3];
        colorBuffer[i][0] = backgroundColor[0];
        colorBuffer[i][1] = backgroundColor[1];
        colorBuffer[i][2] = backgroundColor[2];
    }
}

/* returns the height of the image created
 * by the colorbuffer.
 */
unsigned ColorBuffer::getHeight() {
    return height;
}

/* returns the width of the image created
 * by the colorBuffer.
 */
unsigned ColorBuffer::getWidth() {
    return width;
}

/* returns the color at a certain index */
unsigned *ColorBuffer::getColorAt(unsigned column, unsigned row) {
    if (column >= width || row >= height) {
        ostringstream error;
        error << "ColorBuffer index (" << row << ", " << column << ") is out of bounds.\n";
        throw out_of_range(error.str());
    }

    return colorBuffer[row*width + column];
}

unsigned *ColorBuffer::getBackgroundColor() {
    return backgroundColor;
}

unsigned *ColorBuffer::getStrokeColor() {
    return strokeColor;
}

/* Sets the background color */
void ColorBuffer::setBackgroundColor(unsigned r, unsigned g, unsigned b) {
    backgroundColor[0] = r;
    backgroundColor[1] = g;
    backgroundColor[2] = b;
}

/* Sets the color buffer at the given index to the stroke color */
void ColorBuffer::setColorAt(unsigned column, unsigned row) {
    if (column >= width || row >= height) {
        ostringstream error;
        error << "ColorBuffer index (" << column << ", " << row << ") is out of bounds.\n";
        throw out_of_range(error.str());
    }

    colorBuffer[row*width + column][0] = strokeColor[0];
    colorBuffer[row*width + column][1] = strokeColor[1];
    colorBuffer[row*width + column][2] = strokeColor[2];

}

/* Sets the stroke color */
void ColorBuffer::setStrokeColor(unsigned r, unsigned g, unsigned b) {
    strokeColor[0] = r;
    strokeColor[1] = g;
    strokeColor[2] = b;
}

/* writes the colorBuffer to an imageFile. Currently, fileExtension must
 * be ".ppm".
 */
string ColorBuffer::writeToFile(string fileName, string fileExtension) {
    ofstream outputFile(fileName + fileExtension);

    if (!outputFile.is_open()) {
        return NULL;
    }

    outputFile << "P3\n";
    outputFile << width << " " << height << " " << 255 << "\n";
    for (int i = 0; i < width * height; i++) {
        for (int j = 0; j < 3; j++) {
            outputFile << colorBuffer[i][j] << " ";
        }
    }
    outputFile << "\n";
    outputFile.close();
    return fileName;
}

#endif