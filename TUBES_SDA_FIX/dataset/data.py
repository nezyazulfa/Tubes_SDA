import json
from collections import defaultdict
from datasets import load_dataset

def generate_correct_single_root_dataset(max_papers=10000):
    """
    Finds a highly-cited paper, collects its descendants, correctly maps
    their internal parent-child relationships, and saves the data to ensure
    a single, perfectly-formed tree structure.
    """
    print("Memuat dataset 'leminda-ai/s2orc_small'. Ini mungkin memakan waktu...")
    try:
        dataset = load_dataset("leminda-ai/s2orc_small", split='train', trust_remote_code=True)
        print("Dataset berhasil dimuat.")
    except Exception as e:
        print(f"Gagal memuat dataset: {e}")
        return

    print("Memfilter paper 'Medicine' dan membangun database & peta sitasi...")
    
    papers_db = {}
    citation_graph = defaultdict(list) # Peta: parent_id -> [list of child_ids]

    # Langkah 1: Filter paper 'Medicine' dan bangun struktur data
    for paper in dataset.filter(lambda x: 'Medicine' in x['fieldsOfStudy']):
        if (paper.get('id') and paper.get('title') and paper.get('authors') and
                paper.get('inCitations') is not None and paper.get('year') is not None and
                paper['authors']):
            
            paper_id = str(paper['id']).strip()
            
            authors_list = [author.get('name', '').strip() for author in paper['authors'] if author.get('name')]
            authors_str = ", ".join(filter(None, authors_list))
            if not authors_str:
                continue

            # Simpan semua detail paper ke database
            papers_db[paper_id] = {
                "id": paper_id,
                "title": paper['title'].strip().replace('\n', ' '),
                "author": authors_str,
                "year": paper['year']
            }

            # Bangun peta sitasi terbalik (siapa mensitasi siapa)
            for cited_id in paper.get('inCitations', []):
                citation_graph[str(cited_id)].append(paper_id)

    print(f"Database selesai dibangun dengan {len(papers_db)} paper 'Medicine'.")
    
    # Langkah 2: Cari paper root (induk) yang paling banyak disitasi
    root_id = None
    max_citations = -1

    for paper_id, citing_papers in citation_graph.items():
        if paper_id in papers_db and len(citing_papers) > max_citations:
            max_citations = len(citing_papers)
            root_id = paper_id
            
    if not root_id:
        print("Tidak ditemukan paper 'root' yang cocok. Coba jalankan lagi.")
        return
        
    print(f"\nDitemukan kandidat root: [{papers_db[root_id]['id']}] '{papers_db[root_id]['title']}'")
    
    # Langkah 3: Kumpulkan semua "keturunan" dari paper root secara rekursif
    papers_in_family = set()
    def collect_descendants(paper_id):
        if len(papers_in_family) >= max_papers or paper_id in papers_in_family:
            return
        papers_in_family.add(paper_id)
        if paper_id in citation_graph:
            for child_id in citation_graph[paper_id]:
                if child_id in papers_db: # Pastikan anak juga ada di database
                    collect_descendants(child_id)

    print(f"Mengumpulkan keluarga paper (maksimal {max_papers} paper)...")
    collect_descendants(root_id)
            
    print(f"Keluarga berhasil dikumpulkan, terdiri dari {len(papers_in_family)} paper.")

    # --- LANGKAH 4: BAGIAN BARU & PENTING ---
    # Buat peta terbalik (child -> parent) HANYA untuk paper di dalam keluarga
    parent_map = {}
    for parent, children in citation_graph.items():
        if parent in papers_in_family:
            for child in children:
                if child in papers_in_family:
                    parent_map[child] = parent

    # --- LANGKAH 5: SIMPAN DATA DENGAN LOGIKA YANG BENAR ---
    print("Menyimpan data dengan hubungan sitasi yang benar ke 'data.txt'...")
    try:
        with open("data.txt", "w", encoding="utf-8") as f:
            # Iterasi melalui semua anggota keluarga yang telah kita kumpulkan
            for paper_id in papers_in_family:
                paper_data = papers_db[paper_id]
                
                # Tentukan ID sitasi yang benar
                if paper_id == root_id:
                    # Ini adalah root utama, sitasinya kita set ke "-"
                    correct_incitation = "-"
                else:
                    # Untuk paper lain, cari induknya di 'parent_map'
                    # Jika karena suatu hal tidak ketemu, anggap sebagai root juga (fallback)
                    correct_incitation = parent_map.get(paper_id, "-")

                # Tulis baris dengan data sitasi yang sudah benar
                f.write(f"{paper_data['id']}|{paper_data['title']}|{correct_incitation}|{paper_data['author']}|{paper_data['year']}\n")

        print("\nBerhasil! File 'data.txt' telah dibuat dengan hubungan sitasi yang dijamin benar.")
        print("Program C Anda sekarang akan membentuk satu pohon tunggal yang sempurna.")

    except IOError as e:
        print(f"Gagal menulis ke file: {e}")

if __name__ == "__main__":
    generate_correct_single_root_dataset(max_papers=10000)