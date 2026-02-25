from paho.mqtt import client as mqtt
import config_mqtt_correction
import time
import logging
import yaml
import modele_bayesien


FIRST_RECONNECT_DELAY = 1
RECONNECT_RATE = 2
MAX_RECONNECT_COUNT = 12
MAX_RECONNECT_DELAY = 60

broker = config_mqtt_correction.broker_qam
port = config_mqtt_correction.port_qam
topic = config_mqtt_correction.topic_qam
client_id = config_mqtt_correction.id_client
username = config_mqtt_correction.user
password = config_mqtt_correction.passw

# import capteurs et modèles
with open("data_capteurs.yaml", 'r') as f:
     data = yaml.safe_load(f)

data_capteurs = {c['name']: c for c in data['capteurs']}

# liste_capteurs = [capteur['name'] for capteur in data_capteurs]
liste_capteurs_reg = ['AIR_QAM01','AIR_QAM05']
liste_capteurs_bay = ['AIR_QAM01', 'AIR_QAM03', 'AIR_QAM05']

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
            if parts[1] in liste_capteurs_reg:
                name = parts[1]
                pm2_5 = float(data[1])
                pm10 = float(data[2])
                humidity = float(data[3])
                temp = float(data[4])

                # params modèle
                mean = data_capteurs[name]['modele_pm2.5']['mean']
                sd = data_capteurs[name]['modele_pm2.5']['sd']
                a = data_capteurs[name]['modele_pm2.5']['a']
                b = data_capteurs[name]['modele_pm2.5']['b']

                # correction
                pm2_5_scaled = (pm2_5 - mean) / sd
                new_pm2_5 = pm2_5_scaled * a + b

                parts[1] = name+"_C"
                data[1] = f"{new_pm2_5:.2f}"
                parts[-1] = ":".join(data)
                new_payload = ";".join(parts)
                client.publish(msg.topic, new_payload)
                print(f"Correction appliquée : {new_payload}\n")

            # correction bayesienne (filtre de Kalman)
            if parts[1] in liste_capteurs_bay:
                name = parts[1]
                pm2_5 = float(data[1])
                pm10 = float(data[2])
                humidity = float(data[3])
                temp = float(data[4])
                correction = modele_bayesien.ReceptionDonnees.on_reception(name, pm2_5)
                if correction is not None:
                    print(f"avant :{pm2_5}, après :{correction}")


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
