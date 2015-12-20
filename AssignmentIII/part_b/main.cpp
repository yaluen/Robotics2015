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

	Mat SrcImage, WorkImage;

	// Load a gray scale picture.
	SrcImage = imread(argv[1], 0);
	if (!SrcImage.data)
		exit(1);

	// Create windows for debug.
	//namedWindow("SrcImage", WINDOW_AUTOSIZE);
	//namedWindow("WorkImage", WINDOW_AUTOSIZE);
	if (SrcImage.channels()>1)
		cvtColor(SrcImage, SrcImage, CV_BGR2GRAY);
	//else gray = original;
	
	blur(SrcImage, SrcImage, Size(3, 3));


	// Show the source image.
	imshow("SrcImage", SrcImage);
	//waitKey(0);

	// Duplicate the source iamge.
	WorkImage = SrcImage.clone();


	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	// For images 1-3
	int x_border = 0;
	int y_border = 0;
	int x_border_max = 700;
	int y_border_max = 700;
	int thresh = 100;
	// For image 4
	int max_thresh = 255;
	RNG rng(12345);

	/// Detect edges using canny
	Canny(WorkImage, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Get the moments
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}

	///  Get the mass centers (aka centroid):
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		if (mc[i].x > x_border && mc[i].y > y_border && mc[i].x < x_border_max && mc[i].y < y_border_max)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
			circle(drawing, mc[i], 4, color, -1, 8, 0);
		}
	}

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);

	// Debug use
	/// Calculate the area with the moments 00 and compare with the result of the OpenCV function
	printf("\t Info: Area and Contour Length \n");
	for (int i = 0; i< contours.size(); i++)
	{
		if (mc[i].x > x_border && mc[i].y > y_border && mc[i].x < x_border_max && mc[i].y < y_border_max)
		{
			printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(contours[i]), arcLength(contours[i], true));
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
			circle(drawing, mc[i], 4, color, -1, 8, 0);
		}
	}


	// Calculate the principal angle using the central moments
	// Central moments can be retrieved from moments (mu) by: mu[i].mu20, mu[i].mu11, etc
	// So we should be able to get the principal angle like so:
	for (int i = 0; i < contours.size(); i++)
	{
		float theta;
		// Further filter of noise: No area too small, and no border of the entire picture.
		/*if (mu[i].m00 > 20 && mu[i].m00 < 100000 && (mc[i].x > x_border && mc[i].y > y_border && mc[i].x < x_border_max && mc[i].y < y_border_max))
		{*/
		if (mc[i].x > x_border && mc[i].y > y_border && mc[i].x < x_border_max && mc[i].y < y_border_max)
		{
			theta = 0.5 * atan2(2 * mu[i].m11, mu[i].m20 - mu[i].m02);
			//cout << "theta = " << (theta * 180.0 / 3.14) << endl << endl;
			//cout << "Number: " << i << " " << mc[i].x << " " << mc[i].y << " " << (theta * 180.0 / 3.14) << endl << endl;
			cout << mc[i].x << " " << mc[i].y << " " << (theta * 180.0 / 3.14) << endl;
		}
	}

	//Extract the contour of
	/* If you're familiar with OpenCV, findContours() will be a better way.*/
//	GaussianBlur(WorkImage, WorkImage, Size( 5, 5), 0, 0);
//	threshold(WorkImage, WorkImage, 160, 255, THRESH_BINARY);
//	// Not sure what is the output... because put ONE threshold on 3 channel picture????????????
//	// Or maybe it's not a RGB picture?
//
//
//
//
//	//	// Opening
//	//	erode(WorkImage, WorkImage, Mat());
//	//	dilate(WorkImage, WorkImage, Mat());
//
//	//	// Duplicate the working iamge.
//		ComImage = WorkImage.clone();
//
//	// Show the working image after preprocessing.
//
//
//
//
//
////	for testing, draw a small picture:
////	Mat testImage(4,2,CV_8UC3, Scalar::all(0));
//
////	testImage.at<Vec3b>(0,0)[0] = 255;
////	testImage.at<Vec3b>(0,0)[1] = 255;
////	testImage.at<Vec3b>(0,0)[2] = 255;
//
////	testImage.at<Vec3b>(1,1)[0] = 255;
////	testImage.at<Vec3b>(1,1)[1] = 255;
////	testImage.at<Vec3b>(1,1)[2] = 255;
//
////	testImage.at<Vec3b>(2,1)[0] = 255;
////	testImage.at<Vec3b>(2,1)[1] = 255;
////	testImage.at<Vec3b>(2,1)[2] = 255;
//
////	testImage.at<Vec3b>(3,1)[0] = 255;
////	testImage.at<Vec3b>(3,1)[1] = 255;
////	testImage.at<Vec3b>(3,1)[2] = 255;
//
////	WorkImage = testImage.clone();
////	cout << WorkImage << endl;
//
//
//
//	float xc, yc, m11, m20, m02, theta;
//	xc = calculateMoment(WorkImage, 1, 0) / calculateMoment(WorkImage, 0, 0);
//	yc = calculateMoment(WorkImage, 0, 1) / calculateMoment(WorkImage, 0, 0);
//
//
//	cout << "xc, yc = " << xc << " " << yc << endl;
//	cout << "h, w = " << WorkImage.rows << " " << WorkImage.cols << endl;
//	cout << "xc, yc = " << xc / (float)WorkImage.rows << " " << yc / (float)WorkImage.cols << endl;
//
//
//	m11 = calculateCentralMoment(WorkImage, 1, 1, xc, yc);
//	m20 = calculateCentralMoment(WorkImage, 2, 0, xc, yc);
//	m02 = calculateCentralMoment(WorkImage, 0, 2, xc, yc);
//	theta = 0.5 * atan2(2*m11, m20 - m02);
//	cout << "theta = " << theta * 180.0 / 3.14 << endl << endl;
//
//	for (int i = -2; i <= 2; i++)
//	{
//		for (int j = -2; j <= 2; j++)
//		{
//			// For each chanel.... What's going on?????????????
//			for (int k = 0; k < 3; k++)
//			{
//				WorkImage.at<Vec3b>((int)xc + i, (int)yc + j)[k] = 0;
//			}
//		}
//	}
//
//	imshow("WorkImage", WorkImage);
	waitKey(0);

	return 0;
}

//
//float calculateMoment(Mat Image, float a, float b)
//{
//	float sum = 0;
//	int height = Image.rows;
//	int width  = Image.cols;
//	float i, j;
//	
//	for (i = 0; i < height; i+= 1)
//	{
//		for (j = 0; j < width; j+= 1)
//		{
//			if ((Image.at<Vec3b>(i, j)).val[0] == 255)
//			{
//				sum += pow(i, a) * pow(j, b);
//			}
//		}
//	}
////	cout << "i, j = " << i << " " << j << endl;
//	cout << "calculateMoment, " << a << " "<< b << ",ans:" << sum << endl;
//	return sum;
//}
//
//float calculateCentralMoment(Mat Image, float a, float b, float x, float y)
//{
//	float sum = 0;
//	int height = Image.rows;
//	int width  = Image.cols;
//
//	for (float i = 0; i < height; i++)
//	{
//		for (float j = 0; j < width; j++)
//		{
//			if ((Image.at<Vec3b>(i, j)).val[0] == 255)
//			{
//				sum += pow(i - x, a) * pow(j - y, b);
//			}
//		}
//	}
//	cout << "calculateCentralMoment, " << a << " "<< b << ",ans:" << sum << endl;
//	return sum;
//}
//
//
//
//
