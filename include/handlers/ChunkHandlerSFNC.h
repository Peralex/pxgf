#ifndef CHUNKHANDLERSFNC_H
#define CHUNKHANDLERSFNC_H
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
	/// Handler which deals with the Single channel Float IQ time data chunk, with nanosecond timestamps.
	class cChunkHandlerSFNC : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackSFNC
		{
		public:
			/// Called on the callback handler whenever a SFNC
			/// chunk is processed.
			/// @param lTimestamp_ns Timestamp of first sample in frame
			/// @param vfIqData Vector of float32 samples. The SFNC chunk is used to
			/// determine how this is packed
			virtual void callbackSFNC(int64_t lTimestamp_ns, const std::vector<float> &vfIqData) = 0;
		};

		/// Constructor
		cChunkHandlerSFNC();

		/// Destructor
		virtual ~cChunkHandlerSFNC(){};

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler The callback handler to be invoked to handle the processed data.
		void registerCallbackHandler(cCallbackSFNC *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian Is data big-endian?
		/// @return true if the chunk was processed successfully
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a SFNC chunk to the given stream writer
		/// @param stream Stream to write to
		/// @param lTimestamp_ns Timestamp of first sample in frame
		/// @param vfIqData Vector of float32 containing the packed channel time data.
		/// The stream should also contain a SIQP chunk to describe how the data is packed
		void writeChunkSFNC(cPXGWriterBase &stream, int64_t lTimestamp_ns, const std::vector<float> &vfIqData) const;

		/// Repack the data from the packing specified to the output packing specified
		/// @param oInputPacking The packing that the data currently has
		/// @param oOutputPacking The desired packing for the data
		/// @param vfInputIQData Data to be repacked
		/// @param vfOutputData Repacked data
		static void repack(const cPackingSIQP &oInputPacking, const cPackingSIQP &oOutputPacking, const std::vector<float> &vfInputIQData, std::vector<float> &vfOutputData);

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"SFNC"
			return 0x53464E43;
		}

	private:
		cCallbackSFNC *m_pCallbackSFNCHandler; ///< Pointer to object that implements the callback handler interface.

		std::vector<float> m_vfIqData; ///< Vector used to change endianness if required, and change from char to float.
	};
}

#endif
