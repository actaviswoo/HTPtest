/*
Напишите класс на C++, который реализует простую систему управления базой данных студентов. Каждый студент представлен структурой Student, 
содержащей следующие поля: id (целочисленный идентификатор), name (строка с именем студента), и age (целочисленный возраст студента).
 Ваш класс должен обеспечивать следующие возможности:

- Добавление нового студента в базу данных. 

- Удаление студента по идентификатору.

- Получение информации о студенте по идентификатору. 

- Используйте умный указатель std::shared_ptr для хранения и управления объектами студентов в контейнере внутри класса 
(например, std::vector или std::map). 
*/

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

struct Student {
    unsigned id;
    std::string name;
    unsigned age;
};

class StudentDatabase {
public:
    void addStudent(const std::string& name, int age) {
        std::shared_ptr<Student> newStudent = std::make_shared<Student>();
        newStudent->id = id_++;
        newStudent->name = name;
        newStudent->age = age;
        students_.push_back(newStudent);
    }

    void removeStudent(int id) {
        auto it = std::remove_if(students_.begin(), students_.end(),
                                 [id](const std::shared_ptr<Student>& student) {
                                     return student->id == id;
                                 });
        students_.erase(it, students_.end());
    }

    std::shared_ptr<Student> getStudentById(int id) {
        auto it = std::find_if(students_.begin(), students_.end(),
                               [id](const std::shared_ptr<Student>& student) {
                                   return student->id == id;
                               });
        if (it != students_.end()) {
            return *it;
        } else {
            return nullptr;
        }
    }
private:
    std::vector<std::shared_ptr<Student>> students_;
    int id_ = 0;
};

int main() {
    StudentDatabase database;
    database.addStudent("old", 30);
    database.addStudent("mid", 20);
    database.addStudent("young", 10);
    database.removeStudent(2);
    std::shared_ptr<Student> student = database.getStudentById(1);
    if (student != nullptr) {
        std::cout << "name: " << student->name << ", age: " << student->age << std::endl;
    } else {
        std::cout << "not found" << std::endl;
    }
    return 0;
}