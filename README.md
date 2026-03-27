# DES Encryption with Huffman Encoding

[![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![Encryption](https://img.shields.io/badge/Encryption-DES-green)](https://en.wikipedia.org/wiki/Data_Encryption_Standard)
[![Compression](https://img.shields.io/badge/Compression-Huffman-blue)](https://en.wikipedia.org/wiki/Huffman_coding)
[![Course](https://img.shields.io/badge/Course-Academic-orange)](https://en.wikipedia.org/wiki/Harbin_Institute_of_Technology)

A C++ implementation combining DES encryption with Huffman coding for image compression and encryption.

## Overview

This project was developed during undergraduate studies at Harbin Institute of Technology (哈尔滨工业大学). It demonstrates the combination of two fundamental algorithms:

1. **DES (Data Encryption Standard)**: For secure encryption
2. **Huffman Coding**: For lossless data compression

The workflow is: **Image → DES Encryption → Huffman Encoding**

## Features

- 🔐 **DES Encryption**: 56-bit key symmetric encryption
- 📦 **Huffman Compression**: Optimal prefix coding for compression
- 🖼️ **Image Processing**: Works with image files
- 💾 **File I/O**: Read/write binary files

## Tech Stack

- **Language**: C++
- **IDE**: Visual Studio 2010
- **Platform**: Windows
- **Standards**: C++98/C++03 (VS2010 compatible)

## Prerequisites

- Visual Studio 2010 (or compatible version)
- Windows OS
- Basic understanding of cryptography concepts

## Installation

```bash
# Clone the repository
git clone https://github.com/leelening/DES-encryption-under-huffman-encode.git
cd DES-encryption-under-huffman-encode

# Open in Visual Studio
# The Debug files are included for convenience
# Open the .sln or .vcproj file in Visual Studio 2010

# Build and Run
# Press F5 to build and run in Debug mode
# or build Release version for performance
```

## Usage

### Running the Program

```bash
# In Visual Studio
# 1. Open the project file
# 2. Build the solution (Ctrl+Shift+B)
# 3. Run (F5)

# The program will:
# 1. Load an input image
# 2. Apply DES encryption
# 3. Apply Huffman encoding
# 4. Save the compressed encrypted output
```

## Algorithms

### DES (Data Encryption Standard)

- **Block Size**: 64 bits
- **Key Size**: 56 bits (64 bits with parity)
- **Rounds**: 16 rounds of Feistel network
- **Operations**:
  - Initial permutation
  - 16 rounds of substitution and permutation
  - Final permutation

### Huffman Coding

- **Type**: Lossless compression
- **Method**: Variable-length prefix coding
- **Optimality**: Minimizes expected code length
- **Process**:
  1. Calculate symbol frequencies
  2. Build Huffman tree
  3. Generate codes
  4. Encode data

## Project Structure

```
.
├── src/                      # Source code
│   ├── des.cpp               # DES implementation
│   ├── des.h                 # DES header
│   ├── huffman.cpp           # Huffman coding
│   ├── huffman.h             # Huffman header
│   ├── main.cpp              # Entry point
│   └── image_utils.cpp       # Image I/O utilities
├── include/                  # Header files
├── Debug/                    # Debug build files
├── Release/                  # Release build files
├── input/                    # Sample input images
├── output/                   # Encrypted output
└── README.md                 # This file
```

## How It Works

### Encryption Process

1. **Load Image**: Read binary image data
2. **DES Encryption**:
   - Generate 56-bit key
   - Apply initial permutation
   - 16 rounds of Feistel encryption
   - Apply final permutation
3. **Huffman Encoding**:
   - Analyze frequency of encrypted bytes
   - Build Huffman tree
   - Generate prefix codes
   - Encode encrypted data
4. **Save Output**: Write compressed encrypted file

### Decryption Process

1. **Load Encrypted File**: Read compressed data
2. **Huffman Decoding**:
   - Rebuild Huffman tree from codebook
   - Decode variable-length codes
3. **DES Decryption**:
   - Apply inverse DES operations
   - Decrypt using same key
4. **Save Image**: Write decrypted image

## Security Note

> ⚠️ **Educational Purpose Only**
> 
> DES is considered insecure for modern applications due to its 56-bit key size. This implementation is for educational purposes. For production use, consider:
> - AES (Advanced Encryption Standard)
> - ChaCha20
> - RSA for asymmetric encryption

## Course Information

- **Course**: Information Security / Cryptography
- **Institution**: Harbin Institute of Technology (哈尔滨工业大学)
- **Year**: ~2014 (Undergraduate)
- **Language**: C++

## Key Concepts Learned

- Symmetric encryption algorithms
- Block cipher modes
- Feistel networks
- Lossless data compression
- Prefix codes
- Binary tree algorithms
- File I/O operations
- Bit manipulation

## Educational Value

This project demonstrates:
- How encryption and compression can be combined
- Implementation of classic algorithms from scratch
- Binary data handling in C++
- Understanding of cryptographic principles
- Trade-offs between security and efficiency

## Limitations

- DES is deprecated (use AES instead)
- No authentication (use HMAC)
- Fixed block size
- No padding scheme mentioned
- Visual Studio 2010 specific

## License

Academic use only.

## Author

**Lening Li**
- GitHub: [@leelening](https://github.com/leelening)
