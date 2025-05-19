// импортируем модуль std::io для работы с вводом-выводом
use std::io;
// импортируем модули для работы с системным временем
use std::time::{SystemTime, UNIX_EPOCH};

// определяем структуру Visitor (посетитель), которая может быть скопирована (Clone)
#[derive(Clone)]
struct Visitor {
    ticket: String,   // уникальный билет посетителя
    duration: u32,    // время приема в минутах
}

// функция для генерации уникального билета на основе текущего времени
fn generate_ticket() -> String {
    // получаем текущее системное время с момента эпохи в наносекундах
    let time_ns = SystemTime::now()
        .duration_since(UNIX_EPOCH) // вычисляем разницу с эпохой UNIX
        .unwrap()                     // обрабатываем возможную ошибку (здесь предполагается, что не будет)
        .as_nanos();                  // получаем время в наносекундах

    // форматируем билет как строку "T" + последние три цифры времени
    format!("T{:03}", (time_ns % 1000) as u32)
}

// функция для вывода справочной информации о командах
fn print_help() {
    println!("\nДоступные команды:");
    println!("ENQUEUE <минуты> - добавить посетителя");
    println!("DISTRIBUTE       - распределить очередь");
    println!("HELP             - показать эту справку");
    println!("EXIT             - выход\n");
}

// основная функция программы
fn main() {
    // выводим приветственное сообщение
    println!("=== Электронная очередь поликлиники ===");

    // вводим количество окон, пока пользователь не введет корректное число
    let windows = loop {
        println!("\nВведите количество окон приема:");
        let mut input = String::new(); // создаем строку для ввода
        io::stdin().read_line(&mut input).expect("Ошибка чтения"); // читаем строку с клавиатуры

        // пытаемся преобразовать введенное значение в число
        match input.trim().parse() {
            Ok(num) if num > 0 => break num, // если число > 0, выходим из цикла с этим значением
            _ => println!("! Ошибка: введите число больше 0"), // иначе выводим ошибку и повторяем
        }
    };

    // выводим сообщение о созданных окнах
    println!("\nСоздано {} окон приема.", windows);
    // выводим справку по командам
    print_help();

    // создаем пустую очередь посетителей
    let mut queue: Vec<Visitor> = Vec::new();

    // главный цикл обработки команд
    loop {
        println!("> Введите команду:");
        let mut command = String::new(); // строка для команды
        io::stdin().read_line(&mut command).expect("Ошибка чтения"); // читаем команду

        // приводим команду к верхнему регистру и убираем пробелы
        let command = command.trim().to_uppercase();

        // обработка команд по шаблону
        match command.as_str() {
            // команда "ENQUEUE" без аргументов
            "ENQUEUE" => {
                // ввод времени приема
                let duration = loop {
                    println!("> Введите время приема (минуты):");
                    let mut input = String::new();
                    io::stdin().read_line(&mut input).expect("Ошибка чтения");

                    // преобразуем ввод в число
                    match input.trim().parse() {
                        Ok(num) if num > 0 => break num, // если число > 0, выходим из цикла
                        _ => println!("! Ошибка: введите число больше 0"), // иначе повтор
                    }
                };

                // создаем нового посетителя с уникальным билетом и введенным временем
                let visitor = Visitor {
                    ticket: generate_ticket(),
                    duration,
                };
                // добавляем посетителя в очередь
                queue.push(visitor.clone());
                // выводим сообщение о добавлении
                println!("> Добавлен: {} ({} мин)", visitor.ticket, visitor.duration);
            },

            // команда "DISTRIBUTE" — распределение очереди по окнам
            "DISTRIBUTE" => {
                if queue.is_empty() {
                    println!("! Очередь пуста");
                    continue; // если очередь пуста, переходим к следующей итерации
                }

                // создаем вектор для окон, каждый элемент — кортеж (накопленное время, список посетителей)
                let mut windows_vec = vec![(0, Vec::new()); windows];

                // распределяем посетителей по окнам, выбирая окно с минимальным временем
                for visitor in &queue {
                    if let Some(min_win) = windows_vec.iter_mut().min_by_key(|w| w.0) {
                        min_win.0 += visitor.duration; // увеличиваем время окна
                        min_win.1.push(visitor.clone()); // добавляем посетителя к окну
                    }
                }

                // выводим результат распределения
                println!("\n=== Распределение ===");
                for (i, window) in windows_vec.iter().enumerate() {
                    print!("Окно {} ({} мин): ", i + 1, window.0);
                    // формируем список билетов и времени для каждого окна
                    let tickets: Vec<String> = window.1.iter()
                        .map(|v| format!("{} ({} мин)", v.ticket, v.duration))
                        .collect();
                    println!("{}", tickets.join(", ")); // выводим список
                }
                println!("====================");
                break; // завершаем программу после распределения
            },

            // команда "HELP" — вывод справки
            "HELP" => print_help(),

            // команда "EXIT" — завершение программы
            "EXIT" => {
                println!("Выход из программы");
                return; // выход из функции main
            },

            // обработка команд с аргументами, например "ENQUEUE 10"
            _ => {
                if let Some((cmd, arg)) = command.split_once(' ') {
                    match cmd {
                        "ENQUEUE" => {
                            // обработка аргумента для "ENQUEUE"
                            match arg.trim().parse() {
                                Ok(duration) if duration > 0 => {
                                    // создаем посетителя и добавляем в очередь
                                    let visitor = Visitor {
                                        ticket: generate_ticket(),
                                        duration,
                                    };
                                    queue.push(visitor.clone());
                                    println!("> Добавлен: {} ({} мин)", visitor.ticket, visitor.duration);
                                },
                                _ => println!("! Ошибка: неверное время приема"), // ошибка при вводе
                            }
                        },
                        _ => println!("! Неизвестная команда. Введите HELP"), // неизвестная команда
                    }
                } else {
                    println!("! Неизвестная команда. Введите HELP"); // неизвестная команда без аргументов
                }
            }
        }
    }
}
