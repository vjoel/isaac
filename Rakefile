require 'rake'
require 'rake/testtask'

PRJ = "prng-isaac"

def version
  @version or abort "this task should => :version"
end

def tag
  @tag ||= "#{PRJ}-#{version}"
end

desc "Build extension"
task :ext => "ext/isaac/isaac.so"

file "ext/isaac/isaac.so" =>
       FileList["ext/isaac/*.{c,h}", "ext/isaac/Makefile"] do
  sh "cd ext/isaac && make"
end

file "ext/isaac/Makefile" => "ext/isaac/extconf.rb" do
  sh "cd ext/isaac && ruby extconf.rb"
end

desc "clean compiled files"
task :clean do
  sh "cd ext/isaac && make clean"
end

desc "clean compiled files and Makefile"
task :dist_clean => :clean do
  sh "cd ext/isaac && rm Makefile"
end

desc "Run tests"
Rake::TestTask.new :test => :ext do |t|
  t.libs << "lib"
  t.libs << "ext"
  t.test_files = FileList["test/**/*.rb"]
end

desc "Commit, tag, and push repo; build and push gem"
task :release => ["release:is_new_version", :test] do
  require 'tempfile'
  
  sh "gem build #{PRJ}.gemspec"

  file = Tempfile.new "template"
  begin
    file.puts "release #{version}"
    file.close
    sh "git commit --allow-empty -a -v -t #{file.path}"
  ensure
    file.close unless file.closed?
    file.unlink
  end

  sh "git tag #{tag}"
  sh "git push"
  sh "git push --tags"
  
  sh "gem push #{tag}.gem"
end

namespace :release do
  desc "Diff to latest release"
  task :diff do
    latest = `git describe --abbrev=0 --tags --match '#{PRJ}-*'`.chomp
    sh "git diff #{latest}"
  end

  desc "Log to latest release"
  task :log do
    latest = `git describe --abbrev=0 --tags --match '#{PRJ}-*'`.chomp
    sh "git log #{latest}.."
  end

  task :is_new_version => :version do
    abort "#{tag} exists; update version!" unless `git tag -l #{tag}`.empty?
  end

  task :version => :ext do
    require 'isaac'
    @version = PRNG::ISAAC::VERSION
    warn "VERSION not a string" unless @version.kind_of? String
  end
end
