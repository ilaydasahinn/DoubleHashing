#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define R 2
#define M 17
#define MM 16

/* Samples.doc'ta yaz�l� olan dosya isimlerini tutan char tipinde bir pointer to poiner ve 
    dosya say�s�n� tutan integer tipinde bir m de�erinden olu�ur. */
struct file
{
    char **cp;
    int m;
};

/*  Samples.doc'ta ad� bulunan veya veri taban�na sonradan eklenmek istenen d�k�manlar�n ad� ve key say�s� tutulur.
    Point de�eri bak�lan hash tablosunda d�k�man bulunup bulunmad���n� anlamak i�in tutulmu�tur. */
struct document
{
    long long key;
    int point;
    char name[255];
};

FILE *openFile(char *, char *); //Dosyalar� a�ma fonksiyonu.
struct file readData(FILE *); //Samples.doc'u okuyan fonksiyon.
int readData2(FILE *); //Samples.doc i�erisindeki d�k�manlar� okuyan fonksiyon.
int hashHesaplama(char *); //Okunan d�k�manlar�n hash say�s�n�(key) hesaplayan fonksiyon.
void hashTable(struct document, struct document *); //D�k�manlar� key say�s�na g�re hash tablosuna yerle�tiren fonksiyon.
void answerControl(char, struct document *); //Kullan�c�n�n yeni dosya eklemek isteyip istemedi�ini kontrol eden fonksiyon.
void getDocument(struct document *); //E�er kullan�c� yeni dosya eklemek istiyorsa bu dosyan�n key de�erini hesaplayan fonksiyon.
int *searchDocument(struct document *, struct document); //Yeni eklenecek dosyay� hash tablosunda arayan fonksiyon.
void insert(struct document, struct document *); //Hash tablosuna dosyalar� ekleyen fonksiyon.

int main()
{
    struct file file;
    char x[255];
    int m;
    int i;
    FILE *fp;
    char ch;
    long long key = 0;
    struct document document;
    int k = 0;
    int t = 0;
    struct document *hash;
    hash = (struct document *)calloc(M, sizeof(struct document));
    char answer;

    fp = openFile("directory/index.txt", "r");
    file = readData(fp);
    m = file.m;

    for (i = 0; i < m; i++)
    {

        strcpy(x, file.cp[i]);
        char *m = "directory/";
        snprintf(document.name, sizeof document.name, "%s%s", m, x); //D�k�man� bulundu�u pathten a�abilmek i�in ba��na klas�r ismi eklenir.
        printf("Okunan dokumanin adi: %s \n\n", document.name); 
        fp = openFile(document.name, "r");
        key = readData2(fp);
        document.key = key;
        document.point = 1;

        hashTable(document, hash);
        printf("Hash tablosunun guncel durumu: \n");
        for (k = 0; k < M; k++)
        {
            printf("%d: %s\n", k, hash[k].name);
        }
        printf("\n");
    }

    printf("Hash tablosunun son durumu: \n");

    for (k = 0; k < M; k++)
    {
        printf("%d: %s\n", k, hash[k].name);
    }

    printf("\n");

    printf("Eklemek istediginiz dokuman var mi? (Y/N)");
    scanf("%c", &answer);
    if (answer != 'n' && answer != 'N')
    {
        answerControl(answer, hash);
    }
    return 0;
}

FILE *openFile(char *fileName, char *mode)
{

    FILE *fp = fopen(fileName, mode);
    if (fp == NULL)
    {
        printf("Could not open file! %s", fileName);
        return NULL;
    }
    return fp;
}
struct file readData(FILE *fp)
{
    char **cp;
    char **cp2;
    int m = 1;
    int i = 0;
    int j = 0;

    cp = (char **)calloc(1, sizeof(char *)); //Samples.doc'taki file isimlerini ataca��m�z dizi i�in 1'lik yer ayr�l�r.
    cp[0] = (char *)calloc(255, sizeof(char)); //Yukar�da 1'lik yer a�t���m�z dizinin h�cresi i�in 255 karakterlik yer a��l�r. (Dosya isimlerinin max karakteri kadar.)

    i = 0;

    while (fscanf(fp, "%s\n", cp[i]) != EOF)
    {
        cp2 = (char **)realloc(cp, ((i + 2) * sizeof(char *))); //Dosya isimleri al�nd�k�a dosya sonuna dek ayr�lan yer say�s� art�r�l�r.
        cp = cp2;
        cp[i + 1] = (char *)calloc(255, sizeof(char));
        i++;
    }

    struct file file = {cp, i};

    fclose(fp);
    return file;
}

int readData2(FILE *fp) //Samples.doc'taki dosyalar�n i�eri�i okunur.
{
    char **arr;
    char **arr2;
    int m = 1;
    int i = 0;
    int j = 0;
    int k = 0;
    long long key = 0;
    char c;
    int t = 0;
    int n = 0;
    char *temp;

    arr = (char **)calloc(1, sizeof(char *)); //Stringlerin yerle�tirilece�i dizi i�in yer allocate ettik.
    arr[0] = (char *)calloc(1, sizeof(char)); //Her bir string i�in yer allocate ettik.

    i = 0;

    while ((c = fgetc(fp)) != EOF)
    {
        if (isalpha(c))
        {
            temp = (char *)realloc(arr[n], t + 2); //c bir harf ise realloc yap�yoruz.
            arr[n] = temp;
            arr[n][t] = (char)c; //string dizisinin n. stringinin t. karakterine de c'yi at�yoruz.
            t++; 
        }
        else
        {
            if (t != 0)
            {

                arr[n][t] = '\0'; //E�er karakterler al�nm��sa stringin sonun '\0' koyuyoruz.
                arr2 = (char **)realloc(arr, ((n + 2) * sizeof(char *))); //Bir sonraki string i�in realloc yap�yoruz.
                arr = arr2;
                n++;
                arr[n] = (char *)malloc(sizeof(char)); //Di�er stringin karakterleri i�in 1'lik yer a��yoruz.
                printf("Dokumandaki %d. kelime: '%s' \n", n, arr[n - 1]);
                t = 0;
            }
        }
    }

    if (t != 0)
    {
        arr[n][t] = '\0';
        n++;
        t = 0;
        printf("Dokumandaki %d. kelime: '%s' \n", n, arr[n - 1]);
    }

    for (j = 0; j < n; j++)
    {
        while (((arr[j][k] >= 97 && arr[j][k] <= 122) || (arr[j][k] >= 65 && arr[j][k] <= 90)) && arr[j][k] != '\0') //Karakter'in harf kontrol�.
        {
            k++;
        }
        if (arr[j][k] == '\0')
        {
            key += hashHesaplama(arr[j]); //Key de�eri hesaplan�p atan�r.
        }
        else
        {
            printf("Dokumandaki %d. kelime harf ve bosluk disinda karakterler de icerir.\n", j);
        }
        k = 0;
    }
    printf("\n");

    fclose(fp);

    return key;
}
/* Verilen kelimelerin key de�erini hesaplay�p d�nd�ren fonksiyon. */
int hashHesaplama(char *arr)
{
    int n = 0;
    int i = 0;
    int term;
    long long int key = 0;
    printf("Hesaplama yapilacak kelime: ");
    while (arr[n] != '\0')
    {
        printf("%c", arr[n]);
        n++;
    }
    printf("\nKelimeye ait harf sayisi: %d", n); //Kelimeye ait harf sayisi
    printf("\n");

    for (i = 0; i < n; i++)
    {

        term = arr[i] * pow(R, n - 1);
        printf("TERM : %d\n", term);
        key += term;
        printf("SUM : %lld\n", key);
    }

    return key;
}

/* Key de�erine g�re dosyalar� hash tablosuna ekleyen fonksiyon. */
void hashTable(struct document document, struct document *hash)
{
    int hash1;
    int hash2;
    int hash_key;
    int k = 0;
    int flag = 0;

    hash1 = document.key % M;//HASH1(key)
    printf("HASH1: %d\n", hash1); 
    hash2 = 1 + (document.key % MM);//HASH2(key)
    printf("HASH2: %d\n", hash2); 

    while ((k < M) && !flag)
    {
        hash_key = (hash1 + k * hash2) % M;//HASH(key, i)
        printf("HASH_KEY: %d\n", hash_key);

        if (hash[hash_key].point != 1) //Adres bo�.
        {

            hash[hash_key] = document; //Hash'e ekleme i�lemi.
            flag = 1;
            k = 0;
        }
        else
        { //Adres bos degil.
            printf("%d. hucre bos degil!\n", hash_key);
            k++;
        }
    }

    if (k < M)
    {
        printf("Dokuman hash tablosunun %d. hucresine eklendi!", hash_key);
    }
    else
    {
        printf("Dokuman hash tablosu dolu oldugu icin eklenemedi!");
    }
    printf("\n**%s ", document.name);
    printf("%lld**\n", document.key);

    flag = 0;
}
/* Kullan�c�n�n yeni d�k�man eklemek isteyip istemedi�ini soran fonksiyon. */
void answerControl(char answer, struct document *hash)
{
    while (answer != 'Y' && answer != 'N' && answer != 'y' && answer != 'n')
    {
        printf("Lutfen Y ya da N giriniz.");
        scanf("%c", &answer);
    }
    if (answer == 'Y' || answer == 'y')
    {
        getDocument(hash);
    }
    else
    {
        return;
    }
}
/* Yeni d�k�man eklenmek istenildi�inde �al��an fonksiyon. */
void getDocument(struct document *hash)
{
    struct document new_document;
    char name[255];
    FILE *fp;
    FILE *fp2;
    struct file file;
    int key;
    int *hash_indexes;
    char **arr;
    char **arr2;
    char **arr3;
    int i = 0;
    int k = 0;
    char c;
    char *temp;
    int n = 0;
    int n2 = 0;
    int t = 0;
    int t2 = 0;
    int flag = 0;
    int x = 0;

    printf("Dokumanin adini giriniz: ");
    scanf("%s", name);
    fp = openFile(name, "r");
    key = readData2(fp);
    printf("TOTAL_SUM= %d\n", key);

    new_document.key = key;
    new_document.point = 1;
    strcpy(new_document.name, name);

    hash_indexes = searchDocument(hash, new_document);
    if (hash_indexes[0] == -1) //hash_indexes'�n ilk eleman� -1 ise ayn� key'e sahip bir ba�ka eleman tabloda yok.
    {
        printf("Dokuman hash tablosunda bulunamadi!");
        insert(new_document, hash); //Tabloda ayn� key de�erine sahip eleman bulunamad��� i�in d�k�man uygun yere ekleniyor.
    }
    else
    {
        while (hash_indexes[k] != -1) //hash_indexes'�n -1 de�erine sahip eleman�na rastlayana dek t�m ayn� key say�l� elemanlar taran�yor.
        {
            fp = openFile(new_document.name, "r");
            fp2 = openFile(hash[hash_indexes[k]].name, "r");

            arr = (char **)calloc(1, sizeof(char *));
            arr[0] = (char *)calloc(1, sizeof(char));

            while ((c = fgetc(fp2)) != EOF)
            {
                if (isalpha(c))
                {
                    temp = (char *)realloc(arr[n], t + 2);
                    arr[n] = temp;
                    arr[n][t] = (char)c;
                    t++;
                }
                else
                {
                    if (t != 0)
                    {

                        arr[n][t] = '\0';
                        arr2 = (char **)realloc(arr, ((n + 2) * sizeof(char *)));
                        arr = arr2;
                        n++;
                        arr[n] = (char *)malloc(sizeof(char));
                        printf("Dokumandaki %d. kelime: '%s' \n", n, arr[n - 1]);
                        t = 0;
                    }
                }
            }

            if (t != 0)
            {
                arr[n][t] = '\0';
                n++;
                t = 0;
                printf("Dokumandaki %d. kelime: '%s' \n", n, arr[n - 1]);
            }

            arr3 = (char **)calloc(1, sizeof(char *));
            arr3[0] = (char *)calloc(1, sizeof(char));

            while ((c = fgetc(fp)) != EOF)
            {
                if (isalpha(c))
                {
                    temp = (char *)realloc(arr3[n2], t2 + 2);
                    arr3[n2] = temp;
                    arr3[n2][t2] = (char)c;
                    t2++;
                }
                else
                {
                    if (t2 != 0)
                    {

                        arr3[n2][t2] = '\0';
                        arr2 = (char **)realloc(arr3, ((n2 + 2) * sizeof(char *)));
                        arr3 = arr2;
                        n2++;
                        arr3[n2] = (char *)malloc(sizeof(char));
                        //printf("Dokumandaki %d. kelime: '%s' \n", n2, arr3[n2 - 1]);
                        t2 = 0;
                    }
                }
            }
            if (t != 0)
            {
                arr[n][t] = '\0';
                n++;
                t = 0;
                printf("Dokumandaki %d. kelime: '%s' \n", n, arr[n - 1]);
            }
            if (n == n2) //D�k�manlar�n karakter say�lar� kar��la�t�r�l�yor, e�er ayn�ysa i�erik kar��la�t�r�lmas� yap�l�yor.
            {
                while (i < n && strcmp(arr[i], arr3[i]) == 0)
                {
                    i++;
                }
                if (i == n)
                {
                    printf("Dokuman icerikleri aynidir.\n");
                    flag = 1; //�ki d�k�man�n i�erikleri bire bir ise flag'e -1 atan�yor.
                }
                else
                {
                    printf("Dokuman icerikleri ayni degildir.\n");
                }
            }
            k++;
            n = 0;
            n2 = 0;
        }
        if (flag != 1)
        {
            insert(new_document, hash); //flag -1 ise d�k�man bulunamad��� i�in tabloya eklenmesi i�in insert fonksiyonuna g�nderiliyor.
        }
    }

    for (x = 0; x < M; x++)
    {
        printf("%d: %s\n", x, hash[x].name);
    }
}
/* Eklenilmek istenen d�k�man�n keyine sahip bir ba�ka d�k�man�n hash'te bulunup bulunmad���n�, e�er bulunuyor ise hash'in hangi indislerinde
    bulundu�unu bir pointer ile d�nd�ren fonksiyon. */
int *searchDocument(struct document *hash, struct document document)
{

    int i = 0;
    int j = 0;
    int flag = 0;
    int k = 0;
    int hash1;
    int hash2;
    int hash_key;
    int *hash_indexes;
    int *temp;

    hash_indexes = (int *)calloc(1, sizeof(int)); 

    hash1 = document.key % M;
    printf("HASH1: %d\n", hash1);
    hash2 = 1 + (document.key % MM);
    printf("HASH2: %d\n", hash2);

    while (k < M)
    {
        hash_key = (hash1 + k * hash2) % M;
        printf("HASH_KEY: %d\n", hash_key);

        if (hash[hash_key].key == document.key)
        {
            hash_indexes[j] = hash_key;
            j++;
            temp = (int *)realloc(hash_indexes, (j + 1) * sizeof(int));
            hash_indexes = temp;
            k++;
        }
        else
        {
            k++;
        }
    }
    hash_indexes[j] = -1; //Dizi boyutu bilinmedi�inden dizinin son eleman�na -1 atan�yor.
    printf("Dokuman %d. hucrede bulundu!\n", hash_key);

    return hash_indexes;
}
/* Veri taban�nda bulunmayan ama sonradan eklenmek istenen d�k�manlar�n insert i�lemini ger�ekle�tiren fonksiyon. */
void insert(struct document document, struct document *hash)
{
    char name2[255];
    FILE *fp;
    fp = openFile("directory/index.txt", "a");
    fprintf(fp, "\n%s", document.name); //D�k�man�n ismini samples.doc'a ekler.
    fclose(fp);
    hashTable(document, hash); //D�k�man� hash tablosuna ekler.
    char *m = "directory/"; 
    snprintf(name2, sizeof document.name, "%s%s", m, document.name);
    rename(document.name, name2); //D�k�man� direktory klas�r� i�erisine ta��r.
}

