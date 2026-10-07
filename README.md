# Keylogger PoC — Hacking Ético

> Prueba de concepto académica para el curso de **Hacking Ético**.  
> Universidad Peruana de Ciencias Aplicadas — UPC.

**Repositorio:** [https://github.com/adletgamer/HACKING-TP-PARCIAL-KEYLOGGER](https://github.com/adletgamer/HACKING-TP-PARCIAL-KEYLOGGER)

---

## Descripción

Este proyecto presenta una prueba de concepto de un **keylogger desarrollado en C++ para Windows**, creada exclusivamente con fines educativos y de análisis de ciberseguridad dentro de un entorno controlado.

El objetivo del proyecto es comprender cómo un software puede interactuar con los mecanismos de entrada de Windows, qué evidencias puede generar este comportamiento y cómo puede analizarse desde una perspectiva defensiva.

El proyecto forma parte de un Trabajo Parcial del curso de Hacking Ético y no está diseñado para su utilización sobre sistemas, cuentas o dispositivos de terceros.

---

## Objetivos académicos

El proyecto busca:

- Comprender el concepto de keylogging.
- Analizar mecanismos de captura de entrada disponibles en Windows.
- Estudiar el uso de APIs del sistema operativo.
- Observar artefactos generados durante la ejecución.
- Relacionar el comportamiento observado con MITRE ATT&CK.
- Analizar posibles mecanismos de detección.
- Reforzar buenas prácticas de seguridad frente a software potencialmente malicioso.

---
# Tecnologías utilizadas

| Componente | Descripción |
|---|---|
| Lenguaje | C++17 |
| Compilador | g++ (MSYS2 UCRT64) |
| Librería HTTP | libcurl (estática) |
| API del sistema | Windows API (GetAsyncKeyState, RegOpenKeyExA, GetModuleFileNameA, CreateDirectory, etc.) |
| Entorno de compilación | Visual Studio Code + tasks.json |
| Salida de datos | Archivos .txt en C:\ProgramData\JavaNET\ |
| Canal de exfiltración (simulado) | Telegram Bot API |

---

# Estructura del proyecto

```text
HACKING-TP-PARCIAL-KEYLOGGER/
│
├── keyyZ.cpp                  # Código fuente principal de la PoC
├── curl_key.cpp               # Prueba de funcionamiento de libcurl
├── tasks.json                 # Configuración de compilación para VS Code
├── c_cpp_properties.json      # Configuración de IntelliSense
├── Key_code.txt               # Copia de referencia del código
└── README.md                  # Este archivo
## Arquitectura conceptual

El comportamiento estudiado puede representarse de la siguiente manera:

```text
Entrada de teclado
        │
        ▼
Windows Input API
        │
        ▼
Captura de eventos
        │
        ▼
Procesamiento
        │
        ▼
Registro temporal
        │
        ▼
Análisis del comportamiento
