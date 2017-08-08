//-----------------------------------------------------------------------------
// Copyright (C) 2010 iZsh <izsh at fail0verflow.com>
//
// This code is licensed to you under the terms of the GNU GPL, version 2 or,
// at your option, any later version. See the LICENSE.txt file for the text of
// the license.
//-----------------------------------------------------------------------------
// High frequency ISO14443B commands
//-----------------------------------------------------------------------------

#ifndef CMDHF14B_H__
#define CMDHF14B_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "iso14443crc.h"
#include "proxmark3.h"
#include "data.h"
#include "graph.h"
#include "util.h"
#include "ui.h"
#include "cmdparser.h"
#include "cmdmain.h"
#include "cmdhf14a.h"
#include "cmdhf.h"
#include "prng.h"
#include "sha1.h"
#include "mifare.h"		// structs/enum for ISO14B
#include "protocols.h"	// definitions of ISO14B protocol

int usage_hf_14b_info(void);
int usage_hf_14b_reader(void);
int usage_hf_14b_raw(void);
int usage_hf_14b_snoop(void);
int usage_hf_14b_sim(void);
int usage_hf_14b_read_srx(void);
int usage_hf_14b_write_srx(void);

int CmdHF14B(const char *Cmd);
int CmdHF14BList(const char *Cmd);
int CmdHF14BInfo(const char *Cmd);
int CmdHF14BSim(const char *Cmd);
int CmdHF14BSnoop(const char *Cmd);
int CmdHF14BWrite( const char *cmd);
int CmdHF14BReader(const char *Cmd);

bool HF14BInfo(bool verbose);
bool HF14BReader(bool verbose);
int CmdHF14BCmdRaw (const char *Cmd);

// SRi  ST Microelectronics read/write 
int CmdHF14BReadSri(const char *Cmd);
int CmdHF14BWriteSri(const char *Cmd);

bool waitCmd14b(bool verbose);
#endif
