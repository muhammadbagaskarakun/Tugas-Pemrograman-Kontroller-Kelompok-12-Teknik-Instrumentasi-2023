#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ADC_THRESHOLD 500U
#define ADC_MAX 4095U

void set_servo_position(uint32_t pulse_us);

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

        printf("Nilai Tekanan: %lu\n", adc_value);

        if (adc_value >= ADC_THRESHOLD) {
            pulse = 1000U + ((adc_value * 1000U) / ADC_MAX);
            printf("Tekanan mencukupi. handle bergerak ke posisi: %lu \n", pulse);
        } else {
            pulse = 1500U;
            printf("Tekanan rendah. handle tetap di posisi netral: %lu \n", pulse);
        }

        set_servo_position(pulse);
        printf("\n");
    }

    return 0;
}

void set_servo_position(uint32_t pulse_us)
{
    printf("==> posisi handle di-set ke %lu \n", pulse_us);
}
