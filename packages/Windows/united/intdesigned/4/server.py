import socket

# Sunucu için IP adresi ve port
LHOST = '0.0.0.0'
LPORT = 4444

def main():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((LHOST, LPORT))
    s.listen(1)
    
    print(f"Listening on {LHOST}:{LPORT}")
    conn, addr = s.accept()
    print(f"Connection from {addr}")
    
    while True:
        command = input("Shell> ")
        if command.lower() == 'exit':
            conn.send(b'exit')
            break
        conn.send(command.encode())
        response = conn.recv(4096)
        print(response.decode(), end="")
    
    conn.close()

if __name__ == "__main__":
    main()