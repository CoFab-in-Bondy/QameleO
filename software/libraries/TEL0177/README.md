# Librairie TEL0177

Librairie Arduino pour piloter le module **Gravity 4G TEL0177 / Air780E** avec des commandes AT.

L’objectif de cette librairie est de fournir une API simple pour :

- communiquer avec le modem ;
- gérer la carte SIM ;
- gérer le réseau mobile ;
- activer la connexion data avec APN / PDP / IP ;
- utiliser MQTT ;
- préparer l’intégration dans le projet **QameleO**.

---

## Références utilisées

Les commandes AT doivent être vérifiées avec ces documentations :

- Documentation DFRobot TEL0177 :  
  https://wiki.dfrobot.com/tel0177/docs/22134

- Documentation OpenLuat Air780E AT :  
  https://docs.openluat.com/air780e/at/app/at_command/

---

## Structure de la librairie

```text
TEL0177/
├── src/
│   ├── TEL0177.h
│   ├── TEL0177.cpp
│   ├── TEL0177_Config.h
│   ├── TEL0177_Types.h
│   │
│   ├── at/
│   │   ├── TEL0177_AT_Core.h
│   │   ├── TEL0177_AT_Core.cpp
│   │
│   ├── network/
│   │   ├── TEL0177_Network.h
│   │   ├── TEL0177_Network.cpp
│   │
│   ├── mqtt/
│   │   ├── TEL0177_MQTT.h
│   │   ├── TEL0177_MQTT.cpp
│   │
│   ├── http/
│   │   ├── TEL0177_HTTP.h
│   │   ├── TEL0177_HTTP.cpp
│   │
│   ├── sms/
│       ├── TEL0177_SMS.h
│       ├── TEL0177_SMS.cpp
│
├── examples/
│   ├── Basic/
│   │   └── ModuleInfo.ino
│   ├── Network/
│   │   └── NetworkStatus.ino
│   ├── MQTT/
│   │   ├── MqttPublish.ino
│   │   └── MqttSubscribe.ino
│   └── QameleO/
│       └── QameleO_GSM_Test.ino
│
├── keywords.txt
├── library.properties
└── README.md

```

## Composants

### TEL0177_AT_Core

Couche bas niveau.

Elle sert à envoyer les commandes AT, lire les réponses, attendre OK, ERROR, ou une réponse spécifique.

### TEL0177_Network

Couche réseau.

Elle sert à gérer :

- la SIM ;
- l’enregistrement réseau ;
- la qualité du signal ;
- l’APN ;
- l’attachement data ;
- le contexte PDP ;
- l’adresse IP.

### TEL0177_MQTT

Couche MQTT.

Elle sert à :

- configurer le client MQTT ;
- se connecter au broker ;
- publier des messages ;
- s’abonner à des topics ;
- recevoir des messages MQTT.

### TEL0177

Façade principale de la librairie.

---

## API publique TEL0177.h

### Initialisation / Modem

| Fonction | But | Utilité |
|----------|-----|---------|
| `begin(Stream& serial)` | Initialise la librairie avec le port série du modem | À appeler avant toute autre fonction |
| `loop()` | Fait tourner les services internes | À appeler dans loop() Arduino, surtout pour MQTT |
| `reset()` | Redémarre le module avec la commande configurée | Utile si le modem bloque |
| `isAlive()` | Vérifie si le modem répond à AT | Test de communication |
| `lastError()` | Retourne la dernière erreur TEL_Status | Debug |
| `lastResponse()` | Retourne la dernière réponse AT brute | Debug avancé |

### SIM

| Fonction | But | Commandes AT typiques |
|----------|-----|----------------------|
| `isSIMReady()` | Vérifie si la SIM est prête | `AT+CPIN?` |
| `simUnlock(const char* pin)` | Déverrouille la SIM avec un code PIN | `AT+CPIN="0000"` |

### Réseau mobile

| Fonction | But | Commandes AT typiques |
|----------|-----|----------------------|
| `isNetworkRegistered()` | Vérifie l'enregistrement réseau mobile | `AT+CREG?` |
| `isDataNetworkRegistered()` | Vérifie l'enregistrement réseau data | `AT+CGREG?` |
| `isLTERegistered()` | Vérifie l'enregistrement LTE/EPS | `AT+CEREG?` |
| `waitForNetwork(timeout)` | Attend que le réseau soit disponible | Boucle sur CREG, CGREG, CEREG |
| `getSignalQuality(rssi, ber)` | Lit la qualité du signal | `AT+CSQ` |

### Data / APN / PDP / IP

| Fonction | But | Commandes AT typiques |
|----------|-----|----------------------|
| `isDataAttached()` | Vérifie l'attachement au service data | `AT+CGATT?` |
| `attachData()` | Attache le modem au service data | `AT+CGATT=1` |
| `detachData()` | Détache le modem du service data | `AT+CGATT=0` |
| `activateDataContext()` | Active le contexte PDP | `AT+CGACT=1,1` |
| `deactivateDataContext()` | Désactive le contexte PDP | `AT+CGACT=0,1` |
| `isDataConnected()` | Vérifie si le contexte PDP est actif | `AT+CGACT?` |
| `networkConnect(apn, user, pass, pdpType)` | Configure APN, auth, data, PDP et IP | `CGDCONT`, `CGAUTH`, `CGATT`, `CGACT`, `CGPADDR` |
| `networkDisconnect()` | Coupe proprement la data | `CGACT=0`, `CGATT=0` |
| `getIPAddress(buffer, size)` | Récupère l'adresse IP actuelle | `AT+CGPADDR=1` |

### MQTT

#### Configuration MQTT

| Fonction | But | Commande AT typique |
|----------|-----|---------------------|
| `mqttsetClient(clientId, user, pass, keepAlive)` | Configure l'identité MQTT côté librairie | Prépare `AT+MCONFIG` |
| `mqttsetServer(...)` | Alias de compatibilité de `mqttsetClient()` | Même rôle |

#### Connexion MQTT

| Fonction | But | Commande AT typique |
|----------|-----|---------------------|
| `mqttconnect(host, port, timeout)` | Se connecte au broker MQTT | `MCONFIG`, `MIPSTART`, `MCONNECT` |
| `mqttreconnect()` | Reconnecte avec les derniers paramètres | `MIPSTART`, `MCONNECT` |
| `mqttdisconnect()` | Déconnecte MQTT | `AT+MDISCONNECT` |
| `mqttisConnected()` | Vérifie l'état MQTT | `AT+MQTTSTATU` |

#### Publication MQTT

| Fonction | But | Commande AT typique |
|----------|-----|---------------------|
| `mqttpublish(topic, payload, qos, retain, timeout)` | Publie un message MQTT | `AT+MPUB="topic",qos,retain,"message"` |

#### Abonnement MQTT

| Fonction | But | Commande AT typique |
|----------|-----|---------------------|
| `mqttsubscribe(topic, qos)` | S'abonne à un topic | `AT+MSUB="topic",qos` |
| `mqttunsubscribe(topic)` | Se désabonne d'un topic | `AT+MUNSUB="topic"` |
| `mqttsetCallback(callback)` | Définit la fonction appelée à la réception d'un message | Callback logiciel |
| `mqttsetRawCallback(callback)` | Définit une fonction appelée avec les lignes brutes MQTT | Debug réception MQTT |

