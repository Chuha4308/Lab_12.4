#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <windows.h>

using namespace std;

// ��������� ��� ��������� ����� ��� �����
struct Price {
    char product_name[50];
    char store_name[50];
    double price;
};

// ������� ��� ������ � ���������� ������
void AddProduct(const string& filename);
void PrintProducts(const string& filename);
void SortProducts(const string& filename);
void SearchProduct(const string& filename, const string& store_name);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string filename;
    cout << "������ ��'� �����: ";
    cin >> filename;

    int menuItem;
    do {
        cout << endl << "������ ��:" << endl;
        cout << "1 - ������ ���������� ��� �����" << endl;
        cout << "2 - ������� ���������� ��� ������" << endl;
        cout << "3 - ������������ ������ �� ��������" << endl;
        cout << "4 - ����� ������ �� ������ ��������" << endl;
        cout << "0 - ��������� ������ ��������" << endl;
        cout << "������: ";
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
            cout << "������ ����� �������� ��� ������: ";
            getline(cin, store_name);
            SearchProduct(filename, store_name);
            break;
        }
        case 0:
            cout << "���������� ��������." << endl;
            break;
        default:
            cout << "������� ����, ��������� �� ���." << endl;
        }
    } while (menuItem != 0);

    return 0;
}

void AddProduct(const string& filename) {
    ofstream file(filename, ios::binary | ios::app);
    if (!file) {
        cerr << "������� �������� �����." << endl;
        return;
    }

    Price product;
    cin.ignore();
    cout << "����� ������: ";
    cin.getline(product.product_name, 50);
    cout << "����� ��������: ";
    cin.getline(product.store_name, 50);
    cout << "������� ������: ";
    cin >> product.price;

    file.write(reinterpret_cast<const char*>(&product), sizeof(Price));
    file.close();
}

void PrintProducts(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "������� �������� �����." << endl;
        return;
    }

    Price product;
    cout << "=================================================================" << endl;
    cout << "|  �  |   ����� ������   |   ����� �������� |  ������� ������  |" << endl;
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
        cerr << "������� �������� �����." << endl;
        return;
    }

    file.seekg(0, ios::end);
    size_t count = file.tellg() / sizeof(Price);
    Price* products = new Price[count];

    file.seekg(0, ios::beg);
    file.read(reinterpret_cast<char*>(products), count * sizeof(Price));

    // ���������� �� ��������
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
    cout << "���������� ��������." << endl;
}

void SearchProduct(const string& filename, const string& store_name) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "������� �������� �����." << endl;
        return;
    }

    Price product;
    bool found = false;

    cout << "=================================================================" << endl;
    cout << "|  �  |   ����� ������   |   ����� �������� |  ������� ������  |" << endl;
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
        cout << "������� � ����� ������ �� ��������." << endl;
    }

    cout << "=================================================================" << endl;
    file.close();
}
