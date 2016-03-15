#include <opencv2/opencv.hpp>
#include <iostream>
#include "application.h"

namespace blobspy {

void Application::send_blob_coordinates(const std::vector<cv::KeyPoint> &keypoints)
{
    if (keypoints.size() == 0)
    {
        return;
    }
    else
    {
        unsigned int biggest_index = 0;
        float biggest_size = 0.0f;
        for (unsigned int i = 0; i < keypoints.size(); i++)
        {
            float size = keypoints[i].size;
            if (size > biggest_size)
            {
                biggest_size = size;
                biggest_index = i;
            }
        }
        float x = keypoints[biggest_index].pt.x;
        float y = keypoints[biggest_index].pt.y;
        float size = keypoints[biggest_index].size;
        std::cout << "TODO Send /blob " << x << ", " << y << " " << size << std::endl;
        this->osc_interface->send_blob_position(x, y, size);
    }
}

Application::Application(Configuration& configuration)
{
    this->_configuration = configuration;
    this->osc_interface = new OscInterface(
            std::string("default"), // FIXME
            std::string("13333"), // FIXME
            std::string("127.0.0.1")); // FIXME
}

int Application::run()
{
    cv::VideoCapture cap(this->_configuration.video_device_id);
    if (! cap.isOpened())  // check if we succeeded
    {
        return 1;
    }

    cv::Mat edges;
    cv::SimpleBlobDetector detector;
    cv::namedWindow("Blobspy", 1);
    std::cout << "Press any key to quit" << std::endl;

    while (true)
    {
        cv::Mat frame;
        cap >> frame; // get a new frame from camera
        cv::cvtColor(frame, edges, CV_BGR2GRAY);
        cv::GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);

        // Detect blobs.
        std::vector<cv::KeyPoint> keypoints;
        detector.detect(edges, keypoints);
 
        // Draw detected blobs as red circles.
        // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the
        // circle corresponds to the size of blob
        cv::Mat im_with_keypoints;
        cv::drawKeypoints(edges, keypoints, im_with_keypoints,
                cv::Scalar(0, 0, 255),
                cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        send_blob_coordinates(keypoints);


        // Show blobs
        cv::imshow("Blobspy", im_with_keypoints);

        // cv::Canny(edges, edges, 0, 30, 3);
        // cv::imshow("Blobspy", edges);
        if (cv::waitKey(30) >= 0)
        {
            break;
        }
        // TODO: also stop if the window has been destroyed
    }
    std::cout << "Done" << std::endl;
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

} // end of namespace
