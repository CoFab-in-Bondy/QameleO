#ifndef QameleO_CONF
#define QameleO_CONF 

// 1 - DEFINE THE NAME OF THE QameleO ACCORDING to the DATABASE
#define SENSOR_NAME "TEST_I2C1"

// 2 - DEFINE QameleO INTERNAL COMPONENTS

#define DHT_SENSOR
//#define SHT_SENSOR

#define NextPM_SENSOR

// 3 - DEFINE log system
#define FILE_NAME_LOG "datalog.csv"
#define FILE_NAME_BUFFER "databuffer.csv"


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



#define MEASURE_PERIOD 15 
//#define REBOOT_DELAY 5 


// XXX - DEFINE REBOOT DELAY
#define REBOOT_DELAY 5 
#endif // QameleO_CONF
