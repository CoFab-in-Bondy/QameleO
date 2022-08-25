package fr.ird.projetQameleOApp.VIEW;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener; 
import java.io.*;
import javax.swing.filechooser.*;
import com.fazecast.jSerialComm.*;

/**
 * The <code>MenuView</code> class creates the menu / first page of the app
 *
 * @version 1.1
 * @author Romain SAVARY
 */
public class MenuView extends JPanel
{	
	/**
	 * Attribute to contain the button choose a file
	 */
	private JButton chooseFile;

	/**
	 * Attribute to contain the button to upload a file on the sensor
	 */
	private JButton upload;

	/**
	 * JButton used to exit the application
	 */
	private JButton exit;

	/**
	 * JButton used to refresh the port connected to the computer
	 */ 
	private JButton refresh;

	/**
	 * Attribute to contain the name of the application
	 */
	private JLabel nameApplication;

	/**
	 * Attribute to contain the different error messages
	 */
	private JLabel errorMessage;

	/**
	 * Attribute to contain the instruction to select a file
	 */
	private JLabel instruction1;

	/**
	 * Attribute to contain that file has been chosen 
	 */
	private JLabel fileChosen;

	/**
	 * Attribute to contain the instruction to select a port
	 */
	private JLabel instruction2;

	/**
	 * Attribute to contain the JLabel who valid if a file is selected 
	 */
	//private JLabel validCondition1; //Peut être implémenter plus tard

	/**
	 * Attribute to contain the JLabel who valid if a port is selected 
	 */
	//private JLabel validCondition2; //Peut être implémenter plus tard

	/**
	 * Attribute to contain the all port connected to the computer
	 */
	private JComboBox<String> listOfPort; 

	/**
	 * Attribute to contain the font of the JLabel
	 */
	private Font fontInstruction;

	/**
	 * Attribute to contain the font of the JButton
	 */
	private Font fontButton;

	/**
	 * Attribute to contain the tick icon
	 */
	//private ImageIcon tickIcon; //Peut être implémenter plus tard

	/**
	 * Maker the view with the different attributes 
	 */
	public MenuView(){
		this.setLayout(null);

		//Font & Icon
		//--------------------------//
		this.fontInstruction = new Font("Times New Roman", Font.PLAIN, 20);
		this.fontButton = new Font("Times New Roman", Font.PLAIN, 17);

		//this.tickIcon = new ImageIcon(this.getClass().getResource("/green_tick_32px.png")); //Peut être implémenter plus tard
		//--------------------------//

		//Button to choose a file
		//--------------------------//
		this.chooseFile = new JButton("Select a file");
		this.chooseFile.setFont(this.fontButton);
		this.chooseFile.setBounds(385,164,137,46);
		this.add(this.chooseFile);
		//--------------------------//

		//Button to upload a file on the sensor
		//--------------------------//
		this.upload = new JButton("Upload on the sensor");
		this.upload.setFont(this.fontButton);
		this.upload.setBounds(370,480,210,61);
		this.add(this.upload);
		//--------------------------//

		//Button to exit the app
		//--------------------------//
		this.exit = new JButton("Exit");
		this.exit.setFont(this.fontButton);
		this.exit.setBounds(817,23,85,46);
		this.add(this.exit);
		//--------------------------//

		//Refresh part
		//--------------------------//
		this.refresh = new JButton(new ImageIcon(this.getClass().getResource("/refresh_icon_32px.png")));
		this.refresh.setBounds(785,346,30,30);
		this.add(this.refresh);
		//--------------------------//

		//Title
		//--------------------------//
		this.nameApplication = new JLabel("Projet A");
		this.nameApplication.setFont(new Font("Times New Roman", Font.PLAIN, 25));
		this.nameApplication.setBounds(40,32,200,28);
		this.add(this.nameApplication);
		//--------------------------//

		//Instruction to select a file
		//--------------------------//
		this.instruction1 = new JLabel("Please choose your file to upload :");
		this.instruction1.setFont(this.fontInstruction);
		this.instruction1.setBounds(25,173,370,28);
		this.add(this.instruction1);
		//--------------------------//

		//Message with the file chosen. Here we just define what is fileChosen, the message of this JLabel is defined further
		//--------------------------//
		this.fileChosen = new JLabel(); //"The file selected : /home/mouton/Documents/toto.ino.mega.hex"
		this.fileChosen.setFont(this.fontInstruction);
		this.fileChosen.setBounds(25,225,710,28);
		this.add(this.fileChosen);
		//--------------------------//

		//Instruction to select a port
		//--------------------------//
		this.instruction2 = new JLabel("Please choose your port who will use to upload the file :");
		this.instruction2.setFont(this.fontInstruction);
		this.instruction2.setBounds(25,347,650,28);
		this.add(this.instruction2);
		//--------------------------//

		//JComboBox to choose and see all available ports. Here we just define what is listOfPort, the list is defined further
		//--------------------------//
		this.listOfPort = new JComboBox<>();
		this.listOfPort.setFont(new Font("Times New Roman", Font.PLAIN, 16));
		this.listOfPort.setBounds(600,346,178,30);
		this.add(this.listOfPort);
		//--------------------------//

		//Error message
		//--------------------------//
		this.errorMessage = new JLabel(); //"Error - No file or no port selected"
		this.errorMessage.setFont(this.fontInstruction);
		this.errorMessage.setForeground(Color.RED);	
		this.errorMessage.setBounds(280,445,400,28);
		this.add(this.errorMessage);
		//--------------------------//

		/* Peut être implémenté plus tard
		//Valid condition to upload
		//--------------------------//
		this.validCondition1 = new JLabel(); 
		this.validCondition1.setBounds(535,164,40,40);
		this.add(this.validCondition1);

		this.validCondition2 = new JLabel();
		this.validCondition2.setBounds(830,340,40,40);
		this.add(this.validCondition2);
		//--------------------------// */
		
	}

	/**
	 * Use to get the button who choose a file
	 *
	 * @return the button to choose a file
	 */
	public JButton getChooseFileButton(){
		return this.chooseFile;
	}

	/**
	 * Use to get the button who exit the program
	 *
	 * @return the button to exit the program
	 */
	public JButton getExitButton(){
		return this.exit;
	}

	/** 
	 * Use to know that is the port chosen
	 *
	 * @return the string of the name port selected
	 */
	public String getPortChosen(){
		//System.out.println(this.listOfPort.getSelectedItem().toString());
		if(this.listOfPort.getSelectedItem()==null)
			return "";
		else 
			return this.listOfPort.getSelectedItem().toString();
	}

	/** 
	 * Use to get the button who refresh the list of ports
	 *
	 * @return the button to refresh the list of ports
	 */
	public JButton getRefreshButton(){
		return this.refresh;
	}

	/**
	 * Use to get the button who upload a file on the sensor
	 *
	 * @return the button to upload a file on the sensor
	 */
	public JButton getUploadButton(){
		return this.upload;
	}

	/**
	 * Use to choose the file to upload
	 *
	 * @return the file selected by the user. If the user selected nothing, that return null
	 */
	public File selectAFile(){
		File currentDirectory = null;
		
		try {
			currentDirectory = new File(".").getCanonicalFile();    		
		}catch (IOException e1){
			System.err.println("Error to open the current directory");
		}

		JFileChooser selecter = new JFileChooser(currentDirectory);
		selecter.setAcceptAllFileFilterUsed(false);
 		FileNameExtensionFilter filter = new FileNameExtensionFilter(".hex", "hex");
 		selecter.addChoosableFileFilter(filter);
 		selecter.showOpenDialog(null);

    	return selecter.getSelectedFile();
	}

	/**
	 * Use to associate our button and an ActionListener
	 *
	 * @param al - The Action Listener to associate with our buttons
	 */
	public void setActionListener(ActionListener al){
		this.chooseFile.addActionListener(al);
		this.upload.addActionListener(al);
		this.exit.addActionListener(al);
		this.refresh.addActionListener(al);
	}

	/** 
	 * Set the label errorMessage to show where is the problem to upload
	 *
	 * @param errorStg - The error message
	 */
	public void setErrorMessage(String errorStg)
	{
		this.errorMessage.setText(errorStg);
	}

	/**
	 * Set the label fileChosen to show that chosen file
	 *
	 * @param file - The string with the name of the chosen file 
	 */
	public void setFileChosenLabel(String file)
	{
		if(file.equals("null"))
			this.fileChosen.setText("You have not chosen a file");
		else 
			this.fileChosen.setText("The file selected : " + file);
	}

	/**
	 * Set the JComboBox with the all available ports
	 *
	 * @param ports[] - The table with the all available ports
	 */
	public void setListOfPort(SerialPort[] ports)
	{
		this.listOfPort.removeAllItems();
		for (SerialPort port : ports)
			this.listOfPort.addItem(port.getSystemPortName()); //port.getSystemPortName()
	}

	/* Peut être implémenter plus tard
	public void setValidConditionOne(boolean isCheck){
		if (isCheck)
			this.validCondition1.setIcon(this.tickIcon);
		else	
			this.validCondition1.setIcon(null);
	}

	public void setValidConditionTwo(boolean isCheck){
		if (isCheck)
			this.validCondition2.setIcon(this.tickIcon);
		else	
			this.validCondition2.setIcon(null);
	}*/
}