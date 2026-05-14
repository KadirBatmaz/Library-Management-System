if (girisYapan == UYE || (girisYapan == ADMIN && adminDurumu == 2)) {
                DrawText("KITAP ARAMA SISTEMI", 50, 180, 25, DARKBLUE);
                DrawRectangleRec(isimAra, (aramaKriteri == 0) ? SKYBLUE : LIGHTGRAY); DrawRectangleLinesEx(isimAra, 2, DARKGRAY); DrawText("Isme Gore", isimAra.x + 20, isimAra.y + 10, 20, BLACK);
                DrawRectangleRec(yilAra, (aramaKriteri == 1) ? SKYBLUE : LIGHTGRAY); DrawRectangleLinesEx(yilAra, 2, DARKGRAY); DrawText("Yila Gore", yilAra.x + 25, yilAra.y + 10, 20, BLACK);
                DrawRectangleRec(yazarAra, (aramaKriteri == 2) ? SKYBLUE : LIGHTGRAY); DrawRectangleLinesEx(yazarAra, 2, DARKGRAY); DrawText("Yazara Gore", yazarAra.x + 15, yazarAra.y + 10, 20, BLACK);
                DrawRectangleRec(aramaKutusu, LIGHTGRAY); DrawRectangleLinesEx(aramaKutusu, 2, DARKGRAY);
                DrawText(aramaGirdisi, aramaKutusu.x + 10, aramaKutusu.y + 10, 25, MAROON);
                DrawText("Aramak icin metni yazip ENTER'a basin...", 560, 290, 20, GRAY);
                DrawText("BULUNAN KITAPLAR:", 50, 350, 25, DARKGREEN);
                DrawLine(50, 380, 1100, 360, DARKGRAY);
                for (int i = 0; i < bulunanKitapSayisi; i++) {
                    if (i > 10) break;
                    char sonucMetni[150];
                    sprintf(sonucMetni, "Kitap: %-30s | Yazar: %-25s | Yil: %d", aramaSonuclari[i].isim, aramaSonuclari[i].yazar, aramaSonuclari[i].yil);
                    DrawText(sonucMetni, 50,400 + (i * 25), 20, BLACK);
                }
                if (bulunanKitapSayisi == 0 && aramaHarfSayisi > 0) {
                    DrawText("Sonuc bulunamadi veya aranmadi...", 50, 400, 20, DARKGRAY);
                }
            }
