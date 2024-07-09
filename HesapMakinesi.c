#include <windows.h>
#include <stdio.h>
#include <math.h>

HWND g_islemKutusu;

void  SonucDegerlendirme(HWND hwnd);
double Degerlendirme(const char* ifade);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            g_islemKutusu = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
                                           WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_AUTOHSCROLL,
                                           10, 10, 230, 30,
                                           hwnd, (HMENU)1, GetModuleHandle(0), 0);

            if (g_islemKutusu == NULL) {
                MessageBox(NULL, "İşlem sırasında bir hata oluştu!", "Hata!", MB_ICONERROR);
                return -1;
            }

            // Butonları ekleme
            int sayi = 1;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    char butonmetni[2];
                    butonmetni[0] = '0' + sayi;
                    butonmetni[1] = '\0';

                    // Sayı butonları
                    CreateWindowEx(0, "button", butonmetni,
                                   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                   60 + j * 50, 50 + i * 40, 30, 30,
                                   hwnd, (HMENU)(sayi + 1), GetModuleHandle(0), 0);
                    sayi++;
                }
            }

            // Sıfır butonu
            CreateWindowEx(0, "button", "0",
                           WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                           110, 170, 30, 30,
                           hwnd, (HMENU)11, GetModuleHandle(0), 0);

            // Sil butonu
            CreateWindowEx(0, "button", "Sil",
                           WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                           160, 170, 30, 30,
                           hwnd, (HMENU)12, GetModuleHandle(0), 0);

            // İşlem butonları
            const char* islemler[] = { "+", "-", "*", "/" };
            for (int i = 0; i < 4; ++i) {
                CreateWindowEx(0, "button", islemler[i],
                               WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                               10, 50 + i * 40, 30, 30,
                               hwnd, (HMENU)(i + 13), GetModuleHandle(0), 0);
            }

            // Hepsini Sil butonu
            CreateWindowEx(0, "button", "C",
                           WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                           60, 170, 30, 30, // En geniş olan
                           hwnd, (HMENU)20, GetModuleHandle(0), 0);

            // Eşittir butonu
            CreateWindowEx(0, "button", "=",
                           WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                           10, 210, 230, 30,
                           hwnd, (HMENU)17, GetModuleHandle(0), 0);
            // Sayının karesini alma butonu
            CreateWindowEx(0, "button", "^2",
                           WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                           210, 50, 30, 30,
                           hwnd, (HMENU)18, GetModuleHandle(0), 0);
            // Sayının kökünü alma butonu
            CreateWindowEx(0, "button", "v2",
                           WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                           210, 90, 30, 30,
                           hwnd, (HMENU)19, GetModuleHandle(0), 0);
            // Sayının yüzdesini alma butonu
            CreateWindowEx(0, "button", "%",
                           WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                           210, 130, 30, 30,
                           hwnd, (HMENU)21, GetModuleHandle(0), 0);
            // 1/x işlemi butonu
            CreateWindowEx(0, "button", "1/x",
                           WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                           210, 170, 30, 30,
                           hwnd, (HMENU)22, GetModuleHandle(0), 0);


            return 0;
        }
            // Butonların çalışabilir olmasını sağlama
        case WM_COMMAND: {
            if (HIWORD(wParam) == BN_CLICKED) {
                int butonId = LOWORD(wParam);
                char butonmetni[20];
                GetWindowTextA(GetDlgItem(hwnd, butonId), butonmetni, sizeof(butonmetni));

                if (butonId >= 2 && butonId <= 11) {
                    SendMessage(g_islemKutusu, EM_REPLACESEL, TRUE, (LPARAM)butonmetni);
                } else if (butonId >= 13 && butonId <= 16) {
                    // İşlem butonlarının çalıştırabilme
                    SendMessage(g_islemKutusu, EM_REPLACESEL, TRUE, (LPARAM)butonmetni);
                } else if (butonId == 17) {
                    // Eşittir buttonunu çalıştırabilme
                    SonucDegerlendirme(hwnd);
                } else if (butonId == 12) {
                    // Sil butonunu çalıştırabilme
                    int baslat, son;
                    SendMessage(g_islemKutusu, EM_GETSEL, (WPARAM)&baslat, (LPARAM)&son);

                    if (baslat == son) {
                        SendMessage(g_islemKutusu, EM_SETSEL, baslat - 1, son);
                    }

                    SendMessage(g_islemKutusu, EM_REPLACESEL, TRUE, (LPARAM)"");
                } else if (butonId == 20) {
                    // Hepsini Sil butonunu çalıştırabilme
                    SetWindowTextA(g_islemKutusu, "");
                } else if (butonId == 18){
                    // Karesini alma butonunu çalıştırabilme
                    SendMessage(g_islemKutusu, EM_REPLACESEL, TRUE, (LPARAM)"^2");
                } else if (butonId == 19){
                    // Kökünü alma butonunu çalıştırabilme
                    SendMessage(g_islemKutusu, EM_REPLACESEL, TRUE, (LPARAM)"v2");
                } else if (butonId == 21){
                    // Yüzde alma butonunu çalıştırabilme
                    SendMessage(g_islemKutusu, EM_REPLACESEL, TRUE, (LPARAM)"%");
                } else if (butonId == 22) {
                    SendMessage(g_islemKutusu, EM_REPLACESEL, TRUE, (LPARAM)"1/x");
                }
            }
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void SonucDegerlendirme(HWND hwnd) {
    // İfadeyi işlem kutusundan alma
    int uzunluk = GetWindowTextLength(g_islemKutusu);
    char* ifade = (char*)malloc(uzunluk + 1);
    GetWindowTextA(g_islemKutusu, ifade, uzunluk + 1);

    // İfadeyi değerlendirme
    double sonuc = Degerlendirme(ifade);

    // Sonucu işlem kutusunda görüntüleme
    SetWindowTextA(g_islemKutusu, "");
    char sonucmetni[30];
    sprintf(sonucmetni, "%.3f", sonuc);
    SendMessage(g_islemKutusu, EM_REPLACESEL, TRUE, (LPARAM)sonucmetni);

    free(ifade);
}

double Degerlendirme(const char* ifade) {
    double sayi1, sayi2;
    char islem;

    if (sscanf(ifade, "%lf%c%lf", &sayi1, &islem, &sayi2) == 3) {
        switch (islem) {
            case '+':
                return sayi1 + sayi2;
            case '-':
                return sayi1 - sayi2;
            case '*':
                return sayi1 * sayi2;
            case '/':
                if (sayi2 != 0.0) {
                    return sayi1 / sayi2;
                } else {
                    MessageBoxA(NULL, "Sıfıra bölünemez!", "Hata", MB_ICONERROR);
                    return 0.0; // Hata göstergesi olarak 0'ı döndürme
                }
            default:
                MessageBoxA(NULL, "Geçersiz işlem!", "Hata", MB_ICONERROR);
                return 0.0; // Hata göstergesi olarak 0'ı döndürme
            case '^': // karesini alma işlemi
                return pow(sayi1, 2);
            case 'v': // kök alma işlemi
                return sqrt(sayi1);
            case '%': // yüzde alma işlemi
                return sayi1 * sayi2 / 100.0;
        }
    } else if (sscanf(ifade, "%lf", &sayi1) == 1) {
        return 1 / sayi1; // 1/x işlemi
    } else {
        MessageBoxA(NULL, "Geçersiz ifade!", "Hata", MB_ICONERROR);
        return 0.0; // Hata göstergesi olarak 0'ı döndürme
    }
}

int main() {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(0);
    wc.lpszClassName = "Hesap Makinesi";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, "Hesap Makinesi", "Hesap Makinesi", WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, 270, 310,
                               0, 0, GetModuleHandle(0), 0);

    if (hwnd == 0) {
        return 1;
    }

    ShowWindow(hwnd, SW_SHOWNORMAL);

    MSG mesaj = { 0 };
    while (GetMessage(&mesaj, 0, 0, 0)) {
        TranslateMessage(&mesaj);
        DispatchMessage(&mesaj);
    }

    return 0;
}
