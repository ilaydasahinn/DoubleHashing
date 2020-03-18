# DoubleHashing
 Bu ödevde; web sayfalarına ait dökümanları veri tabanında saklayan bir arama motorunun veri tabanında, eklenmek istenen yeni bir dökümanın bulunup bulunmadığını kontrol eden ve eğer yoksa veritabanına ekleyen bir sistem tasarlanmıştır.  Ancak bu işlem, mevcut veri tabanlarının içine tek tek bakarak yapmak çok zaman alacağı için, hashing yöntemi ile yapılmıştır.  İşlem yapılırken dosyalar bir directory altında toplanmıştır. Bu dosyaların isimleri samples.doc isimli bir dosyaya yazılmıştır. samples.doc dosyası okunmak için açılmış ve bu dosyada adı bulunan her dosya da okunmak için açılmıştır. Her dosyadaki karakterler hash fonksiyonundan geçirilmiş ve dosyaların hash sayıları hesaplanmıştır. Hash tablosunda hash sayısının bulunduğu adres boş ise bu adrese dökümanın adı ve hash sayısı yazılmıştır. Bu hücre dolu ise double hashing yöntemi ile dökümanın adı hash tablosunun uygun hücresine yerleştirilmiştir. 
 
 Double Hashing Yöntemi: 
 h1(key) = key mod M 
 h2(key) = key mod M 
 h(key,i) = [h1(key) + i*h2(key)]mod M 
 
 Veri tabanına yeni bir döküman eklenmek istediğinde ise; ilk olarak döküman okunmuş ve dökümanın hash sayısı hesaplanmıştır.  Hash tablosunda hash sayısının adresi kontrol edilmiş ve eğer bu hücre dolu ise hücrede ismi bulunan döküman açılmıştır.  Bu döküman ile eklenmek istenilen dökümanın içerikleri karşılaştırılmış, içerikler aynı ise işlem bitirilmiştir. Ancak içerikler farklı ise double hashing yöntemi ile tablodaki bir sonraki adres hesaplanarak tablodaki döküman ile aynı karşılaştırma işlemi tekrarlanmıştır. Karşılaştırma işlemi yapılırken strcmp fonkdiyonu kullanılmıştır.  Eğer double hashing yöntemi ile ilerlenirken tabloda boş bir hücreye denk gelinirse yeni dökümanın adı bu hücreye yazılmıştır.  Bu işlemden sonar yeni döküman directory altına taşınmış ve sonrasında diğer directory altında bulunan dökümanların adının bulunduğu samples.doc adlı dökümana adı eklenmiş ve işlem bitirilmiştir. Tüm bu işlemlerin öncesinde hash tablosunun boyutunun; tablodaki eleman sayısının 0.6 ile bölündükten sonra elde edilen sonuçtan en küçük asal sayının olmasına karar verilmiştir. Bu ödev yapılırken her dosyanın büyük ve küçük harflerden oluşan yazılar olduğu kabul edilmiştir.  
 
 Hash Fonksiyonu:  
 key = str[0]*R^(n-1) + str[1]*R^(n-2) +…+str[n-1] 
