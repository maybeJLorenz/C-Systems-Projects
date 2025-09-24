# Huffman File Compression System

A lossless file compression and decompression utility implemented in **C** using **Huffman coding**.  
This project demonstrates **low-level systems programming** with custom bit-level I/O, binary trees, and priority queues to efficiently encode and decode data.  

---

## 📖 Overview
- **Compressor (`huff`)**: Reads a file, builds a frequency histogram, constructs a Huffman tree, and outputs a compressed binary file.  
- **Decompressor (`dehuff`)**: Reads the compressed binary file, reconstructs the Huffman tree, and restores the original file.  
- **Supporting Modules**:  
  - `bitwriter.c / bitreader.c`: Custom bit-level I/O operations  
  - `node.c`: Binary tree structure for code representation  
  - `pq.c`: Priority queue for tree construction  
- **Build System**: Makefile-based with unit tests and system tests.  

---

## 🛠️ Skills Demonstrated
- **Programming & Tools:** C, Makefile, GCC/Clang, Valgrind, Linux CLI  
- **Systems Programming:** Bit-level file I/O, memory management, error handling  
- **Data Structures & Algorithms:** Binary trees, priority queues, recursion, Huffman coding algorithm  
- **Software Engineering:** Modular design, unit/system testing, debugging, static analysis (`scan-build`)  

---

## 🚀 Build & Run

Clone the repository:
```bash
git clone https://github.com/yourusername/huffman-compression-c.git
cd huffman-compression-c
