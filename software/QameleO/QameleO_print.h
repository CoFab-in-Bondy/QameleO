#ifndef QAMELEO_PRINT_H
#define QAMELEO_PRINT_H

// 1) ERREURS critiques (toujours actifs, à garder en fonctionnement)
#define ERROR(X)                      Serial.print("[ERROR] "); Serial.print(X)
#define ERRORLN(X)                    Serial.print("[ERROR] "); Serial.println(X)

// 2) DÉBOGAGE (messages de dev uniquement, à commenter plus tard)
#define DBG(X)                        Serial.print(X)
#define DBGLN(X)                      Serial.println(X)

// 3) LOG / info utilisateur (messages finaux, toujours visibles)
#define LOG(X)                        Serial.print(X)
#define LOGLN(X)                      Serial.println(X)

#endif // QAMELEO_PRINT_H