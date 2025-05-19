#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>

using namespace std;

// Структура ячейки склада
struct Cell {
    string name;     // Название товара
    int count;       // Количество товара
    Cell() : name(""), count(0) {} // Инициализация пустой ячейки
};

// Класс склада
class Warehouse {
    // Вектор всех возможных адресов ячеек
    vector<string> allAddresses;

    // Карта для хранения содержимого ячеек по адресу
    map<string, Cell> cells;

    // Общая вместимость склада (суммарное количество всех ячеек * 10)
    int totalCapacity;
    // Использованное место
    int totalUsed;

public:
    // Конструктор: инициализация всех ячеек по заданным параметрам
    Warehouse() {
        // Новые параметры по условию
        int zoneCount = 2;             // Количество зон хранения
        int shelfCount = 19;           // Количество стеллажей в каждой зоне
        int sectionCount = 4;          // Количество вертикальных секций в стеллаже
        int shelfPerSection = 8;       // Количество полок в секции
        int capacityPerCell = 10;      // Максимум единиц товара в ячейке

        // Расчет общей вместимости
        totalCapacity = zoneCount * shelfCount * sectionCount * shelfPerSection * capacityPerCell;
        totalUsed = 0;

        // Генерация всех возможных адресов ячеек
        // Формат: <Зона><Стеллаж><Секция><Полка>, например: A1739
        for (int z = 0; z < zoneCount; ++z) {
            char zoneLetter = 'A' + z; // Название зоны: A, B
            for (int s = 1; s <= shelfCount; ++s) {
                for (int sec = 1; sec <= sectionCount; ++sec) {
                    for (int p = 1; p <= shelfPerSection; ++p) {
                        string addr = "";
                        addr += zoneLetter; // Зона: A или B
                        addr += to_string(s); // Стеллаж
                        addr += to_string(sec); // Секция
                        addr += to_string(p); // Полка
                        allAddresses.push_back(addr);
                    }
                }
            }
        }
    }

    // Метод для добавления товара
    void addItem(const string& name, int count, const string& addr) {
        Cell& cell = getCell(addr);
        int freeSpace = 10 - cell.count; // Свободное место в ячейке
        if (count > freeSpace) {
            cout << "Недостаточно места в ячейке.\n";
            return;
        }
        cell.name = name; // Название товара
        cell.count += count; // Увеличение количества
        updateUsage(); // Обновление общего использования
        cout << "Товар добавлен.\n";
    }

    // Метод для удаления товара
    void removeItem(const string& name, int count, const string& addr) {
        Cell& cell = getCell(addr);
        // Проверка, есть ли товар и достаточно ли его
        if (cell.count < count || cell.name != name) {
            cout << "Недостаточно товара для списания.\n";
            return;
        }
        cell.count -= count; // Уменьшаем количество
        if (cell.count == 0)
            cell.name = ""; // Очистка ячейки, если товар полностью списан
        updateUsage(); // Обновление общего использования
        cout << "Товар удален.\n";
    }

    // Получение ссылки на ячейку по адресу
    Cell& getCell(const string& addr) {
        if (cells.find(addr) == cells.end())
            cells[addr] = Cell(); // Создаем новую ячейку, если нет
        return cells[addr];
    }

    // Обновление общего количества использованных ячеек
    void updateUsage() {
        totalUsed = 0;
        for (auto& pair : cells)
            totalUsed += pair.second.count;
    }

    // Метод для вывода информации о состоянии склада
    void info() {
        // Расчет общего процента заполнения
        double percentTotal = (double)totalUsed / totalCapacity * 100;

        cout << fixed << setprecision(2);
        cout << "Общий процент заполнения склада: " << percentTotal << "%\n";

        // Расчет заполненности каждой зоны
        int zoneCount = 2; // по условию
        int cellsPerZone = allAddresses.size() / zoneCount;

        for (int z = 0; z < zoneCount; ++z) {
            int zoneUsed = 0;
            int startIdx = z * cellsPerZone;
            int endIdx = startIdx + cellsPerZone;
            for (int i = startIdx; i < endIdx; ++i) {
                const string& addr = allAddresses[i];
                auto it = cells.find(addr);
                if (it != cells.end())
                    zoneUsed += it->second.count;
            }
            double zonePercent = (double)zoneUsed / totalCapacity * 100;
            cout << "Зона " << (char)('A' + z) << " заполнена на " << zonePercent << "%\n";
        }

        // Вывод ячеек с товаром
        cout << "Ячейки с товаром:\n";
        bool hasItems = false;
        for (auto& pair : cells) {
            if (pair.second.count > 0) {
                cout << "Адрес: " << pair.first << ", Товар: " << pair.second.name
                     << ", Количество: " << pair.second.count << "\n";
                hasItems = true;
            }
        }
        if (!hasItems)
            cout << "Нет товаров на складе.\n";

        // Вывод пустых ячеек через запятую
        cout << "Пустые ячейки: ";
        bool firstEmpty = true;
        for (auto& addr : allAddresses) {
            auto it = cells.find(addr);
            if (it == cells.end() || it->second.count == 0) {
                if (!firstEmpty)
                    cout << ", ";
                cout << addr;
                firstEmpty = false;
            }
        }
        if (firstEmpty)
            cout << "Все ячейки заняты.";
        cout << "\n";
    }
};

int main() {
    // Создаем объект склада с новыми параметрами
    Warehouse wh;

    // Инструкции для пользователя
    cout << "Система учета товаров на складе\n";
    cout << "Доступные команды:\n";
    cout << "ADD <наименование> <количество> <адрес>\n";
    cout << "REMOVE <наименование> <количество> <адрес>\n";
    cout << "INFO\n";
    cout << "EXIT\n";

    string cmd;
    while (true) {
        cout << "\nВведите команду: ";
        cin >> cmd;

        if (cmd == "EXIT") {
            break; // Выход из программы
        } else if (cmd == "ADD") {
            string name, addr;
            int count;
            cin >> name >> count >> addr;
            wh.addItem(name, count, addr);
        } else if (cmd == "REMOVE") {
            string name, addr;
            int count;
            cin >> name >> count >> addr;
            wh.removeItem(name, count, addr);
        } else if (cmd == "INFO") {
            wh.info(); // Вывод состояния склада
        } else {
            cout << "Неверная команда.\n";
        }
    }

    return 0;
}
