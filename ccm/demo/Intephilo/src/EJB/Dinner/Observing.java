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

import javax.ejb.*;
import java.rmi.RemoteException;

public interface Observing extends EJBObject 
{
	void change_status(int state, String name)
		throws RemoteException;
}