#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <raylib.h>

#define Genislik_BOYUTU 1150
#define Yükseklik_BOYUTU 700
typedef enum { EKRAN_KULLANICI_GIRISI, EKRAN_KISI } EkranDurumu;
typedef enum { ADMIN,UYE } UyeDurumu;
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

int main() {
    EkranDurumu mevcutDurum = EKRAN_KULLANICI_GIRISI;
    UyeDurumu girisYapan = UYE;
    InitWindow(Genislik_BOYUTU, Yükseklik_BOYUTU, "Kutuphane Yonetim Sistemi");
    SetTargetFPS(60); 
    int aktifKutu=0,adminDurumu=0; 
    char girdiIsim[50] = "\0",girdiYazar[50] = "\0",girdiYil[10] = "\0";
    int harfSayisiIsim = 0,harfSayisiYil = 0,harfSayisiYazar = 0;
    Rectangle adminButon = { 400, 250, 350, 60 };
    Rectangle uyeButon = { 400, 350, 350, 60 };
    Rectangle ekleButon = { 50, 190, 350, 60 };
    Rectangle menuButon = { 50, 50, 150, 60 };
    Rectangle geriButon = { 50, 120, 150, 60 };
    

    while (!WindowShouldClose()) {
        Vector2 farePozisyonu = GetMousePosition();
        if (mevcutDurum == EKRAN_KULLANICI_GIRISI) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(farePozisyonu, adminButon)) {
                    girisYapan = ADMIN;
                    mevcutDurum = EKRAN_KISI;
                    adminDurumu=0;
                }
                else if (CheckCollisionPointRec(farePozisyonu, uyeButon)) {
                    girisYapan = UYE;
                    mevcutDurum = EKRAN_KISI;
                }
            }
        } 
        else if (mevcutDurum == EKRAN_KISI) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(farePozisyonu, menuButon)){
                mevcutDurum = EKRAN_KULLANICI_GIRISI;}
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(farePozisyonu, geriButon)){
                if(girisYapan == ADMIN && adminDurumu == 1) {
                    adminDurumu = 0;
                } else {
                    mevcutDurum = EKRAN_KULLANICI_GIRISI;
                }
            }
            if (girisYapan==ADMIN){
                if(adminDurumu==0){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(farePozisyonu, ekleButon)) {
                        adminDurumu = 1;
                        aktifKutu = 0;
                        memset(girdiIsim, 0, sizeof(girdiIsim));
                        memset(girdiYil, 0, sizeof(girdiYil));
                        memset(girdiYazar, 0, sizeof(girdiYazar));
                        harfSayisiIsim = 0; harfSayisiYil = 0; harfSayisiYazar = 0;
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
                                dosyayaYaz(kitaplar[0]);
                            }
                        }
                    }
                    else {
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(farePozisyonu, ekleButon)) {
                            aktifKutu = 0;
                            memset(girdiIsim, 0, sizeof(girdiIsim));
                            memset(girdiYil, 0, sizeof(girdiYil));
                            memset(girdiYazar, 0, sizeof(girdiYazar));
                            harfSayisiIsim = 0; harfSayisiYil = 0; harfSayisiYazar = 0;
                        }
                    }
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
        }else if(mevcutDurum == EKRAN_KISI) {
            DrawRectangleRec(menuButon, LIGHTGRAY);
            DrawText("MENU", menuButon.x + 35, menuButon.y + 15, 25, BLACK);
            DrawRectangleRec(geriButon, LIGHTGRAY);
            DrawText("GERI", geriButon.x + 35, geriButon.y + 15, 25, BLACK);
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
                        DrawText("tekrar eklememk icin TAB", 400, 200, 35, DARKGREEN);
                    }   
                }
            }
            else if(girisYapan==UYE){
                DrawText("YAKINDA...", 400, 200, 35, DARKGREEN);
            }
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}