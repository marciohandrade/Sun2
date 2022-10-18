class P4Client
  attr_accessor :bin_path, :quiet
  def initialize
    @bin_path = 'C:\Program Files\Perforce'
    @quiet = false
  end
  def sync(path)
    cmd = "#{@bin_path}\\p4.exe sync \"#{path}\""
    run(cmd)
  end
  def force_sync(path)
    cmd = "#{@bin_path}\\p4.exe sync -f \"#{path}\""
    run(cmd)
  end
  def last_error()
    @error ? @error : -1
  end
 private
  def run(cmd)
    cmd.concat(' >> nul') if quiet
    result = system(cmd)
    @error = $?
    raise last_error if result == false or @error != 0
  end
end
