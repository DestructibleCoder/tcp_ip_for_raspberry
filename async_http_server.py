import asyncio
import socket

HOST = '127.0.0.1'
PORT = 8080


async def handler(conn):
    req_data = (await loop.sock_recv(conn, 1024)).decode('utf-8')
    print(f"Request: \n{req_data}")

    resp = "HTTP/1.1 200 OK\r\n"
    resp += "Content-Type: text/html; charset=utf-8\r\n\r\n"
    resp += "<html><body><h1>Hello, this is async http server</h1></body></html>"

    await loop.sock_sendall(conn, resp.encode('utf-8'))

    conn.close()


async def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((HOST, PORT))
        s.listen()
        print(f"Server started at http://{HOST}:{PORT}")
        s.setblocking(False)
        conn, addr = await loop.sock_accept(s)
        with conn:
            loop.create_task(handler(conn))

loop = asyncio.get_event_loop()
loop.run_until_complete(main())
