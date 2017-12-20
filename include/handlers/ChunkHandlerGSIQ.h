#ifndef CHUNKHANDLERGSIQ_H
#define CHUNKHANDLERGSIQ_H
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

#include "../ChunkHandler.h"
#include "../PXGStreamWriter.h"
#include "packings/PackingGIQP.h"

namespace pxgf
{
	/// Handler which deals with the Group Short IQ time data chunk.
	class cChunkHandlerGSIQ : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackGSIQ
		{
		public:
			/// Called on the callback handler whenever a GSIQ
			/// chunk is processed.
			/// @param lTimestamp_us Timestamp of first sample in frame
			/// @param vsIqData Vector of 16 bit integer samples. The GIQP chunk is used to
			/// determine how this is packed
			virtual void callbackGSIQ(int64_t lTimestamp_us, const std::vector<short> &vsIqData) = 0;
		};

		/// Constructor
		cChunkHandlerGSIQ();

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler The callback handler to be invoked to handle the processed data.
		void registerCallbackHandler(cCallbackGSIQ *pHandler);

		/// Perform the appropriate processing required for this chunk.
		//
		/// If any problems are detected during parsing of the data then the
		/// return value should be false.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian Is data big-endian?
		/// @return true if the chunk was processed successfully
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param stream Stream to write to
		/// @param lTimestamp_us Timestamp of first sample in frame
		/// @param vsIqData Vector of shorts containing the packed channel time data.
		void writeChunkGSIQ(cPXGWriterBase &stream, int64_t lTimestamp_us, const std::vector<short> &vsIqData) const;

		/// Repack the data from the packing specified to the output packing specified
		/// @param oInputPacking The packing that the data currently has
		/// @param oOutputPacking The desired packing for the data
		/// @param vsInputIQData Data to be repacked
		/// @param vsOutputIQData Repacked data
		static void repack(const cPackingGIQP &oInputPacking, const cPackingGIQP &oOutputPacking, const std::vector<short> &vsInputIQData, std::vector<short> &vsOutputIQData);

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"GSIQ"
			return 0x47534951;
		}

	private:
		cCallbackGSIQ *m_pCallbackGSIQHandler; ///< Object which implements the callback handler interface.
	};
}

#endif
