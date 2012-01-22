# encoding: utf-8

require 'rubygems' unless defined?(Gem)
require 'rake' unless defined?(Rake)

# Prefer compiled Rubinius bytecode in .rbx/
ENV["RBXOPT"] = "-Xrbc.db"

require 'rake/extensiontask'
require 'rake/testtask'
begin
require 'rdoc/task'
rescue LoadError # fallback to older 1.8.7 rubies
require 'rake/rdoctask'
end

gemspec = eval(IO.read('relp.gemspec'))

Gem::PackageTask.new(gemspec) do |pkg|
end

Rake::ExtensionTask.new('relp', gemspec) do |ext|
  ext.name = 'relp_ext'
  ext.ext_dir = 'ext/relp'

  CLEAN.include 'ext/relp/dst'
  CLEAN.include 'ext/librelp-1.0.0'
  CLEAN.include 'lib/**/relp_ext.*'
end

Rake::RDocTask.new do |rd|
  files = FileList["README.rdoc", "lib/**/*.rb", "ext/relp/*.c"]
  rd.title = "Ruby wrapper for librelp, an easy to use library for the RELP (Reliable Event Logging Protocol) protocol."
  rd.main = "README.rdoc"
  rd.rdoc_dir = "doc"
  rd.options << "--promiscuous"
  rd.rdoc_files.include(files)
end

desc 'Run relp tests'
Rake::TestTask.new(:test) do |t|
  t.test_files = Dir.glob("test/**/test_*.rb")
  t.verbose = true
  t.warning = true
end

task :test => :compile
task :default => :test