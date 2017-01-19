package bank.Ui;
import javax.swing.*;
import java.io.*;
import java.util.*;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: </p>
 * @author not attributable
 * @version 1.0
 */

public class Client {
  public Client() {
  }

  public static  void main(String[] args){
    try {
     UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
   }
   catch(Exception e) {
     e.printStackTrace();
   }

   String ccmhome = System.getProperty("CCM_HOME");
    FileInputStream stream = null;
    //File file = new File("ccmhome\\starccm.ini");
    try {
      stream = new FileInputStream(ccmhome+"starccm.properties");
    }
    catch (FileNotFoundException ex) {
    }
    Properties pros = new Properties();
    try {
      pros.load(stream);
    }
    catch (IOException ex1) {
    }
    String domainip = pros.getProperty("main_host_ip");
    String nameserverPort = pros.getProperty("NameServicePort");
    String arg[] = new String[2];
    arg[0] = domainip;
    arg[1] = nameserverPort;
    args = arg;

   Manager manager=new Manager();
   manager.setDomainIp(domainip);
   manager.setServicePort(nameserverPort);
   Login login=new Login(null,manager,args);



}

}
