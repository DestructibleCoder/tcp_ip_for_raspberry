import socket

HOST = '127.0.0.1'
PORT = 8080

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen(1)
    conn, addr = s.accept()
    with conn:
        print(f'Connected by {addr}')
        conn.send(b'Welcome')
        while True:
            with open(".log", "a+") as file:
                data = conn.recv(4096)
                if not data:
                    break
                file.write(str(data))
                conn.send(data)

