#ifndef PXGSTREAMWRITER_H
#define PXGSTREAMWRITER_H
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

#include <ostream>

#include "PXGWriterBase.h"
#include "ChunkHandler.h"

namespace pxgf
{
	/// PXGWriter class that writes its PXGF stream to a std::ostream.
	//
	/// This class is used for writing PXG streams. It contains helper functions
	/// which take almost all the responsibility for dealing with different endian
	/// orderings on different platforms.
	class cPXGStreamWriter : public cPXGWriterBase
	{
	public:
		/// Constructor
		//
		/// Constructor specifies output stream to write to and endian byte
		/// ordering to use in recording.
		/// @param Stream Valid ouput stream to write to
		/// @param bBigEndian Write stream in big-endian?
		cPXGStreamWriter(std::ostream &Stream, const bool bBigEndian);

		/// Get the output stream
		/// @return the output stream that is being written to
		std::ostream &getOutputStream();

	protected:
		virtual void write(const char *pchData, std::streamsize nLength) override;

	private:
		std::ostream &m_stream; /// The output stream.
	};
}

#endif
