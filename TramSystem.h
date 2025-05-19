#pragma once
#include "Command.h"
#include <map>
#include <vector>
#include <string>

using namespace std;
class TramSystem {
    // Хранение данных:
    map<string, vector<string>> tramRoutes;  // ключ: номер трамвая, значение: вектор остановок
    map<string, vector<string>> stopInfo;    // ключ: название остановки, значение: номера трамваев

public:
    // Основные методы согласно заданию:
    void createTram(const vector<string>& args);       // CREATE_TRAM
    void showTramsAtStop(const vector<string>& args);  // TRAMS_IN_STOP
    void showStopsForTram(const vector<string>& args);  // STOPS_IN_TRAM
    void displayAllTrams();                            // TRAMS
};
