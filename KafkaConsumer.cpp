
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <cppkafka/cppkafka.h>

using namespace cv;
using namespace std;

int main() 
{
    // Create a Kafka consumer
    cppkafka::Configuration config = 
    {
        {"metadata.broker.list", "localhost:9092"},
        {"group.id", "my-group"},
        {"enable.auto.commit", false}
    };
    cppkafka::Consumer consumer(config);
    consumer.subscribe({"opencv-frames"});

    // Consume messages
    while (true) 
    {
        cppkafka::Message msg = consumer.poll();
        if (msg) 
        {
            // Decode the OpenCV frame from the message payload
            vector<uchar> data(msg.get_payload().get_data(),
                               msg.get_payload().get_data() + msg.get_payload().get_size());
            Mat frame = imdecode(data, IMREAD_COLOR);

            // Display the frame
            imshow("Frame", frame);
            if (waitKey(1) == 27) // Press 'ESC' to exit
            {  
                break;
            }
            // Commit the offset
            consumer.commit(msg);
        }
    }

    return 0;
}
