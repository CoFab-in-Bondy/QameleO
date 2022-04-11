#ifndef QameleO_HARD_CONF
#define QameleO_HARD_CONF 

//////////////CONFIGURATION GSM///////////////////////////
  #define GSM_OFF 9
  #define GSM_LOWP 2
  #define  GSM_ON 3
  #define  GSM_RESET 13
  #define TINY_GSM_MODEM_SIM800

  #ifndef GSM_PIN_CODE
    #define GSM_PIN_CODE "0000"
  #endif  
  #ifndef SMS_TARGET
    #define SMS_TARGET  "+33688334906" //"+33638231637"
  #endif
  #ifndef CALL_TARGET
    #define CALL_TARGET "+33688334906" //"+33638231637"
  #endif
  #ifndef NETWORK_REGISTER_TIMEOUT
    #define NETWORK_REGISTER_TIMEOUT 60000L
  #endif

#endif // QameleO_HARD_CONF
