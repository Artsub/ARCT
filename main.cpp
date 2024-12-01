#include <iostream>
#include <fstream>
#include <cmath>

const double g0 = 9.81;         // Ускорение свободного падения на уровне моря (м/с^2)
const double R = 6371000;       // Радиус Земли (м)
const double u = 3000;          // Скорость истечения газов (м/с)
const double Cd = 0.5;          // Коэффициент лобового сопротивления
const double A = 11.4;          // Площадь поперечного сечения ракеты (м^2)
const double rho0 = 1.225;      // Плотность воздуха на уровне моря (кг/м^3)
const double H = 8500;          // Высота масштаба атмосферы (м)
const double burn_rate = 2500;  // Скорость сжигания топлива (кг/с)
const double dt = 0.1;          // Шаг времени (с)

// Функция для расчета плотности воздуха в зависимости от высоты
double airDensity(double h) {
    return rho0 * exp(-h / H);
}

// Основная функция для моделирования
void simulateRocket() {
    std::fstream output("output.txt");
    // Начальные параметры
    double m0 = 549000;         // Полная масса ракеты (кг)
    double mf = 25000;          // Масса пустой ракеты (кг)
    double mass = m0;           // Текущая масса
    double h = 0;               // Высота (м)
    double v = 0;               // Скорость (м/с)
    double time = 0;            // Время (с)
    
    if(output.is_open()){
    output << "Time(s)    Height(m)    Speed(m/s)    Mass(kg)\n";

    while (mass > mf) {
        // Расчет плотности воздуха и гравитационного ускорения
        double rho = airDensity(h);
        double g = g0 * pow(R / (R + h), 2);

        // Сопротивление воздуха
        double drag = 0.5 * rho * Cd * A * v * v;

        // Дифференциальное уравнение движения
        double dv = (u * burn_rate / mass) - g - (drag / mass);
        double dh = v;

        // Обновление параметров
        v += dv * dt;
        h += dh * dt;
        mass -= burn_rate * dt;
        time += dt;

        // Вывод текущих значений
        output << time << "    " << h << "    " << v << "    " << mass << std::endl;

        // Прерывание при достижении орбиты
        if (v >= 7800) {
            output << "Orbit achieved!\n";
            break;
        }
    }
    output.close();
    }
}

int main() {
    simulateRocket();
    return 0;
}