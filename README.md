# Tugas Kecil 3 | Strategi Algoritma (IF2211)

Ini adalah Tugas Kecil 3 mata kuliah Strategi Algoritma (IF2211) yang berupa program penyelesaian permainan Rush Hour. 

## Permainan Rush Hour 

Rush Hour adalah sebuah permainan puzzle logika berbasis grid yang menantang pemain untuk menggeser kendaraan di dalam sebuah kotak (biasanya berukuran $6 \times 6$) agar mobil utama (biasanya berwarna merah) dapat keluar dari kemacetan melalui pintu keluar di sisi papan. Pada tugas ini, dilakukan perumuman grid menjadi sebarang mn, lalu setiap kendaraan direpresentasikan sebagai potongan $p \times 1$ atau $1 \times p$ tergantung orientasinya (horizontal atau vertikal) di grid. 

## Algoritma _Pathfinding_

Untuk mencari solusi dari puzzle permainan puzzle rush hour ini, kami memanfaatkan algoritma uninformed search seperti Uniform Cost Search (UCS) dan informed search seperti Greedy Best First Search (GBFS) dan A* Search. UCS menggunakan fungsi evaluasi berupa harga jarak dari suatu status ke status awal, GBFS menggunakan fungsi evaluasi berupa taksiran harga jarak dari suatu status ke status tujuan menggunakan suatu heuristik, dan A* menggunakan fungsi evaluasi kombinasi dari keduanya. Agar solusi yang digunakan optimal, A* harus menggunakan heuristik yang admissible. Untuk Rush Hour, kami pilih heuristik jumlah kendaraan yang menghalangi jalur keluar. 

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

4. Run `gui.bat` to build run the GUI program instead.
   ```sh
   ./gui.bat
   ```

## Struktur Program

Berikut struktur dari program ini.

```
├── bin/
│   ├── gui.exe
│   ├── main.exe
│   ├── solver.exe
│   └── tasks.exe
├── doc/
│   └── Tucil3_K1_10123004_Adiel Rum_Timothy Niels Ruslim.pdf
├── src/
│   ├── gui.cpp
│   ├── main.cpp
│   ├── move.cpp
│   ├── move.h
│   ├── papan.cpp
│   ├── papan.h
│   ├── piece.cpp
│   ├── piece.h
│   ├── rush_gui.py
│   ├── solver.cpp
│   ├── solver.h
│   ├── state.cpp
│   └── state.h
├── test/
│   ├── impossible_1.txt
│   ├── medium_bawah.txt
│   ├── mudah_kanan.txt
│   ├── mudah_kiri.txt
│   ├── susah_atas.txt
│   ├── susah_kanan.txt
│   ├── susah_atas.txt
│   └── susah_kiri.txt
├── .gitignore
├── README.md
├── build.bat
├── gui.bat
└── run.bat
```

## How It Works

Program ini berupa CLI. Setelah menjalankan program, berikut masukan yang diperlukan.

1. Alamat dari file .txt yang mengandung konfigurasi papan.
2. Algoritma Pilihan (0 untuk UCS, 1 untuk GBFS, 2 untuk A*)

Lalu, program akan menghasilkan keluaran berikut.

1. Tampilan awal papan
2. Banyak simpul yang dikunjungi
3. Waktu pencaian (dalam ms)
4. Gerakan-gerakan pada solusi (Piece apa pindah kemana berapa jauh) dan tampilan papan setelah melakukan gerakan tersebut

Berikut contoh penyelesaian permainan Rush Hour yang dilakukan program.
```
[Di dalam file mudah_kanan berbentuk seperti ini]
6 6
11
AAB..F
..BCDF
GPPCDFK
GH.III
GHJ...
LLJMM.

Masukan file (.txt): test\mudah_kanan.txt

Berikut beberapa algoritma pathfinding.
  0. Uniform Cost Search (UCS)
  1. Greedy Best First Search
  2. A* Search
Pilih algoritma (0,1,2): 1

Papan Awal
* * * * * * * *
* A A B . . F *
* . . B C D F *
* G P P C D F K
* G H . I I I *
* G H J . . . *
* L L J M M . *
* * * * * * * *

Solusi ditemukan!
Dikunjungi 8 simpul dalam 1.010 ms.

Gerakan 1: Pindah piece C ke atas 1 langkah.
* * * * * * * *
* A A B C . F *
* . . B C D F *
* G P P . D F K
* G H . I I I *
* G H J . . . *
* L L J M M . *
* * * * * * * *

Gerakan 2: Pindah piece D ke atas 1 langkah.
* * * * * * * *
* A A B C D F *
* . . B C D F *
* G P P . . F K
* G H . I I I *
* G H J . . . *
* L L J M M . *
* * * * * * * *

Gerakan 3: Pindah piece I ke kiri 1 langkah.
* * * * * * * * 
* A A B C D F *
* . . B C D F *
* G P P . . F K
* G H I I I . *
* G H J . . . *
* L L J M M . *
* * * * * * * *

Gerakan 4: Pindah piece F ke bawah 3 langkah.
* * * * * * * *
* A A B C D . *
* . . B C D . *
* G P P . . . K
* G H I I I F *
* G H J . . F *
* L L J M M F *
* * * * * * * *

Gerakan 5: Pindah piece P ke kanan 6 langkah.
* * * * * * * *
* A A B C D . *
* . . B C D . *
* G . . . . . K
* G H I I I F *
* G H J . . F *
* L L J M M F *
```

## Author

[Adiel Rum](https://github.com/adielrum) (10123004)
[Timothy Niels Ruslim](https://github.com/timoruslim) (10123053)
