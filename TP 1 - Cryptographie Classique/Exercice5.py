import itertools

# Fonction XOR pour deux chaînes sous forme de caractères alphabétiques
def xor_texts(text1, text2):
    result = []
    for char1, char2 in zip(text1.upper(), text2.upper()):
        if char1.isalpha() and char2.isalpha():  # Ignorer les caractères non alphabétiques
            value1 = ord(char1) - ord('A')
            value2 = ord(char2) - ord('A')
            # Calculer la différence en considérant l'alphabet circulaire
            xor_value = (value1 - value2 + 26) % 26
            result.append(chr(xor_value + ord('A')))
        else:
            result.append(char1)  # Garder les caractères non alphabétiques inchangés
    return ''.join(result)

# Charger le corpus de mots français à partir d'un fichier
def charger_corpus(filepath):
    with open(filepath, 'r', encoding='utf-8') as fichier:
        # Retourner une liste de mots sans les espaces
        return [mot.strip() for mot in fichier]

# Trouver des paires de mots correspondant à un XOR donné et s'arrêter dès qu'une paire est trouvée
def chercher_paires(xor_target, liste_mots):
    for mot_a, mot_b in itertools.combinations(liste_mots, 2):
        if len(mot_a) == len(mot_b) == len(xor_target):
            if xor_texts(mot_a, mot_b) == xor_target:
                print(f"Correspondance trouvée : {mot_a}, {mot_b}")
                return  # Arrêter la recherche après la première correspondance trouvée

# Les textes chiffrés
texte_chiffre1 = "HQQYAJT"
texte_chiffre2 = "RJAJPWG"

# Calculer le XOR des deux textes chiffrés
xor_resultat = xor_texts(texte_chiffre1, texte_chiffre2)
print(f"XOR des deux chiffrés : {xor_resultat}")

# Charger le corpus depuis le fichier 'wordlist.txt'
mots_corpus = charger_corpus('wordlist.txt')

# Rechercher les paires correspondantes et s'arrêter dès qu'une correspondance est trouvée
chercher_paires(xor_resultat, mots_corpus)
