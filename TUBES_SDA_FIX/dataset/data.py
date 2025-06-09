import json
from collections import defaultdict, deque
from datasets import load_dataset

def generate_connected_dataset(max_papers_to_process=75000):
    """
    Memproses dataset s2orc_small, memfilter paper 'Medicine' yang valid,
    menemukan komponen terhubung (gugusan) terbesar dalam jaringan sitasi,

    dan hanya menyimpan paper dari gugusan tersebut ke data.txt.
    """
    print("Memuat dataset 'leminda-ai/s2orc_small'. Proses ini mungkin memakan waktu...")
    try:
        dataset = load_dataset("leminda-ai/s2orc_small", split=f'train[:{max_papers_to_process}]', trust_remote_code=True)
        print("Dataset berhasil dimuat.")
    except Exception as e:
        print(f"Gagal memuat dataset: {e}")
        return

    # =====================================================================================
    # LANGKAH 1: Kumpulkan semua paper 'Medicine' yang valid dan bangun peta sitasi awal
    # =====================================================================================
    print("\nLangkah 1: Memfilter paper 'Medicine' dan membangun database kandidat...")
    
    papers_db = {}
    # Graph ini akan kita buat 'undirected' untuk mencari komponen terhubung
    adjacency_list = defaultdict(list)

    medicine_papers = dataset.filter(lambda x: 'Medicine' in x.get('fieldsOfStudy', []))
    
    for paper in medicine_papers:
        if not all([paper.get('id'), paper.get('title'), paper.get('authors'), paper.get('year') is not None, paper.get('inCitations') is not None]):
            continue
        authors_list = [author.get('name', '').strip() for author in paper['authors'] if author.get('name')]
        authors_str = ", ".join(filter(None, authors_list))
        if not authors_str:
            continue
            
        clean_paper_id = str(paper['id']).strip()
        papers_db[clean_paper_id] = {
            "id": clean_paper_id,
            "title": paper['title'].strip().replace('\n', ' ').replace('|', ';'),
            "author": authors_str.replace('|', ';'),
            "year": paper['year']
        }
        # Bangun adjacency list (graf tidak berarah)
        for cited_id in paper.get('inCitations', []):
            clean_cited_id = str(cited_id).strip()
            if clean_cited_id:
                adjacency_list[clean_paper_id].append(clean_cited_id)
                adjacency_list[clean_cited_id].append(clean_paper_id)

    print(f"Selesai. Ditemukan {len(papers_db)} kandidat paper 'Medicine' yang valid.")

    # =====================================================================================
    # LANGKAH 2: Temukan Gugusan (Connected Component) Terbesar dalam Jaringan
    # =====================================================================================
    print("\nLangkah 2: Menganalisis jaringan untuk menemukan gugusan sitasi terbesar...")
    
    visited = set()
    all_components = []
    
    # Algoritma Breadth-First Search (BFS) untuk mencari semua komponen terhubung
    for paper_id in papers_db:
        if paper_id not in visited:
            current_component = set()
            q = deque([paper_id])
            visited.add(paper_id)
            
            while q:
                current_paper = q.popleft()
                current_component.add(current_paper)
                
                for neighbor in adjacency_list[current_paper]:
                    # Hanya proses tetangga yang juga ada di db kita dan belum dikunjungi
                    if neighbor in papers_db and neighbor not in visited:
                        visited.add(neighbor)
                        q.append(neighbor)
            all_components.append(current_component)
    
    if not all_components:
        print("Tidak ada komponen sitasi yang ditemukan. Program berhenti.")
        return

    # Temukan komponen dengan jumlah anggota terbanyak
    largest_component = max(all_components, key=len)
    print(f"Ditemukan! Gugusan terbesar terdiri dari {len(largest_component)} paper yang saling berkaitan.")

    # =====================================================================================
    # LANGKAH 3: Siapkan dan Simpan Data HANYA dari Gugusan Terbesar
    # =====================================================================================
    print("\nLangkah 3: Mempersiapkan dan menyimpan data dari gugusan terpilih...")
    
    # Kita butuh peta sitasi asli (parent -> children) untuk menentukan hubungan
    citation_graph = defaultdict(list)
    for parent_id, children_ids in adjacency_list.items():
        # Hanya proses paper dari gugusan terbesar
        if parent_id in largest_component:
            for child_id in children_ids:
                # Periksa apakah sitasi ini ada di dataset asli paper anak
                child_paper = next((p for p in medicine_papers if str(p.get('id')).strip() == child_id), None)
                if child_paper and parent_id in child_paper.get('inCitations',[]):
                    citation_graph[parent_id].append(child_id)

    # Buat peta terbalik (child -> parent) khusus untuk anggota gugusan terbesar
    parent_map = {}
    for parent_id, children_ids in citation_graph.items():
        if parent_id in largest_component:
            for child_id in children_ids:
                if child_id in largest_component:
                    parent_map[child_id] = parent_id

    try:
        count = 0
        with open("data.txt", "w", encoding="utf-8") as f:
            # Iterasi hanya pada paper yang ada di gugusan terbesar
            for paper_id in largest_component:
                paper_data = papers_db[paper_id]
                
                # Tentukan ID sitasi. Jika tidak ada di peta, dia adalah root dari gugusan ini.
                correct_incitation = parent_map.get(paper_id, "-")
                
                f.write(f"{paper_data['id']}|{paper_data['title']}|{correct_incitation}|{paper_data['author']}|{paper_data['year']}\n")
                count += 1

        print(f"\n✅ Berhasil! {count} paper dari gugusan terbesar telah disimpan ke 'data.txt'.")
        print("Dataset ini sekarang dijamin saling berkaitan dan ideal untuk visualisasi pohon.")

    except IOError as e:
        print(f"Gagal menulis ke file: {e}")

if __name__ == "__main__":
    generate_connected_dataset(max_papers_to_process=75000)