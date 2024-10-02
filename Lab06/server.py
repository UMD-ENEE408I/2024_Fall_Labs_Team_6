import socket
import cv2
import numpy as np
from ultralytics import YOLO

# Server address and port
SERVER_IP = '0.0.0.0'
SERVER_PORT = 5000

# Load YOLOv8 model
model = YOLO('yolov8n.pt')  # Load YOLOv8 nano version

# Initialize the server
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((SERVER_IP, SERVER_PORT))
server_socket.listen(1)
print("Server listening on port", SERVER_PORT)

def receive_frame(conn):
    # Receive frame size
    frame_size = int(conn.recv(8).decode())

    # Receive frame bytes
    data = b""
    while len(data) < frame_size:
        packet = conn.recv(frame_size - len(data))
        if not packet:
            return None
        data += packet

    # Convert to numpy array and decode image
    frame = np.frombuffer(data, dtype=np.uint8)
    frame = cv2.imdecode(frame, cv2.IMREAD_COLOR)
    return frame

def send_result(conn, result):
    conn.sendall(result.encode())

while True:
    # Wait for a connection
    conn, addr = server_socket.accept()
    print(f"Connection from {addr}")

    try:
        while True:
            # Receive a frame from the client
            frame = receive_frame(conn)
            if frame is None:
                break

            # Run YOLOv8 object detection
            results = model(frame)

            # Annotate the frame with results
            annotated_frame = results[0].plot()

            # Send detection results back to client (as text for simplicity)
            detection_info = str(results[0].boxes)
            send_result(conn, detection_info)

            # Optionally display frame on the server
            cv2.imshow('YOLOv8 Detection', annotated_frame)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

    finally:
        conn.close()
        cv2.destroyAllWindows()

