# Tugas Kecil 3 | Strategi Algoritma (IF2211)

Ini adalah Tugas Kecil 2 mata kuliah Strategi Algoritma (IF2211) yang berupa program penyelesaian permainan Rush Hour.

## Algoritma _Pathfinding_

Suatu gambar dapat dikompresi dengan membaginya menjadi empat blok. Lalu, setiap blok akan dibagikan terus-menerus secara rekursif. Jika suatu blok sudah dapat dihampirkan dengan warna rataannya dengan cukup baik (galat di bawah suatu ambang batas), atau blok tersebut sudah cukup kecil, pembagian akan dihentikan. Proses ini dapat dipermudahkan menggunakan data struktur quadtree, dengan setiap simpul merepresentasikan suatu blok dari gambar, lalu simpul daun menyatakan blok yang sudah dihampirkan dengan warna rataannya. Untuk informasi selengkapnya, dapat dilihat di laporan.

## Program Requirements

Program menggunakan bahasa C++, sehingga diperlukan hal berikut.

1. C++ Compiler (misalnya GCC)

## Build and Run Program

Berikut langkah-langkah untuk menjalankan program.

1. Clone repository ini.

   ```sh
   git clone https://github.com/adielrum/Tucil3_10123004_10123053
   ```

2. Navigate to `Tucil3_10123004_10123053` directory.

   ```sh
   cd Tucil3_10123004_10123053
   ```

3. Run `build.bat` to build and run C++ file.

   ```sh
   ./build.bat
   ```

## Struktur Program

Berikut struktur dari program ini.

```
├── bin/
│   ├── App.class
│   ├── ErrorCalculator.class
│   ├── GifSequenceWriter.class
│   ├── Quadtree.class
│   ├── QuadtreeNode.class
│   └── ThresholdCalculator.class
├── doc/
│   └── Tucil2_K1_10123053_Timothy Niels Ruslim.pdf
├── lib/
│   └── gif-sequence-writer.jar
├── src/
│   ├── App.java
│   ├── ErrorCalculator.java
│   ├── GifSequenceWriter.java
│   ├── Quadtree.java
│   ├── QuadtreeNode.java
│   └── ThresholdCalculator.java
├── test/
│   ├── bird.jpg
│   ├── boat.jpeg
│   ├── cabin.png
│   ├── flowers.jpg
│   ├── suit.jpeg
│   ├── timo.jpg
│   └── tram.png
└── README.md
```

## How It Works

Program ini berupa CLI. Setelah menjalankan program, berikut masukan yang diperlukan.

1. Alamat dari file .txt yang mengandung konfigurasi papan.
2.

Lalu, program akan menghasilkan keluaran berikut.

1. Waktu eksekusi kompresi (dalam ms).
2.
3.

Berikut contoh penyelesaian permainan Rush Hour yang dilakukan program.

## Author

[Adiel Rum](https://github.com/adielrum) (10123004)
[Timothy Niels Ruslim](https://github.com/timoruslim) (10123053)
