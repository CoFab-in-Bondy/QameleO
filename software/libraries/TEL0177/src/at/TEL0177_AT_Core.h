#ifndef TEL0177_AT_CORE_H
#define TEL0177_AT_CORE_H

#include <Arduino.h>
#include <Stream.h>

#include "../TEL0177_Config.h"
#include "../TEL0177_Types.h"


/**
 * @file TEL0177_AT_Core.h
 * @brief Couche bas niveau de communication AT avec le module TEL0177.
 *
 * Cette classe gère la série, l'envoi de commandes AT, l'attente de réponses
 * et quelques utilitaires de base.
 */
class TEL0177_AT_Core {
private:
    Stream*  _serial   = nullptr;   ///< Port série associé au modem.
    char     _rxBuffer[TEL_RX_BUFFER_SIZE] = {0};  ///< Buffer de réception.

    /**
     * @brief Attend une réponse "OK" ou "ERROR" du modem.
     * @param timeout Délai maximal d'attente.
     * @return TEL_OK, TEL_ERROR ou TEL_TIMEOUT.
     */
    TEL_Status _waitForOKorERROR(uint32_t timeout);

    /**
     * @brief Vérifie si une sous‑chaîne est présente dans la chaîne donnée.
     * @param haystack Chaîne principale.
     * @param needle   Sous‑chaîne recherchée.
     * @return true si trouvée, false sinon.
     */
    bool _contains(const char* haystack, const char* needle);

    /**
     * @brief Vérifie si la réponse contient une erreur.
     * @param response Chaîne à analyser.
     * @return true si une erreur est détectée.
     */
    bool _hasError(const char* response);

public:
    // ==========================================================================
    // INITIALISATION
    // ==========================================================================

    /**
     * @brief Associe le port série au cœur AT.
     * @param serial Flux série déjà initialisé.
     */
    void begin(Stream& serial);


    // ==========================================================================
    // ENVOI
    // ==========================================================================

    /**
     * @brief Envoie une commande AT puis attend "OK" ou "ERROR".
     * @param command Commande à envoyer.
     * @param timeout Délai maximal d'attente.
     * @return TEL_Status.
     */
    TEL_Status send(const char* command, uint32_t timeout = TEL_TIMEOUT_DEFAULT);

    /**
     * @brief Envoie une commande AT puis attend une sous‑chaîne précise.
     * @param command  Commande à envoyer.
     * @param expected Sous‑chaîne recherchée.
     * @param timeout  Délai maximal d'attente.
     * @return TEL_OK si trouvée, TEL_TIMEOUT sinon.
     */
    TEL_Status sendAndWait(const char* command, const char* expected, uint32_t timeout = TEL_TIMEOUT_DEFAULT);

    /**
     * @brief Envoie une commande AT et récupère la réponse complète.
     * @param command     Commande à envoyer.
     * @param buffer      Buffer de destination.
     * @param bufferSize  Taille du buffer.
     * @param timeout     Délai maximal d'attente.
     * @return TEL_Status.
     */
    TEL_Status sendAndRead(const char* command, char* buffer, uint16_t bufferSize, uint32_t timeout = TEL_TIMEOUT_DEFAULT);

    /**
     * @brief Envoie une commande AT et récupère toute la réponse (pas de parsing).
     * @param command Commande à envoyer.
     * @param buffer  Buffer de destination.
     * @param size    Taille du buffer.
     * @param timeout Délai maximal d'attente.
     * @return TEL_Status.
     */
    TEL_Status sendAndReadRaw(const char* command, char* buffer, uint16_t size, uint32_t timeout = TEL_TIMEOUT_DEFAULT);

    /**
     * @brief Envoie une commande AT, récupère la réponse et vérifie OK/ERROR.
     * @param command Commande à envoyer.
     * @param buffer  Buffer de destination.
     * @param size    Taille du buffer.
     * @param timeout Délai maximal d'attente.
     * @return TEL_Status.
     */
    TEL_Status sendAndReadChecked(const char* command, char* buffer, uint16_t size, uint32_t timeout = TEL_TIMEOUT_DEFAULT);

    /**
     * @brief Envoie des données brutes sans CRLF.
     * @param data   Données à envoyer.
     * @param length Longueur (0 = strlen).
     * @return TEL_Status.
     */
    TEL_Status sendRaw(const char* data, uint16_t length = 0);


    // ==========================================================================
    // LECTURE
    // ==========================================================================

    /**
     * @brief Attend qu'une sous‑chaîne apparaisse dans le flux.
     * @param expected Sous‑chaîne recherchée.
     * @param timeout  Délai maximal d'attente.
     * @return true si trouvé, false sinon.
     */
    bool waitFor(const char* expected, uint32_t timeout = TEL_TIMEOUT_DEFAULT);

    /**
     * @brief Lit une ligne terminée par '\n'.
     * @param buffer Buffer de destination.
     * @param size   Taille du buffer.
     * @param timeout Délai maximal d'attente.
     * @return true si une ligne valide est lue.
     */
    bool readLine(char* buffer, uint16_t size, uint32_t timeout = TEL_TIMEOUT_DEFAULT);

    /**
     * @brief Lit tous les octets disponibles dans la limite du temps.
     * @param buffer Buffer de destination.
     * @param size   Taille du buffer.
     * @param timeout Délai maximal d'attente.
     * @return Nombre d'octets copiés.
     */
    uint16_t readAll(char* buffer, uint16_t size, uint32_t timeout = TEL_TIMEOUT_DEFAULT);


    // ==========================================================================
    // UTILITAIRES
    // ==========================================================================

    /**
     * @brief Vide le flux série entrant et le buffer interne.
     */
    void clearInput();

    /**
     * @brief Indique si des données sont disponibles.
     * @return true si disponibles, false sinon.
     */
    bool isAvailable();

    /**
     * @brief Vérifie que le modem répond à "AT".
     * @return true si OK, false sinon.
     */
    bool isAlive();

    /**
     * @brief Retourne la dernière réponse du modem.
     * @return Chaîne de la dernière réponse.
     */
    const char* lastResponse() const;

    /**
     * @brief Retourne la taille de la dernière réponse.
     * @return Longueur de la dernière réponse.
     */
    uint16_t lastResponseLength() const;

    /**
     * @brief Indique si le cœur AT est prêt (serial associé).
     * @return true si prêt, false sinon.
     */
    bool isReady() const;
};

#endif // TEL0177_AT_CORE_H