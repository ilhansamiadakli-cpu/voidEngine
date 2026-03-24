🌌 voidEngine v0.2
C Dili ile Geliştirilmiş, Veri Odaklı Terminal Oyun Motoru

voidEngine, terminal tabanlı oyunlar ve simülasyonlar geliştirmek için tasarlanmış, düşük seviyeli (low-level) bir oyun motorudur. Modern oyun motoru mimarilerini (Buffer, Input Management, State Machine) terminalin kısıtlı ortamına taşımayı amaçlar.

🚀 Öne Çıkan Özellikler
Çift Mimari (Dual Architecture): Motor çekirdeği (Engine), oyun mantığından (Game) ve harita tasarımcısından (Editor) tamamen izole edilmiştir.

Titremesiz Render (Flicker-Free): ANSI Kaçış Dizileri ve "Overwrite" mantığı ile saniyede 60 FPS'e kadar pürüzsüz çizim sağlar.

Asenkron Sistemler: Oyunu dondurmayan (non-blocking) girdi okuma ve gerçek zamanlı daktilo (Typewriter) efekti.

Zırhlı Çekirdek: atexit ve sinyal yakalayıcılar (SIGINT, SIGSEGV) ile program çökse dahi terminali sağlıklı haline geri döndüren koruma mekanizması.

Veri Odaklı Tasarım: Haritalar ve oyun verileri dışarıdan .txt dosyalarıyla yüklenir, kod değişikliği gerektirmez.

🛠️ Klasör Yapısı
Plaintext
voidEngine/
├── Engine/       # Motor Çekirdeği (Core, Render, Input, Utils)
├── Game/         # Oyun Mantığı ve Varlıklar
├── Editor/       # Terminal İçi Harita Tasarım Aracı
└── Makefile      # Gelişmiş Derleme Otomasyonu
📦 Kurulum ve Çalıştırma
Proje standart C kütüphanelerini kullanır, ek bir bağımlılık gerektirmez.

Repoyu Klonlayın:

Bash
git clone https://github.com/kullanici_adin/voidEngine.git
cd voidEngine
Derleme:

Bash
make all
Çalıştırma:

Oyun için: ./voidcore

Editör için: ./voideditor

🎮 Kontroller
W, A, S, D: Hareket / İmleç Kontrolü

SPACE: Editörde Obje Yerleştirme

K: Haritayı Kaydet (Editör)

Q: Güvenli Çıkış
