// Convert a color image into grayscale image
/*#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
	const float pi = 3.14;
	Mat src1, src2;
	src1 = imread("anhdep.jfif", CV_LOAD_IMAGE_COLOR);
	src2 = Mat::zeros(src1.rows, src1.cols, CV_8UC1);
	if (!src1.data) { printf("Error loading src1 \n"); return -1; }

	for (int i = 0; i < src1.cols; i++) {
		for (int j = 0; j < src1.rows; j++)
		{
			Vec3b color1 = src1.at<Vec3b>(Point(i, j));
			Scalar color2 = src2.at<uchar>(Point(i, j));
			color2 = (color1.val[0] + color1.val[1] + color1.val[2]) / 3;
			src2.at<uchar>(Point(i, j)) = color2.val[0];
		}
	}
	imwrite("anh.jpg", src2);
	
	namedWindow("GRAYSCALE_IMAGE", CV_WINDOW_AUTOSIZE);
	imshow("GRAYSCALE_IMAGE", src2);

	namedWindow("Original Image", CV_WINDOW_AUTOSIZE);
	imshow("Original Image", src1);

	waitKey(0);
	return 0;
}
//Connected components labeling
*/
#include <iostream>  
#include <string>  
#include <vector>  
#include <map>  

#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/highgui/highgui.hpp>  


void icvprCcaByTwoPass(const cv::Mat& _binImg, cv::Mat& _lableImg)
{
	// connected component analysis (4-component)  
	// use two-pass algorithm  
	// 1. first pass: label each foreground pixel with a label  
	// 2. second pass: visit each labeled pixel and merge neighbor labels  
	//   
	// foreground pixel: _binImg(x,y) = 1  
	// background pixel: _binImg(x,y) = 0  


	if (_binImg.empty() ||
		_binImg.type() != CV_8UC1)
	{
		return;
	}

	// 1. first pass  

	_lableImg.release();
	_binImg.convertTo(_lableImg, CV_32SC1);

	int label = 1;  // start by 2  
	std::vector<int> labelSet;
	labelSet.push_back(0);   // background: 0  
	labelSet.push_back(1);   // foreground: 1  

	int rows = _binImg.rows - 1;
	int cols = _binImg.cols - 1;
	for (int i = 1; i < rows; i++)
	{
		int* data_preRow = _lableImg.ptr<int>(i - 1);
		int* data_curRow = _lableImg.ptr<int>(i);
		for (int j = 1; j < cols; j++)
		{
			if (data_curRow[j] == 1)
			{
				std::vector<int> neighborLabels;
				neighborLabels.reserve(2);
				// 4-connectivity
				int leftPixel = data_curRow[j - 1];
				int upPixel = data_preRow[j];

				// 8-connectivity
				//int leftupPixel = data_preRow[j-1];           
				//int rightupPixel = data_preRow[j+1];           
				
				if (leftPixel > 1)
				{
					neighborLabels.push_back(leftPixel);
				}
				if (upPixel > 1)
				{
					neighborLabels.push_back(upPixel);
				}
				
				//if (leftupPixel > 1) {
				//	neighborLabels.push_back(leftupPixel);
				//}
				
				//if (rightupPixel > 1) {
				//	neighborLabels.push_back(rightupPixel);
				//}
				if (neighborLabels.empty())
				{
					labelSet.push_back(++label);  // assign to a new label  
					data_curRow[j] = label;
					labelSet[label] = label;
				}
				else
				{
					std::sort(neighborLabels.begin(), neighborLabels.end());
					int smallestLabel = neighborLabels[0];
					data_curRow[j] = smallestLabel;

					// save equivalence  
					for (size_t k = 1; k < neighborLabels.size(); k++)
					{
						int tempLabel = neighborLabels[k];
						int& oldSmallestLabel = labelSet[tempLabel];
						if (oldSmallestLabel > smallestLabel)
						{
							labelSet[oldSmallestLabel] = smallestLabel;
							oldSmallestLabel = smallestLabel;
						}
						else if (oldSmallestLabel < smallestLabel)
						{
							labelSet[smallestLabel] = oldSmallestLabel;
						}
					}
				}
			}
		}
	}

	// update equivalent labels  
	// assigned with the smallest label in each equivalent label set  
	for (size_t i = 2; i < labelSet.size(); i++)
	{
		int curLabel = labelSet[i];
		int preLabel = labelSet[curLabel];
		while (preLabel != curLabel)
		{
			curLabel = preLabel;
			preLabel = labelSet[preLabel];
		}
		labelSet[i] = curLabel;
	}


	// 2. second pass  
	for (int i = 0; i < rows; i++)
	{
		int* data = _lableImg.ptr<int>(i);
		for (int j = 0; j < cols; j++)
		{
			int& pixelLabel = data[j];
			pixelLabel = labelSet[pixelLabel];
		}
	}
}
cv::Scalar icvprGetRandomColor()
{
	uchar r = 255 * (rand() / (1.0 + RAND_MAX));
	uchar g = 255 * (rand() / (1.0 + RAND_MAX));
	uchar b = 255 * (rand() / (1.0 + RAND_MAX));
	return cv::Scalar(b, g, r);
}


void icvprLabelColor(const cv::Mat& _labelImg, cv::Mat& _colorLabelImg)
{
	if (_labelImg.empty() ||
		_labelImg.type() != CV_32SC1)
	{
		return;
	}

	std::map<int, cv::Scalar> colors;

	int rows = _labelImg.rows;
	int cols = _labelImg.cols;

	_colorLabelImg.release();
	_colorLabelImg.create(rows, cols, CV_8UC3);
	_colorLabelImg = cv::Scalar::all(0);

	for (int i = 0; i < rows; i++)
	{
		const int* data_src = (int*)_labelImg.ptr<int>(i);
		uchar* data_dst = _colorLabelImg.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			int pixelValue = data_src[j];
			if (pixelValue > 1)
			{
				if (colors.count(pixelValue) <= 0)
				{
					colors[pixelValue] = icvprGetRandomColor();
				}
				cv::Scalar color = colors[pixelValue];
				*data_dst++ = color[0];
				*data_dst++ = color[1];
				*data_dst++ = color[2];
			}
			else
			{
				data_dst++;
				data_dst++;
				data_dst++;
			}
		}
	}
}
int main(int argc, char** argv)
{
	cv::Mat OriginalImage = cv::imread("anhdep2.jfif", 1);
	cv::Mat binImage = cv::imread("anhdep2.jfif", 0);
	cv::threshold(binImage, binImage, 100, 1, CV_THRESH_BINARY_INV);

	// connected component labeling  
	cv::Mat labelImg;
	icvprCcaByTwoPass(binImage, labelImg);
	
	// show result  
	cv::Mat grayImg;
	labelImg *= 10;
	labelImg.convertTo(grayImg, CV_8UC1);
	cv::imshow("OriginalImage", OriginalImage);
	cv::imshow("labelImg", grayImg);

	cv::Mat colorLabelImg;
	icvprLabelColor(labelImg, colorLabelImg);
	cv::imshow("colorImg", colorLabelImg);
	cv::waitKey(0);

	return 0;
}