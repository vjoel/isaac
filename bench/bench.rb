require 'benchmark'
require '../ext/isaac/isaac'

srand(1) # use ruby's prng to seed isaac
ary = (0..255).map {rand(2**32-1)}
r = ISAAC.new
r.srand(ary)

times = Benchmark.measure do
  1_000_000.times do
    r.rand32
  end
end

puts times

__END__

Times to generate 1_000_000 numbers:

With 256 entry state vectors (crypto):
  1.860000   0.200000   2.060000 (  2.185345)
  
With 16 entry state vectors (ok for simulation):
  1.900000   0.200000   2.100000 (  2.224832)
  
