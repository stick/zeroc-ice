// **********************************************************************
//
// Copyright (c) 2003-2009 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

import Test.*;

public final class EI extends E
{
    public
    EI()
    {
        super(1, "hello");
    }

    public boolean
    checkValues(Ice.Current current)
    {
        return i == 1 && s.equals("hello");
    }
}