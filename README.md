#### Copyright 2023 Nastase Cristian-Gabriel 315CA
#
# Tema 3

## Task 1 - Magic Keyboard

    Pentru acest task, am folosit implementarea mea a laboratorului 11,
    de pe lambda checker. Este implementarea clasica si nu am modificat
    nimic la aceasta, intrucat nu consider ca este necesar sa explic sau
    sa adaug comentarii implementarii pt trie.

        In main am facut un while care citeste comenzi pana atunci cand
    se introduce comanda "EXIT".
        Nu stiu cat de eficient este, probabil ca nu este atat de eficient,
    dar atunci cand m-am apucat de implementare am considerat ca este
    elegant ca intr-o singura functie sa ma ocup si de AUTOCORRECT si
    de AUTOCOMPLETE. Astfel, am creat functiile "correct_or_complete",
    care la randul ei apeleaza functia "traverse_trie". Pentru a stii
    care comanda este apelata, am pus parametrul "command_type" care
    poate fi 0 (pt AUTOCORRECT) si 1 (pt AUTOCOMPLETE).

        - functia "correct_or_complete": am declarat tot ce am nevoie.
    variabilele "shortest_word", "first_in_dict", "highest_freq_word"
    sunt pentru autocomplete. Numele sunt sugestive. Le-am initializat
    pe toate cu "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx", deoarece este un sir
    suficient de mare, ca sa nu am probleme cand caut cuvintele cerute.
    Nu pot sa spun ca sunt multumit, dar doar la asta m-am putut gandi.
    Pentru optimizaze (liniile 98->114), in cazul in care am autocorrect,
    adica prefix, am redus numarul de apeluri recursive pe care trebuie
    sa le faca functia "traverse_trie". Astfel, incep parcurgerea tuturor
    cuvintelor din trie incepand de la "prefix......".
        La final afisez corespunzator cerintei.

        - functia "traverse_trie": in aceasta functie se parcurg absolut
    toate cuvintele din trie -> in liniile 68->74 se construieste un cuvant
    in variabila "search" si se apeleaza recursiv de la nodul respectiv.
    Acel if este pentru optimizare, intrucat cautam cuvinte doar pe o cale
    care este initializata. Daca node->children[i] nu este initializat,
    inseamna ca mai departe nu vom avea niciun cuvant.
        In continuare, la inceputul functiei, avem un if (de la linia 19 la 63),
    care verifica daca am am gasit un cuvant (adica end_of_word == 1). Odata
    ajunsi aici, verificam ce comanda avem: autocomplete sau autocorrect.
        Pentru autocomplete numaram cate litere difera cuvantul gasit fata de cel
    primit de la tastatura. Daca acest contor este mai mic sau egal cu
    k(numarul maxim de caractere cu care poate sa difere), inseamna ca avem
    un cuvant valid si il afisam.
        Pentru autocorrect numaram daca primele strlen(word) caractere din
    cuvantul cautat sunt aceleasi ca ale prefixului dat de la tastatura(word).
    Daca da, stocam cuvantul in variabilele shortest_word, first_in_dict,
    highest_freq_word, daca gasim cel mai scurt cuvant/ primul cuvant in
    dictionar/ cuvantul cu cea mai mare frecventa.

        - functia "load": citeste cuvant cu cuvant din fisier si le
    introduce in trie.


## Task 2 - kNN

        Am folosit functiile de la laboratorul 09 pentru BST, insa am aplicat cateva
    modificari. In functia "bst_tree_insert" Am contorizat in variabila "contor"
    nivelul pe care ma aflu, pentru a detecta unde isi are loc noul nod: stanga sau
    dreapta. Am tratat cazurile pentru 2 coordonate si 3 coordonate. In functia
    load_file, citesc cu fgets fiecare set de coordonate si le pun cu memcpy in
    "data" fiecarui nod. Astfel, cu aceasta implementare am decis sa folosesc de-a
    lungul intregului cod "sscanf" pentru a extrage valorile. (am pus "test = sscanf(.....);"
    deoarece sscanf returneaza o valoare. este un fel de programare defensiva,
    plus ca primeam errori de coding style).

        Se garanteaza ca doar pe prima linie se va incepe cu operatia LOAD, asa ca
    nu este necesar sa introducem aceasta comanda in loop.
        In while tratam fiecare comanda a programului: "NN", "RS", "EXIT".
    Am creeat doua structuri:
            -NN-> in care stochez suma si valoarea celui mai apropiat vecin fata
        de punctul dat. Am creeat aceasta structura pentru a putea stoca vecini
        multiplii care se afla la acceasi distanta. Stiind suma, putem identifica
        usor daca exista vecini multiplii (toata magia se intampla in liniile 73->80).
            -RS-> in care stochez coordonatele introduse de la tastatura

        In functia "NN", calculez distanta eucliniana in variabila sum, verific daca
    este cel mai apropiat vecin, iar apoi apelez recursiv left si right. In cazul in
    care am mai multi vecini la aceeasi distanta lipesc cu strcat \n + <noul nod>.
    La final afisez in main valoarea stringului din structura NN.

        In functia "RS", parcurg absolut fiecare nod (left, right) si pentru fiecare
    nod, verific daca punctul gasit se incadreaza in coordonatele date de la tastatura.



