# encoding: utf-8

require File.expand_path('../lib/relp/version', __FILE__)

Gem::Specification.new do |s|
  s.name = "relp"
  s.version = Relp::VERSION
  s.summary = "Ruby wrapper for librelp, an easy to use library for the RELP (Reliable Event Logging Protocol) protocol."
  s.description = "Ruby wrapper for librelp, an easy to use library for the RELP (Reliable Event Logging Protocol) protocol."
  s.authors = ["Lourens Naudé", "Erkki Eilonen"]
  s.email = ["lourens@methodmissing.com", "erkki@itech.ee"]
  s.homepage = "http://github.com/methodmissing/relp"
  s.date = Time.now.utc.strftime('%Y-%m-%d')
  s.platform = Gem::Platform::RUBY
  s.extensions = "ext/relp/extconf.rb"
  s.has_rdoc = true
  s.files = Dir.glob("*/**/*")
  s.test_files = Dir.glob("test/*")
  s.rdoc_options = ["--charset=UTF-8"]
  s.require_paths = ["lib"]
  s.add_development_dependency('rake-compiler', '~> 0.8.0')
end
