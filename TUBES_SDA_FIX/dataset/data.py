import json
from datasets import load_dataset

def prepare_dataset_for_c_program():
    """
    Downloads, filters, and formats the s2orc_small dataset for the C program,
    limiting the output to 10 entries.
    """
    print("Memuat dataset 'leminda-ai/s2orc_small'. Ini mungkin memakan waktu...")
    # Memuat dataset dari Hugging Face
    try:
        dataset = load_dataset("leminda-ai/s2orc_small", split='train', trust_remote_code=True)
        print("Dataset berhasil dimuat.")
    except Exception as e:
        print(f"Gagal memuat dataset: {e}")
        return

    print("Memfilter paper dengan bidang studi 'Medicine'...")
    # Filter dataset untuk bidang studi 'Medicine'
    medicine_papers = dataset.filter(lambda example: 'Medicine' in example['fieldsOfStudy'])

    print("Memvalidasi dan mengumpulkan paper 'Medicine' yang datanya lengkap...")
    valid_medicine_papers = []
    medicine_paper_ids = set()

    # Iterasi pertama untuk mengumpulkan semua ID paper 'Medicine' yang valid
    for paper in medicine_papers:
        # Memeriksa apakah kolom yang dibutuhkan tidak kosong
        if (paper.get('id') and paper.get('title') and paper.get('authors') and
                paper.get('inCitations') is not None and paper.get('year') is not None and
                paper['authors']): # Memastikan list authors tidak kosong
            
            # Mengambil hanya sitasi pertama untuk kesederhanaan, sesuai format C
            incitation = paper['inCitations'][0] if paper['inCitations'] else "N/A"

            # Membersihkan dan menggabungkan nama penulis
            authors_list = [author.get('name', '').strip() for author in paper['authors'] if author.get('name')]
            authors_str = ", ".join(filter(None, authors_list))

            if authors_str: # Hanya proses jika ada nama penulis yang valid
                valid_paper_data = {
                    "id": str(paper['id']).strip(),
                    "title": paper['title'].strip().replace('\n', ' '),
                    "incitation": str(incitation).strip(),
                    "author": authors_str,
                    "year": paper['year'],
                    "all_incitations": set(paper['inCitations'])
                }
                valid_medicine_papers.append(valid_paper_data)
                medicine_paper_ids.add(valid_paper_data["id"])

    print(f"Ditemukan {len(valid_medicine_papers)} paper 'Medicine' dengan data lengkap.")
    print("Memfilter untuk memastikan keterkaitan sitasi internal...")

    interlinked_papers = []
    # Iterasi kedua untuk memastikan setiap paper mensitasi paper lain dalam set
    for paper in valid_medicine_papers:
        # Cek apakah ada irisan antara sitasi paper ini dan ID paper 'Medicine' yang ada
        if not paper["all_incitations"].isdisjoint(medicine_paper_ids):
            interlinked_papers.append(paper)

    print(f"Ditemukan {len(interlinked_papers)} paper 'Medicine' yang saling terkait.")

    # --- PERUBAHAN DI SINI ---
    # Batasi jumlah data yang akan disimpan menjadi 10
    final_papers_to_save = interlinked_papers[:10]
    print(f"Mengambil {len(final_papers_to_save)} data teratas untuk disimpan ke 'data.txt'...")

    # Menyimpan data yang sudah difilter ke file
    try:
        with open("data.txt", "w", encoding="utf-8") as f:
            for paper in final_papers_to_save:
                # Format: id|title|incitation|author|year
                f.write(f"{paper['id']}|{paper['title']}|{paper['incitation']}|{paper['author']}|{paper['year']}\n")
        
        print("\nBerhasil! File 'data.txt' telah dibuat dan berisi 10 data.")
        print("File ini siap digunakan oleh program C Anda.")
        print(f"Format data: id|title|inCitations|authors|year")
        print("Pastikan path file 'data.txt' di dalam main.c sudah benar.")
        print("Contoh path di main.c: loadDataFromFile(\"path/ke/data.txt\");")

    except IOError as e:
        print(f"Gagal menulis ke file: {e}")

if __name__ == "__main__":
    prepare_dataset_for_c_program()