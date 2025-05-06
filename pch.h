//zjd 2212

#pragma once
#include <stdexcept>
#include <string>
#include <limits> 

// 银行账户类，封装账户的基本操作
class BankAccount {
private:
    double balance;          // 账户余额
    bool m_isActive;        // 账户状态（激活/冻结）
    std::string accountNumber; // 账户编号

    // 浮点数精确比较辅助函数
    // 参数 a, b: 待比较的两个浮点数
    // 返回值: 若两数差值小于最小精度阈值，则认为相等
    bool isEqual(double a, double b) const {
        return std::abs(a - b) < std::numeric_limits<double>::epsilon();
    }

public:
    // 默认构造函数，初始化账户为激活状态，账号为"DEFAULT"
    BankAccount();

    // 带账号参数的构造函数，初始化余额为0，状态为激活
    // 参数 number: 账户编号（必须非空）
    explicit BankAccount(const std::string& number);

    // 激活账户
    void activate();

    // 冻结账户
    void freeze();

    // 查询账户是否激活
    // 返回值: true表示激活，false表示冻结
    bool isActive() const;

    // 获取当前余额
    // 返回值: 账户余额
    double getBalance() const;

    // 获取账户编号
    // 返回值: 账户编号字符串
    std::string getAccountNumber() const;

    // 存款操作
    // 参数 amount: 存款金额（必须为正数且非NaN）
    // 异常: 若账户冻结或金额非法，抛出std::runtime_error或std::invalid_argument
    void deposit(double amount);

    // 取款操作
    // 参数 amount: 取款金额（必须为正数且不超过余额）
    // 返回值: true表示取款成功，false表示失败（金额非法或余额不足）
    bool withdraw(double amount);

    // 转账操作
    // 参数 to: 目标账户引用
    // 参数 amount: 转账金额
    // 返回值: true表示转账成功，false表示失败（包括转账给自己）
    bool transfer(BankAccount& to, double amount);
};