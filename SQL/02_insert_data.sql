USE DigitalWallet;

INSERT INTO Users (name, email, phone, password, dob) VALUES
('Kajal Singh', 'kajal@gmail.com', '9876543210', 'pass123', '2004-05-15'),
('Rahul Sharma', 'rahul@gmail.com', '9876543211', 'rahul123', '2003-09-12'),
('Priya Verma', 'priya@gmail.com', '9876543212', 'priya123', '2002-11-20'),
('Amit Kumar', 'amit@gmail.com', '9876543213', 'amit123', '2001-07-18'),
('Sneha Patel', 'sneha@gmail.com', '9876543214', 'sneha123', '2004-03-25');

INSERT INTO Wallets (user_id, balance) VALUES
(1, 5000.00),
(2, 3000.00),
(3, 7000.00),
(4, 1500.00),
(5, 10000.00);

INSERT INTO Admins (username, password)
VALUES
('admin', 'admin123');

INSERT INTO BankAccounts
(user_id, bank_name, account_number, ifsc, account_holder_name)
VALUES
(1, 'State Bank of India', '12345678901', 'SBIN0001001', 'Kajal Singh'),
(2, 'HDFC Bank', '22345678901', 'HDFC0002002', 'Rahul Sharma'),
(3, 'ICICI Bank', '32345678901', 'ICIC0003003', 'Priya Verma'),
(4, 'Axis Bank', '42345678901', 'UTIB0004004', 'Amit Kumar'),
(5, 'Punjab National Bank', '52345678901', 'PUNB0005005', 'Sneha Patel');

INSERT INTO Transactions
(sender_wallet_id, receiver_wallet_id, amount, transaction_type, status)
VALUES

(1, 2, 500.00, 'Transfer', 'Success'),

(2, 3, 1000.00, 'Transfer', 'Success'),

(NULL, 1, 2000.00, 'Add Money', 'Success'),

(3, NULL, 700.00, 'Withdraw', 'Success'),

(5, 4, 1500.00, 'Transfer', 'Success');


INSERT INTO Cashback
(transaction_id, cashback_amount)
VALUES
(2, 50.00),
(5, 75.00);

INSERT INTO Notifications
(user_id, message)
VALUES
(1, '₹2000 added successfully.'),
(2, '₹500 received from Kajal Singh.'),
(3, '₹1000 received from Rahul Sharma.'),
(4, '₹1500 received from Sneha Patel.'),
(5, '₹75 cashback credited.');

INSERT INTO LoginHistory
(user_id, login_status, ip_address)
VALUES
(1, 'Success', '192.168.1.10'),
(2, 'Success', '192.168.1.11'),
(3, 'Failed', '192.168.1.12'),
(4, 'Success', '192.168.1.13'),
(5, 'Failed', '192.168.1.14');

