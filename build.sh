#!/usr/bin/env bash
# ============================================================
# 🧰 Gabisa-Ngoding CLI Installer
# ============================================================
# Dukungan: Linux, macOS
# Fungsi:
#   - Deteksi OS dan compiler
#   - Cek & install dependensi dasar (cmake, g++, clang++)
#   - Build otomatis (fast atau shared)
# ============================================================

set -e

# ------------------------------------------------------------
# 🔍 Deteksi Sistem Operasi
# ------------------------------------------------------------
isMacos() {
    [[ "$OSTYPE" == "darwin"* ]]
}

isLinux() {
    [[ "$OSTYPE" == "linux"* ]]
}

jenisLinux() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        echo "$ID"
    else
        echo "unknown"
    fi
}

# ------------------------------------------------------------
# 🧩 Deteksi Compiler
# ------------------------------------------------------------
deteksiKompiler() {
    if command -v clang++ >/dev/null 2>&1; then
        echo "clang++"
    elif command -v g++ >/dev/null 2>&1; then
        echo "g++"
    else
        echo "tidak ada"
    fi
}

# ------------------------------------------------------------
# 🧱 Cek Dependensi
# ------------------------------------------------------------
cekDepensi() {
    echo "🔍 Mengecek dependensi..."

    local missing=()

    for dep in cmake make; do
        if ! command -v "$dep" >/dev/null 2>&1; then
            missing+=("$dep")
        fi
    done

    local compiler
    compiler=$(deteksiKompiler)
    if [ "$compiler" = "tidak ada" ]; then
        missing+=("g++ atau clang++")
    fi

    if [ ${#missing[@]} -gt 0 ]; then
        echo "⚠️ Dependensi yang hilang: ${missing[*]}"
        return 1
    else
        echo "✅ Semua dependensi sudah terpasang."
        return 0
    fi
}

# ------------------------------------------------------------
# 📦 Install Dependensi (Linux)
# ------------------------------------------------------------
installDepensi() {
    if isMacos; then
        echo "🍎 Menggunakan Homebrew..."
        if ! command -v brew >/dev/null 2>&1; then
            echo "⚠️ Homebrew tidak ditemukan. Silakan install manual dari https://brew.sh/"
            exit 1
        fi
        brew install cmake llvm make
    elif isLinux; then
        local distro
        distro=$(jenisLinux)
        echo "🐧 Mendeteksi Linux: $distro"
        case "$distro" in
            ubuntu|debian)
                sudo apt update
                sudo apt install -y build-essential cmake clang make
                ;;
            fedora)
                sudo dnf install -y gcc-c++ cmake clang make
                ;;
            arch)
                sudo pacman -Sy --noconfirm base-devel cmake clang make
                ;;
            *)
                echo "⚠️ Distro tidak dikenal. Silakan install dependensi manual."
                ;;
        esac
    else
        echo "⚠️ OS tidak dikenali. Instalasi otomatis dibatalkan."
        exit 1
    fi
}

# ------------------------------------------------------------
# 🏗️ Jalankan Build
# ------------------------------------------------------------
buildProject() {
    local shared=$1
    local debug=$2

    echo "🧱 Menyiapkan build folder..."
    mkdir -p build && cd build

    if [ "$shared" = true ]; then
        cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DDEBUG_AKTIF=$debug ..
    else
        cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DDEBUG_AKTIF=$debug ..
    fi

    echo "⚙️ Membuild proyek..."
    cmake --build . -j$(nproc)

    echo "📦 Menginstall (lokal ke /usr/local)..."
    sudo cmake --install . --prefix /usr/local

    echo "✅ Instalasi selesai!"
}

# ------------------------------------------------------------
# 🚀 CLI Init
# ------------------------------------------------------------
cliInit() {
    local os="unknown"
    local compiler="tidak ada"

    if isMacos; then os="macOS"
    elif isLinux; then os="Linux ($(jenisLinux))"
    fi

    compiler=$(deteksiKompiler)

    echo ""
    echo "---------- GABISA-NGODING INSTALLER ----------"
    echo "🖥️  Sistem operasi kamu: $os"
    echo "🧩 Kompiler yang dideteksi: $compiler"
    echo "----------------------------------------------"
    echo "1. Install cepat (static release)"
    echo "2. Install shared library"
    echo "3. Install mode debug"
    echo "0. Keluar"
    echo "----------------------------------------------"
    read -rp "Pilih opsi: " opsi

    case $opsi in
        1)
            buildProject false 0
            ;;
        2)
            buildProject true 0
            ;;
        3)
            buildProject false 1
            ;;
        0)
            echo "Keluar dari installer."
            exit 0
            ;;
        *)
            echo "❌ Opsi tidak dikenal."
            ;;
    esac
}

# ------------------------------------------------------------
# 🚦 Main
# ------------------------------------------------------------
main() {
    echo "🧠 Memulai installer Gabisa-Ngoding..."
    if ! cekDepensi; then
        read -rp "Ingin install dependensi otomatis? (y/n): " yn
        if [[ $yn == [Yy]* ]]; then
            installDepensi
        else
            echo "❌ Dependensi belum lengkap. Instalasi dibatalkan."
            exit 1
        fi
    fi
    cliInit
}

main "$@"
