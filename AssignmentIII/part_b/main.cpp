// This program is a simple template of an C++ program loading and showing image with OpenCV.
// You can ignore this file and write your own program.
// The program takes a image file name as an argument.

#include <stdio.h>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

float calculateMoment(Mat Image, float a, float b);
float calculateCentralMoment(Mat Image, float j, float b, float x, float y);

int main(int argc, char **argv) {

	// Check number of arguments.
	// If no argument input, exit the program.
	if ( argc != 2 )
		exit(1);

	Mat SrcImage, WorkImage, ComImage;

	// Load a gray scale picture.
	SrcImage = imread(argv[1], 0);
	if (!SrcImage.data)
		exit(1);

	// Create windows for debug.
	namedWindow("SrcImage", WINDOW_AUTOSIZE);
	namedWindow("WorkImage", WINDOW_AUTOSIZE);

	// Show the source image.

	imshow("SrcImage", SrcImage);
	waitKey(0);

	// Duplicate the source iamge.
	WorkImage = SrcImage.clone();




	//Extract the contour of
	/* If you're familiar with OpenCV, findContours() will be a better way.*/
	GaussianBlur(WorkImage, WorkImage, Size( 5, 5), 0, 0);
	threshold(WorkImage, WorkImage, 160, 255, THRESH_BINARY);
	// Not sure what is the output... because put ONE threshold on 3 channel picture????????????
	// Or maybe it's not a RGB picture?




	//	// Opening
	//	erode(WorkImage, WorkImage, Mat());
	//	dilate(WorkImage, WorkImage, Mat());

	//	// Duplicate the working iamge.
		ComImage = WorkImage.clone();

	// Show the working image after preprocessing.





//	for testing, draw a small picture:
//	Mat testImage(4,2,CV_8UC3, Scalar::all(0));

//	testImage.at<Vec3b>(0,0)[0] = 255;
//	testImage.at<Vec3b>(0,0)[1] = 255;
//	testImage.at<Vec3b>(0,0)[2] = 255;

//	testImage.at<Vec3b>(1,1)[0] = 255;
//	testImage.at<Vec3b>(1,1)[1] = 255;
//	testImage.at<Vec3b>(1,1)[2] = 255;

//	testImage.at<Vec3b>(2,1)[0] = 255;
//	testImage.at<Vec3b>(2,1)[1] = 255;
//	testImage.at<Vec3b>(2,1)[2] = 255;

//	testImage.at<Vec3b>(3,1)[0] = 255;
//	testImage.at<Vec3b>(3,1)[1] = 255;
//	testImage.at<Vec3b>(3,1)[2] = 255;

//	WorkImage = testImage.clone();
//	cout << WorkImage << endl;



	float xc, yc, m11, m20, m02, theta;
	xc = calculateMoment(WorkImage, 1, 0) / calculateMoment(WorkImage, 0, 0);
	yc = calculateMoment(WorkImage, 0, 1) / calculateMoment(WorkImage, 0, 0);


	cout << "xc, yc = " << xc << " " << yc << endl;
	cout << "h, w = " << WorkImage.rows << " " << WorkImage.cols << endl;
	cout << "xc, yc = " << xc / (float)WorkImage.rows << " " << yc / (float)WorkImage.cols << endl;


	m11 = calculateCentralMoment(WorkImage, 1, 1, xc, yc);
	m20 = calculateCentralMoment(WorkImage, 2, 0, xc, yc);
	m02 = calculateCentralMoment(WorkImage, 0, 2, xc, yc);
	theta = 0.5 * atan2(2*m11, m20 - m02);
	cout << "theta = " << theta * 180.0 / 3.14 << endl << endl;

	for (int i = -2; i <= 2; i++)
	{
		for (int j = -2; j <= 2; j++)
		{
			// For each chanel.... What's going on?????????????
			for (int k = 0; k < 3; k++)
			{
				WorkImage.at<Vec3b>((int)xc + i, (int)yc + j)[k] = 0;
			}
		}
	}

	imshow("WorkImage", WorkImage);
	waitKey(0);

	return 0;
}


float calculateMoment(Mat Image, float a, float b)
{
	float sum = 0;
	int height = Image.rows;
	int width  = Image.cols;
	float i, j;
	
	for (i = 0; i < height; i+= 1)
	{
		for (j = 0; j < width; j+= 1)
		{
			if ((Image.at<Vec3b>(i, j)).val[0] == 255)
			{
				sum += pow(i, a) * pow(j, b);
			}
		}
	}
//	cout << "i, j = " << i << " " << j << endl;
	cout << "calculateMoment, " << a << " "<< b << ",ans:" << sum << endl;
	return sum;
}

float calculateCentralMoment(Mat Image, float a, float b, float x, float y)
{
	float sum = 0;
	int height = Image.rows;
	int width  = Image.cols;

	for (float i = 0; i < height; i++)
	{
		for (float j = 0; j < width; j++)
		{
			if ((Image.at<Vec3b>(i, j)).val[0] == 255)
			{
				sum += pow(i - x, a) * pow(j - y, b);
			}
		}
	}
	cout << "calculateCentralMoment, " << a << " "<< b << ",ans:" << sum << endl;
	return sum;
}




