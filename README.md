

```python
import socket

# Define the UDP IP address and port to listen on
UDP_IP = ""
UDP_PORT = 4444

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening for UDP packets on {UDP_IP}:{UDP_PORT}")

while True:
    # Receive data from the socket
    data, addr = sock.recvfrom(1024)
    print(f"received packet from {addr}: {data.decode('utf-8')}")
```
