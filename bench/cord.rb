$:.unshift "."
require 'benchmark'
require File.dirname(__FILE__) + '/../ext/cord/cord'

TESTS = 10_000
PAYLOADS = [
('a' * 5),
('b' * 10),
('c' * 20),
('d' * 40),
('e' * 80),
('f' * 120),
('g' * 200),
('h' * 300)
]

string = ''
cord = Cord.new
ary = []

Benchmark.bmbm do |results|
  results.report("[cord] obj << a") { TESTS.times{ cord << PAYLOADS[rand(6)] } }
  results.report("[str] obj << a") { TESTS.times{ string << PAYLOADS[rand(6)] } }
  results.report("[ary] obj << a") { TESTS.times{ ary << PAYLOADS[rand(6)] } }
end
