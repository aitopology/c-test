// BankAccountTests.cpp
#include "pch.h"
#include <gtest/gtest.h>
#include <limits>

class BankAccountTest : public ::testing::Test {
protected:
    void SetUp() override {
        // ÿ����������ǰ�����˻�״̬
        account.activate();
        recipient.activate();
        account.deposit(1000.0); // ��ʼ���1000Ԫ
    }

    BankAccount account{ "ACC001" };
    BankAccount recipient{ "ACC002" };
    BankAccount inactiveAccount{ "INACTIVE" };
};

// ================== �ںв��� ==================

// �ȼ��໮�ֲ��ԣ���������
TEST_F(BankAccountTest, DepositValidAmount) {
    account.deposit(500.0);
    EXPECT_DOUBLE_EQ(1500.0, account.getBalance());
}
TEST_F(BankAccountTest, DepositZeroAmount) {
    EXPECT_THROW(account.deposit(0.0), std::invalid_argument);
}
TEST_F(BankAccountTest, DepositNaN) {
    EXPECT_THROW(account.deposit(std::nan("")), std::invalid_argument);
}

// �߽�ֵ�������ԣ�ȡ�������
TEST_F(BankAccountTest, Withdraw_ExactBalance) {
    // �߽�ֵ��ȡ����������
    EXPECT_TRUE(account.withdraw(1000.0));
    EXPECT_DOUBLE_EQ(0.0, account.getBalance());
}

TEST_F(BankAccountTest, Withdraw_OneCentOverBalance) {
    // �߽�ֵ��ȡ��������0.01Ԫ
    EXPECT_FALSE(account.withdraw(1000.01));
    EXPECT_DOUBLE_EQ(1000.0, account.getBalance());
}

// �ж������ԣ�ת�˲�����
TEST_F(BankAccountTest, TransferDecisionTable) {
    // ���1: �ɹ�ת��
    recipient.activate();
    EXPECT_TRUE(account.transfer(recipient, 500.0));

    // ���2: ����
    EXPECT_FALSE(account.transfer(recipient, 1500.0));

    // ���3: ת�뷽����
    recipient.freeze();
    EXPECT_FALSE(account.transfer(recipient, 500.0));

    // ���4: ת��������
    account.freeze();
    EXPECT_FALSE(account.transfer(recipient, 500.0));
}

// ���ͼ�����ԣ�ȡ�������
TEST_F(BankAccountTest, Withdraw_CausalGraphing) {
    // ����1: ������������ (C1=T, C2=T, C3=T)
    EXPECT_TRUE(account.withdraw(500.0));
    EXPECT_DOUBLE_EQ(500.0, account.getBalance());

    // ����2: �˻����� (C1=F)
    account.freeze();
    EXPECT_FALSE(account.withdraw(500.0));
    account.activate();

    // ����3: �����Ч (C2=F)
    EXPECT_FALSE(account.withdraw(-100.0));

    // ����4: ������� (C3=F)
    EXPECT_FALSE(account.withdraw(1500.0));
}

// ================== �׺в��� ==================

// ��串�ǲ���
TEST_F(BankAccountTest, Deposit_StatementCoverage) {
    try {
        // �����׳�invalid_argument���
        account.deposit(-100.0);
    }
    catch (const std::invalid_argument&) {
        // Ԥ�ڲ����쳣
    }
    catch (...) {
        FAIL() << "Unexpected exception type";
    }
}

// ��֧���ǲ���
TEST_F(BankAccountTest, Withdraw_BranchCoverage) {
    // ��֧1���˻�����
    account.freeze();
    EXPECT_FALSE(account.withdraw(100.0));

    // ��֧2�������Ч
    account.activate();
    EXPECT_FALSE(account.withdraw(-50.0));

    // ��֧3������
    EXPECT_FALSE(account.withdraw(1500.0));

    // ��֧4��������������
    EXPECT_TRUE(account.withdraw(500.0));
}

// ����·�����ǲ��ԣ�ת�˲�����
TEST_F(BankAccountTest, Transfer_IndependentPaths) {
    // ·��1������ת��
    EXPECT_FALSE(account.transfer(account, 500.0));

    // ·��2��ȡ��ʧ��
    EXPECT_FALSE(account.transfer(recipient, 1500.0));

    // ·��3�����ʧ�ܻع�
    recipient.freeze();
    EXPECT_FALSE(account.transfer(recipient, 500.0));
    EXPECT_DOUBLE_EQ(1000.0, account.getBalance());

    // ·��4�������ɹ�·��
    recipient.activate();
    EXPECT_TRUE(account.transfer(recipient, 500.0));
}

// �������ǲ��ԣ���������
TEST_F(BankAccountTest, Deposit_ConditionCoverage) {
    // ����1��!m_isActive
    account.freeze();
    EXPECT_THROW(account.deposit(500.0), std::runtime_error);

    // ����2��amount <= 0
    account.activate();
    EXPECT_THROW(account.deposit(-100.0), std::invalid_argument);

    // ����3��isNaN(amount)
    EXPECT_THROW(account.deposit(std::numeric_limits<double>::quiet_NaN()),
        std::invalid_argument);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*#include "pch.h"
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
*/