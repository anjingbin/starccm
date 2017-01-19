// **********************************************************************
//
// Copyright (c) 2001-2004
// StarMiddleware.net
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Wang Kebo    mep@263.net
//
// **********************************************************************

// Version: 1.0.0

package Dinner;

import javax.ejb.CreateException;
import javax.ejb.SessionBean;
import javax.ejb.SessionContext;
//import javax.naming.Context;
//import javax.naming.InitialContext;
//import javax.naming.NamingException;

public class ObservingBean implements SessionBean
{
	private SessionContext ctx;
	
	public void setSessionContext(SessionContext ctx)
	{
    	this.ctx = ctx;
	}
	
	public void ejbActivate()
	{
	}
	
	public void ejbPassivate()
	{
	}
	
	public void ejbRemove()
	{
	}
	
	public void ejbCreate() throws CreateException
	{
	}
	
	public void change_status(int state, String name)
		throws java.rmi.RemoteException
	{
		switch(state)
		{
			case 0:
				System.out.println("=====>" + name + " is EATING");
				break;
			case 1:
				System.out.println("=====>" + name + " is THINKING");
				break;
			case 2:
				System.out.println("=====>" + name + " is SLEEPING");
				break;
			case 3:
				System.out.println("=====>" + name + " is HUNGRY");
				break;
			case 4:
				System.out.println("=====>" + name + " is DEAD");
				break;
			default:
				System.out.println("=====>" + name + " is in ERROR STATE!");
				break;
		}
	}
}