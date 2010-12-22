require 'mkmf'

dir_config('cord')
have_func('rb_thread_blocking_region')
$defs.push("-pedantic")
create_makefile('cord')