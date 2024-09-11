import string

alphabet = string.ascii_uppercase

# Convertir un caractère en index dans l'alphabet
def char_to_index(char):
    return ord(char) - ord('A')

# Convertir un index en un caractère dans l'alphabet
def index_to_char(index):
    return chr(index % 26 + ord('A'))

# Calculer le XOR entre deux caractères
def xor_chars(c1, c2):
    return (char_to_index(c1) ^ char_to_index(c2)) % 26  # Limiter à 26 pour rester dans l'alphabet

# Appliquer le XOR entre les deux chiffrés pour trouver la différence
def calculate_key_diff(cipher1, cipher2):
    return [xor_chars(c1, c2) for c1, c2 in zip(cipher1, cipher2)]

# Appliquer la clé dérivée pour déchiffrer un mot
def apply_key(word, key_diff):
    decoded_word = ''.join(index_to_char((char_to_index(c) ^ key) % 26) for c, key in zip(word, key_diff))
    print(f"Mot: {word} -> Déchiffré: {decoded_word}")
    return decoded_word

# Charger un corpus à partir d'un fichier texte
def load_corpus(filepath):
    with open(filepath, 'r', encoding='utf-8') as file:
        # Limiter le corpus à 1000 mots pour éviter un trop grand nombre de combinaisons
        corpus = [line.strip().upper() for line in file if line.strip().isalpha() and len(line.strip()) == 7][:1000]
    print(f"Corpus chargé avec {len(corpus)} mots.")
    return corpus

# Chiffrés donnés
cipher1 = "HQQYAJT"
cipher2 = "RJAJPWG"

# Calcul de la différence de clé entre les deux chiffrés
key_diff = calculate_key_diff(cipher1, cipher2)
print(f"Différence des deux chiffrés : {key_diff}")

# Charger le corpus de mots
corpus = load_corpus('wordlist.txt')

possible_pairs = []

# Parcourir chaque mot du corpus et vérifier si une correspondance est trouvée avec le premier chiffré
for word in corpus:
    decoded_word1 = apply_key(word, key_diff)
    # Si le premier mot décrypté correspond au premier chiffré, tester le deuxième mot
    if decoded_word1 == cipher1:
        print(f"Correspondance trouvée pour le premier chiffré: {word}")
        decoded_word2 = apply_key(word, key_diff)
        if decoded_word2 == cipher2:
            print(f"Correspondance trouvée pour les deux chiffrés: {word}")
            possible_pairs.append((word, word))

# Affichage des résultats
if possible_pairs:
    print("Couples de textes clairs possibles :")
    for pair in possible_pairs:
        print(pair)
else:
    print("Aucun couple trouvé.")
