/*****************************************************************************
*
* Copyright (c) 2000 - 2009, The Regents of the University of California
* Produced at the Lawrence Livermore National Laboratory
* All rights reserved.
*
* This file is part of VisIt. For details, see http://www.llnl.gov/visit/. The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or materials provided with the distribution.
*  - Neither the name of the UC/LLNL nor  the names of its contributors may be
*    used to  endorse or  promote products derived from  this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED.  IN  NO  EVENT  SHALL  THE  REGENTS  OF  THE  UNIVERSITY OF
* CALIFORNIA, THE U.S.  DEPARTMENT  OF  ENERGY OR CONTRIBUTORS BE  LIABLE  FOR
* ANY  DIRECT,  INDIRECT,  INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

#include "SiloValueView.h"
#include <SiloFile.h>
#include <QLabel>
#include <iostream>
#include <cstdlib>
using std::cerr;

// ----------------------------------------------------------------------------
//                            Value View
// ----------------------------------------------------------------------------

// ****************************************************************************
//  Constructor:  SiloValueViewWindow::SiloValueViewWindow
//
//  Programmer:  Jeremy Meredith
//  Creation:    November 12, 2001
//
//  Modifications:
//    Mark Miller, Tue 23 Mar 11:19:13 PDT 2004
//    Added call to free memory from the Silo DBGetVar call.
//
//    Jeremy Meredith, Thu Nov 20 17:28:45 EST 2008
//    Ported to Qt4.
//
//    Mark C. Miller, Fri Dec  4 15:19:47 PST 2009
//    Adding support for long long type
//
//    Mark C. Miller, Mon Dec  7 09:50:19 PST 2009
//    Conditionally compile long long support only when its
//    different from long.
// ****************************************************************************
SiloValueViewWindow::SiloValueViewWindow(SiloFile *s, const QString &n, QWidget *p)
    : QMainWindow(p), silo(s), name(n)
{
    setWindowTitle(QString("Value: ")+name);

    QLabel *l = new QLabel(this);
    setCentralWidget(l);

    void *var = silo->GetVar(name);
    if (!var)
    {
        cerr << "SiloValueViewWindow::SiloValueViewWindow -- not an array var\n";
        throw;
    }

    int type = silo->GetVarType(name);
    int len  = silo->GetVarLength(name);
    char str[4096];
    switch (type)
    {
      case DB_INT:
        sprintf(str, "int: %d", *((int*)var));
        
        break;
      case DB_SHORT:
        sprintf(str, "short: %d", *((short*)var));
        break;
      case DB_LONG:
        sprintf(str, "long: %ld", *((long*)var));
        break;
      case DB_LONG_LONG:
        sprintf(str, "long long: %lld", *((long long*)var));
        break;
      case DB_FLOAT:
        sprintf(str, "float: %g", *((float*)var));
        break;
      case DB_DOUBLE:
        sprintf(str, "double: %g", *((double*)var));
        break;
      case DB_CHAR:
        if (len == 1)
            sprintf(str, "char: %c", *((char*)var));
        else
            sprintf(str, "string: %s", ((char*)var));
        break;
      case DB_NOTYPE:
        sprintf(str, "NOTYPE: ???");
        break;
      default:
        sprintf(str, "???: ???");
        break;
    }
    l->setText(str);

    free(var);
}

