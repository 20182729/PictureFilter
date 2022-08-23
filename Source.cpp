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
//
//void icvprLabelColor(const cv::Mat& _labelImg, cv::Mat& _colorLabelImg) {
//	if (_labelImg.empty() || _labelImg.type() != CV_32SC1) {
//		return;
//	}
//
//	std::map<int, cv::Scalar> color;
//
//	int rows = _labelImg.rows;
//	int cols = _labelImg.cols;
//
//	_colorLabelImg.release();
//	_colorLabelImg.create(rows, cols, CV_8UC3);
//	_colorLabelImg = cv::Scalar::all(0);
//
//	for (int i = 0; i < rows; i++) {
//		const int* data_src = (int*)_labelImg.ptr<int>(i);
//		uchar* data_dst = _colorLabelImg.ptr<uchar>(i);
//
//		for (int j = 0; j < cols; j++) {
//			int pixelValue = data_src[j];
//
//			if (pixelValue > 1) {
//				if (color.count(pixelValue) <= 0) {
//					color[pixelValue] = icvprGetRandomColor();
//				}
//
//				cv::Scalar color = color[pixelValue];
//				*data_dst++ = color[0];
//				*data_dst++ = color[1];
//				*data_dst++ = color[2];
//			}
//			else {
//				data_dst++;
//				data_dst++;
//				data_dst++;
//			}
//		}
//
//	}
//		
//}
