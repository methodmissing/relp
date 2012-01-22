# encoding: utf-8

require 'mkmf'
require 'pathname'

def sys(cmd, err_msg)
  p cmd
  system(cmd) || fail(err_msg)
end

def fail(msg)
  STDERR.puts msg
  exit(1)
end

RbConfig::MAKEFILE_CONFIG['CC'] = ENV['CC'] if ENV['CC']
LIBRELP_VERSION = '1.0.0'

# XXX fallbacks specific to Darwin for JRuby (does not set these values in RbConfig::CONFIG)
LIBEXT = RbConfig::CONFIG['LIBEXT'] || 'a'
DLEXT = RbConfig::CONFIG['DLEXT'] || 'bundle'

cwd = Pathname(File.expand_path(File.dirname(__FILE__)))
dst_path = cwd + 'dst'
libs_path = dst_path + 'lib'
vendor_path = cwd + '..'
librelp_path = vendor_path + "librelp-#{LIBRELP_VERSION}"
librelp_include_path = librelp_path + 'src'

# extract dependencies
unless File.directory?(librelp_path)
  fail "The 'tar' (creates and manipulates streaming archive files) utility is required to extract dependencies" if `which tar`.strip.empty?
  Dir.chdir(vendor_path) do
    sys "tar xvzf librelp-#{LIBRELP_VERSION}.tar.gz", "Could not extract the librelp archive!"
  end
end

# build librelp
lib = libs_path + "librelp.#{LIBEXT}"
Dir.chdir librelp_path do
  # Replace select() with rb_thread_select() in virgin source
  sys "sed -ie \"s/select(/rb_thread_select(/g\" src/relpsess.c src/relp.c", "librelp rb_thread_select patch error!"
  sys "./configure --prefix=#{dst_path} --disable-shared --enable-static --enable-debug --with-pic && make && make install", "librelp compile error!"
end unless File.exist?(lib)

dir_config('relp')

have_func('rb_thread_blocking_region')

$INCFLAGS << " -I#{librelp_include_path}" if find_header("librelp.h", librelp_include_path)

if defined?(RUBY_ENGINE) && RUBY_ENGINE =~ /rbx/
  $LDFLAGS << " -L#{libs_path}"
else
  $LIBPATH << libs_path.to_s
end

fail "Error compiling and linking librelp" unless find_library("relp", "relpEngineConstruct", libs_path.to_s)

case RUBY_PLATFORM
when /darwin/, /linux/, /freebsd/
  CONFIG['LDSHARED'] = "$(CXX) " + CONFIG['LDSHARED'].split[1..-1].join(' ')
end

$defs << "-pedantic"

$CFLAGS << ' -Wall -funroll-loops'
$CFLAGS << ' -Wextra -O0 -ggdb3' if ENV['DEBUG']

create_makefile('relp_ext')