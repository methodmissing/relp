# encoding: utf-8

require File.join(File.dirname(__FILE__), 'helper')

class TestEngine < RelpTestCase
  def test_alloc
    engine = Relp::Engine.new
    assert_instance_of Relp::Engine, engine
    assert_nil engine.destroy
  end

  def test_resolve
    engine = Relp::Engine.new
    assert_raises ArgumentError do
      engine.resolve = :invalid
    end
    engine.resolve = true
  end

  def test_comms
    messages = %w(m0 m1 m2 m3 m4 m5 m6 m7 m8 m9)
    received = []
    cb = Proc.new{|host,ip,msg| received << msg }
    spawn_server(5002, cb)
    engine = Relp::Engine.new
    ct = Thread.new do
      client = Relp::Client.new(engine)
      client.connect(Socket::AF_INET, 'localhost', 5002)
      messages.each{|m| client.send(m) }
    end
    sleep 1
    assert_equal received, messages
  ensure
    engine.destroy if engine
    kill_server
  end
end