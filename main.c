#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <raylib.h>

#define Genislik_BOYUTU 1150
#define Yükseklik_BOYUTU 700
typedef enum { EKRAN_KULLANICI_GIRISI,EKRAN_GIRIS,EKRAN_KISI } EkranDurumu;
typedef enum { ADMIN,UYE } UyeDurumu;
struct kitap {
    char isim[50];
    int yil;
    char yazar[50];
    int musaitlik;
} kitaplar[100];
struct kitap aramaSonuclari[100];
struct kullanici{
    char isim[50];
    int sifre;
};
int bulunanKitapSayisi = 0;

void dosyayaYaz(struct kitap eklenecekKitap) {
    FILE *dosya = fopen("kutuphane.txt", "a");
    if (dosya == NULL) {
        printf("Hata: Dosya acilamadi!\n");
        return; 
    }
    fprintf(dosya, "%s,%d,%s,%d\n", eklenecekKitap.isim, eklenecekKitap.yil, eklenecekKitap.yazar,eklenecekKitap.musaitlik);
    fclose(dosya);
}

void metinGirisiAl(char *metin,int *harfSayisi,int maxHarf,int sadeceSayi){
    int tus = GetCharPressed();
    while (tus > 0) {
        if ((tus >= 32) && (tus <= 125)) {
            if (*harfSayisi < maxHarf) {
                if (sadeceSayi == 1 && !(tus >= '0' && tus <= '9')) {
                    tus = GetCharPressed();
                    continue; 
                }
                metin[*harfSayisi] = (char)tus;
                metin[*harfSayisi + 1] = '\0';
                (*harfSayisi)++;
            }
        }
        tus = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        if (*harfSayisi > 0) {
            (*harfSayisi)--;
            metin[*harfSayisi] = '\0';
        }
    }
}
void kitapAra(int kriter, char *aranan) {
    bulunanKitapSayisi = 0;
    memset(aramaSonuclari, 0, sizeof(aramaSonuclari));
    FILE *dosya = fopen("kutuphane.txt", "r");
    if (dosya == NULL) return;
    char satir[150];
    struct kitap okunanKitap;
    while (fgets(satir, sizeof(satir), dosya) && bulunanKitapSayisi < 100) {
        if (sscanf(satir, "%49[^,],%d,%49[^,],%d", okunanKitap.isim, &okunanKitap.yil, okunanKitap.yazar,&okunanKitap.musaitlik) == 4) {
            int eslesme = 0;
            if (kriter == 0 && strstr(okunanKitap.isim, aranan) != NULL) eslesme = 1;
            else if (kriter == 1 && okunanKitap.yil == atoi(aranan)) eslesme = 1;
            else if (kriter == 2 && strstr(okunanKitap.yazar, aranan) != NULL) eslesme = 1;
            if (eslesme == 1) {
                aramaSonuclari[bulunanKitapSayisi] = okunanKitap;
                bulunanKitapSayisi++;
            }
        }
    }
    fclose(dosya);
}
int hesapIslemi(const char *dosyaAdi, const char *girilenIsim, int girilenSifre) {
    char satir[150];
    struct kullanici okunan;
    if (strcmp(dosyaAdi, "ADMIN.txt") == 0) {
        FILE *dosya = fopen("ADMIN.txt", "r");
        if (dosya == NULL) {
            printf("Hata: ADMIN.txt bulunamadi!\n");
            return 0;
        }
        while (fgets(satir, sizeof(satir), dosya)) {
            if (sscanf(satir, "%49[^,],%d", okunan.isim, &okunan.sifre) == 2) {
                if (strcmp(okunan.isim, girilenIsim) == 0) {
                    fclose(dosya);
                    if (okunan.sifre == girilenSifre) {
                        return 1;
                    } else {
                        return 0; 
                    }
                }
            }
        }
        fclose(dosya);
        return 0; 
    } 
    else if (strcmp(dosyaAdi, "uye.txt") == 0) {
        FILE *dosya = fopen("uye.txt", "r");
        int isimBulundu = 0;
        if (dosya != NULL) {
            while (fgets(satir, sizeof(satir), dosya)) {
                if (sscanf(satir, "%49[^,],%d", okunan.isim, &okunan.sifre) == 2) {
                    if (strcmp(okunan.isim, girilenIsim) == 0) {
                        isimBulundu = 1;
                        fclose(dosya);
                        if (okunan.sifre == girilenSifre) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            }
            fclose(dosya);
        }
        if (isimBulundu == 0) {
            dosya = fopen("uye.txt", "a");
            if (dosya != NULL) {
                fprintf(dosya, "%s,%d\n", girilenIsim, girilenSifre);
                fclose(dosya);
                return 1;
            }
        }
    }
    return 0; 
}

int kirala(const char *istenenKitapAdi, const char *kullaniciAdi, int islemTuru) {
    if (islemTuru == 1) {
        FILE *oduncDosya = fopen("odunc.txt", "r");
        FILE *geciciOdunc = fopen("gecici_odunc.txt", "w");
        int yetkiliMi = 0;
        if (oduncDosya != NULL && geciciOdunc != NULL) {
            char oSatir[150], oKullanici[50], oKitap[50];
            while (fgets(oSatir, sizeof(oSatir), oduncDosya)) {
                if (sscanf(oSatir, "%49[^,],%49[^\n\r]", oKullanici, oKitap) >= 2) {
                    if (strcmp(oKullanici, kullaniciAdi) == 0 && strcmp(oKitap, istenenKitapAdi) == 0) {
                        yetkiliMi = 1;
                    } else {
                        fprintf(geciciOdunc, "%s", oSatir);
                    }
                } else {
                    fprintf(geciciOdunc, "%s", oSatir);
                }
            }
            fclose(oduncDosya);
            fclose(geciciOdunc);
            if (yetkiliMi == 1) {
                remove("odunc.txt");
                rename("gecici_odunc.txt", "odunc.txt");
            } else {
                remove("gecici_odunc.txt");
                return -1;
            }
        } else {
            if(oduncDosya) fclose(oduncDosya);
            if(geciciOdunc) fclose(geciciOdunc);
            return -1; 
        }
    }
    FILE *dosya = fopen("kutuphane.txt", "r");
    FILE *geciciDosya = fopen("gecici.txt", "w"); 
    if (dosya == NULL || geciciDosya == NULL) {
        if(dosya) fclose(dosya);
        if(geciciDosya) fclose(geciciDosya);
        return 0; 
    }
    char satir[150];
    struct kitap okunanKitap;
    int islemYapildi = 0; 
    int beklenenDurum = (islemTuru == 0) ? 1 : 0;
    int yeniDurum = (islemTuru == 0) ? 0 : 1;
    while (fgets(satir, sizeof(satir), dosya)) {
        if (sscanf(satir, "%49[^,],%d,%49[^,],%d", okunanKitap.isim, &okunanKitap.yil, okunanKitap.yazar, &okunanKitap.musaitlik) == 4) {
            if (strcmp(okunanKitap.isim, istenenKitapAdi) == 0 && okunanKitap.musaitlik == beklenenDurum) {
                okunanKitap.musaitlik = yeniDurum; 
                fprintf(geciciDosya, "%s,%d,%s,%d\n", okunanKitap.isim, okunanKitap.yil, okunanKitap.yazar, okunanKitap.musaitlik);
                islemYapildi = 1;
            } else {
                fprintf(geciciDosya, "%s", satir); 
            }
        } else {
            fprintf(geciciDosya, "%s", satir);
        }
    }
    fclose(dosya);
    fclose(geciciDosya);
    if (islemYapildi == 1) {
        remove("kutuphane.txt");
        rename("gecici.txt", "kutuphane.txt");
        if (islemTuru == 0) {
            FILE *oduncDosya = fopen("odunc.txt", "a");
            if(oduncDosya != NULL) {
                fprintf(oduncDosya, "%s,%s\n", kullaniciAdi, istenenKitapAdi);
                fclose(oduncDosya);
            }
        }
        return 1;
    } else {
        remove("gecici.txt"); 
        return 0;
    }
}
int main() {
    EkranDurumu mevcutDurum = EKRAN_KULLANICI_GIRISI;
    UyeDurumu girisYapan = UYE;
    InitWindow(Genislik_BOYUTU, Yükseklik_BOYUTU, "Kutuphane Yonetim Sistemi");
    SetTargetFPS(60); 
    int aktifKutu=0,adminDurumu=0,uyeDurumu=0,aramaKriteri=0,aramaHarfSayisi=0; 
    char girdiIsim[50] = "\0",girdiYazar[50] = "\0",girdiYil[10] = "\0",aramaGirdisi[50]="\0",aktifKullanici[50] = "\0";;
    int harfSayisiIsim = 0,harfSayisiYil = 0,harfSayisiYazar = 0;
    char girisIsim[50] = "\0",girisSifre[20] = "\0"; 
    int harfSayisiGirisIsim = 0,harfSayisiGirisSifre = 0,aktifGirisKutusu = 0,girisHatasi = 0,kiralamaMesajDurumu = -1;;
    Rectangle adminButon = { 400, 250, 350, 60 };Rectangle uyeButon = { 400, 350, 350, 60 };Rectangle ekleButon = { 750, 120, 300, 60 };
    Rectangle araButon = { 750, 50, 300, 60 };Rectangle menuButon = { 50, 50, 150, 60 };Rectangle geriButon = { 50, 120, 150, 60 };
    Rectangle isimAra = { 50, 220, 150, 40 };Rectangle yilAra = { 220, 220, 150, 40 };Rectangle yazarAra = { 390, 220, 150, 40 };
    Rectangle aramaKutusu = { 50, 280, 490, 45 };
    while (!WindowShouldClose()) {
        Vector2 farePozisyonu = GetMousePosition();
        if (mevcutDurum == EKRAN_KULLANICI_GIRISI) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(farePozisyonu, adminButon)) {
                    memset(girisIsim, 0, sizeof(girisIsim)); memset(girisSifre, 0, sizeof(girisSifre));
                    harfSayisiGirisIsim = 0; harfSayisiGirisSifre = 0; aktifGirisKutusu = 0; girisHatasi = 0;
                    girisYapan = ADMIN;
                    mevcutDurum = EKRAN_GIRIS;
                    adminDurumu=0;
                }
                else if (CheckCollisionPointRec(farePozisyonu, uyeButon)) {
                    memset(girisIsim, 0, sizeof(girisIsim)); memset(girisSifre, 0, sizeof(girisSifre));
                    harfSayisiGirisIsim = 0; harfSayisiGirisSifre = 0; aktifGirisKutusu = 0; girisHatasi = 0;
                    girisYapan = UYE;
                    mevcutDurum = EKRAN_GIRIS;
                    bulunanKitapSayisi = 0;
                    memset(aramaGirdisi, 0, sizeof(aramaGirdisi));
                    aramaHarfSayisi = 0;
                    uyeDurumu=0;
                }
            }
        }
        else if (mevcutDurum == EKRAN_GIRIS) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(farePozisyonu, geriButon)){
                mevcutDurum = EKRAN_KULLANICI_GIRISI;
            }
            if (aktifGirisKutusu == 0) {
                metinGirisiAl(girisIsim, &harfSayisiGirisIsim, 49, 0);
            } else if (aktifGirisKutusu == 1) {
                metinGirisiAl(girisSifre, &harfSayisiGirisSifre, 19, 1);
            }
            if (IsKeyPressed(KEY_ENTER)) {
                if (aktifGirisKutusu == 0) {
                    aktifGirisKutusu = 1;
                } else if (aktifGirisKutusu == 1) {
                    int sifreInt = atoi(girisSifre);
                    const char* dosyaAdi = (girisYapan == ADMIN) ? "ADMIN.txt" : "uye.txt";
                    if (hesapIslemi(dosyaAdi, girisIsim, sifreInt) == 1) {
                        mevcutDurum = EKRAN_KISI;
                        strcpy(aktifKullanici, girisIsim);
                        adminDurumu = 0; 
                        uyeDurumu = 0;
                    } else {
                        girisHatasi = 1;
                        memset(girisSifre, 0, sizeof(girisSifre));
                        harfSayisiGirisSifre = 0;
                        aktifGirisKutusu=0;
                    }
                }
            }
        }
        else if (mevcutDurum == EKRAN_KISI) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(farePozisyonu, menuButon)){
                mevcutDurum = EKRAN_KULLANICI_GIRISI;}
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(farePozisyonu, geriButon)){
                memset(girdiIsim, 0, sizeof(girdiIsim));memset(girdiYil, 0, sizeof(girdiYil));memset(girdiYazar, 0, sizeof(girdiYazar));
                if((girisYapan == ADMIN && adminDurumu == 1)||(girisYapan == ADMIN && adminDurumu == 2)) {
                    adminDurumu = 0;
                } else if((girisYapan == UYE && uyeDurumu == 1)) {
                    uyeDurumu=0;
                }else
                    mevcutDurum=EKRAN_KULLANICI_GIRISI;
            }
            if (girisYapan==ADMIN){
                if(adminDurumu==0){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(farePozisyonu, ekleButon)) {
                        adminDurumu = 1; aktifKutu = 0;
                        harfSayisiIsim = 0; harfSayisiYil = 0; harfSayisiYazar = 0;
                    }
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(farePozisyonu, araButon)) {
                            adminDurumu = 2,uyeDurumu=1;
                            bulunanKitapSayisi = 0;
                            memset(aramaGirdisi, 0, sizeof(aramaGirdisi));
                            aramaHarfSayisi = 0;
                        }
                }
                else if (adminDurumu==1) {
                    if (aktifKutu < 3) {
                        if (aktifKutu == 0 && harfSayisiIsim < 49) metinGirisiAl(girdiIsim, &harfSayisiIsim,49,0);
                        else if (aktifKutu == 1 && harfSayisiYil < 9) metinGirisiAl(girdiYil, &harfSayisiYil,9,1);
                        else if (aktifKutu == 2 && harfSayisiYazar < 49) metinGirisiAl(girdiYazar, &harfSayisiYazar,49,0);
                        if (IsKeyPressed(KEY_ENTER)){
                            aktifKutu++;
                            if (aktifKutu == 3) {
                                strcpy(kitaplar[0].isim, girdiIsim);
                                kitaplar[0].yil = atoi(girdiYil);
                                strcpy(kitaplar[0].yazar, girdiYazar);
                                kitaplar[0].musaitlik = 1;
                                dosyayaYaz(kitaplar[0]);
                            }
                        }
                    }else {
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(farePozisyonu, ekleButon)) {
                            aktifKutu = 0;
                            memset(girdiIsim, 0, sizeof(girdiIsim));memset(girdiYil, 0, sizeof(girdiYil));memset(girdiYazar, 0, sizeof(girdiYazar));
                            harfSayisiIsim = 0; harfSayisiYil = 0; harfSayisiYazar = 0;
                        }
                    }
                }
            }
            if ((girisYapan == UYE && uyeDurumu==0) || (girisYapan == ADMIN && adminDurumu == 2)) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(farePozisyonu, isimAra)) aramaKriteri = 0;
                    if (CheckCollisionPointRec(farePozisyonu, yilAra)) aramaKriteri = 1;
                    if (CheckCollisionPointRec(farePozisyonu, yazarAra)) aramaKriteri = 2;
                    if (girisYapan == UYE) {
                        for (int i = 0; i < bulunanKitapSayisi; i++) {
                            if (i > 10) break;
                            Rectangle satirButonu = { 950, 400 + (i * 25) - 2, 85, 20 };
                            if (CheckCollisionPointRec(farePozisyonu, satirButonu)) {
                                if (aramaSonuclari[i].musaitlik == 1) { 
                                    int sonuc = kirala(aramaSonuclari[i].isim, aktifKullanici, 0);
                                    kiralamaMesajDurumu = (sonuc == 1) ? 1 : 0;
                                } else { 
                                    int sonuc = kirala(aramaSonuclari[i].isim, aktifKullanici, 1);
                                    if (sonuc == 1) kiralamaMesajDurumu = 2;
                                    else if (sonuc == -1) kiralamaMesajDurumu = -1; 
                                    else kiralamaMesajDurumu = 0;
                                }
                                kitapAra(aramaKriteri, aramaGirdisi); 
                                break; 
                            }
                        }
                    }
                }
                int sadeceSayiMi = (aramaKriteri == 1) ? 1 : 0;
                metinGirisiAl(aramaGirdisi, &aramaHarfSayisi, 49, sadeceSayiMi);
                if (IsKeyPressed(KEY_ENTER)) {
                    kitapAra(aramaKriteri, aramaGirdisi);
                }
            }
        }  
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (mevcutDurum == EKRAN_KULLANICI_GIRISI) {
            DrawText("KUTUPHANE SISTEMINE HOSGELDINIZ", 250, 100, 35, DARKBLUE);
            DrawRectangleRec(adminButon, LIGHTGRAY);
            DrawRectangleLinesEx(adminButon, 2, DARKGRAY);
            DrawText("ADMIN Girisi", adminButon.x + 35, adminButon.y + 15, 25, BLACK);
            DrawRectangleRec(uyeButon, LIGHTGRAY);
            DrawRectangleLinesEx(uyeButon, 2, DARKGRAY);
            DrawText("UYE Girisi", uyeButon.x + 45, uyeButon.y + 15, 25, BLACK);
        }else if (mevcutDurum == EKRAN_GIRIS) {
            DrawRectangleRec(geriButon, LIGHTGRAY);
            DrawText("GERI", geriButon.x + 35, geriButon.y + 15, 25, BLACK);
            const char* baslik = (girisYapan == ADMIN) ? "ADMIN GIRISI" : "UYE GIRISI / KAYDI";
            DrawText(baslik, 400, 150, 30, DARKBLUE);
            DrawText("Kullanici Adi:", 250, 250, 25, DARKGRAY);
            DrawRectangle(450, 240, 300, 45, (aktifGirisKutusu == 0) ? LIGHTGRAY : RAYWHITE);
            DrawRectangleLines(450, 240, 300, 45, DARKGRAY);
            DrawText(girisIsim, 460, 250, 25, MAROON);
            DrawText("Sifre (Sayi):", 250, 320, 25, DARKGRAY);
            DrawRectangle(450, 310, 300, 45, (aktifGirisKutusu == 1) ? LIGHTGRAY : RAYWHITE);
            DrawRectangleLines(450, 310, 300, 45, DARKGRAY);
            DrawText(girisSifre, 460, 320, 25, MAROON); 
            DrawText("Sonraki kutu / Giris icin ENTER'a basin", 350, 400, 20, GRAY);
            if (girisHatasi == 1&&girisYapan==ADMIN) {
                DrawText(" Sifre yanlis veya Admin bulunamadi!", 350, 450, 25, RED);
            }else if(girisHatasi == 1&&girisYapan==UYE)
                DrawText(" Sifre yanlis veya UYE bulunamadi!", 350, 450, 25, RED);
        }else if(mevcutDurum == EKRAN_KISI) {
            DrawRectangleRec(menuButon, LIGHTGRAY);
            DrawText("MENU", menuButon.x + 35, menuButon.y + 15, 25, BLACK);
            DrawRectangleRec(geriButon, LIGHTGRAY);
            DrawText("GERI", geriButon.x + 35, geriButon.y + 15, 25, BLACK);
            DrawRectangleRec(araButon,DARKGRAY);
            DrawText("Kitap Ara", araButon.x + 35, araButon.y + 15, 25, BLACK);
            if(girisYapan == ADMIN){
                if(adminDurumu==0){
                    DrawRectangleRec(ekleButon,DARKGRAY);
                    DrawText("Kitap Ekle", ekleButon.x + 35, ekleButon.y + 15, 25, BLACK);
                }else if(adminDurumu==1){
                    if (aktifKutu < 3) {
                        DrawText("Yeni Kitap Ekle (Sonraki alana gecmek icin ENTER'a basin)", 50, 250, 30, DARKGRAY);
                        DrawText("Kitap Ismi:", 50, 350, 25, DARKGRAY);//kutu1
                        DrawRectangle(250, 340, 500, 45, (aktifKutu == 0) ? LIGHTGRAY : RAYWHITE);
                        DrawRectangleLines(250, 340, 500, 45, DARKGRAY);
                        DrawText(girdiIsim, 260, 350, 25, MAROON);
                        DrawText("Basim Yili:", 50, 420, 25, DARKGRAY);//kutu2
                        DrawRectangle(250, 410, 500, 45, (aktifKutu == 1) ? LIGHTGRAY : RAYWHITE);
                        DrawRectangleLines(250, 410, 500, 45, DARKGRAY);
                        DrawText(girdiYil, 260, 420, 25, MAROON);
                        DrawText("Yazar Ismi:", 50, 490, 25, DARKGRAY);//kutu3
                        DrawRectangle(250, 480, 500, 45, (aktifKutu == 2) ? LIGHTGRAY : RAYWHITE);
                        DrawRectangleLines(250, 480, 500, 45, DARKGRAY);
                        DrawText(girdiYazar, 260, 490, 25, MAROON);
                        if (aktifKutu == 0) DrawText("-> Isim yaziliyor...", 780, 350, 20, GRAY);
                        if (aktifKutu == 1) DrawText("-> Sadece sayi giriniz...", 780, 420, 20, GRAY);
                        if (aktifKutu == 2) DrawText("-> Yazar yaziliyor...", 780, 490, 20, GRAY);
                    }else{
                        DrawText("Kitap sisteme eklendi.", 400, 150, 35, DARKGREEN);
                    }   
                }
            }
            if (girisYapan == UYE || (girisYapan == ADMIN && adminDurumu == 2)) {
                DrawText("KITAP ARAMA SISTEMI", 50, 180, 25, DARKBLUE);
                DrawRectangleRec(isimAra, (aramaKriteri == 0) ? SKYBLUE : LIGHTGRAY); DrawRectangleLinesEx(isimAra, 2, DARKGRAY); DrawText("Isme Gore", isimAra.x + 20, isimAra.y + 10, 20, BLACK);
                DrawRectangleRec(yilAra, (aramaKriteri == 1) ? SKYBLUE : LIGHTGRAY); DrawRectangleLinesEx(yilAra, 2, DARKGRAY); DrawText("Yila Gore", yilAra.x + 25, yilAra.y + 10, 20, BLACK);
                DrawRectangleRec(yazarAra, (aramaKriteri == 2) ? SKYBLUE : LIGHTGRAY); DrawRectangleLinesEx(yazarAra, 2, DARKGRAY); DrawText("Yazara Gore", yazarAra.x + 15, yazarAra.y + 10, 20, BLACK);
                DrawRectangleRec(aramaKutusu, LIGHTGRAY); DrawRectangleLinesEx(aramaKutusu, 2, DARKGRAY);
                DrawText(aramaGirdisi, aramaKutusu.x + 10, aramaKutusu.y + 10, 25, MAROON);
                DrawText("Aramak icin metni yazip ENTER'a basin...", 560, 290, 20, GRAY);
                DrawText("BULUNAN KITAPLAR:", 50, 350, 25, DARKGREEN);
                DrawLine(50, 380, 1100, 380, DARKGRAY);
                for (int i = 0; i < bulunanKitapSayisi; i++) {
                    if (i > 10) break;
                    char sonucMetni[150];
                    const char* durumMetni = (aramaSonuclari[i].musaitlik == 1) ? "MUSAIT" : "KIRALANDI";
                    sprintf(sonucMetni, "Kitap: %-30s | Yazar: %-20s | Yil: %-20d | DURUM: %-20s", aramaSonuclari[i].isim, aramaSonuclari[i].yazar, aramaSonuclari[i].yil,durumMetni);
                    Color metinRengi = (aramaSonuclari[i].musaitlik == 1) ? DARKGREEN : MAROON;
                    DrawText(sonucMetni, 50, 400 + (i * 25), 20, metinRengi);
                }
                if (bulunanKitapSayisi == 0 && aramaHarfSayisi > 0) {
                    DrawText("Sonuc bulunamadi veya aranmadi...", 50, 400, 20, DARKGRAY);
                }
            }
            if (girisYapan == UYE) {
                if (kiralamaMesajDurumu == 1) DrawText("Kitap basariyla kiralandi!", 560, 350, 20, DARKGREEN);
                else if (kiralamaMesajDurumu == 2) DrawText("Kitap basariyla iade edildi!", 560, 350, 20, BLUE);
                else if (kiralamaMesajDurumu == -1) DrawText("HATA: Bu kitabi siz kiralamadiniz!", 560, 350, 20, MAROON); // YENİ
                else if (kiralamaMesajDurumu == 0) DrawText("Islem basarisiz!", 560, 350, 20, MAROON);
                }
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
