#ifndef TEL0177_MQTT_H
#define TEL0177_MQTT_H

#include "../at/TEL0177_AT_Core.h"
#include "../TEL0177_Config.h"
#include "../TEL0177_Types.h"


/**
 * @file TEL0177_MQTT.h
 * @brief Gestion MQTT du module TEL0177.
 *
 * Cette classe configure le client MQTT, se connecte à un broker,
 * publie/écoute des messages et expose des callbacks.
 */


/**
 * @brief Callback appelé lorsqu'un message MQTT est reçu.
 * @param topic Topic du message.
 * @param payload Contenu du message.
 * @param length Longueur du payload.
 */
typedef void (*TEL_MQTT_Callback)(const char* topic, const char* payload, uint16_t length);


/**
 * @brief Callback appelé avec les URC MQTT bruts.
 * @param urc Ligne brute reçue du modem.
 */
typedef void (*TEL_MQTT_RawCallback)(const char* urc);


class TEL0177_MQTT {
private:
    TEL0177_AT_Core*     _at         = nullptr;   ///< Cœur AT.
    TEL_MQTT_Callback    _callback   = nullptr;   ///< Callback messages MQTT.
    TEL_MQTT_RawCallback _rawCallback = nullptr; ///< Callback URC bruts.
    bool               _connected   = false;     ///< Connexion MQTT.
    char               _host[TEL_MQTT_HOST_SIZE]     = {0};  ///< Host du broker.
    char               _clientId[TEL_MQTT_CLIENT_ID_SIZE] = {0};  ///< Id client.
    char               _user[TEL_MQTT_USER_SIZE]       = {0};  ///< User MQTT.
    char               _pass[TEL_MQTT_PASS_SIZE]       = {0};  ///< Pass MQTT.
    uint16_t           _port        = TEL_MQTT_DEFAULT_PORT;    ///< Port broker.
    uint16_t           _keepAlive   = TEL_MQTT_KEEP_ALIVE;      ///< Keep‑alive.

    /**
     * @brief Configure l'adresse et le port du broker dans le modem.
     * @param host Adresse du broker.
     * @param port Port du broker.
     * @return TEL_Status.
     */
    TEL_Status _configBroker(const char* host, uint16_t port);

    /**
     * @brief Configure les identifiants client MQTT dans le modem.
     * @param clientId Id client.
     * @param user User MQTT (peut être nullptr).
     * @param pass Pass MQTT (peut être nullptr).
     * @return TEL_Status.
     */
    TEL_Status _configCredentials(const char* clientId, const char* user, const char* pass);

    /**
     * @brief Parse une ligne URC MQTT pour extraire un message.
     * @param urc Ligne reçue.
     * @param msg Structure de destination.
     * @return true si message reconnu.
     */
    bool _parseIncomingMessage(const char* urc, TEL_MQTT_Message& msg);

    /**
     * @brief Traite une ligne MQTT reçue et appelle les callbacks.
     * @param urc Ligne brute reçue.
     */
    void _handleRecv(const char* urc);

public:
    /**
     * @brief Associe le cœur AT à la couche MQTT.
     * @param at Instance déjà initialisée de TEL0177_AT_Core.
     */
    void begin(TEL0177_AT_Core& at);

    /**
     * @brief Configure l'identité et le keep‑alive MQTT.
     * @param clientId Id client.
     * @param user User MQTT (optionnel).
     * @param pass Pass MQTT (optionnel).
     * @param keepAlive Keep‑alive en secondes.
     */
    void setClientConfig(
        const char* clientId,
        const char* user = nullptr,
        const char* pass = nullptr,
        uint16_t keepAlive = TEL_MQTT_KEEP_ALIVE
    );

    /**
     * @brief Se connecte à un broker MQTT.
     * @param host Adresse du broker.
     * @param port Port du broker.
     * @param timeout Délai maximal d'attente.
     * @return TEL_OK si OK, sinon un code d'erreur.
     */
    TEL_Status connect(const char* host, uint16_t port, uint32_t timeout = TEL_TIMEOUT_MQTT);

    /**
     * @brief Déconnecte le client MQTT.
     * @return TEL_Status.
     */
    TEL_Status disconnect();

    /**
     * @brief Indique si la connexion MQTT est active.
     * @return true si connecté.
     */
    bool isConnected();

    /**
     * @brief Tente une reconnexion MQTT avec les derniers paramètres.
     * @return TEL_Status.
     */
    TEL_Status reconnect();

    /**
     * @brief Publie un message MQTT sur un topic.
     * @param topic Topic de destination.
     * @param payload Message à publier.
     * @param qos Niveau QoS.
     * @param retain true pour message retained.
     * @param timeout Délai maximal d'attente.
     * @return TEL_Status.
     */
    TEL_Status publish(
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
     * @return TEL_Status.
     */
    TEL_Status subscribe(const char* topic, uint8_t qos = 0);

    /**
     * @brief Se désabonne d'un topic MQTT.
     * @param topic Topic à ne plus écouter.
     * @return TEL_Status.
     */
    TEL_Status unsubscribe(const char* topic);

    /**
     * @brief Traite les événements MQTT entrants.
     *
     * À appeler régulièrement (ex: dans loop()).
     */
    void loop();

    /**
     * @brief Traite les événements MQTT pendant une durée donnée.
     * @param durationMs Durée en millisecondes.
     */
    void loopFor(uint32_t durationMs);

    /**
     * @brief Définit le callback pour les messages MQTT reçus.
     * @param callback Fonction utilisateur.
     */
    void setCallback(TEL_MQTT_Callback callback);

    /**
     * @brief Définit le callback pour les URC MQTT bruts.
     * @param callback Fonction utilisateur.
     */
    void setRawCallback(TEL_MQTT_RawCallback callback);
};

#endif // TEL0177_MQTT_H