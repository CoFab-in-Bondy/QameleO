package fr.ird.projetQameleOApp.CONTROLLER;

import fr.ird.projetQameleOApp.VIEW.*;

import java.awt.event.*;
import java.awt.Toolkit;
import javax.swing.*;
import java.util.*;

/**
 * The <code>WindowController</code> class controls the Window
 *
 * @version 1.1
 * @author Romain SAVARY
 */
public class WindowController implements WindowListener
{
	/**
	 * Attribute to have the link with Window
	 */
	private Window linkToView;

	/**
	 * Maker of the class
	 *
	 * @param w - The Window to check
	 */
	public WindowController(Window w){
		this.linkToView=w;
		this.linkToView.addWindowListener(this); 
	}

	/**
	 * Créer un évenement de type WindowClosing afin d'orienter tous les types
	 * de fermeture vers une même méthode.
	 */
	public void closeWindow()
	{
		WindowEvent windowEvent = new WindowEvent(this.linkToView, WindowEvent.WINDOW_CLOSING);
		Toolkit.getDefaultToolkit().getSystemEventQueue().postEvent(windowEvent);
	}

	public void windowActivated(WindowEvent e){}
	public void windowClosed(WindowEvent e){}

	/**
	 * Is activated when the window is closed.
	 *
	 * @param e event.
	 */
	@Override
	public void windowClosing(WindowEvent e)
	{
	    int res = JOptionPane.showConfirmDialog(this.linkToView,"Do you want to leave the application ?","Exit", JOptionPane.YES_NO_OPTION);  

		if (res == JOptionPane.YES_OPTION)
		{  
	    	this.linkToView.dispose(); 
	    	System.exit(0);
		}
	}

	public void windowDeactivated(WindowEvent e){}
	public void windowDeiconified(WindowEvent e){}
	public void windowIconified(WindowEvent e){}
	public void windowOpened(WindowEvent e){}
}