require 'minitest/autorun'
require 'isaac'

class TestIsaac < MiniTest::Unit::TestCase
  SEED = (0..255).map {rand(2**32-1)}
    # note that minitest supplies the seed to ruby's Kernel#rand
    # use '--seed N' to select a particular seed

  def setup
    @r = ISAAC.new
    @r.srand SEED
  end
  
  def test_ranges
    range = 0..1
    range32 = 0..2**32-1
    
    10.times do
      assert_includes range, @r.rand
      assert_includes range32, @r.rand32
    end
  end  
  
  def test_repeatable
    r2 = ISAAC.new
    r2.srand SEED
    10.times do
      assert_equal(@r.rand32, r2.rand32)
    end
  end
  
  SEQ_1_2_3 = [
    2356990817, 1396826555, 488165990, 3792500048, 3350490136,
    3216315542, 2855747110, 2938836516, 2909213788, 1662784833]
  
  def test_predictable
    @r.srand [1,2,3]
    assert_equal(SEQ_1_2_3, (0..9).map {@r.rand32})
  end
  
  def test_marshal
    r2 = Marshal.load(Marshal.dump(@r))
    10.times do
      assert_equal(@r.rand32, r2.rand32)
    end
  end
end
