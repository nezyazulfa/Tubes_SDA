# Pastikan Anda telah menginstal pustaka yang diperlukan:
# pip install datasets
from datasets import load_dataset

def process_s2orc_dataset():
    """
    Mengunduh dataset s2orc_small, memfilter berdasarkan bidang studi "Medicine",
    memilih 100 data pertama, dan menyimpannya ke dalam file .txt dengan
    pemisah '|'.
    """
    try:
        # Memuat dataset dari Hugging Face
        # Menggunakan streaming=True agar tidak mengunduh seluruh dataset sekaligus
        print("Memuat dataset...")
        dataset = load_dataset("leminda-ai/s2orc_small", split='train', streaming=True)
        
        filtered_data = []
        count = 0
        
        print("Mulai memfilter data...")
        # Iterasi melalui dataset
        for record in dataset:
            # Hentikan jika sudah mencapai 100 data
            if count >= 100:
                break

            # Filter berdasarkan fieldsOfStudy yang mengandung 'Medicine'
            if record['fieldsOfStudy'] and 'Medicine' in record['fieldsOfStudy']:
                # Ekstrak kolom yang diinginkan
                paper_id = record.get('id', 'N/A')
                title = record.get('title', 'N/A')
                
                # Ubah daftar penulis menjadi satu string
                authors_list = record.get('authors', [])
                authors = ', '.join([author['name'] for author in authors_list if author and 'name' in author]) if authors_list else 'N/A'
                
                # Ambil jumlah sitasi masuk
                incitations = record.get('inCitations', [])
                incitation_count = len(incitations) if incitations else 0

                year = record.get('year', 'N/A')

                # Buat baris yang akan ditulis ke file
                line = f"{paper_id}|{title}|{authors}|{incitation_count}|{year}"
                filtered_data.append(line)
                
                count += 1
                print(f"Data ke-{count} ditemukan...")

        # Tulis data yang telah difilter ke file .txt
        output_filename = "s2orc_medicine_data.txt"
        with open(output_filename, 'w', encoding='utf-8') as f:
            for line in filtered_data:
                f.write(line + '\n')
                
        print(f"\nProses selesai. {count} data telah disimpan di file '{output_filename}'")

    except Exception as e:
        print(f"Terjadi kesalahan: {e}")

if __name__ == "__main__":
    process_s2orc_dataset()