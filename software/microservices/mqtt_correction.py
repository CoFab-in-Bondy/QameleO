from paho.mqtt import client as mqtt
import time
import logging

FIRST_RECONNECT_DELAY = 1
RECONNECT_RATE = 2
MAX_RECONNECT_COUNT = 12
MAX_RECONNECT_DELAY = 60

broker = "***REMOVED***" # adresse du serveur MQTT
port = ***REMOVED***
topic = "***REMOVED***" # canal radio
client_id = "***REMOVED***"
username = "***REMOVED***"
password = "***REMOVED***"

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
            current_topic = msg.topic
            print(f"[{current_topic}] -> {payload}")
            parts = payload.split(";")
            raw_data = parts[-1]
            data = raw_data.split(':')
            print(f"Capteur :{parts[1]}, pm1 :{data[0]}, pm2.5 :{data[1]}, pm10 :{data[2]}, humidité :{data[3]}, température :{data[4]}")
            if parts[1] == "AIR_QAM01":
                pm2_5 = data[1]
                pm2_5_scaled = (float(pm2_5)- 7.9778953) / 5.63427334
                new_pm2_5 = pm2_5_scaled * 4.43620441 + 6.978750000000002
                parts[1] = "AIR_QAM01_C"
                data[1] = f"{new_pm2_5:.2f}"
                parts[-1] = ":".join(data)
                new_payload = ";".join(parts)
                client.publish(msg.topic, new_payload)
                print(f"Correction appliquée : {new_payload}\n")


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
