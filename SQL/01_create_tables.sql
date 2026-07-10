USE DigitalWallet;
CREATE TABLE Users (

    user_id INT AUTO_INCREMENT PRIMARY KEY,

    name VARCHAR(100) NOT NULL,

    email VARCHAR(100) NOT NULL UNIQUE,

    phone VARCHAR(15) NOT NULL UNIQUE,

    password VARCHAR(255) NOT NULL,

    dob DATE,

    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    status ENUM('Active','Blocked') DEFAULT 'Active'

);

CREATE TABLE Wallets (

    wallet_id INT AUTO_INCREMENT PRIMARY KEY,

    user_id INT UNIQUE NOT NULL,

    balance DECIMAL(12,2) DEFAULT 0.00,

    wallet_status ENUM('Active','Blocked')
                   DEFAULT 'Active',

    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY(user_id)
    REFERENCES Users(user_id)

);

CREATE TABLE Transactions (

    transaction_id INT AUTO_INCREMENT PRIMARY KEY,

    sender_wallet_id INT NULL,

    receiver_wallet_id INT NULL,

    amount DECIMAL(12,2) NOT NULL,

    transaction_type ENUM('Transfer', 'Add Money', 'Withdraw') NOT NULL,

    status ENUM('Success', 'Failed', 'Pending') DEFAULT 'Pending',

    transaction_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY(sender_wallet_id) REFERENCES Wallets(wallet_id),

    FOREIGN KEY(receiver_wallet_id) REFERENCES Wallets(wallet_id)

);

CREATE TABLE BankAccounts (

    bank_id INT AUTO_INCREMENT PRIMARY KEY,

    user_id INT NOT NULL,

    bank_name VARCHAR(100) NOT NULL,

    account_number VARCHAR(30) NOT NULL UNIQUE,

    ifsc VARCHAR(20) NOT NULL,

    account_holder_name VARCHAR(100) NOT NULL,

    FOREIGN KEY(user_id) REFERENCES Users(user_id)

);

CREATE TABLE Notifications (

    notification_id INT AUTO_INCREMENT PRIMARY KEY,

    user_id INT NOT NULL,

    message VARCHAR(255) NOT NULL,

    notification_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    status ENUM('Unread', 'Read') DEFAULT 'Unread',

    FOREIGN KEY(user_id) REFERENCES Users(user_id)

);

CREATE TABLE Cashback (

    cashback_id INT AUTO_INCREMENT PRIMARY KEY,

    transaction_id INT NOT NULL,

    cashback_amount DECIMAL(10,2) NOT NULL,

    cashback_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY(transaction_id) REFERENCES Transactions(transaction_id)

);

CREATE TABLE LoginHistory (

    login_id INT AUTO_INCREMENT PRIMARY KEY,

    user_id INT NOT NULL,

    login_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    logout_time TIMESTAMP NULL,

    login_status ENUM('Success', 'Failed') NOT NULL,

    ip_address VARCHAR(45),

    FOREIGN KEY(user_id) REFERENCES Users(user_id)

);

CREATE TABLE Admins (

    admin_id INT AUTO_INCREMENT PRIMARY KEY,

    username VARCHAR(50) NOT NULL UNIQUE,

    password VARCHAR(255) NOT NULL

);