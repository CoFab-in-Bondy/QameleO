#ifndef QAMELEO_CONF_H
#define QAMELEO_CONF_H

// 1 - DEFINE THE NAME OF THE QameleO ACCORDING to the DATABASE
#define SENSOR_NAME "TEST_"

// 2 - DEFINE QameleO INTERNAL COMPONENTS

//#define DHT_SENSOR
#define SHT_SENSOR

#define NextPM_SENSOR
//#define PMS7003_SENSOR

#define CONFIG_MEGA
 
// 3 - DEFINE log system
#define FILE_NAME_LOG "datalog.csv"           //Contient les données envoyées en GSM    //"datalog.csv"
#define FILE_NAME_BUFFER "notsend.csv"        //Contient les données qui n'ont pas encore été envoyé en GSM  //"databuffer.csv"
#define FILE_NAME_SAVE "timesave.csv"     //Contient les temps effectués par le programme en cas de reboot


// 4 - DEFINE GSM Connection
// Set Sim card pin code
#define GSM_PIN_CODE "0000"
#define GSM_APN "orange"
#define GSM_USER ""
#define GSM_PASS ""
#define GSM_MQTT_BROKER "vmpams.ird.fr"

// Set phone numbers, if you want to test SMS and Calls
//#define SMS_TARGET  "+33688334906" //"+33638231637"
//#define CALL_TARGET "+33688334906" //"+33638231637"
// Set phone connection timeout
//#define NETWORK_REGISTER_TIMEOUT 60000L



#define MEASURE_PERIOD 1            // Temps en minutes     //default = 15
#define SEND_PERIOD 4               // Temps en minutes     //default = 60
#define RECEIVE_UNIXTIME_PERIOD 240 // Temps en minutes     //default = 240


// XXX - DEFINE REBOOT DELAY
#define REBOOT_DELAY 5 
#endif // QAMELEO_CONF_H
