package fr.ird.projetQameleOApp.CONTROLLER;

import fr.ird.projetQameleOApp.VIEW.*;
import fr.ird.projetQameleOApp.MODEL.*;

import java.awt.event.*;
import java.io.*;

/**
 * The <code>Controller</code> class controls the different actions of MenuView
 *
 * @version 1.1
 * @author Romain SAVARY
 */
public class Controller implements ActionListener
{	
	/**
	 *	Attribute to have the link with MenuView
	 */
	private MenuView linkToMenuView;

	/**
	 *	Attribute to have the link with the model
	 */
	private Model linkToModel;

	/**
	 * Attribute to have the link with the controller of the frame
	 */
	private WindowController linkToWC;

	/**
	 * Maker of the class
	 *
	 * @param mv - The MenuView to check
	 */
	public Controller(MenuView mv, Model m, WindowController wc){
		//Link
		//--------------------------//
		this.linkToMenuView=mv;
		this.linkToModel=m;
		this.linkToWC=wc;
		//--------------------------//

		//Setting the view
		//--------------------------//
		this.linkToMenuView.setActionListener(this);
		if (this.linkToModel.getFileSelected()==null)
			this.linkToMenuView.setFileChosenLabel("null");
		else
			this.linkToMenuView.setFileChosenLabel(this.linkToModel.getFileSelected().getName());
		this.linkToMenuView.setListOfPort(this.linkToModel.getAvailablePorts()); //il faudra les décommenter
		//--------------------------//
	}

	/**
	 * Depending on the event, choose the behavior of the program
	 *
	 * @param e - The event created
	 */
	@Override
	public void actionPerformed(ActionEvent e){
		if (e.getSource()==this.linkToMenuView.getChooseFileButton()){
			this.chooseFileAction();
		}
		if (e.getSource()==this.linkToMenuView.getUploadButton()){
			this.uploadOnSensorAction();
			//if (this.addASerialPortToTheModel())
			//	this.linkToModel.temporaryName10();
			
			//this.linkToModel.temporaryName12();
		}
		if (e.getSource()==this.linkToMenuView.getExitButton()){
			this.exitAction();
		}
		if (e.getSource()==this.linkToMenuView.getRefreshButton()){
			this.linkToMenuView.setListOfPort(this.linkToModel.getAvailablePorts()); //il faudra les décommenter

			//Temporary, check the baud rate of the port selected
			//--------------------------//
			//if (this.addASerialPortToTheModel())
			//	this.linkToModel.reformPortLocation();
			//	System.out.println("listDevices : ");
			//	this.linkToModel.listDevices();

			//	System.out.println("dumpDevices : ");
			//	this.linkToModel.dumpDevices();
			//--------------------------//
		}

	}

	/** 
	 * Add a SerialPort to the model
	 */
	private boolean addASerialPortToTheModel(){
		if(!(this.linkToMenuView.getPortChosen().equals(""))){
			//System.out.println("Je rentre ici");
			this.linkToModel.setPortSelected(this.linkToMenuView.getPortChosen());
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Called when the button to choose a file is pressed
	 */
	private void chooseFileAction(){
		System.out.println("Je veux sélectionner un ficher");
		File tmpFile = this.linkToMenuView.selectAFile();
		if (tmpFile==null){
			//Do nothing
		}else {
		this.linkToModel.setFileSelected(tmpFile);
		this.linkToMenuView.setFileChosenLabel(this.linkToModel.getFileSelected().getName());
		}
	}

	/**
	 * Called when the button to exit the program is pressed
	 */
	private void exitAction(){
		System.out.println("Je veux quitter l'app");
		this.linkToWC.closeWindow();
	}

	/**
	 * Called when the button to upload on the sensor is pressed
	 */
	private void uploadOnSensorAction(){
		System.out.println("Je veux téléverser un ficher");

		this.addASerialPortToTheModel();

		if (this.linkToModel.getFileSelected()==null && this.linkToModel.getPortSelected()==null){
			System.out.println("Le fichier et le port n'ont pas été selectionnés");
			this.linkToMenuView.setErrorMessage("Error - No file and port selected");

		}else if (this.linkToModel.getFileSelected()==null){
			System.out.println("Le fichier n'a pas été selectionné");
			this.linkToMenuView.setErrorMessage("Error - No file selected");

		}else if (this.linkToModel.getPortSelected()==null){
			System.out.println("Le port n'a pas été selectionné");
			this.linkToMenuView.setErrorMessage("Error - No port selected");

		}else {
			this.linkToMenuView.setErrorMessage("");
			//Write here to upload on the sensor

			this.linkToModel.uploadOnTheSensor();
		}
	}
}