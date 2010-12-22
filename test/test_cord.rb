class TestCord < Test::Unit::TestCase
  def test_append
    c = Cord.new
    assert_equal 'string', (c << 'string').to_s
    oc = Cord.new('test')
    assert_equal 'stringtest', (c << oc).to_s
  end

  def test_to_s_empty_cord
    c = Cord.new
    assert_equal '', c.to_s
  end

  def test_append_non_string
    c = Cord.new
    assert_raises(TypeError){ c << :symbol }
  end

  def test_slice
    c = Cord.new('test')
    assert_equal 'te', c.slice(0..1)
    assert_equal 'es', c.slice(1,2)
  end

  def test_length
    c = Cord.new
    assert_equal 0, c.length
    c << 'cord'
    assert_equal 4, c.length
    c << 'test'
    assert_equal 8, c.length
  end

  def test_depth
    c = Cord.new
    assert_equal 0, c.depth
    c << 'aaaa'
    assert_equal 1, c.depth
    c << 'bbbbbbbbbbbbbbbbbbbbbbbbbbb'
    assert_equal 2, c.depth
    c << 'ccccccccccccccccccccccccccc'
    assert_equal 3, c.depth
  end

  def test_initialize
    c = Cord.new
    assert_instance_of Cord, c
    assert_equal '', c.to_s
    c = Cord.new('test')
    assert_equal 'test', c.to_s
  end

  def test_to_s
    c = Cord.new
    s = 'aaaa'
    c << s
    assert_equal 'aaaa', c.to_s
    c << 'bbbb'
    assert_equal 'aaaabbbb', c.to_s
    c << ''
    assert_equal 'aaaabbbb', c.to_s
    c << 'cccccccccccccccccccccccccccccccccccc'
    assert_equal "aaaabbbbcccccccccccccccccccccccccccccccccccc", c.to_s
    c << 'ddddddddddddddddddddddddddddddddddddddd'
    assert_equal 'aaaabbbbccccccccccccccccccccccccccccccccccccddddddddddddddddddddddddddddddddddddddd', c.to_s
    c << 'eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee'
    assert_equal 'aaaabbbbccccccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddddddddddeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee', c.to_s
    c << 'ffff'
    assert_equal 'aaaabbbbccccccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddddddddddeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeffff', c.to_s
  end
end