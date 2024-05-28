#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <librdkafka/rdkafkacpp.h>

int main() 
{
    // Create a Kafka consumer instance
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    RdKafka::Conf::ConfResult result = conf->set("metadata.broker.list", "kafka_broker_ip:9092", errstr);
    RdKafka::KafkaConsumer *consumer = RdKafka::KafkaConsumer::create(conf, errstr);

    // Subscribe to the topic
    std::vector<std::string> topics = {"opencv_frames"};
    consumer->subscribe(topics);

    // Continuously consume messages from Kafka
    while (true) 
    {
        RdKafka::Message *msg = consumer->consume(1000);
        if (msg->err() == RdKafka::ERR_NO_ERROR) 
        {
            // Decode the received message as an OpenCV frame
            cv::Mat frame = cv::imdecode(cv::Mat(1, msg->len(), CV_8UC1, msg->payload()), cv::IMREAD_COLOR);

            // Display the frame
            cv::imshow("Received Frame", frame);
            cv::waitKey(1);
        } 
        else 
        {
            std::cerr << "Error consuming message: " << msg->errstr() << std::endl;
        }
        delete msg;
    }

    consumer->close();
    delete consumer;
    delete conf;

    return 0;
}
