package bank.Ui;

import javax.swing.*;
import java.awt.*;
import javax.swing.border.*;
import com.borland.jbcl.layout.*;
import org.omg.CORBA.*;
import java.util.*;
import org.omg.CORBA.ORBPackage.*;
import org.omg.CosNaming.*;
import org.omg.CosNaming.NamingContextPackage.InvalidName;
import org.omg.CosNaming.NamingContextPackage.*;
import org.omg.CosNaming.NamingContextPackage.*;
import bank.Bank.*;
import java.awt.event.*;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: </p>
 * @author not attributable
 * @version 1.0
 */

public class Atm
    extends JFrame {
  TitledBorder titledBorder1;
  BorderLayout borderLayout1 = new BorderLayout();
  JPanel jPanel2 = new JPanel();
  JPanel jPanel3 = new JPanel();
  JButton jButton9 = new JButton();
  GridLayout gridLayout1 = new GridLayout();
  JPanel jPanel1 = new JPanel();
  JButton jButton2 = new JButton();
  JButton Atm_Cancel = new JButton();
  JButton jButton4 = new JButton();
  JButton jButton7 = new JButton();
  JButton jButton1 = new JButton();
  JButton Atm_OK = new JButton();
  JButton jButton5 = new JButton();
  JButton jButton8 = new JButton();
  JButton jButton0 = new JButton();
  JButton jButton3 = new JButton();
  JButton jButton6 = new JButton();
  BorderLayout borderLayout2 = new BorderLayout();
  JPanel jPanel4 = new JPanel();
  XYLayout xYLayout1 = new XYLayout();
  JButton withdrawButton = new JButton();
  JButton depositButton = new JButton();
  JButton queryButton = new JButton();
  JButton changepasswordButton = new JButton();
  JLabel jLabel2 = new JLabel();
  JLabel jLabel3 = new JLabel();
  JLabel jLabel4 = new JLabel();
  JLabel jLabel5 = new JLabel();
  BorderLayout borderLayout3 = new BorderLayout();
  JPanel jPanel5 = new JPanel();
  JPanel jPanel6 = new JPanel();
  PaneLayout paneLayout1 = new PaneLayout();
  BorderLayout borderLayout4 = new BorderLayout();
  JButton jButton15 = new JButton();
  JButton transactionButton = new JButton();
  JLabel jLabel1 = new JLabel();

  //difinition
  private NamingContext nc = null;
  private String amount = "";
  JTextArea jEditorPane1 = new JTextArea();
  //Client account
  private String atmAccounId;
  //transfer service
  private int tansferflag = 0;
  private boolean isTransfer = false;
  private boolean isinputAccount = false;
  private boolean isinputAmount = false;
  private String transAmount = "";
  private String transferToID = "";
  //deposit servic
  private int depositflag = 0;
  private boolean isDeposit = false;
  private boolean isinputDepositAmount = false;
  private String depositAmount = "";

  // withdraw service
  private int withdrawFlag = 0;
  private boolean isWithdraw = false;
  private boolean isinputWithdrawAmount = false;
  private String WithdrawAmount = "";

// change password service
  private int ChangepassFlag = 0;
  private boolean isChangepass = false;
  private boolean isinputfirstpass = false;
  private boolean isinputsecondpass = false;
  private String firinputnewpass = "";
  private String secondinputnewpass = "";

  //query service
  private int balance;
  private boolean queryFlag = false;
  //
  private bank.Bank.Manager openManager = null;
  private ManagerOp managerop;

  private bank.Bank.ATM atm = null;
  private ATMOp atmop;
  private static String domainIp;
  private static String servicePort;
  public Atm() {

    try {

      jbInit();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
  }

  public void setNamingContext(NamingContext nc_) {
    nc = nc_;

  }

  public void setAtmaccountId(String id) {
    this.atmAccounId = id;
  }
  public void setDomainIp(String ip){
   this.domainIp = ip;
 }
 public void setServicePort(String port){
  this.servicePort = port;
}


  private void jbInit() throws Exception {
    ImageIcon image1 = new ImageIcon(Manager.class.getResource("atm.jpg"));
    ImageIcon image2 = new ImageIcon(Manager.class.getResource("allinone.jpg"));
    //ImageIcon image3 = new ImageIcon(Manager.class.getResource("dl.jpg"));
    titledBorder1 = new TitledBorder(BorderFactory.createEtchedBorder(Color.
        white, new Color(148, 145, 140)), "键盘");
    this.getContentPane().setLayout(borderLayout1);
    titledBorder1.setTitle("键盘");
    titledBorder1.setTitleFont(new java.awt.Font("Dialog", 0, 12));
    jButton9.setBackground(new Color(141, 191, 238));
    //jButton8.setMaximumSize(new Dimension(31, 15));
    //jButton8.setMinimumSize(new Dimension(31, 15));
    jButton9.setPreferredSize(new Dimension(31, 15));
    jButton9.setText("9");
    jButton9.addActionListener(new Atm_jButton9_actionAdapter(this));
    gridLayout1.setColumns(3);
    gridLayout1.setHgap(2);
    gridLayout1.setRows(4);
    gridLayout1.setVgap(2);
    jPanel1.setBackground(new Color(141, 191, 238));
    jPanel1.setBorder(titledBorder1);
    jPanel1.setDebugGraphicsOptions(0);
    jPanel1.setMaximumSize(new Dimension(193, 142));
    jPanel1.setPreferredSize(new Dimension(193, 142));
    jPanel1.setLayout(gridLayout1);
    jButton2.setBackground(new Color(141, 191, 238));
    jButton2.setMaximumSize(new Dimension(31, 15));
    jButton2.setMinimumSize(new Dimension(31, 15));
    jButton2.setPreferredSize(new Dimension(31, 15));
    jButton2.setText("2");
    jButton2.addActionListener(new Atm_jButton2_actionAdapter(this));
    Atm_Cancel.setBackground(new Color(141, 191, 238));
    Atm_Cancel.setFont(new java.awt.Font("Dialog", 0, 12));
    Atm_Cancel.setMaximumSize(new Dimension(31, 15));
    Atm_Cancel.setMinimumSize(new Dimension(31, 15));
    Atm_Cancel.setPreferredSize(new Dimension(31, 15));
    Atm_Cancel.setToolTipText("");
    Atm_Cancel.setText("退出");
    Atm_Cancel.addActionListener(new Atm_Atm_Cancel_actionAdapter(this));
    jButton4.setBackground(new Color(141, 191, 238));
    jButton4.setMaximumSize(new Dimension(31, 15));
    jButton4.setMinimumSize(new Dimension(31, 15));
    jButton4.setPreferredSize(new Dimension(31, 15));
    jButton4.setText("4");
    jButton4.addActionListener(new Atm_jButton4_actionAdapter(this));
    jButton7.setBackground(new Color(141, 191, 238));
    jButton7.setMaximumSize(new Dimension(31, 15));
    jButton7.setMinimumSize(new Dimension(31, 15));
    jButton7.setPreferredSize(new Dimension(31, 15));
    jButton7.setText("7");
    jButton7.addActionListener(new Atm_jButton7_actionAdapter(this));
    jButton1.setBackground(new Color(141, 191, 238));
    jButton1.setMaximumSize(new Dimension(31, 15));
    jButton1.setMinimumSize(new Dimension(31, 15));
    jButton1.setPreferredSize(new Dimension(31, 15));
    jButton1.setMnemonic('0');
    jButton1.setText("1");
    jButton1.addActionListener(new Atm_jButton1_actionAdapter(this));
    Atm_OK.setBackground(new Color(141, 191, 238));
    Atm_OK.setFont(new java.awt.Font("Dialog", 0, 12));
    Atm_OK.setMaximumSize(new Dimension(31, 15));
    Atm_OK.setMinimumSize(new Dimension(31, 15));
    Atm_OK.setPreferredSize(new Dimension(31, 15));
    Atm_OK.setActionCommand("确定");
    Atm_OK.setText("确定");
    Atm_OK.addActionListener(new Atm_Atm_OK_actionAdapter(this));
    jButton5.setBackground(new Color(141, 191, 238));
    jButton5.setMaximumSize(new Dimension(31, 15));
    jButton5.setMinimumSize(new Dimension(31, 15));
    jButton5.setPreferredSize(new Dimension(31, 15));
    jButton5.setText("5");
    jButton5.addActionListener(new Atm_jButton5_actionAdapter(this));
    jButton8.setBackground(new Color(141, 191, 238));
    jButton8.setMaximumSize(new Dimension(31, 15));
    jButton8.setMinimumSize(new Dimension(31, 15));
    jButton8.setPreferredSize(new Dimension(31, 15));
    jButton8.setText("8");
    jButton8.addActionListener(new Atm_jButton8_actionAdapter(this));
    jButton0.setBackground(new Color(141, 191, 238));
    jButton0.setMaximumSize(new Dimension(31, 15));
    jButton0.setMinimumSize(new Dimension(31, 15));
    jButton0.setPreferredSize(new Dimension(31, 15));
    jButton0.setText("0");
    jButton0.addActionListener(new Atm_jButton0_actionAdapter(this));
    jButton3.setBackground(new Color(141, 191, 238));
    jButton3.setMaximumSize(new Dimension(31, 15));
    jButton3.setMinimumSize(new Dimension(31, 15));
    jButton3.setPreferredSize(new Dimension(31, 15));
    jButton3.setText("3");
    jButton3.addActionListener(new Atm_jButton3_actionAdapter(this));
    jButton6.setBackground(new Color(141, 191, 238));
    jButton6.setMaximumSize(new Dimension(31, 15));
    jButton6.setMinimumSize(new Dimension(31, 15));
    jButton6.setPreferredSize(new Dimension(31, 15));
    jButton6.setText("6");
    jButton6.addActionListener(new Atm_jButton6_actionAdapter(this));
    jPanel2.setLayout(borderLayout2);
    jPanel4.setBackground(new Color(141, 191, 238));
    jPanel4.setDebugGraphicsOptions(0);
    jPanel4.setLayout(xYLayout1);
    changepasswordButton.setBackground(new Color(141, 191, 238));
    changepasswordButton.setActionCommand("jButton17");
    changepasswordButton.setText("");
    changepasswordButton.addActionListener(new
        Atm_changepasswordButton_actionAdapter(this));
    jLabel2.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel2.setText("转帐");
    jLabel3.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel3.setText("取钱");
    jLabel4.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel4.setText("存钱");
    jLabel5.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel5.setText("查询");
    jPanel3.setDebugGraphicsOptions(0);
    jPanel3.setLayout(borderLayout3);
    jPanel5.setMinimumSize(new Dimension(10, 10));
    jPanel5.setLayout(paneLayout1);

    jPanel6.setLayout(borderLayout4);
    withdrawButton.setBackground(new Color(141, 191, 238));
    withdrawButton.setText("");
    withdrawButton.addActionListener(new Atm_withdrawButton_actionAdapter(this));
    depositButton.setBackground(new Color(141, 191, 238));
    depositButton.setText("");
    depositButton.addActionListener(new Atm_depositButton_actionAdapter(this));
    queryButton.setBackground(new Color(141, 191, 238));
    queryButton.setText("");
    queryButton.addActionListener(new Atm_queryButton_actionAdapter(this));
    jButton15.setEnabled(true);
    jButton15.setBorder(null);
    jButton15.setFocusPainted(false);
    jButton15.setIcon(image1);
    jButton15.setText("");
    transactionButton.setBackground(new Color(141, 191, 238));
    transactionButton.setText("");
    transactionButton.addActionListener(new Atm_transactionButton_actionAdapter(this));
    jLabel1.setFont(new java.awt.Font("Dialog", 0, 12));
    jLabel1.setText("修改密码");
    this.setEnabled(true);
    this.setResizable(false);
    //jEditorPane1.setText("尊贵的客户，您好！欢迎使用ATM取款机，转帐金额必须为50元的整数倍！");
    jEditorPane1.setBackground(Color.white);
    jEditorPane1.setEnabled(true);
    jEditorPane1.setFont(new java.awt.Font("Dialog", 0, 15));
    jEditorPane1.setForeground(new Color(43, 64, 169));
    jEditorPane1.setAlignmentX( (float) 0.5);
    jEditorPane1.setAlignmentY( (float) 0.5);
    jEditorPane1.setBorder(BorderFactory.createEtchedBorder());
    jEditorPane1.setDebugGraphicsOptions(0);
    jEditorPane1.setDoubleBuffered(false);
    jEditorPane1.setToolTipText("");
    jEditorPane1.setCaretColor(Color.black);
    jEditorPane1.setDisabledTextColor(Color.gray);
    jEditorPane1.setEditable(true);
    //jEditorPane1.setCaretPosition(57);
    jEditorPane1.setText("" + '\n' + '\n' + '\n' + "尊贵的客户，您好！欢迎使用"+'\n'+"ATM自动取款机!" +
                         '\n' + "ATM机提供如下服务:"+'\n'+'\t'+"取款"+'\n'+'\t'+"存款"+'\n'+'\t'+"查询"+'\n'+'\t'+"转帐"+'\n'+'\t'+"修改密码！");
    jEditorPane1.setTabSize(8);
    this.getContentPane().add(jPanel2, BorderLayout.EAST);
    jPanel1.add(jButton1, null);
    jPanel1.add(jButton2, null);
    jPanel1.add(jButton3, null);
    jPanel1.add(jButton4, null);
    jPanel1.add(jButton5, null);
    jPanel1.add(jButton6, null);
    jPanel1.add(jButton7, null);
    jPanel1.add(jButton8, null);
    jPanel1.add(jButton9, null);
    jPanel1.add(Atm_Cancel, null);
    jPanel1.add(jButton0, null);
    jPanel1.add(Atm_OK, null);
    jPanel2.add(jPanel4, BorderLayout.NORTH);
    jPanel4.add(changepasswordButton,   new XYConstraints(75, 176, 75, 25));
    jPanel4.add(queryButton,         new XYConstraints(75, 139, 75, 25));
    jPanel4.add(depositButton,         new XYConstraints(75, 103, 75, 25));
    jPanel4.add(withdrawButton,         new XYConstraints(75, 66, 75, 25));
    jPanel4.add(jLabel2,  new XYConstraints(17, 27, 46, -1));
    jPanel4.add(jLabel3,  new XYConstraints(17, 67, 46, -1));
    jPanel4.add(jLabel4, new XYConstraints(17, 103, 46, -1));
    jPanel4.add(jLabel5,  new XYConstraints(17, 141, 46, -1));
    jPanel4.add(jLabel1,  new XYConstraints(17, 179, 50, -1));
    jPanel4.add(transactionButton,     new XYConstraints(75, 29, 75, 25));
    jPanel2.add(jPanel1, BorderLayout.CENTER);
    this.getContentPane().add(jPanel3, BorderLayout.CENTER);
    jPanel3.add(jPanel5, BorderLayout.CENTER);
    jPanel5.add(jEditorPane1,
                new PaneConstraints("jTextArea1", "jTextArea1",
                                    PaneConstraints.ROOT, 1.0f));
    jPanel3.add(jPanel6, BorderLayout.NORTH);
    jPanel6.add(jButton15, BorderLayout.NORTH);
  }

  void jButton1_actionPerformed(ActionEvent e) {
    if (isTransfer && isinputAccount) {
      transferToID = transferToID + "1";
      //jEditorPane1.setText("");
      jEditorPane1.append("1");
    }
    if (isTransfer && isinputAmount) {
      transAmount = transAmount + "1";
      //jEditorPane1.setText("");
      jEditorPane1.append("1");
    }
    if (isDeposit && isinputDepositAmount) {
      depositAmount = depositAmount + "1";
      //jEditorPane1.setText("");
      jEditorPane1.append("1");
    }
    if (isWithdraw && isinputWithdrawAmount) {
      WithdrawAmount = WithdrawAmount + "1";
      //jEditorPane1.setText("");
      jEditorPane1.append("1");
    }
    if (isChangepass && isinputfirstpass) {
      firinputnewpass = firinputnewpass + "1";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }
    if (isChangepass && isinputsecondpass) {
      secondinputnewpass = secondinputnewpass + "1";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }

  }

  void jButton2_actionPerformed(ActionEvent e) {
    if (isTransfer && isinputAccount) {
      transferToID = transferToID + "2";
      //jEditorPane1.setText("");
      jEditorPane1.append("2");
    }
    if (isTransfer && isinputAmount) {
      transAmount = transAmount + "2";
      //jEditorPane1.setText("");
      jEditorPane1.append("2");
    }
    if (isDeposit && isinputDepositAmount) {
      depositAmount = depositAmount + "2";
      //jEditorPane1.setText("");
      jEditorPane1.append("2");
    }
    if (isWithdraw && isinputWithdrawAmount) {
      WithdrawAmount = WithdrawAmount + "2";
      //jEditorPane1.setText("");
      jEditorPane1.append("2");
    }
    if (isChangepass && isinputfirstpass) {
      firinputnewpass = firinputnewpass + "2";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }
    if (isChangepass && isinputsecondpass) {
      secondinputnewpass = secondinputnewpass + "2";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }

  }

  void jButton3_actionPerformed(ActionEvent e) {
    if (isTransfer && isinputAccount) {
      transferToID = transferToID + "3";
      //jEditorPane1.setText("");
      jEditorPane1.append("3");
    }
    if (isTransfer && isinputAmount) {
      transAmount = transAmount + "3";
      //jEditorPane1.setText("");
      jEditorPane1.append("3");
    }
    if (isDeposit && isinputDepositAmount) {
      depositAmount = depositAmount + "3";
      //jEditorPane1.setText("");
      jEditorPane1.append("3");
    }
    if (isWithdraw && isinputWithdrawAmount) {
      WithdrawAmount = WithdrawAmount + "3";
      //jEditorPane1.setText("");
      jEditorPane1.append("3");
    }
    if (isChangepass && isinputfirstpass) {
      firinputnewpass = firinputnewpass + "3";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }
    if (isChangepass && isinputsecondpass) {
      secondinputnewpass = secondinputnewpass + "3";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }

  }

  void jButton4_actionPerformed(ActionEvent e) {
    if (isTransfer && isinputAccount) {
      transferToID = transferToID + "4";
      //jEditorPane1.setText("");
      jEditorPane1.append("4");
    }
    if (isTransfer && isinputAmount) {
      transAmount = transAmount + "4";
      //jEditorPane1.setText("");
      jEditorPane1.append("4");
    }
    if (isDeposit && isinputDepositAmount) {
      depositAmount = depositAmount + "4";
      //jEditorPane1.setText("");
      jEditorPane1.append("4");
    }
    if (isWithdraw && isinputWithdrawAmount) {
      WithdrawAmount = WithdrawAmount + "4";
      //jEditorPane1.setText("");
      jEditorPane1.append("4");
    }
    if (isChangepass && isinputfirstpass) {
      firinputnewpass = firinputnewpass + "4";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }
    if (isChangepass && isinputsecondpass) {
      secondinputnewpass = secondinputnewpass + "4";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }

  }

  void jButton5_actionPerformed(ActionEvent e) {
    if (isTransfer && isinputAccount) {
      transferToID = transferToID + "5";
      //jEditorPane1.setText("");
      jEditorPane1.append("5");
    }
    if (isTransfer && isinputAmount) {
      transAmount = transAmount + "5";
      //jEditorPane1.setText("");
      jEditorPane1.append("5");
    }
    if (isDeposit && isinputDepositAmount) {
      depositAmount = depositAmount + "5";
      //jEditorPane1.setText("");
      jEditorPane1.append("5");
    }
    if (isWithdraw && isinputWithdrawAmount) {
      WithdrawAmount = WithdrawAmount + "5";
      //jEditorPane1.setText("");
      jEditorPane1.append("5");
    }
    if (isChangepass && isinputfirstpass) {
      firinputnewpass = firinputnewpass + "5";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }
    if (isChangepass && isinputsecondpass) {
      secondinputnewpass = secondinputnewpass + "5";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }

  }

  void jButton6_actionPerformed(ActionEvent e) {
    if (isTransfer && isinputAccount) {
      transferToID = transferToID + "6";
      //jEditorPane1.setText("");
      jEditorPane1.append("6");
    }
    if (isTransfer && isinputAmount) {
      transAmount = transAmount + "6";
      //jEditorPane1.setText("");
      jEditorPane1.append("6");
    }
    if (isDeposit && isinputDepositAmount) {
      depositAmount = depositAmount + "6";
      //jEditorPane1.setText("");
      jEditorPane1.append("6");
    }
    if (isWithdraw && isinputWithdrawAmount) {
      WithdrawAmount = WithdrawAmount + "6";
      //jEditorPane1.setText("");
      jEditorPane1.append("6");
    }
    if (isChangepass && isinputfirstpass) {
      firinputnewpass = firinputnewpass + "6";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }
    if (isChangepass && isinputsecondpass) {
      secondinputnewpass = secondinputnewpass + "6";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }

  }

  void jButton7_actionPerformed(ActionEvent e) {
    if (isTransfer && isinputAccount) {
      transferToID = transferToID + "7";
      //jEditorPane1.setText("");
      jEditorPane1.append("7");
    }
    if (isTransfer && isinputAmount) {
      transAmount = transAmount + "7";
      //jEditorPane1.setText("");
      jEditorPane1.append("7");
    }
    if (isDeposit && isinputDepositAmount) {
      depositAmount = depositAmount + "7";
      //jEditorPane1.setText("");
      jEditorPane1.append("7");
    }
    if (isWithdraw && isinputWithdrawAmount) {
      WithdrawAmount = WithdrawAmount + "7";
      //jEditorPane1.setText("");
      jEditorPane1.append("7");
    }
    if (isChangepass && isinputfirstpass) {
      firinputnewpass = firinputnewpass + "7";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }
    if (isChangepass && isinputsecondpass) {
      secondinputnewpass = secondinputnewpass + "7";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }

  }

  void jButton8_actionPerformed(ActionEvent e) {
    if (isTransfer && isinputAccount) {
      transferToID = transferToID + "8";
      //jEditorPane1.setText("");
      jEditorPane1.append("8");
    }
    if (isTransfer && isinputAmount) {
      transAmount = transAmount + "8";
      //jEditorPane1.setText("");
      jEditorPane1.append("8");
    }
    if (isDeposit && isinputDepositAmount) {
      depositAmount = depositAmount + "8";
      //jEditorPane1.setText("");
      jEditorPane1.append("8");
    }
    if (isWithdraw && isinputWithdrawAmount) {
      WithdrawAmount = WithdrawAmount + "8";
      //jEditorPane1.setText("");
      jEditorPane1.append("8");
    }
    if (isChangepass && isinputfirstpass) {
      firinputnewpass = firinputnewpass + "8";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }
    if (isChangepass && isinputsecondpass) {
      secondinputnewpass = secondinputnewpass + "8";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }

  }

  void jButton9_actionPerformed(ActionEvent e) {
    if (isTransfer && isinputAccount) {
      transferToID = transferToID + "9";
      //jEditorPane1.setText("");
      jEditorPane1.append("9");
    }
    if (isTransfer && isinputAmount) {
      transAmount = transAmount + "9";
      //jEditorPane1.setText("");
      jEditorPane1.append("9");
    }
    if (isDeposit && isinputDepositAmount) {
      depositAmount = depositAmount + "9";
      //jEditorPane1.setText("");
      jEditorPane1.append("9");
    }
    if (isWithdraw && isinputWithdrawAmount) {
      WithdrawAmount = WithdrawAmount + "9";
      //jEditorPane1.setText("");
      jEditorPane1.append("9");
    }
    if (isChangepass && isinputfirstpass) {
      firinputnewpass = firinputnewpass + "9";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }
    if (isChangepass && isinputsecondpass) {
      secondinputnewpass = secondinputnewpass + "9";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }

  }

  void jButton0_actionPerformed(ActionEvent e) {
    if (!transferToID.equals("") && isTransfer && isinputAccount) {
      transferToID = transferToID + "0";
      jEditorPane1.append("0");
    }
    if (!transAmount.equals("") && isTransfer && isinputAmount) {
      transAmount = transAmount + "0";
      jEditorPane1.append("0");
    }
    if (isDeposit && isinputDepositAmount && !depositAmount.equals("")) {
      depositAmount = depositAmount + "0";
      //jEditorPane1.setText("");
      jEditorPane1.append("0");
    }
    if (isWithdraw && isinputWithdrawAmount && !WithdrawAmount.equals("")) {
      WithdrawAmount = WithdrawAmount + "0";
      //jEditorPane1.setText("");
      jEditorPane1.append("0");
    }
    if (isChangepass && isinputfirstpass && !firinputnewpass.equals("")) {
      firinputnewpass = firinputnewpass + "0";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }
    if (isChangepass && isinputsecondpass && !secondinputnewpass.equals("")) {
      secondinputnewpass = secondinputnewpass + "0";
      //jEditorPane1.setText("");
      jEditorPane1.append("*");
    }

  }

  void Atm_OK_actionPerformed(ActionEvent e) {
    if (isTransfer) {
      tansferflag = tansferflag + 1;
    }

    if (tansferflag == 1) {
      jEditorPane1.append("" + '\n');
      jEditorPane1.append("请输入金额:");
      isinputAccount = false;
      isinputAmount = true;
    }
    if (tansferflag == 3) {

      Integer tem = Integer.valueOf(transAmount);
      int tem_amount = tem.intValue();
      transfer(atmAccounId, transferToID, tem_amount);
      jEditorPane1.append("" + '\n' +"转帐成功!" +'\n'+ "继续服务请单击确定!");

    }
    if (tansferflag == 2) {

      isinputAmount = false;
      jEditorPane1.append("" + '\n' + "单击确定进行转帐!");

    }
    if (tansferflag == 4) {
      isTransfer = false;
      tansferflag = 0;
      jEditorPane1.setText("" + '\n' + '\n' + "尊贵的客户，您好！欢迎使用"+'\n'+"ATM自动取款机!" +
                         '\n' + "ATM机提供如下服务:"+'\n'+'\t'+"取款"+'\n'+'\t'+"存款"+'\n'+'\t'+"查询"+'\n'+'\t'+"转帐"+'\n'+'\t'+"修改密码！");


    }
    //depost service
    if (isDeposit) {
      depositflag = depositflag + 1;
    }
    if (depositflag == 3) {
      depositflag = 0;
      isDeposit = false;
      isinputDepositAmount = false;
      depositAmount = "";
      jEditorPane1.setText("" + '\n' + '\n' + "尊贵的客户，您好！欢迎使用"+'\n'+"ATM自动取款机!" +
                         '\n' + "ATM机提供如下服务:"+'\n'+'\t'+"取款"+'\n'+'\t'+"存款"+'\n'+'\t'+"查询"+'\n'+'\t'+"转帐"+'\n'+'\t'+"修改密码！");


    }
    if (depositflag == 1) {

      jEditorPane1.append("" + '\n' +"单击确定进行存款服务!");

    }

    if (depositflag == 2) {
      Integer tem = Integer.valueOf(depositAmount);
      int tem_amount = tem.intValue();
      deposit(tem_amount);
      jEditorPane1.append("" + '\n' +"存款成功!" + "继续服务请单击确定!");
    }
    //withdraw service
    if (isWithdraw) {
      withdrawFlag = withdrawFlag + 1;
    }

    if (withdrawFlag == 1) {

      jEditorPane1.append("" + '\n' +"单击确定进行取款服务!");

    }

    if (withdrawFlag == 2) {
      Integer tem = Integer.valueOf(WithdrawAmount);
      int tem_amount = tem.intValue();
      withdraw(tem_amount);
      jEditorPane1.append("" + '\n' +"取款成功!" + "继续服务请单击确定!");


      WithdrawAmount = "";
      isinputWithdrawAmount = false;

    }
    if (withdrawFlag == 3) {

      jEditorPane1.setText("" +  '\n' + '\n' + "尊贵的客户，您好！欢迎使用"+'\n'+"ATM自动取款机!" +
                         '\n' + "ATM机提供如下服务:"+'\n'+'\t'+"取款"+'\n'+'\t'+"存款"+'\n'+'\t'+"查询"+'\n'+'\t'+"转帐"+'\n'+'\t'+"修改密码！");
       withdrawFlag = 0;
       isWithdraw = false;
       WithdrawAmount = "";


    }
    //change password service
    if (isChangepass) {
      ChangepassFlag += 1;

    }
    if (ChangepassFlag == 1) {
      jEditorPane1.append("" + '\n');
      jEditorPane1.append("请再次输入新的密码:");
      isinputfirstpass = false;
      isinputsecondpass = true;
    }

    if (ChangepassFlag == 2) {

      isinputsecondpass = false;
      jEditorPane1.append("" + '\n' + "单击确定进行密码修改!");

    }
    if (ChangepassFlag == 3) {

      //confirm newpassword
      if (! (secondinputnewpass.equals(firinputnewpass))) {
        JOptionPane jp = new JOptionPane();
        jp.setBackground(new Color(141, 191, 238));
        jp.showMessageDialog(this, "对不起!两次输入密码不一致,请重新输入!");
        ActionEvent teme = null;

        isChangepass = false;
        isinputfirstpass = false;
        isinputsecondpass = false;
        firinputnewpass = "";
        secondinputnewpass = "";

        changepasswordButton_actionPerformed(teme);
        ChangepassFlag = 0;

        return;

      }
      changepassword(atmAccounId, secondinputnewpass);
      jEditorPane1.append("" + '\n' +"密码修改成功!" + "继续服务请单击确定!");

    }

    if (ChangepassFlag == 4) {
      isChangepass = false;
      ChangepassFlag = 0;
      jEditorPane1.setText("" +  '\n' + '\n' + "尊贵的客户，您好！欢迎使用"+'\n'+"ATM自动取款机!" +
                        '\n' + "ATM机提供如下服务:"+'\n'+'\t'+"取款"+'\n'+'\t'+"存款"+'\n'+'\t'+"查询"+'\n'+'\t'+"转帐"+'\n'+'\t'+"修改密码！");
       ChangepassFlag = 0;
       firinputnewpass = "";
        secondinputnewpass = "";

    }
    //query service
    if(queryFlag){
      jEditorPane1.setText("" +  '\n' + '\n' + "尊贵的客户，您好！欢迎使用"+'\n'+"ATM自动取款机!" +
                         '\n' + "ATM机提供如下服务:"+'\n'+'\t'+"取款"+'\n'+'\t'+"存款"+'\n'+'\t'+"查询"+'\n'+'\t'+"转帐"+'\n'+'\t'+"修改密码！");


      queryFlag = false;

    }

  }

  public void changepassword(String id, String newpassword) {
    NameComponent name[] = new NameComponent[1];
    name[0] = new NameComponent();
    name[0].id = "ATM";
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
    atm = ATMHelper.narrow(obj);

    try {

      atmop = atm.provide_ATM_op();
    }
    catch (Exception ex1) {
      System.out.println("failure..");
    }

    try {

      atmop.changePassword(id, newpassword);
    }
    catch (Exception ex2) {
      JOptionPane.showMessageDialog(this, "change password failure!");

    }

    System.out.println(" change password success...");

  }

  public void withdraw(int wihtdrawamount) {
    NameComponent name[] = new NameComponent[1];
    name[0] = new NameComponent();
    name[0].id = "ATM";
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
    atm = ATMHelper.narrow(obj);

    try {

      atmop = atm.provide_ATM_op();
    }
    catch (Exception ex1) {
      System.out.println("failure..");
    }

    try {

      atmop.withdraw(atmAccounId, wihtdrawamount);
    }
    catch (Exception ex2) {
      JOptionPane.showMessageDialog(this, "withdraw failure!");

    }

    System.out.println(" withdraw success...");

  }

  public void transfer(String id, String targetid_, int amount_) {
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

      managerop.transfer(id, targetid_, amount_);
    }
    catch (Exception ex2) {
      JOptionPane.showMessageDialog(this, "transfer failure!");

    }

    System.out.println("transfer success...");

  }

  void Atm_Cancel_actionPerformed(ActionEvent e) {
        this.dispose();
        String args[] = new String[2];
        args[0] = domainIp;
        args[1]= servicePort;
        Atm atm=new Atm();
        Login login = new Login(atm, null,args);

  }

  void transactionButton_actionPerformed(ActionEvent e) {

    isTransfer = true;
    isinputAccount = true;
    jEditorPane1.setText("" + '\n' + '\n' + '\n');
    jEditorPane1.append("请输入对方账号:");
    System.out.println("begin transfer...");

  }

  void withdrawButton_actionPerformed(ActionEvent e) {
    isWithdraw = true;
    isinputWithdrawAmount = true;
    jEditorPane1.setText("" + '\n' + '\n' + '\n');
    jEditorPane1.append("请输入取款金额:");
    System.out.println("begin withdraw...");

  }

  void depositButton_actionPerformed(ActionEvent e) {
    isDeposit = true;
    isinputDepositAmount = true;
    jEditorPane1.setText("" + '\n' + '\n' + '\n');
    jEditorPane1.append("请输入存款金额:");
    System.out.println("begin deposit...");

  }

  public void deposit(int depositamount) {
    NameComponent name[] = new NameComponent[1];
    name[0] = new NameComponent();
    name[0].id = "ATM";
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
    atm = ATMHelper.narrow(obj);

    try {

      atmop = atm.provide_ATM_op();
    }
    catch (Exception ex1) {
      System.out.println("failure..");
    }

    try {

      atmop.deposit(atmAccounId, depositamount);
    }
    catch (Exception ex2) {
      JOptionPane.showMessageDialog(this, "transfer failure!");

    }

    System.out.println("transfer success...");

  }

  void queryButton_actionPerformed(ActionEvent e) {
    //if(!isTransfer&&!isDeposit&&!isChangepass&&!isWithdraw){
      queryFlag = true;
      queryop();
   // }

  }
  public void queryop(){
    if (queryFlag) {
      NameComponent name[] = new NameComponent[1];
      name[0] = new NameComponent();
      name[0].id = "ATM";
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
      atm = ATMHelper.narrow(obj);

      try {

        atmop = atm.provide_ATM_op();
      }
      catch (Exception ex1) {
        System.out.println("failure..");
      }

      try {

        balance = atmop.getBalance(atmAccounId);
      }
      catch (Exception ex2) {
        JOptionPane.showMessageDialog(this, "quey failure!");

      }

      System.out.println("query success...");
      jEditorPane1.setText("" + '\n' + '\n' + '\n');

      jEditorPane1.append("" + "您目前帐户余额为:" + balance + '\n');
      jEditorPane1.append("" + '\n' + "继续服务请单击确定!");
      queryFlag = false;

    }
  }

  void changepasswordButton_actionPerformed(ActionEvent e) {
    isChangepass = true;
    isinputfirstpass = true;
    jEditorPane1.setText("" + '\n' + '\n' + '\n');
    jEditorPane1.append("请输入新的密码:");
    System.out.println("change password...");

  }
}

class Atm_jButton1_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_jButton1_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jButton1_actionPerformed(e);
  }
}

class Atm_jButton2_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_jButton2_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jButton2_actionPerformed(e);
  }
}

class Atm_jButton3_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_jButton3_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jButton3_actionPerformed(e);
  }
}

class Atm_jButton4_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_jButton4_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jButton4_actionPerformed(e);
  }
}

class Atm_jButton5_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_jButton5_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jButton5_actionPerformed(e);
  }
}

class Atm_jButton6_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_jButton6_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jButton6_actionPerformed(e);
  }
}

class Atm_jButton7_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_jButton7_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jButton7_actionPerformed(e);
  }
}

class Atm_jButton8_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_jButton8_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jButton8_actionPerformed(e);
  }
}

class Atm_jButton9_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_jButton9_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jButton9_actionPerformed(e);
  }
}

class Atm_jButton0_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_jButton0_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.jButton0_actionPerformed(e);
  }
}

class Atm_Atm_OK_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_Atm_OK_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.Atm_OK_actionPerformed(e);
  }
}

class Atm_Atm_Cancel_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_Atm_Cancel_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.Atm_Cancel_actionPerformed(e);
  }
}

class Atm_transactionButton_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_transactionButton_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.transactionButton_actionPerformed(e);
  }
}

class Atm_withdrawButton_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_withdrawButton_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.withdrawButton_actionPerformed(e);
  }
}

class Atm_depositButton_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_depositButton_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.depositButton_actionPerformed(e);
  }
}

class Atm_queryButton_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_queryButton_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.queryButton_actionPerformed(e);
  }
}

class Atm_changepasswordButton_actionAdapter
    implements java.awt.event.ActionListener {
  Atm adaptee;

  Atm_changepasswordButton_actionAdapter(Atm adaptee) {
    this.adaptee = adaptee;
  }

  public void actionPerformed(ActionEvent e) {
    adaptee.changepasswordButton_actionPerformed(e);
  }
}
