# The Space Protector

Galaksiyi istilacı uzaylı gemilerinden korumak üzere sonu olmayan bir yola çıkan uzay gemisini yönettiğiniz bir oyun. C++ ve Raylib kütüphanesi kullanılarak Visual Studio 2022 üzerinde geliştirildi.

## Oynanış

- Oyuncu, ekranın alt kısmında yer alan bir uzay gemisini x ekseninde kontrol eder.
- Üzerine durmadan gelen düşman uzay gemilerini yok etmek için sürekli olarak ateş eder.
- Düşman uzay gemileri, seviyelerine göre farklı canlara sahip olup oyuncunun her bir mermisi ile bir canları azalır ve canları bitince yok olurlar.
- Yok olduklarında ihtimallere bağlı olarak bir takım güçlendirmeler düşürürler ve bu güçlendirmeleri oyuncu toplarsa, oyuncunun uzay gemisini bir süreliğine güçlendirir.
- Oyuncu olabildiğince uzun süre hayatta kalmaya çalışıp zamanla artan skor çarpanı ile beraber yüksek bir skora ulaşmayı hedefler.

&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; ![image alt](https://github.com/Rogedeen/FirstOneEver/blob/b826061c7de352960250e575fc91ef6680731ac0/normal%20oyun%20i%C3%A7i%20ss.png) &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;  &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;![iamge alt](https://github.com/Rogedeen/FirstOneEver/blob/65f54877d51bffcccff1b9cdb4d69c8de241dd5d/klon%20ve%20ulti%20%2B%20power%20up%20d%C3%BC%C5%9Fme.png)

## Mekanikler

- **Düşman uzay gemileri**: Farklı seviyelerde ve can sayılarında düşman uzay gemileri.
- **Güçlendirmeler**: Düşman uzay gemilerinin düşürdüğü güçlendirmeler.
- **Skor sistemi**: Oyuncunun düşmanları yok ettikçe artan bir skor.
- **Zorluk seviyesi**: Oyunun zorluk seviyesi zamanla artar.
- **Ses efektleri**: Oyuncunun ateş etmesi ve düşmanların yok olması gibi olaylar için ses efektleri.
- **Karakter hareketi**: Oyuncunun uzay gemisini x ekseninde hareket ettirebilmesi.

## Güçlendirmeler

- **Hız Güçlendirmesi**: Oyuncunun uzay gemisinin hızını artırır.
- **Atış Hızı Güçlendirmesi**: Oyuncunun atış hızını artırır.
- **Can Güçlendirmesi**: Oyuncunun uzay gemisinin canını maksimum 5 olacak şekilde arttırır.
- **Klonlama Güçlendirmesi**: Oyuncunun uzay gemisinin yanında bir klon oluşturur ve bu klon oyuncu ile simetrik şekilde hareket ederek oyuncuyu güçlendirmeleri ile beraber taklit eder.
- **Düşman Yavaşlatma Güçlendirmesi**: Düşman uzay gemilerinin hızını azaltır.

  ![image alt](https://github.com/Rogedeen/FirstOneEver/blob/a1d857e7f7b242bfbfd78ada3e123b0cc0eb92fb/G%C3%BC%C3%A7lendirmeler.png)

## Kurulum

1. Projeyi https://github.com/Rogedeen/FirstOneEver adresinden indirin veya klonlayın.
2. Raylib kütüphanesini kurun. Raylib'i [resmi web sitesinden](https://www.raylib.com/) indirebilirsiniz.
3. Visual Studio 2022'yi açın ve projeyi açın.
4. Projeyi derleyin ve çalıştırın.
