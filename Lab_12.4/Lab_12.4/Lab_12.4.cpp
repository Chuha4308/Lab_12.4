#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <windows.h>

using namespace std;

// Структура для зберігання даних про товар
struct Price {
    char product_name[50];
    char store_name[50];
    double price;
};

// Функції для роботи з типізованим файлом
void AddProduct(const string& filename);
void PrintProducts(const string& filename);
void SortProducts(const string& filename);
void SearchProduct(const string& filename, const string& store_name);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string filename;
    cout << "Введіть ім'я файлу: ";
    cin >> filename;

    int menuItem;
    do {
        cout << endl << "Оберіть дію:" << endl;
        cout << "1 - Додати інформацію про товар" << endl;
        cout << "2 - Вивести інформацію про товари" << endl;
        cout << "3 - Впорядкувати товари за алфавітом" << endl;
        cout << "4 - Пошук товарів за назвою магазину" << endl;
        cout << "0 - Завершити роботу програми" << endl;
        cout << "Введіть: ";
        cin >> menuItem;

        switch (menuItem) {
        case 1:
            AddProduct(filename);
            break;
        case 2:
            PrintProducts(filename);
            break;
        case 3:
            SortProducts(filename);
            break;
        case 4: {
            cin.ignore();
            string store_name;
            cout << "Введіть назву магазину для пошуку: ";
            getline(cin, store_name);
            SearchProduct(filename, store_name);
            break;
        }
        case 0:
            cout << "Завершення програми." << endl;
            break;
        default:
            cout << "Невірний вибір, спробуйте ще раз." << endl;
        }
    } while (menuItem != 0);

    return 0;
}

void AddProduct(const string& filename) {
    ofstream file(filename, ios::binary | ios::app);
    if (!file) {
        cerr << "Помилка відкриття файлу." << endl;
        return;
    }

    Price product;
    cin.ignore();
    cout << "Назва товару: ";
    cin.getline(product.product_name, 50);
    cout << "Назва магазину: ";
    cin.getline(product.store_name, 50);
    cout << "Вартість товару: ";
    cin >> product.price;

    file.write(reinterpret_cast<const char*>(&product), sizeof(Price));
    file.close();
}

void PrintProducts(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Помилка відкриття файлу." << endl;
        return;
    }

    Price product;
    cout << "=================================================================" << endl;
    cout << "|  №  |   Назва товару   |   Назва магазину |  Вартість товару  |" << endl;
    cout << "-----------------------------------------------------------------" << endl;

    int index = 1;
    while (file.read(reinterpret_cast<char*>(&product), sizeof(Price))) {
        cout << "|" << setw(4) << right << index++ << " ";
        cout << "|" << setw(18) << left << product.product_name;
        cout << "|" << setw(18) << left << product.store_name;
        cout << "|" << setw(18) << right << product.price << " |" << endl;
    }

    cout << "=================================================================" << endl;
    file.close();
}

void SortProducts(const string& filename) {
    fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file) {
        cerr << "Помилка відкриття файлу." << endl;
        return;
    }

    file.seekg(0, ios::end);
    size_t count = file.tellg() / sizeof(Price);
    Price* products = new Price[count];

    file.seekg(0, ios::beg);
    file.read(reinterpret_cast<char*>(products), count * sizeof(Price));

    // Сортування за алфавітом
    for (size_t i = 0; i < count - 1; i++) {
        for (size_t j = 0; j < count - i - 1; j++) {
            if (string(products[j].product_name) > string(products[j + 1].product_name)) {
                swap(products[j], products[j + 1]);
            }
        }
    }

    file.seekp(0, ios::beg);
    file.write(reinterpret_cast<const char*>(products), count * sizeof(Price));
    file.close();

    delete[] products;
    cout << "Сортування виконано." << endl;
}

void SearchProduct(const string& filename, const string& store_name) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Помилка відкриття файлу." << endl;
        return;
    }

    Price product;
    bool found = false;

    cout << "=================================================================" << endl;
    cout << "|  №  |   Назва товару   |   Назва магазину |  Вартість товару  |" << endl;
    cout << "-----------------------------------------------------------------" << endl;

    int index = 1;
    while (file.read(reinterpret_cast<char*>(&product), sizeof(Price))) {
        if (store_name == product.store_name) {
            found = true;
            cout << "|" << setw(4) << right << index++ << " ";
            cout << "|" << setw(18) << left << product.product_name;
            cout << "|" << setw(18) << left << product.store_name;
            cout << "|" << setw(18) << right << product.price << " |" << endl;
        }
    }

    if (!found) {
        cout << "Магазин з такою назвою не знайдено." << endl;
    }

    cout << "=================================================================" << endl;
    file.close();
}
