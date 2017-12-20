#ifndef CCHUNKHANDLER_DBTG_H
#define CCHUNKHANDLER_DBTG_H
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
	/// Handler which deals with the TotalGain-dB chunk
	class cChunkHandler_dBTG : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallback_dBTG
		{
		public:
			/// Called on the callback handler whenever a _dBTG
			/// chunk is processed.
			/// @param fTotalGain_dB Total system gain in dB
			virtual void callback_dBTG(float fTotalGain_dB) = 0;
		};

		/// Constructor
		cChunkHandler_dBTG();

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler Pointer to the callback handler that is to be used
		void registerCallbackHandler(cCallback_dBTG *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian Is the stream big-endian?
		/// @return true if the chunk was processed successfully, false otherwise.
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a _dBTG chunk to the given stream writer
		/// @param stream Stream to write to
		/// @param fTotalGain_dB Total system gain in dB
		void writeChunk_dBTG(cPXGWriterBase &stream, float fTotalGain_dB) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"_dBTG"
			return 0x64425447;
		}

	private:
		cCallback_dBTG *m_pCallback_dBTGHandler; ///< Object which implements the callback handler interface.
	};
}

#endif
