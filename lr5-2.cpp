#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace std;

// функция генерации номера талона
string genTicket() {
    // инициализируем генератор случайных чисел текущим временем
    srand(time(nullptr));
    
    // создаем строковый поток для форматирования номера
    ostringstream oss;
    
    // генерируем номер в формате TXXX, где XXX - случайное трехзначное число
    oss << "T" << setw(3) << setfill('0') << (rand() % 1000);
    
    // возвращаем сгенерированный номер
    return oss.str();
}

// структура для хранения информации о посетителе
struct Visitor {
    string ticket;   // номер талона (например "T123")
    int duration;    // продолжительность приема в минутах
};

// функция вывода справки по командам
void printHelp() {
    cout << "\nдоступные команды:\n";
    cout << "ENQUEUE <минуты> - добавить посетителя в очередь\n";
    cout << "DISTRIBUTE       - распределить очередь по окнам\n";
    cout << "HELP             - показать эту справку\n";
    cout << "EXIT             - завершить программу\n\n";
}

// главная функция программы
int main() {
    int windows;                // количество окон приема
    vector<Visitor> queue;      // очередь посетителей
    
    // выводим приветственное сообщение
    cout << "=== электронная очередь поликлиники ===\n\n";
    
    // запрашиваем количество окон
    cout << "введите количество окон приема: ";
    cin >> windows;
    cout << "\nсоздано " << windows << " окон приема.\n";
    
    // выводим справку по командам
    printHelp();
    
    string command;  // переменная для хранения введенной команды
    
    // основной цикл программы
    while (true) {
        cout << "> введите команду: ";
        cin >> command;
        
        // обработка команды добавления посетителя
        if (command == "ENQUEUE") {
            int duration;
            
            // проверяем корректность ввода продолжительности
            if (!(cin >> duration)) {
                cout << "! ошибка: введите число минут после команды ENQUEUE\n";
                cin.clear();          // сбрасываем флаги ошибок
                cin.ignore(10000, '\n');  // очищаем буфер ввода
                continue;
            }
            
            // создаем нового посетителя
            Visitor v = {genTicket(), duration};
            
            // добавляем посетителя в очередь
            queue.push_back(v);
            
            // выводим подтверждение добавления
            cout << "> добавлен посетитель с талоном " << v.ticket 
                 << " (" << duration << " минут)\n";
            
        } 
        // обработка команды распределения очереди
        else if (command == "DISTRIBUTE") {
            // проверяем, есть ли посетители в очереди
            if (queue.empty()) {
                cout << "! очередь пуста, нечего распределять\n";
                continue;
            }
            
            // создаем вектор для хранения информации по окнам
            // каждый элемент - пара: общее время и список посетителей
            vector<pair<int, vector<Visitor>>> win(windows);
            
            // распределяем посетителей по окнам
            for (const auto& vis : queue) {
                // находим окно с минимальным текущим временем
                auto minWin = min_element(win.begin(), win.end(),
                    [](const auto& a, const auto& b) {
                        return a.first < b.first;
                    });
                
                // добавляем посетителя в это окно
                minWin->first += vis.duration;       // увеличиваем общее время
                minWin->second.push_back(vis);        // добавляем посетителя
            }
            
            // выводим результаты распределения
            cout << "\n=== результаты распределения ===\n";
            for (int i = 0; i < windows; ++i) {
                cout << "окно " << i+1 << " (общее время: " << win[i].first 
                     << " мин): ";
                
                // выводим список талонов для этого окна
                bool first = true;
                for (const auto& vis : win[i].second) {
                    if (!first) cout << ", ";
                    cout << vis.ticket << " (" << vis.duration << " мин)";
                    first = false;
                }
                cout << "\n";
            }
            cout << "===============================\n";
            
            break;  // завершаем работу после распределения
            
        } 
        // обработка команды вывода справки
        else if (command == "HELP") {
            printHelp();
            
        } 
        // обработка команды выхода
        else if (command == "EXIT") {
            cout << "завершение работы программы.\n";
            return 0;
            
        } 
        // обработка неизвестной команды
        else {
            cout << "! неизвестная команда. введите HELP для списка команд\n";
        }
    }
    
    return 0;
}
