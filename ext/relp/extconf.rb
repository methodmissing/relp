# encoding: utf-8

require 'mkmf'

RbConfig::MAKEFILE_CONFIG['CC'] = ENV['CC'] if ENV['CC']

# XXX fallbacks specific to Darwin for JRuby (does not set these values in RbConfig::CONFIG)
LIBEXT = RbConfig::CONFIG['LIBEXT'] || 'a'
DLEXT = RbConfig::CONFIG['DLEXT'] || 'bundle'

dir_config('relp')

have_func('rb_thread_blocking_region')

case RUBY_PLATFORM
when /darwin/, /linux/, /freebsd/
  CONFIG['LDSHARED'] = "$(CXX) " + CONFIG['LDSHARED'].split[1..-1].join(' ')
end

$defs << "-pedantic"

$CFLAGS << ' -Wall -funroll-loops'
$CFLAGS << ' -Wextra -O0 -ggdb3' if ENV['DEBUG']

fail "Error compiling and linking librelp" unless find_library("relp", "relpEngineConstruct")

create_makefile('relp_ext')
