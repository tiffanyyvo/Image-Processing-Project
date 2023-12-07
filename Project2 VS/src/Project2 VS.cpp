#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct Header {
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;

};

struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

class Image {
public:
        Header header;
        vector<Pixel> pixelVec;
        void read(const string& inName);
        void write(const string& outName);

        Image();
private:
    string name;
    ifstream stream;
};
Image::Image() {
};

void Image::read(const string& inName) {
    //(14) Read in the .tga file
    this->stream = ifstream(inName, ios_base::binary);

    //(15) Check if the file is opening
    if (!stream.is_open()) {
        throw runtime_error("could not open file");
    }

    //(16) Read in each element of the .tga file
    stream.read(&header.idLength, sizeof(header.idLength));
    stream.read(&header.colorMapType, sizeof(header.colorMapType));
    stream.read(&header.dataTypeCode, sizeof(header.dataTypeCode));
    stream.read((char*)&header.colorMapOrigin, sizeof(header.colorMapOrigin));
    stream.read((char*)&header.colorMapLength, sizeof(header.colorMapLength));
    stream.read(&header.colorMapDepth, sizeof(header.colorMapDepth));
    stream.read((char*)&header.xOrigin, sizeof(header.xOrigin));
    stream.read((char*)&header.yOrigin, sizeof(header.yOrigin));
    stream.read((char*)&header.width, sizeof(header.width));
    stream.read((char*)&header.height, sizeof(header.height));
    stream.read(&header.bitsPerPixel, sizeof(header.bitsPerPixel));
    stream.read(&header.imageDescriptor, sizeof(header.imageDescriptor));

    //(17) Get the image size by multiplying the image height and width
    int imageSize = (int)(header.width * header.height);

    //(18) Create a for loop to read in all the pixels
    for (int i = 0; i < imageSize; i++) {
        //(21) Push in each pixel to the pixel vector
        Pixel pixel;

        stream.read((char*)&pixel.blue, sizeof(pixel.blue));
        stream.read((char*)&pixel.green, sizeof(pixel.green));
        stream.read((char*)&pixel.red, sizeof(pixel.red));

        pixelVec.push_back(pixel);
    }
}

void Image::write(const string& outName) {
    //(22) Create a .tga file
    ofstream stream(outName, ios_base::binary);

    //(23) Write out each .tga file header element
    stream.write(&header.idLength, sizeof(header.idLength));
    stream.write(&header.colorMapType, sizeof(header.colorMapType));
    stream.write(&header.dataTypeCode, sizeof(header.dataTypeCode));
    stream.write((char*)&header.colorMapOrigin, sizeof(header.colorMapOrigin));
    stream.write((char*)&header.colorMapLength, sizeof(header.colorMapLength));
    stream.write(&header.colorMapDepth, sizeof(header.colorMapDepth));
    stream.write((char*)&header.xOrigin, sizeof(header.xOrigin));
    stream.write((char*)&header.yOrigin, sizeof(header.yOrigin));
    stream.write((char*)&header.width, sizeof(header.width));
    stream.write((char*)&header.height, sizeof(header.height));
    stream.write(&header.bitsPerPixel, sizeof(header.bitsPerPixel));
    stream.write(&header.imageDescriptor, sizeof(header.imageDescriptor));

    //(24) Create a for loop to write out each pixel
    for (unsigned int i = 0; i < pixelVec.size(); i++) {

        //(25) Write out each image pixel
        Pixel pixel = pixelVec[i];

        stream.write((char*)&pixel.blue, sizeof(pixel.blue));
        stream.write((char*)&pixel.green, sizeof(pixel.green));
        stream.write((char*)&pixel.red, sizeof(pixel.red));
    }
}
// Helpers

// Multiply
unsigned char multiply(unsigned char value1, unsigned char value2) {
    float newValue1 = float(value1) / 255;
    float newValue2 = float(value2) / 255;

    float product = newValue1 * newValue2;
    product = product * 255;
    product = product + 0.5f;
    product = int(product);

    return (unsigned char)product;
}

// Subtract
unsigned char subtract(unsigned char value1, unsigned char value2) {
    int newValue = int(value1) - int(value2);

    if (newValue > 255) {
        newValue = 255;
    }
    else if (newValue < 0) {
        newValue = 0;
    }

    return (unsigned char)newValue;
}

// Screen
unsigned char screen(unsigned char value1, unsigned char value2) {

    float newValue1 = float(value1 / 255.0f);
    float newValue2 = float(value2 / 255.0f);

    newValue1 = 1 - newValue1;
    newValue2 = 1 - newValue2;

    float product = newValue1 * newValue2;
    //product = product * 255;
    //product = product + 0.5f;

    float sum = 1 - product;
    sum = sum * 255;
    sum = sum + 0.5f;

    return (unsigned char)sum;

}

// Overlay
unsigned char overlay(unsigned char value1, unsigned char value2) {
    float overlay;
    float newValue1 = float(value1 / 255.0f);
    float newValue2 = float(value2 / 255.0f);

    if (newValue2 <= 0.5) {
        overlay = 2.0f * newValue1 * newValue2;
    }
    else if (newValue2 > 0.5) {
        newValue1 = 1 - newValue1;
        newValue2 = 1 - newValue2;

        overlay = 2 * newValue1 * newValue2;
        overlay = 1 - overlay;
    }
    overlay = overlay * 255;
    overlay = overlay + 0.5f;
    return (unsigned char)overlay;
}

// Addition to add a specific integer
unsigned char addition(unsigned char value1, int value2) {
    int newValue = int(value1) + int(value2);

    if (newValue > 255) {
        newValue = 255;
    }
    else if (newValue < 0) {
        newValue = 0;
    }

    return (unsigned char)newValue;
}

// Multiply by a specific integer
unsigned char scaleMultiply(unsigned char value1, int value2) {
    //float newValue1 = float(value1) / 255;
    //float newValue2 = float(value2) / 255;

    float product = float(value1) * float(value2);
    //product = product * 255;
    product = product + 0.5f;
    product = int(product);
    if (product > 255) {
        product = 255;
    }

    return (unsigned char)product;
}

//Task Functions

// Task 1 
void task1(string file1, string file2) {
    Image image1;
    Image image2;
    

    image1.read(file1);
    image2.read(file2);

    for (int i = 0; i < image1.pixelVec.size(); i++) {
        image1.pixelVec[i].blue = multiply(image1.pixelVec[i].blue, image2.pixelVec[i].blue);
        image1.pixelVec[i].green = multiply(image1.pixelVec[i].green, image2.pixelVec[i].green);
        image1.pixelVec[i].red = multiply(image1.pixelVec[i].red, image2.pixelVec[i].red);
    }
    image1.write("output/part1.tga");
}

// Task 2
void task2(string file1, string file2) {
    Image image1;
    Image image2;

    image1.read(file1);
    image2.read(file2);

    for (int i = 0; i < image1.pixelVec.size(); i++) {
        image1.pixelVec[i].blue = subtract(image1.pixelVec[i].blue, image2.pixelVec[i].blue);
        image1.pixelVec[i].green = subtract(image1.pixelVec[i].green, image2.pixelVec[i].green);
        image1.pixelVec[i].red = subtract(image1.pixelVec[i].red, image2.pixelVec[i].red);
    }
    image1.write("output/part2.tga");
}

// Task 3
Pixel* task3(string file1, string file2, string file3) {
    Image image1;
    Image image2;
    Image image3;

    image1.read(file1);
    image2.read(file2);
    image3.read(file3);

    int size = image1.pixelVec.size();

    Pixel* arr = new Pixel[size];

    for (int i = 0; i < image1.pixelVec.size(); i++) {
        image1.pixelVec[i].blue = multiply(image1.pixelVec[i].blue, image2.pixelVec[i].blue);
        image1.pixelVec[i].green = multiply(image1.pixelVec[i].green, image2.pixelVec[i].green);
        image1.pixelVec[i].red = multiply(image1.pixelVec[i].red, image2.pixelVec[i].red);

       /* Pixel newPixel;
        newPixel.blue = image1.pixelVec[i].blue;
        newPixel.green = image1.pixelVec[i].green;
        newPixel.red = image1.pixelVec[i].red;
        */

        arr[i].blue = image1.pixelVec[i].blue;
        arr[i].green = image1.pixelVec[i].green;
        arr[i].red = image1.pixelVec[i].red;

        image1.pixelVec[i].blue = screen(image3.pixelVec[i].blue, arr[i].blue);
        image1.pixelVec[i].green = screen(image3.pixelVec[i].green, arr[i].green);
        image1.pixelVec[i].red = screen(image3.pixelVec[i].red, arr[i].red);
    }
    image1.write("output/part3.tga");
    //delete[] arr;
    return arr;

}

// Task 4
void task4(string file1, string file2, string file3) {
    Image image1;
    Image image2;
    Image image3;

    image1.read(file1);
    image2.read(file2);
    image3.read(file3);

    for (int i = 0; i < image1.pixelVec.size(); i++) {
        image1.pixelVec[i].blue = multiply(image1.pixelVec[i].blue, image2.pixelVec[i].blue);
        image1.pixelVec[i].green = multiply(image1.pixelVec[i].green, image2.pixelVec[i].green);
        image1.pixelVec[i].red = multiply(image1.pixelVec[i].red, image2.pixelVec[i].red);

        image1.pixelVec[i].blue = subtract(image1.pixelVec[i].blue, image3.pixelVec[i].blue);
        image1.pixelVec[i].green = subtract(image1.pixelVec[i].green, image3.pixelVec[i].green);
        image1.pixelVec[i].red = subtract(image1.pixelVec[i].red, image3.pixelVec[i].red);
    }
    image1.write("output/part4.tga");
}

// Task 5
void task5(string file1, string file2) {
    Image image1;
    Image image2;

    image1.read(file1);
    image2.read(file2);


    for (int i = 0; i < image1.pixelVec.size(); i++) {
        image1.pixelVec[i].blue = overlay(image1.pixelVec[i].blue, image2.pixelVec[i].blue);
        image1.pixelVec[i].green = overlay(image1.pixelVec[i].green, image2.pixelVec[i].green);
        image1.pixelVec[i].red = overlay(image1.pixelVec[i].red, image2.pixelVec[i].red);
    }
    image1.write("output/part5.tga");
}

// Task 6
void task6(string file1) {
    Image image1;

    image1.read(file1);
    for (int i = 0; i < image1.pixelVec.size(); i++) {
        image1.pixelVec[i].green = addition(image1.pixelVec[i].green, 200);
    }
    image1.write("output/part6.tga");
}

// Task 7
void task7(string file1) {
    Image image1;
    image1.read(file1);

    for (int i = 0; i < image1.pixelVec.size(); i++) {
        image1.pixelVec[i].red = scaleMultiply(image1.pixelVec[i].red, 4);
        image1.pixelVec[i].blue = scaleMultiply(image1.pixelVec[i].blue, 0);
    }
    image1.write("output/part7.tga");
}

// Task 8
void task8(string file1) {
    Image image1;
    Image image2;
    Image image3;

    image1.read(file1);
    for (int i = 0; i < image1.pixelVec.size(); i++) {
        image1.pixelVec[i].red = image1.pixelVec[i].red;
        image1.pixelVec[i].blue = image1.pixelVec[i].red;
        image1.pixelVec[i].green = image1.pixelVec[i].red;
    }
    image1.write("output/part8_r.tga");

    image2.read(file1);
    for (int i = 0; i < image1.pixelVec.size(); i++) {
        image2.pixelVec[i].red = image2.pixelVec[i].green;
        image2.pixelVec[i].blue = image2.pixelVec[i].green;
        image2.pixelVec[i].green = image2.pixelVec[i].green;
    }

    image2.write("output/part8_g.tga");
    image3.read(file1);
    for (int i = 0; i < image1.pixelVec.size(); i++) {
        image3.pixelVec[i].red = image3.pixelVec[i].blue;
        image3.pixelVec[i].blue = image3.pixelVec[i].blue;
        image3.pixelVec[i].green = image3.pixelVec[i].blue;
    }
    image3.write("output/part8_b.tga");
}

// Task 9
void task9(string file1, string file2, string file3) {
    Image image1;
    Image image2;
    Image image3;

    image1.read(file1);
    image2.read(file2);
    image3.read(file3);

    for (int i = 0; i < image1.pixelVec.size(); i++) {
        image1.pixelVec[i].blue = image3.pixelVec[i].blue;
        image1.pixelVec[i].green = image2.pixelVec[i].green;
        image1.pixelVec[i].red = image1.pixelVec[i].red;
    }
    image1.write("output/part9.tga");
}

// Task 10
void task10(string file1) {
    Image image1;
    Image image2;

    image2.read(file1);
    image1.read(file1);

    int count = 0;

    for (unsigned int i = image1.pixelVec.size()-1; i>0; i--) {
        image2.pixelVec[count].blue = image1.pixelVec[i].blue;
        image2.pixelVec[count].green = image1.pixelVec[i].green;
        image2.pixelVec[count].red = image1.pixelVec[i].red;
        count++;
    }
    image2.write("output/part10.tga");
}

int main(int argc, char* argv[]) {

    //Part 1
    /*
    //Image image;

    // Task 1
    task1("input/layer1.tga", "input/pattern1.tga");

    // Task 2
    task2("input/car.tga", "input/layer2.tga");

    // Task 3
    task3("input/layer1.tga", "input/pattern2.tga", "input/text.tga");

    // Task 4
    task4("input/layer2.tga", "input/circles.tga", "input/pattern2.tga");

    // Task 5
    task5("input/layer1.tga", "input/pattern1.tga");

    // Task 6
    task6("input/car.tga");

    // Task 7
    task7("input/car.tga");

    // Task 8
    task8("input/car.tga");

    // Task 9
    task9("input/layer_red.tga", "input/layer_green.tga", "input/layer_blue.tga");

    //Task 10
    task10("input/text2.tga");
    */



    // Part 2

    //print tests
    cout << "argc: " << argc << endl;

    // Check help message
    if (strcmp(argv[1], "help") == 0) {
        cout << "You didn't pass any arguments! \n";
        return 0;
    }

    // Print Test
    cout << "argv[0]: " << argv[0] << endl;
    int argIndex = 1;

    try {
        while (argIndex < argc) {

            // Print test each Argument
            cout << "arg[" << argIndex << "]: " << argv[argIndex] << endl;

            // If user entered multiply
            if (strcmp(argv[argIndex], "multiply") == 0) {

                argIndex++;

                if (argIndex >= argc) {

                    // Throw an invalid argument is argIndex is greater than argc
                    //throw invalid_argument("Not enough argument for multiply()");
                    throw invalid_argument("Missing argument.");
                }
                //Print argv[argIndex] again
                cout << "argv[" << argIndex << "]: " << argv[argIndex] << endl;

                //Set num1 equal to argv[argIndex], make sure to convert the string to int
                int num1 = stoi(argv[argIndex]);
                argIndex++;

                if (argIndex >= argc) {
                    //Throw an invalid argument is argIndex is greater than argc
                    throw invalid_argument("Not enough argument for multiply()");
                }

                cout << "argv[" << argIndex << "]: " << argv[argIndex] << endl;
                int num2 = stoi(argv[argIndex]);

                // Call multiply function to multiply arguments from user

            }

            else {
                //throw invalid_argument("Invalid command: " + string(argv[argIndex]));
                throw invalid_argument("Invalid method name.");
            }
        }
    }
    catch (invalid_argument& e) {
        //cout << "Error: " << e.what() << endl;
        cout << "Invalid file name." << endl;
    }
}



