#include <iostream>
#include <sstream>
#include "TramSystem.h"
#include "Command.h"
#include <vector>
#include <map>
using namespace std;

// функция для разделения строки на отдельные слова
vector<string> splitCommand(const string& input) {
    vector<string> tokens;  // вектор для хранения результата
    istringstream iss(input); // строковый поток для чтения
    string token;            // временная переменная для хранения слова
    
    // последовательное чтение слов из потока
    while (iss >> token) {
        tokens.push_back(token);  // добавление слова в вектор
    }
    return tokens;
}

int main() {
    TramSystem system;  // создание объекта системы трамваев
    string input;       // переменная для хранения ввода пользователя

    // вывод приветствия и списка команд
    cout << "=== система учета трамвайных маршрутов ===" << endl;
    cout << "доступные команды:" << endl
         << "  CREATE_TRAM <номер> <ост1> <ост2>... - добавить маршрут" << endl
         << "  TRAMS_IN_STOP <остановка> - трамваи, проходящие через эту остановку" << endl
         << "  STOPS_IN_TRAM <номер> - остановки трамвая" << endl
         << "  TRAMS - список всех трамваев" << endl
         << "  QUIT - выход" << endl;

    // основной цикл программы
    while (true) {
        cout << "\n>>> ";
        getline(cin, input);  // чтение ввода пользователя
        if (input.empty()) continue; // пропуск пустых строк

        // разбиение ввода на аргументы
        auto args = splitCommand(input);
        if (args.empty()) continue; // пропуск, если нет аргументов

        // определение типа команды
        CmdType cmd = parseCommand(args[0]);
        // подготовка аргументов команды (без первого слова)
        vector<string> cmdArgs(args.begin() + 1, args.end());

        // обработка команды
        switch (cmd) {
            case CmdType::CREATE_TRAM:
                system.createTram(cmdArgs);  // создание маршрута
                break;
            case CmdType::TRAMS_IN_STOP:
                system.showTramsAtStop(cmdArgs);  // показ трамваев на остановке
                break;
            case CmdType::STOPS_IN_TRAM:
                system.showStopsForTram(cmdArgs);  // показ остановок маршрута
                break;
            case CmdType::TRAMS:
                system.displayAllTrams();  // показ всех маршрутов
                break;
            case CmdType::QUIT:
                cout << "выход из системы" << endl;
                return 0;  // завершение программы
            case CmdType::UNKNOWN:
                cout << "неизвестная команда" << endl;
                break;
        }
    }
}
