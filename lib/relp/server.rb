class Relp::Server
  def self.bind(port, cb = nil)
    engine = Relp::Engine.new
    server = new(engine)
    server.bind(port)
    server.on_receive(cb) if cb
    engine.run
    # we only get here in exceptional cases, return a Relp::Server instance for API compat with Relp::Client
    server
  end
end