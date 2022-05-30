#include "QameleO_SD.h"

#include <SD.h>
#include "QameleO_conf.h"
#include "QameleO_hard_conf.h"
#include "MyPile.h"
#include "MyPile.cpp"
#include "MyFile.h"
#include "MyFile.cpp"

/**
 * Maker of the class
 */
QameleO_SD::QameleO_SD()
{
  //Do nothing
}

/**
 * Write in the SD card on the buffer file
 * 
 * @param msg - The message to write
 */
void QameleO_SD::saveDataInBufferFile(String msg)
{
  myFile = SD.open(FILE_NAME_BUFFER, FILE_WRITE);
   // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to " + String(FILE_NAME_BUFFER) + " ... ");
    myFile.println(msg);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening " + String(FILE_NAME_BUFFER));
  }
}

/**
 * Write in the SD card on the log file
 * 
 * @param msg - The message to write
 */
void QameleO_SD::saveDataInLogFile(String msg)
{
  myFile = SD.open(FILE_NAME_LOG, FILE_WRITE);
   // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to " + String(FILE_NAME_LOG) + " ... ");
    myFile.println(msg);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening " + String(FILE_NAME_LOG));
  }
}

/**
 * That save the number return by millis() at the begining of the file
 */
void QameleO_SD::saveMillisInFile()
{
  if(this->wakeupSD()){
    if(SD.exists(FILE_NAME_SAVE)){
      myFile = SD.open(FILE_NAME_SAVE, FILE_READ);
      if (myFile){
        //read the file before, get all, sup the file, create the file and push the data
        char reader[myFile.size()];
        String oneLine;
        Queue<String> millisToSave;
        /*int nbLine=0;*/
      
        //Serial.println("Reading to " + String(FILE_NAME_SAVE) + " ... ");
        int nbByteRead=myFile.read(reader, sizeof(reader));
        if (nbByteRead != myFile.size())
          Serial.println("Not all of the file has been read ");

        for(int i=0; i < sizeof(reader); i++){
          if (reader[i] == '\r' || reader[i] == '\n'){
            if (reader[i] == '\n'){
              //Throw '\n'
            }else{
              /*nbLine++;
              //Serial.println(oneLine);*/
              millisToSave.enqueue(oneLine);
              oneLine="";
            }
          }else{
            oneLine+=reader[i];
          }
        }
        myFile.close();
        SD.remove(FILE_NAME_SAVE);

        myFile = SD.open(FILE_NAME_SAVE, FILE_WRITE);
        if (myFile){
          Serial.print("Writing the millis backup on " + String(FILE_NAME_SAVE)+" ... ");
          
          String tmp = String(millis()) + ";";
           myFile.println(tmp);
           
          while(!millisToSave.isEmpty())
            myFile.println(millisToSave.dequeue());
          //Serial.print(" - Data millis() save " + tmp +" ");
  
          // close the file:
          myFile.close();
          Serial.println("done.");
        }else{
          Serial.println("Error opening " + String(FILE_NAME_SAVE));
        } 
      }else{
        Serial.println("Error opening " + String(FILE_NAME_SAVE));
      }
    }else {
      myFile = SD.open(FILE_NAME_SAVE, FILE_WRITE);
        if (myFile){
          Serial.print("Writing the millis backup on " + String(FILE_NAME_SAVE)+" ... ");
          String tmp = String(millis()) + ";";
          myFile.println(tmp);
          //Serial.print(" - Data millis() save " + tmp +" ");
  
          // close the file:
          myFile.close();
          Serial.println("done.");
        }else{
          Serial.println("Error opening " + String(FILE_NAME_SAVE));
        } 
    }
  }
}

/**
 * Check and start the SD card
 */
bool QameleO_SD::wakeupSD()
{
  if (!SD.begin(SD_CONTROL_PIN)) {
    Serial.println("initialization failed!");
    return false;
   
  }else {
    //Serial.println("initialization done.");
      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
    return true;
  }

}

/**
 * Saved the data on a other file and Return all data saved but not send
 */
//Queue<String> QameleO_SD::getDataToSend()
//{
//  if(this->wakeupSD()){
//    myFile = SD.open(FILE_NAME_BUFFER, FILE_READ);
//    myFile2 = SD.open(FILE_NAME_LOG, FILE_WRITE);
//    if (!myFile || !myFile2){
//      if(!myFile)
//        Serial.println("Error opening " + String(FILE_NAME_BUFFER));
//      if(!myFile2)
//        Serial.println("Error opening " + String(FILE_NAME_LOG));
//      Queue<String> empty;
//      return empty;
//    }else {
//      char reader[myFile.size()];
//      String oneLine;
//      Queue<String> dataToSend;
      /*int nbLine=0;*/
      
//      Serial.println("Reading to " + String(FILE_NAME_BUFFER) + " ... ");
//      int nbByteRead=myFile.read(reader, sizeof(reader)); //int read(void *buf, uint16_t nbyte);
//      if (nbByteRead != myFile.size())
//        Serial.println("Not all of the file has been read ");
        
      //Serial.println("int read(void *buf, uint16_t nbyte) est passé");
      //Serial.println("Sizeof reader : " + String(sizeof(reader)));
           
//      for(int i=0; i < sizeof(reader); i++){
//        Serial.println("i = " + String(i) + " reader[i] = " + reader[i]);
//        if (reader[i] == '\r' || reader[i] == '\n'){
//          if (reader[i] == '\n'){
            //Throw '\n'
//          }else{
            /*nbLine++;
            //Serial.println(oneLine);*/
//            myFile2.println(oneLine);
//            dataToSend.enqueue(oneLine);
//            oneLine="";
//          }
//        }else{
//          oneLine+=reader[i];
//        }
//      }
      //Serial.println("La boucle est passé");
      /*Serial.println("Nombre de ligne dans le fichier " + String(nbLine));*/
        
      // close the file:
//      myFile.close();
//      myFile2.close();
//      Serial.println("done.");

//      return dataToSend;
//    } 
//  }else{
//    Queue<String> empty;
//    return empty;
//  }
//}
Queue<String> QameleO_SD::getDataToSend()
{
  if(this->wakeupSD()){
    myFile = SD.open(FILE_NAME_BUFFER, FILE_READ);
    if(!myFile){
      Serial.println("Error opening " + String(FILE_NAME_BUFFER));
      
      Queue<String> empty;
      return empty;
    }else {
      char reader[myFile.size()];
      String oneLine;
      Queue<String> dataToSend;
      /*int nbLine=0;*/
      
      Serial.println("Reading to " + String(FILE_NAME_BUFFER) + " ... ");
      int nbByteRead=myFile.read(reader, sizeof(reader)); //int read(void *buf, uint16_t nbyte);
      if (nbByteRead != myFile.size())
        Serial.println("Not all of the file has been read ");
        
      //Serial.println("int read(void *buf, uint16_t nbyte) est passé");
      //Serial.println("Sizeof reader : " + String(sizeof(reader)));
           
      for(int i=0; i < sizeof(reader); i++){
        //Serial.println("i = " + String(i) + " reader[i] = " + reader[i]);
        if (reader[i] == '\r' || reader[i] == '\n'){
          if (reader[i] == '\n'){
            //Throw '\n'
          }else{
            /*nbLine++;
            //Serial.println(oneLine);*/
            dataToSend.enqueue(oneLine);
            oneLine="";
          }
        }else{
          oneLine+=reader[i];
        }
      }
      //Serial.println("La boucle est passé");
      /*Serial.println("Nombre de ligne dans le fichier " + String(nbLine));*/
        
      // close the file:
      myFile.close();
      Serial.println("done.");

      return dataToSend;
    } 
  }else{
    Queue<String> empty;
    return empty;
  }
}

/**
 * Remove the file use to save the data not send
 */
bool QameleO_SD::resetFileDataToSend()
{
  if(this->wakeupSD()){
    Serial.println("Supression du fichier " + String(FILE_NAME_BUFFER));
    return SD.remove(FILE_NAME_BUFFER); //If the file doesn't exists, that return false, but this methode is use to recreate the file. So, it's not too important if that return false
  }else {
    return false;
  }
}

/**
 * Read the file backup
 * 
 * @param idReboot  - the id of reboot 
 * @param nbRebootMax - the number of program restarts
 * @return the time millis() save on the file for the corresponding id
 */
long QameleO_SD::readBackup(int idReboot, int nbRebootMax)
{
  int a = nbRebootMax - idReboot;
  if(this->wakeupSD()){
    if(SD.exists(FILE_NAME_SAVE)){
      myFile = SD.open(FILE_NAME_SAVE, FILE_READ);
      if (!myFile){
        Serial.println("Error opening " + String(FILE_NAME_SAVE));
        return millis();
      }else {
        char reader[myFile.size()];
        String oneLine;
        int check=0;
        Pile<String> pileOfMillis;
        long oldMillis=0;

        //Serial.println("Reading to " + String(FILE_NAME_SAVE) + " ... ");
        int nbByteRead=myFile.read(reader, sizeof(reader)); 
        if (nbByteRead != myFile.size())
          Serial.println("Not all of the file has been read ");

        for(int i=0; i < sizeof(reader) && check<a; i++){
          if(reader[i]==';' || reader[i]=='\r' || reader[i]=='\n'){
            if(reader[i]=='\r' || reader[i]=='\n'){
              //Throw \r and \n
            }else {
              check++;
              pileOfMillis.push(oneLine);
              oneLine="";
            }
          }else{
            oneLine+=reader[i];
          }
        }
        while(!pileOfMillis.isEmpty()){
          oldMillis+=pileOfMillis.pop().toInt();
        }
        
        oldMillis+=millis();

        //Serial.println("Données millis reçu : " + oldMillis);
        return oldMillis;
      }
    }else{
      //Serial.println("There are not any file to read the backup");
      return millis();
    }
  }else{
    //If there are a problem with the sd card, that return millis()
    //Serial.println("Error with the sd card, can't open it");
    return millis();
  }
}


/**
 * Read on a file, and 
 * 
 * @return the number of reboot
 */
int QameleO_SD::readNbReboot()
{
  if(this->wakeupSD()){
    if(SD.exists(FILE_NAME_SAVE)){
      myFile = SD.open(FILE_NAME_SAVE, FILE_READ);
      if (!myFile){
        Serial.println("Error opening " + String(FILE_NAME_SAVE));
        return 0;
      }else {
        char reader[myFile.size()];
        int nbLine=0;

        //Serial.println("Reading to " + String(FILE_NB_REBOOT) + " ... ");
        int nbByteRead=myFile.read(reader, sizeof(reader)); 
        if (nbByteRead != myFile.size())
          Serial.println("Not all of the file has been read ");

        for(int i=0; i < sizeof(reader); i++){
          if(reader[i]=='\r')
            nbLine++;
        }
        Serial.println("Number of reboot read : " + String(nbLine));
        return nbLine;
      }
    }else {
      Serial.println("The are not any reboot, return the default value");
      return 0;
    }
  }else {
    Serial.println("SD access refused, return the default value");
    return 0;
  }
}


/**
 * Used before a reboot of the program. 
 * Call saveMillisInFile() //& saveNbRebootInFile(int n)
 */
void QameleO_SD::saveOnBackup(int n)
{ 
  this->saveMillisInFile();
}

/**
 * Call wakeupSD and saveDataInBufferFile
 * 
 * @param msg - The argument of saveDataInBufferFile
 */
void QameleO_SD::saveToSDBuffer(String msg)
{
  if(this->wakeupSD()) 
    this->saveDataInBufferFile(msg);
  
}

void QameleO_SD::saveToSDLog(String msg)
{
  if(this->wakeupSD()) 
    this->saveDataInLogFile(msg);
}

/**
 * Setup the pin of the SD card
 */
void QameleO_SD::setupSD()
{
  Serial.print("Initializing SD card... ");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  pinMode(SD_CONTROL_PIN, OUTPUT);
}

void QameleO_SD::temporaryMethod()
{
  if(this->wakeupSD()){
    Serial.println("Je supprime les fichiers : " + String(FILE_NAME_BUFFER) + ", " + String(FILE_NAME_SAVE));
    if(SD.exists(FILE_NAME_BUFFER))
      SD.remove(FILE_NAME_BUFFER);
      
    if(SD.exists(FILE_NAME_SAVE))
      SD.remove(FILE_NAME_SAVE);
  }
}

void QameleO_SD::temporaryMethod2()
{
  if(this->wakeupSD()){
    if (!SD.exists(FILE_NAME_SAVE)){
      Serial.println("The file " + String(FILE_NAME_SAVE) + " doesn't exist");
    }else {
      myFile = SD.open(FILE_NAME_SAVE, FILE_READ);
      if (myFile){
        Serial.println("Lecture du fichier " + String(FILE_NAME_SAVE));
        while(myFile.available())
          Serial.write(myFile.read());

        myFile.close();
      }else {
        Serial.println("Error opening file " + String(FILE_NAME_SAVE));
      }
    }
  }
}

void QameleO_SD::temporaryMethod3()
{
  if(this->wakeupSD()){
    if (!SD.exists(FILE_NAME_BUFFER)){
      Serial.println("The file " + String(FILE_NAME_BUFFER) + " doesn't exist");
    }else {
      myFile = SD.open(FILE_NAME_BUFFER, FILE_READ);
      if (myFile){
        Serial.println("Lecture du fichier " + String(FILE_NAME_BUFFER));
        while(myFile.available())
          Serial.write(myFile.read());

        myFile.close();
      }else {
        Serial.println("Error opening file " + String(FILE_NAME_BUFFER));
      }
    }
  }
}
