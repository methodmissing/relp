# encoding: utf-8

require 'test/unit'
require 'relp'

Thread.abort_on_exception = true

class RelpTestCase < Test::Unit::TestCase
  undef_method :default_test if method_defined? :default_test

  def spawn_server(port, cb = nil)
    @server = Thread.new{ Relp::Server.bind(port, cb) }
    sleep 0.1 until listening?(port)
  end

  def kill_server
    @server.kill if @server
  end

  private
  def listening?(port)
    socket = TCPSocket.new('127.0.0.1', port)
    socket.close unless socket.nil?
    true
  rescue Errno::ECONNREFUSED, SocketError,
    Errno::EBADF,           # Windows
    Errno::EADDRNOTAVAIL    # Windows
    false
  end
end