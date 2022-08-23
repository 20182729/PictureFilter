//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
//// Create Kernel matrix
//Mat Kernel_matrix_3_3 = (
//	Mat_<double>(3, 3) <<
//	0, -1, 0,
//	-1, 5, -1,
//	0, -1, 0);
//// Convolution Function
//void Convlution(Mat  InputImage, Mat  OutputImage, Mat kernel)
//{
//	// Calculate the convolution kernel radius
//	int sub_x = kernel.cols / 2;
//	int sub_y = kernel.rows / 2;
//	// Traverse picture  
//	for (int image_y = 0; image_y < InputImage.rows - 2 * sub_y; image_y++)
//	{
//		for (int image_x = 0; image_x < InputImage.cols - 2 * sub_x; image_x++)
//		{
//			int pix_value = 0;
//			for (int kernel_y = 0; kernel_y < kernel.rows; kernel_y++)
//			{
//				for (int kernel_x = 0; kernel_x < kernel.cols; kernel_x++)
//				{
//					double  weihgt = kernel.at<double>(kernel_y, kernel_x);
//					int value = (int)InputImage.at<uchar>(image_y + kernel_y, image_x + kernel_x);
//					pix_value += weihgt * value;
//				}
//			}
//			OutputImage.at<uchar>(image_y + sub_y, image_x + sub_x) = (uchar)pix_value;
//		}
//	}
//}
//
//int main()
//{
//	string path = "20182729.jpg";
//	Mat srcImage = imread(path, 0);
//	imshow("OriginalImage", srcImage);
//
//	// filter2D convolution
//	Mat dstImage_oprncv(srcImage.rows, srcImage.cols, CV_8UC1, Scalar(0));
//	filter2D(srcImage, dstImage_oprncv, srcImage.depth(), Kernel_matrix_3_3);
//	imshow("filter2DImage", dstImage_oprncv);
//	imwrite("Image_1.jpg", dstImage_oprncv);
//
//	// Custom convolution
//	Mat dstImage_mycov(srcImage.rows, srcImage.cols, CV_8UC1, Scalar(0));
//	Convlution(srcImage, dstImage_mycov, Kernel_matrix_3_3);
//	imshow("ConvolutionImage", dstImage_mycov);
//	imwrite("Image_Convolution.jpg", dstImage_mycov);
//
//	waitKey(0);
//	return 0;
//}
