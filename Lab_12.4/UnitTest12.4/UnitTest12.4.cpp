#include "pch.h"
#include "CppUnitTest.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include "../Lab_12.4/Lab_12.4.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(UnitTest1)
    {
    public:

        // Тест для перевірки додавання товарів у файл
        TEST_METHOD(TestAddProduct)
        {
            const string test_file = "test_prices.bin";

            // Видалення старого файлу, якщо існує
            remove(test_file.c_str());

            // Додати тестовий продукт
            ofstream f(test_file, ios::binary);
            Assert::IsTrue(f.is_open(), L"Помилка відкриття файлу для запису.");

            Price product = { "Apple", "Supermarket", 10.50 };
            f.write(reinterpret_cast<const char*>(&product), sizeof(Price));
            f.close();

            // Перевірка запису
            ifstream file(test_file, ios::binary);
            Assert::IsTrue(file.is_open(), L"Помилка відкриття файлу для читання.");

            Price read_product;
            file.read(reinterpret_cast<char*>(&read_product), sizeof(Price));
            file.close();

            // Перевірка відповідності записаних даних
            Assert::AreEqual(string("Apple"), string(read_product.product_name));
            Assert::AreEqual(string("Supermarket"), string(read_product.store_name));
            Assert::AreEqual(10.50, read_product.price);

            remove(test_file.c_str()); // Видалення тестового файлу
        }

        // Тест для перевірки пошуку існуючого магазину
        TEST_METHOD(TestSearchProductFound)
        {
            const string test_file = "test_prices.bin";

            // Створення тестового файлу
            ofstream f(test_file, ios::binary);
            Assert::IsTrue(f.is_open(), L"Помилка відкриття файлу для запису.");

            Price product1 = { "Apple", "Supermarket", 10.50 };
            Price product2 = { "Banana", "Supermarket", 15.75 };
            f.write(reinterpret_cast<const char*>(&product1), sizeof(Price));
            f.write(reinterpret_cast<const char*>(&product2), sizeof(Price));
            f.close();

            // Перенаправлення cout у буфер
            streambuf* original_cout = cout.rdbuf();
            ostringstream oss;
            cout.rdbuf(oss.rdbuf());

            // Виклик функції пошуку
            SearchProduct(test_file, "Supermarket");

            // Відновлення стандартного потоку
            cout.rdbuf(original_cout);

            // Перевірка результату
            string result = oss.str();
            Assert::IsTrue(result.find("Apple") != string::npos, L"Товар Apple не знайдено.");
            Assert::IsTrue(result.find("Banana") != string::npos, L"Товар Banana не знайдено.");

            remove(test_file.c_str()); // Видалення тестового файлу
        }

        // Тест для перевірки пошуку неіснуючого магазину
        TEST_METHOD(TestSearchProductNotFound)
        {
            const string test_file = "test_prices.bin";

            // Створення тестового файлу
            ofstream f(test_file, ios::binary);
            Assert::IsTrue(f.is_open(), L"Помилка відкриття файлу для запису.");

            Price product1 = { "Apple", "Supermarket", 10.50 };
            f.write(reinterpret_cast<const char*>(&product1), sizeof(Price));
            f.close();

            // Перенаправлення cout у буфер
            streambuf* original_cout = cout.rdbuf();
            ostringstream oss;
            cout.rdbuf(oss.rdbuf());

            // Виклик функції пошуку
            SearchProduct(test_file, "NonExistentStore");

            // Відновлення стандартного потоку
            cout.rdbuf(original_cout);

            // Перевірка результату
            string result = oss.str();
            Assert::IsTrue(result.find("Магазин з такою назвою не знайдено.") != string::npos, L"Неправильне повідомлення для відсутнього магазину.");

            remove(test_file.c_str()); // Видалення тестового файлу
        }
    };
}
