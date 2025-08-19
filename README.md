
# Password Manager

Password Manager is a secure, command-line application written in C for managing your credentials. It provides strong encryption for your password database using SQLCipher and OpenSSL, and ensures your master password is checked for strength and securely stored. The app is designed for simplicity, security, and portability, making it ideal for users who want a lightweight, offline password manager.

!()
## General Overview
After launching the app, you will be prompted to set or enter your master password. Once authenticated, you can store, retrieve, update, and delete passwords for various services. All sensitive data is encrypted and only accessible after successful authentication.

## Main Functions
- **Set Master Password:** On first use, create a strong master password to protect your data.
- **Verify Master Password:** Authenticate yourself before accessing the password database.
- **Add Password:** Store a new password for a service.
- **Retrieve Password:** Look up a password for a specific service.
- **Update Password:** Change an existing password for a service.
- **Delete Password:** Remove a password entry from the database.
- **Password Entropy Check:** Ensure your master password meets minimum strength requirements.
- **Secure Storage:** All passwords and master credentials are encrypted using SQLCipher.


## Features
- Master password authentication
- Password entropy check
- Encrypted password database (SQLCipher)
- Secure password storage and retrieval
- Simple command-line interface


## Requirements
- GCC (or compatible C compiler)
- [SQLCipher](https://www.zetetic.net/sqlcipher/) (`libsqlcipher-dev`)
- [OpenSSL](https://www.openssl.org/) (`libssl-dev`)

## Build Instructions
1. Install dependencies:
	```sh
	sudo apt-get install libsqlcipher-dev libssl-dev
	```
2. Build the project:
	```sh
	make
	```
	Or manually:
	```sh
	gcc -Wall -Wextra -Iinclude -o passmgr src/*.c -lsqlcipher -lssl -lcrypto -lm
	```

## Usage
1. Run the password manager:
	```sh
	./passmgr
	```
2. On first run, set your master password.
3. Use the CLI to add, retrieve, and manage passwords securely.

## File Structure
- `src/` — Source code
- `include/` — Header files
- `data/` — Encrypted databases (`master_pass.db`, `passwords.db`)
- `Makefile` — Build instructions

## Security Notes
- All passwords are stored encrypted using SQLCipher.
- The master password is checked for entropy and securely hashed.
- Database files are only accessible after successful authentication.

## License
MIT
