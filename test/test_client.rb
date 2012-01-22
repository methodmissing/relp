# encoding: utf-8

require File.join(File.dirname(__FILE__), 'helper')

class TestClient < RelpTestCase
  def test_alloc
    engine = Relp::Engine.new
    assert_raises TypeError do
      Relp::Client.new(:invalid)
    end
    client = Relp::Client.new(engine)
    assert_instance_of Relp::Client, client
    assert_nil client.destroy
  end

  def test_connect
    assert_raises TypeError do
      Relp::Client.connect(Socket::AF_INET, :invalid, 5000)
    end
    spawn_server(5000)
    Relp::Client.connect(Socket::AF_INET, 'localhost', 5000)
  ensure
    kill_server
  end

  def test_send
    spawn_server(5003)
    client = Relp::Client.connect(Socket::AF_INET, 'localhost', 5003)
    assert client.send("test")
  ensure
    kill_server
  end

  def test_connect_without_server
    # XXX: fixed 10s connect timeout in librelp
    Relp::Client.connect(Socket::AF_INET, 'localhost', 5000)
  rescue => e
    assert_match(/timeout occured/, e.message)
  end
end