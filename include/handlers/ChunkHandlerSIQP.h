#ifndef CHUNKHANDLERSIQP_H
#define CHUNKHANDLERSIQP_H
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
#include "packings/PackingSIQP.h"

namespace pxgf
{
	/// Chunk handler that describes how the data in a SSIQ chunk is packed.
	class cChunkHandlerSIQP : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackSIQP
		{
		public:
			/// Called on the callback handler whenever a SIQP
			/// chunk is processed.
			/// @param oPacking Object describing whether the data is IQ or QI packed
			virtual void callbackSIQP(const cPackingSIQP &oPacking) = 0;
		};

		/// Constructor
		cChunkHandlerSIQP();

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler The callback handler to be invoked to handle the processed data.
		void registerCallbackHandler(cCallbackSIQP *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian Is data big-endian?
		/// @return true if the chunk was processed successfully
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a SIQP chunk to the given stream writer
		/// @param stream Stream to write to
		/// @param oPacking Object describing whether the data is IQ or QI packed
		void writeChunkSIQP(cPXGWriterBase &stream, const cPackingSIQP &oPacking) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"SIQP"
			return 0x53495150;
		}

	private:
		cCallbackSIQP *m_pCallbackSIQPHandler; ///< Object which implements the callback handler interface.

		const cPackingSIQP m_oIQPacking;
		const cPackingSIQP m_oQIPacking;
	};
}

#endif
