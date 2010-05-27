/*
    Copyright (C) 2010 ruth(f.c.s) (deinococcus.radiophilus@gmail.com)
    partially based on GPL code by Jonah Sherman <sherman.jonah@gmail.com>
    that code was based on:
    AdBlockPlus by Wladimir Palant <trev@adblockplus.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#include "AdBlock.h"

#include "base/pickle.h"
#include "base/unix_domain_socket_posix.h"
#include "chrome/common/sandbox_methods_linux.h"

#include <iostream>

using namespace std;

static const int kMagicSandboxIPCDescriptor = 5;

namespace WebCore {

  // Proxy to browser
  bool shouldbeBlocked(const KURL& url) {
    String urlString = url.string();
    if (!urlString.startsWith("http://")) {
      return false;
    }

    //    cout << "Sending request " << urlString.utf8().data() << " to browser" << endl;
    Pickle request;
    uint8_t reply_buf[256];
    request.WriteInt(LinuxSandbox::METHOD_SHOULD_BLOCK_URL);
    request.WriteString(urlString.utf8().data());
    const ssize_t r = base::SendRecvMsg(kMagicSandboxIPCDescriptor,
                                        reply_buf, sizeof(reply_buf),
                                        NULL, request);
    if (r == -1) {
      cerr << "AdBlock: Request failed!!" << endl;
      return false;
    }

    Pickle reply(reinterpret_cast<char*>(reply_buf), r);
    void* iter = NULL;
    bool ret;
    if (!reply.ReadBool(&iter, &ret))  {
      cerr << "AdBlock: Request failed!!" << endl;
      return false;
    }
    return ret;
  }

}
