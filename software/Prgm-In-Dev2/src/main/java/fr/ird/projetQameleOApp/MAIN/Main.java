package fr.ird.projetQameleOApp.MAIN;

import fr.ird.projetQameleOApp.CONTROLLER.*;
import fr.ird.projetQameleOApp.MODEL.*;
import fr.ird.projetQameleOApp.VIEW.*;

public class Main
{
	public static void main(String[] args) 
	{	
		Window myWindow = new Window("Projet A");
		WindowController myWindowController = new WindowController(myWindow);
		
		MenuView myMenuView = new MenuView();
		Model myModel = new Model();

		Controller myController = new Controller(myMenuView, myModel, myWindowController);
		
		myWindow.add(myMenuView);
		myWindow.setVisible(true);
	}
}