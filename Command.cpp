#include "Command.h"

using namespace std;

// функция для определения типа введенной команды
CmdType parseCommand(const string& cmdStr) {
    // сравнение введенной строки с известными командами
    if (cmdStr == "CREATE_TRAM") return CmdType::CREATE_TRAM;    // команда создания маршрута
    if (cmdStr == "TRAMS_IN_STOP") return CmdType::TRAMS_IN_STOP; // команда просмотра трамваев на остановке
    if (cmdStr == "STOPS_IN_TRAM") return CmdType::STOPS_IN_TRAM; // команда просмотра остановок маршрута
    if (cmdStr == "TRAMS") return CmdType::TRAMS;               // команда вывода всех маршрутов
    if (cmdStr == "QUIT") return CmdType::QUIT;                 // команда выхода из программы
    return CmdType::UNKNOWN;                                    // неизвестная команда
}
