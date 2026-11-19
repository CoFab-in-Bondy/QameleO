#ifndef TEL0177_TYPES_H
#define TEL0177_TYPES_H

#include <Arduino.h>
#include "TEL0177_Config.h"

/**
 * @file TEL0177_Types.h
 * @brief Définit les types communs utilisés par la librairie TEL0177.
 */

/**
 * @enum TEL_Status
 * @brief Codes de retour unifiés des fonctions de la librairie.
 */
enum TEL_Status : uint8_t {
    TEL_OK = 0,             ///< Exécution réussie
    TEL_ERROR,              ///< Erreur générique
    TEL_TIMEOUT,            ///< Délai dépassé avant réception d'une réponse
    TEL_BUSY,               ///< Modem occupé
    TEL_NO_RESPONSE,        ///< Aucun octet reçu
    TEL_INVALID_PARAM,      ///< Paramètre invalide passé à la fonction
    TEL_NOT_CONNECTED,      ///< Action impossible sans connexion active
    TEL_BUFFER_OVERFLOW,    ///< Buffer trop petit ou commande tronquée
    TEL_NOT_READY,          ///< Ressource non prête, par exemple SIM verrouillée ou réseau absent
    TEL_PARSE_ERROR         ///< Réponse reçue mais impossible à analyser
};

/**
 * @enum TEL_NetState
 * @brief État de la connexion réseau cellulaire.
 */
enum TEL_NetState : uint8_t {
    NET_DETACHED = 0,       ///< Aucun réseau ou service data non disponible
    NET_REGISTERED,         ///< Modem enregistré sur le réseau mobile
    NET_DATA_ATTACHED,      ///< Modem attaché au service packet/data
    NET_DATA_CONNECTED      ///< Contexte PDP actif, adresse IP disponible
};

/**
 * @struct TEL_MQTT_Message
 * @brief Représente un message MQTT entrant.
 *
 * @note Le champ `length` contient la longueur utile du payload,
 *       sans compter le caractère nul '\0'.
 * @note Les champs `topic` et `payload` doivent être terminés par '\0'.
 */
struct TEL_MQTT_Message {
    char     topic[TEL_MQTT_TOPIC_SIZE];     ///< Topic MQTT reçu
    char     payload[TEL_MQTT_PAYLOAD_SIZE]; ///< Payload MQTT reçu
    uint8_t  qos;                            ///< Niveau QoS du message si disponible
    uint16_t length;                         ///< Longueur utile du payload
};

#endif // TEL0177_TYPES_H