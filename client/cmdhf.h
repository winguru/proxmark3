//-----------------------------------------------------------------------------
// Copyright (C) 2010 iZsh <izsh at fail0verflow.com>
//
// This code is licensed to you under the terms of the GNU GPL, version 2 or,
// at your option, any later version. See the LICENSE.txt file for the text of
// the license.
//-----------------------------------------------------------------------------
// High frequency commands
//-----------------------------------------------------------------------------

#ifndef CMDHF_H__
#define CMDHF_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "proxmark3.h"
#include "graph.h"
#include "ui.h"
#include "cmdparser.h"
#include "cmdhf14a.h"
#include "cmdhf14b.h"
#include "cmdhf15.h"
#include "cmdhfepa.h"
#include "cmdhflegic.h"	 // LEGIC
#include "cmdhficlass.h" // ICLASS
#include "cmdhfmf.h"	 // CLASSIC
#include "cmdhfmfu.h"	 // ULTRALIGHT/NTAG etc
#include "cmdhfmfdes.h"	 // DESFIRE
#include "cmdhftopaz.h"	 // TOPAZ
#include "cmdhfemv.h"	 // EMV
#include "protocols.h"
#include "parity.h"  // oddparity

extern int CmdHF(const char *Cmd);
extern int CmdHFTune(const char *Cmd);
extern int CmdHFList(const char *Cmd);
extern int CmdHFSearch(const char *Cmd);
extern int CmdHFSnoop(const char *Cmd);

extern int usage_hf_list();					 
extern int usage_hf_search();
extern int usage_hf_snoop();
#endif
