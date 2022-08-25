/**
*
* @version 1.1 6 janvier 2022
* @author Romain SAVARY
*/
package fr.ird.projetQameleOApp.VIEW;

import javax.swing.*;
import java.awt.*;

public class Window extends JFrame {
	public Window(){
   	  super();
	  this.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);	
      this.setSize(new Dimension(920,600));
      this.setLocation(0,0);
      this.setResizable(false);
	}

	public Window(String title){
	  super(title);
	  this.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);	
      this.setSize(new Dimension(920,600));
      this.setLocation(0,0);
      this.setResizable(false);
	}

}