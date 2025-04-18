# Kruk Cerdas Berbasis Sensor Tekanan untuk Mencegah Cedera Kronis pada Pengguna Kruk

## Deskripsi Proyek

Proyek ini merupakan implementasi dari alat bantu jalan kruk cerdas berbasis mikrokontroler STM32 Nucleo F446RE, yang dilengkapi dengan sensor tekanan FSR 402 dan motor servo MG996R. Sistem ini dirancang untuk membantu pengguna kruk dalam mendistribusikan tekanan secara optimal guna mencegah cedera kronis, terutama akibat penggunaan kruk dalam jangka panjang dan beban yang tidak merata.
Logika sistem menggunakan pembacaan ADC dari sensor FSR untuk mengontrol posisi servo melalui sinyal PWM. Tujuan utama adalah menciptakan sistem kruk yang dapat beradaptasi secara dinamis terhadap tekanan yang diberikan oleh pengguna.
Anggota Kelompok:
Muhammad Aufa Affandi (2042231011)
Muhammad Bagaskara (2042231017)
Ziyad Zaky Permana (2042231077)
---

## Tujuan

- Mendeteksi besaran tekanan dari pengguna menggunakan sensor FSR.
- Mengatur posisi atau resistansi kruk secara otomatis dengan motor servo berbasis logika pembacaan tekanan.
- Memberikan umpan balik terhadap beban berlebih melalui mekanisme kontrol adaptif.
- Memungkinkan debugging dan analisis script melalui Compiler C atau percobaan langsung

---

## Perangkat Keras yang Digunakan

| Komponen            | Spesifikasi Teknis                                               |
|---------------------|------------------------------------------------------------------|
| Mikrokontroler      | STM32 Nucleo F446RE (dengan STM32CubeMX dan HAL Library)                    |
| Sensor Tekanan      | FSR 402 (Force Sensitive Resistor)                              |
| Aktuator            | Servo Motor MG996R                                              |
| Komunikasi Serial   | UART USART2                                                     |
| Regulator Tegangan  | 5V / 3.3V (tergantung board STM32 yang digunakan)               |
| Sumber Daya         | USB 5V / Adaptor eksternal                                       |

---

## Perangkat Lunak

### Tools

- Keil uVision dengan file project `.uvprojx` yang dihasilkan dari STM32CubeMX
- STM32CubeMX untuk konfigurasi pin, clock, dan peripheral
- VSCode untuk pengembangan atau simulasi logika terpisah
- GNU GCC (untuk simulasi C)

---

## File Utama

- `main.c`: Program utama yang mengatur siklus kerja pembacaan sensor, dan logika pemetaan ke PWM servo
- `main.h`: Header utama (hanya digunakan saat menggunakan Keil / STM32)
- `Tubes-2 pemkon kel12.uvprojx`: File proyek STM32 untuk Keil uVision IDE

---

## Struktur Direktori

```
.
├── Core/
│   ├── Inc/
│   │   └── main.h
│   └── Src/
│       └── main.c
├── Tubes-2 pemkon kel12.uvprojx
├── README.md
```

---

## Penjelasan Alur Program

1. Inisialisasi seluruh peripheral (GPIO, ADC, TIM, UART, DMA).
2. Pembacaan nilai tekanan dari sensor FSR melalui ADC1.
3. Mapping nilai ADC (0–4095) ke durasi pulsa PWM servo (1000–2000 µs).
4. Servo disesuaikan berdasarkan nilai tekanan.
5. Siklus dijalankan terus-menerus dengan delay stabilisasi 50 ms.

```c
// Mapping nilai ADC ke durasi pulse (PWM servo)
pulse = 1000 + (adcValue * 1000 / 4095);
__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse);
```

---

## Konfigurasi Periferal STM32 (dari CubeMX)

- **ADC1 (PA0 - FSR input)**
  - Resolution: 12-bit
  - Trigger: Software
  - Mode: Single Conversion

- **TIM2 - PWM Output (PA5 atau sesuai pin servo)**
  - Prescaler: 83 (1 MHz timer clock)
  - Period: 19999 (20 ms PWM frame)
  - Channel 1: PWM Mode 1

- **USART2 (PA2 TX, PA3 RX)**
  - Baudrate: 115200
  - Mode: TX & RX

- **DMA** (untuk komunikasi UART - optional)

---

## Kompilasi dan Deploy
![Screenshot 2025-04-16 132828](https://github.com/user-attachments/assets/c4a6eb6d-043b-49d2-a59b-2d5afcec2367)

### Menggunakan Keil uVision

1. Buka `Tubes-2 pemkon kel12.uvprojx`
2. Klik "Rebuild All"
3. Hubungkan board STM32 ke komputer
4. Klik "Download" untuk upload ke board

### Simulasi Logika di VSCode (tanpa dependency STM32)

1. Pastikan file `main.c` telah dimodifikasi agar tidak bergantung pada header HAL
2. Kompilasi dengan GCC:

```bash
gcc main.c -o kruk_simulasi
./kruk_simulasi
```

> Catatan: Versi simulasi hanya untuk menguji algoritma pemetaan ADC–PWM

---

## Pengujian dan Validasi

- Sensor FSR diuji menggunakan tekanan manual bertingkat dan dibandingkan dengan hasil pembacaan ADC.
- PWM servo divalidasi menggunakan osiloskop untuk melihat durasi pulsa aktual.
- Kruk diuji pada pengguna untuk melihat konsistensi responsif terhadap beban.

---

## Lisensi

Proyek ini menggunakan lisensi MIT. Bebas digunakan dan dimodifikasi untuk tujuan pendidikan maupun pengembangan lebih lanjut.

---

## Pengembangan Selanjutnya

- Implementasi fitur alarm untuk tekanan melebihi ambang batas.
- Logging data tekanan via UART ke PC.
- Integrasi Bluetooth untuk monitoring tekanan secara wireless.
