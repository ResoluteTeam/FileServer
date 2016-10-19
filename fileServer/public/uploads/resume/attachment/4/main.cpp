#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
#include <termios.h>

//7. Створити типований файл записів, який містить дані про автомобілі. Поля #запису – модель (до 15 символів),
//номер (код регіону, цифровий код, буквенний #код), колір, дані про власника (прізвище, ім’я, по-батькові),
//дата останнього #техогляду. Довідки для пошуку:
//даних про всі автомобілі, які не пройшли техогляд в цьому році;
//інформації про всіх власників авто певного кольору;
//всю інформації про авто та її власника за  номером.



using namespace std;

struct TerminalOpt {
    TerminalOpt() {
        termios new_settings;
        tcgetattr(0,&stored_settings);
        new_settings = stored_settings;
        new_settings.c_lflag &= (~ICANON);
        new_settings.c_cc[VTIME] = 0;
        new_settings.c_cc[VMIN] = 1;
        tcsetattr(0,TCSANOW,&new_settings);
    }
    ~TerminalOpt() {
        tcsetattr(0,TCSANOW,&stored_settings);
    }
    termios stored_settings;
};

struct Car {
    string model;
    string number;
    string color;
    string holder;
    string date;
};

void addRecord();
void viewRecords(std::vector<Car *> arr);
void find(int selection);
void findByDate();
void findByColor();
void findByNumber();
void writeToFile();
void readFromFile();
bool keyX(unsigned char command);
bool keyEnter(unsigned char command);
bool keyV(unsigned char command);
bool keyF(unsigned char command);

std::vector<Car*> cars;

int main()
{
    setlocale(LC_ALL,"Russian");
    TerminalOpt term;
    bool exit = false;
        unsigned char commandChar;
        readFromFile();
        printf("\t\t\t\t\t\tАвтомобілі\n\n");
        printf("Натисніть Enter для додавання автомобіля, клавішу V для перегляду всіх автомобілів, F для пошуку та Х для виходу\n");
        while(!exit)
        {
            commandChar = getchar();
            if(keyEnter(commandChar))
            {
                addRecord();
            }
            else if(keyF(commandChar))
            {
                std::cout << "\n";
                system("clear");
                int i;
                std::cout << "Натисніть:\n1 - для виведення всіх автомобілів, що не пройшли техогляд в цьому році\n"
                             "2 - для пошуку автомобіля за кольором\n3 - для пошуку за номером\n";
                i = (int)getchar() - 48;
                std::cout << "\n";
                system("clear");
                find(i);
            }
            else if(keyV(commandChar))// Enter
            {
                viewRecords(cars);
            }
            else if (!exit)
            {
                fflush(stdin);
                printf("\n\t\t\t\t\t\tАвтомобілі");
                system("clear");
                printf("\nВи ввели неправильну команду.\n");
            }
            printf("Натисніть Enter для додавання автомобіля, клавішу V для перегляду всіх автомобілів, F для пошуку та Х для виходу\n");
            if(keyX(commandChar))// X
            {
                writeToFile();
                exit = true;
                printf("\n");
                system("clear");
            }
        }
    return 0;
}

int func(const char *str)
{
    int len = 0;

    while (*str != '\0') {
        if ((*str & 0xc0) == 0x80) {
            len++;
        }
        str++;
    }
    return len;
}

void find(int selection)
{
    switch (selection) {
    case 1:
        findByDate();
        break;
    case 2:
        findByColor();
        break;
    case 3:
        findByNumber();
        break;
    default:
        break;
    }
    getchar();
    std::cout << std::endl;
}

void findByDate()
{
    int year;
    std::cout << "Введіть рік: ";
    std::cin >> year;
    std::vector<Car*> temp;

    for(int i = 0; i < cars.size(); i++)
    {
        string carYear;

        for(int j = 0; j < 4; j++)
                carYear += cars.at(i)->date[6+j];

        if(stoi(carYear) < year)
        {

            std::string dr = cars.at(i)->date;
            temp.push_back(cars.at(i));
        }
    }
    viewRecords(temp);
}

void findByColor()
{
    std::string color;
    std::cout << "Введіть колір для пошуку: ";
    std::cin >> color;
    std::vector<Car*> temp;
    for(int i = 0; i < cars.size(); i++)
    {
        if(cars.at(i)->color == color)
        {
            temp.push_back(cars.at(i));
        }
    }
    viewRecords(temp);
}

void findByNumber()
{
    std::string number;
    std::cout << "Введіть номер для пошуку: ";
    std::cin >> number;
    std::vector<Car*> temp;
    for(int i = 0; i < cars.size(); i++)
    {
        if(cars.at(i)->number == number)
        {
            temp.push_back(cars.at(i));
        }
    }
    viewRecords(temp);
}

bool keyX(unsigned char command)
{
    if(command == 135 || command == 120 || command == 88 || command == 167)
    {
        return true;
    }
    return false;
}
bool keyEnter(unsigned char command)
{
    if(command == 10)
    {
        return true;
    }
    return false;
}
bool keyV(unsigned char command)
{
    if(command == 118 || command == 86 || command == 188 || command == 156)
    {
        return true;
    }
    return false;
}
bool keyF(unsigned char command)
{
    if(command == 102 || command == 70 || command == 176 || command == 144)
    {
        return true;
    }
    return false;
}

void readFromFile()
{
    std::string line;
    std::ifstream file ("cars.txt");
    if(file.is_open())
    {
        while (std::getline(file, line))
        {
            std::string temp;

            Car* car = new Car;

            int i = 0;
            while (line[i] != '~')
            {
                temp += line[i];
                i++;
            }
            car->model = temp;
            temp = "";
            i++;

            while (line[i] != '~')
            {
                temp += line[i];
                i++;
            }
            car->number = temp;
            temp = "";
            i++;

            while (line[i] != '~')
            {
                temp += line[i];
                i++;
            }
            car->color = temp;
            temp = "";
            i++;

            while (line[i] != '~')
            {
                temp += line[i];
                i++;
            }
            car->holder = temp;
            temp = "";
            i++;

            while (line[i] != '~')
            {
                temp += line[i];
                i++;
            }
            car->date = temp;
            temp = "";

            cars.push_back(car);
        }
        file.close();
    }
    else
    {
        std::cout << "Can't open file! Empty file created!\n";
        ofstream file("cars.txt");
        file.close();
        readFromFile();
    }
}

void writeToFile()
{
    std::ofstream file ("cars.txt");
    if(file.is_open())
    {
        for(int i = 0; i < cars.size(); i++)
        {
            file << cars.at(i)->model << "~";
            file << cars.at(i)->color << "~";
            file << cars.at(i)->number << "~";
            file << cars.at(i)->holder << "~";
            file << cars.at(i)->date << "~\n";
        }
        file.close();
    }
    else
    {
        std::cout << "Can't open file!";
    }
}

void addRecord()
{
    std::cout << "\n";
    system("clear");
    Car* car = new Car;

    cout << "Введіть модель авто: ";
    std::string temp;
    std::getline(std::cin, temp);
    car->model = temp;
    fflush(stdin);
    std::cin.clear();

    cout << "Введіть номер авто: ";
    cin >> car->number;

    cout << "Введіть кольор авто: ";
    cin >> car->color;

    cout << "Введіть власника авто: ";
    string temp2;
    getchar();
    std::getline(std::cin, temp2);
    car->holder = temp2;
    fflush(stdin);
    std::cin.clear();

    cout << "Введіть дату останнього техогляду: ";
    cin >> car->date;

    cars.push_back(car);
    getchar();
    fflush(stdin);
    std::cout << "\n";
    system("clear");
}

void viewRecords(std::vector<Car *> arr)
{
    if(!arr.size())
    {
        cout << "Немає записів.\n";
    }
    else{
        system("clear");
        cout << "\t\t\t\t\t\tАвтомобілі\n\n";

        printf("  %-*s", 20 + func("Модель") , "Модель");
        printf("  %-*s", 12 + func("Колір") , "Колір");
        printf("  %-*s ", 8 + func("Номер") , "Номер");
        printf("  %-*s ", 30 + func("Власник") , "Власник");
        printf("  %*s", 20 + func("Дата техогляду") , "Дата техогляду");
        cout << "\n";
        cout << "------------------------------------------------------------------------------------------------"
                "---\n";

        for(int i = 0; i < arr.size(); i++)
        {

            printf("| %-*s", 20 + func(arr.at(i)->model.c_str()) , arr.at(i)->model.c_str());
            printf("| %-*s", 12 + func(arr.at(i)->color.c_str()) , arr.at(i)->color.c_str());
            printf("| %-*s ", 8 + func(arr.at(i)->number.c_str()) , arr.at(i)->number.c_str());
            printf("| %-*s ", 30 + func(arr.at(i)->holder.c_str()) , arr.at(i)->holder.c_str());
            printf("| %-*s |", 15 + func(arr.at(i)->date.c_str()) , arr.at(i)->date.c_str());
            cout << "\n";
            cout << "------------------------------------------------------------------------------------------------"
                    "---\n";
        }
    }

}
