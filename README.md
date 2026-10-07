# Keylogger PoC — Hacking Ético

> Prueba de concepto académica para el curso de **Hacking Ético**.  
> Universidad Peruana de Ciencias Aplicadas — UPC.

## Descripción

Este proyecto presenta una prueba de concepto de un **keylogger desarrollado en C++ para Windows**, creada exclusivamente con fines educativos y de análisis de ciberseguridad dentro de un entorno controlado.

El objetivo del proyecto es comprender cómo un software puede interactuar con los mecanismos de entrada de Windows, qué evidencias puede generar este comportamiento y cómo puede analizarse desde una perspectiva defensiva.

El proyecto forma parte de un Trabajo Parcial del curso de Hacking Ético y no está diseñado para su utilización sobre sistemas, cuentas o dispositivos de terceros.

---

## Objetivos académicos

El proyecto busca:

- comprender el concepto de keylogging;
- analizar mecanismos de captura de entrada disponibles en Windows;
- estudiar el uso de APIs del sistema operativo;
- observar artefactos generados durante la ejecución;
- relacionar el comportamiento observado con MITRE ATT&CK;
- analizar posibles mecanismos de detección;
- reforzar buenas prácticas de seguridad frente a software potencialmente malicioso.

---

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
