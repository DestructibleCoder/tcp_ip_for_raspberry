import socket

def is_client_connection_closed(msg: str) -> bool:
    for i in msg:
        if i == '#':
            return True
    return False

HOST = "127.0.0.1"
PORT = 8080

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((HOST, PORT))
sock.listen(1)
conn, addr = sock.accept()

print("Client connected")
conn.send(b"Welcome to server")

while True:
    data = conn.recv(4096)
    print(data.decode())
    if is_client_connection_closed(data.decode()):
        break
    conn.send(data.upper())

conn.close()
