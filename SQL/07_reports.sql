

-- =====================================================
-- REPORT 1 : User Wallet Details
-- Description: Displays user information along with wallet details.
-- =====================================================

SELECT
    u.user_id,
    u.name,
    u.email,
    w.wallet_id,
    w.balance,
    w.wallet_status
FROM Users u
JOIN Wallets w
ON u.user_id = w.user_id;


-- =====================================================
-- REPORT 2 : Top 5 Users by Wallet Balance
-- Description: Displays users having the highest wallet balance.
-- =====================================================

SELECT
    u.user_id,
    u.name,
    w.balance
FROM Users u
JOIN Wallets w
ON u.user_id = w.user_id
ORDER BY w.balance DESC
LIMIT 5;


-- =====================================================
-- REPORT 3 : Total Money Available in Wallet System
-- Description: Calculates the total balance across all wallets.
-- =====================================================

SELECT
    SUM(balance) AS Total_Wallet_Balance
FROM Wallets;


-- =====================================================
-- REPORT 4 : Total Number of Transactions
-- Description: Displays the total transactions performed.
-- =====================================================

SELECT
    COUNT(*) AS Total_Transactions
FROM Transactions;


-- =====================================================
-- REPORT 5 : Transactions by Type
-- Description: Counts transactions based on their type.
-- =====================================================

SELECT
    transaction_type,
    COUNT(*) AS Total
FROM Transactions
GROUP BY transaction_type;


-- =====================================================
-- REPORT 6 : Transaction Status Summary
-- Description: Shows successful, failed and pending transactions.
-- =====================================================

SELECT
    status,
    COUNT(*) AS Total
FROM Transactions
GROUP BY status;


-- =====================================================
-- REPORT 7 : Total Cashback Distributed
-- Description: Calculates total cashback given to users.
-- =====================================================

SELECT
    SUM(cashback_amount) AS Total_Cashback
FROM Cashback;


-- =====================================================
-- REPORT 8 : Blocked Users
-- Description: Displays all blocked users.
-- =====================================================

SELECT
    user_id,
    name,
    email
FROM Users
WHERE status = 'Blocked';


-- =====================================================
-- REPORT 9 : Recent 10 Transactions
-- Description: Displays the latest 10 transactions.
-- =====================================================

SELECT *
FROM Transactions
ORDER BY transaction_time DESC
LIMIT 10;