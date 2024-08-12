#!/usr/bin/python3

import socket
import sys

def main():
    host = '127.0.0.1'  # Change this to your desired IP address
    port = 12345        # Change this to your desired port number

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind((host, port))
        server_socket.listen()

        print(f"Listening on {host}:{port}")

        try:
            while True:
                client_socket, client_address = server_socket.accept()
                print(f"Connection from {client_address}")

                with client_socket:
                    while True:
                        data = client_socket.recv(1024)
                        if not data:
                            print("Client Disconnected")
                            break;
                        send_data = b"Hello from Server: " + data
                        print(f"Received msg:{data}")
                        print(f"sending:{send_data}")
                        client_socket.sendall(send_data)
        except KeyboardInterrupt:
            print("\nServer shutting down")
            sys.exit(0)

if __name__ == "__main__":
    main()

