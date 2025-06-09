import json
from collections import defaultdict
from datasets import load_dataset

def generate_single_root_dataset(max_papers=20):
    """
    Finds a highly-cited paper, collects its descendants, and explicitly sets
    the ancestor as the single root to guarantee a single-tree structure.
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
    citation_graph = defaultdict(list)

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

            papers_db[paper_id] = {
                "id": paper_id,
                "title": paper['title'].strip().replace('\n', ' '),
                "incitation": str(paper['inCitations'][0]).strip() if paper['inCitations'] else "N/A",
                "author": authors_str,
                "year": paper['year']
            }

            for cited_id in paper.get('inCitations', []):
                citation_graph[str(cited_id)].append(paper_id)

    print(f"Database selesai dibangun dengan {len(papers_db)} paper 'Medicine'.")
    
    # Langkah 2: Cari paper root (induk) yang paling banyak disitasi
    root_candidate_id = None
    max_citations = -1

    for paper_id, citing_papers in citation_graph.items():
        if paper_id in papers_db and len(citing_papers) > max_citations:
            max_citations = len(citing_papers)
            root_candidate_id = paper_id
            
    if not root_candidate_id:
        print("Tidak ditemukan paper 'root' yang cocok. Coba jalankan lagi atau perbesar dataset.")
        return
        
    print(f"\nDitemukan kandidat root: [{papers_db[root_candidate_id]['id']}] '{papers_db[root_candidate_id]['title']}'")
    print(f"Paper ini disitasi oleh {max_citations} paper lain di dalam set 'Medicine'.")
    
    # Langkah 3: Kumpulkan semua "keturunan" dari paper root secara rekursif
    papers_in_family = set()
    
    def collect_descendants(paper_id):
        if len(papers_in_family) >= max_papers or paper_id in papers_in_family:
            return
        papers_in_family.add(paper_id)
        if paper_id in citation_graph:
            for child_id in citation_graph[paper_id]:
                collect_descendants(child_id)

    print(f"\nMengumpulkan keluarga paper (maksimal {max_papers} paper)...")
    papers_in_family.add(root_candidate_id)
    if root_candidate_id in citation_graph:
        for child_id in citation_graph[root_candidate_id]:
            collect_descendants(child_id)
            
    print(f"Keluarga berhasil dikumpulkan, terdiri dari {len(papers_in_family)} paper.")

    # Langkah 4: Simpan data keluarga ke file dengan JAMINAN SATU ROOT
    print("Menyimpan data ke 'data.txt'...")
    try:
        with open("data.txt", "w", encoding="utf-8") as f:
            
            # Ambil data paper root dari database
            root_paper_data = papers_db[root_candidate_id]
            
            # --- PERUBAHAN UTAMA DI SINI ---
            # Paksa 'incitation' dari root menjadi "-", agar program C
            # PASTI mengenalinya sebagai satu-satunya root dalam dataset ini.
            root_paper_data['incitation'] = "-"
            
            # Tulis data root yang sudah dimodifikasi sebagai baris pertama
            f.write(f"{root_paper_data['id']}|{root_paper_data['title']}|{root_paper_data['incitation']}|{root_paper_data['author']}|{root_paper_data['year']}\n")
            
            # Tulis sisa anggota keluarga
            for paper_id in papers_in_family:
                if paper_id == root_candidate_id:
                    continue # Lewati root karena sudah ditulis
                
                paper = papers_db[paper_id]
                f.write(f"{paper['id']}|{paper['title']}|{paper['incitation']}|{paper['author']}|{paper['year']}\n")

        print("\nBerhasil! File 'data.txt' telah dibuat dengan jaminan satu root.")
        print("Jalankan kembali program C Anda, visualisasi pohon sekarang akan sempurna.")

    except IOError as e:
        print(f"Gagal menulis ke file: {e}")

if __name__ == "__main__":
    generate_single_root_dataset(max_papers=20)