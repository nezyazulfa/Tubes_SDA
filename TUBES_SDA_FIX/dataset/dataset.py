# Pastikan Anda telah menginstal pustaka yang diperlukan:
# pip install datasets
from datasets import load_dataset

def generate_citation_tree_data():
    """
    Menghasilkan dataset untuk membangun pohon sitasi.
    Setiap baris adalah paper yang mensitasi paper lain yang juga ada di dalam dataset.
    Format: id_paper_anak|judul_paper_anak|id_paper_induk|author_paper_anak|tahun_paper_anak
    """
    try:
        # --- LANGKAH 1: KUMPULKAN KANDIDAT PAPER ---
        print("Memuat dataset dan mengumpulkan kandidat paper (bidang Medicine)...")
        dataset = load_dataset("leminda-ai/s2orc_small", split='train', streaming=True)
        
        # Kolam untuk menampung paper yang relevan agar mudah dicari berdasarkan ID
        # Kita ambil lebih banyak dari 100 untuk meningkatkan kemungkinan menemukan hubungan internal
        paper_pool_limit = 500 
        paper_pool = {} # Format: { 'paper_id': record }

        for record in dataset:
            if len(paper_pool) >= paper_pool_limit:
                break
            
            # Filter hanya untuk 'Medicine' dan yang memiliki ID
            if record['id'] and record['fieldsOfStudy'] and 'Medicine' in record['fieldsOfStudy']:
                paper_pool[record['id']] = record
        
        print(f"Berhasil mengumpulkan {len(paper_pool)} kandidat paper.")

        # --- LANGKAH 2 & 3: CARI HUBUNGAN SITASI INTERNAL & BUAT DATASET FINAL ---
        print("Mencari hubungan sitasi internal untuk membangun dataset tree...")
        
        citation_links = []
        final_data_limit = 100

        # Iterasi melalui paper yang sudah kita kumpulkan
        for source_paper_id, source_paper_record in paper_pool.items():
            if len(citation_links) >= final_data_limit:
                break

            # Dapatkan daftar ID paper yang disitasi oleh source_paper
            # Ini adalah 'outCitations'
            cited_paper_ids = source_paper_record.get('outCitations', [])
            if not cited_paper_ids:
                continue

            for target_paper_id in cited_paper_ids:
                # Periksa apakah paper yang disitasi (target) juga ada di kolam kita
                if target_paper_id in paper_pool:
                    # Hubungan ditemukan!
                    # Format data sesuai kebutuhan program C Anda.
                    
                    # Ambil data dari source paper (paper anak)
                    paper_id = source_paper_record.get('id', 'N/A')
                    title = source_paper_record.get('title', 'N/A').replace('|', ' ') # Ganti | agar tidak merusak format
                    
                    authors_list = source_paper_record.get('authors', [])
                    authors = ', '.join([author['name'] for author in authors_list if author and 'name' in author]) if authors_list else 'N/A'
                    authors = authors.replace('|', ' ')
                    
                    year = source_paper_record.get('year', 0)

                    # ID sitasi adalah ID dari paper induk (target_paper_id)
                    incitation_id = target_paper_id
                    
                    # Format baris: id_anak|judul_anak|id_induk|author_anak|tahun_anak
                    line = f"{paper_id}|{title}|{incitation_id}|{authors}|{year}"
                    citation_links.append(line)
                    
                    print(f"Hubungan ditemukan ({len(citation_links)}/{final_data_limit}): Paper {paper_id} mensitasi {incitation_id}")

                    # Hentikan pencarian untuk paper ini dan lanjut ke paper berikutnya
                    # agar dataset lebih beragam
                    if len(citation_links) >= final_data_limit:
                        break
            
    
        # --- LANGKAH 4: TULIS HASIL KE FILE ---
        output_filename = "data.txt"
        with open(output_filename, 'w', encoding='utf-8') as f:
            for line in citation_links:
                f.write(line + '\n')

        print(f"\nProses selesai. {len(citation_links)} data hubungan sitasi telah disimpan di file '{output_filename}'")
        print("File ini siap digunakan oleh program C Anda.")

    except Exception as e:
        print(f"Terjadi kesalahan: {e}")

if __name__ == "__main__":
    generate_citation_tree_data()