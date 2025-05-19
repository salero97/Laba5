#include "TramSystem.h"
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;

// метод для создания нового трамвайного маршрута
void TramSystem::createTram(const vector<string>& args) {
    // проверка наличия минимально необходимых аргументов
    if (args.size() < 2) {
        cout << "ошибка: требуется номер трамвая и хотя бы одна остановка" << endl;
        return;
    }

    const string& tramNum = args[0];  // первый аргумент - номер трамвая
    vector<string> stops(args.begin() + 1, args.end());  // остальные аргументы - остановки

    // проверка что номер трамвая - число (начиная с 1)
    if (tramNum.empty() || !all_of(tramNum.begin(), tramNum.end(), ::isdigit)) {
        cout << "ошибка: номер трамвая должен быть числом (начиная с 1)" << endl;
        return;
    }

    // добавление маршрута в основную базу данных
    tramRoutes[tramNum] = stops;

    // обновление информации по остановкам
    for (const auto& stop : stops) {
        auto& trams = stopInfo[stop];  // получение списка трамваев для текущей остановки
        // проверка на дублирование трамвая на остановке
        if (find(trams.begin(), trams.end(), tramNum) == trams.end()) {
            trams.push_back(tramNum);  // добавление трамвая на остановку
        }
    }

    cout << "трамвай " << tramNum << " создан. остановок: " << stops.size() << endl;
}

// метод для показа трамваев на конкретной остановке
void TramSystem::showTramsAtStop(const vector<string>& args) {
    // проверка наличия аргумента (названия остановки)
    if (args.empty()) {
        cout << "ошибка: укажите название остановки" << endl;
        return;
    }

    const string& stopName = args[0];  // получение названия остановки
    auto it = stopInfo.find(stopName); // поиск остановки в базе данных

    // проверка существования остановки и наличия трамваев
    if (it == stopInfo.end() || it->second.empty()) {
        cout << "через остановку " << stopName << " не проходит ни один трамвай" << endl;
        return;
    }

    // вывод всех трамваев, проходящих через эту остановку
    cout << "трамваи через " << stopName << ": ";
    for (const auto& tram : it->second) {
        cout << tram << " ";  // вывод номеров трамваев
    }
    cout << endl;
}

// метод для показа остановок конкретного трамвая
void TramSystem::showStopsForTram(const vector<string>& args) {
    // проверка наличия аргумента (номера трамвая)
    if (args.empty()) {
        cout << "ошибка: укажите номер трамвая" << endl;
        return;
    }

    const string& tramNum = args[0];  // получение номера трамвая
    auto it = tramRoutes.find(tramNum); // поиск трамвая в базе данных

    // проверка существования маршрута
    if (it == tramRoutes.end()) {
        cout << "трамвай " << tramNum << " не найден" << endl;
        return;
    }

    // вывод информации по маршруту
    cout << "маршрут трамвая " << tramNum << ":" << endl;
    for (const auto& stop : it->second) {
        cout << " - " << stop << " (пересадки: ";
        
        // поиск трамваев для пересадки (исключая текущий)
        size_t transfers = 0;
        for (const auto& otherTram : stopInfo.at(stop)) {
            if (otherTram != tramNum) {
                cout << otherTram << " ";
                transfers++;
            }
        }
        
        // если пересадок нет
        if (transfers == 0) cout << "нет";
        cout << ")" << endl;
    }
}

// метод для показа всех трамвайных маршрутов
void TramSystem::displayAllTrams() {
    // проверка наличия маршрутов в системе
    if (tramRoutes.empty()) {
        cout << "в системе нет трамваев" << endl;
        return;
    }

    // вывод всех маршрутов с их остановками
    cout << "список всех трамваев:" << endl;
    for (const auto& [num, stops] : tramRoutes) {
        cout << "трамвай №" << num << " (" << stops.size() << " остановок): ";
        for (const auto& stop : stops) {
            cout << stop << " ";  // вывод всех остановок маршрута
        }
        cout << endl;
    }
}
