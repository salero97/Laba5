#pragma once
#include <string>

// Типы команд для управления трамвайными маршрутами
enum class CmdType {
    CREATE_TRAM,      // Добавить новый маршрут трамвая
    TRAMS_IN_STOP, // Показать маршруты через остановку
    STOPS_IN_TRAM, // Показать остановки на маршруте
    TRAMS,     // Показать все маршруты
    QUIT,           // Выйти из программы
    UNKNOWN         // Некорректная команда
};

// Определяет тип команды по строке ввода
CmdType parseCommand(const std::string& cmdStr);
