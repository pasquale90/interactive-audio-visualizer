#include<opencv2/opencv.hpp>//OpenCV header to use VideoCapture class//
// #include<opencv2/core/core.hpp>
#include<opencv2/core/mat.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
using namespace std;
using namespace cv;
int main() {
   Mat myImage;//Declaring a matrix to load the frames//
   Mat thresImage;

   namedWindow("Video Player");//Declaring the video to show the video//
   VideoCapture cap(0);//Declaring an object to capture stream of frames from default camera//
   if (!cap.isOpened()){ //This section prompt an error message if no video stream is found//
      cout << "No video stream detected" << endl;
      system("pause");
      return-1;
   }
   while (true){ //Taking an everlasting loop to show the video//
      cap >> myImage;

      // std::cout<<myImage<<std::endl;

      // inRange(myImage,  cv::Scalar(30, 137, 0), cv::Scalar(70, 255, 255), thresImage);
      inRange(myImage,  cv::Scalar(0, 184, 0), cv::Scalar(158, 255, 255), thresImage);

      if (myImage.empty()){ //Breaking the loop if no video frame is detected//
         break;
      }
      imshow("Video Player", thresImage);//Showing the video//
      char c = (char)waitKey(25);//Allowing 25 milliseconds frame processing time and initiating break condition//
      if (c == 27){ //If 'Esc' is entered break the loop//
         break;
      }
      
   }
   // imwrite("test.jpg", myImage);
   cap.release();//Releasing the buffer memory//
   return 0;
}