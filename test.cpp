#include "pch.h"
#include <gtest/gtest.h>

// ���Լо��࣬��ʼ�����Ի���
class BankAccountTest : public ::testing::Test {
protected:
    void SetUp() override {
        // ÿ�β���ǰȷ���˻����ڼ���״̬��Ԥ��1000Ԫ
        account.activate();
        recipient.activate();
        account.deposit(1000.0);
    }

    // �����˻�����
    BankAccount account{ "ACC001" };       // �������˻�
    BankAccount recipient{ "ACC002" };     // �����˻�
    BankAccount inactiveAccount{ "INACTIVE" }; // �ǻ�Ծ�˻������ڲ��Զ���״̬��
};

// ����1����֤�������˻��Ƿ��׳��쳣
TEST_F(BankAccountTest, DepositWhenFrozen) {
    account.freeze();
    EXPECT_THROW(account.deposit(500.0), std::runtime_error);
}

// ����2����֤����ת��Ӧʧ��������
TEST_F(BankAccountTest, TransferToSelf) {
    EXPECT_FALSE(account.transfer(account, 500.0)); // ����ת��ʧ��
    EXPECT_DOUBLE_EQ(1000.0, account.getBalance()); // ��֤���δ��
}

// ����3����֤ת�˵������˻�ʱ�Ļع�����
TEST_F(BankAccountTest, TransferWithRollback) {
    inactiveAccount.freeze(); // ��������˻�
    EXPECT_FALSE(account.transfer(inactiveAccount, 500.0)); // ת��Ӧʧ��
    EXPECT_DOUBLE_EQ(1000.0, account.getBalance()); // ��֤ת���˻����ָ�
}

// ����4����֤���NaNֵ�Ƿ��׳��Ƿ������쳣
TEST_F(BankAccountTest, DepositNaN) {
    double nanValue = std::numeric_limits<double>::quiet_NaN();
    EXPECT_THROW(account.deposit(nanValue), std::invalid_argument);
}

// ����5����֤�˻����¼�����ܷ���������
TEST_F(BankAccountTest, ReactivateAccount) {
    account.freeze();
    account.activate(); // ���¼���
    EXPECT_TRUE(account.withdraw(500.0)); // Ӧ����ȡ��
    EXPECT_DOUBLE_EQ(500.0, account.getBalance()); // ��֤�����ȷ
}

// ������������Google Test���
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}