use std::collections::HashMap;
use std::io::{self, Write};

// структура ячейки, хранит название товара и его количество
#[derive(Debug, Clone)]
struct Cell {
    name: String,
    count: usize,
}

impl Cell {
    fn new() -> Self {
        Self {
            name: String::new(),
            count: 0,
        }
    }
}

// класс склада
struct Warehouse {
    all_addresses: Vec<String>, // все возможные адреса ячеек
    cells: HashMap<String, Cell>, // карта адрес -> ячейка
    total_capacity: usize, // общая вместимость склада
    total_used: usize, // сколько единиц товара уже занято
}

impl Warehouse {
fn new() -> Self {
    // параметры склада
    let zone_count = 2; // число зон
    let shelf_count = 19; // число стеллажей
    let section_count = 4; // число секций
    let shelf_per_section = 8; // число полок
    let capacity_per_cell = 10; // вместимость ячейки

    // генерируем все адреса ячеек
    let mut addresses = Vec::new();
    for z in 0..zone_count {
        let zone_char = (b'A' + z) as char; // зоны: A, B
        for s in 1..=shelf_count {
            for sec in 1..=section_count {
                for p in 1..=shelf_per_section {
                    let addr = format!("{}{}{}{}", zone_char, s, sec, p);
                    addresses.push(addr);
                }
            }
        }
    }

    // считаем общую вместимость с проверкой переполнения
    let total_capacity: usize = (zone_count as usize)
        .checked_mul(shelf_count as usize)
        .and_then(|v| v.checked_mul(section_count as usize))
        .and_then(|v| v.checked_mul(shelf_per_section as usize))
        .and_then(|v| v.checked_mul(capacity_per_cell as usize))
        .expect("Capacity calculation overflowed");

    Self {
        all_addresses: addresses,
        cells: HashMap::new(),
        total_capacity,
        total_used: 0,
    }
}
    // получаем mutable ссылку на ячейку по адресу, создаем если нет
    fn get_cell(&mut self, addr: &str) -> &mut Cell {
        self.cells.entry(addr.to_string()).or_insert_with(Cell::new)
    }

    // добавляем товар
    fn add_item(&mut self, name: &str, count: usize, addr: &str) {
        let cell = self.get_cell(addr);
        let free_space = 10 - cell.count; // свободное место
        if count > free_space {
            println!("недостаточно места в ячейке");
            return;
        }
        cell.name = name.to_string();
        cell.count += count;
        self.update_usage();
        println!("товар добавлен");
    }

    // удаляем товар
    fn remove_item(&mut self, name: &str, count: usize, addr: &str) {
        if let Some(cell) = self.cells.get_mut(addr) {
            if cell.count < count || cell.name != name {
                println!("недостаточно товара для списания");
                return;
            }
            cell.count -= count;
            if cell.count == 0 {
                cell.name.clear();
            }
            self.update_usage();
            println!("товар удален");
        } else {
            println!("ячейка не найдена");
        }
    }

    // обновляем общее использованное количество
    fn update_usage(&mut self) {
        self.total_used = self
            .cells
            .values()
            .map(|cell| cell.count)
            .sum();
    }

    // выводим информацию о состоянии склада
    fn info(&self) {
        // расчет общего процента заполнения
        let percent_total = (self.total_used as f64 / self.total_capacity as f64) * 100.0;
        println!("общий процент заполнения склада: {:.2}%", percent_total);

        // расчет заполненности по зонам
        let zone_count = 2;
        let cells_per_zone = self.all_addresses.len() / zone_count;

        for z in 0..zone_count {
            let start_idx = z * cells_per_zone;
            let end_idx = start_idx + cells_per_zone;
            let zone_addresses = &self.all_addresses[start_idx..end_idx];

            let zone_used: usize = zone_addresses.iter()
                .filter_map(|addr| self.cells.get(addr))
                .map(|cell| cell.count)
                .sum();

            let zone_percent = (zone_used as f64 / self.total_capacity as f64) * 100.0;
            println!("зона {} заполнена на {:.2}%", (b'A' + z as u8) as char, zone_percent);
        }

        // вывод ячеек с товаром
        println!("ячейки с товаром:");
        let mut has_items = false;
        for (addr, cell) in &self.cells {
            if cell.count > 0 {
                println!("адрес: {}, товар: {}, количество: {}", addr, cell.name, cell.count);
                has_items = true;
            }
        }
        if !has_items {
            println!("нет товаров на складе");
        }

        // вывод пустых ячеек через запятую
        print!("пустые ячейки: ");
        let mut first = true;
        for addr in &self.all_addresses {
            let cell = self.cells.get(addr);
            if cell.is_none() || cell.unwrap().count == 0 {
                if !first {
                    print!(", ");
                }
                print!("{}", addr);
                first = false;
            }
        }
        if first {
            print!("все ячейки заняты");
        }
        println!();
    }
}

fn main() {
    let mut warehouse = Warehouse::new();

    println!("система учета товаров на складе");
    println!("доступные команды:");
    println!("ADD <наименование> <количество> <адрес>");
    println!("REMOVE <наименование> <количество> <адрес>");
    println!("INFO");
    println!("EXIT");

    let stdin = io::stdin();

    loop {
        print!("\nвведите команду: ");
        io::stdout().flush().unwrap();

        let mut line = String::new();
        stdin.read_line(&mut line).unwrap();
        let line = line.trim();

        if line.eq_ignore_ascii_case("EXIT") {
            break;
        }

        let parts: Vec<&str> = line.split_whitespace().collect();
        if parts.is_empty() {
            continue;
        }

        match parts[0].to_uppercase().as_str() {
            "ADD" => {
                if parts.len() != 4 {
                    println!("некорректный формат команды");
                    continue;
                }
                let name = parts[1];
                let count: usize = match parts[2].parse() {
                    Ok(c) => c,
                    Err(_) => {
                        println!("некорректное число");
                        continue;
                    }
                };
                let addr = parts[3];
                warehouse.add_item(name, count, addr);
            }
            "REMOVE" => {
                if parts.len() != 4 {
                    println!("некорректный формат команды");
                    continue;
                }
                let name = parts[1];
                let count: usize = match parts[2].parse() {
                    Ok(c) => c,
                    Err(_) => {
                        println!("некорректное число");
                        continue;
                    }
                };
                let addr = parts[3];
                warehouse.remove_item(name, count, addr);
            }
            "INFO" => {
                warehouse.info();
            }
            _ => {
                println!("неверная команда");
            }
        }
    }
}
