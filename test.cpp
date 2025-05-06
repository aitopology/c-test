#include "pch.h"
#include <gtest/gtest.h>

// 测试夹具类，初始化测试环境
class BankAccountTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 每次测试前确保账户处于激活状态并预存1000元
        account.activate();
        recipient.activate();
        account.deposit(1000.0);
    }

    // 测试账户定义
    BankAccount account{ "ACC001" };       // 主测试账户
    BankAccount recipient{ "ACC002" };     // 接收账户
    BankAccount inactiveAccount{ "INACTIVE" }; // 非活跃账户（用于测试冻结状态）
};

// 测试1：验证存款到冻结账户是否抛出异常
TEST_F(BankAccountTest, DepositWhenFrozen) {
    account.freeze();
    EXPECT_THROW(account.deposit(500.0), std::runtime_error);
}

// 测试2：验证自我转账应失败且余额不变
TEST_F(BankAccountTest, TransferToSelf) {
    EXPECT_FALSE(account.transfer(account, 500.0)); // 断言转账失败
    EXPECT_DOUBLE_EQ(1000.0, account.getBalance()); // 验证余额未变
}

// 测试3：验证转账到冻结账户时的回滚机制
TEST_F(BankAccountTest, TransferWithRollback) {
    inactiveAccount.freeze(); // 冻结接收账户
    EXPECT_FALSE(account.transfer(inactiveAccount, 500.0)); // 转账应失败
    EXPECT_DOUBLE_EQ(1000.0, account.getBalance()); // 验证转出账户余额恢复
}

// 测试4：验证存款NaN值是否抛出非法参数异常
TEST_F(BankAccountTest, DepositNaN) {
    double nanValue = std::numeric_limits<double>::quiet_NaN();
    EXPECT_THROW(account.deposit(nanValue), std::invalid_argument);
}

// 测试5：验证账户重新激活后能否正常操作
TEST_F(BankAccountTest, ReactivateAccount) {
    account.freeze();
    account.activate(); // 重新激活
    EXPECT_TRUE(account.withdraw(500.0)); // 应允许取款
    EXPECT_DOUBLE_EQ(500.0, account.getBalance()); // 验证余额正确
}

// 主函数：启动Google Test框架
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}