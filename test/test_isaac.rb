require 'minitest/autorun'
require 'isaac'

include PRNG

class TestIsaac < Minitest::Test
  SEED = (0..255).map {rand(2**32-1)}
    # note that minitest supplies the seed to ruby's Kernel#rand
    # use '--seed N' to select a particular seed

  SEQ_1_2_3 = {
    ISAAC4 => [
      200253249, 3829869937, 4163837080, 3505347059, 4075830238,
      2736331801, 3164829547, 410071848, 1780052116, 1192885451],
    ISAAC => [
      2356990817, 1396826555, 488165990, 3792500048, 3350490136,
      3216315542, 2855747110, 2938836516, 2909213788, 1662784833]
   }
  
  CASES = [ISAAC4, ISAAC]

  def setup
    @rs = CASES.map {|cl| r=cl.new; r.srand SEED; r}
  end
  
  def test_ranges
    @rs.each do |r|
      range = 0..1
      range32 = 0..2**32-1

      10.times do
        assert_includes range, r.rand, r.class
        assert_includes range32, r.rand32, r.class
      end
    end
  end  
  
  def test_repeatable
    @rs.each do |r|
      r2 = r.class.new
      r2.srand SEED
      10.times do
        assert_equal(r.rand32, r2.rand32, r.class)
      end
    end
  end
  
  def test_predictable
    @rs.each do |r|
      r.srand [1,2,3]
      assert_equal(SEQ_1_2_3[r.class], (0..9).map {r.rand32}, r.class)
    end
  end
  
  def test_marshal
    @rs.each do |r|
      r2 = Marshal.load(Marshal.dump(r))
      10.times do
        assert_equal(r.rand32, r2.rand32, r.class)
      end
    end
  end
end
