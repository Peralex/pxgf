#ifndef CHUNKHANDLERSOFH_H
#define CHUNKHANDLERSOFH_H
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
	/// Chunk handler to handle Start Of File Header chunks.
	class cChunkHandlerSOFH : public cChunkHandler
	{
	public:
		/// Callback handler interface.
		class cCallbackSOFH
		{
		public:
			/// Called on the callback handler whenever a SOFH
			/// chunk is processed.
			/// @param iFormat The chunk type ID of the principle data chunk type.  This will generally the chunk type ID of SSIQ or SSNC.
			virtual void callbackSOFH(int iFormat) = 0;
		};

		/// Creates a new instance of cChunkHandlerSOFH
		cChunkHandlerSOFH();

		/// Register the object which will provide an implementation of this handler's callback function.
		/// @param pHandler The callback handler to be invoked to handle the processed data.
		void registerCallbackHandler(cCallbackSOFH *pHandler);

		/// Perform the appropriate processing required for this chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian True if the data is to be written in big-endian format.
		/// @return true if the chunk was processed successfully or ignored.
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a SOFH chunk to the given stream writer
		/// @param stream The output PXGWriter stream.
		/// @param iFormat The chunk type ID of the principle data chunk type.  This is generally the chunk type ID of SSIQ or SSNC.
		void writeChunkSOFH(cPXGWriterBase &stream, int iFormat) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"SOFH"
			return 0x534F4648;
		}

	private:
		cCallbackSOFH *m_pCallbackSOFHHandler; ///< Object which implements the callback handler interface.
	};
}

#endif
