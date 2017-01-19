package bank.Ui;

import javax.swing.*;
import java.awt.*;
import com.borland.jbcl.layout.*;
import java.awt.event.*;
import org.omg.CORBA.*;
import java.util.*;
import org.omg.CORBA.ORBPackage.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.InvalidName;
import org.omg.CosNaming.NamingContextPackage.*;
import org.omg.CosNaming.NamingContextPackage.*;
import bank.Bank.*;
/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: </p>
 * @author not attributable
 * @version 1.0
 */

public class Login
    extends JFrame {
  private Atm atm; //= new Atm();
  private Manager manager; //= new Manager();
 // private Manager accountmanager;
  private String args[];
  private org.omg.CORBA.ORB orb = null;
  private org.omg.CosNaming.NamingContext nc = null;


  private  bank.Bank.Authentication authentication = null;
  private AuthenticationOp authenticationop ;

  private String atmAccountId;

  public Login(){}
  public Login(Atm a, Manager m,String arg[]) {
    atm = a;
    manager = m;
    args = arg;
    EmployeeNumEdit.setFocusable(true);
    initORB();

    getNamingService();
    if(manager!=null){
      manager.setNamingContext(nc);
      manager.setORB(orb);
    }
    if(atm!=null){
      atm.setNamingContext(nc);

    }

    try {

      jbInit();

    }
    catch (Exception e) {
      e.printStackTrace();
    }
  }

  JPanel LoginPane = new JPanel();
  BorderLayout borderLayout1 = new BorderLayout();
  XYLayout xYLayout1 = new XYLayout();
  JPanel LoginImagePane = new JPanel();
  JLabel EmpoyeeNumLab = new JLabel();
  JLabel passwordLable = new JLabel();
  JTextField EmployeeNumEdit = new JTextField();
  JPasswordField PassWordEdit = new JPasswordField();
  JButton jButton1 = new JButton();
  BorderLayout borderLayout2 = new BorderLayout();
  JButton jButton2 = new JButton();

  //initialize orb
  public org.omg.CORBA.ORB initORB() {

    Properties props = System.getProperties();
    props.put("org.omg.CORBA.ORBClass", "com.ooc.CORBA.ORB");
    props.put("org.omg.CORBA.ORBSingletonClass", "com.ooc.CORBA.ORBSingleton");
    props.put("org.omg.CORBA.ORBClass", "net.nis.CORBA.ORB");
    props.put("org.omg.CORBA.ORBSingletonClass",
              "net.nis.CORBA.ORBSingleton");


      try {
        orb = org.omg.CORBA.ORB.init(args, props);

      }

    catch (Exception ex) {
      System.out.println("init orb failure...");
      JOptionPane.showConfirmDialog(this,"init orb failure!");

    }

      return orb;
  }
  public void getNamingService(){


      try {

        String  corbaloc = "corbaloc::"+args[0]+":"+args[1]+"/NameService";
        org.omg.CORBA.Object obj = orb.string_to_object(
           corbaloc);
        nc = NamingContextHelper.narrow(obj);
      }
      catch (Exception ex) {
          JOptionPane.showMessageDialog(this,"init NamingService failure!");
      }


  }
  public boolean Authenticationop(String id,String password){

    boolean tem = false;
    NameComponent name[] = new NameComponent[1];
    name[0] = new NameComponent();
    name[0].id = "Authentication";
    name[0].kind = "";
    org.omg.CORBA.Object obj = null;
    try {
      obj = nc.resolve(name);
    }
    catch (InvalidName ex) {
      JOptionPane.showMessageDialog(this, "resolve Authentication failure!");
        return false;
    }
    catch (CannotProceed ex) {
      JOptionPane.showMessageDialog(this, "resolve Authentication failure!");
      return false;
    }
    catch (NotFound ex) {
      JOptionPane.showMessageDialog(this, "resolve Authentication failure!");
        return false;
    }
    try {
      authentication = AuthenticationHelper.narrow(obj);
    }
    catch (Exception ex3) {
      System.out.println("AuthenticationHelper narrow failure...");
    }

    try {

      authenticationop = authentication.provide_authentication_op();
    }
    catch (Exception ex1) {
      System.out.println("authentication.provide_authentication_op failure..");
      return false;
    }
    try {
      tem = authenticationop.authenticate(id, password);
      System.out.println("login success...");
      return tem;
    }
    catch (Exception ex2) {
      JOptionPane.showMessageDialog(this, "login failure!");
      return false;
    }



  }

  private void jbInit() throws Exception {
    ImageIcon image1 = new ImageIcon(Manager.class.getResource("atm.jpg"));
    ImageIcon image2 = new ImageIcon(Manager.class.getResource("jhtb.jpg"));
    //ImageIcon image3 = new ImageIcon(Manager.class.getResource("dl.jpg"));
    ImageIcon image4 = new ImageIcon(Manager.class.getResource("jhk.jpg"));

    this.getContentPane().setLayout(borderLayout1);
    LoginPane.setLayout(xYLayout1);
    EmpoyeeNumLab.setFont(new java.awt.Font("Dialog", 0, 12));
    EmpoyeeNumLab.setText("«Î ‰»Î’À∫≈");
    passwordLable.setFont(new java.awt.Font("Dialog", 0, 12));
    passwordLable.setText("«Î ‰»Î√‹¬Î");
    PassWordEdit.addKeyListener(new Login_PassWordEdit_keyAdapter(this));
    EmployeeNumEdit.setText("");
    jButton1.setBackground(new Color(42, 138, 230));
    jButton1.setEnabled(true);
    jButton1.setForeground(new Color(42, 138, 230));
    jButton1.setBorder(null);
    jButton1.setOpaque(true);
    jButton1.setFocusPainted(false);
    jButton1.setIcon(image2);
    jButton1.setSelected(false);
    jButton1.setText("");

    jButton2.setBackground(new Color(42, 138, 230));
    jButton2.setEnabled(true);
    jButton2.setForeground(new Color(42, 138, 230));
    jButton2.setBorder(null);
    jButton2.setFocusPainted(false);
    jButton2.setIcon(image4);
    jButton2.setSelected(false);
    jButton2.setText("");

    jButton1.addActionListener(new Login_jButton1_actionAdapter(this));

    LoginImagePane.setLayout(borderLayout2);
    LoginPane.setBackground(new Color(147, 191, 238));
    LoginPane.setMinimumSize(new Dimension(468, 55));
    LoginPane.setPreferredSize(new Dimension(468, 55));

    this.setResizable(false);
    this.getContentPane().add(LoginImagePane, BorderLayout.NORTH);
    LoginImagePane.add(jButton1, BorderLayout.CENTER);
    this.getContentPane().add(LoginPane, BorderLayout.CENTER);
    LoginPane.add(passwordLable, new XYConstraints(331, 152, 99, 24));
    LoginPane.add(EmployeeNumEdit, new XYConstraints(130, 152, 145, 28));
    LoginPane.add(EmpoyeeNumLab, new XYConstraints(31, 152, 86, 27));
    LoginPane.add(PassWordEdit, new XYConstraints(417, 152, 167, 27));
    LoginPane.add(jButton2,   new XYConstraints(497, 247, 88, 59));
    this.show();
    this.setSize(new Dimension(620, 418));
    this.setVisible(true);
    Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();

    this.setLocation( (screenSize.width - 777) / 2,
                     (screenSize.height - 318) / 2);
   addWindowListener(new WindowAdapter() {
     public void windowClosing(WindowEvent e) {
       dispose();
       System.exit(0);
     }
   });

  }

  void PassWordEdit_keyPressed(KeyEvent e) {
    if (e.getKeyCode() == KeyEvent.VK_ENTER) {
      Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
      System.out.println("enter system...");
      if(atm!=null){
        atmAccountId = EmployeeNumEdit.getText();
        atm.setAtmaccountId(atmAccountId);

        //String id = EmployeeNumEdit.getText();
        String password = PassWordEdit.getText();
        boolean firm = false;
        firm = Authenticationop(atmAccountId, password);
        if (firm) {

          atm.show();
          atm.setSize(450, 400);
          atm.setVisible(true);
          atm.setLocation( (screenSize.width - 450) / 2,
                          (screenSize.height - 450) / 2);


          this.setVisible(false);
        }else{
          JOptionPane.showMessageDialog(this,"µ«¬Ω ß∞‹!«Î÷ÿ–¬ ‰»Î’À∫≈∫Õ√‹¬Î");
          return;
        }
      }
      if(manager!=null){

        String id = EmployeeNumEdit.getText();
        String password = PassWordEdit.getText();
        boolean firm = false;
        firm = Authenticationop(id,password);
        if(firm){
          manager.show();
          manager.setSize(500, 500);
          manager.setVisible(true);
          manager.setLocation( (screenSize.width - 450) / 2,
                              (screenSize.height - 450) / 2);

          this.setVisible(false);
        }else{
          JOptionPane.showMessageDialog(this,"µ«¬Ω ß∞‹!«Î÷ÿ–¬ ‰»Î’À∫≈∫Õ√‹¬Î");
          return;
        }

      }

    }

  }

  void jButton1_actionPerformed(ActionEvent e) {

  }
}

class Login_PassWordEdit_keyAdapter
    extends java.awt.event.KeyAdapter {
  Login adaptee;

  Login_PassWordEdit_keyAdapter(Login adaptee) {
    this.adaptee = adaptee;
  }

  public void keyPressed(KeyEvent e) {
    adaptee.PassWordEdit_keyPressed(e);
  }
}

class Login_jButton1_actionAdapter
    implements java.awt.event.ActionListener {
  Login adaptee;

  Login_jButton1_actionAdapter(Login adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jButton1_actionPerformed(e);
  }
}