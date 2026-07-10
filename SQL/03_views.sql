USE DigitalWallet;


CREATE VIEW UserWalletDetails AS
SELECT
    u.user_id,
    u.name,
    u.email,
    u.phone,
    w.wallet_id,
    w.balance,
    w.wallet_status
FROM Users u
JOIN Wallets w
ON u.user_id = w.user_id;

CREATE VIEW TransactionDetails AS
SELECT
    t.transaction_id,
    su.name AS sender_name,
    ru.name AS receiver_name,
    t.amount,
    t.transaction_type,
    t.status,
    t.transaction_time
FROM Transactions t
LEFT JOIN Wallets sw
ON t.sender_wallet_id = sw.wallet_id
LEFT JOIN Users su
ON sw.user_id = su.user_id
LEFT JOIN Wallets rw
ON t.receiver_wallet_id = rw.wallet_id
LEFT JOIN Users ru
ON rw.user_id = ru.user_id;

CREATE VIEW ActiveUsers AS
SELECT *
FROM Users
WHERE status = 'Active';

CREATE VIEW BlockedUsers AS
SELECT *
FROM Users
WHERE status = 'Blocked';

CREATE VIEW CashbackDetails AS
SELECT
    c.cashback_id,
    c.cashback_amount,
    c.cashback_date,
    t.transaction_id,
    t.amount AS transaction_amount
FROM Cashback c
JOIN Transactions t
ON c.transaction_id = t.transaction_id;