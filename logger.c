#include <stdio.h>
#include <time.h>
#include "logger.h"

void escribirLog(const char *tipo, const char *mensaje) {
    FILE *f = fopen("log.txt", "a");
    if (!f) return;

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    char fecha[26];
    strftime(fecha, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(f, "[%s] [%s] %s\n", fecha, tipo, mensaje);
    fclose(f);
}

void logInfo(const char *mensaje) {
    escribirLog("INFO", mensaje);
}

void logError(const char *mensaje) {
    escribirLog("ERROR", mensaje);
}

void logAccion(const char *usuario, const char *accion) {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Usuario '%s': %s", usuario, accion);
    escribirLog("ACCION", buffer);
}
