#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace cv;


int       LENGTH;
const int         HEIGHT = 100;
const int         WIDTH  = 80;

void rotate(Mat &input, Mat &output) {

    int sign = rand() % 2;
    if ( sign == 0 ) {
        sign = -1;
    }

    int angle = ( rand() % 30 ) * sign; // between -30 and 30

    Point2f center(input.cols/2.0F, input.rows/2.0F);
    Mat rotationMatrix = getRotationMatrix2D(center, angle, 1.0);

    warpAffine(input, output, rotationMatrix, input.size(),
        INTER_LINEAR, BORDER_CONSTANT, Scalar(255,255,255,0));

}

void scale(Mat &input, Mat &output, float height, float width) {

    height = ( ( rand() % 20 ) * -1 ) + height;
    width  = ( ( rand() % 20 ) * -1 ) + width;

    Size s(width, height);
    cv::resize(input, output, s);
}

void addLines(Mat &image) {
    rand();
    RNG rng(rand());
    int numLines = 9;
    for ( int i = 0; i < numLines; ++i) {
        int startX = rand() % image.cols;
        int endX = rand()   % image.cols;
        int startY = rand() % image.rows;
        int endY = rand()   % image.rows;

        cv::line(image,
            cv::Point(startX, startY),
            cv::Point(endX, endY),
            Scalar(255,255,255),
            rng.uniform(0, 3), CV_AA);
    }
}

void transform(Mat &charImage) {

    cv::Point2f src[4];
    src[0] = cv::Point2f(0, 0);
    src[1] = cv::Point2f(0, HEIGHT);
    src[2] = cv::Point2f(WIDTH, 0);
    src[3] = cv::Point2f(WIDTH, HEIGHT);

    cv::Point2f dst[4];
    dst[0] = cv::Point2f(0, 0);
    dst[1] = cv::Point2f(0, HEIGHT);
    dst[2] = cv::Point2f(WIDTH, 0);

    int varWidth  = WIDTH / 2;
    int varHeight = HEIGHT / 2;
    int widthWarp  = WIDTH - varWidth + (rand() % varWidth);
    int heightWarp = HEIGHT - varHeight + (rand() % varHeight);
    dst[3] = cv::Point2f(widthWarp, heightWarp);

    Mat perspectiveTranx = cv::getPerspectiveTransform(src, dst);
    cv::warpPerspective(charImage, charImage, perspectiveTranx,
        cv::Size(charImage.cols, charImage.rows),
        INTER_CUBIC,
        BORDER_CONSTANT,
        Scalar(255,255,255,0));
}

void randomize(Mat &input, float height, float width) {
    rotate(input, input);
    scale(input, input, height, width);
    transform(input);
}

void addPoints(Mat &image) {

    rand();
    RNG rng(rand());
    int i,j;
    for (int n = 0; n < 100; n++) {

        i= rand() % image.cols;
        j= rand() % image.rows;

//        i = image.cols / 2;
//        j = image.rows / 2;
        Point center(i, j);

        circle(image, center, rng.uniform(1, 3),
            Scalar(rng.uniform(0, 256),rng.uniform(0, 256),rng.uniform(0, 256)),
            -2,
            CV_AA);
    }
}

void addCircle(Mat &image) {
    int i,j;
    i = 100;
    j = image.rows / 2;
    Point center(i, j);

    ellipse( image, center, Size( ( rand() % 90 ),  ( rand() % 150 )  ), rand() % 25, 0, 360, Scalar( 0, 0, 0 ), -2, 8 );
    i = image.cols - 100;
    j = image.rows / 2;
    Point second(i, j);
    ellipse( image, second, Size( ( rand() % 90 ), ( rand() % 90 ) ), rand() % 20, 0, 360, Scalar( 0, 0, 0 ), -2, 8 );
    bitwise_or ( image, image, image );
}

int main(int argc, char *argv[])
{
    string inputWord = argv[1];
    LENGTH = inputWord.size();

    Mat outImage(HEIGHT, WIDTH * LENGTH, CV_8UC3, Scalar(255,255,255,0));

    srand((unsigned)time(0));
    rand();
    RNG rng(rand());

    Scalar color = CV_RGB(0, 0, 0);

//    addCircle(outImage);

    for (std::string::size_type i = 0; i < inputWord.size(); i++) {
        cv::Mat charImage(HEIGHT, WIDTH, CV_8UC3, Scalar(255,255,255,0));

        string c(1, inputWord[i]);
        putText(charImage, c, Point(10, HEIGHT - 10), rng.uniform(1, 6), rng.uniform(3.0, 4.0), color, rng.uniform(1, 5), CV_AA);

        randomize(charImage, HEIGHT, WIDTH);

        if(i % (rng.uniform(1,6)) == 0) {

            bitwise_not(charImage, charImage);
        }
        charImage.copyTo(outImage(cv::Rect(WIDTH * i, 0, charImage.cols, charImage.rows)));

    }

    cout << rand() % inputWord.size() <<endl;

    if ((rand() % inputWord.size()) % 2 == 0) {
        addLines(outImage);
    }

    if ((rand() % inputWord.size()) % 3 == 0) {
        addPoints(outImage);
    }


    imshow("captcha", outImage);
    waitKey(0);
    imwrite( inputWord+".jpg", outImage );
    return EXIT_SUCCESS;
}
