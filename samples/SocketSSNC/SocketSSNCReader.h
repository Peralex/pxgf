// Copyright 2006 Peralex Electronics (Pty) Ltd
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>

#include "PXGSocketReader.h"
#include "handlers/ChunkHandlerSIQP.h"
#include "handlers/ChunkHandlerSSNC.h"
#include "handlers/ChunkHandlerCF__.h"
#include "handlers/ChunkHandlerBW__.h"
#include "handlers/ChunkHandler_dBFS.h"
#include "handlers/ChunkHandler_dBTG.h"
#include "handlers/ChunkHandlerIQDC.h"
#include "handlers/ChunkHandlerSR__.h"

#ifdef __unix__
typedef int SOCKET;
#endif

/// Class that simply provides an example on how to use the cPXGSocketReader class.
/// It reads a SSIQ based PXGF stream and prints out information relating to the data.
/// Inherit from the callback interfaces of all of the chunk types that you'll be reading.
class cSocketSSNCReader : public pxgf::cChunkHandlerSIQP::cCallbackSIQP,
						  public pxgf::cChunkHandlerSSNC::cCallbackSSNC,
						  public pxgf::cChunkHandlerCF__::cCallbackCF__,
						  public pxgf::cChunkHandlerBW__::cCallbackBW__,
						  public pxgf::cChunkHandler_dBFS::cCallback_dBFS,
						  public pxgf::cChunkHandler_dBTG::cCallback_dBTG,
						  public pxgf::cChunkHandlerIQDC::cCallbackIQDC,
						  public pxgf::cChunkHandlerSR__::cCallbackSR__
{
public:
	/// Constructor
	/// @param strHost Hostname or IP Address of socket server that is streaming the relevant PXGf stream
	/// @param usPortNo Port number on host to connect to
	cSocketSSNCReader(std::string strHost, unsigned short usPortNo);

	/// Destructor
	virtual ~cSocketSSNCReader();

	// pxgf handler callbacks, overridden from the inherited cCallback classes.
	void callbackSIQP(const pxgf::cPackingSIQP &oPacking) override;
	void callbackSSNC(int64_t lTimestamp_ns, const std::vector<short> &vsIqData) override;
	void callbackCF__(int64_t lCentreFrequency_uHz) override;
	void callbackBW__(int64_t lBandwidth_uHz) override;
	void callback_dBFS(float f_dBFullScale_dBm) override;
	void callback_dBTG(float fTotalGain_dB) override;
	void callbackIQDC() override;
	void callbackSR__(int64_t lSampleRate_uHz) override;

private:
	/// Handles the connection to a socket server
	/// @param strAddress The network address (either DNS name or IP address in string format (e.g. "192.168.0.1")) of the server.
	/// @param usPort The TCP port to connect to.
	/// @return True if connection to socket was successful.
	bool Connect(const std::string &strAddress, unsigned short usPort);

	/// Calculates RMS power of the given IQ data and returns it in dBm.
	/// @param vsIQData Vector of 16 bit signed integers, with I and Q samples interleaved.
	/// @param iFullScale_dBm The level (in dBm) represented by a full scale sample (MAX_SHORT).
	/// @param iTotalGain_dBm The gains applied by all stages before sampling.
	/// @return The RMS power in dBm.
	float CalculateRMSPower(std::vector<short> vsIQData, int iFullScale_dBm, int iTotalGain_dB);

	SOCKET m_hSocket; ///< Socket handle to PXGF socket stream.
	pxgf::cPXGSocketReader m_PXGReader; ///< pxgf socket reader.

	// pxgf handlers.  Add one of these for each chunk type that you need to process.  You can ignore chunk types that you're not interested in.
	pxgf::cChunkHandlerSIQP m_PXGHandlerSIQP;
	pxgf::cChunkHandlerSSNC m_PXGHandlerSSNC;
	pxgf::cChunkHandlerCF__ m_PXGHandlerCF__;
	pxgf::cChunkHandlerBW__ m_PXGHandlerBW__;
	pxgf::cChunkHandler_dBFS m_PXGHandler_dBFS;
	pxgf::cChunkHandler_dBTG m_PXGHandler_dBTG;
	pxgf::cChunkHandlerIQDC m_PXGHandlerIQDC;
	pxgf::cChunkHandlerSR__ m_PXGHandlerSR__;

	// Local copy of PXGF stream attributes.  Many of these are in different units from those in the stream.
	unsigned m_uCentreFrequency_Hz; ///< Centre frequency in Hz.
	unsigned m_uSampleRate_Hz; ///< Sample rate in Hz.
	unsigned m_uBandwidth_Hz; ///< Bandwidth in Hz.
	float m_fTotalGain_dB; ///< Total gain in dB of the analogue components (receiver, ADC gain stage, pre-amps, etc.).
	float m_fFullScale_dBm; ///< The level (in dBm) represented by a full scale (SHRT_MAX) sample value.
	bool m_bIsIQPacked; ///< True if data is in IQIQIQ... order or false if in QIQIQI... order.

	std::chrono::time_point<std::chrono::system_clock> m_LastTime; ///< Used to limit amount of info printed to screen
};
