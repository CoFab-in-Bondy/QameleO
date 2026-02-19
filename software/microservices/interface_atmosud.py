import requests
from datetime import datetime
from paho.mqtt import client as mqtt
import time
import logging

# Atmosud
TOKEN = "tlxhZaBHiiP76wZqUiwArPiJ9H6ZkM5azxsoqYrSstXHSqM7cxFmotsLLv2Uw47B"
URL = "https://api-staging.uspot.probesys.net/observations"

# Qameleo
broker = "mqtt.qameleo.fr" # adresse du serveur MQTT
port = 1935
topic = "gamasenseit" # canal radio
client_id = "vvvvv1"
username = "gamasenseit"
password = "gamasenseit"


# Variables
FIRST_RECONNECT_DELAY = 1
RECONNECT_RATE = 2
MAX_RECONNECT_COUNT = 12
MAX_RECONNECT_DELAY = 60

liste_capteurs = {
    "AIR_QAM01": (43.1408, 5.88994),
    "AIR_QAM02": (43.1408, 5.88994),
    "AIR_QAM03": (43.1408, 5.88994),
    "AIR_QAM04": (43.0998, 6.32567),
    "AIR_QAM05": (43.0998, 6.32567)
}


logging.basicConfig(format='%(asctime)s - %(levelname)s - %(message)s', level=logging.INFO)

def on_disconnect(client, userdata, rc):
    logging.info("Déconnecté avec le code: %s", rc)
    reconnect_count, reconnect_delay = 0, FIRST_RECONNECT_DELAY
    while reconnect_count < MAX_RECONNECT_COUNT:
        logging.info("Tentative de reconnexion dan %d secondes...", reconnect_delay)
        time.sleep(reconnect_delay)
        try:
            client.reconnect()
            logging.info("Connexion réussie!")
            return
        except Exception as err:
            logging.error("%s. Echec. Nouvelle tentative...", err)
        reconnect_delay *= RECONNECT_RATE
        reconnect_delay = min(reconnect_delay, MAX_RECONNECT_DELAY)
        reconnect_count += 1
    logging.critical("Impossible de reconnecter après plusieurs tentatives.")

def connect_mqtt() -> mqtt.Client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            logging.info("Connecté au Broker MQTT")
        else:
            logging.error(f"Echec connexion, code retour {rc}")
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION1, client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect

    try:
        client.connect(broker, port)
    except Exception as e:
        logging.critical(f"Impossible de joindre le serveur: {e}")
        exit(1)
    return client

def subscribe(client: mqtt.Client):
    def on_message(client, userdata, msg):
        try:
            payload = msg.payload.decode()
            parts = payload.split(";")
            raw_data = parts[-1]
            data = raw_data.split(':')

            if parts[1] in liste_capteurs.keys():
                DEVICE_UID = parts[1]
                date = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                pm10 = float(data[2])
                pm25 = float(data[1])
                temp = float(data[4])
                humidite = float(data[3])


                to_insert = [
                    {
                        "deviceUid" : DEVICE_UID,
                        "deviceModelUid" : "qameleo",
                        "isoCode" : "24",
                        "happenedAt" : date,
                        "value" : pm10,
                        "longitude" : liste_capteurs[DEVICE_UID][1],
                        "latitude" : liste_capteurs[DEVICE_UID][0]
                    },
                    {
                        "deviceUid" : DEVICE_UID,
                        "deviceModelUid" : "qameleo",
                        "isoCode" : "39",
                        "happenedAt" : date,
                        "value" : pm25,
                        "longitude" : liste_capteurs[DEVICE_UID][1],
                        "latitude" : liste_capteurs[DEVICE_UID][0]
                    },
                    {
                        "deviceUid" : DEVICE_UID,
                        "deviceModelUid" : "qameleo",
                        "isoCode" : "54",
                        "happenedAt" : date,
                        "value" : temp,
                        "longitude" : liste_capteurs[DEVICE_UID][1],
                        "latitude" : liste_capteurs[DEVICE_UID][0]
                    },
                    {
                        "deviceUid" : DEVICE_UID,
                        "deviceModelUid" : "qameleo",
                        "isoCode" : "58",
                        "happenedAt" : date,
                        "value" : humidite,
                        "longitude" : liste_capteurs[DEVICE_UID][1],
                        "latitude" : liste_capteurs[DEVICE_UID][0]
                    }
                ]
                headers = {"Content-type": "application/json", "Authorization" : f"Bearer {TOKEN}"}
                response = requests.post(URL, json=to_insert, headers=headers)
                print("Status code:", response.status_code)
                print("Response:", response.text)



        except Exception as e:
            logging.error(f"Erreur lors du traitement du message: {e}")

    client.subscribe(topic)
    client.on_message = on_message

def run():
    client = connect_mqtt()
    client.on_disconnect = on_disconnect
    subscribe(client)
    client.loop_forever()

if __name__ == '__main__':
    run()
