# encoding: utf-8

#--
#
# Author:: Lourens Naudé
# Homepage::  http://github.com/methodmissing/rbczmq
# Date:: 20120123
#
#----------------------------------------------------------------------------
#
# Copyright (C) 2012 by Lourens Naudé. All Rights Reserved.
# Email: lourens at methodmissing dot com
#
# (The MIT License)
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# 'Software'), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
#---------------------------------------------------------------------------
#
#

# Prefer compiled Rubinius bytecode in .rbx/
ENV["RBXOPT"] = "-Xrbc.db"

begin
  require "relp/relp_ext"
rescue LoadError
  require "relp_ext"
end

require 'relp/version' unless defined? Relp::VERSION

module Relp

  # Map numeric librelp return codes to end user friendly strings
  #
  ERRORS = {
    OUT_OF_MEMORY => "out of memory occured",
    INVALID_FRAME => "relp frame received is invalid",
    PARAM_ERROR => "an (API) calling parameer is in error",
    INVALID_PORT => "invalid port value",
    COULD_NOT_BIND => "could not bind socket, defunct",
    ACCEPT_ERR => "error during accept() system call",
    SESSION_BROKEN => "the RELP session is broken",
    SESSION_CLOSED => "the RELP session was closed (not an error)",
    INVALID_CMD => "the command contained in a RELP frame was unknown",
    DATA_TOO_LONG => "DATALEN exceeds permitted length",
    INVALID_TXNR => "a txnr is invalid (probably code error)",
    INVALID_DATALEN => "DATALEN field is invalid (probably code error)",
    PARTIAL_WRITE => "only partial data written (state, not an error)",
    IO_ERR => "IO error occured",
    TIMED_OUT => "timeout occured",
    NOT_FOUND => "searched entity not found",
    NOT_IMPLEMENTED => "functionality not implemented",
    INVALID_RSPHDR => "'rsp' packet header is invalid",
    END_OF_DATA => "no more data available",
    RSP_STATE_ERR => "error status in relp rsp frame",
    INVALID_OFFER => "invalid offer (e.g. malformed) during open",
    UNKNOWN_CMD => "command is unknown (e.g. not in this version)",
    CMD_DISABLED => "tried to use a cmd that is disabled in this session",
    INVALID_HDL => "invalid object handle (pointer) provided by caller",
    INCOMPAT_OFFERS => "client and server offers are incompatible",
    RQD_FEAT_MISSING => "the remote peer does not support a feature required by us",
    MALICIOUS_HNAME => "remote peer is trying malicious things with its hostname",
    INVALID_HNAME => "remote peer's hostname invalid or unobtainable",
    ADDR_UNKNOWN => "remote peer's IP address could not be obtained",
    INVALID_PARAM => "librelp API called with wrong parameter"
  }

  def self.error(code)
    ERRORS[code]
  end
end

require 'relp/client'
require 'relp/server'