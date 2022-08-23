#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {
    Mat img;
    Mat img_padding;
    Mat my_kernel;
    Mat my_conv;

    int kernel_size;

    // Chon ma tran Kernel

    int selection;

        cout << "PLEASE SELECT THE TYPE OF KERNEL:\n"
            << "=================================\n"
            << "1. Blur 3x3 \n"
            << "2. Gaussian blur 3x3 \n"
            << "3. Gaussian blur 5x5 \n"
            << "4. Edge Detection Sobel to X 3x3 \n"
            << "5. Edge Detection Sobel to Y 3x3 \n"
            << "6. Laplacian - 4 way filter \n"
            << "7. Laplacian - 8 way filter \n"
            << "8. Sharpen \n"
            << "9. Unsharp masking \n"
            << "=================================\n";
             
        do {
            cout << "Choose your selection here: "; cin >> selection;
        } while (selection <= 0 || selection >= 10);

        switch (selection) {
        case 1:
            // Ma trận làm mờ ảnh 3x3 - Blur filter
            kernel_size = 3;
            my_kernel = (Mat_<double>(kernel_size, kernel_size) <<
                1, 1, 1,
                1, 1, 1,
                1, 1, 1) / (kernel_size * kernel_size);
            break;
        case 2:
            // Gaussian blur 3x3
            kernel_size = 3;
            my_kernel = (Mat_<double>(kernel_size, kernel_size) <<
                1 / 16.0, 2 / 16.0, 1 / 16.0,
                2 / 16.0, 4 / 16.0, 2 / 16.0,
                1 / 16.0, 2 / 16.0, 1 / 16.0);
            break;
        case 3:
            // Gaussian blur 5x5
            kernel_size = 5;
            my_kernel = (Mat_<double>(kernel_size, kernel_size) <<
                1 / 256.0, 4 / 256.0, 6 / 256.0, 4 / 256.0, 1 / 256.0,
                4 / 256.0, 16 / 256.0, 24 / 256.0, 16 / 256.0, 4 / 256.0,
                6 / 256.0, 24 / 256.0, 36 / 256.0, 24 / 256.0, 6 / 256.0,
                4 / 256.0, 16 / 256.0, 24 / 256.0, 16 / 256.0, 4 / 256.0,
                1 / 256.0, 4 / 256.0, 6 / 256.0, 4 / 256.0, 1 / 256.0);
            break;
        case 4:
            // Phát hiện biên trong ảnh dùng bộ lọc Sobel theo trục X - Edge detection
            kernel_size = 3;
            my_kernel = (Mat_<double>(kernel_size, kernel_size) <<
                -1, 0, 1,
                -2, 0, 2,
                -1, 0, 1);
            break;
        case 5:
            // Phát hiện biên trong ảnh dùng bộ lọc Sobel theo trục Y - Edge detection
            kernel_size = 3;
            my_kernel = (Mat_<double>(kernel_size, kernel_size) <<
                -1, -2, -1,
                0, 0, 0,
                1, 2, 1);
            break;
        case 6:
            // Lọc 4 hướng - Laplacian
            kernel_size = 3;
            my_kernel = (Mat_<double>(kernel_size, kernel_size) <<
                0, -1, 0,
                -1, 4, -1,
                0, -1, 0);
            break;
        case 7:
            // Lọc 8 hướng - Laplacian
            kernel_size = 3;
            my_kernel = (Mat_<double>(kernel_size, kernel_size) <<
                -1, -1, -1,
                -1, 8, -1,
                -1, -1, -1);
            break;
        case 8:
            // Làm nét ảnh - Sharpen
            kernel_size = 3;
            my_kernel = (Mat_<double>(kernel_size, kernel_size) <<
                0, -1, 0,
                -1, 5, -1,
                0, -1, 0);
            break;
        case 9:
            // Unsharp masking
            kernel_size = 5;
            my_kernel = (Mat_<double>(kernel_size, kernel_size) <<
                -1 / 256.0, -4 / 256.0, -6 / 256.0, -4 / 256.0, -1 / 256.0,
                -4 / 256.0, -16 / 256.0, -24 / 256.0, -16 / 256.0, -4 / 256.0,
                -6 / 256.0, -24 / 256.0, 476 / 256.0, -24 / 256.0, -6 / 256.0,
                -4 / 256.0, -16 / 256.0, -24 / 256.0, -16 / 256.0, -4 / 256.0,
                -1 / 256.0, -4 / 256.0, -6 / 256.0, -4 / 256.0, -1 / 256.0);
            break;
        default:
            cerr << "Invalid selection";
            return 1;
            break;
        }

    int kernel_radius = (kernel_size - 1) / 2;

    // Load Image - Doc anh
    img = imread("20182729.jpg");
    if (!img.data)
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }
    imshow("original image", img);
    img.convertTo(img, CV_64FC3);


    // Them vien cho anh goc de tranh loi tran vien khi tinh tich chap
    img_padding = Mat::Mat(img.rows + kernel_radius, img.cols + kernel_radius, CV_64FC3, CV_RGB(0, 0, 0));

    for (int x = 0; x < img.rows; x++) {
        for (int y = 0; y < img.cols; y++) {
            img_padding.at<Vec3d>(x + 1, y + 1)[0] = img.at<Vec3d>(x, y)[0];
            img_padding.at<Vec3d>(x + 1, y + 1)[1] = img.at<Vec3d>(x, y)[1];
            img_padding.at<Vec3d>(x + 1, y + 1)[2] = img.at<Vec3d>(x, y)[2];
        }
    }

    //Tinh toan tich chap
    my_conv = Mat::Mat(img.rows, img.cols, CV_64FC3, CV_RGB(0, 0, 0));
    for (int x = kernel_radius; x < img_padding.rows - kernel_radius; x++) {
        for (int y = kernel_radius; y < img_padding.cols - kernel_radius; y++) {
            double B = 0;
            double G = 0;
            double R = 0;
            for (int u = -kernel_radius; u <= kernel_radius; u++) {
                for (int v = -kernel_radius; v <= kernel_radius; v++) {
                    B += (img_padding.at<Vec3d>(x + u, y + v)[0] * my_kernel.at<double>(u + kernel_radius, v + kernel_radius));
                    G += (img_padding.at<Vec3d>(x + u, y + v)[1] * my_kernel.at<double>(u + kernel_radius, v + kernel_radius));
                    R += (img_padding.at<Vec3d>(x + u, y + v)[2] * my_kernel.at<double>(u + kernel_radius, v + kernel_radius));
                }
            }
            my_conv.at<Vec3d>(x - kernel_radius, y - kernel_radius)[0] = B;
            my_conv.at<Vec3d>(x - kernel_radius, y - kernel_radius)[1] = G;
            my_conv.at<Vec3d>(x - kernel_radius, y - kernel_radius)[2] = R;
        }
    }
    my_conv.convertTo(my_conv, CV_8UC3);
    imshow("Convolution_Image", my_conv);

    Mat dst;
    filter2D(img, dst, -1, my_kernel, Point(-1, -1), 0, BORDER_DEFAULT);
    dst.convertTo(dst, CV_8UC3);
    imshow("filter2D-Opencv", dst);
    imwrite("filter2D-Opencv.jpg", dst);

    my_conv.convertTo(my_conv, CV_8UC3);
    imshow("Convolution_Image", my_conv);
    imwrite("Convolution_Image.jpg", my_conv);

    waitKey();
    return 0;
}