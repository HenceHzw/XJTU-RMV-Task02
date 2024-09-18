#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
    // 读取图像

    Mat image = imread("/opt/RM_tasks/XJTU-RMV-Task02/opencv_project/resources/test_image.png");
    //使用imread函数第二个参数设置为 0 ，得到灰度图
    // Mat image0 = imread("/opt/RM_tasks/XJTU-RMV-Task02/opencv_project/resources/test_image.png",0);
    // 检查图像是否成功加载
    if(image.empty()) {
        cout << "无法加载图片!" << endl;
        return -1;
    }

    // 显示原始图像
    imshow("Original Image", image);
    
    // 图像颜色空间转换
    
    // 转化为灰度图
    Mat gray_image;
    cvtColor(image, gray_image, COLOR_BGR2GRAY);
    imwrite("../resources/GrayscaleImage.png",gray_image);


    // 转化为 HSV 图像
    Mat hsv_image;
    cvtColor(image, hsv_image, COLOR_BGR2HSV);
    imwrite("../resources/HSVImage.png",hsv_image);

    // 应用各种滤波操作

    // 均值滤波
    Mat mean_blurred_image;
    blur(image, mean_blurred_image, Size(5, 5)); // 5x5 的滤波器
    imwrite("../resources/MeanBlurred.png",mean_blurred_image);


    // 高斯滤波
    Mat gaussian_blurred_image;
    GaussianBlur(image, gaussian_blurred_image, Size(5, 5), 0); // 5x5 的高斯滤波器
    imwrite("../resources/GaussianBlurredImage.png",gaussian_blurred_image);


    // 提取红色区域
    Mat mask1, mask2, mask;
    // 红色的 HSV 范围
    Scalar lower_red1(0, 100, 100);
    Scalar upper_red1(10, 255, 255);
    Scalar lower_red2(160, 100, 100);
    Scalar upper_red2(180, 255, 255);
    
    inRange(hsv_image, lower_red1, upper_red1, mask1);
    inRange(hsv_image, lower_red2, upper_red2, mask2);
    mask = mask1 | mask2;

    // 将掩码应用到原始图像上，只保留红色区域
    Mat red_region;
    bitwise_and(image, image, red_region, mask);

    // 掩码和提取后的红色区域
    imwrite("../resources/RedMask.png",mask);
    imwrite("../resources/RedRegion.png",red_region);

    // 寻找红色的外轮廓
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    Mat contour_image = image.clone();  // 克隆原图用于绘制轮廓
    for (size_t i = 0; i < contours.size(); i++) {
        drawContours(contour_image, contours, static_cast<int>(i), Scalar(0, 255, 0), 2);  // 绿色轮廓
    }

    imwrite("../resources/RedContours.png",contour_image);


    // 处理每个轮廓
    Mat BoundingBoxImage=image.clone();int num=0;
    for (size_t i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);  // 计算轮廓面积

        if(area!=0)
        {
            num++;
            cout << "Contour " << num << " Area: " << area <<"      pixels^2 "<< endl;
        }

        // 绘制外轮廓
        drawContours(BoundingBoxImage, contours, static_cast<int>(i), Scalar(255, 0, 0), 2);
        
        // 计算 bounding box
        Rect boundingBox = boundingRect(contours[i]);
        rectangle(BoundingBoxImage, boundingBox, Scalar(0, 255, 0), 2); // 绘制 bounding box
    }
    imwrite("../resources/BoundingBoxImage.png",BoundingBoxImage);


    // 提取高亮颜色区域并进行图形处理
    Mat highlighted;
    image.copyTo(highlighted, mask); // 高亮红色区域

    // 灰度化
    Mat gray_image0;
    cvtColor(highlighted, gray_image0, COLOR_BGR2GRAY);

    // 二值化
    Mat binary_image;
    threshold(gray_image0, binary_image, 50, 255, THRESH_BINARY);

    // 膨胀和腐蚀
    Mat dilated_image, eroded_image;
    dilate(highlighted, dilated_image, Mat(), Point(-1, -1), 2);
    erode(highlighted, eroded_image, Mat(), Point(-1, -1), 2);

    // 漫水处理
    Mat flood_filled = highlighted.clone();
    floodFill(flood_filled, Point(1000, 1000), Scalar(255,255,255));//从像素点（1000, 1000）开始漫水填充
    
    // 处理结果
    imwrite("../resources/highlighted.png",highlighted);

    imwrite("../resources/GrayImage.png",gray_image0);

    imwrite("../resources/BinaryImage.png",binary_image);

    imwrite("../resources/DilatedImage.png",dilated_image);

    imwrite("../resources/ErodedImage.png",eroded_image);

    imwrite("../resources/FloodFilledImage.png",flood_filled);

    // 图像绘制
    Mat DrawnImage=image.clone();
    // 绘制圆形
    circle(DrawnImage, Point(100,250), 50, Scalar(0, 0, 255), -1); // 红色圆形

    // 绘制方形
    rectangle(DrawnImage, Point(200, 200), Point(300, 300), Scalar(255, 0, 255), -1); // 紫色方形

    // 绘制文字
    putText(DrawnImage, "RM TASK 02", Point(100, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 2);

    // 绘制后的图像
    imwrite("../resources/DrawnImage.png",DrawnImage);

    // 图像旋转 35 度
    Point2f center(image.cols / 2.0f, image.rows / 2.0f);
    Mat rotation_matrix = getRotationMatrix2D(center, 35, 1.0); // 旋转中心和角度
    Mat rotated_image;
    warpAffine(image, rotated_image, rotation_matrix, image.size());
    imwrite("../resources/RotatedImage.png",rotated_image);

    // 图像裁剪为原图的左上角 1/4
    Rect roi(0, 0, image.cols / 2, image.rows / 2); // 选择左上角 1/4 区域
    Mat cropped_image = image(roi);
    imwrite("../resources/CroppedImage.png",cropped_image);

    
    waitKey(0);


    return 0;
}