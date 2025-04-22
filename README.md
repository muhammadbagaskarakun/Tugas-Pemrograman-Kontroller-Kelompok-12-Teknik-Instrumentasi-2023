# Kruk Cerdas Berbasis Sensor Tekanan untuk Mencegah Cedera Kronis pada Pengguna Kruk

## Deskripsi Proyek

Proyek ini merupakan implementasi dari alat bantu jalan kruk cerdas berbasis mikrokontroler STM32 Nucleo F446RE, yang dilengkapi dengan sensor tekanan FSR 402 dan motor servo MG996R. Sistem ini dirancang untuk membantu pengguna kruk dalam mendistribusikan tekanan secara optimal guna mencegah cedera kronis, terutama akibat penggunaan kruk dalam jangka panjang dan beban yang tidak merata.
Logika sistem menggunakan pembacaan ADC dari sensor FSR untuk mengontrol posisi servo melalui sinyal PWM. Tujuan utama adalah menciptakan sistem kruk yang dapat beradaptasi secara dinamis terhadap tekanan yang diberikan oleh pengguna.
Anggota Kelompok:
Muhammad Aufa Affandi (2042231011)
Muhammad Bagaskara (2042231017)
Ziyad Zaky Permana (2042231077)

Pembimbing:
Ahmad Radhy
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
---

## 🛠️ Struktur Proyek (Berbasis V-Model)

```
├── Inc/
│   ├── fsr_sensor.h       # Header sensor tekanan
│   └── servo_motor.h      # Header motor servo
├── Src/
│   ├── fsr_sensor.c       # Implementasi pembacaan sensor FSR
│   ├── servo_motor.c      # Implementasi kontrol servo
│   └── main.c             # Integrasi utama sistem berbasis STM32 HAL
├── test2.c                # Simulasi input tegangan → PWM → jarak handle (untuk MinGW)
└── README.md              # Dokumentasi proyek
```

---

## 🔧 Penjelasan Kode `main.c`

`main.c` berisi loop utama sistem embedded yang bekerja sebagai berikut:

1. **Inisialisasi Perangkat:**
   - `HAL_Init()` dan `SystemClock_Config()` menjalankan setup awal STM32.
   - Peripherals seperti ADC, TIM2 (PWM), UART, DMA, dan GPIO diinisialisasi.

2. **Loop Utama:**
   - Tekanan dibaca dari sensor FSR melalui `HAL_ADC_GetValue()`.
   - Jika nilai tekanan melewati ambang batas (`adc_threshold`), sistem menghitung nilai PWM untuk mengatur sudut motor servo.
   - Servo dikendalikan via `__HAL_TIM_SET_COMPARE()` berdasarkan hasil pengolahan ADC.
   - Delay 50 ms menjaga siklus pembacaan stabil.

3. **Fail-Safe:**
   - Jika terjadi error pada proses ADC atau PWM, sistem akan masuk ke `Error_Handler()` dengan loop tak berhingga dan interrupt dimatikan.

---

## 🧪 Penjelasan `test2.c`: Simulasi PWM Handle (Via MinGW)

File `test2.c` digunakan untuk **simulasi berbasis terminal** tanpa perangkat keras, dan mensimulasikan alur konversi tegangan ke:

- Nilai ADC
- PWM output (dalam mikrodetik)
- Estimasi jarak handle kruk (dalam cm)

### Cara Kerja:
1. User memasukkan nilai tegangan (0.0–3.3V).
2. Program mengkonversinya ke nilai ADC (0–4095).
3. PWM dihitung dari ADC menggunakan formula:
   ```
   PWM = PWM_MIN + ((adc * (PWM_MAX - PWM_MIN)) / ADC_MAX)
   ```
4. Posisi handle dihitung dalam cm dari PWM, diasumsikan dalam rentang 0–10 cm.
5. Semua hasil ditampilkan ke terminal, termasuk pemanggilan fungsi `set_servo_position()` sebagai simulasi perintah fisik ke motor servo.

### Tujuan:
- Menguji logika konversi tegangan ke PWM tanpa hardware.
- Menyediakan **alat bantu simulasi** untuk debugging atau verifikasi alur matematis sebelum deployment ke STM32.

---

## ▶️ Cara Menjalankan Simulasi (test2.c)

### 1. Kompilasi dengan MinGW:
```bash
gcc test2.c -o simulasi_servo
```

### 2. Jalankan program:
```bash
./simulasi_servo
```

### 3. Masukkan tegangan 0.0 – 3.3 dan lihat hasil simulasi PWM serta jarak handle.

---

## Kesimpulan

Proyek ini menerapkan prinsip pengembangan **V-Model**, mulai dari analisis kebutuhan hingga pengujian sistem menggunakan simulasi berbasis terminal dan kode embedded STM32 HAL. Pendekatan ini memungkinkan sistem untuk diujicoba dengan atau tanpa perangkat keras, mempercepat iterasi pengembangan, dan meningkatkan keandalan sistem bantu jalan adaptif seperti kruk cerdas.
---
