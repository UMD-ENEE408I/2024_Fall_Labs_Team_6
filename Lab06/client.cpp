#include <opencv2/opencv.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#define SERVER_IP "127.0.0.1"  // Replace with server IP
#define SERVER_PORT 5000

int main() {
    // Initialize socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Socket creation failed!" << std::endl;
        return -1;
    }

    // Define server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Connection to server failed!" << std::endl;
        close(client_socket);
        return -1;
    }

    // Open camera (default camera index 0)
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Failed to open camera!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (true) {
        // Capture frame from camera
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Failed to capture frame!" << std::endl;
            break;
        }

        // Encode frame as JPEG
        std::vector<uchar> encoded;
        cv::imencode(".jpg", frame, encoded);

        // Send frame size to the server
        std::string frame_size_str = std::to_string(encoded.size());
        frame_size_str.resize(8, ' ');
        send(client_socket, frame_size_str.c_str(), 8, 0);

        // Send the actual frame data to the server
        send(client_socket, encoded.data(), encoded.size(), 0);

        // Receive detection result from the server
        char buffer[1024] = {0};
        int bytes_received = recv(client_socket, buffer, 1024, 0);
        if (bytes_received > 0) {
            std::cout << "Detection Result: " << std::string(buffer, bytes_received) << std::endl;
        }

        // Break loop on pressing 'q'
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // Release resources
    cap.release();
    close(client_socket);
    return 0;
}
