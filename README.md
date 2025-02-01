Brute Force Attack in C: Cracking Web Login Passwords

🔐 Overview

This project demonstrates a brute-force attack on a web login form using C and libcurl. The program systematically tests multiple passwords from a wordlist until it finds the correct one. Once successful, it saves the credentials in a text file and stops execution automatically.

⚠️ Disclaimer: This project is strictly for educational purposes. Unauthorized access to systems is illegal.

📌 Features

Reads passwords from a predefined wordlist (passwords.txt)

Automates login attempts using HTTP POST requests

Detects successful login based on the server response

Saves correct credentials to successful_login.txt

Stops execution once the correct password is found

🛠️ Requirements

Linux/macOS (or WSL on Windows)

GCC compiler

libcurl installed

Apache server with PHP support (for testing the login page)

🚀 Setup & Installation

1️⃣ Clone the Repository

git clone https://github.com/yourusername/brute-force-c.git
cd brute-force-c

2️⃣ Install Dependencies

Ensure libcurl is installed:

sudo apt install libcurl4-openssl-dev  # Ubuntu/Debian
yum install libcurl-devel             # CentOS/RHEL
brew install curl                     # macOS

3️⃣ Compile the Program

gcc -o brute_force brute_force.c -lcurl

4️⃣ Create a Password List (passwords.txt)

echo -e "123456\nqwerty\npassword123\nadmin\nwelcome" > passwords.txt

5️⃣ Run the Brute-Force Program

./brute_force

Once the correct password is found, it will be saved in successful_login.txt.

🌐 Running the Test Website

This project includes a simple HTML + PHP login form for local testing.

1️⃣ Start a Local Server

Ensure Apache and PHP are installed, then move the files to your web directory:

sudo cp -r website /var/www/html/
sudo systemctl restart apache2  # Start Apache on Linux

2️⃣ Access the Login Page

Open a browser and visit:

http://localhost/login.php

📝 License

This project is licensed under the MIT License - see the LICENSE file for details.

📢 Contributing

Feel free to fork the repository and submit pull requests to improve the project!

📎 Contact

For any questions, reach out via GitHub or LinkedIn.

