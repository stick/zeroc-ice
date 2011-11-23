// **********************************************************************
//
// Copyright (c) 2003
// ZeroC, Inc.
// Billerica, MA, USA
//
// All Rights Reserved.
//
// Ice is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation.
//
// **********************************************************************

#include "stdafx.h"
#include "HelloServer.h"
#include "HelloServerDlg.h"
#include "HelloI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CHelloServerApp, CWinApp)
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CHelloServerApp::CHelloServerApp()
{
    // Place all significant initialization in InitInstance
}

// The one and only CHelloServerApp object

CHelloServerApp theApp;

BOOL CHelloServerApp::InitInstance()
{
    //
    // InitCommonControls() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    //
    InitCommonControls();

    CWinApp::InitInstance();

    //
    // Create a communicator and object adapter.
    //
    Ice::CommunicatorPtr communicator;
    Ice::ObjectAdapterPtr adapter;
    LogIPtr log;
    try
    {
        int argc = 0;
        Ice::PropertiesPtr properties = Ice::createProperties();
        //properties->setProperty("Hello.Endpoints", "tcp -p 10000:udp -p 10000:ssl -p 10001");
        properties->setProperty("Hello.Endpoints", "tcp -p 10000:udp -p 10000");
        properties->setProperty("Ice.Trace.Network", "1");
        properties->setProperty("Ice.Warn.Connections", "1");
        communicator = Ice::initializeWithProperties(argc, 0, properties);
        log = new LogI;
        communicator->setLogger(log);
        adapter = communicator->createObjectAdapter("Hello");
    }
    catch(const IceUtil::Exception& ex)
    {
        std::ostringstream ostr;
        ostr << ex;
        std::string s = ostr.str();
        AfxMessageBox(CString(s.c_str()), MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    //
    // Create the dialog.
    //
    CHelloServerDlg dlg(communicator, log);

    //
    // Instantiate the servant.
    //
    Ice::ObjectPtr servant = new HelloI(log, &dlg);
    adapter->add(servant, Ice::stringToIdentity("hello"));
    adapter->activate();
    log->message("Ready to receive requests.");

    //
    // Show dialog and wait until it is closed, or until the servant receives
    // a shutdown request.
    //
    m_pMainWnd = &dlg;
    dlg.DoModal();

    //
    // Edit control no longer exists.
    //
    log->setControl(0);

    //
    // Clean up.
    //
    try
    {
        communicator->destroy();
    }
    catch(const IceUtil::Exception&)
    {
    }

    // Since the dialog has been closed, return FALSE so that we exit the
    // application, rather than start the application's message pump.
    return FALSE;
}