#ifndef CHUNKHANDLERSSNR_H
#define CHUNKHANDLERSSNR_H
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

namespace pxgf
{
	/// Handler which deals with the Single channel Short Real chunk
	//
	/// This chunk handles a data stream containing real samples only.
	/// useful for ADC data or audio data.
	class cChunkHandlerSSNR : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackSSNR
		{
		public:
			/// Called on the callback handler whenever a SSNR
			/// chunk is processed.
			/// @param lTimestamp_ns Timestamp of first sample in frame
			/// @param vsRealData Vector of 16 bit samples
			virtual void callbackSSNR(int64_t lTimestamp_ns, const std::vector<short> &vsRealData) = 0;
		};

		/// Constructor
		cChunkHandlerSSNR();

		/// Destructor
		virtual ~cChunkHandlerSSNR(){};

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler The callback handler to be invoked to handle the processed data.
		void registerCallbackHandler(cCallbackSSNR *pHandler);

		/// Perform the appropriate processing required for this chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian True if the data is big-endian.
		/// @return true if the chunk was processed successfully or ignored.
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a SSNR chunk to the given stream writer
		/// @param stream Stream to write to
		/// @param lTimestamp_ns Timestamp of first sample in frame
		/// @param vsRealData Vector of 16 bit samples
		void writeChunkSSNR(cPXGWriterBase &stream, int64_t lTimestamp_ns, const std::vector<short> &vsRealData) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"SSNR"
			return 0x53534E52;
		}

	private:
		cCallbackSSNR *m_pCallbackSSNRHandler; ///< Pointer to object that implements the callback handler interface.
		std::vector<short> m_vsIqData; ///< Vector used to change endianness if required, and change from char to short.
	};
}

#endif
