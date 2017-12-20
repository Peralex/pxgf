#ifndef RESETHANDLER_H
#define RESETHANDLER_H
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

namespace pxgf
{
	/// Interface to be implemented in order to provide a mechanism for resetting
	/// state in the application when cPXGStreamReader resynchronises with the data
	/// stream.
	class cResetHandler
	{
	public:
		/// Callback function used for resetting application state
		//
		/// This function should be implemented in the application and take
		/// responsibility for resetting the stream state. It will be called
		/// whenever the engine resynchronised with the input stream.
		virtual void resetState() = 0;
	};
}

#endif
