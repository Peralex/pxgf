#ifndef CHUNKHANDLERGIQP_H
#define CHUNKHANDLERGIQP_H
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
	/// Handler which deals with the group channel packing chunk
	class cChunkHandlerGIQP : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackGIQP
		{
		public:
			/// Called on the callback handler whenever a GIQP
			/// chunk is processed.
			/// @param oPacking Object describing how the IQ data is packed
			/// in the GSIQ stream
			virtual void callbackGIQP(const cPackingGIQP &oPacking) = 0;
		};

		/// Contructor
		cChunkHandlerGIQP();

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler The callback handler to be invoked to handle the processed data.
		void registerCallbackHandler(cCallbackGIQP *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian Is data big-endian?
		/// @return true if the chunk was processed successfully.
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a GIQP chunk to the given stream writer
		/// @param stream Stream to write to
		/// @param oPacking Object describing how the IQ data is packed
		/// in the GSIQ stream
		void writeChunkGIQP(cPXGWriterBase &stream, const cPackingGIQP &oPacking) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"GIQP"
			return 0x47495150;
		}

	private:
		cCallbackGIQP *m_pCallbackGIQPHandler; ///< Object which implements the callback handler interface.
	};
}

#endif
