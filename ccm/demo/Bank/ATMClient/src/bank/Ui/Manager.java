package bank.Ui;

import javax.swing.*;
import java.io.*;
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

public class Manager
    extends JFrame {
  JPanel jPanel1 = new JPanel();
  BorderLayout borderLayout1 = new BorderLayout();
  JPanel jPanel2 = new JPanel();
  JTabbedPane jTabbedPane1 = new JTabbedPane();
  JPanel WithdrawPane = new JPanel();
  JPanel jPanel4 = new JPanel();
  JPanel DepositPane = new JPanel();
  JPanel queryPane = new JPanel();
  BorderLayout borderLayout2 = new BorderLayout();
  JPanel jPanel7 = new JPanel();
  JTextArea jEditorPane1 = new JTextArea();
  JPanel jPanel8 = new JPanel();
  XYLayout xYLayout2 = new XYLayout();
  BorderLayout borderLayout4 = new BorderLayout();
  JTextField jTextField1 = new JTextField();
  JButton jButton1 = new JButton();
  JButton jButton2 = new JButton();
  JLabel jLabel1 = new JLabel();
  JLabel jLabel2 = new JLabel();
  JTextField jTextField2 = new JTextField();
  GridLayout gridLayout1 = new GridLayout();
  GridLayout gridLayout2 = new GridLayout();
  JPanel WithdrawPane_inputpane = new JPanel();
  JPanel WithdrawPane_Edit = new JPanel();
  GridLayout gridLayout3 = new GridLayout();
  JEditorPane jEditorPane2 = new JEditorPane();
  BorderLayout borderLayout3 = new BorderLayout();
  JPanel WithdrawPane_inputpane_Enter = new JPanel();
  BorderLayout borderLayout5 = new BorderLayout();
  XYLayout xYLayout1 = new XYLayout();
  JButton With_OK_Button = new JButton();
  JButton with_Cancel_Button = new JButton();
  JTextField withdrawdEdit1 = new JTextField();
  JLabel jLabel3 = new JLabel();
  JPanel DepositInputPane = new JPanel();
  JPanel DepositEditPane = new JPanel();
  GridLayout gridLayout4 = new GridLayout();
  BorderLayout borderLayout6 = new BorderLayout();
  XYLayout xYLayout3 = new XYLayout();
  JButton deposit_OK_Button = new JButton();
  JButton deposit_Cancle_Button = new JButton();
  JLabel jLabel4 = new JLabel();
  JTextField depositEdit1 = new JTextField();
  JPanel jPanel3 = new JPanel();
  JPanel jPanel5 = new JPanel();
  //JEditorPane jEditorPane4 = new JEditorPane();
  BorderLayout borderLayout7 = new BorderLayout();
  JButton queryOkButton = new JButton();
  XYLayout xYLayout4 = new XYLayout();
  JButton queryCancleButton = new JButton();
  JTextField queryEdit = new JTextField();
  JLabel jLabel5 = new JLabel();
  JPanel changepassword = new JPanel();
  JPanel newAccountPane = new JPanel();
  JPanel deleAccountPane = new JPanel();
  JPanel jPanel12 = new JPanel();
  JButton jButton3 = new JButton();
  BorderLayout borderLayout8 = new BorderLayout();
  GridLayout gridLayout5 = new GridLayout();
  JPanel jPanel10 = new JPanel();
  JPanel jPanel13 = new JPanel();
  JTextArea openaccounttextArea = new JTextArea();
  BorderLayout borderLayout9 = new BorderLayout();
  XYLayout xYLayout5 = new XYLayout();
  JButton openAccountOK = new JButton();
  JButton openAccountCancel = new JButton();
  JLabel jLabel6 = new JLabel();

  //definition
  private org.omg.CosNaming.NamingContext nc = null;
  private bank.Bank.Manager openManager = null;
  private Delegation delegation = null;
  private DelegationOp delegationop = null;
  private ManagerOp managerop;
  private org.omg.CORBA.ORB orb;

  JLabel jLabel7 = new JLabel();
  JTextField withdrawdEdit2 = new JTextField();
  JLabel jLabel8 = new JLabel();
  JTextField depositEdit2 = new JTextField();
  JTextArea withdrawTextArea = new JTextArea();
  JTextArea depositTextArea = new JTextArea();
  JTextArea jEditorPane4 = new JTextArea();
  JPanel jPanel6 = new JPanel();
  JPanel jPanel9 = new JPanel();
  GridLayout gridLayout6 = new GridLayout();
  JTextArea jTextArea1 = new JTextArea();
  BorderLayout borderLayout10 = new BorderLayout();
  XYLayout xYLayout6 = new XYLayout();
  JButton changepass_ok = new JButton();
  JButton changepass_Cancel = new JButton();
  JLabel jLabel9 = new JLabel();
  JTextField jTextField3 = new JTextField();
  JLabel jLabel10 = new JLabel();
  JPasswordField jPasswordField1 = new JPasswordField();
  JPanel jPanel14 = new JPanel();
  JPanel jPanel15 = new JPanel();
  GridLayout gridLayout7 = new GridLayout();
  JTextArea jTextArea2 = new JTextArea();
  BorderLayout borderLayout11 = new BorderLayout();
  JLabel jLabel11 = new JLabel();
  JTextField jTextField4 = new JTextField();
  JButton deleAccount_OK = new JButton();
  XYLayout xYLayout7 = new XYLayout();
  JButton deleAccount_Cancel = new JButton();
  JPasswordField jPasswordField2 = new JPasswordField();
  JPanel payPane = new JPanel();
  JPanel jPanel16 = new JPanel();
  GridLayout gridLayout8 = new GridLayout();
  JTextArea jTextArea3 = new JTextArea();
  BorderLayout borderLayout12 = new BorderLayout();
  JLabel jLabel12 = new JLabel();
  XYLayout xYLayout8 = new XYLayout();
  JLabel jLabel13 = new JLabel();
  JTextField jTextField5 = new JTextField();
  JTextField jTextField6 = new JTextField();
  JButton telephone_ok = new JButton();
  JButton telephone_cancel = new JButton();
  JTextField jTextField7 = new JTextField();
  JLabel jLabel14 = new JLabel();
  JLabel jLabel15 = new JLabel();
  JLabel jLabel16 = new JLabel();
  JPasswordField jPasswordField3 = new JPasswordField();
  JPasswordField jPasswordField4 = new JPasswordField();
  private static String domainIp;
  private static String port;
  public Manager() {
    try {
      UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
    }
    catch (Exception e) {
      e.printStackTrace();
    }

    try {
      jbInit();
      //newAccount();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
  }

  public void setNamingContext(NamingContext nc_) {
    nc = nc_;

  }

  public void setORB(org.omg.CORBA.ORB orb_) {
    this.orb = orb_;
    // payForPhone("43010006",200);

  }
  public void setDomainIp(String ip){
  this.domainIp = ip;
}
public void setServicePort(String port) {
  this.port = port;
}


  void newnamecontext() {
    try {

      org.omg.CORBA.Object obj = orb.string_to_object(
          "corbaloc::"+domainIp+":"+port+"/NameService");
      nc = NamingContextHelper.narrow(obj);
    }
    catch (Exception ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }

  }

  public String newAccount(String password) {

    String newaccount = "";
    FileInputStream accountfile = null;
    try {
      accountfile = new FileInputStream("account.txt");
    }
    catch (FileNotFoundException ex4) {
    }
    java.io.DataInputStream filein = null;

    filein = new java.io.DataInputStream(accountfile);

    String tem = "";
    long accountid = 0;
    ;
    if (filein != null) {
      try {
        newaccount = filein.readLine();

        while (newaccount != null) {
          tem = newaccount;
          newaccount = filein.readLine();

        }
        Long number = new Long(tem);
        accountid = number.longValue();
        accountid = accountid + 1;
      }
      catch (Exception ex3) {
      }
    }
    newaccount = newaccount.valueOf(accountid);
    try {
      accountfile.close();
      filein.close();
    }
    catch (IOException ex2) {
    }

    NameComponent name[] = new NameComponent[1];
    name[0] = new NameComponent();
    name[0].id = "Manager";
    name[0].kind = "";
    org.omg.CORBA.Object obj = null;
    try {
      obj = nc.resolve(name);
    }
    catch (InvalidName ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    catch (CannotProceed ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    catch (NotFound ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    openManager = ManagerHelper.narrow(obj);

    try {

      managerop = openManager.provide_manager_op();
    }
    catch (Exception ex1) {
      System.out.println("failure..");
    }

    //String id = "jxh1";
    //String password = "111";
    System.out.println("create a new account:" + newaccount);
    managerop.createAccount(newaccount, "111");
    System.out.println("opening account success..");

    return newaccount;

//       NameComponent name1[] = new NameComponent[1];
//       name1[0] = new NameComponent();
//       name1[0].id = "OpenAccountComp";
//
//       name1[0].kind = "";
//       org.omg.CORBA.Object obj1 =  null;
//       try {
//         obj1 = nc.resolve(name1);
//       }
//       catch (InvalidName ex) {
//          JOptionPane.showMessageDialog(this,"init NamingService failure!");
//       }
//       catch (CannotProceed ex) {
//          JOptionPane.showMessageDialog(this,"init NamingService failure!");
//       }
//       catch (NotFound ex) {
//          JOptionPane.showMessageDialog(this,"init NamingService failure!");
//       }
//       OpenAccountComp openaccount = OpenAccountCompHelper.narrow(obj1);
//
//
//       OpenAccountOp op = openaccount.provide_open_account_op();
//       op.openAccount("jxh","123");

  }

  private void jbInit() throws Exception {
    ImageIcon image1 = new ImageIcon(Manager.class.getResource("jhy.jpg"));
    ImageIcon image2 = new ImageIcon(Manager.class.getResource("allinone.jpg"));
    //ImageIcon image3 = new ImageIcon(Manager.class.getResource("dl.jpg"));
    this.getContentPane().setLayout(borderLayout1);
    jPanel1.setLayout(borderLayout2);
    jTabbedPane1.setBackground(new Color(141, 191, 238));
    jTabbedPane1.setFont(new java.awt.Font("Dialog", 0, 14));
    jTabbedPane1.setMinimumSize(new Dimension(150, 364));
    jTabbedPane1.setPreferredSize(new Dimension(450, 450));
    jTabbedPane1.addFocusListener(new Manager_jTabbedPane1_focusAdapter(this));
    WithdrawPane.setBackground(new Color(141, 191, 238));
    WithdrawPane.setFont(new java.awt.Font("Dialog", 0, 12));
    WithdrawPane.setMinimumSize(new Dimension(394, 280));
    WithdrawPane.setPreferredSize(new Dimension(394, 280));
    WithdrawPane.setToolTipText("");
    WithdrawPane.addFocusListener(new Manager_WithdrawPane_focusAdapter(this));
    WithdrawPane.setLayout(gridLayout3);
    jPanel4.setBackground(new Color(141, 191, 238));
    jPanel4.setFont(new java.awt.Font("Dialog", 0, 12));
    jPanel4.setMinimumSize(new Dimension(208, 220));
    jPanel4.setToolTipText("");
    jPanel4.addFocusListener(new Manager_jPanel4_focusAdapter(this));
    jPanel4.setLayout(gridLayout1);
    jPanel7.setLayout(borderLayout4);
    jEditorPane1.setFont(new java.awt.Font("Dialog", 0, 18));
    jEditorPane1.setForeground(new Color(43, 64, 169));
    jEditorPane1.setBorder(BorderFactory.createEtchedBorder());
    jEditorPane1.setMinimumSize(new Dimension(104, 100));
    jEditorPane1.setPreferredSize(new Dimension(197, 280));
    jEditorPane1.setSelectionColor(new Color(43, 64, 169));
    jEditorPane1.setSelectionStart(38);
    jEditorPane1.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");
    jPanel8.setLayout(xYLayout2);
    jPanel7.setBackground(new Color(141, 191, 238));
    jPanel7.setMinimumSize(new Dimension(204, 100));
    jPanel7.setPreferredSize(new Dimension(204, 100));
    queryPane.setBackground(new Color(141, 191, 238));
    queryPane.setFont(new java.awt.Font("Dialog", 0, 12));
    queryPane.addFocusListener(new Manager_queryPane_focusAdapter(this));
    queryPane.setLayout(gridLayout2);
    DepositPane.setBackground(new Color(141, 191, 238));
    DepositPane.setFont(new java.awt.Font("Dialog", 0, 12));
    DepositPane.setForeground(new Color(43, 64, 169));
    DepositPane.setMaximumSize(new Dimension(32767, 32767));
    DepositPane.setPreferredSize(new Dimension(394, 280));
    DepositPane.addFocusListener(new Manager_DepositPane_focusAdapter(this));
    DepositPane.setLayout(gridLayout4);
    jPanel8.setBackground(new Color(141, 191, 238));
    jPanel8.setFont(new java.awt.Font("Dialog", 0, 12));
    jPanel8.setPreferredSize(new Dimension(180, 110));
    jPanel2.setBackground(new Color(141, 191, 238));
    jPanel2.setFont(new java.awt.Font("Dialog", 0, 12));
    jPanel2.setMinimumSize(new Dimension(453, 140));
    jPanel2.setPreferredSize(new Dimension(451, 140));
    jPanel2.setLayout(borderLayout8);
    jButton1.setBackground(new Color(141, 191, 238));
    jButton1.setFont(new java.awt.Font("Dialog", 0, 12));
    jButton1.setText("确   定");
    jButton1.addActionListener(new Manager_jButton1_actionAdapter(this));
    jButton2.setBackground(new Color(141, 191, 238));
    jButton2.setFont(new java.awt.Font("Dialog", 0, 12));
    jButton2.setText("退   出");
    jButton2.addActionListener(new Manager_jButton2_actionAdapter(this));
    jLabel1.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel1.setText("请您输入转入卡卡号");
    jLabel2.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel2.setText("请输入转入金额");
    jTextField2.setText("");
    gridLayout1.setColumns(2);
    gridLayout2.setColumns(2);
    gridLayout2.setRows(1);
    gridLayout3.setColumns(2);
    WithdrawPane_Edit.setLayout(borderLayout3);
    WithdrawPane_Edit.setBackground(new Color(141, 191, 238));
    WithdrawPane_Edit.setMinimumSize(new Dimension(197, 280));
    WithdrawPane_Edit.setPreferredSize(new Dimension(197, 280));
    WithdrawPane_inputpane.setDebugGraphicsOptions(0);
    WithdrawPane_inputpane.setMinimumSize(new Dimension(192, 235));
    WithdrawPane_inputpane.setOpaque(true);
    WithdrawPane_inputpane.setLayout(borderLayout5);
    WithdrawPane_inputpane_Enter.setLayout(xYLayout1);
    With_OK_Button.setBackground(new Color(141, 191, 238));
    With_OK_Button.setFont(new java.awt.Font("Dialog", 0, 12));
    With_OK_Button.setText("确   定");
    With_OK_Button.addActionListener(new Manager_With_OK_Button_actionAdapter(this));
    with_Cancel_Button.setBackground(new Color(141, 191, 238));
    with_Cancel_Button.setFont(new java.awt.Font("Dialog", 0, 12));
    with_Cancel_Button.setText("退   出");
    with_Cancel_Button.addActionListener(new
        Manager_with_Cancel_Button_actionAdapter(this));
    jLabel3.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel3.setPreferredSize(new Dimension(60, 18));
    jLabel3.setText("请输入卡号");
    depositTextArea.setBorder(BorderFactory.createEtchedBorder());
    depositTextArea.setPreferredSize(new Dimension(197, 280));
    depositTextArea.setSelectionColor(new Color(43, 64, 169));
    depositTextArea.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");
    DepositEditPane.setBackground(new Color(141, 191, 238));
    DepositEditPane.setFont(new java.awt.Font("Dialog", 0, 12));
    DepositEditPane.setMinimumSize(new Dimension(197, 280));
    DepositEditPane.setOpaque(true);
    DepositEditPane.setPreferredSize(new Dimension(197, 280));
    DepositEditPane.setLayout(borderLayout6);
    gridLayout4.setColumns(2);
    DepositInputPane.setLayout(xYLayout3);
    deposit_OK_Button.setBackground(new Color(141, 191, 238));
    deposit_OK_Button.setFont(new java.awt.Font("Dialog", 0, 12));
    deposit_OK_Button.setText("确   定");
    deposit_OK_Button.addActionListener(new
                                        Manager_deposit_OK_Button_actionAdapter(this));
    deposit_Cancle_Button.setBackground(new Color(141, 191, 238));
    deposit_Cancle_Button.setFont(new java.awt.Font("Dialog", 0, 12));
    deposit_Cancle_Button.setText("退   出");
    deposit_Cancle_Button.addActionListener(new
        Manager_deposit_Cancle_Button_actionAdapter(this));
    jLabel4.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel4.setText("请输入卡号");
    jPanel3.setLayout(borderLayout7);
    queryOkButton.setBackground(new Color(141, 191, 238));
    queryOkButton.setFont(new java.awt.Font("Dialog", 0, 12));
    queryOkButton.setText("确   定");
    queryOkButton.addActionListener(new Manager_queryOkButton_actionAdapter(this));
    jPanel5.setLayout(xYLayout4);
    queryCancleButton.setBackground(new Color(141, 191, 238));
    queryCancleButton.setFont(new java.awt.Font("Dialog", 0, 12));
    queryCancleButton.setText("退   出");
    queryCancleButton.addActionListener(new
                                        Manager_queryCancleButton_actionAdapter(this));
    jLabel5.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel5.setText("请输入卡号");
    changepassword.setBackground(new Color(141, 191, 238));
    changepassword.setFont(new java.awt.Font("Dialog", 0, 12));
    changepassword.setAlignmentX( (float) 0.5);
    changepassword.addFocusListener(new Manager_changepassword_focusAdapter(this));
    changepassword.setLayout(gridLayout6);
    newAccountPane.setBackground(new Color(141, 191, 238));
    newAccountPane.setEnabled(true);
    newAccountPane.setFont(new java.awt.Font("Dialog", 0, 12));
    newAccountPane.setToolTipText("");
    newAccountPane.addFocusListener(new Manager_newAccountPane_focusAdapter(this));
    newAccountPane.setLayout(gridLayout5);
    deleAccountPane.setBackground(new Color(141, 191, 238));
    deleAccountPane.setFont(new java.awt.Font("Dialog", 0, 12));
    deleAccountPane.setDoubleBuffered(true);
    deleAccountPane.addFocusListener(new Manager_deleAccountPane_focusAdapter(this));
    deleAccountPane.setLayout(gridLayout7);
    jPanel12.setBackground(new Color(141, 191, 238));
    jPanel12.setFont(new java.awt.Font("Dialog", 0, 12));
    jPanel12.addFocusListener(new Manager_jPanel12_focusAdapter(this));
    jPanel12.setLayout(gridLayout8);
    withdrawdEdit1.setPreferredSize(new Dimension(6, 22));
    withdrawdEdit1.setCaretColor(Color.black);
    withdrawdEdit1.setText("");
    queryEdit.setText("");
    jTextField1.setText("");
    depositEdit1.setPreferredSize(new Dimension(6, 22));
    depositEdit1.setText("");
    jButton3.setBackground(new Color(141, 191, 238));
    jButton3.setBorder(null);
    jButton3.setPreferredSize(new Dimension(451, 209));
    jButton3.setFocusPainted(false);
    jButton3.setIcon(image1);
    jButton3.setText("");
    WithdrawPane_inputpane_Enter.setBackground(new Color(141, 191, 238));
    WithdrawPane_inputpane_Enter.setFont(new java.awt.Font("Dialog", 0, 12));
    WithdrawPane_inputpane_Enter.setMinimumSize(new Dimension(450, 450));
    WithdrawPane_inputpane_Enter.setPreferredSize(new Dimension(450, 450));
    jPanel1.setBackground(new Color(141, 191, 238));
    DepositInputPane.setBackground(new Color(141, 191, 238));
    DepositInputPane.setAlignmentY( (float) 0.5);
    jPanel3.setBackground(new Color(141, 191, 238));
    jPanel5.setBackground(new Color(141, 191, 238));
    this.setResizable(false);
    openaccounttextArea.setFont(new java.awt.Font("Dialog", 0, 18));
    openaccounttextArea.setForeground(new Color(43, 64, 169));
    openaccounttextArea.setBorder(BorderFactory.createEtchedBorder());
    openaccounttextArea.setMinimumSize(new Dimension(197, 280));
    openaccounttextArea.setPreferredSize(new Dimension(197, 280));
    openaccounttextArea.setSelectionColor(new Color(43, 64, 169));
    openaccounttextArea.setCaretPosition(0);
    openaccounttextArea.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");
    jPanel10.setBackground(new Color(141, 191, 237));
    jPanel10.setMinimumSize(new Dimension(104, 100));
    jPanel10.setPreferredSize(new Dimension(104, 100));
    jPanel10.setLayout(borderLayout9);
    jPanel13.setLayout(xYLayout5);
    openAccountOK.setBackground(new Color(141, 191, 238));
    openAccountOK.setFont(new java.awt.Font("Dialog", 0, 12));
    openAccountOK.setText("确   定");
    openAccountOK.addActionListener(new Manager_openAccountOK_actionAdapter(this));
    openAccountCancel.setBackground(new Color(141, 191, 238));
    openAccountCancel.setFont(new java.awt.Font("Dialog", 0, 12));
    openAccountCancel.setText("退   出");
    openAccountCancel.addActionListener(new
                                        Manager_openAccountCancel_actionAdapter(this));
    jLabel6.setBackground(new Color(141, 191, 238));
    jLabel6.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel6.setText("请输入密码");
    jPanel13.setBackground(new Color(141, 191, 238));
    jLabel7.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel7.setText("请输入取款金额");
    withdrawdEdit2.setText("");
    jLabel8.setFont(new java.awt.Font("Dialog", 0, 15));
    jLabel8.setText("请输入存款金额");
    depositEdit2.setText("");
    withdrawTextArea.setFont(new java.awt.Font("Dialog", 0, 18));
    withdrawTextArea.setForeground(new Color(43, 64, 169));
    withdrawTextArea.setAlignmentY( (float) 0.5);
    withdrawTextArea.setBorder(BorderFactory.createEtchedBorder());
    withdrawTextArea.setMinimumSize(new Dimension(197, 280));
    withdrawTextArea.setPreferredSize(new Dimension(197, 280));
    withdrawTextArea.setMargin(new Insets(0, 0, 0, 0));
    withdrawTextArea.setSelectionColor(new Color(43, 64, 169));
    withdrawTextArea.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");
    depositTextArea.setFont(new java.awt.Font("Dialog", 0, 18));
    depositTextArea.setForeground(new Color(34, 64, 169));
    depositTextArea.setBorder(BorderFactory.createEtchedBorder());
    jEditorPane4.setBorder(BorderFactory.createEtchedBorder());
    jEditorPane4.setMinimumSize(new Dimension(106, 20));
    jEditorPane4.setOpaque(true);
    jEditorPane4.setPreferredSize(new Dimension(197, 280));
    jEditorPane4.setToolTipText("");
    jEditorPane4.setSelectionColor(new Color(43, 64, 169));
    jEditorPane4.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");
    jEditorPane4.setTabSize(8);
    jEditorPane4.setFont(new java.awt.Font("Dialog", 0, 18));
    jEditorPane4.setForeground(new Color(43, 64, 169));
    jEditorPane4.setAlignmentX( (float) 0.5);
    jEditorPane4.setPreferredSize(new Dimension(197, 280));
    gridLayout6.setColumns(2);
    jTextArea1.setFont(new java.awt.Font("Dialog", 0, 18));
    jTextArea1.setForeground(new Color(43, 64, 169));
    jTextArea1.setBorder(BorderFactory.createEtchedBorder());
    jTextArea1.setOpaque(true);
    jTextArea1.setPreferredSize(new Dimension(197, 280));
    jTextArea1.setDisabledTextColor(new Color(43, 64, 169));
    jTextArea1.setSelectionColor(new Color(43, 64, 169));
    jTextArea1.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");
    jPanel9.setLayout(borderLayout10);
    jPanel6.setLayout(xYLayout6);
    changepass_ok.setBackground(new Color(141, 191, 238));
    changepass_ok.setFont(new java.awt.Font("Dialog", 0, 12));
    changepass_ok.setText("确   定");
    changepass_ok.addActionListener(new Manager_changepass_ok_actionAdapter(this));
    changepass_Cancel.setBackground(new Color(141, 191, 238));
    changepass_Cancel.setFont(new java.awt.Font("Dialog", 0, 12));
    changepass_Cancel.setText("退   出");
    changepass_Cancel.addActionListener(new
                                        Manager_changepass_Cancel_actionAdapter(this));
    jLabel9.setBackground(new Color(141, 191, 238));
    jLabel9.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel9.setText("请输入卡号");
    jTextField3.setBackground(Color.white);
    jTextField3.setFont(new java.awt.Font("Dialog", 0, 12));
    jTextField3.setText("");
    jLabel10.setBackground(new Color(141, 191, 238));
    jLabel10.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel10.setText("请输入新密码");
    jPanel6.setBackground(new Color(141, 191, 238));
    jPanel9.setBackground(new Color(141, 191, 238));
    jPasswordField1.setFont(new java.awt.Font("Dialog", 0, 12));
    jPasswordField1.setText("");
    gridLayout7.setColumns(2);
    jTextArea2.setBackground(Color.white);
    jTextArea2.setFont(new java.awt.Font("Dialog", 0, 18));
    jTextArea2.setForeground(new Color(43, 64, 169));
    jTextArea2.setBorder(BorderFactory.createEtchedBorder());
    jTextArea2.setMaximumSize(new Dimension(2147483647, 2147483647));
    jTextArea2.setMinimumSize(new Dimension(197, 280));
    jTextArea2.setPreferredSize(new Dimension(197, 280));
    jTextArea2.setSelectionColor(new Color(43, 64, 169));
    jTextArea2.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");
    jPanel15.setLayout(borderLayout11);
    jLabel11.setBackground(new Color(141, 191, 238));
    jLabel11.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel11.setText("请输入卡号");
    deleAccount_OK.setBackground(new Color(141, 191, 238));
    deleAccount_OK.setFont(new java.awt.Font("Dialog", 0, 12));
    deleAccount_OK.setText("确   定");
    deleAccount_OK.addActionListener(new Manager_deleAccount_OK_actionAdapter(this));
    jPanel14.setLayout(xYLayout7);
    deleAccount_Cancel.setBackground(new Color(141, 191, 238));
    deleAccount_Cancel.setFont(new java.awt.Font("Dialog", 0, 12));
    deleAccount_Cancel.setText("退   出");
    deleAccount_Cancel.addActionListener(new
        Manager_deleAccount_Cancel_actionAdapter(this));
    jTextField4.setBackground(Color.white);
    jTextField4.setFont(new java.awt.Font("Dialog", 0, 12));
    jTextField4.setText("");
    jPanel14.setBackground(new Color(141, 191, 238));
    jPanel15.setBackground(new Color(141, 191, 238));
    jPasswordField2.setMinimumSize(new Dimension(6, 22));
    jPasswordField2.setPreferredSize(new Dimension(6, 22));
    jPasswordField2.setText("");
    gridLayout8.setColumns(2);
    gridLayout8.setRows(1);
    jTextArea3.setFont(new java.awt.Font("Dialog", 0, 18));
    jTextArea3.setForeground(new Color(43, 64, 169));
    jTextArea3.setBorder(BorderFactory.createEtchedBorder());
    jTextArea3.setPreferredSize(new Dimension(197, 280));
    jTextArea3.setSelectionColor(new Color(43, 64, 169));
    jTextArea3.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");
    jPanel16.setBackground(new Color(141, 191, 238));
    jPanel16.setFont(new java.awt.Font("Dialog", 0, 12));
    jPanel16.setLayout(borderLayout12);
    payPane.setBackground(new Color(141, 191, 238));
    payPane.setLayout(xYLayout8);
    jLabel12.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel12.setText("请输入卡号");
    jLabel13.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel13.setText("请输入交费金额");
    telephone_ok.setBackground(new Color(141, 191, 238));
    telephone_ok.setFont(new java.awt.Font("Dialog", 0, 12));
    telephone_ok.setText("确   定");
    telephone_ok.addActionListener(new Manager_telephone_ok_actionAdapter(this));
    telephone_cancel.setBackground(new Color(141, 191, 238));
    telephone_cancel.setEnabled(true);
    telephone_cancel.setFont(new java.awt.Font("Dialog", 0, 12));
    telephone_cancel.setText("退   出");
    telephone_cancel.addActionListener(new
                                       Manager_telephone_cancel_actionAdapter(this));
    jTextField5.setFont(new java.awt.Font("Dialog", 0, 12));
    jTextField5.setText("");
    jTextField6.setFont(new java.awt.Font("Dialog", 0, 12));
    jTextField6.setText("");
    jLabel14.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel14.setText(" 请输入您的卡号");
    jTextField7.setText("");
    jLabel15.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel15.setText("请输入密码");
    jLabel16.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel16.setText("请输入密码");
    jPasswordField4.setText("");
    this.getContentPane().add(jPanel1, BorderLayout.CENTER);
    jPanel1.add(jTabbedPane1, BorderLayout.CENTER);
    jTabbedPane1.addTab("取钱", WithdrawPane);
    WithdrawPane.add(WithdrawPane_Edit, null);
    WithdrawPane_Edit.add(withdrawTextArea, BorderLayout.NORTH);
    WithdrawPane.add(WithdrawPane_inputpane, null);
    WithdrawPane_inputpane.add(WithdrawPane_inputpane_Enter,
                               BorderLayout.CENTER);
    WithdrawPane_inputpane_Enter.add(With_OK_Button, new XYConstraints(38, 238, -1, -1));
    WithdrawPane_inputpane_Enter.add(with_Cancel_Button, new XYConstraints(119, 236, -1, -1));
    WithdrawPane_inputpane_Enter.add(withdrawdEdit2,             new XYConstraints(53, 190, 123, -1));
    WithdrawPane_inputpane_Enter.add(jLabel3,      new XYConstraints(53, 32, 99, 22));
    WithdrawPane_inputpane_Enter.add(withdrawdEdit1,             new XYConstraints(53, 56, 125, -1));
    WithdrawPane_inputpane_Enter.add(jLabel15,      new XYConstraints(53, 98, 99, 22));
    WithdrawPane_inputpane_Enter.add(jLabel7,     new XYConstraints(53, 162, 99, 22));
    WithdrawPane_inputpane_Enter.add(jPasswordField4,      new XYConstraints(53, 128, 123, -1));
    jTabbedPane1.add("存钱", DepositPane);
    jTabbedPane1.add("查帐", queryPane);
    jTabbedPane1.add("转帐", jPanel4);
    jPanel4.add(jPanel7, null);
    jPanel7.add(jEditorPane1, BorderLayout.NORTH);
    jPanel4.add(jPanel8, null);
    jPanel8.add(jTextField2, new XYConstraints(55, 188, 127, -1));
    jPanel8.add(jLabel2, new XYConstraints(55, 156, 99, -1));
    jPanel8.add(jTextField1, new XYConstraints(55, 122, 125, -1));
    jPanel8.add(jTextField7, new XYConstraints(55, 61, 121, -1));
    jPanel8.add(jLabel14, new XYConstraints(55, 29, 121, -1));
    jPanel8.add(jLabel1, new XYConstraints(55, 90, 121, 27));
    jPanel8.add(jButton2, new XYConstraints(128, 238, -1, -1));
    jPanel8.add(jButton1, new XYConstraints(44, 238, -1, -1));
    this.getContentPane().add(jPanel2, BorderLayout.NORTH);
    jPanel2.add(jButton3, BorderLayout.CENTER);
    DepositPane.add(DepositEditPane, null);
    DepositEditPane.add(depositTextArea, BorderLayout.NORTH);
    DepositPane.add(DepositInputPane, null);
    DepositEditPane.add(depositTextArea, BorderLayout.NORTH);
    DepositInputPane.add(jLabel4, new XYConstraints(51, 42, 103, 27));
    DepositInputPane.add(deposit_Cancle_Button,
                         new XYConstraints(121, 216, -1, -1));
    DepositInputPane.add(deposit_OK_Button, new XYConstraints(44, 216, -1, -1));
    DepositInputPane.add(jLabel8, new XYConstraints(51, 118, 128, -1));
    DepositInputPane.add(depositEdit2, new XYConstraints(51, 148, 127, -1));
    DepositInputPane.add(depositEdit1, new XYConstraints(51, 74, 125, -1));
    queryPane.add(jPanel3, null);
    jPanel3.add(jEditorPane4, BorderLayout.NORTH);
    queryPane.add(jPanel5, null);
    jPanel5.add(queryOkButton, new XYConstraints(44, 216, -1, -1));
    jPanel5.add(queryCancleButton, new XYConstraints(121, 216, -1, -1));
    jPanel5.add(queryEdit,  new XYConstraints(49, 74, 125, -1));
    jPanel5.add(jLabel5, new XYConstraints(51, 42, 103, 27));
    jPanel5.add(jLabel16,  new XYConstraints(49, 112, 125, 22));
    jPanel5.add(jPasswordField3,   new XYConstraints(49, 144, 123, -1));
    jTabbedPane1.add(changepassword, "修改密码");
    jTabbedPane1.add(newAccountPane, "开户");
    jTabbedPane1.add(deleAccountPane, "销户");
    deleAccountPane.add(jPanel15, null);
    jPanel15.add(jTextArea2, BorderLayout.NORTH);
    deleAccountPane.add(jPanel14, null);
    jPanel14.add(jLabel11, new XYConstraints(51, 42, 103, 27));
    jPanel14.add(deleAccount_OK, new XYConstraints(44, 216, -1, -1));
    jPanel14.add(deleAccount_Cancel, new XYConstraints(121, 216, -1, -1));
    jPanel14.add(jTextField4, new XYConstraints(51, 74, 125, -1));
    jTabbedPane1.add(jPanel12, "代交电话费");
    jPanel12.add(jPanel16, null);
    jPanel16.add(jTextArea3, BorderLayout.NORTH);
    jPanel12.add(payPane, null);
    payPane.add(jLabel13, new XYConstraints(51, 118, 99, -1));
    payPane.add(jTextField6, new XYConstraints(51, 148, 127, -1));
    payPane.add(jLabel12, new XYConstraints(51, 42, 103, 27));
    payPane.add(telephone_ok, new XYConstraints(44, 216, -1, -1));
    payPane.add(telephone_cancel, new XYConstraints(121, 216, -1, -1));
    payPane.add(jTextField5, new XYConstraints(51, 74, 125, -1));
    newAccountPane.add(jPanel10, null);
    jPanel10.add(openaccounttextArea, BorderLayout.NORTH);
    newAccountPane.add(jPanel13, null);
    jPanel13.add(jLabel6, new XYConstraints(51, 42, 103, 27));
    jPanel13.add(openAccountOK, new XYConstraints(44, 216, -1, -1));
    jPanel13.add(openAccountCancel, new XYConstraints(121, 216, -1, -1));
    changepassword.add(jPanel9, null);
    jPanel9.add(jTextArea1, BorderLayout.NORTH);
    changepassword.add(jPanel6, null);
    jPanel6.add(jLabel9, new XYConstraints(51, 42, 103, 27));
    jPanel6.add(jTextField3, new XYConstraints(51, 74, 125, -1));
    jPanel6.add(jLabel10, new XYConstraints(51, 118, 99, -1));
    jPanel6.add(changepass_ok, new XYConstraints(44, 216, -1, -1));
    jPanel6.add(changepass_Cancel, new XYConstraints(121, 216, -1, -1));
    jPanel6.add(jPasswordField1, new XYConstraints(51, 148, 127, -1));
    jPanel13.add(jPasswordField2, new XYConstraints(51, 74, 125, -1));
  }

  void openAccountOK_actionPerformed(ActionEvent e) {
    String password = "";
    password = jPasswordField2.getText();
    String newaccount = "";
    newaccount = newAccount(password);
    openaccounttextArea.setText("");
    openaccounttextArea.append("" + '\n' + '\n' + '\n' );
    openaccounttextArea.append("您的帐户号码是:  " + newaccount + '\n');
    openaccounttextArea.append("谢谢您的光临!");
//    FileOutputStream file = null;
//    try {
//      file = new FileOutputStream("account.txt");
//    }
//    catch (FileNotFoundException ex) {
//      JOptionPane.showMessageDialog(this,"the account.txt file is not exist!");
//    }
//    java.io.DataOutputStream fileout = new java.io.DataOutputStream(file);
//    try {
//      fileout.write(newaccount);
//      System.out.println("create a new account is:"+newaccount);
//    }
//    catch (IOException ex1) {
//      JOptionPane.showMessageDialog(this,"writeing account.txt failure!");
//    }
    FileWriter fw = null;
    File file;

    try {
      fw = new FileWriter("account.txt", true);
    }
    catch (IOException ex) {
      JOptionPane.showMessageDialog(this, "the account.txt file is not exist!");
    }
    BufferedWriter bw = new BufferedWriter(fw, 1024);
    try {
      bw.newLine();
      bw.write(newaccount);
    }
    catch (IOException ex1) {
    }
    try {
      bw.flush();
      fw.close();
    }
    catch (IOException ex2) {
    }
    jPasswordField2.setText("");

  }

  public void withdraw(String id, int amount) {

    NameComponent name[] = new NameComponent[1];
    name[0] = new NameComponent();
    name[0].id = "Manager";
    name[0].kind = "";
    org.omg.CORBA.Object obj = null;
    try {
      obj = nc.resolve(name);
    }
    catch (InvalidName ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    catch (CannotProceed ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    catch (NotFound ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    openManager = ManagerHelper.narrow(obj);

    try {

      managerop = openManager.provide_manager_op();
    }
    catch (Exception ex1) {
      System.out.println("failure..");
    }
    try {
      managerop.withdraw(id, amount);
    }
    catch (Exception ex2) {
      JOptionPane.showMessageDialog(this, "withdraw failure!");
    }
    System.out.println("withdraw success...");

  }

  void With_OK_Button_actionPerformed(ActionEvent e) {
    withdrawTextArea.setText("");
    String id_ = withdrawdEdit1.getText();
    String amount_ = withdrawdEdit2.getText();
    String password = jPasswordField4.getText();
    if (id_.equals("")) {
      JOptionPane.showMessageDialog(this, "请输入卡号");
      return;
    }
    if (amount_.equals("")) {
      JOptionPane.showMessageDialog(this, "请输入取款金额");
      return;
    }
    if (password.equals("")) {
     JOptionPane.showMessageDialog(this, "请输入取款金额");
     return;
   }

    Integer tem = Integer.valueOf(amount_);
    int tem2 = tem.intValue();
    withdraw(id_, tem2);
    withdrawTextArea.append("" + '\n' + '\n' + '\n');
    withdrawTextArea.append("您本次取款金额为:  " + tem2 + '\n');
    withdrawTextArea.append("谢谢您的光临!");
    withdrawdEdit1.setText("");
    withdrawdEdit2.setText("");
    jPasswordField4.setText("");

  }

  public void deposit(String id, int amount) {

    NameComponent name[] = new NameComponent[1];
    name[0] = new NameComponent();
    name[0].id = "Manager";
    name[0].kind = "";
    org.omg.CORBA.Object obj = null;
    try {
      obj = nc.resolve(name);
    }
    catch (InvalidName ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    catch (CannotProceed ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    catch (NotFound ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    openManager = ManagerHelper.narrow(obj);

    try {

      managerop = openManager.provide_manager_op();
    }
    catch (Exception ex1) {
      System.out.println("failure..");
    }
    try {
      managerop.deposit(id, amount);
    }
    catch (Exception ex2) {
      JOptionPane.showMessageDialog(this, "deposit failure!");
    }
    System.out.println("deposit success...");

  }

  void deposit_OK_Button_actionPerformed(ActionEvent e) {
    depositTextArea.setText("");
    String id_ = depositEdit1.getText();
    String amount_ = depositEdit2.getText();
    if (id_.equals("")) {
      JOptionPane.showMessageDialog(this, "请输入卡号");
      return;
    }
    if (amount_.equals("")) {
      JOptionPane.showMessageDialog(this, "请输入存款金额");
      return;
    }
    Integer tem = Integer.valueOf(amount_);
    int tem2 = tem.intValue();
    deposit(id_, tem2);
    depositTextArea.append("" + '\n' + '\n' + '\n');
    depositTextArea.append("您本次存款金额为:  " + tem2 + '\n');
    depositTextArea.append("谢谢您的光临!");
    depositEdit1.setText("");
    depositEdit2.setText("");

  }

  void with_Cancel_Button_actionPerformed(ActionEvent e) {
    this.dispose();
    String args[] = new String[2];
    args[0] = domainIp;
    args[1] = port;
    Manager manager = new Manager();
    Login login = new Login(null, manager, args);

  }

  void queryOkButton_actionPerformed(ActionEvent e) {
    withdrawTextArea.setText("");
    String id = queryEdit.getText();
    int balance = query(id);
    jEditorPane4.setText("");
    jEditorPane4.append(""+'\n' + '\n' + '\n' + "您目前帐户余额为:" + balance);
    queryEdit.setText("");
    jPasswordField3.setText("");

  }

  public int query(String id) {
    NameComponent name[] = new NameComponent[1];
    name[0] = new NameComponent();
    name[0].id = "Manager";
    name[0].kind = "";
    org.omg.CORBA.Object obj = null;
    try {
      obj = nc.resolve(name);
    }
    catch (InvalidName ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
      return -1;
    }
    catch (CannotProceed ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
      return -1;
    }
    catch (NotFound ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
      return -1;
    }
    openManager = ManagerHelper.narrow(obj);

    try {

      managerop = openManager.provide_manager_op();
    }
    catch (Exception ex1) {
      System.out.println("failure..");
      return -1;
    }

    try {

      int tem = managerop.getBalance(id);
      System.out.println("query success...");
      return tem;
    }
    catch (Exception ex2) {
      JOptionPane.showMessageDialog(this, "query failure!");
      return -1;
    }

  }

  void changepass_ok_actionPerformed(ActionEvent e) {
    String id = jTextField3.getText();
    String newpass = jPasswordField1.getText();
    changepassword(id, newpass);
    jTextArea1.setText("");
    jTextArea1.append("\n\n\n您的密码修改成功!请记住新密码!");
    jTextField3.setText("");
    jPasswordField1.setText("");
    jTextField3.setText("");
    jPasswordField1.setText("");

  }

  public void changepassword(String id, String newpss) {
    NameComponent name[] = new NameComponent[1];
    name[0] = new NameComponent();
    name[0].id = "Manager";
    name[0].kind = "";
    org.omg.CORBA.Object obj = null;
    try {
      obj = nc.resolve(name);
    }
    catch (InvalidName ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    catch (CannotProceed ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    catch (NotFound ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    openManager = ManagerHelper.narrow(obj);

    try {

      managerop = openManager.provide_manager_op();
    }
    catch (Exception ex1) {
      System.out.println("failure..");
    }

    managerop.changePassword(id, newpss);
    System.out.println("change password success!");

  }

  void changepass_Cancel_actionPerformed(ActionEvent e) {
    this.dispose();
    String args[] = null;
    Manager manager = new Manager();
    Login login = new Login(null, manager, args);

  }

  public void deleaccount(String id) {
    NameComponent name[] = new NameComponent[1];
    name[0] = new NameComponent();
    name[0].id = "Manager";
    name[0].kind = "";
    org.omg.CORBA.Object obj = null;
    try {
      obj = nc.resolve(name);
    }
    catch (InvalidName ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    catch (CannotProceed ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    catch (NotFound ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    openManager = ManagerHelper.narrow(obj);

    try {

      managerop = openManager.provide_manager_op();
    }
    catch (Exception ex1) {
      System.out.println("failure..");
    }

    managerop.deleteAccount(id);
    System.out.println("delete account successful!");

  }

  void deleAccount_OK_actionPerformed(ActionEvent e) {
    String id = jTextField4.getText();
    deleaccount(id);
    jTextField4.setText("");
    jTextArea2.setText("");
    jTextArea2.append("" + '\n' + '\n' + '\n' + "账号" + id + "已经被销户!");
  }

  public void payForPhone(String id, int amount) {
    newnamecontext();
    NameComponent name[] = new NameComponent[1];
    name[0] = new NameComponent();
    name[0].id = "Delegation";
    name[0].kind = "";
    org.omg.CORBA.Object obj = null;
    try {
      obj = nc.resolve(name);
    }
    catch (InvalidName ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    catch (CannotProceed ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    catch (NotFound ex) {
      JOptionPane.showMessageDialog(this, "系统目前不支持这项服务");
      jTextArea3.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");
      return;

    }
    delegation = DelegationHelper.narrow(obj);

    try {

      delegationop = delegation.provide_delegation_Op();
    }
    catch (Exception ex1) {
      System.out.println("failure..");
    }

    delegationop.payForPhone(id, amount);
    jTextArea3.setText(""+"\n\n\n交费成功！");
    System.out.println("payForPhoned success!");

  }

  void telephone_ok_actionPerformed(ActionEvent e) {
    jTextArea3.setText("");
    String id = jTextField5.getText();
    String tem = jTextField6.getText();
    Integer tem2 = new Integer(tem);

    int amount = tem2.intValue();
    payForPhone(id, amount);
    jTextField5.setText("");
    jTextField6.setText("");


  }

  void deposit_Cancle_Button_actionPerformed(ActionEvent e) {
    this.dispose();
    String args[] = null;
    Manager manager = new Manager();
    Login login = new Login(null, manager, args);

  }

  void queryCancleButton_actionPerformed(ActionEvent e) {
    this.dispose();
    String args[] = null;
    Manager manager = new Manager();
    Login login = new Login(null, manager, args);

  }

  void jButton2_actionPerformed(ActionEvent e) {
    this.dispose();
    String args[] = null;
    Manager manager = new Manager();
    Login login = new Login(null, manager, args);

  }

  void openAccountCancel_actionPerformed(ActionEvent e) {
    this.dispose();
    String args[] = null;
    Manager manager = new Manager();
    Login login = new Login(null, manager, args);

  }

  void deleAccount_Cancel_actionPerformed(ActionEvent e) {
    this.dispose();
    String args[] = null;
    Manager manager = new Manager();
    Login login = new Login(null, manager, args);

  }

  void telephone_cancel_actionPerformed(ActionEvent e) {
    this.dispose();
    String args[] = null;
    Manager manager = new Manager();
    Login login = new Login(null, manager, args);

  }

  void jButton1_actionPerformed(ActionEvent e) {
    String targetid = jTextField1.getText();
    String id = jTextField7.getText();
    String amount = jTextField2.getText();
    if (targetid.equals("")) {
      JOptionPane.showMessageDialog(this, "请输入转入卡卡号");
      return;
    }
    if (id.equals("")) {
      JOptionPane.showMessageDialog(this, "请输入您的卡号");
      return;
    }
    if (amount.equals("")) {
      JOptionPane.showMessageDialog(this, "请输入转帐金额");
      return;
    }

    Integer tem = Integer.valueOf(amount);
    int tem2 = tem.intValue();

    transfer(id, targetid, tem2);
    jEditorPane1.append("" + '\n' + '\n' + '\n');
    jEditorPane1.append("您本次取款金额为:  " + tem2 + '\n');
    jEditorPane1.append("谢谢您的光临!");



  }

  public void transfer(String id, String targetid, int amount) {
    NameComponent name[] = new NameComponent[1];
    name[0] = new NameComponent();
    name[0].id = "Manager";
    name[0].kind = "";
    org.omg.CORBA.Object obj = null;
    try {
      obj = nc.resolve(name);
    }
    catch (InvalidName ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    catch (CannotProceed ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    catch (NotFound ex) {
      JOptionPane.showMessageDialog(this, "init NamingService failure!");
    }
    openManager = ManagerHelper.narrow(obj);

    try {

      managerop = openManager.provide_manager_op();
    }
    catch (Exception ex1) {
      System.out.println("failure..");
    }

    try {

      managerop.transfer(id, targetid, amount);
    }
    catch (Exception ex2) {
      JOptionPane.showMessageDialog(this, "transfer failure!");

    }

    System.out.println("transfer success...");

  }

  void jTabbedPane1_focusGained(FocusEvent e) {


  }

  void WithdrawPane_focusGained(FocusEvent e) {
      withdrawTextArea.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");

  }

  void DepositPane_focusGained(FocusEvent e) {
      depositTextArea.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");

  }

  void queryPane_focusGained(FocusEvent e) {
     jEditorPane4.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");

  }

  void jPanel4_focusGained(FocusEvent e) {
       jEditorPane1.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");
  }

  void changepassword_focusGained(FocusEvent e) {
      jTextArea1.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");
  }

  void newAccountPane_focusGained(FocusEvent e) {
     openaccounttextArea.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");
  }

  void deleAccountPane_focusGained(FocusEvent e) {
     jTextArea2.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");
  }

  void jPanel12_focusGained(FocusEvent e) {
     jTextArea3.setText("\n\n\n给客户一个完美的服务!\n给社会一个真挚的回馈!\n给员工一个创造的空间!");
  }
}

class Manager_openAccountOK_actionAdapter
    implements java.awt.event.ActionListener {
  Manager adaptee;

  Manager_openAccountOK_actionAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.openAccountOK_actionPerformed(e);
  }
}

class Manager_With_OK_Button_actionAdapter
    implements java.awt.event.ActionListener {
  Manager adaptee;

  Manager_With_OK_Button_actionAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.With_OK_Button_actionPerformed(e);
  }
}

class Manager_deposit_OK_Button_actionAdapter
    implements java.awt.event.ActionListener {
  Manager adaptee;

  Manager_deposit_OK_Button_actionAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.deposit_OK_Button_actionPerformed(e);
  }
}

class Manager_with_Cancel_Button_actionAdapter
    implements java.awt.event.ActionListener {
  Manager adaptee;

  Manager_with_Cancel_Button_actionAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.with_Cancel_Button_actionPerformed(e);
  }
}

class Manager_queryOkButton_actionAdapter
    implements java.awt.event.ActionListener {
  Manager adaptee;

  Manager_queryOkButton_actionAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.queryOkButton_actionPerformed(e);
  }
}

class Manager_changepass_ok_actionAdapter
    implements java.awt.event.ActionListener {
  Manager adaptee;

  Manager_changepass_ok_actionAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.changepass_ok_actionPerformed(e);
  }
}

class Manager_changepass_Cancel_actionAdapter
    implements java.awt.event.ActionListener {
  Manager adaptee;

  Manager_changepass_Cancel_actionAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.changepass_Cancel_actionPerformed(e);
  }
}

class Manager_deleAccount_OK_actionAdapter
    implements java.awt.event.ActionListener {
  Manager adaptee;

  Manager_deleAccount_OK_actionAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.deleAccount_OK_actionPerformed(e);
  }
}

class Manager_telephone_ok_actionAdapter
    implements java.awt.event.ActionListener {
  Manager adaptee;

  Manager_telephone_ok_actionAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.telephone_ok_actionPerformed(e);
  }
}

class Manager_deposit_Cancle_Button_actionAdapter
    implements java.awt.event.ActionListener {
  Manager adaptee;

  Manager_deposit_Cancle_Button_actionAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.deposit_Cancle_Button_actionPerformed(e);
  }
}

class Manager_queryCancleButton_actionAdapter
    implements java.awt.event.ActionListener {
  Manager adaptee;

  Manager_queryCancleButton_actionAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.queryCancleButton_actionPerformed(e);
  }
}

class Manager_jButton2_actionAdapter
    implements java.awt.event.ActionListener {
  Manager adaptee;

  Manager_jButton2_actionAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jButton2_actionPerformed(e);
  }
}

class Manager_openAccountCancel_actionAdapter
    implements java.awt.event.ActionListener {
  Manager adaptee;

  Manager_openAccountCancel_actionAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.openAccountCancel_actionPerformed(e);
  }
}

class Manager_deleAccount_Cancel_actionAdapter
    implements java.awt.event.ActionListener {
  Manager adaptee;

  Manager_deleAccount_Cancel_actionAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.deleAccount_Cancel_actionPerformed(e);
  }
}

class Manager_telephone_cancel_actionAdapter
    implements java.awt.event.ActionListener {
  Manager adaptee;

  Manager_telephone_cancel_actionAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.telephone_cancel_actionPerformed(e);
  }
}

class Manager_jButton1_actionAdapter
    implements java.awt.event.ActionListener {
  Manager adaptee;

  Manager_jButton1_actionAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jButton1_actionPerformed(e);
  }
}

class Manager_jTabbedPane1_focusAdapter extends java.awt.event.FocusAdapter {
  Manager adaptee;

  Manager_jTabbedPane1_focusAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }
  public void focusGained(FocusEvent e) {
    adaptee.jTabbedPane1_focusGained(e);
  }
}

class Manager_WithdrawPane_focusAdapter extends java.awt.event.FocusAdapter {
  Manager adaptee;

  Manager_WithdrawPane_focusAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }
  public void focusGained(FocusEvent e) {
    adaptee.WithdrawPane_focusGained(e);
  }
}

class Manager_DepositPane_focusAdapter extends java.awt.event.FocusAdapter {
  Manager adaptee;

  Manager_DepositPane_focusAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }
  public void focusGained(FocusEvent e) {
    adaptee.DepositPane_focusGained(e);
  }
}

class Manager_queryPane_focusAdapter extends java.awt.event.FocusAdapter {
  Manager adaptee;

  Manager_queryPane_focusAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }
  public void focusGained(FocusEvent e) {
    adaptee.queryPane_focusGained(e);
  }
}

class Manager_jPanel4_focusAdapter extends java.awt.event.FocusAdapter {
  Manager adaptee;

  Manager_jPanel4_focusAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }
  public void focusGained(FocusEvent e) {
    adaptee.jPanel4_focusGained(e);
  }
}

class Manager_changepassword_focusAdapter extends java.awt.event.FocusAdapter {
  Manager adaptee;

  Manager_changepassword_focusAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }
  public void focusGained(FocusEvent e) {
    adaptee.changepassword_focusGained(e);
  }
}

class Manager_newAccountPane_focusAdapter extends java.awt.event.FocusAdapter {
  Manager adaptee;

  Manager_newAccountPane_focusAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }
  public void focusGained(FocusEvent e) {
    adaptee.newAccountPane_focusGained(e);
  }
}

class Manager_deleAccountPane_focusAdapter extends java.awt.event.FocusAdapter {
  Manager adaptee;

  Manager_deleAccountPane_focusAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }
  public void focusGained(FocusEvent e) {
    adaptee.deleAccountPane_focusGained(e);
  }
}

class Manager_jPanel12_focusAdapter extends java.awt.event.FocusAdapter {
  Manager adaptee;

  Manager_jPanel12_focusAdapter(Manager adaptee) {
    this.adaptee = adaptee;
  }
  public void focusGained(FocusEvent e) {
    adaptee.jPanel12_focusGained(e);
  }
}