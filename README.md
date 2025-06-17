# 📚 Paper Citation Manager

## 📌 Deskripsi
**Paper Citation Manager** adalah aplikasi berbasis konsol yang dirancang untuk membantu pengguna dalam mengelola dan menganalisis data paper ilmiah. Aplikasi ini memanfaatkan berbagai struktur data seperti **Single Linked List**, **Double Linked List**, **Tree**, **Stack**, dan **Queue** untuk menyediakan fitur-fitur seperti pencarian, pengurutan, visualisasi, rekomendasi, dan bookmark paper. Proyek ini dikembangkan menggunakan bahasa pemrograman **C** sebagai bagian dari **Tugas Besar Mata Kuliah Struktur Data dan Algoritma** di **Jurusan Teknik Komputer dan Informatika, POLBAN**.

## 👥 Anggota Kelompok (C6)
- **Wyandhanu Maulidan Nugraha** (241511092) - [@wyandhanupapoy](https://github.com/wyandhanupapoy)
- **Siti Soviyyah** (241511090) - [@nsitizen](https://github.com/nsitizen)
- **Nezya Zulfa Fauziah** (241511085) - [@nezyazulfa](https://github.com/nezyazulfa)
- **Muhammad Raihan Abubakar** (241511084) - [@hanzz78](https://github.com/hanzz78)
- **Muhammad Brata Hadinata** (241511082) - [@MuhammadBrataH](https://github.com/MuhammadBrataH)
- **Ahmad Habib Mutaqqin** (241511065)

### 📌 Dosen Pengampu: **Ade Hodijah**

## 🧩 Fitur Aplikasi
- **Tampilkan Semua Paper**: Menampilkan seluruh data paper yang dimuat dari dataset.
- **Cari Paper Berdasarkan ID**: Mencari dan menampilkan detail paper berdasarkan ID yang dimasukkan pengguna.
- **Urutkan Paper Berdasarkan Tahun**: Mengurutkan data paper berdasarkan tahun publikasi secara ascending atau descending menggunakan Double Linked List.
- **Visualisasi Paper Menggunakan Tree**: Menampilkan struktur hierarki citation antar paper dalam bentuk pohon.
- **Bookmark Paper**: Menyimpan paper ke dalam stack bookmark untuk akses cepat di masa mendatang.
- **Tampilkan Bookmark**: Menampilkan seluruh paper yang telah dibookmark oleh pengguna.
- **Simpan Bookmark ke File**: Menyimpan daftar bookmark ke file `bookmark.txt`.
- **Rekomendasi Paper Berdasarkan Citation**: Menampilkan rekomendasi paper yang mensitasi paper tertentu menggunakan struktur queue.
- **Keluar dari Program**: Menghapus semua alokasi memori dan keluar dari aplikasi.

## 🛠️ Teknologi yang Digunakan
- **Bahasa Pemrograman**: C
- **Struktur Data**:
  - Single Linked List (SLL)
  - Double Linked List (DLL)
  - Binary Tree
  - Stack
  - Queue

## 📥 Instalasi & Cara Menjalankan
### 1️⃣ Clone Repository
```bash
git clone https://github.com/nezyazulfa/Tubes_SDA.git
cd TUBES_SDA
```

### 2️⃣ Compile Program
```bass
gcc TUBES_SDA_FIX/codingan/*.c -o program -Wall -g
```

### 3️⃣ Jalankan Aplikasi
```bash
./program
```

## 📄 Format Dataset
Dataset disimpan dalam file `data.txt` dengan format sebagai berikut:
```
id|title|incitation|author|year
```
Contoh:
```
P001|Implementasi Algoritma Sorting|P000|John Doe|2020
```

## 📜 Lisensi
Proyek ini dibuat untuk tujuan akademik dalam mata kuliah **Struktur Data dan Algoritma** di **Politeknik Negeri Bandung (POLBAN)**.

---

📌 **Repository GitHub**: [Paper Citation Manager](https://github.com/nezyazulfa/Tubes_SDA.git)
