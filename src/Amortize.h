//
// Created by as on 22.03.17.
//

#ifndef AMORTIZE_AMORTIZE_H
#define AMORTIZE_AMORTIZE_H


#include <iosfwd>
#include <iostream>
#include <list>

using namespace std;

class Amortize {
private:
    struct rowTable
    {
        int rowYear;
        int rowMonth;
        double principal_payment;
        double interest_payment;
        double principal_payment_pv;
        double interest_payment_pv;
        double ratio;
    };
    double amount_;
    double years_;
    double rate_;
    double selloff_year_;
    double extra_payoff_;
    double inflation_;
    int    months_;
    int    selloff_month_;
    double total_interest_;
    double interest_pv_;
    double monthly_payment_;
    double years_to_payoff_;
    list<rowTable> calcTable;
    bool inputDataChange;

    void clearBeforeCalculate();

public:
    Amortize();
    Amortize(const double amount, const double inflation=0,
             const double rate=4.5,
             const int months=0,
             const double years=30, const double selloff_month=0,
             const double selloff_year=0, const double extra_payoff=0);
    Amortize(const Amortize &);
    ~Amortize();

    void   setAmount(const double amount);
    double getAmount() const;

    void   setYears(const double years);
    double getYears() const;

    void setMonths(const int months);
    int  getMonths() const;

    void   setRate(const double rate);
    double getRate() const;

    void   setSellOffYear(const double selloff_year);
    double getSellOffYear() const;

    void   setSellOffMonth(const double selloff_month);
    double getSellOffMonth() const;

    void   setExtraPayOff(const double extra_payoff);
    double getExtraPayOff() const;

    void   setInflation(const double inflation);
    double getInflation() const;

    double getTotalInterest() const;
    double getInterestPV() const;
    double getMonthlyPayment() const;
    double getYearsToPayOff() const;

    bool Calculate();
    void ShowTable(std::ostream & =std::cout);
    bool saveToFile(const char * ="", const char delimiter=';', const bool includeHeader = true);
    Amortize &operator =(const Amortize &);
    friend std::ostream & operator <<(std::ostream &, const Amortize &);
    friend bool operator ==(const Amortize &, const Amortize &);

};

bool operator !=(const Amortize &loper, const Amortize &roper);


#endif //AMORTIZE_AMORTIZE_H
