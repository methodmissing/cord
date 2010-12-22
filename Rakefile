#!/usr/bin/env rake
require 'rake/testtask'
require 'rake/clean'
$:.unshift(File.expand_path('lib'))
CORD_ROOT = 'ext/cord'

desc 'Default: test'
task :default => :test

desc 'Run cord tests.'
Rake::TestTask.new(:test) do |t|
  t.libs = [CORD_ROOT]
  t.pattern = 'test/test_*.rb'
  t.ruby_opts << '-rtest'
  t.libs << 'test'
  t.warning = true
  t.verbose = true
end
task :test => :build

namespace :build do
  file "#{CORD_ROOT}/cord.c"
  file "#{CORD_ROOT}/extconf.rb"
  file "#{CORD_ROOT}/Makefile" => %W(#{CORD_ROOT}/cord.c #{CORD_ROOT}/extconf.rb) do
    Dir.chdir(CORD_ROOT) do
      ruby 'extconf.rb'
    end
  end

  desc "generate makefile"
  task :makefile => %W(#{CORD_ROOT}/Makefile #{CORD_ROOT}/cord.c)

  dlext = Config::CONFIG['DLEXT']
  file "#{CORD_ROOT}/cord.#{dlext}" => %W(#{CORD_ROOT}/Makefile #{CORD_ROOT}/cord.c) do
    Dir.chdir(CORD_ROOT) do
      sh 'make' # TODO - is there a config for which make somewhere?
    end
  end

  desc "compile cord extension"
  task :compile => "#{CORD_ROOT}/cord.#{dlext}"

  task :clean do
    Dir.chdir(CORD_ROOT) do
      sh 'make clean'
    end if File.exists?("#{CORD_ROOT}/Makefile")
  end

  CLEAN.include("#{CORD_ROOT}/Makefile")
  CLEAN.include("#{CORD_ROOT}/cord.#{dlext}")
end

task :clean => %w(build:clean)

desc "compile"
task :build => %w(build:compile)

task :install do |t|
  Dir.chdir(CORD_ROOT) do
    sh 'sudo make install'
  end
end

desc "clean build install"
task :setup => %w(clean build install)

desc 'Run benchmarks'
task :bench do
  ruby "bench/cord.rb"
end
task :bench => :build

desc 'Run qsort benchmarks from RubyQuiz'
task :bench_qsort do
  ruby "bench/quiz.rb String"
  ruby "bench/quiz.rb Cord"
end
task :bench_qsort => :build