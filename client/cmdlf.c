//-----------------------------------------------------------------------------
// Copyright (C) 2010 iZsh <izsh at fail0verflow.com>
//
// This code is licensed to you under the terms of the GNU GPL, version 2 or,
// at your option, any later version. See the LICENSE.txt file for the text of
// the license.
//-----------------------------------------------------------------------------
// Low frequency commands
//-----------------------------------------------------------------------------
#include "cmdlf.h"

bool g_lf_threshold_set = false;

static int CmdHelp(const char *Cmd);

int usage_lf_cmdread(void) {
	PrintAndLog("Usage: lf cmdread d <delay period> z <zero period> o <one period> c <cmdbytes> [H]");
	PrintAndLog("Options:");
	PrintAndLog("       h             This help");
	PrintAndLog("       L             Low frequency (125 KHz)");
	PrintAndLog("       H             High frequency (134 KHz)");
	PrintAndLog("       d <delay>     delay OFF period, (decimal)");
	PrintAndLog("       z <zero>      time period ZERO, (decimal)");
	PrintAndLog("       o <one>       time period ONE, (decimal)");
	PrintAndLog("       c <cmd>       Command bytes  (in ones and zeros)");
	PrintAndLog("       ************* All periods in microseconds (ms)");
	PrintAndLog("Examples:");
	PrintAndLog("      lf cmdread d 80 z 100 o 200 c 11000");
	PrintAndLog("      lf cmdread d 80 z 100 o 100 c 11000 H");
	return 0;
}
int usage_lf_read(void){
	PrintAndLog("Usage: lf read [h] [s] [d numofsamples]");
	PrintAndLog("Options:");
	PrintAndLog("       h            This help");
	PrintAndLog("       s            silent run no printout");
	PrintAndLog("       d #samples   # samples to collect (optional)");	
	PrintAndLog("Use 'lf config' to set parameters.");
	PrintAndLog("");
	PrintAndLog("Samples:");
	PrintAndLog("         lf read s d 12000     - collects 12000samples silent");
	PrintAndLog("         lf read s");
	return 0;
}
int usage_lf_snoop(void) {
	PrintAndLog("Snoop low frequence signal. Use 'lf config' to set parameters.");
	PrintAndLog("Usage: lf snoop [h]");
	PrintAndLog("Options:");
	PrintAndLog("      h         This help");
	PrintAndLog("This function takes no arguments. ");
	PrintAndLog("Use 'lf config' to set parameters.");
	return 0;
}
int usage_lf_config(void) {
	PrintAndLog("Usage: lf config [h] [H|<divisor>] [b <bps>] [d <decim>] [a 0|1]");
	PrintAndLog("Options:");
	PrintAndLog("       h             This help");
	PrintAndLog("       L             Low frequency (125 KHz)");
	PrintAndLog("       H             High frequency (134 KHz)");
	PrintAndLog("       q <divisor>   Manually set divisor. 88-> 134KHz, 95-> 125 Hz");
	PrintAndLog("       b <bps>       Sets resolution of bits per sample. Default (max): 8");
	PrintAndLog("       d <decim>     Sets decimation. A value of N saves only 1 in N samples. Default: 1");
	PrintAndLog("       a [0|1]       Averaging - if set, will average the stored sample value when decimating. Default: 1");
	PrintAndLog("       t <threshold> Sets trigger threshold. 0 means no threshold (range: 0-128)");
	PrintAndLog("Examples:");
	PrintAndLog("      lf config b 8 L");
	PrintAndLog("                    Samples at 125KHz, 8bps.");
	PrintAndLog("      lf config H b 4 d 3");
	PrintAndLog("                    Samples at 134KHz, averages three samples into one, stored with ");
	PrintAndLog("                    a resolution of 4 bits per sample.");
	PrintAndLog("      lf read");
	PrintAndLog("                    Performs a read (active field)");
	PrintAndLog("      lf snoop");
	PrintAndLog("                    Performs a snoop (no active field)");
	return 0;
}
int usage_lf_simfsk(void) {
	PrintAndLog("Usage: lf simfsk [c <clock>] [i] [H <fcHigh>] [L <fcLow>] [d <hexdata>]");
	PrintAndLog("Options:");
	PrintAndLog("       h              This help");
	PrintAndLog("       c <clock>      Manually set clock - can autodetect if using DemodBuffer");
	PrintAndLog("       i              invert data");
	PrintAndLog("       H <fcHigh>     Manually set the larger Field Clock");
	PrintAndLog("       L <fcLow>      Manually set the smaller Field Clock");
	//PrintAndLog("       s              TBD- -to enable a gap between playback repetitions - default: no gap");
	PrintAndLog("       d <hexdata>    Data to sim as hex - omit to sim from DemodBuffer");
	PrintAndLog("\n  NOTE: if you set one clock manually set them all manually");
	return 0;
}
int usage_lf_simask(void) {
	PrintAndLog("Usage: lf simask [c <clock>] [i] [b|m|r] [s] [d <raw hex to sim>]");
	PrintAndLog("Options:");
	PrintAndLog("       h              This help");
	PrintAndLog("       c <clock>      Manually set clock - can autodetect if using DemodBuffer");
	PrintAndLog("       i              invert data");
	PrintAndLog("       b              sim ask/biphase");
	PrintAndLog("       m              sim ask/manchester - Default");
	PrintAndLog("       r              sim ask/raw");
	PrintAndLog("       s              add t55xx Sequence Terminator gap - default: no gaps (only manchester)");
	PrintAndLog("       d <hexdata>    Data to sim as hex - omit to sim from DemodBuffer");
	return 0;
}
int usage_lf_simpsk(void) {
	PrintAndLog("Usage: lf simpsk [1|2|3] [c <clock>] [i] [r <carrier>] [d <raw hex to sim>]");
	PrintAndLog("Options:");
	PrintAndLog("       h              This help");
	PrintAndLog("       c <clock>      Manually set clock - can autodetect if using DemodBuffer");
	PrintAndLog("       i              invert data");
	PrintAndLog("       1              set PSK1 (default)");
	PrintAndLog("       2              set PSK2");
	PrintAndLog("       3              set PSK3");
	PrintAndLog("       r <carrier>    2|4|8 are valid carriers: default = 2");
	PrintAndLog("       d <hexdata>    Data to sim as hex - omit to sim from DemodBuffer");
	return 0;
}
int usage_lf_find(void){
    PrintAndLog("Usage:  lf search [h] <0|1> [u]");
    PrintAndLog("");
	PrintAndLog("Options:");
	PrintAndLog("       h             This help");
	PrintAndLog("       <0|1>         Use data from Graphbuffer, if not set, try reading data from tag.");
    PrintAndLog("       u             Search for Unknown tags, if not set, reads only known tags.");
	PrintAndLog("Examples:");
    PrintAndLog("      lf search     = try reading data from tag & search for known tags");
    PrintAndLog("      lf search 1   = use data from GraphBuffer & search for known tags");
    PrintAndLog("      lf search u   = try reading data from tag & search for known and unknown tags");
    PrintAndLog("      lf search 1 u = use data from GraphBuffer & search for known and unknown tags");
	return 0;
}


/* send a LF command before reading */
int CmdLFCommandRead(const char *Cmd) {

	bool errors = false;
	bool useHighFreq = false;
	uint16_t one = 0, zero = 0;
  	uint8_t cmdp = 0;
	UsbCommand c = {CMD_MOD_THEN_ACQUIRE_RAW_ADC_SAMPLES_125K, {0,0,0}};
	
	while(param_getchar(Cmd, cmdp) != 0x00 && !errors) {
		switch(param_getchar(Cmd, cmdp)) {
		case 'h':
			return usage_lf_cmdread();
		case 'H':
			useHighFreq = true;
			cmdp++;
			break;
		case 'L':
			cmdp++;
			break;
		case 'c':
			param_getstr(Cmd, cmdp+1, (char *)&c.d.asBytes);
			cmdp+=2;
			break;
		case 'd':
			c.arg[0] = param_get32ex(Cmd, cmdp+1, 0, 10);
			cmdp+=2;
			break;
		case 'z':
			zero = param_get32ex(Cmd, cmdp+1, 0, 10) & 0xFFFF;
			cmdp+=2;
			break;
		case 'o':
			one = param_get32ex(Cmd, cmdp+1, 0, 10) & 0xFFFF;
			cmdp+=2;
			break;
		default:
			PrintAndLog("Unknown parameter '%c'", param_getchar(Cmd, cmdp));
			errors = 1;
			break;
		}
	}

	//Validations
	if (errors || cmdp == 0)  return usage_lf_cmdread();
	
	// zero and one lengths
	c.arg[1] = (uint32_t)(zero << 16 | one);
	
	// add frequency 125 or 134
	c.arg[2] = useHighFreq;

	clearCommandBuffer();
	SendCommand(&c);
	return 0;
}

int CmdFlexdemod(const char *Cmd) {
#define LONG_WAIT 100	
	int i, j, start, bit, sum;
	int phase = 0;

	for (i = 0; i < GraphTraceLen; ++i)
		GraphBuffer[i] = (GraphBuffer[i] < 0) ? -1 : 1;

	for (start = 0; start < GraphTraceLen - LONG_WAIT; start++) {
		int first = GraphBuffer[start];
		for (i = start; i < start + LONG_WAIT; i++) {
			if (GraphBuffer[i] != first) {
				break;
			}
		}
		if (i == (start + LONG_WAIT))
			break;
	}
	
	if (start == GraphTraceLen - LONG_WAIT) {
		PrintAndLog("nothing to wait for");
		return 0;
	}

	GraphBuffer[start] = 2;
	GraphBuffer[start+1] = -2;
	uint8_t bits[64] = {0x00};

	i = start;
	for (bit = 0; bit < 64; bit++) {
		sum = 0;
		for (int j = 0; j < 16; j++) {
			sum += GraphBuffer[i++];
		}
		bits[bit] = (sum > 0) ? 1 : 0;
		PrintAndLog("bit %d sum %d", bit, sum);
	}

	for (bit = 0; bit < 64; bit++) {
		sum = 0;
		for (j = 0; j < 16; j++)
			sum += GraphBuffer[i++];

		if (sum > 0 && bits[bit] != 1) PrintAndLog("oops1 at %d", bit);

		if (sum < 0 && bits[bit] != 0) PrintAndLog("oops2 at %d", bit);

	}

	// HACK writing back to graphbuffer.
	GraphTraceLen = 32*64;
	i = 0;
	for (bit = 0; bit < 64; bit++) {
		
		phase = (bits[bit] == 0) ? 0 : 1;
		
		for (j = 0; j < 32; j++) {
			GraphBuffer[i++] = phase;
			phase = !phase;
		}
	}
	RepaintGraphWindow();
	return 0;
}

int CmdLFSetConfig(const char *Cmd) {
	uint8_t divisor =  0;//Frequency divisor
	uint8_t bps = 0; // Bits per sample
	uint8_t decimation = 0; //How many to keep
	bool averaging = 1; // Defaults to true
	bool errors = false;
	int trigger_threshold = -1;//Means no change
	uint8_t unsigned_trigg = 0;

	uint8_t cmdp = 0;
	while(param_getchar(Cmd, cmdp) != 0x00 && !errors) {
		switch(param_getchar(Cmd, cmdp)) {
		case 'h':
			return usage_lf_config();
		case 'H':
			divisor = 88;
			cmdp++;
			break;
		case 'L':
			divisor = 95;
			cmdp++;
			break;
		case 'q':
			errors |= param_getdec(Cmd, cmdp+1, &divisor);
			cmdp+=2;
			break;
		case 't':
			errors |= param_getdec(Cmd, cmdp+1, &unsigned_trigg);
			cmdp+=2;
			if(!errors) {
				trigger_threshold = unsigned_trigg;
				g_lf_threshold_set = (trigger_threshold > 0);
			}
			break;
		case 'b':
			errors |= param_getdec(Cmd, cmdp+1, &bps);
			cmdp+=2;
			break;
		case 'd':
			errors |= param_getdec(Cmd, cmdp+1, &decimation);
			cmdp+=2;
			break;
		case 'a':
			averaging = param_getchar(Cmd, cmdp+1) == '1';
			cmdp+=2;
			break;
		default:
			PrintAndLog("Unknown parameter '%c'", param_getchar(Cmd, cmdp));
			errors = 1;
			break;
		}
	}

	//Validations
	if (errors || cmdp == 0) return usage_lf_config();
	
	//Bps is limited to 8
	if (bps >> 4) bps = 8;

	sample_config config = { decimation, bps, averaging, divisor, trigger_threshold };

	UsbCommand c = {CMD_SET_LF_SAMPLING_CONFIG, {0,0,0} };
	memcpy(c.d.asBytes, &config, sizeof(sample_config));
	clearCommandBuffer();
	SendCommand(&c);
	return 0;
}

bool lf_read(bool silent, uint32_t samples) {
	if (offline) return false;
	UsbCommand c = {CMD_ACQUIRE_RAW_ADC_SAMPLES_125K, {silent, samples, 0}};
	clearCommandBuffer();
	SendCommand(&c);

	UsbCommand resp;
	if (g_lf_threshold_set) {
		WaitForResponse(CMD_ACK, &resp);
	} else {
		if ( !WaitForResponseTimeout(CMD_ACK, &resp, 2500) ) {
			PrintAndLog("command execution time out");
			return false;
		}
	}
	// resp.arg[0] is bits read not bytes read.
	getSamples(resp.arg[0]/8, silent);

	return true;
}

int CmdLFRead(const char *Cmd) {
	
	if (offline) return 0;
	
	bool errors = false;
	bool silent = false;
	uint32_t samples = 0;
	uint8_t cmdp = 0;
	while(param_getchar(Cmd, cmdp) != 0x00 && !errors) {
		switch(param_getchar(Cmd, cmdp)) {
		case 'h':
		case 'H':
			return usage_lf_read();
		case 's':
		case 'S':
			silent = true;
			cmdp++;
			break;
		case 'd':
		case 'D':  
			samples = param_get32ex(Cmd, cmdp, 0, 10);
			cmdp +=2;
			break;
		default:
			PrintAndLog("Unknown parameter '%c'", param_getchar(Cmd, cmdp));
			errors = true;
			break;
		}
	}

	//Validations
	if (errors) return usage_lf_read();

	return lf_read(silent, samples);
}

int CmdLFSnoop(const char *Cmd) {
	uint8_t cmdp = param_getchar(Cmd, 0);
	if(cmdp == 'h' || cmdp == 'H') return usage_lf_snoop();
	
	UsbCommand c = {CMD_LF_SNOOP_RAW_ADC_SAMPLES,{0,0,0}};
	clearCommandBuffer();	
	SendCommand(&c);
	WaitForResponse(CMD_ACK,NULL);
	getSamples(0, false);
	return 0;
}

static void ChkBitstream(const char *str) {
	// convert to bitstream if necessary
	for (int i = 0; i < (int)(GraphTraceLen / 2); i++){
		if (GraphBuffer[i] > 1 || GraphBuffer[i] < 0) {
			CmdGetBitStream("");
			break;
		}
	}
}
//Attempt to simulate any wave in buffer (one bit per output sample)
// converts GraphBuffer to bitstream (based on zero crossings) if needed.
int CmdLFSim(const char *Cmd) {
	int i,j;
	static int gap;

	sscanf(Cmd, "%i", &gap);

	// convert to bitstream if necessary 
	ChkBitstream(Cmd);

	if (g_debugMode) 
		printf("DEBUG: Sending [%d bytes]\n", GraphTraceLen);
	
	//can send only 512 bits at a time (1 byte sent per bit...)
	for (i = 0; i < GraphTraceLen; i += USB_CMD_DATA_SIZE) {
		UsbCommand c = {CMD_DOWNLOADED_SIM_SAMPLES_125K, {i, 0, 0}};

		for (j = 0; j < USB_CMD_DATA_SIZE; j++)
			c.d.asBytes[j] = GraphBuffer[i+j];

		clearCommandBuffer();
		SendCommand(&c);
		WaitForResponse(CMD_ACK, NULL);
		printf(".");
	}

	PrintAndLog("Simulating");

	UsbCommand c = {CMD_SIMULATE_TAG_125K, {GraphTraceLen, gap, 0}};
	clearCommandBuffer();
	SendCommand(&c);
	return 0;
}

// by marshmellow - sim fsk data given clock, fcHigh, fcLow, invert 
// - allow pull data from DemodBuffer
int CmdLFfskSim(const char *Cmd) {
	//might be able to autodetect FCs and clock from Graphbuffer if using demod buffer
	// otherwise will need FChigh, FClow, Clock, and bitstream
	uint8_t fcHigh = 0, fcLow = 0, clk = 0;
	uint8_t invert = 0;
	bool errors = false;
	char hexData[32] = {0x00}; // store entered hex data
	uint8_t data[255] = {0x00}; 
	int dataLen = 0;
	uint8_t cmdp = 0;
	
	while(param_getchar(Cmd, cmdp) != 0x00 && !errors) {
		switch(param_getchar(Cmd, cmdp)){
			case 'h':
				return usage_lf_simfsk();
			case 'i':
				invert = 1;
				cmdp++;
				break;
			case 'c':
				errors |= param_getdec(Cmd, cmdp+1, &clk);
				cmdp += 2;
				break;
			case 'H':
				errors |= param_getdec(Cmd, cmdp+1, &fcHigh);
				cmdp += 2;
				break;
			case 'L':
				errors |= param_getdec(Cmd, cmdp+1, &fcLow);
				cmdp += 2;
				break;
			//case 's':
			//  separator = 1;
			//  cmdp++;
			//  break;
			case 'd':
				dataLen = param_getstr(Cmd, cmdp+1, hexData);
				if (dataLen == 0)
					errors = true; 
				else
					dataLen = hextobinarray((char *)data, hexData);
				   
				if (dataLen == 0) errors = true; 
				if (errors) PrintAndLog ("Error getting hex data");
				cmdp+=2;
				break;
			default:
				PrintAndLog("Unknown parameter '%c'", param_getchar(Cmd, cmdp));
				errors = true;
				break;
		}
	}
	
	// No args
	if (cmdp == 0 && DemodBufferLen == 0) return usage_lf_simfsk();

	//Validations
	if (errors) return usage_lf_simfsk();

	int firstClockEdge = 0;
	if (dataLen == 0){ //using DemodBuffer 
		if (clk == 0 || fcHigh == 0 || fcLow == 0){ //manual settings must set them all
			uint8_t ans = fskClocks(&fcHigh, &fcLow, &clk, 0, &firstClockEdge);
			if (ans==0){
				if (!fcHigh) fcHigh = 10;
				if (!fcLow) fcLow = 8;
				if (!clk) clk = 50;
			}
		}
	} else {
		setDemodBuf(data, dataLen, 0);
	}

	//default if not found
	if (clk == 0) clk = 50;
	if (fcHigh == 0) fcHigh = 10;
	if (fcLow == 0) fcLow = 8;

	uint16_t arg1, arg2;
	arg1 = fcHigh << 8 | fcLow;
	arg2 = invert << 8 | clk;
	size_t size = DemodBufferLen;
	if (size > USB_CMD_DATA_SIZE) {
		PrintAndLog("DemodBuffer too long for current implementation - length: %d - max: %d", size, USB_CMD_DATA_SIZE);
		size = USB_CMD_DATA_SIZE;
	} 
	UsbCommand c = {CMD_FSK_SIM_TAG, {arg1, arg2, size}};

	memcpy(c.d.asBytes, DemodBuffer, size);
	clearCommandBuffer();
	SendCommand(&c);
	return 0;
}

// by marshmellow - sim ask data given clock, invert, manchester or raw, separator 
// - allow pull data from DemodBuffer
int CmdLFaskSim(const char *Cmd) {
	// autodetect clock from Graphbuffer if using demod buffer
	// needs clock, invert, manchester/raw as m or r, separator as s, and bitstream
	uint8_t encoding = 1, separator = 0, clk = 0, invert = 0;
	bool errors = false;
	char hexData[32] = {0x00}; 
	uint8_t data[255]= {0x00}; // store entered hex data
	int dataLen = 0;
	uint8_t cmdp = 0;
	
	while(param_getchar(Cmd, cmdp) != 0x00 && !errors) {
		switch(param_getchar(Cmd, cmdp)) {
			case 'H':
			case 'h': return usage_lf_simask();
			case 'i':
				invert = 1;
				cmdp++;
				break;
			case 'c':
				errors |= param_getdec(Cmd, cmdp+1, &clk);
				cmdp += 2;
				break;
			case 'b':
				encoding = 2; //biphase
				cmdp++;
				break;
			case 'm':
				encoding = 1; //manchester
				cmdp++;
				break;
			case 'r':
				encoding = 0; //raw
				cmdp++;
				break;
			case 's':
				separator = 1;
				cmdp++;
				break;
			case 'd':
				dataLen = param_getstr(Cmd, cmdp+1, hexData);
				if (dataLen == 0)
					errors = true; 
				else
					dataLen = hextobinarray((char *)data, hexData);
				
				if (dataLen == 0) errors = true; 
				if (errors) PrintAndLog ("Error getting hex data, datalen: %d", dataLen);
				cmdp += 2;
				break;
			default:
				PrintAndLog("Unknown parameter '%c'", param_getchar(Cmd, cmdp));
				errors = true;
				break;
		}
	}

	// No args
	if (cmdp == 0 && DemodBufferLen == 0) return usage_lf_simask();

	//Validations
	if (errors) return usage_lf_simask();
	
	if (dataLen == 0){ //using DemodBuffer
		if (clk == 0) 
			clk = GetAskClock("0", false, false);
	} else {
		setDemodBuf(data, dataLen, 0);
	}
	if (clk == 0) clk = 64;
	if (encoding == 0) clk /= 2; //askraw needs to double the clock speed
	
	size_t size = DemodBufferLen;

	if (size > USB_CMD_DATA_SIZE) {
		PrintAndLog("DemodBuffer too long for current implementation - length: %d - max: %d", size, USB_CMD_DATA_SIZE);
		size = USB_CMD_DATA_SIZE;
	}
	
	PrintAndLog("preparing to sim ask data: %d bits", size);	

	uint16_t arg1, arg2;	
	arg1 = clk << 8 | encoding;
	arg2 = invert << 8 | separator;

	UsbCommand c = {CMD_ASK_SIM_TAG, {arg1, arg2, size}};
	memcpy(c.d.asBytes, DemodBuffer, size);
	clearCommandBuffer();
	SendCommand(&c);
	return 0;
}

// by marshmellow - sim psk data given carrier, clock, invert 
// - allow pull data from DemodBuffer or parameters
int CmdLFpskSim(const char *Cmd) {
	//might be able to autodetect FC and clock from Graphbuffer if using demod buffer
	//will need carrier, Clock, and bitstream
	uint8_t carrier=0, clk=0;
	uint8_t invert=0;
	bool errors = false;
	char hexData[32] = {0x00}; // store entered hex data
	uint8_t data[255] = {0x00}; 
	int dataLen = 0;
	uint8_t cmdp = 0;
	uint8_t pskType = 1;
	
	while(param_getchar(Cmd, cmdp) != 0x00 && !errors) {
		switch(param_getchar(Cmd, cmdp)) {
			case 'h':
				return usage_lf_simpsk();
			case 'i':
				invert = 1;
				cmdp++;
				break;
			case 'c':
				errors |= param_getdec(Cmd,cmdp+1,&clk);
				cmdp +=2;
				break;
			case 'r':
				errors |= param_getdec(Cmd,cmdp+1,&carrier);
				cmdp += 2;
				break;
			case '1':
				pskType = 1;
				cmdp++;
				break;
			case '2':
				pskType = 2;
				cmdp++;
				break;
			case '3':
				pskType = 3;
				cmdp++;
				break;
			case 'd':
				dataLen = param_getstr(Cmd, cmdp+1, hexData);
				if (dataLen == 0)
					errors = true; 
				else
					dataLen = hextobinarray((char *)data, hexData);
				    
				if (dataLen == 0) errors = true; 
				if (errors) PrintAndLog ("Error getting hex data");
				cmdp+=2;
				break;
			default:
				PrintAndLog("Unknown parameter '%c'", param_getchar(Cmd, cmdp));
				errors = true;
				break;
			}
	}
	// No args
	if (cmdp == 0 && DemodBufferLen == 0)
		errors = true;

	//Validations
	if (errors) return usage_lf_simpsk();

	if (dataLen == 0){ //using DemodBuffer
		PrintAndLog("Getting Clocks");
		
		if (clk==0) clk = GetPskClock("", false, false);
		PrintAndLog("clk: %d",clk);
		
		if (!carrier) carrier = GetPskCarrier("", false, false); 
		PrintAndLog("carrier: %d", carrier);
		
	} else {
		setDemodBuf(data, dataLen, 0);
	}

	if (clk <= 0) clk = 32;

	if (carrier == 0) carrier = 2;
  
	if (pskType != 1){
		if (pskType == 2){
			//need to convert psk2 to psk1 data before sim
			psk2TOpsk1(DemodBuffer, DemodBufferLen);
		} else {
			PrintAndLog("Sorry, PSK3 not yet available");
		}
	}
	uint16_t arg1, arg2;
	arg1 = clk << 8 | carrier;
	arg2 = invert;
	size_t size = DemodBufferLen;
	if (size > USB_CMD_DATA_SIZE) {
		PrintAndLog("DemodBuffer too long for current implementation - length: %d - max: %d", size, USB_CMD_DATA_SIZE);
		size = USB_CMD_DATA_SIZE;
	}
	UsbCommand c = {CMD_PSK_SIM_TAG, {arg1, arg2, size}};
	PrintAndLog("DEBUG: Sending DemodBuffer Length: %d", size);
	memcpy(c.d.asBytes, DemodBuffer, size);
	clearCommandBuffer();
	SendCommand(&c);
	return 0;
}

int CmdLFSimBidir(const char *Cmd) {
	// Set ADC to twice the carrier for a slight supersampling
	// HACK: not implemented in ARMSRC.
	PrintAndLog("Not implemented yet.");
	UsbCommand c = {CMD_LF_SIMULATE_BIDIR, {47, 384, 0}};
	SendCommand(&c);
	return 0;
}

int CmdVchDemod(const char *Cmd) {
	// Is this the entire sync pattern, or does this also include some
	// data bits that happen to be the same everywhere? That would be
	// lovely to know.
	static const int SyncPattern[] = {
		1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
		1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	};

	// So first, we correlate for the sync pattern, and mark that.
	int bestCorrel = 0, bestPos = 0;
	int i, j, sum = 0;

	// It does us no good to find the sync pattern, with fewer than 2048 samples after it.

	for (i = 0; i < (GraphTraceLen - 2048); i++) {
		for (j = 0; j < ARRAYLEN(SyncPattern); j++) {
			sum += GraphBuffer[i+j] * SyncPattern[j];
		}
		if (sum > bestCorrel) {
			bestCorrel = sum;
			bestPos = i;
		}
	}
	PrintAndLog("best sync at %d [metric %d]", bestPos, bestCorrel);

	char bits[257];
	bits[256] = '\0';

	int worst = INT_MAX, worstPos = 0;

	for (i = 0; i < 2048; i += 8) {
		sum = 0;
		for (j = 0; j < 8; j++) 
			sum += GraphBuffer[bestPos+i+j];
		
		if (sum < 0)
			bits[i/8] = '.';
		else
			bits[i/8] = '1';
		
		if(abs(sum) < worst) {
			worst = abs(sum);
			worstPos = i;
		}
	}
	PrintAndLog("bits:");
	PrintAndLog("%s", bits);
	PrintAndLog("worst metric: %d at pos %d", worst, worstPos);

	// clone
	if (strcmp(Cmd, "clone")==0) {
		GraphTraceLen = 0;
		char *s;
			for(s = bits; *s; s++) {
				for(j = 0; j < 16; j++) {
					GraphBuffer[GraphTraceLen++] = (*s == '1') ? 1 : 0;
				}
			}
		RepaintGraphWindow();
	}
	return 0;
}

//by marshmellow
int CheckChipType(bool getDeviceData) {

	if (!getDeviceData) return 0;
	
	save_restoreDB(GRAPH_SAVE);

	//check for em4x05/em4x69 chips first
	uint32_t word = 0;
	if (EM4x05IsBlock0(&word)) {
		PrintAndLog("\nValid EM4x05/EM4x69 Chip Found\nTry lf em 4x05... commands\n");
		save_restoreGB(GRAPH_RESTORE);
		return 1;
	}

	//check for t55xx chip...
	if (tryDetectP1(true)) {
		PrintAndLog("\nValid T55xx Chip Found\nTry `lf t55xx` commands\n");
		save_restoreGB(GRAPH_RESTORE);
		return 1;		
	}

	save_restoreDB(GRAPH_RESTORE);
	return 0;
}

//by marshmellow
int CmdLFfind(const char *Cmd) {
	int ans = 0;
	size_t minLength = 1000;
	char cmdp = param_getchar(Cmd, 0);
	char testRaw = param_getchar(Cmd, 1);
	
	if (strlen(Cmd) > 3 || cmdp == 'h' || cmdp == 'H') return usage_lf_find();
	
	if (cmdp == 'u' || cmdp == 'U') testRaw = 'u';
	
	bool isOnline = (!offline && (cmdp != '1') );
	
	if (isOnline)
		lf_read(true, 30000);
	
	if (GraphTraceLen < minLength) {
		PrintAndLog("Data in Graphbuffer was too small.");
		return 0;
	}

	PrintAndLog("NOTE: some demods output possible binary\n  if it finds something that looks like a tag");
	PrintAndLog("False Positives ARE possible\n");  
	PrintAndLog("\nChecking for known tags:\n");
	
	// only run these tests if device is online
	if (isOnline) {

		// only run if graphbuffer is just noise as it should be for hitag/cotag
		if (is_justnoise(GraphBuffer, minLength)) {
			
			if (CheckChipType(isOnline) ) return 1;			
			if (CmdLFHitagReader("26")) { PrintAndLog("\nValid Hitag Found!"); return 1;}
			if (CmdCOTAGRead("")) 		{ PrintAndLog("\nValid COTAG ID Found!"); return 1;}

			PrintAndLog("Signal looks just like noise. Quitting.");
		    return 0;
		}
	}
	if (EM4x50Read("", false))	{ PrintAndLog("\nValid EM4x50 ID Found!"); return 1;}
	if (CmdAWIDDemod(""))		{ PrintAndLog("\nValid AWID ID Found!"); goto out;}
	if (CmdEM410xDemod(""))		{ PrintAndLog("\nValid EM410x ID Found!"); goto out;}
	if (CmdFdxDemod(""))		{ PrintAndLog("\nValid FDX-B ID Found!"); goto out;}	
	if (CmdGuardDemod(""))		{ PrintAndLog("\nValid Guardall G-Prox II ID Found!"); goto out; }
	if (CmdHIDDemod(""))		{ PrintAndLog("\nValid HID Prox ID Found!"); goto out;}
	if (CmdPSKIdteck(""))		{ PrintAndLog("\nValid Idteck ID Found!"); goto out;}
	if (CmdIndalaDemod(""))		{ PrintAndLog("\nValid Indala ID Found!");  goto out;}
	if (CmdIOProxDemod(""))		{ PrintAndLog("\nValid IO Prox ID Found!"); goto out;}
	if (CmdJablotronDemod(""))	{ PrintAndLog("\nValid Jablotron ID Found!"); goto out;}
	if (CmdLFNedapDemod(""))	{ PrintAndLog("\nValid NEDAP ID Found!"); goto out;}
	if (CmdNexWatchDemod("")) 	{ PrintAndLog("\nValid NexWatch ID Found!"); goto out;}
	if (CmdNoralsyDemod(""))	{ PrintAndLog("\nValid Noralsy ID Found!"); goto out;}
	if (CmdPacDemod(""))		{ PrintAndLog("\nValid PAC/Stanley ID Found!"); goto out;}	
	if (CmdParadoxDemod(""))	{ PrintAndLog("\nValid Paradox ID Found!"); goto out;}
	if (CmdPrescoDemod(""))		{ PrintAndLog("\nValid Presco ID Found!"); goto out;}				 
	if (CmdPyramidDemod(""))	{ PrintAndLog("\nValid Pyramid ID Found!"); goto out;}
	if (CmdSecurakeyDemod(""))	{ PrintAndLog("\nValid Securakey ID Found!"); goto out;}
	if (CmdVikingDemod(""))		{ PrintAndLog("\nValid Viking ID Found!"); goto out;}	
	if (CmdVisa2kDemod(""))		{ PrintAndLog("\nValid Visa2000 ID Found!"); goto out;}

	// TIdemod?  flexdemod?
	
	PrintAndLog("\nNo Known Tags Found!\n");
	
	if (testRaw=='u' || testRaw=='U'){
		//test unknown tag formats (raw mode)
		PrintAndLog("\nChecking for Unknown tags:\n");
		ans = AutoCorrelate(GraphBuffer, GraphBuffer, GraphTraceLen, 4000, false, false);
		if (ans > 0) {

			PrintAndLog("Possible Auto Correlation of %d repeating samples",ans);

			if ( ans % 8 == 0)  {
				int bytes = (ans / 8);
				PrintAndLog("Possible %d bytes", bytes);
				int blocks = 0;
				if ( bytes % 2 == 0) {
					blocks = (bytes / 2);	
					PrintAndLog("Possible  2 blocks, width %d", blocks);
				}
				if ( bytes % 4 == 0) {
					blocks = (bytes / 4);	
					PrintAndLog("Possible  4 blocks, width %d", blocks);
				}
				if ( bytes % 8 == 0) {
					blocks = (bytes / 8);	
					PrintAndLog("Possible  8 blocks, width %d", blocks);
				}
				if ( bytes % 16 == 0) {
					blocks = (bytes / 16);	
					PrintAndLog("Possible 16 blocks, width %d", blocks);
				}
			}
		}

		 //fsk
		if ( GetFskClock("",false,false) ) {
			if ( FSKrawDemod("",true) ) { 
				PrintAndLog("\nUnknown FSK Modulated Tag Found!"); goto out;
			}
		}
		
		bool st = true;
		if ( ASKDemod_ext("0 0 0",true,false,1,&st) ) {
		  PrintAndLog("\nUnknown ASK Modulated and Manchester encoded Tag Found!");
		  PrintAndLog("\nif it does not look right it could instead be ASK/Biphase - try 'data rawdemod ab'");
		  goto out;
		}
		
		if ( CmdPSK1rawDemod("") ) {
			PrintAndLog("Possible unknown PSK1 Modulated Tag Found above!\n\nCould also be PSK2 - try 'data rawdemod p2'");
			PrintAndLog("\nCould also be PSK3 - [currently not supported]");
			PrintAndLog("\nCould also be NRZ - try 'data nrzrawdemod");
			goto out;
		}
		
		PrintAndLog("\nNo Data Found!\n");
	}
out:
	// identify chipset
	CheckChipType(isOnline);
	return 0;
}

static command_t CommandTable[] = {
	{"help",        CmdHelp,            1, "This help"},
	{"awid",        CmdLFAWID,          1, "{ AWID RFIDs...              }"},
	{"cotag",       CmdLFCOTAG,         1, "{ COTAG CHIPs...             }"},
	{"em",          CmdLFEM4X,          1, "{ EM4X CHIPs & RFIDs...      }"},
	{"fdx",         CmdLFFdx,           1, "{ FDX-B RFIDs...             }"},
	{"gproxii",     CmdLFGuard,			1, "{ Guardall Prox II RFIDs...  }"},
	{"hid",         CmdLFHID,           1, "{ HID RFIDs...               }"},
	{"hitag",       CmdLFHitag,         1, "{ Hitag CHIPs...             }"},
	{"indala",      CmdLFINDALA,        1, "{ Indala RFIDs...            }"},
	{"io",          CmdLFIO,            1, "{ ioProx RFIDs...            }"},
	{"jablotron",	CmdLFJablotron,		1, "{ Jablotron RFIDs...         }"},
	{"nedap",		CmdLFNedap,			1, "{ Nedap RFIDs...             }"},
	{"nexwatch",    CmdLFNEXWATCH,      1, "{ NexWatch RFIDs...          }"},
	{"noralsy",		CmdLFNoralsy,		1, "{ Noralsy RFIDs...           }"},	
	{"pac",         CmdLFPac,           1, "{ PAC/Stanley RFIDs...       }"},
	{"paradox",     CmdLFParadox,       1, "{ Paradox RFIDs...           }"},
	{"pcf7931",     CmdLFPCF7931,       1, "{ PCF7931 CHIPs...           }"},
	{"presco",      CmdLFPresco,        1, "{ Presco RFIDs...            }"},
	{"pyramid",		CmdLFPyramid,       1, "{ Farpointe/Pyramid RFIDs... }"},	
	{"securakey",   CmdLFSecurakey,     1, "{ Securakey RFIDs...         }"},
	{"ti",          CmdLFTI,            1, "{ TI CHIPs...                }"},
	{"t55xx",       CmdLFT55XX,         1, "{ T55xx CHIPs...             }"},
	{"viking",      CmdLFViking,        1, "{ Viking RFIDs...            }"},
	{"visa2000",    CmdLFVisa2k,        1, "{ Visa2000 RFIDs...          }"},
	{"config",      CmdLFSetConfig,     0, "Set config for LF sampling, bit/sample, decimation, frequency"},
	{"cmdread",     CmdLFCommandRead,   0, "<off period> <'0' period> <'1' period> <command> ['h' 134] \n\t\t-- Modulate LF reader field to send command before read (all periods in microseconds)"},
	{"flexdemod",   CmdFlexdemod,       1, "Demodulate samples for FlexPass"},
	{"read",        CmdLFRead,          0, "['s' silent] Read 125/134 kHz LF ID-only tag. Do 'lf read h' for help"},
	{"search",      CmdLFfind,          1, "[offline] ['u'] Read and Search for valid known tag (in offline mode it you can load first then search) \n\t\t-- 'u' to search for unknown tags"},
	{"sim",         CmdLFSim,           0, "[GAP] -- Simulate LF tag from buffer with optional GAP (in microseconds)"},
	{"simask",      CmdLFaskSim,        0, "[clock] [invert <1|0>] [biphase/manchester/raw <'b'|'m'|'r'>] [msg separator 's'] [d <hexdata>] \n\t\t-- Simulate LF ASK tag from demodbuffer or input"},
	{"simfsk",      CmdLFfskSim,        0, "[c <clock>] [i] [H <fcHigh>] [L <fcLow>] [d <hexdata>] \n\t\t-- Simulate LF FSK tag from demodbuffer or input"},
	{"simpsk",      CmdLFpskSim,        0, "[1|2|3] [c <clock>] [i] [r <carrier>] [d <raw hex to sim>] \n\t\t-- Simulate LF PSK tag from demodbuffer or input"},
	{"simbidir",    CmdLFSimBidir,      0, "Simulate LF tag (with bidirectional data transmission between reader and tag)"},
	{"snoop",       CmdLFSnoop,         0, "Snoop LF"},
	{"vchdemod",    CmdVchDemod,        1, "['clone'] -- Demodulate samples for VeriChip"},
	{NULL, NULL, 0, NULL}
};

int CmdLF(const char *Cmd) {
	clearCommandBuffer();
	CmdsParse(CommandTable, Cmd);
	return 0; 
}

int CmdHelp(const char *Cmd) {
	CmdsHelp(CommandTable);
	return 0;
}
