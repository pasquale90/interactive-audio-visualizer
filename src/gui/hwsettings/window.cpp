#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>
#include <string>

void createRandomColorWindow(int width, int height) {
    // Initialize random seed
    std::srand(std::time(0));

    // Generate random color values for B, G, R
    uchar blue = std::rand() % 256;
    uchar green = std::rand() % 256;
    uchar red = std::rand() % 256;

    // Create an image with the specified dimensions and fill it with the random color
    cv::Mat image(height, width, CV_8UC3, cv::Scalar(blue, green, red));

    std::string text = "("+std::to_string(width)+","+std::to_string(height)+")";
    cv::putText(image, //target image
            text, //text
            cv::Point(width/2, height / 2), //top-left position
            cv::FONT_HERSHEY_DUPLEX,
            1.0,
            CV_RGB(118, 185, 0), //font color
            2);

    // Display the image in a window
    cv::imshow("Random Color Window", image);

    // Wait indefinitely for a key press to close the window
    cv::waitKey(0);
    cv::destroyAllWindows();
}

int main() {
    // Example usage

    std::vector< std::pair< int, int>> resolutions = { \
                                                        {1920,1080}, \
                                                        {1680,1050}, \
                                                        {1280,1024}, \
                                                        {1440,900}, \ 
                                                        {1280,800}, \
                                                        {1280,720}, \
                                                        {1024,768}, \
                                                        {800,600}, \
                                                        {640,480}};
                                            
    for (auto &[width,height]:resolutions){
        createRandomColorWindow(width, height);
    }
    
    return 0;
}