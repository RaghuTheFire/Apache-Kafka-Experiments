# Apache-Kafka-Experiments

## KafkaConsumer.cpp


## KafkaConsumer.cpp
This C++ code is designed to consume video frames from a Kafka topic named "opencv_frames" and display them using the OpenCV library. Here's a breakdown of what the code does: 
1. The necessary headers are included: `opencv2/opencv.hpp` (main OpenCV library), `opencv2/imgcodecs.hpp` (for image/video encoding and decoding), `opencv2/highgui.hpp` (for GUI functionality), and `librdkafka/rdkafkacpp.hpp` (for Kafka C++ client library).
2. In the `main` function, a Kafka consumer instance is created using the `RdKafka::KafkaConsumer` class from the Kafka C++ library. - A configuration object (`RdKafka::Conf`) is created and set with the Kafka broker IP address and port (`"kafka_broker_ip:9092"`). - The consumer instance is created using the configuration object.
3. The consumer subscribes to the "opencv_frames" topic using the `consumer->subscribe` method.
4. The code enters an infinite loop (`while (true)`) to continuously consume messages from the Kafka topic.
- The `consumer->consume` method is called with a timeout of 1000 milliseconds (1 second) to retrieve a message from the topic.
- If a message is successfully received (`msg->err() == RdKafka::ERR_NO_ERROR`), the following steps are performed:
- The message payload is decoded as an OpenCV image using `cv::imdecode`. The payload is assumed to be a compressed image data.
- The decoded image frame is displayed using `cv::imshow` with the window title "Received Frame".
- `cv::waitKey(1)` is called to allow the window to refresh and process any user input (with a delay of 1 millisecond).
- If there is an error consuming the message, an error message is printed to the standard error stream (`std::cerr`).
- The message object is deleted using `delete msg` to free the memory.
5. After the infinite loop, the consumer is closed using `consumer->close()`, and the consumer, configuration objects are deleted using `delete` to free the memory.
6. The program exits with a return code of 0. In summary, this code sets up a Kafka consumer to receive video frames from the "opencv_frames" topic, decodes the received frames using OpenCV, and displays them in a window. It continuously listens for new messages and processes them as they arrive.
