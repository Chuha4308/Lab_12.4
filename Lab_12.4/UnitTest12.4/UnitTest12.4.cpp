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

        // ���� ��� �������� ��������� ������ � ����
        TEST_METHOD(TestAddProduct)
        {
            const string test_file = "test_prices.bin";

            // ��������� ������� �����, ���� ����
            remove(test_file.c_str());

            // ������ �������� �������
            ofstream f(test_file, ios::binary);
            Assert::IsTrue(f.is_open(), L"������� �������� ����� ��� ������.");

            Price product = { "Apple", "Supermarket", 10.50 };
            f.write(reinterpret_cast<const char*>(&product), sizeof(Price));
            f.close();

            // �������� ������
            ifstream file(test_file, ios::binary);
            Assert::IsTrue(file.is_open(), L"������� �������� ����� ��� �������.");

            Price read_product;
            file.read(reinterpret_cast<char*>(&read_product), sizeof(Price));
            file.close();

            // �������� ���������� ��������� �����
            Assert::AreEqual(string("Apple"), string(read_product.product_name));
            Assert::AreEqual(string("Supermarket"), string(read_product.store_name));
            Assert::AreEqual(10.50, read_product.price);

            remove(test_file.c_str()); // ��������� ��������� �����
        }

        // ���� ��� �������� ������ ��������� ��������
        TEST_METHOD(TestSearchProductFound)
        {
            const string test_file = "test_prices.bin";

            // ��������� ��������� �����
            ofstream f(test_file, ios::binary);
            Assert::IsTrue(f.is_open(), L"������� �������� ����� ��� ������.");

            Price product1 = { "Apple", "Supermarket", 10.50 };
            Price product2 = { "Banana", "Supermarket", 15.75 };
            f.write(reinterpret_cast<const char*>(&product1), sizeof(Price));
            f.write(reinterpret_cast<const char*>(&product2), sizeof(Price));
            f.close();

            // ��������������� cout � �����
            streambuf* original_cout = cout.rdbuf();
            ostringstream oss;
            cout.rdbuf(oss.rdbuf());

            // ������ ������� ������
            SearchProduct(test_file, "Supermarket");

            // ³��������� ������������ ������
            cout.rdbuf(original_cout);

            // �������� ����������
            string result = oss.str();
            Assert::IsTrue(result.find("Apple") != string::npos, L"����� Apple �� ��������.");
            Assert::IsTrue(result.find("Banana") != string::npos, L"����� Banana �� ��������.");

            remove(test_file.c_str()); // ��������� ��������� �����
        }

        // ���� ��� �������� ������ ���������� ��������
        TEST_METHOD(TestSearchProductNotFound)
        {
            const string test_file = "test_prices.bin";

            // ��������� ��������� �����
            ofstream f(test_file, ios::binary);
            Assert::IsTrue(f.is_open(), L"������� �������� ����� ��� ������.");

            Price product1 = { "Apple", "Supermarket", 10.50 };
            f.write(reinterpret_cast<const char*>(&product1), sizeof(Price));
            f.close();

            // ��������������� cout � �����
            streambuf* original_cout = cout.rdbuf();
            ostringstream oss;
            cout.rdbuf(oss.rdbuf());

            // ������ ������� ������
            SearchProduct(test_file, "NonExistentStore");

            // ³��������� ������������ ������
            cout.rdbuf(original_cout);

            // �������� ����������
            string result = oss.str();
            Assert::IsTrue(result.find("������� � ����� ������ �� ��������.") != string::npos, L"����������� ����������� ��� ���������� ��������.");

            remove(test_file.c_str()); // ��������� ��������� �����
        }
    };
}
