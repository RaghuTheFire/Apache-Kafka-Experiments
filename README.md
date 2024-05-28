# Apache-Kafka-Experiments

## KafkaProducer.cpp
This C++ code is designed to capture frames from a camera and send them to a Kafka topic. Here's a breakdown of what the code does: 
1. The necessary headers are included, such as `iostream`, `vector`, `cstring`, `opencv2/opencv.hpp`, and `rdkafkacpp.h`.
2. The `main` function is the entry point of the program.
3. An instance of `VideoCapture` is created to access the camera (in this case, the default camera with index 0).
4. The program checks if the camera is opened successfully. If not, it prints an error message and exits.
5. A Kafka producer is created by configuring the Kafka broker address (`localhost:9092` in this case).
6. If the Kafka configuration is successful, a `RdKafka::Producer` instance is created.
7. The program enters a loop where it continuously captures frames from the camera.
8. Each captured frame is checked for emptiness. If the frame is empty, an error message is printed, and the loop is broken.
9. The captured frame is encoded as a vector of bytes using the OpenCV `imencode` function with the JPEG format.
10. The encoded frame is sent to a Kafka topic named "opencv_frames" using the `produce` method of the Kafka producer.
11. If there is an error sending the frame to Kafka, an error message is printed.
12. The buffer used for encoding the frame is cleared for the next iteration.
13. The program waits for the user to press the Esc key (ASCII code 27) to exit the loop.
14. After the loop ends, the camera is released, and the Kafka producer and configuration objects are deleted.
15. The program exits with a return code of 0. In summary, this code continuously captures frames from a camera, encodes them as JPEG images, and sends them to a Kafka topic named "opencv_frames". It uses the OpenCV library for camera access and image encoding, and the RdKafka C++ library for interacting with the Kafka broker.
    
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

## Producer&Consumer.cpp

This C++ code demonstrates a simple video streaming application using OpenCV and Apache Kafka. It consists of two main components: a publisher and a subscriber. The publisher captures frames from the default camera and sends them to a Kafka topic, while the subscriber consumes frames from the same Kafka topic and displays them in a window. Here's a breakdown of the code: 
1. The necessary headers are included, such as `iostream`, `thread`, `vector`, `opencv2/opencv.hpp`, and `cppkafka/cppkafka.h`. 2. The Kafka broker address and topic name are defined as constants.
3. The `publishFrames` function is responsible for capturing frames from the default camera, encoding them as JPEG, and sending them to the Kafka topic using the `cppkafka::Producer`.
4. The `consumeFrames` function is responsible for consuming messages from the Kafka topic using the `cppkafka::Consumer`, decoding the JPEG data into an OpenCV `Mat` object, and displaying the frames in a window.
5. In the `main` function:
- A Kafka producer and consumer are created with the specified configuration.
- Two threads are created: one for the publisher (`publisherThread`) and one for the subscriber (`subscriberThread`).
- The `publisherThread` calls the `publishFrames` function, passing the Kafka producer as a reference.
- The `subscriberThread` calls the `consumeFrames` function, passing the Kafka consumer as a reference.
6. The main thread waits for both the publisher and subscriber threads to finish by calling `join()` on each thread. The publisher thread continuously captures frames from the camera, encodes them as JPEG, and sends them to the Kafka topic. The subscriber thread continuously consumes messages from the Kafka topic, decodes the JPEG data into OpenCV `Mat` objects, and displays the frames in a window. Both the publisher and subscriber windows can be closed by pressing the 'Esc' key. This code demonstrates the integration of OpenCV for video capture and display, and Apache Kafka for messaging and data streaming between the publisher and subscriber components.
