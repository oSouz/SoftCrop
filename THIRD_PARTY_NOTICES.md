# Third-party notices

Public SDK: Resolume/FFGL v2.1, commit 6697342f153cba9df9fd089a56d29ca87712fe30.
Source: https://github.com/resolume/ffgl/tree/v2.1
Original per-file copyright and license headers are preserved in vendor/ffgl.
The following SDK header is reproduced for binary redistribution:

```text
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FFGL.cpp
//
// FreeFrame is an open-source cross-platform real-time video effects plugin system.
// It provides a framework for developing video effects plugins and hosts on Windows,
// Linux and Mac OSX.
//
// FreeFrameGL (FFGL) is an extension to the FreeFrame spec to support video processing
// with OpenGL on Windows, Linux, and Mac OSX.
//
// Copyright (c) 2018 www.freeframe.org
// All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Redistribution and use in source and binary forms, with or without modification,
//	are permitted provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the
//    distribution.
//  * Neither the name of FreeFrame nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//
//	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//	IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
//	INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
//	OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
//	OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
//	OF THE POSSIBILITY OF SUCH DAMAGE.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// First version, Marcus Clements (marcus@freeframe.org)
// www.freeframe.org
//
// FreeFrame 1.0 upgrade by Russell Blakeborough
// email: boblists@brightonart.org
//
// FreeFrame 1.0 - 03 upgrade
// and implementation of FreeFrame SDK methods by Gualtiero Volpe
// email: Gualtiero.Volpe@poste.it
//
// FFGL upgrade by Trey Harrison
// email: trey@harrisondigitalmedia.com
//
// FFGL 2.0 by Menno Vink (menno@resolume.com)
// www.resolume.com
// -Removed the old FreeFrame functionality, and thus CPU processing is no longer supported.
// -Completed the FF_TYPE_OPTION parameter type. Plugins can add parameter elements which hosts
//  should use to show a dropdown styled parameter. When one of the options is selected the parameter
//  will get a value equal to that of the option's value.
// -Completed the FF_TYPE_BUFFER parameter type. When this parameter is used it should provide a usage indication
//  which the host can then use to fill that buffer with the requested data. This can be used by plugins to
//  access the host's global fft data for example.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


```

The bundled GL loader includes MIT-licensed files by Jason L. McKesson,
Copyright (C) 2011-2013. Their original notices are preserved in the SDK.

MIT License (GL loader)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
