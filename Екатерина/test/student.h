#ifndef STUDENT_H
#define STUDENT_H

#include <string>

class student {
    float averageMark;
    unsigned int age;


    /*float getAverageMark( bool sex ) //-- COMPILE ERROR
    {
        if (sex)
            return averageMark;
        else
            return averageMark + 1;
    }*/

public:
    student & operator=( student &s ) = delete;

    float getAverageMark( void )
    {
        return averageMark;
    }

    int getAverageMark( bool sex ) const // метод, который не меняет данный экземмпляр
    {
        if (sex)
            return int(averageMark);
        else
            return int(averageMark + 1);
    }

    explicit student( bool s )
    {
        sex = s;
        this->age = 20; // получение доступа к полю ДАННОГО ЭКЗЕМПЛЯРА
        // this - указатель на данный экземпляр класса
        // student *this неявно передаётся аргументом во все методы класса
        // в функцию с модификатором const передаётся student const * this
        age = 20;
    }

    explicit student( float avgMark )
    {
        averageMark = avgMark;
    }

    ~student( void )
    {
        age = 0;
    }
protected:
    bool sex; // false - male true - female
}; // инкапсуляция

class olymp_student : public student, public std::string // наследование
{

    // порядок вызова конструкторов
    // сначала - контруструкторы родителей в порядке объявления в ШАПКЕ КЛАССА (строка 53)
    // далее вызываются конструкторы полей в порядке объявления (сначла предмет, потом число решенных задач)
    // далее вызывается тело самого конструктора (между {})
    // вызов деструкторов идёт в обратном порядке
    olymp_student( std::string const &name ) :
        std::string("aaa"), student(5.0f), successfulSolvedProblems(40), subjectToParticipate("maths")
    {
        subjectToParticipate = "russian";
        if (name.back() == 'a')
            sex = true;
        else
            sex = false;
    }
private:
    std::string subjectToParticipate;
    int successfulSolvedProblems;
};

#endif // STUDENT_H
