#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <time.h>

#define KEYSIZE 16  // 128 bits
#define SECONDS_IN_24H 86400  // 24 hours in seconds
#define SECONDS_IN_12H 43200  // 12 hours in seconds

// Fonction pour convertir une chaîne hexadécimale en tableau d'octets
void hex_to_bytes(const char *hex, unsigned char *bytes) {
    for (size_t i = 0; i < strlen(hex) / 2; i++) {
        sscanf(hex + 2 * i, "%2hhx", &bytes[i]);
    }
}

// Fonction pour tester si une clé donnée correspond au texte chiffré connu
int test_key(unsigned char *key, unsigned char *iv, unsigned char *plaintext, unsigned char *ciphertext) {
    AES_KEY enc_key;
    unsigned char out[16];
    
    AES_set_encrypt_key(key, 128, &enc_key);
    AES_cbc_encrypt(plaintext, out, 16, &enc_key, iv, AES_ENCRYPT);
    
    // Comparer le texte chiffré généré avec celui fourni
    return memcmp(out, ciphertext, 16) == 0;
}

int main() {
    // Les données fournies : plaintext, ciphertext et IV
    char plaintext_hex[] = "255044462d312e350a25d0d4c5d80a34";
    char ciphertext_hex[] = "d06bf9d0dab8e8ef880660d2af65aa82";
    char iv_hex[] = "09080706050403020100A2B2C2D2E2F2";

    unsigned char plaintext[16], ciphertext[16], iv[16];
    hex_to_bytes(plaintext_hex, plaintext);
    hex_to_bytes(ciphertext_hex, ciphertext);
    hex_to_bytes(iv_hex, iv);

    // Timestamp de l'horodatage du fichier chiffré (2018-04-17 23:08:49 UTC)
    time_t file_time = 1524004129;  // 2018-04-17 23:08:49 en timestamp UNIX

    unsigned char key[KEYSIZE];
    int found = 0;
    
    // Étendre la fenêtre de recherche à -24h et +12h autour de l'horodatage du fichier
    for (time_t t = file_time - SECONDS_IN_24H - SECONDS_IN_12H; t <= file_time + SECONDS_IN_12H; t++) {
        srand(t);
        
        // Générer la clé à partir de srand
        for (int i = 0; i < KEYSIZE; i++) {
            key[i] = rand() % 256;
        }

        // Tester si la clé est correcte
        if (test_key(key, iv, plaintext, ciphertext)) {
            printf("Clé trouvée pour timestamp : %lld\n", (long long)t);
            printf("Clé : ");
            for (int i = 0; i < KEYSIZE; i++) {
                printf("%.2x", key[i]);
            }
            printf("\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Clé non trouvée dans la plage de temps donnée.\n");
    }

    return 0;
}
