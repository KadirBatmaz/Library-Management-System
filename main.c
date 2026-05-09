#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <raylib.h>

#define Genislik_BOYUTU 1150
#define Yükseklik_BOYUTU 700
typedef enum { EKRAN_KULLANICI_GIRISI, EKRAN_KISI } KullaniciDurumu;
struct kitap {
    char isim[50];
    int yil;
    char yazar[50];
} kitaplar[100];

void dosyayaYaz(struct kitap eklenecekKitap) {
    FILE *dosya = fopen("kutuphane.txt", "a");
    if (dosya == NULL) {
        printf("Hata: Dosya acilamadi!\n");
        return; 
    }
    fprintf(dosya, "%s,%d,%s\n", eklenecekKitap.isim, eklenecekKitap.yil, eklenecekKitap.yazar);
    fclose(dosya);
}

int main() {
    InitWindow(Genislik_BOYUTU, Yükseklik_BOYUTU, "Kutuphane Yonetim Sistemi");
    SetTargetFPS(60); 
    int aktifKutu = 0; 
    char girdiIsim[50] = "\0",girdiYazar[50] = "\0",girdiYil[10] = "\0";
    int harfSayisiIsim = 0,harfSayisiYil = 0,harfSayisiYazar = 0;
    while (!WindowShouldClose()) {
        if (aktifKutu < 3) {            
            int tus = GetCharPressed();
            while (tus > 0) {
                if ((tus >= 32) && (tus <= 125)) {
                    if (aktifKutu == 0 && harfSayisiIsim < 49) {
                        girdiIsim[harfSayisiIsim] = (char)tus;
                        girdiIsim[harfSayisiIsim + 1] = '\0'; // Metnin sonunu kapat
                        harfSayisiIsim++;
                    }
                    else if (aktifKutu == 1 && harfSayisiYil < 9) {
                        if (tus >= '0' && tus <= '9') { // Yıl için SADECE rakamlara izin ver
                            girdiYil[harfSayisiYil] = (char)tus;
                            girdiYil[harfSayisiYil + 1] = '\0';
                            harfSayisiYil++;
                        }
                    }
                    else if (aktifKutu == 2 && harfSayisiYazar < 49) {
                        girdiYazar[harfSayisiYazar] = (char)tus;
                        girdiYazar[harfSayisiYazar + 1] = '\0';
                        harfSayisiYazar++;
                    }
                }
                tus = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (aktifKutu == 0 && harfSayisiIsim > 0) {
                    harfSayisiIsim--;
                    girdiIsim[harfSayisiIsim] = '\0';
                }
                else if (aktifKutu == 1 && harfSayisiYil > 0) {
                    harfSayisiYil--;
                    girdiYil[harfSayisiYil] = '\0';
                }
                else if (aktifKutu == 2 && harfSayisiYazar > 0) {
                    harfSayisiYazar--;
                    girdiYazar[harfSayisiYazar] = '\0';
                }
            }
            if (IsKeyPressed(KEY_ENTER)) {
                aktifKutu++;
                if (aktifKutu == 3) {
                    strcpy(kitaplar[0].isim, girdiIsim);
                    kitaplar[0].yil = atoi(girdiYil);
                    strcpy(kitaplar[0].yazar, girdiYazar);
                    dosyayaYaz(kitaplar[0]);
                }
            }
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (aktifKutu < 3) {
            DrawText("Yeni Kitap Ekle (Sonraki alana gecmek icin ENTER'a basin)", 50, 50, 30, DARKGRAY);
            DrawText("Kitap Ismi:", 50, 150, 25, DARKGRAY);//kutu1
            DrawRectangle(250, 140, 500, 45, (aktifKutu == 0) ? LIGHTGRAY : RAYWHITE);
            DrawRectangleLines(250, 140, 500, 45, DARKGRAY);
            DrawText(girdiIsim, 260, 150, 25, MAROON);
            DrawText("Basim Yili:", 50, 220, 25, DARKGRAY);//kutu2
            DrawRectangle(250, 210, 500, 45, (aktifKutu == 1) ? LIGHTGRAY : RAYWHITE);
            DrawRectangleLines(250, 210, 500, 45, DARKGRAY);
            DrawText(girdiYil, 260, 220, 25, MAROON);
            DrawText("Yazar Ismi:", 50, 290, 25, DARKGRAY);//kutu3
            DrawRectangle(250, 280, 500, 45, (aktifKutu == 2) ? LIGHTGRAY : RAYWHITE);
            DrawRectangleLines(250, 280, 500, 45, DARKGRAY);
            DrawText(girdiYazar, 260, 290, 25, MAROON);
            if (aktifKutu == 0) DrawText("-> Isim yaziliyor...", 780, 150, 20, GRAY);
            if (aktifKutu == 1) DrawText("-> Sadece sayi giriniz...", 780, 220, 20, GRAY);
            if (aktifKutu == 2) DrawText("-> Yazar yaziliyor...", 780, 290, 20, GRAY);
        } else {
            DrawText("Kitap sisteme eklendi.", 50, 100, 35, DARKGREEN);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}