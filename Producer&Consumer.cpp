#include <iostream>
#include <thread>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cppkafka/cppkafka.h>

using namespace cv;
using namespace std;

// Kafka configuration
const string broker = "localhost:9092";
const string topic = "opencv_frames";

// OpenCV frame publisher
void publishFrames(cppkafka::Producer& producer) 
{
    VideoCapture cap(0);  // Open default camera

    if (!cap.isOpened()) 
    {
        cerr << "Failed to open camera" << endl;
        return;
    }

    Mat frame;
    vector<uchar> buffer;

    while (true) 
    {
        cap >> frame;  // Capture frame
        if (frame.empty()) 
        {
            cerr << "Failed to capture frame" << endl;
            break;
        }
        imencode(".jpg", frame, buffer);  // Encode frame as JPEG
        // Send frame to Kafka
        producer.produce(cppkafka::MessageBuilder(topic).payload(buffer.data(), buffer.size()));
        imshow("Publisher", frame);
        if (waitKey(1) == 27) 
          break;  // Press 'Esc' to exit
    }
    cap.release();
    destroyAllWindows();
}

// OpenCV frame subscriber
void consumeFrames(cppkafka::Consumer& consumer) 
{
    namedWindow("Subscriber", WINDOW_NORMAL);

    while (true) 
    {
        // Consume message from Kafka
        cppkafka::Message msg = consumer.poll();

        if (msg) 
        {
            // Decode JPEG data to OpenCV Mat
            vector<uchar> buffer(msg.get_payload().get_data(), msg.get_payload().get_data() + msg.get_payload().get_size());
            Mat frame = imdecode(buffer, IMREAD_COLOR);
            if (!frame.empty()) 
            {
                imshow("Subscriber", frame);
                if (waitKey(1) == 27) break;  // Press 'Esc' to exit
            }
        }
    }
    destroyAllWindows();
}

int main() 
{
    // Create Kafka producer and consumer
    cppkafka::Configuration config = {{"metadata.broker.list", broker}};
    cppkafka::Producer producer(config);
    cppkafka::Consumer consumer(config, {topic});

    // Start publisher and subscriber threads
    thread publisherThread(publishFrames, ref(producer));
    thread subscriberThread(consumeFrames, ref(consumer));

    publisherThread.join();
    subscriberThread.join();

    return 0;
}
