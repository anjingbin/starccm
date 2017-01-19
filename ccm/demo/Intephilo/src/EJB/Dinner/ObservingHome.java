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

import java.rmi.RemoteException;
import javax.ejb.CreateException;
import javax.ejb.EJBHome;

public interface ObservingHome extends EJBHome
{
	Observing create() throws CreateException, RemoteException;
}