#ifndef FEE_MANAGER_HPP
#define FEE_MANAGER_HPP

#include <string>
#include <vector>
#include <map>
#include <mutex>

namespace Service {

struct FeePayment {
    std::string rollNo;
    double amount;
    std::string date;

    std::string toJson() const;
};

struct FeeStatus {
    std::string rollNo;
    std::string studentName;
    double totalFee;
    double totalPaid;
    double pendingBalance;

    std::string toJson() const;
};

class FeeManager {
private:
    std::string filePath;
    std::vector<FeePayment> payments;
    std::map<std::string, double> customTotalFees; // rollNo -> total fee
    double defaultTotalFee;
    std::mutex feeMutex;

    void loadFromFile();
    void saveToFile();
    void seedInitialData();

public:
    explicit FeeManager(std::string filePath = "data/fees.csv", double defaultTotal = 55000.0);

    // PPT Required Methods
    bool recordFeePayment(const std::string& rollNo, double amount, const std::string& date);
    double getPendingFee(const std::string& rollNo);
    std::string generateFeeReceipt(const std::string& rollNo, double amount, const std::string& date, const std::string& studentName = "");
    std::vector<FeePayment> getFeeHistory(const std::string& rollNo);

    // Helpers
    FeeStatus getFeeStatus(const std::string& rollNo, const std::string& studentName = "");
    std::vector<FeeStatus> getAllFeeStatuses();
    void setTotalFee(const std::string& rollNo, double total);
    double getTotalPaid(const std::string& rollNo);
};

} // namespace Service

#endif // FEE_MANAGER_HPP

