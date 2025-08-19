
# Password Manager

A password manager for securely storing your passwords locally. This project was built with a strong focus on security. The password manager allows you to add, edit, view, and delete passwords. When adding a password, you must provide a service name, which is later used to edit or delete the corresponding entry.

The application also includes an option to auto-generate strong, secure passwords. The master password is stored in a database as a securely hashed value, using a hashing algorithm with a high number of iterations to slow down dictionary attacks. Additionally, the master password is validated to ensure it has sufficient entropy to resist brute-force attempts.

User passwords are stored in an SQLite database encrypted with the master password using ![SQLCipher](https://www.zetetic.net/sqlcipher/). SQLCipher is a fork of SQLite that provides AES-256 encryption to secure the database.


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

![sample](media/sample_run.gif)


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
	./passmgr -h
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
