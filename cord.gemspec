Gem::Specification.new do |s|
  s.name = 'cord'
  s.version = '0.1'
  s.date = '2010-10-23'
  s.authors = ['Lourens Naudé']
  s.email = ['lourens@methodmissing.com']
  s.description = 'WIP implementation of a Concat Tree / Rope / Cord for Ruby MRI.'
  s.homepage = 'http://github.com/methodmissing/rehash'
  s.summary = 'WIP implementation of a Concat Tree / Rope / Cord for Ruby MRI.'
  s.extensions = 'ext/cord/extconf.rb'
  s.files = Dir.glob("{ext,test,bench}/**/*") + %w[README Rakefile cord.gemspec]
  s.has_rdoc = true
  s.extra_rdoc_files = Dir['ext/cord/*.c']
end