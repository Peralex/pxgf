// Copyright 2006 Peralex Electronics (Pty) Ltd
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// This example opens a connection to a TCP socket that transmits PXG
// formatted data.  The code reads the data and prints out some of the
// information contained in the chunks.

#include <iostream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <chrono>

#include "SocketSSNCReader.h"

#include "exceptions/PxgfException.h"

using namespace std;
using namespace pxgf;

void cSocketSSNCReader::callbackSIQP(const pxgf::cPackingSIQP &oPacking)
{
	m_bIsIQPacked = oPacking.getIsIQPacked();
}

void cSocketSSNCReader::callbackCF__(int64_t lCentreFrequency_uHz)
{
	m_uCentreFrequency_Hz = lCentreFrequency_uHz / 1000000;
}

void cSocketSSNCReader::callbackBW__(int64_t lBandwidth_uHz)
{
	m_uBandwidth_Hz = lBandwidth_uHz / 1000000;
}

void cSocketSSNCReader::callback_dBFS(float f_dBFullScale_dBm)
{
	m_fFullScale_dBm = f_dBFullScale_dBm;
}

void cSocketSSNCReader::callback_dBTG(float fTotalGain_dB)
{
	m_fTotalGain_dB = fTotalGain_dB;
}

void cSocketSSNCReader::callbackIQDC()
{
	cout << "Discontinuity detected" << endl;
}

void cSocketSSNCReader::callbackSR__(int64_t lSampleRate_uHz)
{
	m_uSampleRate_Hz = lSampleRate_uHz / 1000000;
}

void cSocketSSNCReader::callbackSSNC(int64_t lTimestamp_ns, const std::vector<short> &vsIqData)
{
	std::chrono::time_point<std::chrono::system_clock> CurrentTime = std::chrono::system_clock::now();

	// Check time count for overflow / wraparound / initialization.
	if(CurrentTime < m_LastTime)
		m_LastTime = CurrentTime;
	// If required, limit the rate at which data is output.
	//if((CurrentTime - m_LastTime) > std::chrono::milliseconds(500)) // Restrict printing info to every 1/2 a sec
	{
		m_LastTime = CurrentTime;
		time_t TimeStamp_sec = (unsigned)(lTimestamp_ns / INT64_C(1000000000));
		char *pchTimeString = ctime(&TimeStamp_sec);
		pchTimeString[24] = 0; // Remove trailing \n
		cout << "(" << pchTimeString << " + " << (unsigned)(lTimestamp_ns % INT64_C(1000000000)) << " us):\n\t"
			 << "CentreFrequency: " << m_uCentreFrequency_Hz << " Hz\n\t"
			 << "Bandwidth:       " << m_uBandwidth_Hz << " Hz\n\t"
			 << "Sample Rate:     " << m_uSampleRate_Hz << " Hz\n\t"
			 << "Signal level:    " << CalculateRMSPower(vsIqData, m_fFullScale_dBm, m_fTotalGain_dB) << " dBm (measured over " << vsIqData.size() / 2 << " samples)"
			 << endl
			 << endl;
	}
}

float cSocketSSNCReader::CalculateRMSPower(std::vector<short> vsIQData, int iFullScale_dBm, int iTotalGain_dB)
{
	float fAccum = 0.0f;
	float fTemp;
	for(unsigned i = 0; i < vsIQData.size(); i++) {
		fTemp = ((float)vsIQData[i]) / 32767.0f; // Full scale is +-MAX_SHORT = 2^15 = 32767
		fAccum += fTemp * fTemp;
	}
	fAccum /= (float)vsIQData.size();
	return 10.0f * log10(fAccum) + iFullScale_dBm - iTotalGain_dB;
}

bool cSocketSSNCReader::Connect(const string &strAddress, unsigned short usPort)
{
	// Create socket.
	if((m_hSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
#ifndef __unix__
		if(WSAGetLastError() == WSANOTINITIALISED) {
			WSADATA wsaData;
			WORD wVersionRequested = MAKEWORD(1, 1);
			if(WSAStartup(wVersionRequested, &wsaData) != 0)
				throw string("Failed to initialise WSA");
			if((m_hSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
#endif
				throw string("Failed to create socket!");
#ifndef __unix__
		}
#endif
	}

	// Disable TCP/IP delay.  This may or may not be desirable, depending on your application.
	int iTrue = 1;
	setsockopt(m_hSocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&iTrue, sizeof(int));

	// Then connect
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(usPort);
	if(isdigit(strAddress[0]))
		address.sin_addr.s_addr = inet_addr(strAddress.c_str());
	else {
		// Use this on Windows - Windows doesn't do getaddrinfo very well (VC6) and its
		//    gethostbyname is thread safe
		hostent *pHostent = gethostbyname(strAddress.c_str());
		if(!pHostent)
			throw string("Host lookup failure for " + strAddress);
		address.sin_addr.s_addr = *(unsigned long *)pHostent->h_addr_list[0];
	}
	if(address.sin_addr.s_addr == INADDR_NONE)
		return false;

	if(connect(m_hSocket, (sockaddr *)&address, sizeof(sockaddr_in)) != SOCKET_ERROR)
		return true;
	else
		return false;
}

cSocketSSNCReader::cSocketSSNCReader(string strHost, unsigned short usPortNo)
  : m_hSocket(INVALID_SOCKET),
	m_fFullScale_dBm(0.0f),
	m_fTotalGain_dB(0.0f)
{
	// Connect to socket
	if(!Connect(strHost, usPortNo)) {
		stringstream strstrError;
		strstrError << "Connection to " << strHost << ":" << usPortNo << " failed!";
		throw strstrError.str();
	}

	// m_hSocket is now ready to use

	// Register all the appropriate chunk handlers to the instance of this class
	m_PXGHandlerSSNC.registerCallbackHandler(this);
	m_PXGHandlerSIQP.registerCallbackHandler(this);
	m_PXGHandlerCF__.registerCallbackHandler(this);
	m_PXGHandlerBW__.registerCallbackHandler(this);
	m_PXGHandlerSR__.registerCallbackHandler(this);
	m_PXGHandler_dBTG.registerCallbackHandler(this);
	m_PXGHandler_dBFS.registerCallbackHandler(this);
	m_PXGHandlerIQDC.registerCallbackHandler(this);

	// Attach all the appropriate chunk handlers to the PXGF socket reader
	m_PXGReader.registerChunkHandler(m_PXGHandlerSSNC);
	m_PXGReader.registerChunkHandler(m_PXGHandlerSIQP);
	m_PXGReader.registerChunkHandler(m_PXGHandlerCF__);
	m_PXGReader.registerChunkHandler(m_PXGHandlerBW__);
	m_PXGReader.registerChunkHandler(m_PXGHandlerSR__);
	m_PXGReader.registerChunkHandler(m_PXGHandler_dBTG);
	m_PXGReader.registerChunkHandler(m_PXGHandler_dBFS);
	m_PXGReader.registerChunkHandler(m_PXGHandlerIQDC);

	// Init last tick variable.
	m_LastTime = std::chrono::system_clock::now();

	// The PXG Reader is now reay to accept a stream.
	try {
		// We pass it the cPXGReaderBase::Endian_Auto parameter to allow the library to automatically detect the endian
		// The function processStream will read from the stream and call the callback functions until it reaches the end of the stream (i.e. the socket closes or the file ends).
		m_PXGReader.processStream(m_hSocket, cPXGReaderBase::Endian_Auto);
	}
	catch(cPxgfException &e) {
		cout << e.toString() << endl;
	}
}

cSocketSSNCReader::~cSocketSSNCReader()
{
}

int main(int argc, char *argv[])
{
	string strHost;
	unsigned short usPort;

	// Validate command line arguments
	if(argc > 3) {
		cout << "Usage: \n\t" << argv[0] << " HostName|IpAddress PortNumber" << endl;
		return 1;
	}
	else if(argc == 3) {
		strHost = argv[1];
		usPort = atoi(argv[2]);
		if(usPort == 0) {
			cout << "Usage: \n\t" << argv[0] << " HostName|IpAddress PortNumber" << endl;
			return 1;
		}
	}
	else if(argc == 2) {
		strHost = argv[1];
		cout << "Enter port number for host " << strHost << ":" << endl;
		cin >> usPort;
	}
	else if(argc == 1) {
		cout << "Enter host name / IP Address of PXGF stream:" << endl;
		cin >> strHost;
		cout << "Enter port number for host " << strHost << ":" << endl;
		cin >> usPort;
	}

	try {
		cSocketSSNCReader(strHost, usPort);
	}
	catch(const string &strError) {
		cout << strError << endl;
		cout << "Enter x to exit:" << endl;
		char chDummy;
		cin >> chDummy;
		return 1;
	}
	return 0;
}
