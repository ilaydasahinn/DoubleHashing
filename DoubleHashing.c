#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define R 2
#define M 17
#define MM 16

/* Samples.doc'ta yazýlý olan dosya isimlerini tutan char tipinde bir pointer to poiner ve 
    dosya sayýsýný tutan integer tipinde bir m deðerinden oluþur. */
struct file
{
    char **cp;
    int m;
};

/*  Samples.doc'ta adý bulunan veya veri tabanýna sonradan eklenmek istenen dökümanlarýn adý ve key sayýsý tutulur.
    Point deðeri bakýlan hash tablosunda döküman bulunup bulunmadýðýný anlamak için tutulmuþtur. */
struct document
{
    long long key;
    int point;
    char name[255];
};

FILE *openFile(char *, char *); //Dosyalarý açma fonksiyonu.
struct file readData(FILE *); //Samples.doc'u okuyan fonksiyon.
int readData2(FILE *); //Samples.doc içerisindeki dökümanlarý okuyan fonksiyon.
int hashHesaplama(char *); //Okunan dökümanlarýn hash sayýsýný(key) hesaplayan fonksiyon.
void hashTable(struct document, struct document *); //Dökümanlarý key sayýsýna göre hash tablosuna yerleþtiren fonksiyon.
void answerControl(char, struct document *); //Kullanýcýnýn yeni dosya eklemek isteyip istemediðini kontrol eden fonksiyon.
void getDocument(struct document *); //Eðer kullanýcý yeni dosya eklemek istiyorsa bu dosyanýn key deðerini hesaplayan fonksiyon.
int *searchDocument(struct document *, struct document); //Yeni eklenecek dosyayý hash tablosunda arayan fonksiyon.
void insert(struct document, struct document *); //Hash tablosuna dosyalarý ekleyen fonksiyon.

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
        snprintf(document.name, sizeof document.name, "%s%s", m, x); //Dökümaný bulunduðu pathten açabilmek için baþýna klasör ismi eklenir.
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

    cp = (char **)calloc(1, sizeof(char *)); //Samples.doc'taki file isimlerini atacaðýmýz dizi için 1'lik yer ayrýlýr.
    cp[0] = (char *)calloc(255, sizeof(char)); //Yukarýda 1'lik yer açtýðýmýz dizinin hücresi için 255 karakterlik yer açýlýr. (Dosya isimlerinin max karakteri kadar.)

    i = 0;

    while (fscanf(fp, "%s\n", cp[i]) != EOF)
    {
        cp2 = (char **)realloc(cp, ((i + 2) * sizeof(char *))); //Dosya isimleri alýndýkça dosya sonuna dek ayrýlan yer sayýsý artýrýlýr.
        cp = cp2;
        cp[i + 1] = (char *)calloc(255, sizeof(char));
        i++;
    }

    struct file file = {cp, i};

    fclose(fp);
    return file;
}

int readData2(FILE *fp) //Samples.doc'taki dosyalarýn içeriði okunur.
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

    arr = (char **)calloc(1, sizeof(char *)); //Stringlerin yerleþtirileceði dizi için yer allocate ettik.
    arr[0] = (char *)calloc(1, sizeof(char)); //Her bir string için yer allocate ettik.

    i = 0;

    while ((c = fgetc(fp)) != EOF)
    {
        if (isalpha(c))
        {
            temp = (char *)realloc(arr[n], t + 2); //c bir harf ise realloc yapýyoruz.
            arr[n] = temp;
            arr[n][t] = (char)c; //string dizisinin n. stringinin t. karakterine de c'yi atýyoruz.
            t++; 
        }
        else
        {
            if (t != 0)
            {

                arr[n][t] = '\0'; //Eðer karakterler alýnmýþsa stringin sonun '\0' koyuyoruz.
                arr2 = (char **)realloc(arr, ((n + 2) * sizeof(char *))); //Bir sonraki string için realloc yapýyoruz.
                arr = arr2;
                n++;
                arr[n] = (char *)malloc(sizeof(char)); //Diðer stringin karakterleri için 1'lik yer açýyoruz.
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
        while (((arr[j][k] >= 97 && arr[j][k] <= 122) || (arr[j][k] >= 65 && arr[j][k] <= 90)) && arr[j][k] != '\0') //Karakter'in harf kontrolü.
        {
            k++;
        }
        if (arr[j][k] == '\0')
        {
            key += hashHesaplama(arr[j]); //Key deðeri hesaplanýp atanýr.
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
/* Verilen kelimelerin key deðerini hesaplayýp döndüren fonksiyon. */
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

/* Key deðerine göre dosyalarý hash tablosuna ekleyen fonksiyon. */
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

        if (hash[hash_key].point != 1) //Adres boþ.
        {

            hash[hash_key] = document; //Hash'e ekleme iþlemi.
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
/* Kullanýcýnýn yeni döküman eklemek isteyip istemediðini soran fonksiyon. */
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
/* Yeni döküman eklenmek istenildiðinde çalýþan fonksiyon. */
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
    if (hash_indexes[0] == -1) //hash_indexes'ýn ilk elemaný -1 ise ayný key'e sahip bir baþka eleman tabloda yok.
    {
        printf("Dokuman hash tablosunda bulunamadi!");
        insert(new_document, hash); //Tabloda ayný key deðerine sahip eleman bulunamadýðý için döküman uygun yere ekleniyor.
    }
    else
    {
        while (hash_indexes[k] != -1) //hash_indexes'ýn -1 deðerine sahip elemanýna rastlayana dek tüm ayný key sayýlý elemanlar taranýyor.
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
            if (n == n2) //Dökümanlarýn karakter sayýlarý karþýlaþtýrýlýyor, eðer aynýysa içerik karþýlaþtýrýlmasý yapýlýyor.
            {
                while (i < n && strcmp(arr[i], arr3[i]) == 0)
                {
                    i++;
                }
                if (i == n)
                {
                    printf("Dokuman icerikleri aynidir.\n");
                    flag = 1; //Ýki dökümanýn içerikleri bire bir ise flag'e -1 atanýyor.
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
            insert(new_document, hash); //flag -1 ise döküman bulunamadýðý için tabloya eklenmesi için insert fonksiyonuna gönderiliyor.
        }
    }

    for (x = 0; x < M; x++)
    {
        printf("%d: %s\n", x, hash[x].name);
    }
}
/* Eklenilmek istenen dökümanýn keyine sahip bir baþka dökümanýn hash'te bulunup bulunmadýðýný, eðer bulunuyor ise hash'in hangi indislerinde
    bulunduðunu bir pointer ile döndüren fonksiyon. */
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
    hash_indexes[j] = -1; //Dizi boyutu bilinmediðinden dizinin son elemanýna -1 atanýyor.
    printf("Dokuman %d. hucrede bulundu!\n", hash_key);

    return hash_indexes;
}
/* Veri tabanýnda bulunmayan ama sonradan eklenmek istenen dökümanlarýn insert iþlemini gerçekleþtiren fonksiyon. */
void insert(struct document document, struct document *hash)
{
    char name2[255];
    FILE *fp;
    fp = openFile("directory/index.txt", "a");
    fprintf(fp, "\n%s", document.name); //Dökümanýn ismini samples.doc'a ekler.
    fclose(fp);
    hashTable(document, hash); //Dökümaný hash tablosuna ekler.
    char *m = "directory/"; 
    snprintf(name2, sizeof document.name, "%s%s", m, document.name);
    rename(document.name, name2); //Dökümaný direktory klasörü içerisine taþýr.
}

