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
        float voltage = (adc_value * VREF) / ADC_MAX;

        // Konversi langsung ADC → PWM secara linier
        uint32_t pulse = PWM_MIN + ((adc_value * (PWM_MAX - PWM_MIN)) / ADC_MAX);

        // Konversi PWM → cm (0–10 cm linier)
        float handle_cm = ((float)(pulse - PWM_MIN) / (PWM_MAX - PWM_MIN)) * HANDLE_MAX_CM;

        // Tampilkan hasil
        printf("Nilai Tekanan: %lu | Tegangan ADC: %.2f V\n", adc_value, voltage);
        printf("PWM: %lu | Jarak Handle: %.2f cm\n", pulse, handle_cm);

        set_servo_position(pulse, handle_cm);
        printf("\n");
    }

    return 0;
}

void set_servo_position(uint32_t pulse_us, float handle_cm)
{
    printf("==> Servo di-set ke %lu (%.2f cm dari posisi terendah)\n", pulse_us, handle_cm);
}
