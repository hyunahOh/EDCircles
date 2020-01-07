#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat nms(Mat input);

int main() {
	Mat input;

	input = imread("img.jpg", IMREAD_COLOR);
	if (input.empty())
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	//1. gray scale·Î º¯È¯
	Mat gray;
	cvtColor(input, gray, COLOR_BGR2GRAY);

	//2. gaussian blur
	Mat gaussianSmoothedImage;
	GaussianBlur(gray, gaussianSmoothedImage, Size(5, 5), 1.0);

	//3. edge detection
	Mat edgeDetectionX, edgeDetectionY, edgeDetection;
	Sobel(gaussianSmoothedImage, edgeDetectionX, CV_32F, 1, 0);
	convertScaleAbs(edgeDetectionX, edgeDetectionX);
	Sobel(gaussianSmoothedImage, edgeDetectionY, CV_32F, 0, 1);
	convertScaleAbs(edgeDetectionY, edgeDetectionY);
	addWeighted(edgeDetectionX, 0.5, edgeDetectionY, 0.5, 0, edgeDetection);

	//4. NMS
	Mat nonMax;
	nonMax = nms(edgeDetection);

	namedWindow("Original", WINDOW_AUTOSIZE);
	namedWindow("Gray", WINDOW_AUTOSIZE);
	namedWindow("Smoothed", WINDOW_AUTOSIZE);
	namedWindow("EdgeDetect", WINDOW_AUTOSIZE);
	namedWindow("NonMax", WINDOW_AUTOSIZE);

	imshow("Original", input);
	imshow("Gray", gray);
	imshow("Smoothed", gaussianSmoothedImage);
	imshow("EdgeDetect", edgeDetection);
	imshow("NonMax", nonMax);

	waitKey(0);
}

Mat nms(Mat input) {
	Mat out = Mat::zeros(input.size(), CV_32F);
	float max = 0;
	float current = 0;
	int maxRow = 0;
	int maxCol = 0;
	
	for (int i = 1; i < input.rows - 1; i++) {
		for (int j = 1; j < input.cols - 1; j++) {
			max = 0;

			for (int k = i - 1; k < i + 1; k++) {
				for (int l = j - 1; l < j + 1; l++) {
					current = input.at<uint8_t>(k, l);
					if (current > max) {
						max = current;
						maxRow = k;
						maxCol = l;
					}
				}
			}

			// Maintain maximum value and draw circle on the originating image to highlight interest point
			if (max > 0) {
				out.at<float>(maxRow, maxCol) = max;
				//circle(input, Point(maxCol, maxRow), 4, Scalar(0, 0, 0, 255));
			} 
		}
	}

	return out;
}