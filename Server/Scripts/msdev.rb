class DevEnv
  attr_accessor :solution, :solution_config, :project, :quiet, :log_file
  def initialize
    @solution = ''
    @solution_config = ''
    @project = ''
    @quiet = false
    @log_file = 'build_log.txt'
  end
  def build(rebuild)
    action = rebuild ? 'rebuild' : 'build'
    action_str = rebuild ? 'Rebuilding' : 'Building'
    project_str = @project != '' ? "project \"#{@project}\" in " : ''
    project_opt = @project != '' ? " /project \"#{@project}\"" : ''
    puts "#{action_str} #{project_str}\"#{solution}\" for \"#{solution_config}\"..." unless quiet
    cmd = "devenv.exe /#{action} \"#{@solution_config}\"#{project_opt} \"#{@solution}\" /out #{@log_file}"
    run(cmd)
  end
  def last_error()
    @error ? @error : -1
  end
  def clear_build_log()
    begin
      File.delete(@log_file) if File.exist?(@log_file)
    rescue
    end
  end
 private
  def run(cmd)
    result = system(cmd)
    @error = $?
    return result
  end
end