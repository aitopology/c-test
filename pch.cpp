#include "pch.h"

// Ĭ�Ϲ��캯��
BankAccount::BankAccount()
    : balance(0.0), m_isActive(true), accountNumber("DEFAULT") {
}

// ���˺Ų����Ĺ��캯��
BankAccount::BankAccount(const std::string& number)
    : balance(0.0), m_isActive(true), accountNumber(number) {
}

// �����˻������������
void BankAccount::activate() {
    m_isActive = true;
}

// �����˻�����ֹ������
void BankAccount::freeze() {
    m_isActive = false;
}

// ��ѯ�˻�״̬
bool BankAccount::isActive() const {
    return m_isActive;
}

// ��ȡ���
double BankAccount::getBalance() const {
    return balance;
}

// ��ȡ�˻����
std::string BankAccount::getAccountNumber() const {
    return accountNumber;
}

// ���ʵ��
void BankAccount::deposit(double amount) {
    // ����˻�״̬
    if (!m_isActive)
        throw std::runtime_error("Account frozen");

    // �������Ч�ԣ������ҷ�NaN
    if (amount <= 0 || !isEqual(amount, amount))  // ��amountΪNaN��isEqual����false
        throw std::invalid_argument("Invalid deposit amount");

    balance += amount;
}

// ȡ��ʵ��
bool BankAccount::withdraw(double amount) {
    // ����˻�״̬�ͽ����Ч��
    if (!m_isActive || amount <= 0 || !isEqual(amount, amount))
        return false;

    // �������Ƿ��㹻
    if (amount > balance)
        return false;

    balance -= amount;
    return true;
}

// ת��ʵ��
bool BankAccount::transfer(BankAccount& to, double amount) {
    // ��ֹ����ת��
    if (this == &to)
        return false;

    // ���Դӵ�ǰ�˻��ۿ�
    if (!withdraw(amount))
        return false;

    try {
        // ������Ŀ���˻����
        to.deposit(amount);
        return true;
    }
    catch (...) {
        // ���ʧ�ܣ��ع���ǰ�˻��Ŀۿ����
        deposit(amount);
        return false;
    }
}