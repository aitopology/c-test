#include "pch.h"

// 默认构造函数
BankAccount::BankAccount()
    : balance(0.0), m_isActive(true), accountNumber("DEFAULT") {
}

// 带账号参数的构造函数
BankAccount::BankAccount(const std::string& number)
    : balance(0.0), m_isActive(true), accountNumber(number) {
}

// 激活账户（允许操作）
void BankAccount::activate() {
    m_isActive = true;
}

// 冻结账户（禁止操作）
void BankAccount::freeze() {
    m_isActive = false;
}

// 查询账户状态
bool BankAccount::isActive() const {
    return m_isActive;
}

// 获取余额
double BankAccount::getBalance() const {
    return balance;
}

// 获取账户编号
std::string BankAccount::getAccountNumber() const {
    return accountNumber;
}

// 存款实现
void BankAccount::deposit(double amount) {
    // 检查账户状态
    if (!m_isActive)
        throw std::runtime_error("Account frozen");

    // 检查金额有效性：正数且非NaN
    if (amount <= 0 || !isEqual(amount, amount))  // 若amount为NaN，isEqual返回false
        throw std::invalid_argument("Invalid deposit amount");

    balance += amount;
}

// 取款实现
bool BankAccount::withdraw(double amount) {
    // 检查账户状态和金额有效性
    if (!m_isActive || amount <= 0 || !isEqual(amount, amount))
        return false;

    // 检查余额是否足够
    if (amount > balance)
        return false;

    balance -= amount;
    return true;
}

// 转账实现
bool BankAccount::transfer(BankAccount& to, double amount) {
    // 禁止自我转账
    if (this == &to)
        return false;

    // 尝试从当前账户扣款
    if (!withdraw(amount))
        return false;

    try {
        // 尝试向目标账户存款
        to.deposit(amount);
        return true;
    }
    catch (...) {
        // 存款失败，回滚当前账户的扣款操作
        deposit(amount);
        return false;
    }
}