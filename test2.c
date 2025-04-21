#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ADC_THRESHOLD 500U
#define ADC_MAX 4095U
#define VREF 3.3F               // Tegangan referensi ADC
#define HANDLE_MAX_CM 10.0F     // Maksimal pergerakan handle

void set_servo_position(uint32_t pulse_us, float handle_cm);

int main(void)
{
    uint32_t adc_value = 0U;

    while (1)
    {
        printf("Masukkan nilai Tekanan (0 - %u, ketik -1 untuk keluar): ", ADC_MAX);
        int input = -1;

        if (scanf("%d", &input) != 1) {
            printf("nilai tidak valid. Coba lagi.\n");
            while (getchar() != '\n'); // Clear buffer
            continue;
        }

        if (input == -1) {
            printf("Simulasi selesai.\n");
            break;
        }

        if (input < 0 || input > (int)ADC_MAX) {
            printf("Nilai Tekanan di luar kapasitas. Coba lagi.\n");
            continue;
        }

        adc_value = (uint32_t)input;
        uint32_t pulse = 1500U;
        float voltage = (adc_value * VREF) / ADC_MAX;
        float handle_cm = 0.0F;

        printf("Nilai Tekanan: %lu | Tegangan ADC: %.2f V\n", adc_value, voltage);

        if (adc_value >= ADC_THRESHOLD) {
            pulse = 1000U + ((adc_value * 1000U) / ADC_MAX);
            handle_cm = ((float)(pulse - 1000U) / 1000.0F) * HANDLE_MAX_CM;
            printf("Tekanan mencukupi. Handle bergerak ke posisi PWM: %lu | Jarak: %.2f cm\n", pulse, handle_cm);
        } else {
            pulse = 1500U;
            handle_cm = ((float)(pulse - 1000U) / 1000.0F) * HANDLE_MAX_CM;
            printf("Tekanan rendah. Handle tetap di posisi netral: %lu | Jarak: %.2f cm\n", pulse, handle_cm);
        }

        set_servo_position(pulse, handle_cm);
        printf("\n");
    }

    return 0;
}

void set_servo_position(uint32_t pulse_us, float handle_cm)
{
    printf("==> Servo di-set ke %lu µs (%.2f cm dari posisi terendah)\n", pulse_us, handle_cm);
}
