# encoding: utf-8

require File.join(File.dirname(__FILE__), 'helper')

class TestServer < RelpTestCase
  def test_alloc
    engine = Relp::Engine.new
    assert_raises TypeError do
      Relp::Server.new(:invalid)
    end
    server = Relp::Server.new(engine)
    assert_instance_of Relp::Server, server
    assert_nil server.destroy
  end

  def test_bind
    engine = Relp::Engine.new
    server = Relp::Server.new(engine)
    server.bind(5001)
    sleep 0.5
    socket = TCPSocket.new('127.0.0.1', 5001)
    assert socket
    socket.close
    engine.destroy
  end

  def test_on_receive
    engine = Relp::Engine.new
    server = Relp::Server.new(engine)
    assert server.on_receive(Proc.new{|host,ip,msg| nil })
  end
end