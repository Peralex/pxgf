#ifndef PXGF_TO_MATLAB_H
#define PXGF_TO_MATLAB_H
// Copyright 2017 Peralex Electronics (Pty) Ltd
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

// This is an example of how to connect a PXG stream reader to to a socket,
// read the data from the stream, and print some information regarding the
// data read.  Note that it contains some code that only compiles on Visual Studio,
// but it should be simple to modify to run on Linux and gcc.

#include <fstream>
#include <vector>
#include <memory>

#include "PXGStreamReader.h"
#include "handlers/ChunkHandlerSIQP.h"
#include "handlers/ChunkHandlerSSNC.h"
#include "handlers/ChunkHandlerSSIQ.h"
#include "handlers/ChunkHandlerCF__.h"
#include "handlers/ChunkHandlerBW__.h"
#include "handlers/ChunkHandler_dBFS.h"
#include "handlers/ChunkHandler_dBTG.h"
#include "handlers/ChunkHandlerIQDC.h"
#include "handlers/ChunkHandlerSR__.h"

#include "handlers/ChunkHandlerGSNC.h"
#include "handlers/ChunkHandlerGFNC.h"
#include "handlers/ChunkHandlerFFS_.h"
#include "handlers/ChunkHandlerGIQP.h"
#include "handlers/ChunkHandlerGCF_.h"
#include "handlers/ChunkHandlerGCBW.h"

///< Class that reads in a PXGF file and outputs a .m file that can be loaded into matlab
///< Only handles SSIQ data and for a single frequency
class cPXGFToMatlab : public pxgf::cChunkHandlerSIQP::cCallbackSIQP,
					  public pxgf::cChunkHandlerSSNC::cCallbackSSNC,
					  public pxgf::cChunkHandlerSSIQ::cCallbackSSIQ,
					  public pxgf::cChunkHandlerCF__::cCallbackCF__,
					  public pxgf::cChunkHandlerBW__::cCallbackBW__,
					  public pxgf::cChunkHandler_dBFS::cCallback_dBFS,
					  public pxgf::cChunkHandler_dBTG::cCallback_dBTG,
					  public pxgf::cChunkHandlerIQDC::cCallbackIQDC,
					  public pxgf::cChunkHandlerSR__::cCallbackSR__,
					  public pxgf::cChunkHandlerGSNC::cCallbackGSNC,
				      public pxgf::cChunkHandlerGFNC::cCallbackGFNC,
					  public pxgf::cChunkHandlerFFS_::cCallbackFFS_,
					  public pxgf::cChunkHandlerGIQP::cCallbackGIQP,
					  public pxgf::cChunkHandlerGCF_::cCallbackGCF_,
					  public pxgf::cChunkHandlerGCBW::cCallbackGCBW
{
public:
	cPXGFToMatlab();
	~cPXGFToMatlab();

	void Process(const std::string &strFileIn, const std::string &strFileOut, unsigned uNumSamples, unsigned uOffset);

private:
	/// Opens the next output file. Whenever a discontinuity is detected a new .m file is created
	void OpenNextOutputFile();

	/// Close the current output file by writing out the remaining data and closing the file
	void CloseOutputFile();

	//pxgf handler callBacks
	void callbackSIQP(const pxgf::cPackingSIQP &oPacking) override;
	void callbackSSNC(int64_t lTimestamp_ns, const std::vector<short> &vsIqData) override;
	void callbackSSIQ(int64_t lTimestamp_us, const std::vector<short> &vsIqData) override;
	void callbackCF__(int64_t lCentreFrequency_uHz) override;
	void callbackBW__(int64_t lBandwidth_uHz) override;
	void callback_dBFS(float f_dBFullScale_dBm) override;
	void callback_dBTG(float fTotalGain_dB) override;
	void callbackSR__(int64_t lSampleRate_uHz) override;
	void callbackIQDC() override;
	void callbackGSNC(int64_t lTimestamp_ns, const std::vector<short> &vsIqData) override;
	void callbackGFNC(int64_t lTimestamp_ns, const std::vector<float> &vfIqData) override;
	void callbackFFS_(float fFullscale) override;
	void callbackGIQP(const pxgf::cPackingGIQP &oPacking) override;
	void callbackGCF_(const std::vector<int64_t> &vlCentreFrequencies_uHz) override;
	void callbackGCBW(int64_t lChannelBandwidth_uHz) override;

	enum {
		StreamType_Unknown,
		StreamType_Single,
		StreamType_Group
	} m_eStreamType;
	void ReCalcConversionFactor();

	//pxgf reader
	pxgf::cPXGStreamReader m_PXGReader;

	//pxgf handlers
	pxgf::cChunkHandlerSIQP m_PXGHandlerSIQP;
	pxgf::cChunkHandlerSSNC m_PXGHandlerSSNC;
	pxgf::cChunkHandlerSSIQ m_PXGHandlerSSIQ;
	pxgf::cChunkHandlerCF__ m_PXGHandlerCF__;
	pxgf::cChunkHandlerBW__ m_PXGHandlerBW__;
	pxgf::cChunkHandler_dBFS m_PXGHandler_dBFS;
	pxgf::cChunkHandler_dBTG m_PXGHandler_dBTG;
	pxgf::cChunkHandlerIQDC m_PXGHandlerIQDC;
	pxgf::cChunkHandlerSR__ m_PXGHandlerSR__;
	pxgf::cChunkHandlerGSNC m_PXGHandlerGSNC;
	pxgf::cChunkHandlerGFNC m_PXGHandlerGFNC;
	pxgf::cChunkHandlerFFS_ m_PXGHandlerFFS_;
	pxgf::cChunkHandlerGIQP m_PXGHandlerGIQP;
	pxgf::cChunkHandlerGCF_ m_PXGHandlerGCF_;
	pxgf::cChunkHandlerGCBW m_PXGHandlerGCBW;

	std::ifstream m_fsIn;
	std::ofstream m_fsOut;

	float m_fShortConversionFactor_V;			///< A scaling factor used to convert integer (short) sample values into input voltages.
	float m_fFloatConversionFactor_V;		///< A scaling factor used to convert float sample values into input voltages.
	int64_t m_lCenterFrequency_uHz;
	int64_t m_lBandwidth_uHz;
	int64_t m_lSampleRate_uHz;
	float m_fTotalGain_dB;
	float m_fFullScale_dBm;
	float m_fFloatFullscale;
	bool m_bIQPacked;
	std::vector<int64_t> m_vliGCF_uHz;

	std::shared_ptr<pxgf::cPackingGIQP> m_pGSIQPacking;

	bool m_bSIQPInit;
	bool m_bCF__Init;
	bool m_bBW__Init;
	bool m_b_dBFSInit;
	bool m_b_dBTGInit;
	bool m_bIQDCDetected;
	bool m_bSR__Init;
	bool m_bGCIQPInit;
	bool m_bGCF_Init;
	bool m_bGCBWInit;

	unsigned m_uSampleOffset;
	unsigned m_uNumSamples;
	unsigned m_uSkipSamples;

	std::string m_strFileOut;
	unsigned m_uOutputFileIndex;
};

#endif
