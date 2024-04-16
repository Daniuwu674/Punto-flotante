#include <iostream>
#include <bitset>

// Función para multiplicar dos números en punto flotante de 32 bits
void multiplicar(float num1, float num2, bool& signo, int& exponente, unsigned int& significando) {
    
    unsigned int bits_num1 = *reinterpret_cast<unsigned int*>(&num1);
    unsigned int bits_num2 = *reinterpret_cast<unsigned int*>(&num2);

    bool signo_num1 = (bits_num1 >> 31) & 1;
    bool signo_num2 = (bits_num2 >> 31) & 1;
    int exponente_num1 = (bits_num1 >> 23) & 0xFF;
    int exponente_num2 = (bits_num2 >> 23) & 0xFF;
    unsigned int significando_num1 = bits_num1 & 0x7FFFFF;
    unsigned int significando_num2 = bits_num2 & 0x7FFFFF;

    // Paso 1: Verificar si alguno de los valores ingresados es 0
    if (exponente_num1 == 0 && significando_num1 == 0) {
        signo = false;
        exponente = 0;
        significando = 0;
        return;
    }
    if (exponente_num2 == 0 && significando_num2 == 0) {
        signo = false; 
        exponente = 0;
        significando = 0;
        return;
    }

    // Paso 2: Suma binaria de los exponentes y verificaciones
    int exponente_resultante = exponente_num1 + exponente_num2 - 127; 
    if (exponente_resultante < 0) {
        signo = false; 
        exponente = 0; 
        significando = 0; 
        return;
    }
    if (exponente_resultante > 255) {
        signo = false;
        exponente = 255;
        significando = 0;
        return;
    }

    // Paso 3: Multiplicación de los significandos
    unsigned long long producto_significandos = static_cast<unsigned long long>(significando_num1) * significando_num2;

    // Paso 4: Normalización y redondeo
    while ((producto_significandos & 0x00800000) == 0) {
        producto_significandos <<= 1;
        --exponente_resultante;
    }
    significando = (producto_significandos >> 23) & 0x7FFFFF;
    if ((producto_significandos & 0x00000080) && ((producto_significandos & 0x0000007F) || (significando & 1))) {
        significando += 1;
        if (significando > 0x7FFFFF) {
            significando >>= 1;
            exponente_resultante += 1;
        }
    }

    // Paso 5: Ensamblaje del resultado final
    signo = signo_num1 ^ signo_num2; 
    exponente = exponente_resultante;
}

int main() {
    float a, b;
    bool signo_resultado; 
    int exponente_resultado; 
    unsigned int significando_resultado; 
    std::cout << "Ingrese el primer numero: ";
    std::cin >> a;
    std::cout << "Ingrese el segundo numero: ";
    std::cin >> b;

    multiplicar(a, b, signo_resultado, exponente_resultado, significando_resultado);

    std::cout << "El resultado de la multiplicacion es: ";
    std::cout << signo_resultado << " ";
    std::cout << std::bitset<8>(exponente_resultado) << " ";
    std::cout << std::bitset<23>(significando_resultado) << std::endl;

    return 0;
}