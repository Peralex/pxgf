#ifndef PXGWRITERBASE_H
#define PXGWRITERBASE_H
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
#include <ostream>
#include <string>
#include <cstdint>

#include "ChunkHandler.h"

#define PXG_CHUNK_HEADER_SIZE (12) // = sizeof(m_piChunkHeader[3]) (which is an array of int32_t)

namespace pxgf
{
	/// Base class for writing PXGF streams
	//
	/// This class is used for writing PXG streams. It contains helper functions
	/// which take almost all the responsibility for dealing with different endian
	/// orderings on different platforms.
	/// Do not use this class directly, but rather make use of one the derived classes.
	class cPXGWriterBase
	{
		friend class cPXGReaderBase;

	public:
		/// Constructor
		//
		/// Constructor specifies output stream to write to and endian byte
		/// ordering to use in recording.
		/// @param bBigEndian Write stream in big-endian?
		cPXGWriterBase(const bool bBigEndian = false);

		/// Get whether the native environment is big-endian or not
		static bool getIsLocalBigEndian();

		/// Get whether the output is big-endian or not
		/// @return is the output stream to be recorded using big-endian?
		bool getIsBigEndian() const;

		/// Set the endian byte ordering used for writes
		void setEndianByteOrdering(bool bBigEndian);

		/// Writes a chunk header
		/// @param handler Handler from which the type is obtained
		/// @param iLength The length of the chunk in bytes
		void writeChunkHeader(const cChunkHandler &handler, int iLength);

		/// Write an integer value using the desired endian
		/// @param iValue Value to write
		void writeInteger(int32_t iValue);

		/// Write a long value using the desired endian
		/// @param lValue Value to write
		void writeInt64(int64_t lValue);

		/// Write a float value using the desired endian
		/// @param fValue Value to write
		void writeFloat(float fValue);

		/// Write a short array of values using the desired endian
		/// @param vsData Short values to write
		void writeShortArray(const std::vector<short> &vsData);

		/// Write an integer array of values using the desired endian
		/// @param viData Integer values to write
		void writeIntegerArray(const std::vector<int> &viData);

		/// Write an float array of values using the desired endian
		/// @param vfData Float values to write
		void writeFloatArray(const std::vector<float> &vfData);

		/// Write a char array to the output stream
		/// @param vchData Char values to write
		void writeCharArray(const std::vector<char> &vchData);

		/// Write a string to the output stream
		/// @param strMessage String to write
		void writeString(const std::string &strMessage);

		/// Write a raw chunk to the output stream.
		//
		/// Note that this function should only be used by cPXGStreamReader to
		/// facilitate in copying streams. Note that care should be taken when
		/// calling this function as the byte ordering of data in vchData must be
		/// correct for the output endian type otherwise you will have BIG problems!
		/// @param iType The chunk type
		/// @param vchData The binary data in the chunk
		virtual void writeRawChunk(int iType, const std::vector<char> &vchData);

		/// Get the special synchronisation word that each chunk starts with
		/// @return the synchronisation word
		inline static int32_t getSynchronisationWord()
		{
			return 0xa1b2c3d4;
		}

		/// Get the maximum length that chunks are allowed to be.
		/// @return the maximum legal chunk length
		inline static int32_t getMaxChunkLength()
		{
			return 65536 * 32;
		}

	protected:
		/// Write data to a sink defined by the derived class.
		/// @param pchData Array of data to write.
		/// @param iLength Length of data in pchData, in bytes.
		virtual void write(const char *pchData, std::streamsize iLength) = 0;

	private:
		bool m_bBigEndian; ///< Should the output stream be written using big-endian byte ordering?
		bool m_bSwapEndian; ///< Is the output endian different from the native endian?
		int32_t m_piChunkHeader[3]; ///< Array used to hold the chunk header to be written to disk
	};
}

#endif
