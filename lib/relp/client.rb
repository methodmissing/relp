require 'socket'

class Relp::Client
  def self.connect(sock_family = Socket::AF_INET, hostname = 'localhost', port = 518)
    engine = Relp::Engine.new
    client = new(engine)
    client.connect(sock_family, hostname, port)
    client
  end
end