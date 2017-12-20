// Copyright 2017 Peralex Electronics (Pty) Ltd
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

#include <iostream>

#include "PXGFToMatlab.h"

using namespace std;

int main(int argc, char *argv[])
{
	cout << endl;
	cout << "------------------------------------------------" << endl;
	cout << "PXGF to Matlab converter (v1.00T002)" << endl;
	cout << "------------------------------------------------" << endl;
	cout << "Build: " << __DATE__ << " " << __TIME__ << endl;
	cout << "------------------------------------------------" << endl;
	cout << endl;

	if(argc != 5) {
		cout << "Usage: " << argv[0] << " PXGF_filename <output file> <num samples> <sample offset>" << endl;
		return 1;
	}

	cPXGFToMatlab proc;

	proc.Process(argv[1], argv[2], atol(argv[3]), atol(argv[4]));

	return 0;
}
