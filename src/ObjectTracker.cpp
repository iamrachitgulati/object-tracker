#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>

/*
TRACK A YELLOW BALL - OBJECT DETECTION METHOD USING COLOR SEPERATION OPEN CV 3.1.0
author - Rachit Gulati
*/

int main() {

	cv::VideoCapture capWebcam(0);		// declare a VideoCapture object to associate webcam, 0 means use 1st (default) webcam

	if (capWebcam.isOpened() == false)	 //  To check if object was associated to webcam successfully
	{				
		std::cout << "error: Webcam connect unsuccessful\n";	// if not then print error message
		return(0);												// and exit program
	}

	cv::Mat imgOriginal;		// Input image
	cv::Mat hsvImg;				// HSV Image
	cv::Mat threshImg;			// Thresh Image

	std::vector<cv::Vec3f> v3fCircles;		// 3 element vector of floats, this will be the pass by reference output of HoughCircles()

	char charCheckForEscKey = 0;

	int lowH = 21;							// Set Hue
	int highH = 30;

	int lowS = 200;							// Set Saturation
	int highS = 255;

	int lowV = 102;							// Set Value
	int highV = 225;
	// HUE for YELLOW is 21-30.
	// Adjust Saturation and Value depending on the lighting condition of the environment as well as the surface of the object.

	while (charCheckForEscKey != 27 && capWebcam.isOpened()) {				// until the Esc is pressed or webcam connection is lost
		bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);		// get next frame

		if (!blnFrameReadSuccessfully || imgOriginal.empty()) {				// if frame read unsuccessfully
			std::cout << "error: frame can't read \n";						// print error message
			break;															// jump out of loop
		}

		cv::cvtColor(imgOriginal, hsvImg, CV_BGR2HSV);						// Convert Original Image to HSV Thresh Image

		cv::inRange(hsvImg, cv::Scalar(lowH, lowS, lowV), cv::Scalar(highH, highS, highV), threshImg);

		cv::GaussianBlur(threshImg, threshImg, cv::Size(3, 3), 0);			//Blur Effect
		cv::dilate(threshImg, threshImg, 0);								// Dilate Filter Effect
		cv::erode(threshImg, threshImg, 0);									// Erode Filter Effect

		// fill circles vector with all circles in processed image
		cv::HoughCircles(threshImg,v3fCircles,CV_HOUGH_GRADIENT,2,threshImg.rows / 4,100,50,10,800);  // algorithm for detecting circles		

		for (int i = 0; i < v3fCircles.size(); i++) {						// for each circle
															
			std::cout << "Ball position X = "<< v3fCircles[i][0]			// x position of center point of circle
				<<",\tY = "<< v3fCircles[i][1]								// y position of center point of circle
				<<",\tRadius = "<< v3fCircles[i][2]<< "\n";					// radius of circle

																				// draw small green circle at center of object detected
			cv::circle(imgOriginal,												// draw on original image
				cv::Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]),		// center point of circle
				3,																// radius of circle in pixels
				cv::Scalar(0, 255, 0),											// draw green
				CV_FILLED);														// thickness

																				// draw red circle around object detected 
			cv::circle(imgOriginal,												// draw on original image
				cv::Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]),		// center point of circle
				(int)v3fCircles[i][2],											// radius of circle in pixels
				cv::Scalar(0, 0, 255),											// draw red
				3);																// thickness
		}	

		// declare windows
		cv::namedWindow("imgOriginal", CV_WINDOW_AUTOSIZE);
		cv::namedWindow("threshImg", CV_WINDOW_AUTOSIZE);	

	    /* Create trackbars in "threshImg" window to adjust according to object and environment.*/
		cv::createTrackbar("LowH", "threshImg", &lowH, 179);	//Hue (0 - 179)
		cv::createTrackbar("HighH", "threshImg", &highH, 179);

		cv::createTrackbar("LowS", "threshImg", &lowS, 255);	//Saturation (0 - 255)
		cv::createTrackbar("HighS", "threshImg", &highS, 255);

		cv::createTrackbar("LowV", "threshImg", &lowV, 255);	//Value (0 - 255)
		cv::createTrackbar("HighV", "threshImg", &highV, 255);
		

		cv::imshow("imgOriginal", imgOriginal);					// show windows
		cv::imshow("threshImg", threshImg);

		charCheckForEscKey = cv::waitKey(1);					// delay and get key press
	}
	
	return(0);											
}
