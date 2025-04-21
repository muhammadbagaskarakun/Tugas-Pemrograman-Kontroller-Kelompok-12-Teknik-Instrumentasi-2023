#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ADC_MAX 4095U
#define VREF 3.3F               // Tegangan referensi ADC
#define HANDLE_MAX_CM 10.0F     // Maksimal pergerakan handle (dari 0 hingga 10 cm)
#define PWM_MIN 1000U
#define PWM_MAX 2000U

void set_servo_position(uint32_t pulse_us, float handle_cm);

int main(void)
{
    while (1)
    {
        float input_voltage = -1.0F;

        printf("Masukkan nilai Tegangan (0.0 - %.1f V, ketik -1 untuk keluar): ", VREF);

        if (scanf("%f", &input_voltage) != 1) {
            printf("Nilai tidak valid. Coba lagi.\n");
            while (getchar() != '\n'); // Clear buffer
            continue;
        }

        if (input_voltage == -1.0F) {
            printf("Simulasi selesai.\n");
            break;
        }

        if (input_voltage < 0.0F || input_voltage > VREF) {
            printf("Tegangan di luar batas (0 - %.1f V). Coba lagi.\n", VREF);
            continue;
        }

        // Konversi Tegangan ke nilai ADC
        uint32_t adc_value = (uint32_t)((input_voltage / VREF) * ADC_MAX);

        // Hitung PWM dari ADC
        uint32_t pulse = PWM_MIN + ((adc_value * (PWM_MAX - PWM_MIN)) / ADC_MAX);

        // Hitung posisi handle dalam cm
        float handle_cm = ((float)(pulse - PWM_MIN) / (PWM_MAX - PWM_MIN)) * HANDLE_MAX_CM;

        // Tampilkan hasil
        printf("Tegangan: %.2f V | Nilai ADC: %lu\n", input_voltage, adc_value);
        printf("PWM: %lu | Jarak Handle: %.2f cm\n", pulse, handle_cm);

        set_servo_position(pulse, handle_cm);
        printf("\n");
    }

    return 0;
}

void set_servo_position(uint32_t pulse_us, float handle_cm)
{
    printf("==> Servo di-set ke %lu µs (%.2f cm dari posisi terendah)\n", pulse_us, handle_cm);
}
