/*
Разработайте программу, демонстрирующую создание двух потоков в C++, где один поток пишет данные в переменную, 
а другой читает из нее, с использованием мьютексов для синхронизации доступа.
*/

#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;
int sharedData = 0;

void writerFunction() {
    for (int i = 0; i < 5; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            sharedData = i;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void readerFunction() {
    for (int i = 0; i < 5; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            int data = sharedData;
            std::cout << "Reader read: " << data << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

int main() {
    std::thread writerThread(writerFunction);
    std::thread readerThread(readerFunction);
    writerThread.join();
    readerThread.join();
    return 0;
}