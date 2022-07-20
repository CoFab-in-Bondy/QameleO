#ifndef QAMELEO_CONF_H
#define QAMELEO_CONF_H


// 1 - DEFINE THE NAME OF THE QameleO ACCORDING to the DATABASE
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)


//#define SENSOR_NAME "zaidoaiuzbfdubzaod"
#ifndef SENSOR_NAME
    #error You must define a SENSOR_NAME
#endif
#pragma message(VAR_NAME_VALUE(SENSOR_NAME))
#pragma message(VAR_NAME_VALUE(SENSOR_NAME))

// 2 - DEFINE QameleO INTERNAL COMPONENTS

// Configuration
//#define DHT_SENSOR
//#define SHT_SENSOR

//#define NextPM_SENSOR
//#define PMS7003_SENSOR

// Default values
#ifndef DHT_SENSOR
    #ifndef SHT_SENSOR
        #define SHT_SENSOR
    #endif
#endif
#ifndef PMS7003_SENSOR
    #ifndef NextPM_SENSOR
        #define NextPM_SENSOR
    #endif
#endif

// Check duplicate
#ifdef DHT_SENSOR
    #ifdef SHT_SENSOR
        #error Must specify only one of NextPM_SENSOR and PMS7003_SENSOR
    #endif
#endif
#ifdef NextPM_SENSOR
    #ifdef PMS7003_SENSOR
        #error Must specify only one of NextPM_SENSOR and PMS7003_SENSOR
    #endif
#endif

#define CONFIG_MEGA

#pragma message(VAR_NAME_VALUE(DHT_SENSOR))
#pragma message(VAR_NAME_VALUE(SHT_SENSOR))
#pragma message(VAR_NAME_VALUE(NextPM_SENSOR))
#pragma message(VAR_NAME_VALUE(PMS7003_SENSOR))
 

// 3 - DEFINE log system

// Contient les données envoyées en GSM
#ifndef FILE_NAME_LOG
    #define FILE_NAME_LOG "datalog.csv"
#endif

// Contient les données qui n'ont pas encore été envoyé en GSM
#ifndef FILE_NAME_BUFFER
    #define FILE_NAME_BUFFER "notsend.csv"
#endif

// Contient les temps effectués par le programme en cas de reboot
#ifndef FILE_NAME_SAVE
    #define FILE_NAME_SAVE "timesave.csv"
#endif


// 4 - DEFINE GSM Connection

// Set Sim card pin code
#ifndef GSM_PIN_CODE
    #define GSM_PIN_CODE "0000"
#endif
#ifndef GSM_APN
    #define GSM_APN "orange"
#endif
#ifndef GSM_USER
    #define GSM_USER ""
#endif
#ifndef GSM_PASS
    #define GSM_PASS ""
#endif
#ifndef GSM_MQTT_BROKER
    #define GSM_MQTT_BROKER "vmpams.ird.fr"
#endif

#ifndef MEASURE_PERIOD
    #define MEASURE_PERIOD 1            // Time in minutes  default = 15
#endif
#ifndef SEND_PERIOD
    #define SEND_PERIOD 4               // Time in minutes  default = 60
#endif
#ifndef RECEIVE_UNIXTIME_PERIOD
    #define RECEIVE_UNIXTIME_PERIOD 240 // Time in minutes  default = 240
#endif

// XXX - DEFINE REBOOT DELAY
#ifndef REBOOT_DELAY
    #define REBOOT_DELAY 5
#endif


// Configuration for send message
#ifndef SMS_TARGET
  #define SMS_TARGET  "+33688334906" // "+33638231637"
#endif
#ifndef CALL_TARGET
  #define CALL_TARGET "+33688334906" // "+33638231637"
#endif
#ifndef NETWORK_REGISTER_TIMEOUT
  #define NETWORK_REGISTER_TIMEOUT 60000L
#endif

#endif // QAMELEO_CONF_H
