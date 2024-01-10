#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>


class CircuitCalculator 
{
private:
    float L, Z, XC, f, Vmax, Imax, theta, VRmax, VLmax, VCmax;
    float R, C, XL;

public:
    CircuitCalculator() : L(0.6), Vmax(150), f(60.0), XC(0.0), Imax(0.0), theta(0.0), VRmax(0.0), VLmax(0.0), VCmax(0.0), R(0.0), C(0.0), XL(0.0) {}

    void setRCValues() 
    {
        std::cout << "R (Ohm): ";
        std::cin >> R;
        std::cout << "C (Farad): ";
        std::cin >> C;
    }

    void calculateCircuitParameters() {
        XL = 2 * M_PI * f * L;
        XC = 1 / (2 * M_PI * f * C);
        Z = sqrt((R * R) + ((XL - XC) * (XL - XC)));
        Imax = Vmax / Z;
        theta = atan((XL - XC) / R) * (180 / M_PI);
        VRmax = Imax * R;
        VLmax = Imax * XL;
        VCmax = Imax * XC;
    }

    void displayResults() 
    {
        std::cout << "유도 리액턴스(XL): " << XL << " (Ohm)\n";
        std::cout << "용량 리액턴스 (XC) : " << XC << " (Ohm)\n";
        std::cout << "최대전류 (Imax) : " << Imax << "(A)\n";
        std::cout << "임피던스 (Z) : " << Z << " (Ohm)\n";
        std::cout << "위상각 (theta): " << theta << " (degree)\n";
        std::cout << "VRmax : " << VRmax << " (Volt)\n";
        std::cout << "VLmax : " << VLmax << " (Volt)\n";
        std::cout << "VCmax : " << VCmax << " (Volt)\n";
    }
};

int main() 
{
    CircuitCalculator circuit;
    circuit.setRCValues();
    circuit.calculateCircuitParameters();
    circuit.displayResults();

    return 0;
}
