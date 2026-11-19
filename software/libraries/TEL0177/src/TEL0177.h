#ifndef TEL0177_H
#define TEL0177_H

#include <Arduino.h>
#include <Stream.h>

#include "TEL0177_Types.h"
#include "TEL0177_Config.h"

#include "at/TEL0177_AT_Core.h"
#include "network/TEL0177_Network.h"
#include "mqtt/TEL0177_MQTT.h"

// #include "http/TEL0177_HTTP.h"
// #include "sms/TEL0177_SMS.h"

/**
 * @file TEL0177.h
 * @brief Façade principale de la librairie TEL0177.
 *
 * Cette classe regroupe les couches AT, Network et MQTT pour fournir
 * une API simple à utiliser dans un projet Arduino.
 *
 * Elle reste générique pour le module TEL0177, tout en permettant ensuite
 * une adaptation facile dans un projet comme QameleO.
 */
class TEL0177 {
private:
    TEL0177_AT_Core _at;       ///< Cœur AT bas niveau.
    TEL0177_Network _network;  ///< Couche réseau SIM / APN / PDP / IP.
    TEL0177_MQTT   _mqtt;      ///< Couche MQTT (connect, publish, subscribe).

    TEL_Status _lastError = TEL_OK;  ///< Dernier code d'erreur.

    /**
     * @brief Enregistre une erreur et retourne true si elle vaut TEL_OK.
     * @param status Code d'état à mémoriser.
     * @return true si status == TEL_OK, false sinon.
     */
    bool _setLastError(TEL_Status status);

    /**
     * @brief Affiche une erreur pour debug.
     * @param expression Nom de l'opération.
     * @param status Code d'erreur.
     */
    void _display(const char* expression, TEL_Status status);

  public:
    // ==========================================================================
    // INITIALISATION / MODEM
    // ==========================================================================

    /**
     * @brief Initialise la librairie avec un port série.
     *
     * Cette fonction initialise les sous-couches AT, Network et MQTT.
     *
     * @param serial Port série utilisé pour communiquer avec le module.
     * @return true si le modem répond, false sinon.
     */
    bool begin(Stream& serial);

    /**
     * @brief Fait tourner les services internes.
     *
     * À appeler régulièrement dans la boucle principale Arduino.
     * Sert notamment au traitement des messages MQTT entrants.
     */
    void loop();

    /**
     * @brief Traite les événements MQTT pendant une durée donnée.
     * @param durationMs Durée en millisecondes.
     */
    void loopFor(uint32_t durationMs);

    /**
     * @brief Redémarre le module par commande AT si supportée.
     * @return true si le reset réussit, false sinon.
     */
    bool reset();

    /**
     * @brief Vérifie si le modem répond à la commande AT.
     * @return true si le modem répond, false sinon.
     */
    bool isAlive();

    /**
     * @brief Retourne la dernière erreur enregistrée.
     * @return Dernier code TEL_Status.
     */
    TEL_Status lastError() const;

    /**
     * @brief Retourne la dernière réponse AT brute connue.
     * @return Chaîne contenant la dernière réponse du modem.
     */
    const char* lastResponse() const;


    // ==========================================================================
    // SIM
    // ==========================================================================

    /**
     * @brief Vérifie si la SIM est prête.
     * @return true si la SIM est prête, false sinon.
     */
    bool isSIMReady();

    /**
     * @brief Déverrouille la SIM avec un code PIN.
     * @param pin Code PIN de la carte SIM.
     * @return true si la SIM est prête ou déverrouillée, false sinon.
     */
    bool simUnlock(const char* pin);


    // ==========================================================================
    // RÉSEAU / DATA
    // ==========================================================================

    /**
     * @brief Vérifie si le modem est enregistré sur le réseau mobile.
     * @return true si enregistré, false sinon.
     */
    bool isNetworkRegistered();

    /**
     * @brief Vérifie l'enregistrement sur le réseau data.
     * @return true si enregistré côté data, false sinon.
     */
    bool isDataNetworkRegistered();

    /**
     * @brief Vérifie l'enregistrement LTE/EPS.
     * @return true si enregistré LTE/EPS, false sinon.
     */
    bool isLTERegistered();

    /**
     * @brief Attend que le réseau soit disponible.
     * @param timeout Délai maximal d'attente.
     * @return true si le réseau devient disponible, false sinon.
     */
    bool waitForNetwork(uint32_t timeout = TEL_TIMEOUT_NETWORK);

    /**
     * @brief Lit la qualité du signal.
     * @param rssi Référence recevant le RSSI.
     * @param ber Référence recevant le BER.
     * @return true si la lecture réussit, false sinon.
     */
    bool getSignalQuality(int& rssi, int& ber);

    /**
     * @brief Vérifie si le modem est attaché au service data.
     * @return true si attaché, false sinon.
     */
    bool isDataAttached();

    /**
     * @brief Attache le modem au service data.
     * @return true si l'attachement réussit, false sinon.
     */
    bool attachData();

    /**
     * @brief Détache le modem du service data.
     * @return true si le détachement réussit, false sinon.
     */
    bool detachData();

    /**
     * @brief Vérifie si le contexte PDP est actif.
     * @return true si la data est active, false sinon.
     */
    bool isDataConnected();

    /**
     * @brief Active le contexte PDP.
     * @return true si le contexte PDP est activé, false sinon.
     */
    bool activateDataContext();

    /**
     * @brief Désactive le contexte PDP.
     * @return true si le contexte PDP est désactivé, false sinon.
     */
    bool deactivateDataContext();

    /**
     * @brief Configure et active la connexion data.
     *
     * Cette fonction prépare la connexion mobile avant MQTT ou HTTP :
     * réseau, APN, authentification, attachement data, activation PDP, IP.
     *
     * @param apn APN opérateur.
     * @param user Nom d'utilisateur APN, optionnel.
     * @param pass Mot de passe APN, optionnel.
     * @param pdpType Type PDP, par exemple "IP" ou "IPV4V6".
     * @return true si la data est prête, false sinon.
     */
    bool networkConnect(
      const char* apn,
      const char* user = nullptr,
      const char* pass = nullptr,
      const char* pdpType = TEL_PDP_TYPE_DEFAULT
    );

    /**
     * @brief Désactive proprement la connexion data.
     * @return true si la déconnexion réussit, false sinon.
     */
    bool networkDisconnect();

    /**
     * @brief Récupère l'adresse IP actuelle.
     * @param buffer Buffer de destination.
     * @param size Taille du buffer.
     * @return true si l'IP est récupérée, false sinon.
     */
    bool getIPAddress(char* buffer, uint16_t size);

    /**
     * @brief Retourne l'état réseau mémorisé.
     * @return État TEL_NetState.
     */
    TEL_NetState networkState() const;

    /**
     * @brief Retourne la dernière IP connue.
     * @return Adresse IP ou chaîne vide.
     */
    const char* ipAddress() const;

    /**
     * @brief Retourne le dernier APN configuré.
     * @return APN ou chaîne vide.
     */
    const char* apn() const;

    /**
     * @brief Retourne le dernier type PDP configuré.
     * @return Type PDP ou chaîne vide.
     */
    const char* pdpType() const;


    // ==========================================================================
    // INFORMATIONS MODULE / SIM / RÉSEAU
    // ==========================================================================

    bool getOperator(char* buffer, uint16_t size);
    bool getIMEI(char* buffer, uint16_t size);
    bool getICCID(char* buffer, uint16_t size);
    bool getIMSI(char* buffer, uint16_t size);
    bool getManufacturer(char* buffer, uint16_t size);
    bool getModel(char* buffer, uint16_t size);
    bool getFirmware(char* buffer, uint16_t size);


    // ==========================================================================
    // MQTT
    // ==========================================================================

    /**
     * @brief Configure l'identité du client MQTT.
     *
     * @param clientId Identifiant client MQTT.
     * @param user Nom d'utilisateur MQTT, optionnel.
     * @param pass Mot de passe MQTT, optionnel.
     * @param keepAlive Keep-alive MQTT en secondes.
     * @return true si la configuration est acceptée.
     */
    bool mqttsetClient(
      const char* clientId,
      const char* user = nullptr,
      const char* pass = nullptr,
      uint16_t keepAlive = TEL_MQTT_KEEP_ALIVE
    );

    /**
     * @brief Alias de compatibilité pour mqttsetClient().
     *
     * Ancien nom conservé pour éviter de casser du code existant.
     */
    bool mqttsetServer(
      const char* clientId,
      const char* user = nullptr,
      const char* pass = nullptr,
      uint16_t keepAlive = TEL_MQTT_KEEP_ALIVE
    );

    /**
     * @brief Connecte le client MQTT au broker.
     * @param host Adresse du broker.
     * @param port Port du broker.
     * @param timeout Délai maximal d'attente.
     * @return true si connecté, false sinon.
     */
    bool mqttconnect(const char* host, uint16_t port, uint32_t timeout = TEL_TIMEOUT_MQTT);

    /**
     * @brief Reconnecte le client MQTT avec les derniers paramètres.
     * @return true si reconnecté, false sinon.
     */
    bool mqttreconnect();

    /**
     * @brief Déconnecte le client MQTT.
     * @return true si déconnecté, false sinon.
     */
    bool mqttdisconnect();

    /**
     * @brief Vérifie si le client MQTT est connecté.
     * @return true si connecté, false sinon.
     */
    bool mqttisConnected();

    /**
     * @brief Publie un message MQTT.
     * @param topic Topic de destination.
     * @param payload Message à publier.
     * @param qos Niveau QoS.
     * @param retain true pour message retained.
     * @param timeout Délai maximal d'attente.
     * @return true si publication réussie, false sinon.
     */
    bool mqttpublish(
      const char* topic,
      const char* payload,
      uint8_t qos = 0,
      bool retain = false,
      uint32_t timeout = TEL_TIMEOUT_MQTT
    );

    /**
     * @brief S'abonne à un topic MQTT.
     * @param topic Topic à écouter.
     * @param qos Niveau QoS demandé.
     * @return true si abonnement réussi, false sinon.
     */
    bool mqttsubscribe(const char* topic, uint8_t qos = 0);

    /**
     * @brief Se désabonne d'un topic MQTT.
     * @param topic Topic à ne plus écouter.
     * @return true si désabonnement réussi, false sinon.
     */
    bool mqttunsubscribe(const char* topic);

    /**
     * @brief Définit le callback de réception MQTT.
     * @param callback Fonction appelée à la réception d'un message MQTT.
     * @return true si callback valide, false sinon.
     */
    bool mqttsetCallback(TEL_MQTT_Callback callback);

    /**
     * @brief Définit le callback brut des URC MQTT.
     * @param callback Fonction appelée avec les lignes brutes MQTT.
     * @return true si callback valide, false sinon.
     */
    bool mqttsetRawCallback(TEL_MQTT_RawCallback callback);
};

#endif // TEL0177_H