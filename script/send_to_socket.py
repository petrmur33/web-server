import socket
import sys
from typing import List


server_address = ('127.0.0.1', 80)


def main(args: List[str]) -> None:
    with open("input.dat", "rb") as file:
        send_buffer = file.read()

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    sock.connect(server_address)
    sock.sendall(send_buffer)

    sock.close()


if __name__ == "__main__":
    main(sys.argv)
