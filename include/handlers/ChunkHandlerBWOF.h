#ifndef CHUNKHANDLERBWOF_H
#define CHUNKHANDLERBWOF_H
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

namespace pxgf
{
	/// Handler which deals with the Bandwidth offset chunk
	//
	/// The BWOF chunk defines what offset frequency the filter has if it is non-symetrical.
	///	An example of this might be an IF stream of an SSB signal which has its filter shifted by BW/2
	class cChunkHandlerBWOF : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackBWOF
		{
		public:
			/// Called on the callback handler whenever a BWOF
			/// chunk is processed.
			/// @param lBandwidth_uHz Bandwidth of filter in uHz
			/// @param lOffsetFrequency_uHz Offset of filter in uHz
			virtual void callbackBWOF(int64_t lBandwidth_uHz, int64_t lOffsetFrequency_uHz) = 0;
		};

		//Constructor
		cChunkHandlerBWOF();

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler Pointer to the callback handler that is to be used
		void registerCallbackHandler(cCallbackBWOF *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian True if the data should be stored in big-endian format, false otherwise.
		/// @return true if the chunk was processed successfully, false otherwise.
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a BWOF chunk to the given stream writer
		/// @param stream Stream to write to
		/// @param lBandwidth_uHz Bandwidth in uHz
		/// @param lOffsetFrequency_uHz Offset in uHz
		void writeChunkBWOF(cPXGWriterBase &stream, int64_t lBandwidth_uHz, int64_t lOffsetFrequency_uHz) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"BWOF"
			return 0x42574F46;
		}

	private:
		cCallbackBWOF *m_pCallbackBWOFHandler; ///< Object which implements the callback handler interface.
	};
}

#endif
