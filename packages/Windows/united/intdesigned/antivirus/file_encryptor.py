from cryptography.fernet import Fernet

# Anahtar oluşturma
def generate_key():
    key = Fernet.generate_key()
    with open("secret.key", "wb") as key_file:
        key_file.write(key)

# Anahtar yükleme
def load_key():
    return open("secret.key", "rb").read()

# Dosya şifreleme
def encrypt_file(file_name):
    key = load_key()
    f = Fernet(key)
    with open(file_name, "rb") as file:
        file_data = file.read()
    encrypted_data = f.encrypt(file_data)
    with open(file_name, "wb") as file:
        file.write(encrypted_data)

# Dosya çözme
def decrypt_file(file_name):
    key = load_key()
    f = Fernet(key)
    with open(file_name, "rb") as file:
        encrypted_data = file.read()
    decrypted_data = f.decrypt(encrypted_data)
    with open(file_name, "wb") as file:
        file.write(decrypted_data)

if __name__ == "__main__":
    action = input("Enter 'generate' to create a key, 'encrypt' to encrypt a file, 'decrypt' to decrypt a file: ").strip()
    if action == "generate":
        generate_key()
        print("Key generated and saved as 'secret.key'.")
    elif action == "encrypt":
        filename = input("Enter the file name to encrypt: ").strip()
        encrypt_file(filename)
        print(f"{filename} has been encrypted.")
    elif action == "decrypt":
        filename = input("Enter the file name to decrypt: ").strip()
        decrypt_file(filename)
        print(f"{filename} has been decrypted.")
    else:
        print("Invalid action.")