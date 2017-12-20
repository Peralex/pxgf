#ifndef CHUNKHANDLERTEXT_H
#define CHUNKHANDLERTEXT_H
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

#include <string>

#include "../ChunkHandler.h"
#include "../PXGStreamWriter.h"
#include "../exceptions/PxgfIllegalArgumentException.h"

namespace pxgf
{
	/// Handler which deals with the Text chunk
	//
	/// This chunk is used to add arbitraty text to the PXGF stream
	class cChunkHandlerTEXT : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackTEXT
		{
		public:
			/// Called on the callback handler whenever a TEXT
			/// chunk is processed.
			/// @param strMessage The text message.
			virtual void callbackTEXT(const std::string &strMessage) = 0;
		};

		/// Default Constructor
		cChunkHandlerTEXT();

		/// Constructor that allows for custom chunk ID specification
		cChunkHandlerTEXT(const std::string &strChunkType);

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler The callback handler to be invoked to handle the processed data.
		void registerCallbackHandler(cCallbackTEXT *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian Is data big-endian?
		/// @return true if the chunk was processed successfully
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a TEXT chunk to the given stream writer
		/// @param stream The output stream to write the chunk to.
		/// @param strMessage The text message to be written to the stream.
		void writeChunkTEXT(cPXGWriterBase &stream, const std::string &strMessage) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"TEXT"
			return 0x54455854;
		}

	private:
		cCallbackTEXT *m_pCallbackTEXTHandler; ///< Object which implements the callback handler interface.
	};
}

#endif
