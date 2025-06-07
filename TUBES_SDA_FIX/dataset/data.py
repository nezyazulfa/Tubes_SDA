from datasets import load_dataset
import json

# Load the dataset
try:
    dataset = load_dataset("leminda-ai/s2orc_small", split="train")
except Exception as e:
    print(f"Error loading dataset: {e}")
    exit(1)

# Inspect dataset schema (optional, for debugging)
print("Dataset columns:", dataset.column_names)

# Filter for Medicine and select required columns
medicine_data = [
    {
        "id": item["id"],
        "title": item["title"],
        "authors": item["authors"],
        "inCitations": item["inCitations"],
        "year": item["year"]
    }
    for item in dataset
    if "fieldsOfStudy" in item and item["fieldsOfStudy"] and "Medicine" in item["fieldsOfStudy"]
]

# Limit to first 100 records
medicine_data = medicine_data[:100]

# Save to a .txt file with | separator
with open("medicine_data.txt", "w", encoding="utf-8") as f:
    # Write header
    f.write("id|title|authors|inCitations|year\n")
    # Write data
    for record in medicine_data:
        line = (
            f"{record['id'] or 'N/A'}|"
            f"{record['title'] or 'N/A'}|"
            f"{json.dumps(record['authors']) if record['authors'] is not None else 'N/A'}|"
            f"{json.dumps(record['inCitations']) if record['inCitations'] is not None else 'N/A'}|"
            f"{record['year'] or 'N/A'}\n"
        )
        f.write(line)

print("Data saved to medicine_data.txt")