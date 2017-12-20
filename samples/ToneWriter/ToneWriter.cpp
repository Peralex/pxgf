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

#include <iostream>
#include <fstream>
#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <string>
#include <cassert>
#include <chrono>
#include <climits>

#include "handlers/ChunkHandlerSOFH.h"
#include "handlers/ChunkHandlerEOFH.h"
#include "handlers/ChunkHandlerSR__.h"
#include "handlers/ChunkHandlerSIQP.h"
#include "handlers/ChunkHandlerSSR_.h"
#include "handlers/ChunkHandlerSSNC.h"
#include "handlers/ChunkHandlerBW__.h"
#include "handlers/ChunkHandlerCF__.h"
#include "handlers/ChunkHandler_dBFS.h"
#include "handlers/ChunkHandler_dBTG.h"
#include "handlers/ChunkHandlerIQDC.h"
#include "handlers/ChunkHandlerGSIQ.h"
#include "handlers/ChunkHandlerGIQP.h"
#include "handlers/ChunkHandlerGCBW.h"
#include "handlers/ChunkHandlerGCF_.h"
#include "PXGStreamReader.h"

using namespace std;
using namespace pxgf;

int main(int argc, char *argv[])
{
	// The chunk handlers will be used for writing the relevant chunks to file.
	cChunkHandlerSOFH SOFH;
	cChunkHandlerEOFH EOFH;
	cChunkHandlerSSNC SSNC;
	cChunkHandlerSR__ SR__;
	cChunkHandlerBW__ BW__;
	cChunkHandlerCF__ CF__;
	cChunkHandler_dBFS dBFS;
	cChunkHandler_dBTG dBTG;
	cChunkHandlerSIQP SIQP;
	cChunkHandlerIQDC IQDC;
	cPackingSIQP Packing(true);

	int64_t bandwidth_uHz = INT64_C(40000000000); // 40 kHz.  The bandwidth tells us where the filter edges are.
	int64_t samplerate_uHz = INT64_C(102400000000); // 102.4 kHz
	int64_t centrefrequency_uHz = INT64_C(70000000000000); // 70 MHz

	cout << "Writing pxg data to file output.pxgf." << endl;
	ofstream outputfile("output.pxgf", ios::binary);

	// Create the stream writer, to be written in big-endian format.
	cPXGStreamWriter streamwriter(outputfile, false);


	auto WriteMetadata = [&]() {
		// Write the metadata chunks.  These should be repeated from time to time, preferably at least once per second.
		SR__.writeChunkSR__(streamwriter, samplerate_uHz);
		BW__.writeChunkBW__(streamwriter, bandwidth_uHz);
		CF__.writeChunkCF__(streamwriter, centrefrequency_uHz);
		// A sine wave of 2 dBm on the digitiser (ADC) will give a maximum value of SHRT_MAX on the digitiser output.
		// If your ADC does give +- 15-bit output, scale this value accordingly.
		dBFS.writeChunk_dBFS(streamwriter, 2.0f); // Full scale results from a 2 dBm analogue signal on the digitiser input.
		// The total gain between your system input and the digitiser output.  This allows the input signal level to be calculated.
		dBTG.writeChunk_dBTG(streamwriter, 0.0f);
		// The data will be packed into the SSNC chunk with I samples preceeding corresponding Q samples.
		SIQP.writeChunkSIQP(streamwriter, Packing);
	};

	// Write the SOFH and EOFH chunks with the dominant sample data type.  This is optional.
	SOFH.writeChunkSOFH(streamwriter, SSNC.getChunkType());
	WriteMetadata();
	EOFH.writeChunkEOFH(streamwriter);

	unsigned chunksamples = 1024;
	vector<short> vsSamples(chunksamples * 2); // 2 because it'll be I and Q data.
	// Create a complex exponential that'll fit one cycle into a chunk so that it can be duplicate easily without creating discontinuities.
	for(unsigned u = 0; u < chunksamples; u++) {
		vsSamples[u * 2    ] = static_cast<short>(static_cast<float>(SHRT_MAX) * cos(static_cast<float>(u) * 2.0f * static_cast<float>(M_PI) / static_cast<float>(chunksamples)));
		vsSamples[u * 2 + 1] = static_cast<short>(static_cast<float>(SHRT_MAX) * sin(static_cast<float>(u) * 2.0f * static_cast<float>(M_PI) / static_cast<float>(chunksamples)));
	}

	//Make the first timestamp roughly the current time.
	int64_t timestamp_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	assert(chunksamples <= 9200); //For larger numbers of samples it's necessary to use a different calculation for the timestamp below.
	// Duplicate the data a few times.
	for(unsigned u = 0; u < 1024; u++) {
		SSNC.writeChunkSSNC(streamwriter, timestamp_ns, vsSamples);
		// Increase the timestamp according to the sample rate.  The first factor of 1000000000 is because the timestamp is in nanoseconds.
		// The second factor of 1000000 is because the sample rate is in microhertz.
		timestamp_ns += (chunksamples * INT64_C(1000000000) * INT64_C(1000000)) / samplerate_uHz;
		//Write metadata every 16 chunks
		if(!(u % 16))
			WriteMetadata();
	}

	outputfile.close();

	return 0;
}
