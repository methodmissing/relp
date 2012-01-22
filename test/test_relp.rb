# encoding: utf-8

require File.join(File.dirname(__FILE__), 'helper')

class TestRelp < RelpTestCase
  def test_version
    assert_match(/1\.0\.0/, Relp.version)
  end

  def test_error
    assert_equal "out of memory occured", Relp.error(Relp::OUT_OF_MEMORY)
  end
end