
#include <iostream>
#include <vector>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <rdkafkacpp.h>

using namespace cv;
using namespace std;

int main() 
{
    // Create a VideoCapture object to read frames from the camera
    VideoCapture cap(0);

    // Check if the camera is opened successfully
    if (!cap.isOpened()) 
    {
        cout << "Error opening camera" << endl;
        return -1;
    }

    // Create a Kafka producer
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    std::string errstr;
    RdKafka::Conf::ConfResult res = conf->set("bootstrap.servers", "localhost:9092", errstr);
    if (res != RdKafka::Conf::CONF_OK) 
    {
        cout << "Error setting Kafka configuration: " << errstr << endl;
        return -1;
    }

    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) 
    {
        cout << "Error creating Kafka producer: " << errstr << endl;
        return -1;
    }

    // Continuously read frames from the camera and send them to Kafka
    Mat frame;
    vector<uchar> buffer;
    while (true) 
    {
        // Capture a frame from the camera
        cap >> frame;

        // Check if the frame is empty
        if (frame.empty()) 
        {
            cout << "Error capturing frame" << endl;
            break;
        }

        // Encode the frame as a vector of bytes
        imencode(".jpg", frame, buffer);

        // Send the frame to Kafka
        RdKafka::ErrorCode err = producer->produce("opencv_frames", RdKafka::Topic::PARTITION_UA, RdKafka::Producer::msgflags::MSG_COPY, &buffer[0], buffer.size(), NULL, NULL);
        if (err != RdKafka::ERR_NO_ERROR) 
        {
            cout << "Error sending frame to Kafka: " << RdKafka::err2str(err) << endl;
        }

        // Clear the buffer for the next frame
        buffer.clear();

        // Wait for a key press to exit
        if (waitKey(1) == 27) 
        {
            break;
        }
    }

    // Clean up
    cap.release();
    delete producer;
    delete conf;

    return 0;
}
