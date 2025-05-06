//zjd 2212

#pragma once
#include <stdexcept>
#include <string>
#include <limits> 

// �����˻��࣬��װ�˻��Ļ�������
class BankAccount {
private:
    double balance;          // �˻����
    bool m_isActive;        // �˻�״̬������/���ᣩ
    std::string accountNumber; // �˻����

    // ��������ȷ�Ƚϸ�������
    // ���� a, b: ���Ƚϵ�����������
    // ����ֵ: ��������ֵС����С������ֵ������Ϊ���
    bool isEqual(double a, double b) const {
        return std::abs(a - b) < std::numeric_limits<double>::epsilon();
    }

public:
    // Ĭ�Ϲ��캯������ʼ���˻�Ϊ����״̬���˺�Ϊ"DEFAULT"
    BankAccount();

    // ���˺Ų����Ĺ��캯������ʼ�����Ϊ0��״̬Ϊ����
    // ���� number: �˻���ţ�����ǿգ�
    explicit BankAccount(const std::string& number);

    // �����˻�
    void activate();

    // �����˻�
    void freeze();

    // ��ѯ�˻��Ƿ񼤻�
    // ����ֵ: true��ʾ���false��ʾ����
    bool isActive() const;

    // ��ȡ��ǰ���
    // ����ֵ: �˻����
    double getBalance() const;

    // ��ȡ�˻����
    // ����ֵ: �˻�����ַ���
    std::string getAccountNumber() const;

    // ������
    // ���� amount: ��������Ϊ�����ҷ�NaN��
    // �쳣: ���˻��������Ƿ����׳�std::runtime_error��std::invalid_argument
    void deposit(double amount);

    // ȡ�����
    // ���� amount: ȡ�������Ϊ�����Ҳ�������
    // ����ֵ: true��ʾȡ��ɹ���false��ʾʧ�ܣ����Ƿ������㣩
    bool withdraw(double amount);

    // ת�˲���
    // ���� to: Ŀ���˻�����
    // ���� amount: ת�˽��
    // ����ֵ: true��ʾת�˳ɹ���false��ʾʧ�ܣ�����ת�˸��Լ���
    bool transfer(BankAccount& to, double amount);
};