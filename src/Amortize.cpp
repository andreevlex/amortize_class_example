//
// Created by as on 22.03.17.
//

#include "Amortize.h"
#include <cmath>
#include <iterator>
#include <iomanip>
#include <fstream>

using namespace std;

Amortize::Amortize()
{

    amount_        = 0.;
    rate_          = 0.;
    inflation_     = 0.;
    months_        = 0;
    years_         = 0.;
    selloff_month_ = 0.;
    selloff_year_  = 0.;
    extra_payoff_  = 0.;
    inputDataChange = false;

    clearBeforeCalculate();
}

Amortize::Amortize(const double amount, const double inflation, const double rate, const int months, const double years,
                   const double selloff_month, const double selloff_year, const double extra_payoff)
{

    setAmount(amount);
    setRate(rate);
    setInflation(inflation);
    setMonths(months);
    setYears(years);
    setSellOffMonth(selloff_month);
    setSellOffYear(selloff_year);
    setExtraPayOff(extra_payoff);
    inputDataChange = true;

    clearBeforeCalculate();
}

Amortize::Amortize(const Amortize &other)
{
    amount_ = other.amount_;
    rate_ = other.rate_;
    inflation_ = other.inflation_;
    months_ = other.months_;
    years_ = other.years_;
    selloff_month_ = other.selloff_month_;
    selloff_year_ = other.selloff_year_;
    extra_payoff_ = other.extra_payoff_;
    inputDataChange = other.inputDataChange;
    total_interest_ = other.total_interest_;
    interest_pv_ = other.interest_pv_;
    monthly_payment_ = other.monthly_payment_;
    years_to_payoff_ = other.years_to_payoff_;
    calcTable = other.calcTable;
}

Amortize::~Amortize()
{
    if(!calcTable.empty())
    {
        calcTable.clear();
    }
}

void Amortize::setAmount(const double amount)
{

    if( !amount || amount < 0 )
    {
        cerr << "amount не должно быть равно 0 или меньше 0\n";
        return;
    }
    amount_ = amount;
    inputDataChange = true;
}

double Amortize::getAmount() const
{
    return amount_;
}

void Amortize::setYears(const double years)
{

    if( years < 0 )
    {
        cerr << "years не должно быть меньше 0\n";
        return;
    }
    years_ = years;

    if( !months_ ){
        if( years_ ) setMonths(years_ * 12);
    }
    inputDataChange = true;
}

double Amortize::getYears() const
{
    return years_;
}

void Amortize::setMonths(const int months)
{

    if( months < 0 )
    {
        cerr << "months не должно быть меньше 0\n";
        return;
    }
    months_ = months;
    inputDataChange = true;
}

int Amortize::getMonths() const
{
    return months_;
}

void Amortize::setRate(const double rate)
{

    if( rate < 0 )
    {
        cerr << "rate не должно быть меньше 0\n";
        return;
    }
    rate_ = rate;
    inputDataChange = true;
}

double Amortize::getRate() const
{
    return rate_;
}

void Amortize::setSellOffYear(const double selloff_year)
{

    if( selloff_year < 0 )
    {
        cerr << "selloff_year не должно быть меньше 0\n";
        return;
    }
    selloff_year_ = selloff_year;

    if( !selloff_month_ && selloff_year_ )
    {
        setSellOffMonth(selloff_year_ * 12);
    }

    inputDataChange = true;
}

double Amortize::getSellOffYear() const
{
    return selloff_year_;
}

void Amortize::setSellOffMonth(const double selloff_month)
{

    if( selloff_month < 0 )
    {
        cerr << "selloff_month не должно быть меньше 0\n";
        return;
    }
    selloff_month_ = selloff_month;
    inputDataChange = true;
}

double Amortize::getSellOffMonth() const
{
    return selloff_month_;
}

void Amortize::setExtraPayOff(const double extra_payoff)
{
    extra_payoff_ = extra_payoff;
    inputDataChange = true;
}

double Amortize::getExtraPayOff() const
{
    return extra_payoff_;
}

void Amortize::setInflation(const double inflation)
{
    inflation_ = inflation;
    inputDataChange = true;
}

double Amortize::getInflation() const
{
    return inflation_;
}

double Amortize::getTotalInterest() const {
    return total_interest_;
}

double Amortize::getInterestPV() const
{
    return interest_pv_;
}

double Amortize::getMonthlyPayment() const
{
    return monthly_payment_;
}

double Amortize::getYearsToPayOff() const
{
    return years_to_payoff_;
}

bool Amortize::Calculate()
{

    if( !inputDataChange ) return true;

    if( !months_ && !years_ )
    {
        cerr << "Должен быть задан хотя бы один параметр months или years\n";
        return false;
    }

    clearBeforeCalculate();

    double mrate = rate_/12000;
    double amount = amount_;

    monthly_payment_ = amount * mrate/(1 - pow(1 + mrate, -months_)) + extra_payoff_;
    int end_month = ( selloff_month_ && selloff_month_ < months_ )
                    ? selloff_month_
                    : months_;
    int m;
    for(m = 0; m < end_month && amount > 0; m++)
    {
        double interest_payment = amount * mrate;
        double principal_payment = monthly_payment_ - interest_payment;

        if( amount <= 0 )
        {
            principal_payment = interest_payment = 0;
        }
        amount -= principal_payment;
        double deflator = pow(1 + inflation_/100, -m/12.);
        interest_pv_ += interest_payment * deflator;
        total_interest_ += interest_payment;

        rowTable curRow;
        curRow.rowYear = m/12;
        curRow.rowMonth = m - 12 * (m/12) + 1;
        curRow.principal_payment = principal_payment;
        curRow.interest_payment = interest_payment;
        curRow.principal_payment_pv = principal_payment * deflator;
        curRow.interest_payment_pv = interest_payment * deflator;
        curRow.ratio = principal_payment/(principal_payment + interest_payment) * 100;

        calcTable.push_back(curRow);
    }

    years_to_payoff_ = m/12.;
    inputDataChange = false;

    return true;
}

void Amortize::ShowTable(std::ostream &os)
{

    os << "yr/mon\tPrinc.\t\tInt.\t|\tPV Princ.\tPV int.\t\tRatio\n";
    if ( calcTable.empty() ) return;
    for(auto iter = calcTable.begin(); iter != calcTable.end(); iter++)
    {

        os << setw(3)
           << left
           << iter->rowYear << '/'
           << setw(2)
           << left
           << iter->rowMonth << "\t"
           << setw(10)
           << fixed
           << setprecision(2)
           << iter->principal_payment << "\t"
           << iter->interest_payment << "\t|\t"
           << iter->principal_payment_pv << "\t\t"
           << iter->interest_payment_pv << "\t\t"
           << iter->ratio << "\n";
    }
}

void Amortize::clearBeforeCalculate()
{

    total_interest_ = 0.0;
    interest_pv_ = 0.0;
    monthly_payment_ = 0.0;
    years_to_payoff_ = 0.0;

    if(!calcTable.empty())
    {
        calcTable.clear();
    }
}

Amortize &Amortize::operator=(const Amortize &other) {

    if(this == &other) return *this;

    amount_ = other.amount_;
    rate_ = other.rate_;
    inflation_ = other.inflation_;
    months_ = other.months_;
    years_ = other.years_;
    selloff_month_ = other.selloff_month_;
    selloff_year_ = other.selloff_year_;
    extra_payoff_ = other.extra_payoff_;
    inputDataChange = other.inputDataChange;
    total_interest_ = other.total_interest_;
    interest_pv_ = other.interest_pv_;
    monthly_payment_ = other.monthly_payment_;
    years_to_payoff_ = other.years_to_payoff_;
    calcTable = other.calcTable;

    return *this;
}

std::ostream &operator<<(std::ostream &os, const Amortize &amt) {

    //if( selloff_month_ == 0 )
    return os << "Total interest: " << amt.getTotalInterest() << endl;
}

bool Amortize::saveToFile(const char *filename, const char delimiter, const bool includeHeader) {

    if(NULL == filename)
    {
        cerr << "Empty filename\n";
        return false;
    }

    if(calcTable.empty())
    {
        cerr << "Empty calculate table\n";
        return false;
    }

    fstream fout;
    fout.open(filename,ios_base::out | ios_base::trunc);
    if(!fout.is_open())
    {
        cerr << "File can not to be open\n";
        return false;
    }

    /* header */
    if(includeHeader) {
        fout << "Year"                  << delimiter
             << "Month"                 << delimiter
             << "Principal payment"     << delimiter
             << "Interest payment"      << delimiter
             << "PV Principal payment"  << delimiter
             << "PV Interest payment"   << delimiter
             << "Ratio"                 << delimiter
             << endl;
    }

    for(auto iter = calcTable.begin(); iter != calcTable.end(); iter++)
    {

        fout << iter->rowYear               << delimiter
             << iter->rowMonth              << delimiter
             << iter->principal_payment     << delimiter
             << iter->interest_payment      << delimiter
             << iter->principal_payment_pv  << delimiter
             << iter->interest_payment_pv   << delimiter
             << iter->ratio                 << endl;
    }

    fout.close();
    return true;

}

bool operator ==(const Amortize &loper, const Amortize &roper) {
    return (loper.amount_ == roper.amount_ &&
            loper.rate_ == roper.rate_ &&
            loper.inflation_ == roper.inflation_ &&
            loper.years_ == roper.years_ &&
            loper.months_ == roper.months_ &&
            loper.selloff_month_ == roper.selloff_month_ &&
            loper.selloff_year_ == roper.selloff_year_ &&
            loper.extra_payoff_ == roper.extra_payoff_);
}

bool operator!=(const Amortize &loper, const Amortize &roper) {
    return !(loper == roper);
}
