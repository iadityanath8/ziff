# Ziff — LCS Diff & Patch Tool (C)

Ziff is a lightweight command-line file diff and patch utility written in **C**, based on the **Longest Common Subsequence (LCS)** algorithm.

It allows you to:

- Compare two files and generate a diff
- Apply a diff file to reconstruct or patch files

The tool is designed to be simple, fast, and dependency-free.

---

## 📌 Features

- Line-based file comparison
- Uses LCS dynamic programming algorithm
- Generates human-readable diff output
- Supports patching files using generated diffs
- Written in portable ANSI C
- No external libraries required

---

## ⚙️ Build

Compile using `gcc` (or any C compiler):

```bash
gcc -o ziff main.c diff.c lcs.c utils.c patch.c
or using make 
make compile
```

For debug build:

```bash
make 
```

---

## 🚀 Usage

```text
Usage:
  ziff diff  <file1> <file2>
  ziff patch <file1> <diff> <out>
```

### 1️⃣ Generate Diff

Compare two files and generate a diff:

```bash
./ziff diff old.txt new.txt > changes.diff
```

This prints the difference between `old.txt` and `new.txt`.

---

### 2️⃣ Apply Patch

Apply a diff file to patch a file:

```bash
./ziff patch old.txt changes.diff updated.txt
```

This generates `updated.txt` by applying `changes.diff` to `old.txt`.

---

## 🧠 How It Works

Ziff uses the **Longest Common Subsequence (LCS)** algorithm to find matching lines between two files.

### Workflow

1. Read both files line-by-line
2. Compute the LCS table using dynamic programming
3. Backtrack to determine:
   - Added lines
   - Removed lines
   - Unchanged lines
4. Generate diff output
5. Use diff instructions for patching

### Why LCS?

- Produces minimal edit sequences
- Stable and well-known algorithm
- Commonly used in diff tools

---

## 📄 Diff Format

The diff output follows a simple line-based format:

- `+` → Added line
- `-` → Removed line
- ` ` → Unchanged line

Example:

```diff
 line one
-line two
+line two modified
 line three
```

---

## 📁 Project Structure (Example)

```text
.
├── main.c        # Entry point
├── lcs.c         # LCS algorithm
├── diff.c        # Diff generation
├── patch.c       # Patch application
├── utils.c       # Helper functions
├── utils.h
└── README.md
```

---

## 🛠️ Limitations

- Line-based only (not character-level)
- Memory usage increases for very large files
- No binary file support
- No context/unified diff format (yet)

---

## 🔮 Future Improvements

- Unified diff format support
- Binary diff support
- Performance optimization for large files
- Colorized output
- Git-style context diffs

---

## 📜 License

MIT License

You are free to use, modify, and distribute this project.

---

## 👤 Author

Developed by **Aditya Nath**

---

## 💡 Example

```bash
./ziff diff v1.txt v2.txt > update.diff
./ziff patch v1.txt update.diff v2_new.txt
```

Happy hacking! 🚀