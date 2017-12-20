#ifndef CHUNKHANDLERSFNR_H
#define CHUNKHANDLERSFNR_H
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

#include <vector>

#include "../ChunkHandler.h"
#include "../PXGStreamWriter.h"
#include "packings/PackingSIQP.h"

namespace pxgf
{
	/// Handler which deals with the Single channel Float Real time data chunk with nanosecond timestamps.
	class cChunkHandlerSFNR : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackSFNR
		{
		public:
			/// Called on the callback handler whenever a SFNR
			/// chunk is processed.
			/// @param lTimestamp_ns Timestamp of first sample in frame
			/// @param vfRealData Vector of float32 samples.
			virtual void callbackSFNR(int64_t lTimestamp_ns, const std::vector<float> &vfRealData) = 0;
		};

		/// Constructor
		cChunkHandlerSFNR();

		/// Destructor
		virtual ~cChunkHandlerSFNR(){};

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler The callback handler to be invoked to handle the processed data.
		void registerCallbackHandler(cCallbackSFNR *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian Is data big-endian?
		/// @return true if the chunk was processed successfully
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a SFNR chunk to the given stream writer
		/// @param stream Stream to write to
		/// @param lTimestamp_ns Timestamp of first sample in frame
		/// @param vfRealData Vector of float32 containing the packed channel time data.
		/// The stream should also contain a SIQP chunk to describe how the data is packed
		void writeChunkSFNR(cPXGWriterBase &stream, int64_t lTimestamp_ns, const std::vector<float> &vfRealData) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"SFNR"
			return 0x53464E52;
		}

	private:
		cCallbackSFNR *m_pCallbackSFNRHandler; ///< Pointer to object that implements the callback handler interface.

		std::vector<float> m_vfRealData; ///< Vector used to change endianness if required, and change from char to float.
	};
}

#endif
