// BankAccountTests.cpp
#include "pch.h"
#include <gtest/gtest.h>
#include <limits>

class BankAccountTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 每个测试用例前重置账户状态
        account.activate();
        recipient.activate();
        account.deposit(1000.0); // 初始余额1000元
    }

    BankAccount account{ "ACC001" };
    BankAccount recipient{ "ACC002" };
    BankAccount inactiveAccount{ "INACTIVE" };
};

// ================== 黑盒测试 ==================

// 等价类划分测试（存款操作）
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

// 边界值分析测试（取款操作）
TEST_F(BankAccountTest, Withdraw_ExactBalance) {
    // 边界值：取款金额等于余额
    EXPECT_TRUE(account.withdraw(1000.0));
    EXPECT_DOUBLE_EQ(0.0, account.getBalance());
}

TEST_F(BankAccountTest, Withdraw_OneCentOverBalance) {
    // 边界值：取款金额超过余额0.01元
    EXPECT_FALSE(account.withdraw(1000.01));
    EXPECT_DOUBLE_EQ(1000.0, account.getBalance());
}

// 判定表法测试（转账操作）
TEST_F(BankAccountTest, TransferDecisionTable) {
    // 组合1: 成功转账
    recipient.activate();
    EXPECT_TRUE(account.transfer(recipient, 500.0));

    // 组合2: 金额不足
    EXPECT_FALSE(account.transfer(recipient, 1500.0));

    // 组合3: 转入方冻结
    recipient.freeze();
    EXPECT_FALSE(account.transfer(recipient, 500.0));

    // 组合4: 转出方冻结
    account.freeze();
    EXPECT_FALSE(account.transfer(recipient, 500.0));
}

// 因果图法测试（取款操作）
TEST_F(BankAccountTest, Withdraw_CausalGraphing) {
    // 用例1: 所有条件满足 (C1=T, C2=T, C3=T)
    EXPECT_TRUE(account.withdraw(500.0));
    EXPECT_DOUBLE_EQ(500.0, account.getBalance());

    // 用例2: 账户冻结 (C1=F)
    account.freeze();
    EXPECT_FALSE(account.withdraw(500.0));
    account.activate();

    // 用例3: 金额无效 (C2=F)
    EXPECT_FALSE(account.withdraw(-100.0));

    // 用例4: 金额超过余额 (C3=F)
    EXPECT_FALSE(account.withdraw(1500.0));
}

// ================== 白盒测试 ==================

// 语句覆盖测试
TEST_F(BankAccountTest, Deposit_StatementCoverage) {
    try {
        // 覆盖抛出invalid_argument语句
        account.deposit(-100.0);
    }
    catch (const std::invalid_argument&) {
        // 预期捕获异常
    }
    catch (...) {
        FAIL() << "Unexpected exception type";
    }
}

// 分支覆盖测试
TEST_F(BankAccountTest, Withdraw_BranchCoverage) {
    // 分支1：账户冻结
    account.freeze();
    EXPECT_FALSE(account.withdraw(100.0));

    // 分支2：金额无效
    account.activate();
    EXPECT_FALSE(account.withdraw(-50.0));

    // 分支3：余额不足
    EXPECT_FALSE(account.withdraw(1500.0));

    // 分支4：所有条件满足
    EXPECT_TRUE(account.withdraw(500.0));
}

// 独立路径覆盖测试（转账操作）
TEST_F(BankAccountTest, Transfer_IndependentPaths) {
    // 路径1：自我转账
    EXPECT_FALSE(account.transfer(account, 500.0));

    // 路径2：取款失败
    EXPECT_FALSE(account.transfer(recipient, 1500.0));

    // 路径3：存款失败回滚
    recipient.freeze();
    EXPECT_FALSE(account.transfer(recipient, 500.0));
    EXPECT_DOUBLE_EQ(1000.0, account.getBalance());

    // 路径4：完整成功路径
    recipient.activate();
    EXPECT_TRUE(account.transfer(recipient, 500.0));
}

// 条件覆盖测试（存款操作）
TEST_F(BankAccountTest, Deposit_ConditionCoverage) {
    // 条件1：!m_isActive
    account.freeze();
    EXPECT_THROW(account.deposit(500.0), std::runtime_error);

    // 条件2：amount <= 0
    account.activate();
    EXPECT_THROW(account.deposit(-100.0), std::invalid_argument);

    // 条件3：isNaN(amount)
    EXPECT_THROW(account.deposit(std::numeric_limits<double>::quiet_NaN()),
        std::invalid_argument);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*#include "pch.h"
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
*/