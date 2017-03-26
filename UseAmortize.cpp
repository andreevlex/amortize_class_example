#include <iostream>
#include "Amortize.h"

int main() {

    setlocale(LC_ALL, "rus");
    Amortize mycredit(200000, 3, 11.5);

    if (mycredit.Calculate())
    {
        std::cout << "платеж в месяц:" << mycredit.getMonthlyPayment() << std::endl;
        std::cout << "общая сумма:" << mycredit.getTotalInterest() << std::endl;
        mycredit.ShowTable();
        if(mycredit.saveToFile("test.csv"))
        {
            cout << "Файл успешно записан!" << endl;
        }
    }

    mycredit.setMonths(8);
    if (mycredit.Calculate())
    {
        std::cout << "платеж в месяц:" << mycredit.getMonthlyPayment() << std::endl;
        std::cout << "общая сумма:" << mycredit.getTotalInterest() << std::endl;
        mycredit.ShowTable();
    }

    Amortize othercredit;
    othercredit = mycredit;
    //othercredit.setMonths(-3);
    if (othercredit.Calculate())
    {
        othercredit.ShowTable();
        cout << othercredit;

        //if(othercredit.saveToFile("test.csv"))
        //{
        //    cout << "Файл успешно записан!" << endl;
        //}
    }

    //system("pause");
    return 0;
}