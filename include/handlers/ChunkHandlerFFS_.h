#ifndef CCHUNKHANDLERFFS__H
#define CCHUNKHANDLERFFS__H
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
#include "../PXGWriterBase.h"

namespace pxgf
{
	/// Handler which deals with the Float Full Scale chunk.  This chunk is used to specify the signal level that corresponds
	/// to the dBFS value, and is used with the floating point sample data chunks.
	class cChunkHandlerFFS_ : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackFFS_
		{
		public:
			/// Called on the callback handler whenever a FFS_
			/// chunk is processed.
			/// @param fFullscale   The float32 value that specifies the full scale
			///						(maximum positive swing) value of the samples in
			///						the  SFIQ, SFR_, SFNC and SFNR chunks.This is
			///						used together with the dBFS chunk to determine
			///						what the actual signal level in dBm is.
			virtual void callbackFFS_(float fFullscale) = 0;
		};

		/// Constructor
		cChunkHandlerFFS_();

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler The callback handler to be invoked to handle the processed data.
		void registerCallbackHandler(cCallbackFFS_ *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian True if the data is to be stored in big-endian format.
		/// @return true if the chunk was processed successfully.
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a FFS_ chunk to the given stream writer
		/// @param stream Stream to write to
		/// @param fFullscale The float32 value that specifies the full scale
		///						 (maximum positive swing) value of the samples
		void writeChunkFFS_(cPXGWriterBase &stream, float fFullscale) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"FFS_"
			return 0x4646535f;
		}

	private:
		cCallbackFFS_ *m_pCallbackFFS_Handler; ///< Object which implements the callback handler interface.
	};
}

#endif
