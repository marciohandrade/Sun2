require 'ftools'

class SvnClient
  attr_accessor :username, :password, :options, :quiet
  def initialize
    @username = ''
    @password = ''
    @options = ''
    @quiet = false
  end
  def checkout(url, path)
    puts "Checking out \"#{url}\" to \"#{full(path)}\"..." unless quiet
    cmd = "svn co \"#{url}\" \"#{path}\" #{@options} #{global_options()}"
    run(cmd)
  end
  def update(path)
    puts "Updating working copy on \"#{full(path)}\"..." unless quiet
    cmd = "svn up \"#{path}\" #{@options} #{global_options()}"
    run(cmd)
  end
  def commit(path, message)
    puts "Committing \"#{full(path)}\"..." unless quiet
    cmd = "svn ci \"#{path}\" #{@options} -m \"#{message}\" #{global_options()}"
    run(cmd)
  end
  def switch(url, path)
    puts "Switching to \"#{url}\" on \"#{full(path)}\"..." unless quiet
    cmd = "svn sw \"#{url}\" \"#{path}\" #{@options} #{global_options()}"
    run(cmd)
  end
  def revert(path)
    puts "Reverting \"#{full(path)}\"..." unless quiet
    cmd = "svn revert \"#{path}\" #{@options} #{global_options()}"
    run(cmd)
  end
  def export(url, path)
    puts "Exporting \"#{url}\" to \"#{full(path)}\"..." unless quiet
    cmd = "svn export \"#{url}\" \"#{path}\" #{@options} #{global_options()}"
    run(cmd)
  end
  def copy(src, dst, message)
    puts "Copying \"#{src}\" to \"#{dst}\"..." unless quiet
    cmd = "svn cp \"#{src}\" \"#{dst}\" #{@options} -m \"#{message}\" #{global_options()}"
    run(cmd)
  end
  def check_for_modifications(path)
    puts "Checking \"#{full(path)}\" for local modifications..." unless quiet
    result = `svn stat #{path} #{global_options}`
    result.include?('M ') ? true : false
  end
  def get_rev_num(path)
    result = `svn info #{path} #{global_options}`
    key_str = 'Last Changed Rev: '
    from = result.index(key_str) + key_str.length
    to = result.index("\n", from)
    result.slice(from, to - from)
  end
  def last_error()
    @error ? @error : -1
  end
 private
  def run(cmd)
    cmd.concat(' >> nul') if quiet
    result = system(cmd)
    @error = $?
    return result
  end
  def global_options()
    "--username #{@username} --password #{@password} --no-auth-cache --non-interactive"
  end
  def full(path)
    File.expand_path(path)
  end
end
