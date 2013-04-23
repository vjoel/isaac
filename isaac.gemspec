Gem::Specification.new do |s|
  s.name = "prng-isaac"
  s.version = "0.0.5"

  s.required_rubygems_version = Gem::Requirement.new(">= 0")
  s.authors = ["Joel VanderWerf"]
  s.date = "2013-04-22"
  s.description = "A pseudorandom number generator based on Bob Jenkins' ISAAC; useful for both simulation and cryptography."
  s.email = "vjoel@users.sourceforge.net"
  s.extra_rdoc_files = ["RELEASE-NOTES", "README.md", "COPYING"]
  s.files = Dir[
    "RELEASE-NOTES", "README.md", "COPYING",
    "lib/**/*.rb",
    "ext/**/*.{rb,c,h}",
    "ext/**/Makefile",
    "bench/*.rb",
    "examples/*.{rb,txt}",
    "test/*.rb"
  ]
  s.extensions = Dir["ext/**/extconf.rb"]
  s.test_files = Dir["test/*.rb"]
  s.homepage = "https://github.com/vjoel/isaac"
  s.rdoc_options = ["--quiet", "--line-numbers", "--inline-source", "--title", "ISAAC", "--main", "README.md"]
  s.require_paths = ["lib", "ext"]
  s.summary = "ISAAC pseudorandom number generator"
end
