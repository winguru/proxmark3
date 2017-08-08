//-----------------------------------------------------------------------------
//
// This code is licensed to you under the terms of the GNU GPL, version 2 or,
// at your option, any later version. See the LICENSE.txt file for the text of
// the license.
//-----------------------------------------------------------------------------
// Low frequency viking tag commands
//-----------------------------------------------------------------------------
#ifndef CMDLFVIKING_H__
#define CMDLFVIKING_H__
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "proxmark3.h"
#include "ui.h"
#include "util.h"
#include "graph.h"
#include "cmdparser.h"
#include "cmddata.h"
#include "cmdmain.h"
#include "cmdlf.h"
#include "lfdemod.h"

extern int CmdLFViking(const char *Cmd);

extern int CmdVikingDemod(const char *Cmd);
extern int CmdVikingRead(const char *Cmd);
extern int CmdVikingClone(const char *Cmd);
extern int CmdVikingSim(const char *Cmd);

extern int detectViking(uint8_t *dest, size_t *size);

extern int usage_lf_viking_clone(void);
extern int usage_lf_viking_sim(void);
#endif

