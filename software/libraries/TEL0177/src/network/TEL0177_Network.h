#ifndef TEL0177_NETWORK_H
#define TEL0177_NETWORK_H

#include <Arduino.h>

#include "../at/TEL0177_AT_Core.h"
#include "../TEL0177_Types.h"
#include "../TEL0177_Config.h"


/**
 * @file TEL0177_Network.h
 * @brief Gestion SIM, réseau cellulaire et service data du module TEL0177.
 *
 * Cette classe vérifie l'état de la SIM, l'enregistrement réseau,
 * la qualité du signal et la connexion data (APN / PDP / IP).
 */
class TEL0177_Network {
private:
    TEL0177_AT_Core*  _at      = nullptr;   ///< Cœur AT pour les commandes.
    TEL_NetState      _state   = NET_DETACHED;  ///< État réseau interne.
    bool              _simReady      = false;   ///< SIM prête.
    bool              _networkReady  = false;   ///< Réseau prêt.
    bool              _dataReady     = false;   ///< Data prête.
    char              _ip[TEL_IP_SIZE]        = {0};  ///< Dernière IP.
    char              _apn[TEL_APN_SIZE]      = {0};  ///< Dernier APN.
    char              _pdpType[TEL_PDP_TYPE_SIZE] = {0};  ///< Type PDP.

    /**
     * @brief Configure le contexte APN/PDP.
     * @param apn Nom de l'APN.
     * @param pdpType Type PDP, par exemple "IP" ou "IPV4V6".
     * @return TEL_OK en cas de succès, sinon une erreur.
     */
    TEL_Status _configureAPN(const char* apn, const char* pdpType = TEL_PDP_TYPE_DEFAULT);

    /**
     * @brief Configure l'authentification APN (optionnelle).
     * @param user Nom d'utilisateur (peut être nullptr).
     * @param pass Mot de passe (peut être nullptr).
     * @return TEL_OK en cas de succès, sinon une erreur.
     */
    TEL_Status _configureAuth(const char* user = nullptr, const char* pass = nullptr);

    /**
     * @brief Attend l'enregistrement du modem sur le réseau mobile.
     * @param timeout Délai maximal d'attente.
     * @return TEL_OK si enregistré, sinon une erreur.
     */
    TEL_Status _waitForNetworkReg(uint32_t timeout);

public:
    /**
     * @brief Associe le cœur AT à la couche réseau.
     * @param at Instance déjà initialisée de TEL0177_AT_Core.
     */
    void begin(TEL0177_AT_Core& at);


    // ==========================================================================
    // SIM
    // ==========================================================================

    /**
     * @brief Vérifie si la SIM est prête (pas de PIN bloqué).
     * @return TEL_OK si prête, sinon un code d'erreur.
     */
    TEL_Status isSIMReady();

    /**
     * @brief Déverrouille la SIM avec un code PIN.
     * @param pin Code PIN.
     * @return TEL_OK si déverrouillée, sinon une erreur.
     */
    TEL_Status unlockSIM(const char* pin);


    // ==========================================================================
    // ENREGISTREMENT RÉSEAU
    // ==========================================================================

    /**
     * @brief Vérifie l'enregistrement réseau mobile.
     * @return TEL_OK si enregistré, sinon une erreur.
     */
    TEL_Status isNetworkRegistered();

    /**
     * @brief Vérifie l'enregistrement réseau data (GPRS/3G/4G).
     * @return TEL_OK si enregistré, sinon une erreur.
     */
    TEL_Status isDataNetworkRegistered();

    /**
     * @brief Vérifie l'enregistrement LTE/EPS.
     * @return TEL_OK si enregistré, sinon une erreur.
     */
    TEL_Status isLTERegistered();

    /**
     * @brief Attend que le modem soit enregistré sur le réseau.
     * @param timeout Délai maximal d'attente.
     * @return TEL_OK si enregistré, sinon une erreur.
     */
    TEL_Status waitForNetwork(uint32_t timeout = TEL_TIMEOUT_NETWORK);

    /**
     * @brief Lit la qualité du signal (RSSI/BER).
     * @param rssi Référence pour la valeur RSSI.
     * @param ber  Référence pour la valeur BER.
     * @return TEL_OK si lu, sinon une erreur.
     */
    TEL_Status getSignalQuality(int& rssi, int& ber);


    // ==========================================================================
    // SERVICE DATA / PDP
    // ==========================================================================

    /**
     * @brief Vérifie si le modem est attaché au service data.
     * @return TEL_OK si attaché, sinon une erreur.
     */
    TEL_Status isDataAttached();

    /**
     * @brief Attache le modem au service data.
     * @return TEL_OK si attachement réussi, sinon une erreur.
     */
    TEL_Status attachData();

    /**
     * @brief Détache le modem du service data.
     * @return TEL_OK si détachement réussi, sinon une erreur.
     */
    TEL_Status detachData();

    /**
     * @brief Vérifie si le contexte PDP est actif.
     * @return TEL_OK si actif, sinon une erreur.
     */
    TEL_Status isDataConnected();

    /**
     * @brief Active le contexte PDP configuré.
     * @return TEL_OK si activé, sinon une erreur.
     */
    TEL_Status activateDataContext();

    /**
     * @brief Désactive le contexte PDP configuré.
     * @return TEL_OK si désactivé, sinon une erreur.
     */
    TEL_Status deactivateDataContext();

    /**
     * @brief Configure APN, auth, attache et active le service data.
     * @param apn Nom de l'APN.
     * @param user Nom d'utilisateur (optionnel, peut être nullptr).
     * @param pass Mot de passe (optionnel, peut être nullptr).
     * @param pdpType Type PDP, par exemple "IP" ou "IPV4V6".
     * @return TEL_OK si data prête, sinon une erreur.
     */
    TEL_Status connectData(
        const char* apn,
        const char* user = nullptr,
        const char* pass = nullptr,
        const char* pdpType = TEL_PDP_TYPE_DEFAULT
    );

    /**
     * @brief Désactive la connexion data (PDP et/ou attachement).
     * @return TEL_OK si déconnexion réussie, sinon une erreur.
     */
    TEL_Status disconnectData();

    /**
     * @brief Retourne l'adresse IP attribuée au contexte PDP.
     * @param buffer Buffer de destination.
     * @param size Taille du buffer.
     * @return TEL_OK si IP récupérée, sinon une erreur.
     */
    TEL_Status getIPAddress(char* buffer, uint16_t size);


    // ==========================================================================
    // INFORMATIONS MODULE / SIM / RÉSEAU
    // ==========================================================================

    /**
     * @brief Lit l'opérateur réseau actuel.
     * @param buffer Buffer de destination.
     * @param size Taille du buffer.
     * @return TEL_OK si lu, sinon une erreur.
     */
    TEL_Status getOperator(char* buffer, uint16_t size);

    /**
     * @brief Lit l'IMEI du module.
     * @param buffer Buffer de destination.
     * @param size Taille du buffer.
     * @return TEL_OK si lu, sinon une erreur.
     */
    TEL_Status getIMEI(char* buffer, uint16_t size);

    /**
     * @brief Lit l'ICCID de la carte SIM.
     * @param buffer Buffer de destination.
     * @param size Taille du buffer.
     * @return TEL_OK si lu, sinon une erreur.
     */
    TEL_Status getICCID(char* buffer, uint16_t size);

    /**
     * @brief Lit l'IMSI de la carte SIM.
     * @param buffer Buffer de destination.
     * @param size Taille du buffer.
     * @return TEL_OK si lu, sinon une erreur.
     */
    TEL_Status getIMSI(char* buffer, uint16_t size);

    /**
     * @brief Lit le fabricant du module.
     * @param buffer Buffer de destination.
     * @param size Taille du buffer.
     * @return TEL_OK si lu, sinon une erreur.
     */
    TEL_Status getManufacturer(char* buffer, uint16_t size);

    /**
     * @brief Lit le modèle du module.
     * @param buffer Buffer de destination.
     * @param size Taille du buffer.
     * @return TEL_OK si lu, sinon une erreur.
     */
    TEL_Status getModel(char* buffer, uint16_t size);

    /**
     * @brief Lit la version firmware du module.
     * @param buffer Buffer de destination.
     * @param size Taille du buffer.
     * @return TEL_OK si lu, sinon une erreur.
     */
    TEL_Status getFirmware(char* buffer, uint16_t size);


    // ==========================================================================
    // GETTERS D'ÉTAT LOCAL
    // ==========================================================================

    /**
     * @brief Retourne l'état réseau interne.
     * @return État TEL_NetState.
     */
    TEL_NetState getState() const;

    /**
     * @brief Indique si la SIM était prête au dernier contrôle.
     * @return true si prête.
     */
    bool simReady() const;

    /**
     * @brief Indique si le réseau était prêt au dernier contrôle.
     * @return true si prêt.
     */
    bool networkReady() const;

    /**
     * @brief Indique si la data était prête au dernier contrôle.
     * @return true si prête.
     */
    bool dataReady() const;

    /**
     * @brief Retourne la dernière adresse IP connue.
     * @return IP ou chaîne vide si inconnue.
     */
    const char* ipAddress() const;

    /**
     * @brief Retourne le dernier APN configuré.
     * @return APN ou chaîne vide si inconnu.
     */
    const char* apn() const;

    /**
     * @brief Retourne le dernier type PDP configuré.
     * @return Type PDP ou chaîne vide si inconnu.
     */
    const char* pdpType() const;
};

#endif // TEL0177_NETWORK_H