import socket

HOST = '127.0.0.1'
PORT = 8080

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((HOST, PORT))
    s.listen(10)
    print(f"Server was started on http://{HOST}:{PORT}")
    conn, addr = s.accept()
    with conn:
        print(f"Get request from user {addr}")
        request_data = conn.recv(1024).decode('utf-8', errors='replace')
        print(f"Request: {request_data}")

        resp = "HTTP:/1.1 200 OK\r\n"
        resp += "Content-Type: text/html; charset=utf-8\r\n"
        resp += "\r\n<html><body><h1>Hello from py server</h1></body></html>"

        conn.sendall(resp.encode('utf-8'))
