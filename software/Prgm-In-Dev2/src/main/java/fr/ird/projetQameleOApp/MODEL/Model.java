package fr.ird.projetQameleOApp.MODEL;

import java.io.*;
import com.fazecast.jSerialComm.*;

//Ces imports sont utilisés uniquement pour les méthodes de tests plus bas.
//----------------------------//
import org.usb4java.*;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.*;
//----------------------------//

public class Model
{

	/**
	 * Attribute to contain the file choose by the user
	 */ 
	private File fileSelected;

	/** 
	 * Attribute to contain the port choose by the user
	 */
	private SerialPort portSelected;	

	/**
	 * Maker of the class
	 */
	public Model(){
		
	}

	/** 
	 * Use to get the available ports 
	 *
	 * @return The available ports 
	 */
	public SerialPort[] getAvailablePorts()
	{
		return SerialPort.getCommPorts();
	}

	/** 
	 * Use to get the file selected by the user
	 *
	 * @return The file selected
	 */
	public File getFileSelected(){
		return this.fileSelected;
	}

	/** 
	 * Use to get the port selected by the user
	 *
	 * @return The port selected
	 */
	public SerialPort getPortSelected(){
		return this.portSelected;
	}

	/**
	 * Use to save the file choose by the user
	 *
	 * @param f - File to save on the model
	 */
	public void setFileSelected(File f){
		this.fileSelected = f;
		System.out.println(this.fileSelected);
		//System.out.println(this.fileSelected.getAbsolutePath()); //ça donne le chemin absolu. Ce que l'on vu
	}

	/**
	 * Use to know the port selected by the user
	 *
	 * @param portDescriptor - Port to save
	 */
	public void setPortSelected(String portDescriptor){
		portSelected = SerialPort.getCommPort(portDescriptor);
		//System.out.println("Port descriptor " + portDescriptor);
		//System.out.println("Port " + portSelected + " et le nom du port " + portSelected.getSystemPortName());
	}

	/**
	 * Use to uplaod the file selected on the sensor
	 *
	 * This method isn't complete. He miss the name of the command on line.
	 */
	public void uploadOnTheSensor(){
		String command = "<the name of command> " + "-v -patmega2560 -cwiring " + "-P" + portSelected.getSystemPortPath() + " -b115200 -D -Uflash:w:" + this.fileSelected.getAbsolutePath() + ":i ";

		//System.out.println(command);

		Runtime runtime = Runtime.getRuntime();
		Process process = null;
		try{
			process = runtime.exec(command);
		}catch(Exception err) {
			System.err.println("Error, catch the Runtime.exec problem");
		}
	}

 // Les méthodes qui suivent sont des méthodes qui ont été utilisé pour essayer de téléverser un programme ou de comprendre comment le faire avec les éléments qui était à disposition. 
 // Certaines de ces méthodes sont des exemples tirées de différents github pour les API : com.fazecast.jSerialComm et org.usb4java
 // 
 //---------------------------------------------------------------------------------//
	public void temporaryName(){
		String command = "avrdude -v -patmega2560 -cwiring -P/dev/ttyACM0 -b115200 -D -Uflash:w:/home/mouton/Documents/Dossier_Avrdude/QameleO.ino.mega.hex:i ";
		Runtime runtime = Runtime.getRuntime();
		Process process = null;
		try{
			process = runtime.exec(command);		//<----------- à décommenter
		}catch(Exception err) {
			System.err.println("Error catch");
		}
	}

	public String temporaryName2(){
		//Pour l'instant on va faire en sorte que ça affiche les ports sur la console
		SerialPort[] ports = SerialPort.getCommPorts();
		System.out.println("Port use : ");
		for (SerialPort port : ports)
			System.out.println(port.getSystemPortName());
		if (ports.length >= 1)
			return ports[0].getSystemPortName();
		else 
			return "null";
	}

	//Tentavive d'écrire sur la carte, mais je ne sais pas si c'est possible de juste écrire le fichier sur la carte et que ça fonctionne
	public void temporaryName3(){
		this.portSelected.setComPortTimeouts(SerialPort.TIMEOUT_WRITE_BLOCKING, 0, 0); //Cette ligne n'est peut être pas bonne ou peut être pas importante.
		//System.out.println("setBaudRate is success ? " + this.portSelected.setBaudRate(115200));
		if (this.portSelected.openPort()){
			System.out.println("J'ai ouvert le port");

			//Opening the file
			//--------------------------//
			BufferedReader reader = null;
			try{
				reader = new BufferedReader(new FileReader(this.fileSelected));
			}catch (FileNotFoundException errorOpen){
				System.err.println("Error to open the file");
			}
			System.out.println("J'ai ouvert le fichier");
			//--------------------------//

			//Reading the file & Write 
			//--------------------------//
			char[] bufReader = new char[2048];
			int nbMaxToRead = bufReader.length;
			int nbRead;
			try{
				/*try {Thread.sleep(100); } catch(Exception e) {}
				PrintWriter outputOfPort = new PrintWriter(this.portSelected.getOutputStream());*/
				//public int read(char[] cbuf,int off,int len)
				while( (nbRead=reader.read(bufReader, 0, nbMaxToRead)) > 0){
					/*for (int i=0; i<nbRead; i++) {
						outputOfPort.print(bufReader[i]);
					}*/
					byte[] bufWriter = String.copyValueOf(bufReader).getBytes();
					this.portSelected.writeBytes(bufWriter, nbRead);
					//System.out.println("Bytes en attentes d'écriture : " + this.portSelected.bytesAwaitingWrite​());
					//System.out.println("Une fois de plus dans la boucle");
				}
				//outputOfPort.flush();
				System.out.println("Je suis sorti de la boucle");
				System.out.println("resultat du flushIOBuffers : " + this.portSelected.flushIOBuffers());
				
			}catch (IOException errorRead){
				System.err.println("Error to read the file");

			}
			//--------------------------//
			


			//Closing the file
			//--------------------------//
			try{
				reader.close();
			}catch (IOException errorClose){
				System.err.println("Error to close the file");
			}
			System.out.println("J'ai fermé le fichier");
			//--------------------------//

			this.portSelected.closePort();
			System.out.println("J'ai fermé le port");
		}else {
			System.err.println("Error to open the port");
		}
	}

	//Ici on test de bien lire dans le fichier
	public void temporaryName4(){	
		//Opening the file
		//--------------------------//
		BufferedReader reader = null;
		try{
			reader = new BufferedReader(new FileReader(this.fileSelected));
		}catch (FileNotFoundException errorOpen){
			System.err.println("Error to open the file");
		}
		System.out.println("J'ai ouvert le fichier");
		//--------------------------//

		//Reading the file
		//--------------------------//
		char[] bufReader = new char[2048];
		int nbMaxToRead = bufReader.length;
		int nbRead;
		try{
			//public int read(char[] cbuf,int off,int len)
			while( (nbRead=reader.read(bufReader, 0, nbMaxToRead)) > 0){
				for (int i=0; i<nbRead; i++)
					System.out.print(bufReader[i]);
			}
			System.out.println("Je suis sorti de la boucle");
			
		}catch (IOException errorRead){
			System.err.println("Error to read the file");

		}
		//--------------------------//
		


		//Closing the file
		//--------------------------//
		try{
			reader.close();
		}catch (IOException errorClose){
			System.err.println("Error to close the file");
		}
		System.out.println("J'ai fermé le fichier");
		//--------------------------//
	}

	/*
	public void noneSense(){
		PrintWriter output = new PrintWriter(this.portSelected.getOutputStream());
		for (int i=0; i<nbRead; i++) {
			output.print(bufReader[i]);
		}
	}
	*/

	//Look the baud rate of the serial port
	public void temporaryName5(){
		System.out.println("Baud rate of the serial port : " + this.portSelected.getBaudRate());
	}

	//Look DTR and RTS before and after the opening port
	public void temporaryName6(){
		System.out.println("What say getDTR ? " + this.portSelected.getDTR());
		System.out.println("What say getRTS ? " + this.portSelected.getRTS());

		System.out.println("Opening the port");
		if (this.portSelected.openPort()){
			System.out.println("What say getDTR with an open port ? " + this.portSelected.getDTR());
			System.out.println("What say getRTS with an open port ? " + this.portSelected.getRTS());
		}
	}

	//Pour essayer d'écrire sur le capteur, en envoyant des bytes pour le passer en mode écriture.
	public void temporaryName7(){
		this.portSelected.setComPortTimeouts(SerialPort.TIMEOUT_WRITE_BLOCKING, 0, 0); //Cette ligne n'est peut être pas bonne ou peut être pas importante.
		//System.out.println("setBaudRate is success ? " + this.portSelected.setBaudRate(115200));
		if (this.portSelected.openPort()){
			System.out.println("J'ai ouvert le port");

			//Opening the file
			//--------------------------//
			BufferedReader reader = null;
			try{
				reader = new BufferedReader(new FileReader(this.fileSelected));

				System.out.println("J'ai ouvert le fichier");
			}catch (FileNotFoundException errorOpen){
				System.err.println("Error to open the file");
			}
			//--------------------------//

			//Reading the file & Write 
			//--------------------------//
			char[] bufReader = new char[2048];
			int nbMaxToRead = bufReader.length;
			int nbRead;
			try{
				byte[] changeModeWrite = {0,0,0,1,0,0,0,0};
				this.portSelected.writeBytes(changeModeWrite, 8);

				while( (nbRead=reader.read(bufReader, 0, nbMaxToRead)) > 0){
					byte[] bufWriter = String.copyValueOf(bufReader).getBytes();
					this.portSelected.writeBytes(bufWriter, nbRead);
				}
				System.out.println("Je suis sorti de la boucle");
				//System.out.println("resultat du flushIOBuffers : " + this.portSelected.flushIOBuffers());

				byte[] changeModeNeutral = {0,0,0,0,0,0,0,0};
				this.portSelected.writeBytes(changeModeNeutral, 8);
				
			}catch (IOException errorRead){
				System.err.println("Error to read the file");

			}
			//--------------------------//
			


			//Closing the file
			//--------------------------//
			try{
				reader.close();
			}catch (IOException errorClose){
				System.err.println("Error to close the file");
			}
			System.out.println("J'ai fermé le fichier");
			//--------------------------//

			this.portSelected.closePort();
			System.out.println("J'ai fermé le port");
		}else {
			System.err.println("Error to open the port");
		}
	}

	//Test de l'API usb4java
	public void temporaryName8(){

		Context context = new Context();
		int result = LibUsb.init(context);
		if (result != LibUsb.SUCCESS) throw new LibUsbException("Unable to initialize libusb.", result);

		Device myCard = this.findDeviceSelected(context);
		//this.dumpDevice(myCard);

		DeviceHandle handle = new DeviceHandle();
		result = LibUsb.open(myCard, handle);
		if (result != LibUsb.SUCCESS) {
			System.out.println(String.format("Unable to open device: %s. ", LibUsb.strError(result)));
			handle=null;
		} else {
			//System.out.println("Je rentre, SUCCESS open");
			try{
	    		ByteBuffer buffer = ByteBuffer.allocateDirect(8);
				buffer.put(new byte[] { 0, 0, 0, 1, 0, 0, 0, 0 });
				
				IntBuffer transfered =  IntBuffer.allocate(1);

				//result = LibUsb.bulkTransfer(handle,/*byte endpoint, exemple : 0x03 */ , buffer, transfered, 5000);
				//if (result != LibUsb.SUCCESS) throw new LibUsbException("Control transfer failed", transfered);
				System.out.println(transfered.get() + " bytes sent");
			} finally {
				if (handle!=null)
	    			LibUsb.close(handle);
			}	
		}


		LibUsb.exit(context);
	}

	public void temporaryName8bis(){
		Context context = new Context();
		int result = LibUsb.init(context);
		if (result != LibUsb.SUCCESS) throw new LibUsbException("Unable to initialize libusb.", result);

		Device myCard = this.findDeviceSelected(context);

		//Si le problème viens de là, il faudra mettre ce bout de code autre part et réfléchir pour mieux organiser les choses.
		//-------------------------//
		this.portSelected = null;
		//-------------------------//


		DeviceHandle handle = new DeviceHandle();
		result = LibUsb.open(myCard, handle);
		if (result != LibUsb.SUCCESS) {
			System.out.println(String.format("Unable to open device: %s. ", LibUsb.strError(result)));
			handle=null;
		} else {
				byte interfaceNumber = 1;
			try {
				//boolean detach = LibUsb.hasCapability(LibUsb.CAP_SUPPORTS_DETACH_KERNEL_DRIVER) && ((boolean)LibUsb.kernelDriverActive(handle, interfaceNumber));
				/*boolean detach = false;
				System.out.println("1 : " + LibUsb.hasCapability(LibUsb.CAP_SUPPORTS_DETACH_KERNEL_DRIVER));
				System.out.println("2 : " + LibUsb.kernelDriverActive(handle, interfaceNumber));*/
				// Detach the kernel driver
				/*if (detach){
    				result = LibUsb.detachKernelDriver(handle,  interfaceNumber);
    				if (result != LibUsb.SUCCESS) throw new LibUsbException("Unable to detach kernel driver", result);
				}*/
				// Communicate with the device
				result = LibUsb.claimInterface(handle, interfaceNumber);
      		if (result != LibUsb.SUCCESS){
         		System.out.println(String.format("Unable to claim interface: %s. ", LibUsb.strError(result)));
      		} else {
	         	try{
	         		//Write here the upload

	         	} finally {
	         		result = LibUsb.releaseInterface(handle, interfaceNumber);
	    				if (result != LibUsb.SUCCESS) throw new LibUsbException("Unable to release interface", result);
	         	}
      		}
      		// Attach the kernel driver again if needed
				/*if (detach){
    				result = LibUsb.attachKernelDriver(handle,  interfaceNumber);
    				if (result != LibUsb.SUCCESS) throw new LibUsbException("Unable to re-attach kernel driver", result);
				}*/
			} finally {
				if (handle!=null)
	    			LibUsb.close(handle);
			}
		}

		LibUsb.exit(context); 
	}

	public void temporaryName9(){
		Context context = new Context();
		int result = LibUsb.init(context);
		if (result != LibUsb.SUCCESS) throw new LibUsbException("Unable to initialize libusb.", result);

		Device myCard = this.findDeviceSelected(context);
		this.dumpDevice(myCard);

		LibUsb.exit(context);
	}

	public void temporaryName10(){
		Context context = new Context();
		int result = LibUsb.init(context);
		if (result != LibUsb.SUCCESS) throw new LibUsbException("Unable to initialize libusb.", result);

		Device myCard = this.findDeviceSelected(context);
		this.dumpConfigurationDescriptors2(myCard);

		LibUsb.exit(context);
	}

	public void temporaryName11(){
		Context context = new Context();
		int result = LibUsb.init(context);
		if (result != LibUsb.SUCCESS) throw new LibUsbException("Unable to initialize libusb.", result);

		this.seeAllDevice(context);

		LibUsb.exit(context);
	}

	public void temporaryName12(){
		int result = LibUsb.init(null);
		if (result != LibUsb.SUCCESS) throw new LibUsbException("Unable to initialize libusb.", result);

		Device myCard = this.findDevice((short) 0x2a03, (short) 0x0043);
		if (myCard!=null)
			System.out.println("Device found");

		DeviceHandle handle = new DeviceHandle();
		result = LibUsb.open(myCard, handle);
		if (result != LibUsb.SUCCESS) {
			System.out.println(String.format("Unable to open device: %s. ", LibUsb.strError(result)));
			handle=null;
		} else {
				byte interfaceNumber = 1;
			try {
				//boolean detach = LibUsb.hasCapability(LibUsb.CAP_SUPPORTS_DETACH_KERNEL_DRIVER) && ((boolean)LibUsb.kernelDriverActive(handle, interfaceNumber));
				boolean detach = true;
				System.out.println("1 : " + LibUsb.hasCapability(LibUsb.CAP_SUPPORTS_DETACH_KERNEL_DRIVER));
				System.out.println("2 : " + LibUsb.kernelDriverActive(handle, interfaceNumber));
				// Detach the kernel driver
				if (detach){
    				result = LibUsb.detachKernelDriver(handle,  interfaceNumber);
    				if (result != LibUsb.SUCCESS) throw new LibUsbException("Unable to detach kernel driver", result);
				}
				// Communicate with the device
				result = LibUsb.claimInterface(handle, interfaceNumber);
      		if (result != LibUsb.SUCCESS){
         		System.out.println(String.format("Unable to claim interface: %s. ", LibUsb.strError(result)));
      		} else {
	         	try{
	         		//Write here the upload
	         		System.out.println("I trying to upload");
	         		ByteBuffer bufferWrite = ByteBuffer.allocateDirect(8);
						bufferWrite.put(new byte[] { 0, 0, 0, 1, 0, 0, 0, 0 });
				
						IntBuffer transfered1 =  IntBuffer.allocate(1);

						result = LibUsb.bulkTransfer(handle, (byte)0x04, bufferWrite, transfered1, 5000);
						if (result != LibUsb.SUCCESS) throw new LibUsbException("Control transfer failed", transfered1.get());
						System.out.println(transfered1.get() + " bytes sent");

						//Opening the file
						//--------------------------//
						BufferedReader reader = null;
						try{
							reader = new BufferedReader(new FileReader(this.fileSelected));

							System.out.println("J'ai ouvert le fichier");
						}catch (FileNotFoundException errorOpen){
							System.err.println("Error to open the file");
						}
						//--------------------------//

						//Reading the file & Write 
						//--------------------------//
						char[] bufReader = new char[1024];
						int nbMaxToRead = bufReader.length;
						int nbRead;
						try{
							while( (nbRead=reader.read(bufReader, 0, nbMaxToRead)) > 0){
								byte[] bufWriter = String.copyValueOf(bufReader).getBytes();
								ByteBuffer bufferUpload = ByteBuffer.allocateDirect(bufReader.length);
								bufferUpload.put(bufWriter);
								IntBuffer inTransfer =  IntBuffer.allocate(4);

								result = LibUsb.bulkTransfer(handle, (byte)0x04, bufferUpload, inTransfer, 5000);
								if (result != LibUsb.SUCCESS) throw new LibUsbException("Control transfer failed", inTransfer.get());
								System.out.println(inTransfer.get() + " bytes sent");
							}
							System.out.println("Je suis sorti de la boucle");
							
						}catch (IOException errorRead){
							System.err.println("Error to read the file");

						}
						//--------------------------//


						//Closing the file
						//--------------------------//
						try{
							reader.close();
						}catch (IOException errorClose){
							System.err.println("Error to close the file");
						}
						System.out.println("J'ai fermé le fichier");
						//--------------------------//

						ByteBuffer bufferNeutral = ByteBuffer.allocateDirect(8);
						bufferNeutral.put(new byte[] { 0, 0, 0, 0, 0, 0, 0, 0 });
						IntBuffer transfered2 =  IntBuffer.allocate(1);

						result = LibUsb.bulkTransfer(handle, (byte)0x04, bufferNeutral, transfered2, 5000);
						if (result != LibUsb.SUCCESS) throw new LibUsbException("Control transfer failed", transfered2.get());
						System.out.println(transfered2.get() + " bytes sent");

	         	} finally {
	         		result = LibUsb.releaseInterface(handle, interfaceNumber);
	    				if (result != LibUsb.SUCCESS) throw new LibUsbException("Unable to release interface", result);
	         	}
      		}
      		// Attach the kernel driver again if needed
				if (detach){
    				result = LibUsb.attachKernelDriver(handle,  interfaceNumber);
    				if (result != LibUsb.SUCCESS)
    					System.out.println(String.format("Unable to re-attach kernel driver: %s. ", LibUsb.strError(result)));
				}
			} finally {
				if (handle!=null)
	    			LibUsb.close(handle);
			}
		}

		LibUsb.exit(null);
	}


	//Exemple donné sur le site pour trouver un appareil usb
	public Device findDevice(short vendorId, short productId)
	{
   	// Read the USB device list
   	DeviceList list = new DeviceList();
   	int result = LibUsb.getDeviceList(null, list);
    	if (result < 0) throw new LibUsbException("Unable to get device list", result);

   	try
   	{
      	// Iterate over all devices and scan for the right one
      	for (Device device: list)
      	{
         	DeviceDescriptor descriptor = new DeviceDescriptor();
            result = LibUsb.getDeviceDescriptor(device, descriptor);
            if (result != LibUsb.SUCCESS) throw new LibUsbException("Unable to read device descriptor", result);
            if (descriptor.idVendor() == vendorId && descriptor.idProduct() == productId) return device;
        }
   	}
   	finally
   	{
      	// Ensure the allocated device list is freed
      	LibUsb.freeDeviceList(list, true);
   	}

   	// Device not found
   	return null;
	}


	//Liste de tous les appareils usb connectés.
	private void seeAllDevice(Context ctxt){
		DeviceList list = new DeviceList();
   	int result = LibUsb.getDeviceList(ctxt, list);
    	if (result < 0) throw new LibUsbException("Unable to get device list", result);

    	try
   	{
      	// Iterate over all devices and scan for the right one
      	for (Device device: list)
      	{
         	DeviceDescriptor descriptor = new DeviceDescriptor();
            result = LibUsb.getDeviceDescriptor(device, descriptor);
            if (result != LibUsb.SUCCESS) throw new LibUsbException("Unable to read device descriptor", result);

            /*if (descriptor.iSerialNumber()==-36) { //en byte -36=220, ce qui est égale au iSerial du carte Arduino. (du moins c'est que je crois)
            	System.out.println("Port number ? " + LibUsb.getPortNumber(device));
            	System.out.println("Bus number ? " + LibUsb.getBusNumber(device));
            	System.out.println("Device address ? " + LibUsb.getDeviceAddress(device));
            	System.out.println(descriptor.dump());
            	//System.out.println(DescriptorUtils.dump(descriptor));
            	/*DeviceHandle handle = new DeviceHandle();
            	handle = LibUsb.openDeviceWithVidPid(context, descriptor.idVendor(), descriptor.idProduct());

            	System.out.println(LibUsb.getStringDescriptor(handle, byte index)); 
            }*/

            if (descriptor.bDeviceClass()==LibUsb.CLASS_COMM){
            	System.out.println(descriptor.dump());
            }
        }
   	}
   	finally
   	{
      	// Ensure the allocated device list is freed
      	LibUsb.freeDeviceList(list, true);
   	}
	}

	//utilisé pour trouver l'appareil séléctionner avec l'interface qui est avec l'API JComm et "transfere" à l'API usb4java(libusb)
	private Device findDeviceSelected(Context ctxt){
		DeviceList list = new DeviceList();
   	int result = LibUsb.getDeviceList(ctxt, list);
    	if (result < 0) throw new LibUsbException("Unable to get device list", result);

    	try
   	{
      	// Iterate over all devices and scan for the right one
      	for (Device device: list)
      	{
         	//DeviceDescriptor descriptor = new DeviceDescriptor();
            //result = LibUsb.getDeviceDescriptor(device, descriptor);
            //if (result != LibUsb.SUCCESS) throw new LibUsbException("Unable to read device descriptor", result);
            if ( LibUsb.getPortNumber(device) == Integer.parseInt(this.portSelected.getPortLocation().substring(this.portSelected.getPortLocation().indexOf("-") + 1)) ) //voir reformPortLocation
            	return device;
        }
   	}
   	finally
   	{
      	// Ensure the allocated device list is freed
      	LibUsb.freeDeviceList(list, true);
   	}

   	// Device not found
   	return null;

	} 

	//Test ici différentes méthodes de l'API JComm
	public void checkingSerialPort(){

		// getPortDescription() donne la description de ce qui est branché au bout
		//-----------------------------//
		System.out.println("Port description ? ");
		System.out.println(this.portSelected.getPortDescription());
		//-----------------------------//

		// getPortLocation(), résultat retourné : 1-5
		// 1- : je ne sais ce que ça représente. Peut être juste pour avoir une mise en forme dans le string.
		// 5 : le numéro "physique" utilisé par l'appareil.
		//-----------------------------//
		System.out.println("Port location ? ");
		System.out.println(this.portSelected.getPortLocation());
		//-----------------------------//

		// getSystemPortName(), le nom du port utilisé par la machine pour communiquer avec la carte Arduino ?
		//-----------------------------//
		System.out.println("System port name ? ");
		System.out.println(this.portSelected.getSystemPortName());
		//-----------------------------//

		// getSystemPortPath(), ça doit être le chemin absolu du port utilisé.
		//-----------------------------//
		System.out.println("System port path ? ");
		System.out.println(this.portSelected.getSystemPortPath());
		//-----------------------------//
	}

	//Enlève le surplus du string renvoyé par la méthode getPortLocation()
	public void reformPortLocation(){
		//int index = this.portSelected.getPortLocation().indexOf("-") + 1 ;
		System.out.println(this.portSelected.getPortLocation().substring(this.portSelected.getPortLocation().indexOf("-") + 1));
		//int numberPort = Integer.parseInt(this.portSelected.getPortLocation().substring(this.portSelected.getPortLocation().indexOf("-") + 1));
	}

	public void listDevices() {
      // Create the libusb context
      Context context = new Context();

      // Initialize the libusb context
      int result = LibUsb.init(context);
      if (result < 0)
         throw new LibUsbException("Unable to initialize libusb", result);

      // Read the USB device list
      DeviceList list = new DeviceList();
      result = LibUsb.getDeviceList(context, list);
      if (result < 0)
         throw new LibUsbException("Unable to get device list", result);

      try{
         // Iterate over all devices and list them
         for (Device device: list){
            int address = LibUsb.getDeviceAddress(device);
            int busNumber = LibUsb.getBusNumber(device);
            DeviceDescriptor descriptor = new DeviceDescriptor();
            result = LibUsb.getDeviceDescriptor(device, descriptor);
            if (result < 0)
               throw new LibUsbException("Unable to read device descriptor", result);
            System.out.format("Bus %03d, Device %03d: Vendor %04x, Product %04x%n", busNumber, address, descriptor.idVendor(), descriptor.idProduct());
            }
      } finally{
         // Ensure the allocated device list is freed
         LibUsb.freeDeviceList(list, true);
      }

      // Deinitialize the libusb context
      LibUsb.exit(context);
   }

   /**
    * Dumps all configuration descriptors of the specified device. Because
    * libusb descriptors are connected to each other (Configuration descriptor
    * references interface descriptors which reference endpoint descriptors)
    * dumping a configuration descriptor also dumps all interface and endpoint
    * descriptors in this configuration.
    * 
    * @param device
    *            The USB device.
    * @param numConfigurations
    *            The number of configurations to dump (Read from the device
    *            descriptor)
    */
   public static void dumpConfigurationDescriptors(final Device device, final int numConfigurations){
   	for (byte i = 0; i < numConfigurations; i += 1){
         final ConfigDescriptor descriptor = new ConfigDescriptor();
         final int result = LibUsb.getConfigDescriptor(device, i, descriptor);
         System.out.println("i = " + i);
         if (result < 0)
            throw new LibUsbException("Unable to read config descriptor",result);
         try {
            System.out.println(descriptor.dump().replaceAll("(?m)^","  "));
         } finally {
            // Ensure that the config descriptor is freed
            LibUsb.freeConfigDescriptor(descriptor);
         }
      }
   }
   public static void dumpConfigurationDescriptors2(final Device device){
      final ConfigDescriptor descriptor = new ConfigDescriptor();
      byte i = 0;
      //final int result = LibUsb.getConfigDescriptor(device, i, descriptor);
      int result = LibUsb.getActiveConfigDescriptor(device, descriptor);
      if (result < 0)
         throw new LibUsbException("Unable to read config descriptor",result);
      try {
         //System.out.println(descriptor.dump().replaceAll("(?m)^","  "));
         System.out.println(descriptor.dump());
         //System.out.println("Nombre d'interface : " + descriptor.bNumInterfaces());
      } finally {
         // Ensure that the config descriptor is freed
         LibUsb.freeConfigDescriptor(descriptor);
      }
   }

   /**
    * Dumps the specified device to stdout.
    * 
    * @param device
    *            The device to dump.
    */
   public static void dumpDevice(final Device device){
      // Dump device address and bus number
      final int address = LibUsb.getDeviceAddress(device);
      final int busNumber = LibUsb.getBusNumber(device);
      System.out.println(String.format("Device %03d/%03d", busNumber, address));

      // Dump port number if available
      final int portNumber = LibUsb.getPortNumber(device);
      if (portNumber != 0)
         System.out.println("Connected to port: " + portNumber);

      // Dump parent device if available
      final Device parent = LibUsb.getParent(device);
      if (parent != null){
         final int parentAddress = LibUsb.getDeviceAddress(parent);
         final int parentBusNumber = LibUsb.getBusNumber(parent);
         System.out.println(String.format("Parent: %03d/%03d", parentBusNumber, parentAddress));
      }

      // Dump the device speed
      System.out.println("Speed: " + DescriptorUtils.getSpeedName(LibUsb.getDeviceSpeed(device)));

      // Read the device descriptor
      final DeviceDescriptor descriptor = new DeviceDescriptor();
      int result = LibUsb.getDeviceDescriptor(device, descriptor);
      if (result < 0)
         throw new LibUsbException("Unable to read device descriptor",result);

      // Try to open the device. This may fail because user has no
      // permission to communicate with the device. This is not
      // important for the dumps, we are just not able to resolve string
      // descriptor numbers to strings in the descriptor dumps.
      DeviceHandle handle = new DeviceHandle();
      result = LibUsb.open(device, handle);
      if (result < 0) {
         System.out.println(String.format("Unable to open device: %s. " + "Continuing without device handle.", LibUsb.strError(result)));
         handle = null;
      }

      // Dump the device descriptor
      System.out.print(descriptor.dump(handle));

      // Dump all configuration descriptors
      //dumpConfigurationDescriptors(device, descriptor.bNumConfigurations());

      dumpConfigurationDescriptors2(device);

      // Close the device if it was opened
      if (handle != null)
            LibUsb.close(handle);
   }

   /**
    * Main method.
    * 
    * @param args
    *            Command-line arguments (Ignored)
    */
   public static void dumpDevices() {
      // Create the libusb context
      final Context context = new Context();

      // Initialize the libusb context
      int result = LibUsb.init(context);
      if (result < 0)
         throw new LibUsbException("Unable to initialize libusb", result);

      // Read the USB device list
      final DeviceList list = new DeviceList();
      result = LibUsb.getDeviceList(context, list);
      if (result < 0)
         throw new LibUsbException("Unable to get device list", result);

      try {
         // Iterate over all devices and dump them
         for (Device device: list) {
            dumpDevice(device);
         }
      } finally {
         // Ensure the allocated device list is freed
         LibUsb.freeDeviceList(list, true);
      }
        
      // Deinitialize the libusb context
      LibUsb.exit(context);
   }


   /**
 	 * Demonstrates how to do synchronous bulk transfers. This demo sends some
 	 * hardcoded data to an Android Device (Samsung Galaxy Nexus) and receives some
 	 * data from it.
 	 * 
 	 * If you have a different Android device then you can get this demo working by
 	 * changing the vendor/product id, the interface number and the endpoint
 	 * addresses.
 	 * 
 	 * @author Klaus Reimer <k@ailis.de>
 	 */

   /** Bytes for a CONNECT ADB message header. */
   private static final byte[] CONNECT_HEADER = new byte[] { 0x43, 0x4E, 0x58, 0x4E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x42, 0x06, 0x00, 0x00, (byte) 0xBC, (byte) 0xB1, (byte) 0xA7, (byte) 0xB1 };

   /** Bytes for a CONNECT ADB message body. */
   private static final byte[] CONNECT_BODY = new byte[] { 0x68, 0x6F, 0x73, 0x74, 0x3A, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x3A, 0x41, 0x44, 0x42, 0x20, 0x44, 0x65, 0x6D, 0x6F, 0x00 };

   /** The vendor ID of the Samsung Galaxy Nexus. */
   private static final short VENDOR_ID = 0x04e8;

   /** The vendor ID of the Samsung Galaxy Nexus. */
   private static final short PRODUCT_ID = 0x6860;

   /** The ADB interface number of the Samsung Galaxy Nexus. */
   private static final byte INTERFACE = 1;

   /** The ADB input endpoint of the Samsung Galaxy Nexus. */
   private static final byte IN_ENDPOINT = (byte) 0x83;

   /** The ADB output endpoint of the Samsung Galaxy Nexus. */
   private static final byte OUT_ENDPOINT = 0x03;

   /** The communication timeout in milliseconds. */
   private static final int TIMEOUT = 5000;

   /**
    * Writes some data to the device.
    * 
    * @param handle
    *            The device handle.
    * @param data
    *            The data to send to the device.
    */
   public static void write(DeviceHandle handle, byte[] data)
   {
      ByteBuffer buffer = BufferUtils.allocateByteBuffer(data.length);
      buffer.put(data);
      IntBuffer transferred = BufferUtils.allocateIntBuffer();
      int result = LibUsb.bulkTransfer(handle, OUT_ENDPOINT, buffer,transferred, TIMEOUT);
      if (result != LibUsb.SUCCESS)
      {
         throw new LibUsbException("Unable to send data", result);
      }
      System.out.println(transferred.get() + " bytes sent to device");
   }

   /**
    * Reads some data from the device.
    * 
    * @param handle
    *            The device handle.
    * @param size
    *            The number of bytes to read from the device.
    * @return The read data.
    */
   public static ByteBuffer read(DeviceHandle handle, int size)
   {
      ByteBuffer buffer = BufferUtils.allocateByteBuffer(size).order(ByteOrder.LITTLE_ENDIAN);
      IntBuffer transferred = BufferUtils.allocateIntBuffer();
      int result = LibUsb.bulkTransfer(handle, IN_ENDPOINT, buffer,transferred, TIMEOUT);
      if (result != LibUsb.SUCCESS)
      {
         throw new LibUsbException("Unable to read data", result);
      }
      System.out.println(transferred.get() + " bytes read from device");
      return buffer;
   }

   /**
    * Main method.
    * 
    * @param args
    *            Command-line arguments (Ignored)
    * @throws Exception
    *             When something goes wrong.
    */
   public static void syncBulkTransfer()
   {
      // Initialize the libusb context
      int result = LibUsb.init(null);
      if (result != LibUsb.SUCCESS)
      {
   	   throw new LibUsbException("Unable to initialize libusb", result);
      }

      // Open test device (Samsung Galaxy Nexus)
      DeviceHandle handle = LibUsb.openDeviceWithVidPid(null, VENDOR_ID, PRODUCT_ID);
      if (handle == null)
      {
         System.err.println("Test device not found.");
         System.exit(1);
      }

      // Claim the ADB interface
      result = LibUsb.claimInterface(handle, INTERFACE);
      if (result != LibUsb.SUCCESS)
      {
         throw new LibUsbException("Unable to claim interface", result);
      }

      // Send ADB CONNECT message
      write(handle, CONNECT_HEADER);
      write(handle, CONNECT_BODY);

      // Receive the header of the ADB answer (Most likely an AUTH message)
      ByteBuffer header = read(handle, 24);
      header.position(12);
      int dataSize = header.asIntBuffer().get();

      // Receive the body of the ADB answer
      @SuppressWarnings("unused")
      ByteBuffer data = read(handle, dataSize);

      // Release the ADB interface
      result = LibUsb.releaseInterface(handle, INTERFACE);
      if (result != LibUsb.SUCCESS)
      {
         throw new LibUsbException("Unable to release interface", result);
      }

      // Close the device
      LibUsb.close(handle);

      // Deinitialize the libusb context
      LibUsb.exit(null);
   }
 //---------------------------------------------------------------------------------//

}