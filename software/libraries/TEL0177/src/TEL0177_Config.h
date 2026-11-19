#ifndef TEL0177_CONFIG_H
#define TEL0177_CONFIG_H

#include <Arduino.h>
#include <stdio.h>

/**
 * @file TEL0177_Config.h
 * @brief Constantes globales, tailles de buffers, timeouts et macros de log
 *        de la librairie TEL0177.
 */

// ============================================================================
// CONFIGURATION GÉNÉRALE
// ============================================================================

/**
 * @brief Vitesse série par défaut du module.
 */
#define TEL_DEFAULT_BAUD 115200

/**
 * @brief Identifiant du contexte PDP utilisé pour la session data.
 */
#define TEL_PDP_CONTEXT_ID 1

/**
 * @brief Type PDP par défaut.
 */
#define TEL_PDP_TYPE_DEFAULT "IP"


// ============================================================================
// PORT DE LOG
// ============================================================================

/**
 * @brief Port série utilisé pour les logs de la librairie.
 *
 * Peut être redéfini avant inclusion du fichier.
 */
#ifndef TEL_LOG_PORT
#define TEL_LOG_PORT Serial
#endif


// ============================================================================
// ACTIVATION DES TRACES
// ============================================================================

/**
 * @brief Active ou désactive les logs standards.
 * 1 = activé, 0 = désactivé.
 */
#ifndef TEL_ENABLE_LOG
#define TEL_ENABLE_LOG 1
#endif

/**
 * @brief Active ou désactive les logs de debug.
 * 1 = activé, 0 = désactivé.
 */
#ifndef TEL_ENABLE_DEBUG
#define TEL_ENABLE_DEBUG 1
#endif


// ============================================================================
// COMMANDE DE RESET
// ============================================================================

/**
 * @brief Commande AT utilisée pour redémarrer le module.
 *
 * Peut être redéfinie si un firmware préfère une autre commande.
 */
#define TEL_RESET_COMMAND "AT+RESET"



// ============================================================================
// MACROS DE LOG
// ============================================================================

#if TEL_ENABLE_LOG
  #define TEL_LOG(x)                 do { TEL_LOG_PORT.print(x); } while (0)
  #define TEL_LOGLN(x)               do { TEL_LOG_PORT.println(x); } while (0)
  #define TEL_LOG_ERROR(fmt, ...)    do { char _buf[256]; snprintf(_buf, sizeof(_buf), fmt, ##__VA_ARGS__); TEL_LOG_PORT.print(_buf); } while (0)
  #define TEL_LOGLN_ERROR(fmt, ...)  do { char _buf[256]; snprintf(_buf, sizeof(_buf), fmt, ##__VA_ARGS__); TEL_LOG_PORT.println(_buf); } while (0)
#else
  #define TEL_LOG(x)                 do {} while (0)
  #define TEL_LOGLN(x)               do {} while (0)
  #define TEL_LOG_ERROR(fmt, ...)    do {} while (0)
  #define TEL_LOGLN_ERROR(fmt, ...)  do {} while (0)
#endif

#if TEL_ENABLE_DEBUG
  #define TEL_DEBUG(x)               do { TEL_LOG_PORT.print(x); } while (0)
  #define TEL_DEBUGLN(x)             do { TEL_LOG_PORT.println(x); } while (0)
#else
  #define TEL_DEBUG(x)               do {} while (0)
  #define TEL_DEBUGLN(x)             do {} while (0)
#endif


// ============================================================================
// TIMEOUTS
// ============================================================================

/**
 * @brief Timeout générique par défaut.
 *
 * Utilisé quand aucune catégorie plus précise n'est fournie.
 */
#define TEL_TIMEOUT_DEFAULT 2000UL

/**
 * @brief Timeout court pour les commandes AT simples.
 *
 * Exemples :
 * - AT
 * - AT+CPIN?
 * - AT+CREG?
 * - AT+CGREG?
 * - AT+CEREG?
 * - AT+CSQ
 * - AT+CGATT?
 * - AT+CGACT?
 * - AT+CGPADDR=1
 * - AT+MQTTSTATU
 */
#define TEL_TIMEOUT_DEFAULT_AT 1200UL

/**
 * @brief Timeout pour les opérations réseau globales.
 *
 * Exemples :
 * - attente réseau
 * - attachement data
 * - activation PDP
 */
#define TEL_TIMEOUT_NETWORK 4000UL

/**
 * @brief Timeout pour les opérations HTTP.
 */
#define TEL_TIMEOUT_HTTP 15000UL

/**
 * @brief Timeout pour les opérations MQTT.
 *
 * Exemples :
 * - AT+MIPSTART
 * - AT+MCONNECT
 * - AT+MSUB
 */
#define TEL_TIMEOUT_MQTT 6000UL

/**
 * @brief Timeout pour les opérations SMS.
 */
#define TEL_TIMEOUT_SMS 5000UL

/**
 * @brief Temps d’attente après démarrage ou reset du module.
 */
#define TEL_TIMEOUT_BOOT 2500UL

/**
 * @brief Délai d’attente très court entre lectures de réponse AT.
 */
#define TEL_TIMEOUT_AT_WAIT_RESPONSE 200UL


// ============================================================================
// RÉPONSES AT STANDARD
// ============================================================================

/**
 * @brief Réponse AT indiquant un succès.
 */
#define TEL_RESPONSE_OK "OK"

/**
 * @brief Réponse AT indiquant une erreur.
 */
#define TEL_RESPONSE_ERROR "ERROR"


// ============================================================================
// RETRY
// ============================================================================

/**
 * @brief Nombre maximal de tentatives pour certaines opérations.
 *
 * Profil plus réactif pour le développement.
 */
#define TEL_MAX_RETRIES 2

/**
 * @brief Délai entre deux tentatives.
 */
#define TEL_RETRY_DELAY_MS 200UL

/**
 * @brief Délai après saisie du PIN SIM avant relecture de l'état.
 */
#define TEL_SIM_UNLOCK_DELAY_MS 1200UL

/**
 * @brief Timeout court pour fermer proprement la socket MQTT.
 */
#define TEL_TIMEOUT_MQTT_CLOSE 1000UL

/**
 * @brief Délai court après fermeture MQTT avant tentative suivante.
 */
#define TEL_MQTT_CLOSE_WAIT_MS 200UL

/**
 * @brief Délai entre deux tentatives de connexion MQTT.
 */
#define TEL_MQTT_RETRY_DELAY_MS 500UL


// ============================================================================
// TAILLES DES BUFFERS
// ============================================================================

/**
 * @brief Taille du buffer de réception brut.
 */
#define TEL_RX_BUFFER_SIZE 256

/**
 * @brief Taille maximale d’une ligne lue depuis le modem.
 */
#define TEL_LINE_BUFFER_SIZE 256

/**
 * @brief Taille maximale d’une commande AT construite.
 */
#define TEL_CMD_BUFFER_SIZE 256

/**
 * @brief Taille maximale du corps HTTP.
 */
#define TEL_HTTP_BODY_SIZE 1024

/**
 * @brief Taille maximale du topic MQTT.
 */
#define TEL_MQTT_TOPIC_SIZE 128

/**
 * @brief Taille maximale du payload MQTT.
 */
#define TEL_MQTT_PAYLOAD_SIZE 512


// ============================================================================
// PARAMÈTRES RÉSEAU ET MQTT
// ============================================================================

/**
 * @brief Port MQTT par défaut.
 */
#define TEL_MQTT_DEFAULT_PORT 1883

/**
 * @brief Intervalle Keep Alive MQTT en secondes.
 */
#define TEL_MQTT_KEEP_ALIVE 60

/**
 * @brief Mode clean session MQTT.
 */
#define TEL_MQTT_CLEAN_SESSION 1

/**
 * @brief Nombre de tentatives de connexion MQTT.
 *
 * Sur ton module, il arrive qu'une première tentative échoue
 * après un CONNECT OK suivi d'un CLOSED. Une seconde tentative
 * suffit souvent.
 */
#ifndef TEL_MQTT_CONNECT_RETRIES
#define TEL_MQTT_CONNECT_RETRIES 2
#endif

#define TEL_MQTT_HOST_SIZE 64
#define TEL_MQTT_CLIENT_ID_SIZE 64
#define TEL_MQTT_USER_SIZE 32
#define TEL_MQTT_PASS_SIZE 64

#define TEL_IP_SIZE 32
#define TEL_APN_SIZE 64
#define TEL_PDP_TYPE_SIZE 12

#endif // TEL0177_CONFIG_H