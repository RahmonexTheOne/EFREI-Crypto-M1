#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

#define KEYSIZE 16
#define BLOCKSIZE 16

// Fonction pour convertir une chaîne hexadécimale en tableau d'octets
void hex_to_bytes(const char* hex_string, unsigned char* byte_array) {
    for (size_t i = 0; i < strlen(hex_string) / 2; i++) {
        sscanf(hex_string + 2 * i, "%2hhx", &byte_array[i]);
    }
}

// Fonction pour générer une clé à partir d'une graine donnée (seed)
void generate_key_from_seed(time_t seed, unsigned char* key) {
    srand(seed);
    for (int i = 0; i < KEYSIZE; i++) {
        key[i] = rand() % 256;
    }
}

// Fonction pour chiffrer un bloc AES-128-CBC
int encrypt_block_aes_128_cbc(unsigned char* key, unsigned char* iv, unsigned char* plaintext, unsigned char* ciphertext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return 0;

    int len;
    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv) != 1) return 0;
    if (EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, BLOCKSIZE) != 1) return 0;

    EVP_CIPHER_CTX_free(ctx);
    return 1;
}

// Fonction pour comparer deux blocs de données
int compare_blocks(unsigned char* block1, unsigned char* block2) {
    return memcmp(block1, block2, BLOCKSIZE) == 0;
}

int main() {
    unsigned char plaintext[BLOCKSIZE], known_ciphertext[BLOCKSIZE], iv[BLOCKSIZE], generated_key[KEYSIZE], generated_ciphertext[BLOCKSIZE];

    // Convertir les chaînes hexadécimales en octets
    hex_to_bytes("255044462d312e350a25d0d4c5d80a34", plaintext);
    hex_to_bytes("d06bf9d0dab8e8ef880660d2af65aa82", known_ciphertext);
    hex_to_bytes("09080706050403020100A2B2C2D2E2F2", iv);

    // Définir la plage de recherche basée sur l'horodatage
    long long base_timestamp = 1524004129;
    time_t start_time = base_timestamp - 24 * 60 * 60;
    time_t end_time = base_timestamp + 24 * 60 * 60;

    // Parcourir toutes les valeurs de graine dans la plage donnée
    for (time_t current_time = start_time; current_time <= end_time; current_time++) {
        generate_key_from_seed(current_time, generated_key);

        if (!encrypt_block_aes_128_cbc(generated_key, iv, plaintext, generated_ciphertext)) {
            printf("Erreur de chiffrement\n");
            return 1;
        }

        // Si les blocs correspondent, afficher la clé
        if (compare_blocks(generated_ciphertext, known_ciphertext)) {
            printf("Clé trouvée ! Timestamp : %lld\nClé : ", (long long)current_time);
            for (int i = 0; i < KEYSIZE; i++) printf("%.2x", generated_key[i]);
            printf("\n");
            return 0;
        }
    }

    printf("Clé non trouvée dans la plage de temps donnée.\n");
    return 1;
}
