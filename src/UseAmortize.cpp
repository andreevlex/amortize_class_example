#include <iostream>
#include "Amortize.h"
#include <fstream>

int main() {

    setlocale(LC_ALL, "rus"); // поддержка русскоого языка
    Amortize mycredit(2000000, 3, 14, 36);

    if (mycredit.Calculate())
    {
        std::cout << "Ежемесячный платеж:" << mycredit.getMonthlyPayment() << std::endl;
        std::cout << "Общая сумма переплаты:" << mycredit.getTotalInterest() << std::endl;

        mycredit.ShowTable();
        // Сохранение графика платежей в файл формата CSV
        if(mycredit.saveToFile("test.csv"))
        {
            cout << "Файл с настройками по умолчанию успешно записан!" << endl;
        }

        // Сохранение графика платежей в файл формата CSV
        // без заголовка и с измененным разделителем
        if(mycredit.saveToFile("test2.csv", '\t', false))
        {
            cout << "Файл без заголовка успешно записан!" << endl;
        }
    }

    // Измение входных данных в уже расчитанном графике и перерасчет
    mycredit.setMonths(8);
    if (mycredit.Calculate())
    {
        mycredit.ShowTable();
    }

    Amortize othercredit;
    // Работа оператора присваения
    othercredit = mycredit;

    // расчет не происходит так как входные данные не изменились
    if (othercredit.Calculate())
    {
        othercredit.ShowTable();
        // Встраивание в поток
        cout << othercredit;
    }

    Amortize jcredit(150000, 8, 14, 12);
    jcredit.setSellOffMonth(10); // изменение входных данных перед расчетом
    if(jcredit.Calculate())
    {
        /* Вывод метода не в консоль, а в файл */
        fstream fout;
        fout.open("test2.txt", ios_base::out | ios_base::trunc);

        if(fout.is_open())
        {
            jcredit.ShowTable(fout);
        }
        fout.close();
    }

    //system("pause");
    return 0;
}